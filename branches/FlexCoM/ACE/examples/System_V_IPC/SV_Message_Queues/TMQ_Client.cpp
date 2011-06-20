// $Id$

#include "ace/Typed_SV_Message_Queue.h"

// FUZZ: disable check_for_streams_include
#include "ace/streams.h"

#include "ace/Log_Msg.h"
#include "test.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_unistd.h"



int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  long pid = long (ACE_OS::getpid ());

  ACE_Typed_SV_Message_Queue<Message_Data> msgque (key_t (SRV_KEY));

  Message_Data msg_data (pid,
                         ACE_OS::cuserid (static_cast<char *> (0)),
                         "did you get this?");

  ACE_Typed_SV_Message<Message_Data> send_msg (msg_data,
                                               SRV_ID,
                                               msg_data.length ()),
                                               recv_msg (pid);

  if (msgque.send (send_msg) < 0)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"),
                       ACE_TEXT ("msgque.send")), 1);

  if (msgque.recv (recv_msg) < 0)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"),
                       ACE_TEXT ("msgque.recv")), 1);

  Message_Data &recv_msg_data = recv_msg.data ();

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("a message of length %d")
              ACE_TEXT (" received from server %d")
              ACE_TEXT (" (user %C): %C\n"),
              recv_msg_data.length (),
              recv_msg_data.pid (),
              recv_msg_data.user (),
              recv_msg_data.text ()));

  return 0;
}

