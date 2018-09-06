

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
		typedef _list_iterator<T, T&, T*>   iterator; //ָ���ڲ�Ԫ��ֵ�õ�����
		typedef _list_iterator<T, Ref, Ptr> self; //ָ��list�ڵ�ĵ�����
		typedef bidirectional_iterator_tag iterator_category;
		typedef T   value_type;
		typedef Ptr pointer;
		typedef Ref reference;
		typedef _list_node<T>* link_type;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;

		link_type node; //��ָͨ��ָ��ڵ�
		_list_iterator(link_type x) : node(x) {}
		_list_iterator() {}
		_list_iterator(const iterator& x) : node(x.node) {}
		bool operator==(const self& x) const { return node == x.node; }
		bool operator!=(const self& x) const { return node != x.node; }

		//deference
		reference operator*() const { return (*node).data; }
		pointer   operator->() const { return &(operator*()); } //�������ĳ�Ա��ȡ��member access�������ӵı�׼����

		//��������ǰ�ƶ�һ��λ��(++i) ǰ��++
		self& operator++() {
			node = node->next;
			return *this;
		}

		//��������ǰ�ƶ�һ��λ��(i++)  ����++
		self operator++(int) {
			self tmp = *this;
			++*this;
			return tmp;
		}

		//�Ե������ݼ�1 (--i)
		self& operator--() {
			node = node->prev;
			return *this;
		}

		//�Ե������ݼ�1 (i--)
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

		//����һ���ڵ㲢����
		link_type get_node() {
			return listNode_Allocator::allocate();
		}

		//�ͷ�һ���ڵ�
		void put_node(link_type x) {
			listNode_Allocator::deallocate(x);
		}

		//����һ���ڵ㣬���Ҵ���Ԫ��ֵ
		link_type create_node(const T& x) {
			link_type newListNode = get_node();
			construct(&newListNode->data, x); //��ָ�����ݣ�����
			return newListNode;
		}

		//����һ��Ԫ�ص� �������ͷ�
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

		//���캯��
		list() { empty_initialize(); } //�����յ�����
		~list() {
			clear();
			erase(end());
		}
		//β������ڵ�
		void push_back(const T& x) { //��listβ����Ԫ�ش��������Ԫ��
			insert(end(), x);
		}

		//ͷ������ڵ�
		void push_front(const T& x) {
			insert(begin(), x);
		}

		//ɾ��ͷ���ڵ�
		void pop_front() {
			erase(begin());
		}

		//ɾ��β���ڵ�
		void pop_back() {
			erase(--end());
		}

		//�������list�ڵ�
		void clear() {
			iterator s = begin();
			while (s != end()) {
				s = erase(s);
			}
			//��list��״̬
			node->next = node;
			node->prev = node;
		}

		//ɾ������ֵΪx�Ľڵ�
		void remove(const T& x) {
			iterator s = begin();
			while (s != end()) {
				if (*s == x)
					s = erase(s);
				else
					s++;
			}
		}

		//�Ƴ���������ͬ��Ԫ��
		void unique() {
			iterator first = begin();
			iterator last = end();
			if (first == end) return;  //������
			iterator next = first;
			while (++next != last) {
				if (*first == *next)
					erase(next);
				else
					first = next;
				next = first;
			}
		}


		// ɾ��positionλ�õĽڵ�
		iterator erase(iterator position) {
			link_type next_node = position.node->next;
			link_type prev_node = position.node->prev;
			prev_node->next = next_node;
			destroy_node(position.node);
			return iterator(next_node);
		}

		//��positionλ��֮ǰ����һ���ڵ�
		iterator insert(iterator position, const T& x) 
		{
			link_type tmp = create_node(x);
			tmp->next = position.node;
			tmp->prev = position.node->prev;
			position.node->prev->next = tmp;
			position.node->prev = tmp;
			return tmp;
		}

		//��[first,last)֮���Ԫ���ƶ���position֮ǰ
		void transfer(iterator position, iterator first, iterator last) {
			if (last == position)
				return;

			link_type last_node = last.node->prev;
			//��first lastȡ����
			first.node->prev->next = last.node;
			last.node->prev = first.node->prev;


			link_type prev_node = position.node->prev;
			prev_node->next = first.node;
			first.node->prev = prev_node;

			last_node->next = position.node;
			position.node->prev = last_node;
		}

		//x�Ӻ���position֮ǰ
		void splice(iterator position, list& x) {
			if (x.empty())
				return;
			transfer(position, x.begin(), x.end());
		}

		//��ĳһ��λ���ϵ�Ԫ�ؽӺϵ�position֮ǰ
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
				std::cout << "��" << index++ << "��Ԫ��Ϊ" << *first << std::endl;
				++first;
			}
		}

		//
		void splice(iterator position, iterator first, iterator last) {
			if (position == last)
				return;
			transfer(position, first, last);
		}

		//merge ��x ��*this�ϲ�������list���뾭����������
		void merge(list& x) {
			iterator p = begin();
			iterator x_start = x.begin();

			while (p != end() && x_start != x.end()) {
				std::cout << *p << std::endl;
				if (*x_start > *p)
					p++;
				else {
					iterator next = x_start + 1; //ע�⣺���������Žڵ��ܣ�������ԭ����list
					splice(p, x_start);
					x_start = next;
				}
			}

			//���xû�����ӵ�β����
			if (!x.empty())
				splice(end(), x_start, x.end());
		}

		//reverse�ߵ�˳��
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

		//��x������Ա ������ɺ�ԭ������list�ϵĵ�����Ҫע��
		void swap(list& x) {
			link_type tmp = x.node;
			x.node = this->node;
			this->node = tmp;
		}



		//sort list����ʹ��STL��sort��ӦΪ���ĵ������ǲ���ramdon��
		//����ʹ�õ��ǲ�������
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
			//��tmp������list
			swap(tmp);
		}

	protected:
		link_type node;  //list��һ����״˫������һ��ָ�뼴�ɱ�ʾ������״˫������ָ��β�˵Ŀհ׽ڵ�
		void empty_initialize() {
			node = get_node();
			node->next = node;
			node->prev = node;
		}
	};
}
