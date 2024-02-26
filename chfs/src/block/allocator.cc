#include "block/allocator.h"
#include "common/bitmap.h"

namespace chfs {

BlockAllocator::BlockAllocator(std::shared_ptr<BlockManager> block_manager)
    : BlockAllocator(std::move(block_manager), 0, true) {}

// Your implementation
BlockAllocator::BlockAllocator(std::shared_ptr<BlockManager> block_manager,
                               usize bitmap_block_id, bool will_initialize)
    : bm(std::move(block_manager)), bitmap_block_id(bitmap_block_id) {
  // calculate the total blocks required
  const auto total_bits_per_block = this->bm->block_size() * KBitsPerByte;
  auto total_bitmap_block = this->bm->total_blocks() / total_bits_per_block;
  if (this->bm->total_blocks() % total_bits_per_block != 0) {
    total_bitmap_block += 1;
  }

  CHFS_VERIFY(total_bitmap_block > 0, "Need blocks in the manager!");
  CHFS_VERIFY(total_bitmap_block + this->bitmap_block_id <= bm->total_blocks(),
              "not available blocks to store the bitmap");

  this->bitmap_block_cnt = total_bitmap_block;
  if (this->bitmap_block_cnt * total_bits_per_block ==
      this->bm->total_blocks()) {
    this->last_block_num = total_bits_per_block;
  } else {
    this->last_block_num = this->bm->total_blocks() % total_bits_per_block;
  }
  CHFS_VERIFY(this->last_block_num <= total_bits_per_block,
              "last block num should be less than total bits per block");

  if (!will_initialize) {
    return;
  }

  // zeroing
  for (block_id_t i = 0; i < this->bitmap_block_cnt; i++) {
    this->bm->zero_block(i + this->bitmap_block_id);
  }

  block_id_t cur_block_id = this->bitmap_block_id;
  std::vector<u8> buffer(bm->block_size());
  auto bitmap = Bitmap(buffer.data(), bm->block_size());
  bitmap.zeroed();

  // set the blocks of the bitmap block to 1
  for (block_id_t i = 0; i < this->bitmap_block_cnt + this->bitmap_block_id;
       i++) {
    // + bitmap_block_id is necessary, since the bitmap block starts with an
    // offset
    auto block_id = i / total_bits_per_block + this->bitmap_block_id;
    auto block_idx = i % total_bits_per_block;

    if (block_id != cur_block_id) {
      bm->write_block(cur_block_id, buffer.data());

      cur_block_id = block_id;
      bitmap.zeroed();
    }

    bitmap.set(block_idx);
  }

  bm->write_block(cur_block_id, buffer.data());
}

// Fixme: currently we don't consider errors in this implementation
auto BlockAllocator::free_block_cnt() const -> usize {
  usize total_free_blocks = 0;
  std::vector<u8> buffer(bm->block_size());

  for (block_id_t i = 0; i < this->bitmap_block_cnt; i++) {
    bm->read_block(i + this->bitmap_block_id, buffer.data()).unwrap();

    usize n_free_blocks = 0;
    if (i == this->bitmap_block_cnt - 1) {
      // last one
      // std::cerr <<"last block num: " << this->last_block_num << std::endl;
      n_free_blocks = Bitmap(buffer.data(), bm->block_size())
                          .count_zeros_to_bound(this->last_block_num);
    } else {
      n_free_blocks = Bitmap(buffer.data(), bm->block_size()).count_zeros();
    }
    // std::cerr << "check free block: " << i << " : " << n_free_blocks
    //           << std::endl;
    total_free_blocks += n_free_blocks;
  }
  return total_free_blocks;
}

// Your implementation
auto BlockAllocator::allocate() -> ChfsResult<block_id_t> {
  std::vector<u8> buffer(bm->block_size());

  for (uint i = 0; i < this->bitmap_block_cnt; i++) {
    bm->read_block(i + this->bitmap_block_id, buffer.data());
      // if(buffer[4]&1 && i==0)
      // {
      //   std::cout<<"32 occupied when to allocate "<<std::endl;
      // }

    // The index of the allocated bit inside current bitmap block.
    std::optional<block_id_t> res = std::nullopt;

    if (i == this->bitmap_block_cnt - 1) {
      // If current block is the last block of the bitmap.

      // TODO: Find the first free bit of current bitmap block
      // and store it in `res`.
      // UNIMPLEMENTED();
      auto bitmap = Bitmap(buffer.data(), bm->block_size());
      res = bitmap.find_first_free_w_bound(this->last_block_num);
    } else {

      // TODO: Find the first free bit of current bitmap block
      // and store it in `res`.
      // UNIMPLEMENTED();
      auto bitmap = Bitmap(buffer.data(), bm->block_size());
      res = bitmap.find_first_free();
    }

    // If we find one free bit inside current bitmap block.
    if (res) {
      // The block id of the allocated block.
      block_id_t retval = (*res);//static_cast<block_id_t>(0);
      // if(buffer[4]&1 && i==0)
      // {
      //   std::cout<<"32 occupied when allocate "<<retval<<std::endl;
      // }

      // TODO:
      // 1. Set the free bit we found to 1 in the bitmap.
      // 2. Flush the changed bitmap block back to the block manager.
      // 3. Calculate the value of `retval`.
      // UNIMPLEMENTED();
      
      // auto bitmap_block_idx = retval / (bm->block_size()*KBitsPerByte);
      // auto bit_idx = retval % (bm->block_size()*KBitsPerByte);
      auto bitmap = Bitmap(buffer.data(), bm->block_size());
      bitmap.set(retval);
      buffer[retval / KBitsPerByte] |= (1 << (retval % KBitsPerByte));
      // memcpy(buffer.data(), bitmap.data, bm->block_size());
      // buffer[bit_idx / 8] |= (1 << (bit_idx % 8));
      bm->write_block(i + this->bitmap_block_id, buffer.data());
      // if(retval==32)
      // {
      //   std::cout<<"allocate "<<retval<<std::endl;
      //   std::cout<<"bitmap set "<<retval + this->bitmap_block_id<<std::endl;
      //   std::cout<<"buffer set "<<retval / KBitsPerByte<<" with "<<(1 << (retval % KBitsPerByte))<<std::endl;
      // }
      return ChfsResult<block_id_t>(i * bm->block_size() * KBitsPerByte + retval);
    }
  }
  return ChfsResult<block_id_t>(ErrorType::OUT_OF_RESOURCE);
}

// Your implementation
auto BlockAllocator::deallocate(block_id_t block_id) -> ChfsNullResult {
  if (block_id >= this->bm->total_blocks()) {
    return ChfsNullResult(ErrorType::INVALID_ARG);
  }
//   std::cout<<"TEST: to deallocated "<<(int)*it<<std::endl;
//   for(int j=0;j<active_blocks.size();j++)
//   {
//     auto jt=active_blocks.begin();
//         std::advance(jt, j);
//       if((*jt)==0)std::cout<<*(jt)<<'('<<j<<')';
//   }
//       std::cout<<std::endl;
//   }

  // TODO: Implement this function.
  // 1. According to `block_id`, zero the bit in the bitmap.
  // 2. Flush the changed bitmap block back to the block manager.
  // 3. Return ChfsNullResult(ErrorType::INVALID_ARG) 
  //    if you find `block_id` is invalid (e.g. already freed).
  // UNIMPLEMENTED();
  
  auto bitmap_block_idx = block_id / (bm->block_size()*KBitsPerByte);
  auto bit_idx = block_id % (bm->block_size()*KBitsPerByte);
  std::vector<u8> buffer(bm->block_size());
  bm->read_block(bitmap_block_idx + this->bitmap_block_id, buffer.data());
      // if(buffer[4]&1 && bitmap_block_idx==0)
      // {
      //   std::cout<<"32 occupied when to deallocate "<<block_id<<std::endl;
      // }

  auto bitmap = Bitmap(buffer.data(), bm->block_size());
  if (!bitmap.check(bit_idx)) {
    // std::cout<<"deallocate "<<block_id<<" but is empty!"<<std::endl;
    return ChfsNullResult(ErrorType::INVALID_ARG);
  }
  bitmap.clear(bit_idx);
  buffer[bit_idx / KBitsPerByte] &= ~(1 << (bit_idx % KBitsPerByte));
  
  bm->write_block(bitmap_block_idx + this->bitmap_block_id, buffer.data());
      // if(block_id==32)
      // {
      //   std::cout<<"deallocate "<<block_id<<std::endl;
      //   std::cout<<"write to block "<<bitmap_block_idx + this->bitmap_block_id<<std::endl;
      //   std::cout<<"bitmap clear index= "<<bit_idx<<std::endl;
      // }

  return KNullOk;
}

} // namespace chfs