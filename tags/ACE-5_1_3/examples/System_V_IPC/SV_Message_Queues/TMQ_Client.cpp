// $Id$

#include "ace/Typed_SV_Message_Queue.h"
#include "ace/streams.h"
#include "test.h"

ACE_RCSID(SV_Message_Queues, TMQ_Client, "$Id$")

int
main (int, char *[])
{
  long pid = long (ACE_OS::getpid ());

  ACE_Typed_SV_Message_Queue<Message_Data> msgque (key_t (SRV_KEY));

  Message_Data msg_data (pid, ACE_OS::cuserid (0), "did you get this?");

  ACE_Typed_SV_Message<Message_Data> send_msg (msg_data,
					       SRV_ID,
					       msg_data.length ()),
					       recv_msg (pid);

  if (msgque.send (send_msg) < 0)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "msgque.send"), 1);

  if (msgque.recv (recv_msg) < 0)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "msgque.recv"), 1);

  Message_Data &recv_msg_data = recv_msg.data ();

  cout << "a message of length "
       << recv_msg_data.length ()
       << " received from server "
       << recv_msg_data.pid ()
       << " (user "
       << recv_msg_data.user () << "): "
       << recv_msg_data.text () << "\n";

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Typed_SV_Message_Queue<Message_Data>;
template class ACE_Typed_SV_Message<Message_Data>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Typed_SV_Message_Queue<Message_Data>
#pragma instantiate ACE_Typed_SV_Message<Message_Data>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

