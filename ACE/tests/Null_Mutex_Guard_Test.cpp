/**
 * @file Null_Mutex_Guard_Test.cpp
 *
 * @brief Checks const guards with ACE_Null_Mutex.
 */

#include "test_config.h"

#include "ace/Global_Macros.h"
#include "ace/Guard_T.h"
#include "ace/Null_Mutex.h"

int
run_main (int, ACE_TCHAR * [])
{
  ACE_START_TEST (ACE_TEXT ("Null_Mutex_Guard_Test"));

  ACE_Null_Mutex mutex;

  ACE_GUARD_RETURN_CONST (ACE_Null_Mutex, guard, mutex, -1);
  ACE_TEST_ASSERT (guard.locked () != 0);

  // Direct const declarations check the specialized guards' const locked ().
  // FUZZ: disable check_for_ACE_Guard
  const ACE_Read_Guard<ACE_Null_Mutex> read_guard (mutex);
  ACE_TEST_ASSERT (read_guard.locked () != 0);

  const ACE_Write_Guard<ACE_Null_Mutex> write_guard (mutex);
  ACE_TEST_ASSERT (write_guard.locked () != 0);
  // FUZZ: enable check_for_ACE_Guard

  ACE_END_TEST;
  return 0;
}
