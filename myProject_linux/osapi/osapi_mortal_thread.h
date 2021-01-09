#ifndef OSAPI_MORTAL_THREAD_H
#define OSAPI_MORTAL_THREAD_H
 
 
class MortalThread : public Thread
{
public:
    MortalThread(int priority, unsigned int stackSize, const char* name = "unnamed")
        : Thread(priority, stackSize, JOINABLE, name), killSignal{0}
    {
    }
 
    virtual ~MortalThread() {
    }                   
                        
    /** Sends termination signal to the thread. */
    void kill()
    {
        killSignal = 1;
    }
  
protected:
    
    virtual void begin(void)
    {
    }
        
    virtual void loop(void)
    {
    }
        
    virtual void end(void)
    {
    }
        
                        
private:
    /** Implementation of the body method */
    virtual void body(void) {
        begin();
        while (0 == killSignal)
        {
            loop();
        }
        end();
    }

    sig_atomic_t killSignal;
};
 
#endif // OSAPI_MORTAL_THREAD_H