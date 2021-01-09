#include "CUnit/CUnitCI.h"
#include "osapi.h"

static osapi::Mutex m;

void TEST_Mutex(void) {

	printf("Running test 6: mutex\n");

	for (int i = 0; i < 3; i++) {
			// lock mutex
			CU_ASSERT(true == m.lock(100));
			// another lock should fail
			CU_ASSERT(false == m.lock(100));
			// check unlock
			m.unlock();
	}
}
