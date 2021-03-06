

#include "Allocator.h"
#include "Algorithm.h"
#include<iostream>



namespace TinySTL
{
	template<typename T>
	struct _list_node
	{
		typedef _list_node* void_pointer;
		void_pointer prev;
		void_pointer next;
		T data;
	};






	template<class T, class Ref, class Ptr>
	struct _list_iterator {
		typedef _list_iterator<T, T&, T*>   iterator; //指向内部元素值得迭代器
		typedef _list_iterator<T, Ref, Ptr> self; //指向list节点的迭代器
		typedef bidirectional_iterator_tag iterator_category;
		typedef T   value_type;
		typedef Ptr pointer;
		typedef Ref reference;
		typedef _list_node<T>* link_type;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;

		link_type node; //普通指针指向节点
		_list_iterator(link_type x) : node(x) {}
		_list_iterator() {}
		_list_iterator(const iterator& x) : node(x.node) {}
		bool operator==(const self& x) const { return node == x.node; }
		bool operator!=(const self& x) const { return node != x.node; }

		//deference
		reference operator*() const { return (*node).data; }
		pointer   operator->() const { return &(operator*()); } //迭代器的成员存取（member access）运算子的标准做法

		//迭代器向前移动一个位置(++i) 前置++
		self& operator++() {
			node = node->next;
			return *this;
		}

		//迭代器向前移动一个位置(i++)  后置++
		self operator++(int) {
			self tmp = *this;
			++*this;
			return tmp;
		}

		//对迭代器递减1 (--i)
		self& operator--() {
			node = node->prev;
			return *this;
		}

		//对迭代器递减1 (i--)
		self operator--(int) {
			self tmp = *this;
			--*this;
			return tmp;
		}

		self operator+(int dis){
			self tmp = *this;
			while (dis-- > 0) {
				tmp = tmp.node->next;
			}
			return tmp;
		}

		self operator-(int dis){
			self tmp = *this;
			while (dis-- > 0) {
				tmp = tmp.node->prev;
			}
			return tmp;
		}
	};

	//list
	
	template <class T, class Alloc = alloc>
	class list {
	public:
		typedef T value_type;
		typedef value_type* pointer;
		typedef value_type& reference;

		typedef _list_node<T> list_node;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;

		typedef list_node* link_type;

		typedef simple_alloc<list_node, Alloc> listNode_Allocator;

		typedef _list_iterator<T, T&, T*>   iterator;

		//配置一个节点并传回
		link_type get_node() {
			return listNode_Allocator::allocate();
		}

		//释放一个节点
		void put_node(link_type x) {
			listNode_Allocator::deallocate(x);
		}

		//产生一个节点，并且带有元素值
		link_type create_node(const T& x) {
			link_type newListNode = get_node();
			construct(&newListNode->data, x); //所指的数据，创建
			return newListNode;
		}

		//销毁一个元素点 析构并释放
		void destroy_node(link_type d) {
			destroy(&d->data);
			put_node(d);
		}

	public:
		iterator begin() const { return node->next;}
		iterator end() const { return node; }
		bool empty() const { return node->next == node; }

		size_type size() const 
		{
			size_type result = 0;
			result = distance(begin(), end());
			return result;
		}

		reference front() { return *begin(); }
		reference back() { return *(--end()); }

		//构造函数
		list() { empty_initialize(); } //产生空的链表
		~list() {
			clear();
			erase(end());
		}
		//尾部插入节点
		void push_back(const T& x) { //在list尾部空元素处添加数据元素
			insert(end(), x);
		}

		//头部插入节点
		void push_front(const T& x) {
			insert(begin(), x);
		}

		//删除头部节点
		void pop_front() {
			erase(begin());
		}

		//删除尾部节点
		void pop_back() {
			erase(--end());
		}

		//清除所有list节点
		void clear() {
			iterator s = begin();
			while (s != end()) {
				s = erase(s);
			}
			//空list的状态
			node->next = node;
			node->prev = node;
		}

		//删除所有值为x的节点
		void remove(const T& x) {
			iterator s = begin();
			while (s != end()) {
				if (*s == x)
					s = erase(s);
				else
					s++;
			}
		}

		//移除连续并相同的元素
		void unique() {
			iterator first = begin();
			iterator last = end();
			if (first == end) return;  //空链表
			iterator next = first;
			while (++next != last) {
				if (*first == *next)
					erase(next);
				else
					first = next;
				next = first;
			}
		}


		// 删除position位置的节点
		iterator erase(iterator position) {
			link_type next_node = position.node->next;
			link_type prev_node = position.node->prev;
			prev_node->next = next_node;
			destroy_node(position.node);
			return iterator(next_node);
		}

		//在position位置之前插入一个节点
		iterator insert(iterator position, const T& x) 
		{
			link_type tmp = create_node(x);
			tmp->next = position.node;
			tmp->prev = position.node->prev;
			position.node->prev->next = tmp;
			position.node->prev = tmp;
			return tmp;
		}

		//将[first,last)之间的元素移动到position之前
		void transfer(iterator position, iterator first, iterator last) {
			if (last == position)
				return;

			link_type last_node = last.node->prev;
			//将first last取下来
			first.node->prev->next = last.node;
			last.node->prev = first.node->prev;


			link_type prev_node = position.node->prev;
			prev_node->next = first.node;
			first.node->prev = prev_node;

			last_node->next = position.node;
			position.node->prev = last_node;
		}

		//x接合与position之前
		void splice(iterator position, list& x) {
			if (x.empty())
				return;
			transfer(position, x.begin(), x.end());
		}

		//将某一个位置上的元素接合到position之前
		void splice(iterator position, iterator i) {
			iterator j = i;
			j++;
			transfer(position, i, j);
		}

		void bianli(iterator first, iterator last)
		{
			int index = 0;
			while (first != last)
			{
				std::cout << "第" << index++ << "个元素为" << *first << std::endl;
				++first;
			}
		}

		//
		void splice(iterator position, iterator first, iterator last) {
			if (position == last)
				return;
			transfer(position, first, last);
		}

		//merge 将x 与*this合并，两个list必须经过递增排序
		void merge(list& x) {
			iterator p = begin();
			iterator x_start = x.begin();

			while (p != end() && x_start != x.end()) {
				std::cout << *p << std::endl;
				if (*x_start > *p)
					p++;
				else {
					iterator next = x_start + 1; //注意：迭代器跟着节点跑，会脱离原来的list
					splice(p, x_start);
					x_start = next;
				}
			}

			//如果x没完就添加到尾巴上
			if (!x.empty())
				splice(end(), x_start, x.end());
		}

		//reverse颠倒顺序
		void reverse() {
			iterator start = begin();
			iterator stop = end();
			if (size() == 0 || size() == 1)
				return;
			start++;
			while (start != stop) {
				iterator next = start + 1;
				splice(begin(), start);
				start = next;
			}
		}

		//与x交换成员 交换完成后原来两个list上的迭代器要注意
		void swap(list& x) {
			link_type tmp = x.node;
			x.node = this->node;
			this->node = tmp;
		}



		//sort list不能使用STL的sort，应为他的迭代器是不是ramdon的
		//这里使用的是插入排序
		void sort() {
			if (size() == 0 || size() == 1)
				return;
			list<T, Alloc> tmp;
			iterator q = begin();
			while (!empty()) {
				iterator p = tmp.begin();
				while (p != tmp.end() && *p < *q)
					p++;
				tmp.splice(p, q);
				q = begin();
			}
			//将tmp赋给本list
			swap(tmp);
		}

	protected:
		link_type node;  //list是一个环状双向链表，一个指针即可表示整个环状双向链表，指向尾端的空白节点
		void empty_initialize() {
			node = get_node();
			node->next = node;
			node->prev = node;
		}
	};
}
