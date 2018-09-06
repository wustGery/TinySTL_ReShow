//
// Created by wust_gery on 18-7-26.
//

#ifndef TINYSTL_ALLOC_H
#define TINYSTL_ALLOC_H

#include <cstdlib>

namespace TinySTL
{
	/* �ռ������������ֽ���Ϊ��λ����
	* �ڶ���������*/
	class alloc {
	private:
		enum { __ALIGN = 8 }; //С��������ϵ��߽�
		enum { __MAX_BYTES = 128 }; //С�����������
		enum { __NFREELISTS = __MAX_BYTES / __ALIGN };  //free_lists����
	private:
		union obj
		{
			union obj* free_list_link;
			char data[1];
		};
	private:
		static obj* free_list[__NFREELISTS];
	private:
		static char* start_free; //�ڴ����ʼλ��,ֻ��chunk_alloc()�б仯
		static char* end_free; //�ڴ�ؽ���λ�ã�ֻ��chunk_alloc()�б仯
		static size_t heap_size; //�����ۻ���
	private:
		static size_t ROUND_UP(size_t bytes)
		{
			return ((bytes)+__ALIGN - 1) & ~(__ALIGN - 1);
		}
		//�������������С������ʹ�õ�n��free��list��n��0����
		static size_t FREELIST_INDEX(size_t bytes)
		{
			return ((bytes + __ALIGN - 1) / __ALIGN - 1);
		}
		//����һ����СΪn�Ķ��󣬲����ܼ����СΪn���������鵽free-list
		static void* refill(size_t n);
		//����һ���ռ䣬������nobjs����СΪsize������
		//�������nobjs�������������㣬nobjs���ܻή��
		static char *chunk_alloc(size_t size, size_t& nobjs);
	public:
		static void* allocate(size_t);
		static void deallocate(void*, size_t);
		static void* reallocate(void* p, size_t, size_t);
	};
}

#endif //TINYSTL_ALLOC_H
