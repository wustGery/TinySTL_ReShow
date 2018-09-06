#include "stdafx.h"
#include "Allocator.h"
#include "Iterator.h"
#include<algorithm>
#include<type_traits>
#include<iostream>
#include<memory>


namespace TinySTL
{
	//********vector********
	template<class T,class Alloc = alloc>
	class vector
	{
	private:
		T *start_;
		T *finish_;
		T *endOfStorage_;
	public:
		typedef T value_type;
		typedef T* iterator;
		typedef const T* const_iterator;
		typedef iterator pointer;
		typedef T& reference;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
	protected:
		typedef simple_alloc<value_type, Alloc> data_Allocator;

		void insert_aux(iterator position, const T& x)
		{
			//cout << "WOOOOOOOOOOOOOOOOOOOOOOOOOO" << endl;
			if (finish_ != endOfStorage_)//还有空间插入这个元素
			{
				std::cout << "我到了这里了" << std::endl;
				construct(finish_, *(finish_ - 1));

				std::cout << "我到了这里了1" << std::endl;
				++finish_;
				T x_copy = x;
				std::copy_backward(position, finish_ - 2, finish_ - 1);
				*position = x_copy;
			}
			else//没有备用空间
			{
				//如果原大小为0，则配置1，如果不为0，则为原大小的2倍
				//前半段用来放数据，后半段用来放置新数据
				const size_type old_size = size();
				const size_type len = old_size != 0 ? 2 * old_size : 1;

				std::cout << "分配了的空间为" << len << std::ends;

				iterator new_start = (iterator)data_Allocator::allocate(len);
				iterator new_finish = new_start;
				try
				{
					//将[start_,position)的数据移到新的地址空间
					new_finish = std::uninitialized_copy(start_, position, new_start);
					//构造插入的点
					construct(new_finish, x);
					std::cout << "构造了" << x << std::endl;
					++new_finish;
					new_finish = std::uninitialized_copy(position, finish_, new_finish);
				}
				catch (...)
				{
					destroy(new_start, new_finish);
					data_Allocator::deallocate(new_start, len);
					throw;
				}
				//析构并且释放原vector
				destroy(begin(), end());
				deallocate();

				//调整迭代器，指向新的vector
				start_ = new_start;
				finish_ = new_finish;
				endOfStorage_ = new_start + len;
			}
		}
		

		void deallocate()
		{
			if (start_)
				data_Allocator::deallocate(start_, endOfStorage_ - start_);
		}
		
		void fill_initialize(size_type n, const T& value)
		{
			start_ = allocate_and_fill(n, value);
			finish_ = start_ + n;
			endOfStorage_ = finish_;  //之所以相等是因为最开始分配的内存是固定的刚好存满了元素
		}

	public:
		//构造函数
		vector() :start_(0), finish_(0), endOfStorage_(0)
		{
			std::cout << "调用了空的构造函数" << std::endl;
		}
		//带参构造函数
		vector(size_type n, const T& value) 
		{
			std::cout << "调用了size_type有参构造函数" << std::endl;
			fill_initialize(n, value); 
		}
		vector(int n, const T& value)
		{
			std::cout << "调用了int有参构造函数" << std::endl;
			fill_initialize(n, value); 
		}
		vector(long n, const T& value) { fill_initialize(n, value); }
		explicit vector(size_type n) { fill_initialize(n, T()); }
		//析构函数
		~vector()
		{
			destroy(start_, finish_); //析构基本工具  
			deallocate();   //这是Vector的一个成员函数
		}
	public:
		iterator begin() { return start_; }
		iterator end() { return finish_; }
		size_type size() const { return size_type(finish_ - start_); }
		size_type capacity() const{ return size_type(endOfStorage_ - start_);}
		bool empty() const { return begin() == end(); }
		reference operator[](size_type n){ return *(begin() + n); }
		//取第一个元素
		reference front() { return *begin(); }
		//取最后一个元素
		reference back() { return *(end()-1); }
		
		//将元素插入至尾端
		void push_back(const T& x)
		{
			//当还有多余的空间
			if (finish_ != endOfStorage_)
			{
				construct(finish_, x); //构造对象
				std::cout << "构造了" << x << std::endl;
				++finish_;
			}
			else insert_aux(end(), x);
		}
		
		//将尾端元素取出来
		void pop_back()
		{
			--finish_;
			destroy(finish_);
		}
		
		//消除某位置上的元素
		iterator erase(iterator position) 
		{
			if (position + 1 != end())
				std::copy(position + 1, finish_, position);
			--finish_;
			destroy(finish_);
			return position;
		}
	//	_SCL_SECURE_NO_WARNINGS
		

		//消除[first,last)中的所有元素
		iterator erase(iterator first, iterator last)
		{
			iterator i = std::copy(last, finish_, first);
			destroy(i, finish_);
			finish_ = finish_ - (last - first);
			return first;
		}
		

		void clear(){erase(begin(), end());}

		void insert(iterator position, const value_type& val)
		{
			if (finish_ +1 != endOfStorage_)
			{
				T x_copy = val;
				std::copy_backward(position, finish_, finish_+1);
				construct(position, x_copy);
				++finish_;
			}
			else
			{
				insert_aux(position, val);
			}
		}

		//vector::insert()的实现内容
		//从position开始，插入n个元素，元素初值为x
		void insert(iterator position, size_type n, const T& x){}


	protected:
		//配置空间并且填满内容
		iterator allocate_and_fill(size_type n, const T& x)
		{	
			iterator result = (iterator)(data_Allocator::allocate(n)); //利用空间配置器配置一块内存，返回内存的起始地址
			std::uninitialized_fill_n(result, n, x);
			return result;
		}
	};
}
