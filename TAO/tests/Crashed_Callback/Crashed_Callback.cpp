//
// $Id$
//
#include "Crashed_Callback.h"

Crashed_Callback::Crashed_Callback (void)
{
}

CORBA::Boolean
Crashed_Callback::are_you_there (void)
{
  return 1;
}

void
Crashed_Callback::crash_now_please (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Crashed_Callback - crashing application\n"));
// OpenVMS seems to hang and not abort and dump core when abort() is called
// here. This needs further
#if defined (DEC_CXX) || defined (ACE_OPENVMS)
  ACE_OS::_exit();
#else
  ACE_OS::abort();
#endif

}

void
Crashed_Callback::test_oneway (void)
{
}
