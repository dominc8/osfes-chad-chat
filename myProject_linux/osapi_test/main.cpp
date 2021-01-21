#include "CUnit/CUnitCI.h"

void TEST_ThreadCreation(void);
void TEST_ThreadJoin(void);
void TEST_ThreadSuspendResume(void);
void TEST_ThreadPriority(void);
void TEST_ThreadDelay(void);
void TEST_Mutex(void);
void TEST_RecursiveMutex(void);
void TEST_MortalThread(void);


CUNIT_CI_RUN("osapi",
        CUNIT_CI_TEST(TEST_ThreadCreation),
        CUNIT_CI_TEST(TEST_ThreadJoin),
        CUNIT_CI_TEST(TEST_ThreadSuspendResume),
        CUNIT_CI_TEST(TEST_ThreadPriority),
        CUNIT_CI_TEST(TEST_ThreadDelay),
        CUNIT_CI_TEST(TEST_Mutex),
        CUNIT_CI_TEST(TEST_RecursiveMutex),
        CUNIT_CI_TEST(TEST_MortalThread)
);
