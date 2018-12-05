/**
 * @file Echo_Callback.cpp
 *
 * @author Carlos O'Ryan <coryan@atdesk.com>
 */

#include "Echo_Callback.h"

Echo_Callback::Echo_Callback()
{
}

void
Echo_Callback::echo_payload(Test::Payload const &)
{
  static int n = 0;
  n++;
  if(n == 30)
    ACE_OS::abort();

  ACE_Time_Value tick(0, 20000);
  ACE_OS::sleep(tick);
}

void
Echo_Callback::echo_payload_excep(Test::AMI_EchoExceptionHolder *)
{
}
