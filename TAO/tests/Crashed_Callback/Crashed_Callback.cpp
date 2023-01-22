#include "Crashed_Callback.h"

Crashed_Callback::Crashed_Callback ()
{
}

CORBA::Boolean
Crashed_Callback::are_you_there ()
{
  return 1;
}

void
Crashed_Callback::crash_now_please ()
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Crashed_Callback - crashing application\n"));
  ACE_OS::abort();
}

void
Crashed_Callback::test_oneway ()
{
}
