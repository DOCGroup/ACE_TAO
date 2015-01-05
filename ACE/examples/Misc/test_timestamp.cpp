// $Id$

// Test the ACE::timestamp method.

#include "ace/OS_main.h"
#include "ace/ACE.h"
#include "ace/Log_Msg.h"



int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_TCHAR day_and_time[27];
  ACE::timestamp (day_and_time,
                  sizeof day_and_time);

  ACE_DEBUG ((LM_DEBUG, "%s", day_and_time));

  return 0;
}
