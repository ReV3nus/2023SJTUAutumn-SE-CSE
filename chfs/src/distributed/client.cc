#include "distributed/client.h"
#include "common/macros.h"
#include "common/util.h"
#include "distributed/metadata_server.h"

namespace chfs {

ChfsClient::ChfsClient() : num_data_servers(0) {}

auto ChfsClient::reg_server(ServerType type, const std::string &address,
                            u16 port, bool reliable) -> ChfsNullResult {
  switch (type) {
  case ServerType::DATA_SERVER:
    num_data_servers += 1;
    data_servers_.insert({num_data_servers, std::make_shared<RpcClient>(
                                                address, port, reliable)});
    break;
  case ServerType::METADATA_SERVER:
    metadata_server_ = std::make_shared<RpcClient>(address, port, reliable);
    break;
  default:
    std::cerr << "Unknown Type" << std::endl;
    exit(1);
  }

  return KNullOk;
}

// {Your code here}
auto ChfsClient::mknode(FileType type, inode_id_t parent,
                        const std::string &name) -> ChfsResult<inode_id_t> {
  // TODO: Implement this function.
  // UNIMPLEMENTED();
  auto srv_call = metadata_server_->call("mknode",(int)type,parent,name);
  if(srv_call.is_err())
    return ChfsResult<inode_id_t>(0);
  auto response = srv_call.unwrap()->as<inode_id_t>();
  if(!response)return ChfsResult<inode_id_t>(ErrorType::BadResponse);
  return ChfsResult<inode_id_t>(response);
}

// {Your code here}
auto ChfsClient::unlink(inode_id_t parent, std::string const &name)
    -> ChfsNullResult {
  // TODO: Implement this function.
  // UNIMPLEMENTED();
  auto srv_call = metadata_server_->call("unlink",parent,name);
  if(srv_call.is_err())
    return ChfsNullResult(srv_call.unwrap_error());
  auto response = srv_call.unwrap()->as<bool>();
  if(!response)return ChfsNullResult(ErrorType::BadResponse);
  return KNullOk;
}

// {Your code here}
auto ChfsClient::lookup(inode_id_t parent, const std::string &name)
    -> ChfsResult<inode_id_t> {
  // TODO: Implement this function.
  // UNIMPLEMENTED();
  auto srv_call = metadata_server_->call("lookup",parent,name);
  if(srv_call.is_err())
    return ChfsResult<inode_id_t>(0);
  auto response = srv_call.unwrap()->as<inode_id_t>();
  return ChfsResult<inode_id_t>(response);
}

// {Your code here}
auto ChfsClient::readdir(inode_id_t id)
    -> ChfsResult<std::vector<std::pair<std::string, inode_id_t>>> {
  // TODO: Implement this function.
  // UNIMPLEMENTED();
  auto srv_call = metadata_server_->call("readdir",id);
  if(srv_call.is_err())
    return ChfsResult<std::vector<std::pair<std::string, inode_id_t>>>({});
  auto response = srv_call.unwrap()->as<std::vector<std::pair<std::string, inode_id_t>>>();
  return ChfsResult<std::vector<std::pair<std::string, inode_id_t>>>(response);
}

// {Your code here}
auto ChfsClient::get_type_attr(inode_id_t id)
    -> ChfsResult<std::pair<InodeType, FileAttr>> {
  // TODO: Implement this function.
  // UNIMPLEMENTED();
  auto srv_call = metadata_server_->call("get_type_attr",id);
  if(srv_call.is_err())
    return ChfsResult<std::pair<InodeType, FileAttr>>({});
  auto response = srv_call.unwrap()->as<std::tuple<u64, u64, u64, u64, u8>>();
  InodeType type = (InodeType)std::get<4>(response);
  FileAttr fileAttr;
  fileAttr.atime = std::get<1>(response);
  fileAttr.mtime = std::get<2>(response);
  fileAttr.ctime = std::get<3>(response);
  fileAttr.size = std::get<0>(response);
  return ChfsResult<std::pair<InodeType, FileAttr>>(std::make_pair(type,fileAttr));
}

/**
 * Read and Write operations are more complicated.
 */
// {Your code here}
auto ChfsClient::read_file(inode_id_t id, usize offset, usize size)
    -> ChfsResult<std::vector<u8>> {
  // TODO: Implement this function.
  // UNIMPLEMENTED();
  auto srv_call = metadata_server_->call("get_block_map",id);
  std::vector<BlockInfo> block_map;
  u64 block_cnt = 0;
  std::vector<u8>ret;
  ret.clear();
  u64 cur_index = 0;
  u64 read_sz = 0;
  block_map.clear();
  ret.clear();

  if(srv_call.is_err())
  {
    goto read_fail;
  }
  block_map = srv_call.unwrap()->as<std::vector<BlockInfo>>();
  block_cnt = block_map.size();

// std::cout<<"read from inode "<<id<<std::endl;
// std::cout<<"showing block map:"<<std::endl;
// for(int i=0;i<block_cnt;i++)std::cout<<std::get<0>(block_map[i])<<' '<<std::get<1>(block_map[i])<<' '<<std::get<2>(block_map[i])<<std::endl;

  while(cur_index < block_cnt && read_sz < size)
  {
    if(offset >= DiskBlockSize)
    {
      offset -= DiskBlockSize;
      cur_index++;
      continue;
    }
    block_id_t block_id = std::get<0>(block_map[cur_index]);
    mac_id_t mac_id = std::get<1>(block_map[cur_index]);
    version_t version = std::get<2>(block_map[cur_index]);

    auto read_len = std::min((u32)(DiskBlockSize - offset), (u32)(size - read_sz));
    auto datasrv_call = data_servers_[mac_id]->call("read_data",block_id,offset,read_len,version);

    if(datasrv_call.is_err())
    {
      goto read_fail;
    }
    auto read_resp = datasrv_call.unwrap()->as<std::vector<u8>>(); 
// std::cout<<"read from machine "<<mac_id<<" block "<<block_id<<" with offset "<<offset<<" and length "<<read_resp.size()<<std::endl;
// for(int i=0;i<read_resp.size();i++)std::cout<<read_resp[i]<<' ';
// std::cout<<std::endl;
    if(read_resp.size()!=read_len)
    {
      goto read_fail;
    }

    for(int i=0;i<read_len;i++)
      ret.push_back(read_resp[i]);
    read_sz += read_len;
    offset = 0;
    cur_index++;
  }
  if(read_sz != size)
  {
    goto read_fail;
  }
  return ChfsResult<std::vector<u8>>(ret);
read_fail:
  return ChfsResult<std::vector<u8>>({});
}

// {Your code here}
auto ChfsClient::write_file(inode_id_t id, usize offset, std::vector<u8> data)
    -> ChfsNullResult {
  // TODO: Implement this function.
  // UNIMPLEMENTED();
  auto srv_call = metadata_server_->call("get_block_map",id);
  std::vector<BlockInfo> block_map;
  u64 block_cnt = 0;
  u64 origin_size = 0;
  u64 cur_index = 0;
  u64 write_sz = 0;
  u64 data_sz = data.size();
  u64 write_index = 0;
  block_map.clear();

  if(srv_call.is_err())
  {
    goto write_fail;
  }
  block_map = srv_call.unwrap()->as<std::vector<BlockInfo>>();
  block_cnt = block_map.size();
// std::cout<<"write to inode "<<id<<std::endl;
// std::cout<<"showing block map:"<<std::endl;
// for(int i=0;i<block_cnt;i++)std::cout<<std::get<0>(block_map[i])<<' '<<std::get<1>(block_map[i])<<' '<<std::get<2>(block_map[i])<<std::endl;
// std::cout<<"###"<<std::endl;
  origin_size = block_cnt * DiskBlockSize;
  while(origin_size < offset + data.size())
  {
    auto alloc_call = metadata_server_->call("alloc_block",id);
    if(alloc_call.is_err())
    {
      goto write_fail;
    }
    auto alloc_resp = alloc_call.unwrap()->as<BlockInfo>();
    block_map.push_back(alloc_resp);
    block_cnt++;
    origin_size += DiskBlockSize;
// std::cout<<"added "<<std::get<0>(alloc_resp)<<' '<<std::get<1>(alloc_resp)<<' '<<std::get<2>(alloc_resp)<<std::endl;
  }
  while(cur_index < block_cnt && write_sz < data_sz)
  {
// std::cout<<"now cur_index and write_sz is "<<cur_index<<' '<<write_sz<<std::endl;
    if(offset >= DiskBlockSize)
    {
      offset -= DiskBlockSize;
      cur_index++;
      continue;
    }
    block_id_t block_id = std::get<0>(block_map[cur_index]);
    mac_id_t mac_id = std::get<1>(block_map[cur_index]);
    // version_t version = std::get<2>(block_map[cur_index]);

    auto write_len = std::min((usize)(DiskBlockSize - offset), (usize)(data_sz - write_sz));
    std::vector<u8>buffer;
    buffer.clear();
    for(int i=0;i<write_len;i++)
      buffer.push_back(data[write_index]),write_index++;

    auto datasrv_call = data_servers_[mac_id]->call("write_data",block_id,offset,buffer);
// std::cout<<"write to machine "<<mac_id<<" block "<<block_id<<" with offset "<<offset<<" and length "<<buffer.size()<<std::endl;
// for(int i=0;i<buffer.size();i++)std::cout<<buffer[i]<<' ';
// std::cout<<std::endl;
    if(datasrv_call.is_err())
    {
      goto write_fail;
    }
    auto write_resp = datasrv_call.unwrap()->as<bool>(); 
    if(write_resp == false)
    {
      goto write_fail;
    }
    write_sz += write_len;
    offset = 0;
    cur_index++;
  }
  if(write_sz != data_sz)
  {
    goto write_fail;
  }
  return KNullOk;
write_fail:
  return ChfsNullResult(ErrorType::BadResponse);

}

// {Your code here}
auto ChfsClient::free_file_block(inode_id_t id, block_id_t block_id,
                                 mac_id_t mac_id) -> ChfsNullResult {
  // TODO: Implement this function.
  // UNIMPLEMENTED();
  auto srv_call = metadata_server_->call("free_block",id,block_id,mac_id);
  if(srv_call.is_err())
    return ChfsNullResult(srv_call.unwrap_error());
  auto response = srv_call.unwrap()->as<bool>();
  if(!response)return ChfsNullResult(ErrorType::BadResponse);
  return KNullOk;
}

} // namespace chfs