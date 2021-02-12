#ifndef OSAPI_MUTEX_LINUX_H
#define OSAPI_MUTEX_LINUX_H

// #include "pthread.h"

class Mutex : public MutexInterface
{
public:
    Mutex()
    {
    }

    virtual ~Mutex()
    {
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
    pthread_mutex_t _mutexHandle = PTHREAD_MUTEX_INITIALIZER;

};


#endif // OSAPI_MUTEX_LINUX_H

