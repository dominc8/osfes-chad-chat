#include "CUnit/CUnitCI.h"
#include "osapi.h"

static volatile int testFlagBegin;
static volatile int testFlagLoop;
static volatile int testFlagEnd;

class MyMortalThread : public osapi::MortalThread
{
public:
	MyMortalThread() : MortalThread(3, 512) { 
	; 
	}
private:
	virtual void begin() 
	{
		testFlagBegin = 1;
	}
	virtual void loop() 
	{
		testFlagLoop = 1;
		delay(10);
	}
	virtual void end()
	{
		testFlagEnd = 1;
	}	
};

void TEST_MortalThread(void)
{	
    printf("Running test 8: mortal thread                    ");
		
	  // set initial test flag value
	  testFlagBegin = 0;
	  testFlagLoop = 0;
	  testFlagEnd = 0;
	  // create thread
	  MyMortalThread t;
	  // run the thread
	  CU_ASSERT(true == t.run());
	  // wait a while
	  usleep(1000000);
	  // test that the thread went through begin() and loop() but not through end()
	  CU_ASSERT(1 == testFlagBegin);
	  CU_ASSERT(1 == testFlagLoop);
	  CU_ASSERT(0 == testFlagEnd);
	  // send termination signal
	  t.kill();
	  // wait for the thread to finish
	  CU_ASSERT(true == t.join(1000));
	  // check that the thread went through the end() method
	  CU_ASSERT(1 == testFlagEnd);
}
