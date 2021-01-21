#ifndef OSAPI_MUTEX_LINUX_H
#define OSAPI_MUTEX_LINUX_H


class Mutex : public MutexInterface
{
public:
    Mutex()
    {
    }

    virtual ~Mutex()
    {
    }

    virtual bool lock(unsigned int timeout)
    {
        return false;
    }

    virtual void unlock()
    {
    }
private:

};


#endif // OSAPI_MUTEX_LINUX_H

