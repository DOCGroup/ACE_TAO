// $Id$

#include "ace/INET_Addr.h"
#include "ace/SOCK_Stream.h"
#include "ace/SOCK_Connector.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_unistd.h"

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  // Listing 1 code/ch06
  ACE_INET_Addr srvr (50000, ACE_LOCALHOST);
  // Listing 1

  // Listing 2 code/ch06
  ACE_SOCK_Connector connector;
  ACE_SOCK_Stream peer;

  if (-1 == connector.connect (peer, srvr))
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("connect")), 1);
  // Listing 2

  ssize_t bc;
  char buf[64];

  // Listing 3 code/ch06
  peer.send_n ("uptime\n", 7);
  bc = peer.recv (buf, sizeof(buf));
  ACE_OS::write (ACE_STDOUT, buf, bc);
  peer.close ();
  // Listing 3

  return (0);
}
