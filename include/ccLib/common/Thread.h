#ifndef THREAD_H
#define THREAD_H

#include "Common.h"

#include <pthread.h>

namespace CCPlayer
{

class CCThread
{
public:
    CCThread();
    virtual ~CCThread();

public:
	int Launch();

public:
	virtual void Run() = 0;

protected:
    bool m_bRunning;

private:
	pthread_t m_threadId;

	//declare the Call back function
public:
    static void* ThreadFunc(void *pUserData);
};

} // end namespace CCPlayer

#endif // THREAD_H
