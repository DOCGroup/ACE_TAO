#include "ace/TMCast/TransactionController.hpp"
#include "ace/Log_Msg.h"
#include "ace/OS_main.h"

#include <cstring>

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  char const payload[] = "message";
  ACE_TMCast::MessagePtr message (new ACE_TMCast::Send (payload, sizeof payload));
  ACE_TMCast::SendPtr send = ACE_TMCast::SendPtr::dynamic_pointer_cast (message);
  ACE_TMCast::RecvPtr const recv = ACE_TMCast::RecvPtr::dynamic_pointer_cast (message);
  if (send.null () || !recv.null ())
    {
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Message cast returned an unexpected type\n")), 1);
    }

  message.reset ();
  if (send->size () != sizeof payload
      || std::memcmp (send->payload (), payload, sizeof payload) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Message cast did not retain the payload\n")), 1);
    }
  return 0;
}
