// memory_allocator_fl.h

/*
 * memory_allocator_fl.h
 *
 * Created on: 2018.02.22
 *     Author: michael gao
 */
#ifndef __MEMORY_ALLOCATOR_FL_H__
#define __MEMORY_ALLOCATOR_FL_H__

#include "global_define.h"

const unsigned int kBlockNumPerSegmnet =1024
const unsigned int kMaxBlockNum = 0

class MemoryAllocatorFL
{
 public:
	MemoryAllocatorFL()	{
		init_finish_ = 0;
	}

	~MemoryAllocatorFL() {
		if (init_finish_)
			reset(1);
	}

	var_4 init (var_u8 block_size, var_u4 block_count = kMaxBlockNum, var_u4 block_num_per_segment = kBlockNumPerSegmnet) {
		
		block_size_ = block_size + sizeof(var_vd*);		
		else if (block_count == kMaxBlockNum) {
			bc_max_ = 0x7FFFFFFFFFFFFFFF;
		}
		else {
			bc_max_ = block_count;
		}
		block_num_of_each_ = block_num_per_segment;
		bc_use_   = 0;
		recycle_lib_   = NULL;
		segment_lib_   = NULL;
		segment_lib_idle_ = NULL;
		cur_seg_pos_ = NULL;
		cur_seg_use_ = 0;
		init_finish_ = 1;
		return 0;
	}

	var_vd* pop() {
		locker_.lock();

		if (bc_use_ >= bc_max_) {
			locker_.unlock();
			return NULL;
		}

		if (recycle_lib_) {
			var_1* buffer = recycle_lib_;

			recycle_lib_ = *(var_1**)buffer;
			buffer += sizeof(var_1*);

			bc_use_++;

			locker_.unlock();
			return (var_vd*)buffer;
		}

		if (cur_seg_use_ >= block_num_of_each_ || cur_seg_pos_ == NULL) {
			var_1* mem_buf = NULL;

			if (segment_lib_idle_ == NULL)
			{
				mem_buf = new(std::nothrow) var_1[block_num_of_each_ * block_size_ + sizeof(var_vd*)];
				if (mem_buf == NULL) {
					locker_.unlock();
					return NULL;
				}
			}
			else {
				mem_buf = segment_lib_idle_;
				segment_lib_idle_ = *(var_1**)segment_lib_idle_;
			}

			*(var_1**)mem_buf = segment_lib_;
			segment_lib_ = mem_buf;

			cur_seg_pos_ = segment_lib_ + sizeof(var_1*);
			cur_seg_use_ = 0;
		}

		var_1* buffer = cur_seg_pos_;
		cur_seg_pos_ += block_size_;
		cur_seg_use_ += 1;
		buffer += sizeof(var_1*);
		bc_use_++;
		locker_.unlock();
		return (var_vd*)buffer;
	}

	var_vd push(var_vd* memory) {
		locker_.lock();

		var_1* buffer = (var_1*)memory - sizeof(var_1*);
		*(var_1**)buffer = recycle_lib_;
		recycle_lib_ = buffer;
		bc_use_ --;

		locker_.unlock();
	}

	void reset(var_4 is_free = 0) {
		for (;;) {
			locker_.lock();
			if (bc_use_ == 0) {
				break;
			}
			locker_.unlock();
			cp_sleep(1);
		}

		if (is_free == 1) {
			while (segment_lib_idle_) {
				var_1* mem_buf = segment_lib_idle_;
				segment_lib_idle_ = *(var_1**)segment_lib_idle_;
				delete[] mem_buf;
			}

			while (segment_lib_) {
				var_1* mem_buf = segment_lib_;
				segment_lib_ = *(var_1**)segment_lib_;
				delete[] mem_buf;
			}
		}else{
			while (segment_lib_){
				var_1* mem_buf = segment_lib_;
				segment_lib_ = *(var_1**)segment_lib_;
				*(var_1**)mem_buf = segment_lib_idle_;
				segment_lib_idle_ = mem_buf;
			}
		}

		recycle_lib_ = NULL;
		cur_seg_pos_ = NULL;
		cur_seg_use_ = 0;
		locker_.unlock();
	}
	
	var_8 use_count(){
		return bc_use_;
	}
	
	const var_1* version() {
		// v1.000 - 2018.02.22
		return "v1.000";
	}

 private:
	var_8  block_size_;
	var_8  block_num_of_each_;
	var_8  bc_use_;                   // block count of use
	var_8  bc_max_;                   // block count max value
	var_1* recycle_lib_;
	var_1* segment_lib_;
	var_1* segment_lib_idle_;
	var_1* cur_seg_pos_;
	var_8  cur_seg_use_;	
	var_4  init_finish_;
	MUTEXLOCK locker_;
};

#endif // __MEMORY_ALLOCATOR_FL_H__


