// $Id$

#include "ace/SV_Message_Queue.h"
#include "test.h"

// FUZZ: disable check_for_streams_include
#include "ace/streams.h"

#include "ace/Malloc.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_stdlib.h"



#if defined (ACE_HAS_SYSV_IPC) && !defined(ACE_LACKS_SYSV_SHMEM)

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  long pid = long (ACE_OS::getpid ());
  ACE_SV_Message_Queue msgque (SRV_KEY);
  Message_Block send_msg (SRV_ID,
                          pid,
                          ACE_OS::cuserid (static_cast<char *> (0)),
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

#else

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_ERROR ((LM_ERROR,
              "SYSV IPC, or SYSV SHMEM is not supported on this platform\n"));
  return 0;
}
#endif /* ACE_HAS_SYSV_IPC && !ACE_LACKS_SYSV_SHMEM*/

