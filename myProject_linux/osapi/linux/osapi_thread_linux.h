#ifndef OSAPI_THREAD_LINUX_H
#define OSAPI_THREAD_LINUX_H

// Pls don't delete
// #include "osapi_thread_interface.h"
// #include <pthread.h>
// #include <bits/local_lim.h>
// #include <stdio.h>
// #include <unistd.h>
// #include <signal.h>
// #include "string.h"

#define SCHED_POLICY    SCHED_FIFO

/** Thread interface implementation for Linux. */
class Thread : public ThreadInterface
{
    public:
    
        /** Thread constructor.
         *  @param[in] priority thread priority
         *  @param[in] stackSize thread stack size in bytes
         *  @param[in] isJoinable decides if the thread supports join operation or not
         *  @param[in] name optional thread name
         */
        Thread(int priority, unsigned int stackSize, Joinable joinable, const char* name = "unnamed")
            : _joinable(joinable), _name(name), _validId(false), _errSuspendResume(false)
        {
            pthread_attr_init(&_attr);

            pthread_attr_setschedpolicy(&_attr, SCHED_POLICY);

            struct sched_param schedPrio =  { .sched_priority = priority };
            pthread_attr_setschedparam(&_attr, &schedPrio);

            if (stackSize <= PTHREAD_STACK_MIN)
            {
                stackSize = PTHREAD_STACK_MIN;
            }
            pthread_attr_setstacksize(&_attr, stackSize);

        }
        
        /** Virtual destructor required to properly destroy derived class objects. */
        virtual ~Thread()
        { 
        }
        
        /** Runs the thread.
         *  @retval true if the thread was started successfully, 
         *  @retval false if the thread was not started successfully, or the thread was already running
         */
        virtual bool run()
        { 
            if (!isRunning())
            {
                int retVal = pthread_create(&_threadId, &_attr, &threadFunction, this);
                if (0 == retVal)
                {
                    _validId = true;
                    return true;
                }
            }
            return false;
        }
        
        /** Checks if the thread is running.
         *  @retval true if the thread is running
         *  @retval false if the thread is not running
         */
        virtual bool isRunning()
        { 
            return _validId;
        }    

        /** Waits for the thread to finish executing, with a given timeout.
         *  @param timeout[in] number of milliseconds to wait for the thread to finish executing
         *  @retval true if the thread was successfully joined in the given time
         *  @retval false if the thread was not joined within the given time or the thread is not joinable at all
         */
        virtual bool join(unsigned int timeout)
        {
            if (isJoinable())
            {

                struct timespec ts;
                calcDelayedTimespec(&ts, timeout);

                int join_status = pthread_timedjoin_np(_threadId, NULL, &ts);

                if (0 == join_status)
                {
                    return true;
                }
            }
            return false;
        }

        /** Checks, if the thread is joinable.
         *  @retval true if the thread is joinable
         *  @retval false if the thread is not joinable
         */
        virtual bool isJoinable()
        {
            return JOINABLE == _joinable;
        }

        /** Suspends thread execution.
         *  @retval true if the thread was suspended successfully
         *  @retval false if the thread was not suspended for some reason
         */
        virtual bool suspend()
        {
            if (false == _errSuspendResume)
            {
                union sigval sigData;
                sigData.sival_ptr = this;
                if (0 == pthread_sigqueue(_threadId, SIGUSR1, sigData))
                {
                    return true;
                }
            }
            return false;
        }

        /** Resumes thread execution.
         *  @retval true if the thread was resumed successfully
         *  @retval false if the thread was not resumed for some reason
         */
        virtual bool resume()
        {
            if (false == _errSuspendResume)
            {
                pthread_mutex_lock(&_condMutex);
                pthread_cond_signal(&_cond);
                pthread_mutex_unlock(&_condMutex);
                return true;
            }
            return false;
        }

        /** Sets thread priority
         *  @param[in] priority new thread priority
         *  @retval true if the priority for the thread was set successfully
         *  @retval false if the priority for the thread was not set successfully for some reason
         */
        virtual bool setPriority(int priority)
        {
            int policy = SCHED_POLICY;
            int minPrio = sched_get_priority_min(policy);
            int maxPrio = sched_get_priority_max(policy);

            if (priority < minPrio || priority > maxPrio)
                return false;

            if (isRunning())
            {
                struct sched_param schedPrio = { .sched_priority = priority };
                int s = pthread_setschedparam(_threadId, policy, &schedPrio);
                if (0 == s)
                {
                    return true;
                }
            }
            else
            {
                struct sched_param schedPrio =  { .sched_priority = priority };
                pthread_attr_setschedparam(&_attr, &schedPrio);
                return true;
            }

            return false;
        }

        /** Gets the thread priority
         *  @return current thread priority
         */
        virtual int getPriority()
        {
            struct sched_param schedPrio;

            if (isRunning())
            {
                int policy;
                int s = pthread_getschedparam(_threadId, &policy, &schedPrio);
            }
            else
            {
                pthread_attr_getschedparam(&_attr, &schedPrio);
            }

            return schedPrio.sched_priority;
        }
        
        /** Gets thread name
         *  @return name of the thread
         */
        virtual const char* getName()
        {
            return _name;
        }

        static void* threadFunction(void *arg)
        {
            Thread* osapiThreadObject = reinterpret_cast<Thread*>(arg);
            if (osapiThreadObject)
            {
                struct sigaction sa;
                sigemptyset(&sa.sa_mask);
                sa.sa_sigaction = &suspendHandler;
                sa.sa_flags = SA_SIGINFO;

                if (0 != sigaction(SIGUSR1, &sa, NULL))
                {
                    osapiThreadObject->_errSuspendResume = true;
                }
                osapiThreadObject->body();
                osapiThreadObject->_validId = false;
            }
            return NULL;
        }

        static void suspendHandler(int signo, siginfo_t *si, void *ucontext)
        {
            Thread *osapiThreadObject = reinterpret_cast<Thread*>(si->si_value.sival_ptr);
            if (osapiThreadObject)
            {
                pthread_mutex_lock(&(osapiThreadObject->_condMutex));
                pthread_cond_wait(&(osapiThreadObject->_cond), &(osapiThreadObject->_condMutex));
                pthread_mutex_unlock(&(osapiThreadObject->_condMutex));
            }
        }
    
    protected:
        
        /** Delays thread execution for a given time.
         *  @param time[in] number of milliseconds to delay thread execution
         */
        virtual void delay(unsigned int time)
        {
             usleep(time*1000);
        }

        /** Causes the thread to temporarily yield execution in favor of other threads.
         */
        virtual void yield()
        {
            // TODO
        }
    private:
        pthread_t _threadId;
        pthread_attr_t _attr;
        pthread_mutex_t _condMutex = PTHREAD_MUTEX_INITIALIZER;
        pthread_cond_t _cond = PTHREAD_COND_INITIALIZER;
        Joinable _joinable;
        const char* _name;
        bool _validId;
        bool _errSuspendResume;
};


#endif /* OSAPI_THREAD_LINUX_H */

