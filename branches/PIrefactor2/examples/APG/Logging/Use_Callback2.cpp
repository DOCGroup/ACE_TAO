// $Id$

#include "ace/Log_Msg.h"
#include "Callback-2.h"

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  Callback *callback = new Callback;

  ACE_LOG_MSG->set_flags (ACE_Log_Msg::MSG_CALLBACK);
  ACE_LOG_MSG->clr_flags (ACE_Log_Msg::STDERR);
  ACE_LOG_MSG->msg_callback (callback);

  ACE_TRACE ("main");

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%IHi Mom\n")));
  ACE_DEBUG ((LM_INFO, ACE_TEXT ("%IGoodnight\n")));

  return 0;
}
