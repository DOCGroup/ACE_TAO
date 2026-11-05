#include "Callback_i.h"
#include "ace/Log_Msg.h"

void
Callback_i::test_method ()
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Callback_i::test_method called\n"));
}

