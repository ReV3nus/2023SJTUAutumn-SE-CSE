#pragma once

#include "common/macros.h"
#include "block/manager.h"
#include "rsm/raft/protocol.h"
#include <mutex>
#include <vector>
#include <cstring>

namespace chfs {

/** 
 * RaftLog uses a BlockManager to manage the data..
 */
template <typename Command>
class RaftLog {
public:
    RaftLog(std::shared_ptr<BlockManager> bm);
    ~RaftLog();

    /* Lab3: Your code here */

    bool restore();
    void saveMetadata();
    void saveSnapshot();
    void saveLogs();
    void saveLogsWithRange(int startIndex, int endIndex);
    void saveAll();
    void updateMeta(int _current_term, int _votedFor);
    void updateSnapshot(std::vector<u8>_snapshot);
    void updateLogs(std::vector<RaftLogEntry<Command>>_log);
    void updateLogsWithRange(std::vector<RaftLogEntry<Command>>_log, int startIndex, int endIndex);

private:
    std::shared_ptr<BlockManager> bm_;
    std::mutex mtx;
    /* Lab3: Your code here */
public:
    usize MetadataBlockId = 1;
    usize SnapshotBlockId = 2;
    usize LogStartBlockId = 3;

    int hasLogPersisted;

    int current_term;
    int votedFor;

    int LogCount;
    std::vector<RaftLogEntry<Command>>log;

    int snapshot_sz;
    std::vector<u8>snapshot;

    bool verbose = false;
    int node_id;

};

std::vector<int> u8_to_int(std::vector<u8>vec){
    std::vector<int>ret(DiskBlockSize/4);
    ret.clear();
    for(int i = 0; i < vec.size(); i += 4){
        int tmp;
        tmp = (vec[i] & 0xff) << 24;
        tmp |= (vec[i+1] & 0xff) << 16;
        tmp |= (vec[i+2] & 0xff) << 8;
        tmp |= vec[i+3] & 0xff;
        ret.push_back(tmp);
    }
    return ret;
}
std::vector<u8> int_to_u8(std::vector<int>vec){
    std::vector<u8>ret(DiskBlockSize);
    ret.clear();
    for(int i = 0; i < vec.size(); i++){
        int tmp = vec[i];
        ret.push_back((tmp >> 24) & 0xff);
        ret.push_back((tmp >> 16) & 0xff);
        ret.push_back((tmp >> 8) & 0xff);
        ret.push_back(tmp & 0xff);
    }
    return ret;
}



template <typename Command>
RaftLog<Command>::RaftLog(std::shared_ptr<BlockManager> bm)
{
    /* Lab3: Your code here */
    bm_ = bm;
}

template <typename Command>
RaftLog<Command>::~RaftLog()
{
    /* Lab3: Your code here */
}

/* Lab3: Your code here */

template <typename Command>
bool RaftLog<Command>::restore(){
    if(verbose)printf("\nNODE%d:restoring.....\n",node_id);
    std::vector<u8>buffer(DiskBlockSize);
    bm_->read_block(MetadataBlockId, buffer.data());

    std::vector<int>intbuf;
    intbuf = u8_to_int(buffer);

    hasLogPersisted = intbuf[0];

    current_term = intbuf[1];

    votedFor = intbuf[2];

    LogCount = intbuf[3];
    snapshot_sz = intbuf[4];
    if(verbose)printf("NODE%d:found hasPersisted = %d, current_term = %d, votedFor = %d, LogCount = %d, snapshot_sz = %d\n",node_id,hasLogPersisted, current_term, votedFor, LogCount, snapshot_sz);

    if(!hasLogPersisted){
        return false;
    }
    
    if(verbose)printf("NODE%d:find data! recovering.....\n",node_id);

    std::vector<u8>ssbuf(DiskBlockSize);
    bm_->read_block(SnapshotBlockId, ssbuf.data());

    snapshot.clear();
    for(int i = 0; i < snapshot_sz; i++){
        snapshot.push_back(ssbuf[i]);
    }

    if(verbose)printf("NODE%d:found %dBytes Snapshot\n",node_id,(int)snapshot.size());

    log.clear();
    for(int i = 0; i < LogCount; i++){
        std::vector<u8>logbuf(DiskBlockSize);
        bm_->read_block(LogStartBlockId + i, logbuf.data());

        RaftLogEntry<Command>curLog;
        std::vector<int>intlogbuf;
        intlogbuf = u8_to_int(logbuf);
        curLog.index = intlogbuf[1000];
        curLog.term = intlogbuf[1001];
        Command cmd;
        cmd.deserialize(logbuf, cmd.size());
        curLog.cmd = cmd;
        log.push_back(curLog);
        if(verbose)printf("NODE%d:found Log %d with (%d,%d) value = %d\n",node_id,i, log[i].index, log[i].term, log[i].cmd.value);
    }
    if(verbose)printf("NODE%d:found %d Logs\n",node_id,(int)log.size());
    return true;
}

template <typename Command>
void RaftLog<Command>::saveMetadata(){
    hasLogPersisted = true;

    std::vector<int>intbuf(DiskBlockSize/4);
    std::vector<u8>buffer(DiskBlockSize);

    intbuf[0] = hasLogPersisted;
    intbuf[1] = current_term;
    intbuf[2] = votedFor;
    intbuf[3] = LogCount;
    intbuf[4] = snapshot_sz;

    buffer = int_to_u8(intbuf);
    bm_->write_block(MetadataBlockId, buffer.data());

    if(verbose)printf("NODE%d:#Saved current_term = %d, votedFor = %d, LogCount = %d, snapshot_sz = %d\n",node_id,current_term, votedFor,LogCount, snapshot_sz);
    bm_->sync(MetadataBlockId);
    // restore();
}

template <typename Command>
void RaftLog<Command>::saveSnapshot(){
    hasLogPersisted = true;

    std::vector<u8>ssbuf(DiskBlockSize);

    snapshot_sz = snapshot.size();
    if(verbose)printf("NODE%d:#Saved %dBytes Snapshot\n",node_id,snapshot_sz);
    for(int i = 0; i < snapshot_sz; i++){
        ssbuf[i] = snapshot[i];
    }

    bm_->write_block(SnapshotBlockId, ssbuf.data());
    bm_->sync(SnapshotBlockId);
}

template <typename Command>
void RaftLog<Command>::saveLogs(){
    hasLogPersisted = true;

    LogCount = log.size();
    if(verbose)printf("NODE%d:#Saved %d Logs\n",node_id,LogCount);
    for(int i = 0; i < LogCount; i++){
        std::vector<u8>buf(DiskBlockSize);
        std::vector<int>intbuf(DiskBlockSize/4);

        intbuf[1000] = log[i].index;
        intbuf[1001] = log[i].term;
        buf = int_to_u8(intbuf);
        std::vector<u8>desbuf;
        desbuf = log[i].cmd.serialize(log[i].cmd.size());
        for(int j = 0; j < desbuf.size(); j++)
            buf[j] = desbuf[j];
        bm_->write_block(LogStartBlockId + i, buf.data());
        bm_->sync(LogStartBlockId + i);
        if(verbose)printf("NODE%d:#Saved Log %d with (%d,%d) value = %d\n",node_id,i, log[i].index, log[i].term, log[i].cmd.value);
    }
}

template <typename Command>
void RaftLog<Command>::saveLogsWithRange(int startIndex, int endIndex){
    hasLogPersisted = true;

    LogCount = log.size();
    if(endIndex == -1)endIndex = LogCount;
    endIndex = std::min(endIndex, LogCount);
    if(verbose)printf("NODE%d:#Saved %d Logs between %d and %d\n",node_id,LogCount, startIndex, endIndex);
    for(int i = startIndex; i < endIndex; i++){
        std::vector<u8>buf(DiskBlockSize);
        std::vector<int>intbuf(DiskBlockSize/4);

        intbuf[1000] = log[i].index;
        intbuf[1001] = log[i].term;
        buf = int_to_u8(intbuf);
        std::vector<u8>desbuf;
        desbuf = log[i].cmd.serialize(log[i].cmd.size());
        for(int j = 0; j < desbuf.size(); j++)
            buf[j] = desbuf[j];
        bm_->write_block(LogStartBlockId + i, buf.data());
        bm_->sync(LogStartBlockId + i);
        if(verbose)printf("NODE%d:#Saved Log %d with (%d,%d) value = %d\n",node_id,i, log[i].index, log[i].term, log[i].cmd.value);
    }
}

template <typename Command>
void RaftLog<Command>::saveAll(){
    saveSnapshot();
    saveLogs();
    saveMetadata();
}


template <typename Command>
void RaftLog<Command>::updateMeta(int _current_term, int _votedFor){
    current_term = _current_term;
    votedFor = _votedFor;
    saveMetadata();
}

template <typename Command>
void RaftLog<Command>::updateSnapshot(std::vector<u8>_snapshot){
    snapshot = _snapshot;
    saveSnapshot();
    saveMetadata();
}

template <typename Command>
void RaftLog<Command>::updateLogs(std::vector<RaftLogEntry<Command>>_log){
    log = _log;
    saveLogs();
    saveMetadata();
}

template <typename Command>
void RaftLog<Command>::updateLogsWithRange(std::vector<RaftLogEntry<Command>>_log, int startIndex, int endIndex){
    log = _log;
    saveLogsWithRange(startIndex, endIndex);
    saveMetadata();
}
} /* namespace chfs */
