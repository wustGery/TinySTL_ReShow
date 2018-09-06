#include "stdafx.h"
#include<iostream>
#include"ListTest.h"

namespace TinySTL
{
	namespace ListTest
	{
		void testCase1()
		{
			tsLst<int>tsList;
			tsList.push_front(1);
			tsList.push_front(2);
			tsList.push_front(3);
			std::cout << "tsList大小为"<<tsList.size() << std::endl;
			tsList.insert(tsList.end(), 100);
			std::cout << "tsList大小为" << tsList.size() << std::endl;
			tsList.bianli(tsList.begin(), tsList.end());
			tsList.sort();
			tsList.bianli(tsList.begin(), tsList.end());
		}

		void testCase2()
		{

		}

		void testAllCases()
		{
			testCase1();
			//testCase2();
		}
	}
}