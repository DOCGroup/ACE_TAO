//
// $Id$
//
#include "Crashed_Callback.h"

ACE_RCSID(Crashed_Callback, Crashed_Callback, "$Id$")

Crashed_Callback::Crashed_Callback (void)
{
}

CORBA::Boolean
Crashed_Callback::are_you_there (CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return 1;
}

void
Crashed_Callback::crash_now_please (CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Crashed_Callback - crashing application %a\n"));
}

void
Crashed_Callback::test_oneway (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}
