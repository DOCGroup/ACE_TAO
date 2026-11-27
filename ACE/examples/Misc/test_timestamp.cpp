// Test the ACE::timestamp method.

#include "ace/OS_main.h"
#include "ace/ACE.h"
#include "ace/Log_Msg.h"
#include <array>

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  std::array<ACE_TCHAR, 27> day_and_time {};
  ACE::timestamp (day_and_time.data (), day_and_time.size ());

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%s"), day_and_time.data ()));

  return 0;
}
