// $Id$

#include "ace/Signal.h"
#include "ace/SV_Message_Queue.h"
#include "ace/streams.h"
#include "test.h"

ACE_RCSID(SV_Message_Queues, MQ_Server, "$Id$")

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
main (int, char *[]) 
{
  long pid = long (ACE_OS::getpid ());
  Message_Block recv_msg (SRV_ID);
  Message_Block send_msg (0,
                          pid,
                          ACE_OS::cuserid (0), 
                          "I received your message.");

  // Register a signal handler.
  ACE_Sig_Action sa ((ACE_SignalHandler) handler, SIGINT);
  ACE_UNUSED_ARG (sa);

  for (;;)
    {
      if (ace_sv_message_queue.recv (recv_msg, 
                                     sizeof (Message_Data),
                                     recv_msg.type ()) == -1)
        ::perror ("ace_sv_message_queue.recv"), ACE_OS::exit (1);
      
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

  /* NOTREACHED */
  return 0;
}
