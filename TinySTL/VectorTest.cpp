#include "stdafx.h"
#include "VectorTest.h"
#include<iostream>

namespace TinySTL
{
	namespace VectorTest
	{
		void testCase1()
		{
			tsVec<int>tsVector(10,1);
			std::cout << "vector分配空间的总容量: "<< tsVector.capacity() << std::endl;
			std::cout << "vector元素总个数: "<< tsVector.size() << std::endl;
			std::cout << "vector首元素: "<< *tsVector.begin() << std::endl;
			std::cout << "vector第6个元素: " << tsVector[5]<< std::endl;
			std::cout << "vector首元素: " << tsVector.front() << std::endl;
			std::cout << "vector尾元素: " << tsVector.back() << std::endl;
			//弹出一个元素
			tsVector.pop_back();
			std::cout << "tsVector容量为"<<tsVector.capacity() << std::endl;
			std::cout << "tsVector大小为"<<tsVector.size() << std::endl;
			tsVector.erase(tsVector.begin());
			//erase以后的元素个数
			std::cout << tsVector.size() << std::endl;
			//对vector进行遍历
			tsVector[5] = 2;
			for (size_t i = 0; i < tsVector.size(); i++) tsVector[i] = i;
			tsVector.erase(tsVector.begin() + 2, tsVector.begin() + 4);
			for (size_t i = 0; i < tsVector.size(); i++) std::cout << tsVector[i] << std::ends;
			std::cout << std::endl;
			std::cout << "---------------------" << std::endl;
			/*tsVector.push_back(10);
			for (size_t i = 0; i < tsVector.size(); i++) std::cout << tsVector[i] << std::ends;
			std::cout << endl;*/
		}

		void testCase2()
		{
			tsVec<int>tsVector1;
			for (size_t i = 0; i < 10; i++) tsVector1.push_back(i);
			vector<int>::iterator iter = tsVector1.begin();
			for (; iter != tsVector1.end(); iter++) std::cout << *iter << " ";
			std::cout << std::endl;
			tsVector1.insert(tsVector1.begin() + 2, 123243);
			for (size_t i = 0; i < tsVector1.size(); i++)  std::cout << tsVector1[i] << std::ends;
			std::cout << std::endl;
			//std::cout << 2 << std::endl;
		}


		void testAllCases()
		{
			testCase1();
			testCase2();
		/*	testCase3();
			testCase4();
			testCase5();
			testCase6();
			testCase7();
			testCase8();
			testCase9();
			testCase10();
			testCase11();
			testCase12();
			testCase13();
			testCase14();
			testCase15();*/
		}
	}
}
