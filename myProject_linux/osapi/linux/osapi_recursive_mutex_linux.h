#ifndef OSAPI_RECURSIVE_MUTEX_LINUX_H
#define OSAPI_RECURSIVE_MUTEX_LINUX_H

class RecursiveMutex : public MutexInterface
{
public:
	RecursiveMutex()
    {
	}

	virtual ~RecursiveMutex()
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


#endif // OSAPI_RECURSIVE_MUTEX_LINUX_H

