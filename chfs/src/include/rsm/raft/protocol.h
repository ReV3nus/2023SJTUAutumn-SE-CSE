#pragma once

#include "rsm/raft/log.h"
#include "rpc/msgpack.hpp"

namespace chfs {

const std::string RAFT_RPC_START_NODE = "start node";
const std::string RAFT_RPC_STOP_NODE = "stop node";
const std::string RAFT_RPC_NEW_COMMEND = "new commend";
const std::string RAFT_RPC_CHECK_LEADER = "check leader";
const std::string RAFT_RPC_IS_STOPPED = "check stopped";
const std::string RAFT_RPC_SAVE_SNAPSHOT = "save snapshot";
const std::string RAFT_RPC_GET_SNAPSHOT = "get snapshot";

const std::string RAFT_RPC_REQUEST_VOTE = "request vote";
const std::string RAFT_RPC_APPEND_ENTRY = "append entries";
const std::string RAFT_RPC_INSTALL_SNAPSHOT = "install snapshot";

struct RequestVoteArgs {
    /* Lab3: Your code here */
    int term;
    int candidateId;
    int lastLogIndex;
    int lastLogTerm;
    MSGPACK_DEFINE(
        term,
        candidateId,
        lastLogIndex,
        lastLogTerm
    )
};

struct RequestVoteReply {
    /* Lab3: Your code here */
    int term;
    bool voteGranted;
    MSGPACK_DEFINE(
        term,
        voteGranted
    )
};
template<typename Command>
class RaftLogEntry {
public:
    int index;
    int term;
    Command cmd;

    RaftLogEntry(int index = 0, int term = 0) : index(index), term(term) {}
    RaftLogEntry(int index, int term, Command cmd) : index(index), term(term), cmd(cmd) {}
};

template <typename Command>
struct AppendEntriesArgs {
    /* Lab3: Your code here */

    int term;
    int leaderId;
    int prevLogIndex;
    int prevLogTerm;
    std::vector<RaftLogEntry<Command>> entries;
    int leaderCommit;

    MSGPACK_DEFINE(
        term,
        leaderId,
        prevLogIndex,
        prevLogTerm,
        entries,
        leaderCommit
    )
};

struct RpcAppendEntriesArgs {
    /* Lab3: Your code here */

    int term;
    int leaderId;
    int prevLogIndex;
    int prevLogTerm;
    std::vector<std::vector<u8>> cmds;
    std::vector<int>indexs;
    std::vector<int>terms;
    int leaderCommit;

    MSGPACK_DEFINE(
        term,
        leaderId,
        prevLogIndex,
        prevLogTerm,
        cmds,
        indexs,
        terms,
        leaderCommit
    )
};

template <typename Command>
RpcAppendEntriesArgs transform_append_entries_args(const AppendEntriesArgs<Command> &arg)
{
    /* Lab3: Your code here */
    RpcAppendEntriesArgs rpc_arg;
    rpc_arg.term = arg.term;
    rpc_arg.leaderId = arg.leaderId;
    rpc_arg.prevLogIndex = arg.prevLogIndex;
    rpc_arg.prevLogTerm = arg.prevLogTerm;
    rpc_arg.leaderCommit = arg.leaderCommit;
    for(int i = 0; i < arg.entries.size(); i++){
        rpc_arg.indexs.push_back(arg.entries[i].index);
        rpc_arg.terms.push_back(arg.entries[i].term);
        rpc_arg.cmds.push_back(arg.entries[i].cmd.serialize(arg.entries[i].cmd.size()));
    }

    return rpc_arg;
}

template <typename Command>
AppendEntriesArgs<Command> transform_rpc_append_entries_args(const RpcAppendEntriesArgs &rpc_arg)
{
    /* Lab3: Your code here */
    AppendEntriesArgs<Command> arg;
    arg.term = rpc_arg.term;
    arg.leaderId = rpc_arg.leaderId;
    arg.prevLogIndex = rpc_arg.prevLogIndex;
    arg.prevLogTerm = rpc_arg.prevLogTerm;
    arg.leaderCommit = rpc_arg.leaderCommit;
    arg.entries.clear();
    for(int i = 0; i < rpc_arg.cmds.size(); i++){
        arg.entries.push_back(RaftLogEntry<Command>(rpc_arg.indexs[i], rpc_arg.terms[i], Command()));
        arg.entries[i].cmd.deserialize(rpc_arg.cmds[i],rpc_arg.cmds[i].size());
    }

    return arg;
}

struct AppendEntriesReply {
    /* Lab3: Your code here */
    int term;
    bool success;

    MSGPACK_DEFINE(
        term,
        success
    )
};

struct InstallSnapshotArgs {
    /* Lab3: Your code here */
    int term;
    int leaderId;
    int lastIncludedIndex;
    int lastIncludedTerm;
    std::vector<uint_least8_t> snapshot;

    MSGPACK_DEFINE(
        term,
        leaderId,
        lastIncludedIndex,
        lastIncludedTerm,
        snapshot
    )
};

struct InstallSnapshotReply {
    /* Lab3: Your code here */
    int term;

    MSGPACK_DEFINE(
        term
    )
};

} /* namespace chfs */