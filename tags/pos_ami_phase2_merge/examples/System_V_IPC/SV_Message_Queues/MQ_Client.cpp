// $Id$

#include "ace/SV_Message_Queue.h"
#include "test.h"
#include "ace/streams.h"

ACE_RCSID(SV_Message_Queues, MQ_Client, "$Id$")

int 
main (int, char *[])
{
  long pid = long (ACE_OS::getpid ());
  ACE_SV_Message_Queue msgque (SRV_KEY);
  Message_Block send_msg (SRV_ID,
                          pid,
                          ACE_OS::cuserid (0),
                          "did you get this?");
  Message_Block recv_msg (pid);

  if (msgque.send (send_msg, send_msg.length ()) < 0)
    ACE_OS::perror ("msgque.send"), ACE_OS::exit (1);

  if (msgque.recv (recv_msg, sizeof (Message_Data), recv_msg.type ()) < 0)
    ACE_OS::perror ("msgrcv"), ACE_OS::exit (1);

  cout << "a message of length " 
       << recv_msg.length ()
       << " received from server " 
       << recv_msg.pid () 
       << " (user " 
       << recv_msg.user () << "): " 
       << recv_msg.text () << "\n";

  return 0;
}
