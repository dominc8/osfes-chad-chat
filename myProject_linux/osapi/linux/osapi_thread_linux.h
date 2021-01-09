#ifndef OSAPI_THREAD_LINUX_H
#define OSAPI_THREAD_LINUX_H

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
		{
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
// 			if (!isRunning())
// 			{
// 				_threadHandle = CreateThread(NULL, _stackSize, &threadFunction, this, 0, nullptr);
// 				if (NULL != _threadHandle)
// 				{
// 					setPriority(_priority);
// 					return true;
// 				}
// 			}
			return false;
		}
		
		/** Checks if the thread is running.
		 *  @retval true if the thread is running
         *  @retval false if the thread is not running
		 */
		virtual bool isRunning()
        { 
// 			if (_threadHandle != NULL)
// 			{
// 				return true;
// 			}
			return false; 
		}	

		/** Waits for the thread to finish executing, with a given timeout.
         *  @param timeout[in] number of milliseconds to wait for the thread to finish executing
		 *  @retval true if the thread was successfully joined in the given time
		 *  @retval false if the thread was not joined within the given time or the thread is not joinable at all
		 */
		virtual bool join(unsigned int timeout)
        {
// 			if (isJoinable())
// 			{
// 				DWORD retVal = WaitForSingleObject(_threadHandle, timeout);
// 				if (WAIT_OBJECT_0 == retVal)
// 				{
// 					return true;
// 				}
// 
// 			}
			return false;
		}

		/** Checks, if the thread is joinable.
		 *  @retval true if the thread is joinable
		 *  @retval false if the thread is not joinable
		 */
		virtual bool isJoinable()
        {
// 			return JOINABLE == _joinable;
            return false;
		}

		/** Suspends thread execution.
		 *  @retval true if the thread was suspended successfully
		 *  @retval false if the thread was not suspended for some reason
		 */
		virtual bool suspend()
        {
// 			DWORD retVal = SuspendThread(_threadHandle);
// 			if (static_cast<DWORD>(-1) == retVal)
// 			{
// 				return false;
// 			}
// 			return true;
			return false;
		}

		/** Resumes thread execution.
		 *  @retval true if the thread was resumed successfully
		 *  @retval false if the thread was not resumed for some reason
		 */
		virtual bool resume()
        {
// 			DWORD retVal = ResumeThread(_threadHandle);
// 			if (static_cast<DWORD>(-1) == retVal)
// 			{
// 				return false;
// 			}
// 			return true;
			return false;
		}

		/** Sets thread priority
		 *  @param[in] priority new thread priority
		 *  @retval true if the priority for the thread was set successfully
		 *  @retval false if the priority for the thread was not set successfully for some reason
		 */
		virtual	bool setPriority(int priority)
        {
// 			if (priority < 0)
// 			{
// 				return false;
// 			}
// 			else
// 			{
// 				if (SetThreadPriority(_threadHandle, priority))
// 				{
// 					_priority = priority;
// 					return true;
// 				}
// 				return false;
// 			}
			return false;
		}

		/** Gets the thread priority
		 *  @return current thread priority
		 */
		virtual	int getPriority()
        {
// 			return GetThreadPriority(_threadHandle);
            return 0;
        }
        
		/** Gets thread name
		 *  @return name of the thread
		 */
		virtual const char* getName()
        {
// 			return _name;
            return "";
		}

// 		static DWORD WINAPI threadFunction(LPVOID args)
// 		{
// 			Thread* osapiThreadObject = reinterpret_cast<Thread*>(args);
// 			if (osapiThreadObject)
// 			{
// 				osapiThreadObject->body();
// 			}
// 			return 0;
// 		}
	
	protected:
		
		/** Delays thread execution for a given time.
		 *  @param time[in] number of milliseconds to delay thread execution
		 */
		virtual void delay(unsigned int time)
        {
// 			Sleep(time);
		}

		/** Causes the thread to temporarily yield execution in favor of other threads.
		 */
		virtual void yield()
        {
			// TODO
		}
	private:
// 		HANDLE _threadHandle;
// 		int _priority;
// 		unsigned int _stackSize;
// 		Joinable _joinable;
// 		const char* _name;
};


#endif /* OSAPI_THREAD_LINUX_H */

