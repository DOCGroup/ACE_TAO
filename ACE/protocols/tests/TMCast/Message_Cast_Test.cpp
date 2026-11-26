#include "ace/TMCast/TransactionController.hpp"
#include "ace/TMCast/LinkListener.hpp"
#include "ace/Log_Msg.h"
#include "ace/OS_main.h"

#include <cstring>
#include <memory>

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

  ACE_TMCast::Protocol::MessageHeader header {};
  header.length = sizeof header + sizeof payload;
  header.current.id = 42;
  alignas (ACE_TMCast::Protocol::MessageHeader) char unaligned[sizeof header + 1];
  std::memcpy (unaligned + 1, std::addressof (header), sizeof header);
  ACE_TMCast::LinkData const data (unaligned + 1, payload, sizeof payload);
  if (data.header ().length != header.length
      || data.header ().current.id != header.current.id
      || data.size () != sizeof payload
      || std::memcmp (data.payload (), payload, sizeof payload) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Link data did not preserve the packet\n")), 1);
    }
  return 0;
}
