#ifndef TESTTOOLLIB_H
#define TESTTOOLLIB_H

#include "testlib_global.h"

#include <thread>

using namespace std;
class TESTLIBSHARED_EXPORT TestToolLib
{
public:
	TestToolLib();

private:
	thread _th;
};

#endif // TESTTOOLLIB_H
