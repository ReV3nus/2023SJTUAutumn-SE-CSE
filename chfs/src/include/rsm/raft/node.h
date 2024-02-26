#pragma once

#include <atomic>
#include <mutex>
#include <chrono>
#include <thread>
#include <ctime>
#include <algorithm>
#include <thread>
#include <memory>
#include <stdarg.h>
#include <unistd.h>
#include <filesystem>
#include <vector>

#include "rsm/state_machine.h"
#include "rsm/raft/log.h"
#include "rsm/raft/protocol.h"
#include "utils/thread_pool.h"
#include "librpc/server.h"
#include "librpc/client.h"
#include "block/manager.h"

namespace chfs {

enum class RaftRole {
    Follower,
    Candidate,
    Leader
};

struct RaftNodeConfig {
    int node_id;
    uint16_t port;
    std::string ip_address;
};


template <typename StateMachine, typename Command>
class RaftNode {

#define RAFT_LOG(fmt, args...)                                                                                   \
    do {                                                                                                         \
        auto now =                                                                                               \
            std::chrono::duration_cast<std::chrono::milliseconds>(                                               \
                std::chrono::system_clock::now().time_since_epoch())                                             \
                .count();                                                                                        \
        char buf[512];                                                                                      \
        sprintf(buf,"[%ld][%s:%d][node %d term %d role %d] " fmt "\n", now, __FILE__, __LINE__, my_id, current_term, role, ##args); \
        thread_pool->enqueue([=]() { std::cerr << buf;} );                                         \
    } while (0);

public:
    RaftNode (int node_id, std::vector<RaftNodeConfig> node_configs);
    ~RaftNode();

    /* interfaces for test */
    void set_network(std::map<int, bool> &network_availablility);
    void set_reliable(bool flag);
    int get_list_state_log_num();
    int rpc_count();
    std::vector<u8> get_snapshot_direct();

private:
    /* 
     * Start the raft node.
     * Please make sure all of the rpc request handlers have been registered before this method.
     */
    auto start() -> int;

    /*
     * Stop the raft node.
     */
    auto stop() -> int;
    
    /* Returns whether this node is the leader, you should also return the current term. */
    auto is_leader() -> std::tuple<bool, int>;

    /* Checks whether the node is stopped */
    auto is_stopped() -> bool;

    /* 
     * Send a new command to the raft nodes.
     * The returned tuple of the method contains three values:
     * 1. bool:  True if this raft node is the leader that successfully appends the log,
     *      false If this node is not the leader.
     * 2. int: Current term.
     * 3. int: Log index.
     */
    auto new_command(std::vector<u8> cmd_data, int cmd_size) -> std::tuple<bool, int, int>;

    /* Save a snapshot of the state machine and compact the log. */
    auto save_snapshot() -> bool;

    /* Get a snapshot of the state machine */
    auto get_snapshot() -> std::vector<u8>;


    /* Internal RPC handlers */
    auto request_vote(RequestVoteArgs arg) -> RequestVoteReply;
    auto append_entries(RpcAppendEntriesArgs arg) -> AppendEntriesReply;
    auto install_snapshot(InstallSnapshotArgs arg) -> InstallSnapshotReply;

    /* RPC helpers */
    void send_request_vote(int target, RequestVoteArgs arg);
    void handle_request_vote_reply(int target, const RequestVoteArgs arg, const RequestVoteReply reply);

    void send_append_entries(int target, AppendEntriesArgs<Command> arg);
    void handle_append_entries_reply(int target, const AppendEntriesArgs<Command> arg, const AppendEntriesReply reply);

    void send_install_snapshot(int target, InstallSnapshotArgs arg);
    void handle_install_snapshot_reply(int target, const InstallSnapshotArgs arg, const InstallSnapshotReply reply);

    /* background workers */
    void run_background_ping();
    void run_background_election();
    void run_background_commit();
    void run_background_apply();


    /* Data structures */
    bool network_stat;          /* for test */

    std::mutex mtx;                             /* A big lock to protect the whole data structure. */
    std::mutex clients_mtx;                     /* A lock to protect RpcClient pointers */
    std::unique_ptr<ThreadPool> thread_pool;
    std::unique_ptr<RaftLog<Command>> log_storage;     /* To persist the raft log. */
    std::unique_ptr<StateMachine> state;  /*  The state machine that applies the raft log, e.g. a kv store. */

    std::unique_ptr<RpcServer> rpc_server;      /* RPC server to recieve and handle the RPC requests. */
    std::map<int, std::unique_ptr<RpcClient>> rpc_clients_map;  /* RPC clients of all raft nodes including this node. */
    std::vector<RaftNodeConfig> node_configs;   /* Configuration for all nodes */ 
    int my_id;                                  /* The index of this node in rpc_clients, start from 0. */

    std::atomic_bool stopped;

    RaftRole role;
    int current_term;
    int leader_id = -1;

    std::unique_ptr<std::thread> background_election;
    std::unique_ptr<std::thread> background_ping;
    std::unique_ptr<std::thread> background_commit;
    std::unique_ptr<std::thread> background_apply;

    /* Lab3: Your code here */
    int votedFor = -1;
    int commitIndex;
    int lastApplied;

    int votesGot = 0;
    std::vector<bool> votesFrom;

    std::condition_variable cv_election;
    std::chrono::system_clock::time_point lastRPC;
    std::chrono::system_clock::duration electionTimeout;
    std::chrono::system_clock::duration candidateTimeout;

    std::vector<int>nextIndex;
    std::vector<int>matchIndex;
    std::vector<int>matchCount;
    std::vector<RaftLogEntry<Command>>log;

    std::vector<u8>snapshot;
    
    bool lockVerbose = false;
    bool LogVerbose = false;
};

template <typename StateMachine, typename Command>
RaftNode<StateMachine, Command>::RaftNode(int node_id, std::vector<RaftNodeConfig> configs):
    network_stat(true),
    node_configs(configs),
    my_id(node_id),
    stopped(true),
    role(RaftRole::Follower),
    current_term(0),
    leader_id(-1)
{
    std::unique_lock<std::mutex> lock(mtx);

    auto my_config = node_configs[my_id];

    /* launch RPC server */
    rpc_server = std::make_unique<RpcServer>(my_config.ip_address, my_config.port);

    /* Register the RPCs. */
    rpc_server->bind(RAFT_RPC_START_NODE, [this]() { return this->start(); });
    rpc_server->bind(RAFT_RPC_STOP_NODE, [this]() { return this->stop(); });
    rpc_server->bind(RAFT_RPC_CHECK_LEADER, [this]() { return this->is_leader(); });
    rpc_server->bind(RAFT_RPC_IS_STOPPED, [this]() { return this->is_stopped(); });
    rpc_server->bind(RAFT_RPC_NEW_COMMEND, [this](std::vector<u8> data, int cmd_size) { return this->new_command(data, cmd_size); });
    rpc_server->bind(RAFT_RPC_SAVE_SNAPSHOT, [this]() { return this->save_snapshot(); });
    rpc_server->bind(RAFT_RPC_GET_SNAPSHOT, [this]() { return this->get_snapshot(); });

    rpc_server->bind(RAFT_RPC_REQUEST_VOTE, [this](RequestVoteArgs arg) { return this->request_vote(arg); });
    rpc_server->bind(RAFT_RPC_APPEND_ENTRY, [this](RpcAppendEntriesArgs arg) { return this->append_entries(arg); });
    rpc_server->bind(RAFT_RPC_INSTALL_SNAPSHOT, [this](InstallSnapshotArgs arg) { return this->install_snapshot(arg); });

   /* Lab3: Your code here */ 
    thread_pool = std::make_unique<ThreadPool>(32);

    // auto bm = std::shared_ptr<BlockManager>(new BlockManager((usize)4096U, (usize)4096U));
    std::string bm_path = "/tmp/raft_log/node" + std::to_string(my_id);
    // printf("node%d trying to create bm at %s, which exist = %d\n",my_id, bm_path.c_str(),is_file_exist(bm_path));
    auto bm = std::shared_ptr<BlockManager>(new BlockManager(bm_path, KDefaultBlockCnt));

    log_storage = std::make_unique<RaftLog<Command>>(bm);
    log_storage->node_id = my_id;
    state = std::make_unique<StateMachine>();

    // persist states, recover from storage
    if(log_storage->restore()){
        current_term = log_storage->current_term;
        votedFor = log_storage->votedFor;

        log = log_storage->log;
        snapshot = log_storage->snapshot;
    }
    else{
        log.clear();
        log.push_back(RaftLogEntry<Command>(0, 0, Command(0)));
        snapshot.clear();

        votedFor = -1;

        log_storage->current_term = current_term;
        log_storage->votedFor = votedFor;

        log_storage->log = log;
        log_storage->snapshot = snapshot;
        log_storage->saveAll();

    }
    if (!snapshot.empty()) {
        state->apply_snapshot(snapshot);
    }

    commitIndex = log.front().index;
    lastApplied = log.front().index;


    votesFrom.clear();
    votesFrom.assign(configs.size(), false);
    votesGot = 0;
    
    nextIndex.clear(),matchIndex.clear(),matchCount.clear();
    nextIndex.assign(configs.size(), 1);
    matchIndex.assign(configs.size(), 0);

    lastRPC = std::chrono::system_clock::now();
    static std::random_device rd;
    static std::minstd_rand gen(rd());
    static std::uniform_int_distribution<int> electionDis(300, 500);
    static std::uniform_int_distribution<int> candidateDis(800, 1000);
    electionTimeout = std::chrono::duration_cast<std::chrono::system_clock::duration>(std::chrono::milliseconds(electionDis(gen)));
    candidateTimeout = std::chrono::duration_cast<std::chrono::system_clock::duration>(std::chrono::milliseconds(candidateDis(gen)));

    lock.unlock();
    rpc_server->run(true, configs.size()); 

}

template <typename StateMachine, typename Command>
RaftNode<StateMachine, Command>::~RaftNode()
{
    stop();

    thread_pool.reset();
    rpc_server.reset();
    state.reset();
    log_storage.reset();
}

/******************************************************************

                        RPC Interfaces

*******************************************************************/


template <typename StateMachine, typename Command>
auto RaftNode<StateMachine, Command>::start() -> int
{
    /* Lab3: Your code here */
    stopped.store(false);
    std::unique_lock<std::mutex> lock(mtx);

    background_election = std::make_unique<std::thread>(&RaftNode::run_background_election, this);
    background_ping = std::make_unique<std::thread>(&RaftNode::run_background_ping, this);
    background_commit = std::make_unique<std::thread>(&RaftNode::run_background_commit, this);
    background_apply = std::make_unique<std::thread>(&RaftNode::run_background_apply, this);

    // role = RaftRole::Follower;
    // current_term = 0;
    // leader_id = -1;
    // votedFor = -1;
    // commitIndex = 0;
    // lastApplied = 0;
    // receivedBeat = false;
    // votesGot = 0;

    // nextIndex.clear();
    // matchIndex.clear();
    // log.clear();
    // log.push_back(std::make_pair({},0));
    for(int i = 0; i < node_configs.size(); i++){
        auto cli = std::make_unique<RpcClient>(node_configs[i].ip_address, node_configs[i].port, true);
        // rpc_clients_map[configs[i].node_id] = cli;
        rpc_clients_map.insert(std::make_pair(node_configs[i].node_id, std::move(cli)));
    }
    lock.unlock();
    return 0;
}

template <typename StateMachine, typename Command>
auto RaftNode<StateMachine, Command>::stop() -> int
{
    /* Lab3: Your code here */
    std::unique_lock<std::mutex> lock(mtx);
    stopped.store(true);
    background_election->join();
    background_ping->join();
    background_commit->join();
    background_apply->join();
    for(int i=0;i<node_configs.size();i++)
        if(rpc_clients_map[i]){
            rpc_clients_map[i].reset();
        }
    lock.unlock();
    return 0;
}

template <typename StateMachine, typename Command>
auto RaftNode<StateMachine, Command>::is_leader() -> std::tuple<bool, int>
{
    /* Lab3: Your code here */
    // RAFT_LOG("IS LEADER RETURN %d",role==RaftRole::Leader);
    if(role == RaftRole::Leader)
        return std::make_tuple(true, current_term);
    return std::make_tuple(false, current_term);
}

template <typename StateMachine, typename Command>
auto RaftNode<StateMachine, Command>::is_stopped() -> bool
{
    return stopped.load();
}

template <typename StateMachine, typename Command>
auto RaftNode<StateMachine, Command>::new_command(std::vector<u8> cmd_data, int cmd_size) -> std::tuple<bool, int, int>
{
    /* Lab3: Your code here */
    if(role != RaftRole::Leader){
        return std::make_tuple(false, current_term, log.back().index);
    }

    std::unique_lock<std::mutex> lock(mtx);
    if(lockVerbose)RAFT_LOG("acquire log in new_command");
    int nxtIndex = log.back().index + 1;
    Command cmd;
    cmd.deserialize(cmd_data, cmd_size);
    log.push_back(RaftLogEntry<Command>(nxtIndex, current_term, cmd));

    nextIndex[my_id] = nxtIndex + 1;
    matchIndex[my_id] = nxtIndex;
    matchCount.push_back(1);

    // log_storage->updateLogs(log);
    log_storage->updateLogsWithRange(log, log.size() - 1, log.size());

    lock.unlock();

    if(LogVerbose)RAFT_LOG("new command term %d index %d",current_term, nxtIndex);
    return std::make_tuple(true, current_term, nxtIndex);
}

template <typename StateMachine, typename Command>
auto RaftNode<StateMachine, Command>::save_snapshot() -> bool
{
    /* Lab3: Your code here */ 
    std::unique_lock<std::mutex> lock(mtx);
    if(lockVerbose)RAFT_LOG("acquire log in save_snapshot");

    snapshot = state->snapshot();


    RaftLogEntry<Command>lastLog;
    if (lastApplied <= log.back().index) {
        lastLog = log[lastApplied - log.front().index];
        log.erase(log.begin(), log.begin() + lastApplied - log.front().index);
    }
    else {
        lastLog = log.back();
        log.clear();
    }
    if(log.empty())log.push_back(lastLog);

    log_storage->updateSnapshot(snapshot);
    log_storage->updateLogs(log);

    lock.unlock();

    if(LogVerbose)RAFT_LOG("snapshot");
    return true;
}

template <typename StateMachine, typename Command>
auto RaftNode<StateMachine, Command>::get_snapshot() -> std::vector<u8>
{
    /* Lab3: Your code here */
    std::vector<u8>ret;
    ret = state->snapshot();
    return ret;
}

/******************************************************************

                         Internal RPC Related

*******************************************************************/


template <typename StateMachine, typename Command>
auto RaftNode<StateMachine, Command>::request_vote(RequestVoteArgs args) -> RequestVoteReply
{
    std::unique_lock<std::mutex> lock(mtx);
    if(lockVerbose)RAFT_LOG("acquire log in request_vote");
    /* Lab3: Your code here */
    RequestVoteReply reply;
    // RAFT_LOG("received request vote from %d when voted for is %d",args.candidateId,votedFor);
    lastRPC = std::chrono::system_clock::now();

    if (args.term < current_term) {
        reply.term = current_term;
        reply.voteGranted = false;
    } else {
        if (args.term > current_term) {
            current_term = args.term;
            role = RaftRole::Follower;
            leader_id = -1;
            votedFor = -1;
        }
        if((votedFor == -1 || votedFor == args.candidateId)&&
                (args.lastLogTerm > log.back().term || 
                (args.lastLogTerm == log.back().term && args.lastLogIndex >= log.back().index))){
            reply.term = current_term;
            reply.voteGranted = true;
            votedFor = args.candidateId;
            cv_election.notify_all();
        }else {
            reply.term = current_term;
            reply.voteGranted = false;
        }
    }

    log_storage->updateMeta(current_term, votedFor);

    lock.unlock();

    return reply;
}

template <typename StateMachine, typename Command>
void RaftNode<StateMachine, Command>::handle_request_vote_reply(int target, const RequestVoteArgs arg, const RequestVoteReply reply)
{
    /* Lab3: Your code here */
    std::unique_lock<std::mutex> lock(mtx);
    if(lockVerbose)RAFT_LOG("acquire log in request_vote_reply");
    // RAFT_LOG("#received vote reply term %d granted %d",reply.term, reply.voteGranted);

    if (reply.term > current_term) {
        current_term = reply.term;
        role = RaftRole::Follower;
        leader_id = -1;
        votedFor = -1;
        log_storage->updateMeta(current_term, votedFor);
    }

    if (role == RaftRole::Candidate) {
        if (reply.voteGranted && !votesFrom[target]) {
            votesGot++;
            votesFrom[target] = true;
            if(votesGot > node_configs.size()/2){
                if(LogVerbose)RAFT_LOG("becoming leader");
                role = RaftRole::Leader;
                leader_id = my_id;

                nextIndex.assign(node_configs.size(), log.back().index+1);
                matchIndex.assign(node_configs.size(), 0);
                matchIndex[my_id] = log.back().index;
                matchCount.assign(log.back().index - commitIndex, 0);
            }
        } else {
            // 如果收到拒绝投票，可能需要更新一些状态以尝试重新发起选举
            // 例如，可能需要将 candidateId 改变，重新增加 term，然后重新发起选举
        }
    }
    lock.unlock();
    return;
}

template <typename StateMachine, typename Command>
auto RaftNode<StateMachine, Command>::append_entries(RpcAppendEntriesArgs rpc_arg) -> AppendEntriesReply
{
    /* Lab3: Your code here */
    std::unique_lock<std::mutex> lock(mtx);
    if(lockVerbose)RAFT_LOG("acquire log in append_entries");
    lastRPC = std::chrono::system_clock::now();
    chfs::AppendEntriesArgs<Command> args = transform_rpc_append_entries_args<Command>(rpc_arg);
    // RAFT_LOG("received append entries from %d",args.leaderId);

    chfs::AppendEntriesReply reply;

    if (args.term < current_term) {
        reply.term = current_term;
        reply.success = false;
        return reply;
    }

    if(args.term > current_term || role == RaftRole::Candidate){
        current_term = args.term;
        role = RaftRole::Follower;
        leader_id = -1;
        votedFor = -1;
        log_storage->updateMeta(current_term, votedFor);
    }

    reply.success = false;
    if(!args.entries.empty()){
        if(LogVerbose)RAFT_LOG("resident log is (index,term)(%d,%d)->(%d,%d), received append entry with (previndex %d prevterm %d) entry size %d"
        ,log.front().index,log.front().term
        ,log.back().index,log.back().term,
        args.prevLogIndex, args.prevLogTerm, (int)args.entries.size());
    }
    if (args.prevLogIndex <= log.back().index && args.prevLogTerm == log[args.prevLogIndex - log.front().index].term) {
        if (!args.entries.empty()) {
            if (args.prevLogIndex < log.back().index) {
                // log_trunc(arg.prevLogIndex + 1);
                log.erase(log.begin() + args.prevLogIndex + 1 - log.front().index, log.end());
                int tmpLogSize = log.size();
                log.insert(log.end(), args.entries.begin(), args.entries.end());
                log_storage->updateLogsWithRange(log, tmpLogSize, log.size());
            }
            else {
                int tmpLogSize = log.size();
                log.insert(log.end(), args.entries.begin(), args.entries.end());
                // if (!storage->append_log(arg.entries, log.size())) {
                //     storage->flush_log(log);
                // }
                log_storage->updateLogsWithRange(log, tmpLogSize, log.size());
            }
        }

        if (args.leaderCommit > commitIndex) {
            commitIndex = std::min(args.leaderCommit, log.back().index);
        }

        reply.success = true;
    }

    reply.term = current_term;
    lock.unlock();
    return reply;
    // leader_id = args.leaderId;

    // if(args.entries.size() == 0){
    //     receivedBeat = true;
    //     leader_id = args.leaderId;
    //     cv_election.notify_all();
    //     reply.term = current_term;
    //     reply.success = true;
    //     return reply;
    // }
    // if (args.prevLogIndex > log.size()-1) {
    //     // 如果日志不包含 prevLogIndex 处的条目或者其任期与 prevLogTerm 不匹配，则回复 false
    //     reply.term = current_term;
    //     reply.success = false;
    //     return reply;
    // }

    // if (args.prevLogIndex > 0) {
    //     auto entry = log[args.prevLogIndex];

    //     if (entry.second() != args.prevLogTerm) {
    //         reply.term = current_term;
    //         reply.success = false;
    //         return reply;
    //     }
    // }

    // // 将新条目附加到日志
    // for (const auto& entry : args.entries) {
    //     log.push_back(std::make_pair(entry, current_term));
    // }

    // // 更新提交索引
    // if (args.leaderCommit > commitIndex) {
    //     commitIndex = std::min(args.leaderCommit, log.size()-1);
    // }

    // reply.term = current_term;
    // reply.success = true;
    // return reply;
}

template <typename StateMachine, typename Command>
void RaftNode<StateMachine, Command>::handle_append_entries_reply(int node_id, const AppendEntriesArgs<Command> arg, const AppendEntriesReply reply)
{
    /* Lab3: Your code here */
    std::unique_lock<std::mutex> lock(mtx);
    if(lockVerbose)RAFT_LOG("acquire log in append_entries_reply");

    if(reply.term > current_term){
        current_term = arg.term;
        role = RaftRole::Follower;
        leader_id = -1;
        votedFor = -1;
        log_storage->updateMeta(current_term, votedFor);
        return;
    }

    if (role == RaftRole::Leader) {
        // if(!arg.entries.empty())RAFT_LOG("received append reply");
        if (reply.success) {
            int tmpIndex = matchIndex[node_id];
            matchIndex[node_id] = std::max(matchIndex[node_id], (int)(arg.prevLogIndex + arg.entries.size()));
            nextIndex[node_id] = matchIndex[node_id]+1;

            tmpIndex = std::max(tmpIndex - commitIndex, 0) - 1;
            for (int i = matchIndex[node_id] - commitIndex - 1; i > tmpIndex; i--) {
                matchCount[i]++;
                if (matchCount[i] > node_configs.size() / 2 && log[commitIndex + i + 1 - log.front().index].term == current_term) {
                    commitIndex += i + 1;
                    matchCount.erase(matchCount.begin(), matchCount.begin() + i + 1);
                    break;
                }
            }
            
            // 如果成功，更新 follower 的 nextIndex 和 matchIndex
            // （注意：你需要为每个 follower 维护 nextIndex 和 matchIndex）
        } else {
            nextIndex[node_id] = std::min(nextIndex[node_id], arg.prevLogIndex);
            // 如果由于日志不一致而导致 AppendEntries 失败，减小 nextIndex 并重试
            // （注意：你需要处理这种情况）
        }
    }
    lock.unlock();
    return;
}


template <typename StateMachine, typename Command>
auto RaftNode<StateMachine, Command>::install_snapshot(InstallSnapshotArgs arg) -> InstallSnapshotReply
{
    /* Lab3: Your code here */
    std::unique_lock<std::mutex> lock(mtx);
    if(lockVerbose)RAFT_LOG("acquire log in install_snapshot");
    lastRPC = std::chrono::system_clock::now();
    InstallSnapshotReply reply;
    reply.term = current_term;

    if (arg.term < current_term) {
        return reply;
    }

    if (arg.term > current_term || role == RaftRole::Candidate) {
        current_term = arg.term;
        role = RaftRole::Follower;
        leader_id = -1;
        votedFor = -1;
        log_storage->updateMeta(current_term, votedFor);
    }

    if (arg.lastIncludedIndex <= log.back().index && arg.lastIncludedTerm == log[arg.lastIncludedIndex - log.front().index].term) {
        if (arg.lastIncludedIndex <= log.back().index) {
            log.erase(log.begin(), log.begin() + arg.lastIncludedIndex - log.front().index);
        }
        else {
            log.clear();
        }
    }
    else {
        log.assign(1, RaftLogEntry<Command>(arg.lastIncludedIndex, arg.lastIncludedTerm));
    }
    snapshot = arg.snapshot;
    state->apply_snapshot(snapshot);

    lastApplied = arg.lastIncludedIndex;
    commitIndex = std::max(commitIndex, arg.lastIncludedIndex);

    log_storage->updateLogs(log);
    log_storage->updateSnapshot(arg.snapshot);

    lock.unlock();
    return reply;
}


template <typename StateMachine, typename Command>
void RaftNode<StateMachine, Command>::handle_install_snapshot_reply(int node_id, const InstallSnapshotArgs arg, const InstallSnapshotReply reply)
{
    /* Lab3: Your code here */
    std::unique_lock<std::mutex> lock(mtx);
    if(lockVerbose)RAFT_LOG("acquire log in handle_install_snapshop_reply");

    if (reply.term > current_term) {
        current_term = reply.term;
        role = RaftRole::Follower;
        leader_id = -1;
        votedFor = -1;
        log_storage->updateMeta(current_term, votedFor);
        return;
    }
    if (role != RaftRole::Leader) {
        return;
    }

    matchIndex[node_id] = std::max(matchIndex[node_id], arg.lastIncludedIndex);
    nextIndex[node_id] = matchIndex[node_id] + 1;

    lock.unlock();
    return;
}

template <typename StateMachine, typename Command>
void RaftNode<StateMachine, Command>::send_request_vote(int target_id, RequestVoteArgs arg)
{
    // if(LogVerbose)RAFT_LOG("trying to send to %d",target_id);
    std::unique_lock<std::mutex> clients_lock(clients_mtx);
    if (rpc_clients_map[target_id] == nullptr
        || rpc_clients_map[target_id]->get_connection_state() != rpc::client::connection_state::connected) {
            if(rpc_clients_map[target_id]){
                // RAFT_LOG("ASKING VOTE FROM %d FAILED BECAUSE STATE IS %d", target_id, rpc_clients_map[target_id]->get_connection_state());
            }
            else {
                // RAFT_LOG("ASKING VOTE FROM %d FAILED BECAUSE ITS EMPTY",target_id);
                auto cli = std::make_unique<RpcClient>(node_configs[target_id].ip_address, node_configs[target_id].port, true);
                rpc_clients_map.insert(std::make_pair(node_configs[target_id].node_id, std::move(cli)));
            }
        return;
    }

    auto res = rpc_clients_map[target_id]->call(RAFT_RPC_REQUEST_VOTE, arg);
    clients_lock.unlock();
    if (res.is_ok()) {
        handle_request_vote_reply(target_id, arg, res.unwrap()->as<RequestVoteReply>());
    } else {
        // RPC fails
        // RAFT_LOG("SENDING FAILURE 2");
    }
}

template <typename StateMachine, typename Command>
void RaftNode<StateMachine, Command>::send_append_entries(int target_id, AppendEntriesArgs<Command> arg)
{
    std::unique_lock<std::mutex> clients_lock(clients_mtx);
    if (rpc_clients_map[target_id] == nullptr 
        || rpc_clients_map[target_id]->get_connection_state() != rpc::client::connection_state::connected) {
            if(rpc_clients_map[target_id]&&arg.entries.empty()){
                // RAFT_LOG("PINGING TO %d FAILED BECAUSE STATE IS %d", target_id, rpc_clients_map[target_id]->get_connection_state());
            }
            else if(rpc_clients_map[target_id]){
                // RAFT_LOG("APPEND ENTRIES TO %d FAILED BECAUSE STATE IS %d", target_id, rpc_clients_map[target_id]->get_connection_state());
                auto cli = std::make_unique<RpcClient>(node_configs[target_id].ip_address, node_configs[target_id].port, true);
                rpc_clients_map.insert(std::make_pair(node_configs[target_id].node_id, std::move(cli)));
            }
        return;
    }

    RpcAppendEntriesArgs rpc_arg = transform_append_entries_args(arg);
    auto res = rpc_clients_map[target_id]->call(RAFT_RPC_APPEND_ENTRY, rpc_arg);
    clients_lock.unlock();
    if (res.is_ok()) {
        handle_append_entries_reply(target_id, arg, res.unwrap()->as<AppendEntriesReply>());
    } else {
        // RPC fails
    }
}

template <typename StateMachine, typename Command>
void RaftNode<StateMachine, Command>::send_install_snapshot(int target_id, InstallSnapshotArgs arg)
{
    std::unique_lock<std::mutex> clients_lock(clients_mtx);
    if (rpc_clients_map[target_id] == nullptr
        || rpc_clients_map[target_id]->get_connection_state() != rpc::client::connection_state::connected) {
        // auto cli = std::make_unique<RpcClient>(node_configs[target_id].ip_address, node_configs[target_id].port, true);
        // rpc_clients_map.insert(std::make_pair(node_configs[target_id].node_id, std::move(cli)));
        return;
    }

    auto res = rpc_clients_map[target_id]->call(RAFT_RPC_INSTALL_SNAPSHOT, arg);
    clients_lock.unlock();
    if (res.is_ok()) { 
        handle_install_snapshot_reply(target_id, arg, res.unwrap()->as<InstallSnapshotReply>());
    } else {
        // RPC fails
    }
}


/******************************************************************

                        Background Workers

*******************************************************************/

template <typename StateMachine, typename Command>
void RaftNode<StateMachine, Command>::run_background_election() {
    // Periodly check the liveness of the leader.

    // Work for followers and candidates.

    /* Uncomment following code when you finish */
    if(lockVerbose)RAFT_LOG("STARTING WITH ELECTION FUNCTION");
    std::unique_lock<std::mutex> lock(mtx, std::defer_lock);
    std::chrono::system_clock::time_point current_time;
    while (true) {
    if(lockVerbose)RAFT_LOG("ENTERING THE ELECTION FUNCTION'S LOOP");
            if (is_stopped()) {
                if(lockVerbose)RAFT_LOG("STOP THE ELECTION FUNCTION");
                return;
            }
            lock.lock();
    if(lockVerbose)RAFT_LOG("acquire lock in \t\trun_background_election");
            current_time = std::chrono::system_clock::now();
            if (role == RaftRole::Follower) {
                // int election_timeout = ;
                // auto cv_status = cv_election.wait_for(lock, std::chrono::milliseconds(election_timeout));

                if (is_stopped()) {
                    return;
                }
                // auto time_t_time = std::chrono::system_clock::to_time_t(lastRPC);
                // RAFT_LOG("last rpc time is %ld",time_t_time);
                if(lockVerbose){
                    
                    auto now1 = std::chrono::duration_cast<std::chrono::milliseconds>(current_time.time_since_epoch());
                    RAFT_LOG("current time %ld", now1.count());
                    auto last1 = std::chrono::duration_cast<std::chrono::milliseconds>(lastRPC.time_since_epoch());
                    RAFT_LOG("lastRPC time %ld", last1.count());
                    auto milisec1 = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - lastRPC);
                    auto milisec2 = std::chrono::duration_cast<std::chrono::milliseconds>(electionTimeout);
                    RAFT_LOG("duration from last RPC is %ld, time out is %ld",milisec1.count(),milisec2.count());
                    
                }

                if(current_time - lastRPC > electionTimeout){
                // if (cv_status == std::cv_status::timeout) {
                    if(LogVerbose)RAFT_LOG("becoming candidate");
                    role = RaftRole::Candidate;
                    current_term += 1;
                    votedFor = my_id;
                    votesGot = 1;
                    votesFrom.assign(node_configs.size(),false);
                    votesFrom[my_id] = true;

                    log_storage->updateMeta(current_term, votedFor);



                    static std::random_device rd;
                    static std::minstd_rand gen(rd());
                    static std::uniform_int_distribution<int> electionDis(300, 500);
                    static std::uniform_int_distribution<int> candidateDis(800, 1000);
                    electionTimeout = std::chrono::duration_cast<std::chrono::system_clock::duration>(std::chrono::milliseconds(electionDis(gen)));
                    candidateTimeout = std::chrono::duration_cast<std::chrono::system_clock::duration>(std::chrono::milliseconds(candidateDis(gen)));

                    leader_id = -1;

                    // RAFT_LOG("electing.");
                    RequestVoteArgs request_args;
                    request_args.term = current_term;
                    request_args.candidateId = my_id;
                    request_args.lastLogIndex = log.back().index;
                    request_args.lastLogTerm = log.back().term;
                    for (int target_id = 0; target_id < node_configs.size(); ++target_id) {
                        if (target_id != my_id) {
                            // RAFT_LOG("asking vote from %d",target_id);
                            // send_request_vote(target_id, request_args);
                            auto result = thread_pool->enqueue(&RaftNode::send_request_vote, this, target_id, request_args); // (function pointer, this pointer, argumen
                        }
                    }
                    lastRPC = std::chrono::system_clock::now();
                }
            }

            else if (role == RaftRole::Candidate) {
                if(current_time - lastRPC > candidateTimeout){
                    if(LogVerbose)RAFT_LOG("becoming next term candidate");
                    role = RaftRole::Candidate;
                    current_term += 1;
                    votedFor = my_id;
                    votesGot = 1;
                    votesFrom.assign(node_configs.size(),false);
                    votesFrom[my_id] = true;

                    log_storage->updateMeta(current_term, votedFor);

                    static std::random_device rd;
                    static std::minstd_rand gen(rd());
                    static std::uniform_int_distribution<int> electionDis(300, 500);
                    static std::uniform_int_distribution<int> candidateDis(800, 1000);
                    electionTimeout = std::chrono::duration_cast<std::chrono::system_clock::duration>(std::chrono::milliseconds(electionDis(gen)));
                    candidateTimeout = std::chrono::duration_cast<std::chrono::system_clock::duration>(std::chrono::milliseconds(candidateDis(gen)));

                    leader_id = -1;

                    RequestVoteArgs request_args;
                    request_args.term = current_term;
                    request_args.candidateId = my_id;
                    request_args.lastLogIndex = log.back().index;
                    request_args.lastLogTerm = log.back().term;
                    for (int target_id = 0; target_id < node_configs.size(); ++target_id) {
                        if (target_id != my_id) {
                            auto result = thread_pool->enqueue(&RaftNode::send_request_vote, this, target_id, request_args); // (function pointer, this pointer, argumen
                        }
                    }
                    lastRPC = std::chrono::system_clock::now();
                }
            }
    if(lockVerbose)RAFT_LOG("going to unlock in \trun_bg_election");
            lock.unlock();

        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
    return;
}

template <typename StateMachine, typename Command>
void RaftNode<StateMachine, Command>::run_background_commit() {
    // Periodly send logs to the follower.

    // Only work for the leader.

    /* Uncomment following code when you finish */
    std::unique_lock<std::mutex> lock(mtx, std::defer_lock);

    while (true) {
        if (is_stopped()) return;

        lock.lock();
    if(lockVerbose)RAFT_LOG("acquire lock in \t\trun_bg_commit");

        if (role == RaftRole::Leader) {
            int last_log_index = log.back().index;
            for (int i = 0; i < node_configs.size(); i++) {
                if (i == my_id) continue;
                if (nextIndex[i] <= last_log_index) {
                    if (nextIndex[i] > log.front().index) {
                        if(LogVerbose)RAFT_LOG("background commit node %d log %d->%d", i, nextIndex[i], last_log_index);
                        AppendEntriesArgs<Command> args;
                        args.term = current_term;
                        args.leaderId = my_id;
                        args.leaderCommit = commitIndex;
                        args.prevLogIndex = nextIndex[i] - 1;
                        args.prevLogTerm = log[args.prevLogIndex - log.front().index].term;
                        args.entries.clear();
                        if(nextIndex[i] < last_log_index + 1){
                            args.entries.assign(log.begin() + nextIndex[i] - log.front().index,
                                        log.begin() + last_log_index + 1 - log.front().index);
                        }
                        thread_pool->enqueue(&RaftNode::send_append_entries, this, i, args); // (function pointer, this pointer, argumen
                    }
                    else {
                        InstallSnapshotArgs args;
                        args.term = current_term;
                        args.leaderId = my_id;
                        args.lastIncludedIndex = log.front().index;
                        args.lastIncludedTerm = log.front().term;
                        args.snapshot = snapshot;
                        thread_pool->enqueue(&RaftNode::send_install_snapshot, this, i, args);
                    }
                }
            }
        }

    if(lockVerbose)RAFT_LOG("going to unlock in \trun_bg_commit");
        lock.unlock();
        
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }   

    return;
}

template <typename StateMachine, typename Command>
void RaftNode<StateMachine, Command>::run_background_apply() {
    // Periodly apply committed logs the state machine

    // Work for all the nodes.

    /* Uncomment following code when you finish */
    std::unique_lock<std::mutex> lock(mtx, std::defer_lock);
    std::vector<RaftLogEntry<Command>> LogsToApply;
    
    while (true) {
        if (is_stopped()) return;
        lock.lock();
    if(lockVerbose)RAFT_LOG("acquire lock in \t\trun_bg_apply");

        if (commitIndex > lastApplied) {
            LogsToApply.clear();
            LogsToApply.assign(log.begin() + lastApplied + 1 - log.front().index,
                                log.begin() + commitIndex + 1 - log.front().index);
            int logCount = 0;
            for (auto & entry : LogsToApply) {
                state->apply_log(entry.cmd);
                logCount++;
            }
            if(LogVerbose)RAFT_LOG("background applied %d->%d with %d entries",lastApplied, commitIndex, logCount);
            lastApplied = commitIndex;
        }

    if(lockVerbose)RAFT_LOG("going to unlock in \trun_bg_apply");
        lock.unlock();
        
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }    
    return;
}

template <typename StateMachine, typename Command>
void RaftNode<StateMachine, Command>::run_background_ping() {
    // Periodly send empty append_entries RPC to the followers.

    // Only work for the leader.

    std::unique_lock<std::mutex> lock(mtx, std::defer_lock);

    while (true) {
        {

            if (is_stopped()) {
                return;
            }
            lock.lock();
    if(lockVerbose)RAFT_LOG("acquire lock in \t\trun_bg_ping");
            if (role == RaftRole::Leader) {
                // RAFT_LOG("pinging");
                AppendEntriesArgs<Command> arg;
                arg.term = current_term;
                arg.leaderId = my_id;
                arg.entries = {}; 
                arg.leaderCommit = commitIndex;

                for (int target_id = 0; target_id < node_configs.size(); ++target_id) {
                    if (target_id != my_id) {
                        // send_append_entries(target_id, arg);
                        // RAFT_LOG("pinging to %d",target_id);
                        arg.prevLogIndex = nextIndex[target_id] - 1;
                        if(arg.prevLogIndex < log.front().index){
                            arg.prevLogIndex = log.front().index;
                        }
                        arg.prevLogTerm = log[arg.prevLogIndex  - log.front().index].term;
                        auto result = thread_pool->enqueue(&RaftNode::send_append_entries, this, target_id, arg); // (function pointer, this pointer, argumen
                    }
                }
            }
    if(lockVerbose)RAFT_LOG("going to unlock in \trun_bg_ping");
            lock.unlock();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100)); 
    }

    return;
}

/******************************************************************

                          Test Functions (must not edit)

*******************************************************************/

template <typename StateMachine, typename Command>
void RaftNode<StateMachine, Command>::set_network(std::map<int, bool> &network_availability)
{
    std::unique_lock<std::mutex> clients_lock(clients_mtx);

    /* turn off network */
    if (!network_availability[my_id]) {
        for (auto &&client: rpc_clients_map) {
            if (client.second != nullptr)
                client.second.reset();
        }

        return;
    }

    for (auto node_network: network_availability) {
        int node_id = node_network.first;
        bool node_status = node_network.second;

        if (node_status && rpc_clients_map[node_id] == nullptr) {
            RaftNodeConfig target_config;
            for (auto config: node_configs) {
                if (config.node_id == node_id) 
                    target_config = config;
            }

            rpc_clients_map[node_id] = std::make_unique<RpcClient>(target_config.ip_address, target_config.port, true);
        }

        if (!node_status && rpc_clients_map[node_id] != nullptr) {
            rpc_clients_map[node_id].reset();
        }
    }
}

template <typename StateMachine, typename Command>
void RaftNode<StateMachine, Command>::set_reliable(bool flag)
{
    std::unique_lock<std::mutex> clients_lock(clients_mtx);
    for (auto &&client: rpc_clients_map) {
        if (client.second) {
            client.second->set_reliable(flag);
        }
    }
}

template <typename StateMachine, typename Command>
int RaftNode<StateMachine, Command>::get_list_state_log_num()
{
    /* only applied to ListStateMachine*/
    std::unique_lock<std::mutex> lock(mtx);

    return state->num_append_logs;
}

template <typename StateMachine, typename Command>
int RaftNode<StateMachine, Command>::rpc_count()
{
    int sum = 0;
    std::unique_lock<std::mutex> clients_lock(clients_mtx);

    for (auto &&client: rpc_clients_map) {
        if (client.second) {
            sum += client.second->count();
        }
    }
    
    return sum;
}

template <typename StateMachine, typename Command>
std::vector<u8> RaftNode<StateMachine, Command>::get_snapshot_direct()
{
    if (is_stopped()) {
        return std::vector<u8>();
    }

    std::unique_lock<std::mutex> lock(mtx);

    return state->snapshot(); 
}

}