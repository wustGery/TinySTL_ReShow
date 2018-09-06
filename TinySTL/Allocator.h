
#include "stdafx.h"
#include "Alloc.h"
#include "Construct.h"
#include <cassert>
#include <new>

namespace TinySTL
{
	/*�ռ�������,�Ա�����ĿΪ��λ����*/
	template<class T,class Alloc>
	class simple_alloc
	{
	public:
		typedef T  value_type;
		typedef T* pointer;
		typedef const T* const_pointer;
		typedef T& reference;
		typedef const T& const_reference;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
	public:
		//�ڴ�������alloc::allocate()�����ڴ��ͷŲ�����alloc::deallocate()����
		//����������� ����construct()���𣬶������������ɣ���destroy()����

		static T *allocate()
		{
			return static_cast<T *>(alloc::allocate(sizeof(T)));
		}
		static T *allocate(size_t n)
		{
			if (n == 0)  return 0;
			return static_cast<T *>(alloc::allocate(sizeof(T)*n));
		}
		static void deallocate(T *ptr)
		{
			alloc::deallocate(static_cast<void*>(ptr), sizeof(T));
		}
		static void deallocate(T *ptr, size_t n)
		{
			if (n == 0) return;
			alloc::deallocate(static_cast<void*>(ptr), sizeof(T)*n);
		}

		static void construct(T *ptr)
		{
			//placement new�������û�ָ�����ڴ�λ���Ϲ����µĶ���
			//����������̲���Ҫ��������ڴ棬ֻ��Ҫ���ö���Ĺ��캯�����ɡ�
			new(ptr)T(); //placementnewд�� 
		}
		static void construct(T* ptr, const T& value)
		{
			new(ptr)T(value);
		}
		static void destroy(T *ptr)
		{
			ptr->~T();
		}
		static void destroy(T *first, T *last)
		{
			for (; first != last; ++first)
				first->~T();
		}
	};
}

