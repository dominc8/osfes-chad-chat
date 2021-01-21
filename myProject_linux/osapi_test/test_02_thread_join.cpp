#include "CUnit/CUnitCI.h"
#include "osapi.h"

static volatile int testFlag;
static volatile int runFlag;

class TestThread2 : public osapi::Thread
{
public:
    TestThread2() : Thread(3, 512, osapi::JOINABLE) { run(); }
private:
    virtual void body() {
        while (runFlag) {
            testFlag = 1;
            delay(10);
        }
        for (int i = 0; i < 100; i++) {
            testFlag = 2;
            delay(20);
        }
        testFlag = 3;
    }
};

void TEST_ThreadJoin(void) {

    printf("Running test 2: thread join\n");

    // set initial flag values
    testFlag = 0;
    runFlag = 1;
    // create thread - it should begin execution
    TestThread2 t;
    // check that thread is running
    CU_ASSERT(true == t.isRunning());
    // check if thread is joinable
    CU_ASSERT(true == t.isJoinable());    
    // test thread operation - after 500ms the testFlag should already change to 1
    usleep(500000);
    CU_ASSERT(1 == testFlag);
    // send termination signal
    runFlag = 0;
    // join thread
    CU_ASSERT(true == t.join(5000));
    // test result
    CU_ASSERT(3 == testFlag);
    // test if the thread is not running anymore
    testFlag = 4;
    usleep(500000);
    CU_ASSERT(4 == testFlag);
}
