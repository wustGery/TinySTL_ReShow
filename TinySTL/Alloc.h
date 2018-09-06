//
// Created by wust_gery on 18-7-26.
//

#ifndef TINYSTL_ALLOC_H
#define TINYSTL_ALLOC_H

#include <cstdlib>

namespace TinySTL
{
	/* 空间配置器，以字节数为单位分配
	* 第二级配置器*/
	class alloc {
	private:
		enum { __ALIGN = 8 }; //小型区块的上调边界
		enum { __MAX_BYTES = 128 }; //小型区块的上限
		enum { __NFREELISTS = __MAX_BYTES / __ALIGN };  //free_lists个数
	private:
		union obj
		{
			union obj* free_list_link;
			char data[1];
		};
	private:
		static obj* free_list[__NFREELISTS];
	private:
		static char* start_free; //内存池起始位置,只在chunk_alloc()中变化
		static char* end_free; //内存池结束位置，只在chunk_alloc()中变化
		static size_t heap_size; //分配累积量
	private:
		static size_t ROUND_UP(size_t bytes)
		{
			return ((bytes)+__ALIGN - 1) & ~(__ALIGN - 1);
		}
		//函数根据区块大小，决定使用第n号free—list。n从0起算
		static size_t FREELIST_INDEX(size_t bytes)
		{
			return ((bytes + __ALIGN - 1) / __ALIGN - 1);
		}
		//返回一个大小为n的对象，并可能加入大小为n的其他区块到free-list
		static void* refill(size_t n);
		//配置一大块空间，可容纳nobjs个大小为size的区块
		//如果配置nobjs个区块有所不便，nobjs可能会降低
		static char *chunk_alloc(size_t size, size_t& nobjs);
	public:
		static void* allocate(size_t);
		static void deallocate(void*, size_t);
		static void* reallocate(void* p, size_t, size_t);
	};
}

#endif //TINYSTL_ALLOC_H
