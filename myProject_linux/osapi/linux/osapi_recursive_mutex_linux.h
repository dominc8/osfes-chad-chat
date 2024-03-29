#ifndef OSAPI_RECURSIVE_MUTEX_LINUX_H
#define OSAPI_RECURSIVE_MUTEX_LINUX_H

// #include "pthread.h"

class RecursiveMutex : public MutexInterface
{
public:
    RecursiveMutex()
    {
        pthread_mutexattr_init(&_mutexAttr);
        pthread_mutexattr_settype(&_mutexAttr, PTHREAD_MUTEX_RECURSIVE);
        pthread_mutex_init(&_mutexHandle, &_mutexAttr);
    }

    virtual ~RecursiveMutex()
    {
        pthread_mutexattr_destroy(&_mutexAttr);
        pthread_mutex_destroy(&_mutexHandle);
    }

    virtual bool lock(unsigned int timeout)
    {
        struct timespec ts;
        calcDelayedTimespec(&ts, timeout);

        int lockStatus = pthread_mutex_timedlock(&_mutexHandle, &ts);
        if (0 == lockStatus)
        {
            return true;
        }
        return false;
    }

    virtual void unlock()
    {
        pthread_mutex_unlock(&_mutexHandle);
    }
private:
    pthread_mutex_t _mutexHandle;
    pthread_mutexattr_t _mutexAttr;

};


#endif // OSAPI_RECURSIVE_MUTEX_LINUX_H

