
//=============================================================================
/**
 *  @file    Native_Condition_Variable_Test.cpp
 *
 *    This program tests the support of Native Condition Variables on
 *    Microsoft Windows. It checks that errno is correctly mutated to
 *    ensure return and errno values are consistent across implementations
 *    using ACE_Condition.
 *
 *  @author Michael Mathers <michael.p.mathers@live.com>
 */
//=============================================================================


#include "test_config.h"
#include "ace/Condition_T.h"
#include "ace/Guard_T.h"


int
run_main(int, ACE_TCHAR *[])
{
    ACE_START_TEST(ACE_TEXT("Native_Condition_Variable_Test"));

    ACE_SYNCH_MUTEX mutex;
    ACE_Condition<ACE_SYNCH_MUTEX> cond(mutex);
    ACE_Time_Value timeout(1, 0);
    bool condition = false; // Condition we are waiting on - will never changed (never intentionally signalled)
    int result = 0;

    // Place the wait within a loop such that we don't just pass on account of a spurious wakeup
    while (!condition && !(result == -1))
    {
        ACE_Guard<ACE_SYNCH_MUTEX> guard(mutex);
        result = cond.wait(&timeout);
    }

    // We should've timed out waiting for the condition variable to be signalled
    // Expected behavior is to return -1 and have errno set to ETIME
    // Note that native Windows condition variable will set errno to ERROR_TIMEOUT instead (mutation required)
    ACE_TEST_ASSERT(result == -1);
    ACE_TEST_ASSERT(ACE_OS::last_error() == ETIME);

    ACE_END_TEST;
    return 0;
}
