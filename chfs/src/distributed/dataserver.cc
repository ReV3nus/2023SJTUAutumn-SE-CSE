#include "distributed/dataserver.h"
#include "common/util.h"

namespace chfs {

auto DataServer::initialize(std::string const &data_path) {
  /**
   * At first check whether the file exists or not.
   * If so, which means the distributed chfs has
   * already been initialized and can be rebuilt from
   * existing data.
   */
  bool is_initialized = is_file_exist(data_path);

  auto bm = std::shared_ptr<BlockManager>(
      new BlockManager(data_path, KDefaultBlockCnt));
  if (is_initialized) {
    block_allocator_ =
        std::make_shared<BlockAllocator>(bm, 0, false);
  } else {
    // We need to reserve some blocks for storing the version of each block
    block_allocator_ = std::shared_ptr<BlockAllocator>(
        new BlockAllocator(bm, 0, true));
  }

  // Initialize the RPC server and bind all handlers
  server_->bind("read_data", [this](block_id_t block_id, usize offset,
                                    usize len, version_t version) {
    return this->read_data(block_id, offset, len, version);
  });
  server_->bind("write_data", [this](block_id_t block_id, usize offset,
                                     std::vector<u8> &buffer) {
    return this->write_data(block_id, offset, buffer);
  });
  server_->bind("alloc_block", [this]() { return this->alloc_block(); });
  server_->bind("free_block", [this](block_id_t block_id) {
    return this->free_block(block_id);
  });

  // Launch the rpc server to listen for requests
  server_->run(true, num_worker_threads);
}

DataServer::DataServer(u16 port, const std::string &data_path)
    : server_(std::make_unique<RpcServer>(port)) {
  initialize(data_path);
}

DataServer::DataServer(std::string const &address, u16 port,
                       const std::string &data_path)
    : server_(std::make_unique<RpcServer>(address, port)) {
  initialize(data_path);
}

DataServer::~DataServer() { server_.reset(); }

// {Your code here}
auto DataServer::read_data(block_id_t block_id, usize offset, usize len,
                           version_t version) -> std::vector<u8> {
  // TODO: Implement this function.
  // UNIMPLEMENTED();

  auto buffer = std::vector<u8>(block_allocator_->bm->block_size(), 0);
  auto errret = std::vector<u8>(0);
  if (block_id >= KDefaultBlockCnt|| block_id < 0)
    return errret;
  
  auto bm = block_allocator_->bm;
  std::vector<u8> map_buffer(bm->block_size());
  bm->read_block(0, map_buffer.data());
  if(!(map_buffer[block_id]&1))
    return errret;
  auto inline_version = map_buffer[block_id];
  if(inline_version != version)
    return errret;

  auto read_res = bm->read_block(block_id, buffer.data());
  if (read_res.is_err()) {
    return errret;
  }


  auto ret = std::vector<u8>(len);
  memcpy(ret.data(), buffer.data()+ offset, len);

  return ret;
}

// {Your code here}
auto DataServer::write_data(block_id_t block_id, usize offset,
                            std::vector<u8> &buffer) -> bool {
  // TODO: Implement this function.
  // UNIMPLEMENTED();
  if(block_id >= KDefaultBlockCnt || block_id <= 0 || offset + buffer.size()>4096)return false;

  auto bm = block_allocator_->bm;
  std::vector<u8> map_buffer(bm->block_size());
  bm->read_block(0, map_buffer.data());

  if(!(map_buffer[block_id]&1))return false;

  auto write_res = block_allocator_->bm->write_partial_block(block_id, buffer.data(), offset, buffer.size());
  if(write_res.is_err())return false;

  return true;
}

// {Your code here}
auto DataServer::alloc_block() -> std::pair<block_id_t, version_t> {
  // TODO: Implement this function.
  // UNIMPLEMENTED();
  // auto allocate_res = this->block_allocator_->allocate();
  // if(allocate_res.is_err())
  // {
  //   return {};
  // }
  // block_id_t allocate_id = allocate_res.unwrap();
  // return std::make_pair(allocate_id, 0);
  auto bm=block_allocator_->bm;
  std::vector<u8> buffer(bm->block_size());
  bm->read_block(0, buffer.data());
//   if(!(buffer[0]>=0))
//   {
// std::cout<<"empty buffer!"<<std::endl;
//     for(int i=0;i<buffer.size();i++)buffer[i]=0;
//   }

  block_id_t res = 0;
  version_t res_v = 0;
  for(int i=1;i<KDefaultBlockCnt;i++)
  {
    if(buffer[i]&1)continue;
    buffer[i]++;
    res_v = buffer[i];
    res = i;
    break;
  }
  if(!res){
    return std::make_pair(res,res_v);
  }

  bm->write_block(0,buffer.data());
  std::vector<u8> zero_buffer(bm->block_size());
  memset(zero_buffer.data(),0,zero_buffer.size());
  bm->write_block(res,zero_buffer.data());

  return std::make_pair(res, res_v); 
}

// {Your code here}
auto DataServer::free_block(block_id_t block_id) -> bool {
  // TODO: Implement this function.
  // UNIMPLEMENTED();
  if(block_id <= 0 || block_id > KDefaultBlockCnt)return false;

  auto bm = block_allocator_->bm;
  std::vector<u8> buffer(bm->block_size());
  bm->read_block(0, buffer.data());

  if(!(buffer[block_id]&1))
  {
    return false;
  }
  buffer[block_id]++;
  bm->write_block(0, buffer.data());
  return true;
}
} // namespace chfs