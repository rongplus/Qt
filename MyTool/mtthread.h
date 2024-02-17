#ifndef MTTHREAD_H
#define MTTHREAD_H

#include <QDebug>
#include <thread>

using namespace std;
#include "mainwindow.h"

class MtThread
{
public:
	MtThread();
	MainWindow* pa;

	static void thread_function();

	static void call_from_thread(int tid,MainWindow* p);

	int thread1();

	int thread2();
};

#endif // MTTHREAD_H
