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
			if (finish_ != endOfStorage_)//���пռ�������Ԫ��
			{
				std::cout << "�ҵ���������" << std::endl;
				construct(finish_, *(finish_ - 1));

				std::cout << "�ҵ���������1" << std::endl;
				++finish_;
				T x_copy = x;
				std::copy_backward(position, finish_ - 2, finish_ - 1);
				*position = x_copy;
			}
			else//û�б��ÿռ�
			{
				//���ԭ��СΪ0��������1�������Ϊ0����Ϊԭ��С��2��
				//ǰ������������ݣ�������������������
				const size_type old_size = size();
				const size_type len = old_size != 0 ? 2 * old_size : 1;

				std::cout << "�����˵Ŀռ�Ϊ" << len << std::ends;

				iterator new_start = (iterator)data_Allocator::allocate(len);
				iterator new_finish = new_start;
				try
				{
					//��[start_,position)�������Ƶ��µĵ�ַ�ռ�
					new_finish = std::uninitialized_copy(start_, position, new_start);
					//�������ĵ�
					construct(new_finish, x);
					std::cout << "������" << x << std::endl;
					++new_finish;
					new_finish = std::uninitialized_copy(position, finish_, new_finish);
				}
				catch (...)
				{
					destroy(new_start, new_finish);
					data_Allocator::deallocate(new_start, len);
					throw;
				}
				//���������ͷ�ԭvector
				destroy(begin(), end());
				deallocate();

				//������������ָ���µ�vector
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
			endOfStorage_ = finish_;  //֮�����������Ϊ�ʼ������ڴ��ǹ̶��ĸպô�����Ԫ��
		}

	public:
		//���캯��
		vector() :start_(0), finish_(0), endOfStorage_(0)
		{
			std::cout << "�����˿յĹ��캯��" << std::endl;
		}
		//���ι��캯��
		vector(size_type n, const T& value) 
		{
			std::cout << "������size_type�вι��캯��" << std::endl;
			fill_initialize(n, value); 
		}
		vector(int n, const T& value)
		{
			std::cout << "������int�вι��캯��" << std::endl;
			fill_initialize(n, value); 
		}
		vector(long n, const T& value) { fill_initialize(n, value); }
		explicit vector(size_type n) { fill_initialize(n, T()); }
		//��������
		~vector()
		{
			destroy(start_, finish_); //������������  
			deallocate();   //����Vector��һ����Ա����
		}
	public:
		iterator begin() { return start_; }
		iterator end() { return finish_; }
		size_type size() const { return size_type(finish_ - start_); }
		size_type capacity() const{ return size_type(endOfStorage_ - start_);}
		bool empty() const { return begin() == end(); }
		reference operator[](size_type n){ return *(begin() + n); }
		//ȡ��һ��Ԫ��
		reference front() { return *begin(); }
		//ȡ���һ��Ԫ��
		reference back() { return *(end()-1); }
		
		//��Ԫ�ز�����β��
		void push_back(const T& x)
		{
			//�����ж���Ŀռ�
			if (finish_ != endOfStorage_)
			{
				construct(finish_, x); //�������
				std::cout << "������" << x << std::endl;
				++finish_;
			}
			else insert_aux(end(), x);
		}
		
		//��β��Ԫ��ȡ����
		void pop_back()
		{
			--finish_;
			destroy(finish_);
		}
		
		//����ĳλ���ϵ�Ԫ��
		iterator erase(iterator position) 
		{
			if (position + 1 != end())
				std::copy(position + 1, finish_, position);
			--finish_;
			destroy(finish_);
			return position;
		}
	//	_SCL_SECURE_NO_WARNINGS
		

		//����[first,last)�е�����Ԫ��
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

		//vector::insert()��ʵ������
		//��position��ʼ������n��Ԫ�أ�Ԫ�س�ֵΪx
		void insert(iterator position, size_type n, const T& x){}


	protected:
		//���ÿռ䲢����������
		iterator allocate_and_fill(size_type n, const T& x)
		{	
			iterator result = (iterator)(data_Allocator::allocate(n)); //���ÿռ�����������һ���ڴ棬�����ڴ����ʼ��ַ
			std::uninitialized_fill_n(result, n, x);
			return result;
		}
	};
}
