#include <algorithm>
#include <sstream>

#include "filesystem/directory_op.h"

namespace chfs {

/**
 * Some helper functions
 */
auto string_to_inode_id(std::string &data) -> inode_id_t {
  std::stringstream ss(data);
  inode_id_t inode;
  ss >> inode;
  return inode;
}

auto inode_id_to_string(inode_id_t id) -> std::string {
  std::stringstream ss;
  ss << id;
  return ss.str();
}

// {Your code here}
auto dir_list_to_string(const std::list<DirectoryEntry> &entries)
    -> std::string {
  std::ostringstream oss;
  usize cnt = 0;
  for (const auto &entry : entries) {
    oss << entry.name << ':' << entry.id;
    if (cnt < entries.size() - 1) {
      oss << '/';
    }
    cnt += 1;
  }
  return oss.str();
}

// {Your code here}
auto append_to_directory(std::string src, std::string filename, inode_id_t id)
    -> std::string {

  // TODO: Implement this function.
  //       Append the new directory entry to `src`.
  // UNIMPLEMENTED();
std::cout<<"#trying to append "<<filename<<std::endl;
  std::string new_entry = filename + ":" + inode_id_to_string(id);
  if (!src.empty()) {
    src += "/";
  }
  src += new_entry;
  
std::cout<<"#success append"<<std::endl;
  return src;
}

// {Your code here}
void parse_directory(std::string &src, std::list<DirectoryEntry> &list) {

  // TODO: Implement this function.
  // UNIMPLEMENTED();
  list.clear();

  std::istringstream iss(src);
  std::string entry;

  while (std::getline(iss, entry, '/')) {
    size_t pos = entry.find(':');
    if (pos != std::string::npos) {
      std::string name = entry.substr(0, pos);
      std::string id_str = entry.substr(pos + 1);
      inode_id_t id = string_to_inode_id(id_str);
      DirectoryEntry newEntry;
      newEntry.id=id,newEntry.name=name;
      list.push_back(newEntry);
    }
  }

}

// {Your code here}
auto rm_from_directory(std::string src, std::string filename) -> std::string {

  auto res = std::string("");

  // TODO: Implement this function.
  //       Remove the directory entry from `src`.
  // UNIMPLEMENTED();
  std::list<DirectoryEntry> entries;
  parse_directory(src, entries);

  entries.remove_if([&filename](const DirectoryEntry &entry) {
    return entry.name == filename;
  });
  res = dir_list_to_string(entries);
  return res;
}

/**
 * { Your implementation here }
 */
auto read_directory(FileOperation *fs, inode_id_t id,
                    std::list<DirectoryEntry> &list) -> ChfsNullResult {
  
  // TODO: Implement this function.
  // UNIMPLEMENTED();
  list.clear(); 

  auto read_dir_res = fs->read_file(id);
  if (read_dir_res.is_err()) {
    return ChfsNullResult(read_dir_res.unwrap_error());
  }

  auto dir_content = read_dir_res.unwrap();
  std::string str="";
  for(int i=0;i<dir_content.size();i++)
    str+=dir_content[i];

  parse_directory(str, list);

  return KNullOk;
}

// {Your code here}
auto FileOperation::lookup(inode_id_t id, const char *name)
    -> ChfsResult<inode_id_t> {
  std::list<DirectoryEntry> list;

  // TODO: Implement this function.
  // UNIMPLEMENTED();
  auto read_dir_res = read_directory(this, id, list);
  if (read_dir_res.is_err()) {
    return ChfsResult<inode_id_t>(read_dir_res.unwrap_error());
  }

  for (const auto &entry : list) {
    if (entry.name == name) {
      return ChfsResult<inode_id_t>(entry.id);
    }
  }

  return ChfsResult<inode_id_t>(ErrorType::NotExist);
}

// {Your code here}
auto FileOperation::mk_helper(inode_id_t id, const char *name, InodeType type)
    -> ChfsResult<inode_id_t> {

  // TODO:
  // 1. Check if `name` already exists in the parent.
  //    If already exist, return ErrorType::AlreadyExist.
  // 2. Create the new inode.
  // 3. Append the new entry to the parent directory.
  // UNIMPLEMENTED();
  std::list<DirectoryEntry> list;
  auto read_dir_res = read_directory(this, id, list);
  if (read_dir_res.is_err()) {
    return ChfsResult<inode_id_t>(read_dir_res.unwrap_error());
  }
  for (const auto &entry : list) {
    if (entry.name == name) {
      return ChfsResult<inode_id_t>(ErrorType::AlreadyExist);
    }
  }

  auto alloc_inode_res = this->alloc_inode(type);
  if (alloc_inode_res.is_err()) {
    return ChfsResult<inode_id_t>(alloc_inode_res.unwrap_error());
  }
  inode_id_t new_inode_id = alloc_inode_res.unwrap();

  list.push_back({name, new_inode_id});
  std::string new_dir_str = dir_list_to_string(list);

  std::vector<u8>new_dir_vec;
  new_dir_vec.clear();
  for(int i=0;i<new_dir_str.size();i++)
    new_dir_vec.push_back(new_dir_str[i]);

  auto write_dir_res = this->write_file(id, new_dir_vec);
  if (write_dir_res.is_err()) {
    return ChfsResult<inode_id_t>(write_dir_res.unwrap_error());
  }
  return ChfsResult<inode_id_t>(static_cast<inode_id_t>(new_inode_id));
}

// {Your code here}
auto FileOperation::unlink(inode_id_t parent, const char *name)
    -> ChfsNullResult {
// std::cout<<parent<<"#######";
  // TODO: 
  // 1. Remove the file, you can use the function `remove_file`
  // 2. Remove the entry from the directory.
  // UNIMPLEMENTED();
  std::list<DirectoryEntry> entries;
  entries.clear();
  auto read_dir_res = read_directory(this, parent, entries);
  if (read_dir_res.is_err()) {
    return ChfsNullResult(read_dir_res.unwrap_error());
  }

  auto lookup_res = lookup(parent, name);
  if(lookup_res.is_err())
  {
    return ChfsNullResult(lookup_res.unwrap_error());
  }
  auto file_id = lookup_res.unwrap();
  // auto free_res = inode_manager_->free_inode(file_id);
  // if(free_res.is_err())
  //   return ChfsNullResult(free_res.unwrap_error());

  auto remove_res=this->remove_file(file_id);
  if(remove_res.is_err())
  {
    return ChfsNullResult(remove_res.unwrap_error());
  }


  entries.remove_if([&file_id](const DirectoryEntry &entry) {
    return entry.id == file_id;
  });
  std::string str=dir_list_to_string(entries);
  std::vector<u8>vec;
  vec.clear();
  for(int i=0;i<str.size();i++)
    vec.push_back(str[i]);
  auto write_dir_res = this->write_file(parent, vec);
  if (write_dir_res.is_err()) {
    return ChfsNullResult(write_dir_res.unwrap_error());
  }

  return KNullOk;
}

} // namespace chfs
