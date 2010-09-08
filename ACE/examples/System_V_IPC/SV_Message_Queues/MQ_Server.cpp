// $Id$

#include "ace/Signal.h"
#include "ace/SV_Message_Queue.h"

// FUZZ: disable check_for_streams_include
#include "ace/streams.h"

#include "test.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_stdlib.h"



#if defined (ACE_HAS_SYSV_IPC) && !defined(ACE_LACKS_SYSV_SHMEM)

// Must be global for signal Message...
static ACE_SV_Message_Queue ace_sv_message_queue (SRV_KEY,
                                                  ACE_SV_Message_Queue::ACE_CREATE);

extern "C" void
handler (int)
{
  if (ace_sv_message_queue.remove () < 0)
    ACE_OS::perror ("ace_sv_message_queue.close"), ACE_OS::exit (1);
  ACE_OS::exit (0);
}

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  long pid = long (ACE_OS::getpid ());
  Message_Block recv_msg (SRV_ID);
  Message_Block send_msg (0,
                          pid,
                          ACE_OS::cuserid (static_cast<char *> (0)),
                          "I received your message.");

  // Register a signal handler.
  ACE_Sig_Action sa ((ACE_SignalHandler) handler, SIGINT);
  ACE_UNUSED_ARG (sa);

  for (;;)
    {
      if (ace_sv_message_queue.recv (recv_msg,
                                     sizeof (Message_Data),
                                     recv_msg.type ()) == -1)
        ACE_OS::perror ("ace_sv_message_queue.recv"), ACE_OS::exit (1);

      cout << "a msg of length "
           << recv_msg.length ()
           << " sent from client "
           << recv_msg.pid ()
           << " (user "
           << recv_msg.user () << "): "
           << recv_msg.text () << "\n";

      cout.flush ();

      send_msg.type (recv_msg.pid ());

      if (ace_sv_message_queue.send (send_msg,
                                     send_msg.length ()) == -1)
        ACE_OS::perror ("ace_sv_message_queue.send"), ACE_OS::exit (1);
    }

  ACE_NOTREACHED (return 0;)
}

#else

#include "ace/Log_Msg.h"

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_ERROR ((LM_ERROR,
              "SYSV IPC, or SYSV SHMEM is not supported on this platform\n"));
  return 0;
}
#endif /* ACE_HAS_SYSV_IPC && !ACE_LACKS_SYSV_SHMEM */
