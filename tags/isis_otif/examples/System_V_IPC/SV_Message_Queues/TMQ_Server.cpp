// $Id$

#include "ace/Signal.h"
#include "ace/Typed_SV_Message_Queue.h"

#include "test.h"
#include "ace/streams.h"
#include "ace/Log_Msg.h"

ACE_RCSID(SV_Message_Queues, TMQ_Server, "$Id$")

#if defined (ACE_HAS_SYSV_IPC) && !defined(ACE_LACKS_SYSV_SHMEM)

// Must be global for signal Message...
static ACE_Typed_SV_Message_Queue<Message_Data> ace_sv_message_queue
  (SRV_KEY, ACE_Typed_SV_Message_Queue<Message_Data>::ACE_CREATE);

extern "C" void
handler (int)
{
  if (ace_sv_message_queue.remove () < 0)
    ACE_ERROR ((LM_ERROR, "%p\n%a", "ace_sv_message_queue.recv", 1));
  ACE_OS::exit (0);
}

int
main (int, char *[])
{
  char *username = ACE_OS::cuserid (ACE_static_cast (char *, 0));
  Message_Data msg_data ((int) ACE_OS::getpid (), username, "I received your message.");
  ACE_Typed_SV_Message<Message_Data> send_msg (msg_data, 0, msg_data.length ());
  ACE_Typed_SV_Message<Message_Data> recv_msg (SRV_ID);

  // Register a signal handler.
  ACE_Sig_Action sa ((ACE_SignalHandler) handler, SIGINT);
  ACE_UNUSED_ARG (sa);

  for (;;)
    {
      if (ace_sv_message_queue.recv (recv_msg) == -1)
        ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "ace_sv_message_queue.recv"), 1);

      Message_Data &recv_msg_data = recv_msg.data ();

      cout << "a msg of length "
           << recv_msg_data.length ()
           << " sent from client "
           << recv_msg_data.pid ()
           << " (user "
           << recv_msg_data.user () << "): "
           << recv_msg_data.text () << "\n";
      cout.flush ();

      send_msg.type (recv_msg_data.pid ());

      if (ace_sv_message_queue.send (send_msg) < 0)
        ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "ace_sv_message_queue.send"), 1);
    }

  ACE_NOTREACHED (return 0;)
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Typed_SV_Message_Queue<Message_Data>;
template class ACE_Typed_SV_Message<Message_Data>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Typed_SV_Message_Queue<Message_Data>
#pragma instantiate ACE_Typed_SV_Message<Message_Data>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#else

int main (int, char *[])
{
  ACE_ERROR ((LM_ERROR,
              "SYSV IPC, or SYSV SHMEM is not supported on this platform\n"));
  return 0;
}
#endif /* ACE_HAS_SYSV_IPC && !ACE_LACKS_SYSV_SHMEM */

