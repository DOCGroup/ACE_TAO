#include "ace/TMCast/TransactionController.hpp"
#include "ace/TMCast/LinkListener.hpp"
#include "ace/Log_Msg.h"
#include "ace/OS_main.h"

#include <cstring>
#include <memory>
#include <thread>

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
  if (!data.valid ()
      || data.header ().length != header.length
      || data.header ().current.id != header.current.id
      || data.size () != sizeof payload
      || std::memcmp (data.payload (), payload, sizeof payload) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Link data did not preserve the packet\n")), 1);
    }

  header.length = sizeof header;
  ACE_TMCast::LinkData const wrong_length (std::addressof (header), payload, sizeof payload);
  std::memset (header.member_id.id, 'x', sizeof header.member_id.id);
  header.length = sizeof header + sizeof payload;
  ACE_TMCast::LinkData const unterminated (std::addressof (header), payload, sizeof payload);
  if (wrong_length.valid () || unterminated.valid ())
    {
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Invalid link data was accepted\n")), 1);
    }

  ACE_Thread_Mutex mutex;
  ACE_TMCast::MessageQueue queue (mutex);
  {
    ACE_TMCast::MessageQueueAutoLock lock (queue);
    lock.unlock ();
    queue.lock ();
  }

  int acquire_result {};
  std::thread contender ([&mutex, &acquire_result] ()
    {
      acquire_result = mutex.tryacquire ();
      if (acquire_result == 0)
        {
          mutex.release ();
        }
    });
  contender.join ();
  queue.unlock ();
  if (acquire_result == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Queue guard unlocked a lock it no longer owned\n")), 1);
    }
  return 0;
}
