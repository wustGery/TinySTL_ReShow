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
		//�����Ҫ������ڴ����128bytes�������1��������
		if (bytes > __MAX_BYTES) 
		{
			return malloc(bytes);
		}
		//�ҵ���������һ��
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

	//���ռ䣬�µĿռ佫ȡ���ڴ��(����chunk_alloc()���)
	void* alloc::refill(size_t n)
	{
		size_t nobjs = 20;
		obj *result;
		obj *current_obj, *next_obj;
		int i;
		char* chunk = static_cast<char*>(chunk_alloc(n, nobjs));

		obj** my_free_list;

		//���ֻ���һ������,�������ͷ������������,free_list���½ڵ�
		if (nobjs == 1) return chunk;

		my_free_list = free_list + FREELIST_INDEX(n);

		//������chunk�ռ��ڽ���free list

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

		//�ڴ��ʣ��ռ������Ҫ�Ŀռ䣬�����ֱ�ӽ�������
		if (bytes_left >= total_bytes)
		{
			result = start_free;
			start_free += total_bytes;
			return result;
		}
		//���ʣ��ռ����ٿ�������һ������
		else if (bytes_left >= size)
		{
			nobjs = bytes_left / size;
			total_bytes = size*nobjs;
			result = start_free;
			start_free += total_bytes;
			return result;
		}
		//�ڴ��ʣ��ռ���һ������Ĵ�С���޷�����
		else
		{
			size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);
			//һ���������ڴ���еĲ�����ͷ�������ü�ֵ
			if (bytes_left>0)
			{
				obj** my_free_list = free_list + FREELIST_INDEX(bytes_left);
				obj *q = (obj*)(start_free);
				q->free_list_link = *my_free_list;
				*my_free_list = q;
			}
			//����heap�ռ䣬���������ڴ��
			start_free = static_cast<char*>(malloc(bytes_to_get));
			if (0 == start_free)
			{
				//heap�ռ䲻��,malloc()ʧ��
				int i;
				obj** my_free_list;
				obj* p;
				//������Ѱ���ʵ�free list
				//��ν�ʵ���ָ������δ�����飬�����鹻��֮free_list
				for (i = size; i <= __MAX_BYTES; i += __ALIGN)
				{
					my_free_list = free_list + FREELIST_INDEX(i);
					p = *my_free_list;
					if (p != 0)
					{
						//free_lists����δ�����飬����free_list���ͷŴ�δ������
						*my_free_list = p->free_list_link;
						start_free = (char*)p;
						end_free = start_free + i;
						return (chunk_alloc(size, nobjs));
					}
				}
				end_free = 0; //����������⣬ɽ��ˮ��������û���ڴ������
			}

			//start_free = static_cast<char*>malloc_alloc::allocate(bytes_to_get));

			heap_size += bytes_to_get;//�Ѿ�����ĵ��ۼ�����
			end_free = start_free + bytes_to_get;
			return(chunk_alloc(size, nobjs));
		}
	}
}
