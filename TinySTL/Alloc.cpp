//
// Created by wust_gery on 18-7-26.
//
#include "stdafx.h"
#include "Alloc.h"

namespace TinySTL
{
	char* alloc::start_free = 0;
	char* alloc::end_free = 0;
	size_t alloc::heap_size = 0;

	alloc::obj* alloc::free_list[__NFREELISTS] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	void *alloc::allocate(size_t bytes)
	{
		obj** my_free_list;
		obj* result;
		//如果需要分配的内存大于128bytes，则调用1级配置器
		if (bytes > __MAX_BYTES) 
		{
			return malloc(bytes);
		}
		//找到所属的那一块
		my_free_list = free_list + FREELIST_INDEX(bytes);
		result = *my_free_list;

		if (0 == result)
		{
			void* r = refill(ROUND_UP(bytes));
			return r;
		}
		*my_free_list = result->free_list_link;
		return result;
	}

	void alloc::deallocate(void *p, size_t bytes)
	{
		obj** my_free_list;
		obj *q = (obj*)p;

		if (bytes>alloc::__MAX_BYTES)
		{
			free(p);
		}
		my_free_list = free_list + FREELIST_INDEX(bytes);
		q->free_list_link = *my_free_list;
		*my_free_list = q;
	}


	void* alloc::reallocate(void *p, size_t old_sz, size_t new_sz)
	{
		deallocate(p, old_sz);
		p = allocate(new_sz);
		return p;
	}

	//填充空间，新的空间将取自内存池(经由chunk_alloc()完成)
	void* alloc::refill(size_t n)
	{
		size_t nobjs = 20;
		obj *result;
		obj *current_obj, *next_obj;
		int i;
		char* chunk = static_cast<char*>(chunk_alloc(n, nobjs));

		obj** my_free_list;

		//如果只获得一个区块,这个区块就分配给调用者用,free_list无新节点
		if (nobjs == 1) return chunk;

		my_free_list = free_list + FREELIST_INDEX(n);

		//以下在chunk空间内建立free list

		result = (obj*)chunk; 

		*my_free_list = next_obj = (obj*)(chunk + n);

		for (i = 1;; i++)
		{
			current_obj = next_obj;
			next_obj = (obj*)((char*)next_obj+n);
			if (nobjs - 1 == i)
			{
				current_obj->free_list_link = 0;
				break;
			}
			else
			{
				current_obj->free_list_link = next_obj;
			}
		}
		return result;
	}

	char* alloc::chunk_alloc(size_t size, size_t& nobjs)
	{
		char* result;
		size_t total_bytes = size*nobjs;
		size_t bytes_left = end_free - start_free;

		//内存池剩余空间大于需要的空间，则可以直接进行配置
		if (bytes_left >= total_bytes)
		{
			result = start_free;
			start_free += total_bytes;
			return result;
		}
		//如果剩余空间至少可以满足一个区块
		else if (bytes_left >= size)
		{
			nobjs = bytes_left / size;
			total_bytes = size*nobjs;
			result = start_free;
			start_free += total_bytes;
			return result;
		}
		//内存池剩余空间练一个区块的大小都无法满足
		else
		{
			size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);
			//一下试着让内存池中的参与零头还有利用价值
			if (bytes_left>0)
			{
				obj** my_free_list = free_list + FREELIST_INDEX(bytes_left);
				obj *q = (obj*)(start_free);
				q->free_list_link = *my_free_list;
				*my_free_list = q;
			}
			//配置heap空间，用来补充内存池
			start_free = static_cast<char*>(malloc(bytes_to_get));
			if (0 == start_free)
			{
				//heap空间不足,malloc()失败
				int i;
				obj** my_free_list;
				obj* p;
				//以下搜寻合适的free list
				//所谓适当是指“尚有未用区块，且区块够大”之free_list
				for (i = size; i <= __MAX_BYTES; i += __ALIGN)
				{
					my_free_list = free_list + FREELIST_INDEX(i);
					p = *my_free_list;
					if (p != 0)
					{
						//free_lists尚有未用区块，调整free_list以释放处未用区块
						*my_free_list = p->free_list_link;
						start_free = (char*)p;
						end_free = start_free + i;
						return (chunk_alloc(size, nobjs));
					}
				}
				end_free = 0; //如果出现意外，山穷水尽，到处没有内存可用了
			}

			//start_free = static_cast<char*>malloc_alloc::allocate(bytes_to_get));

			heap_size += bytes_to_get;//已经分配的得累加起来
			end_free = start_free + bytes_to_get;
			return(chunk_alloc(size, nobjs));
		}
	}
}
