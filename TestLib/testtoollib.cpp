#include "testtoollib.h"
#include <QDebug>
#include <list>
#include <mutex>
#include <algorithm>

std::list<int> some_list;    // 1
std::mutex some_mutex;    // 2
std::once_flag connection_init_flag;
void method2()
{
	qDebug() << "call once";
}

void method1()
{
	qDebug() << "thread runing";

	std::lock_guard<std::mutex> guard(some_mutex);
	  some_list.push_back(5);
}

TestToolLib::TestToolLib()
{
	int a = 0;
	a += a+1;
	int b = 5;
	b = b+a;

	_th = thread(&method1);
	_th.join();

	std::call_once(connection_init_flag,method2); // 可以完整的进行一次初始化
}
