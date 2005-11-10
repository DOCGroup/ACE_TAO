/**
 * $Id$
 *
 * Sample code from The ACE Programmer's Guide,
 * Copyright 2003 Addison-Wesley. All Rights Reserved.
 */

// Listing 1 code/ch09
#include "ace/OS_NS_string.h"
#include "ace/Log_Msg.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_Dgram.h"

int send_unicast (const ACE_INET_Addr &to)
{
  const char *message = "this is the message!\n";
  ACE_INET_Addr my_addr (static_cast<u_short> (10101));
  ACE_SOCK_Dgram udp (my_addr);
  ssize_t sent = udp.send (message,
                           ACE_OS::strlen (message) + 1,
                           to);
  udp.close ();
  if (sent == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"),
                       ACE_TEXT ("send")), -1);
  return 0;
}
// Listing 1

// Listing 2 code/ch09
void echo_dgram (void)
{
  ACE_INET_Addr my_addr (static_cast<u_short> (10102));
  ACE_INET_Addr your_addr;
  ACE_SOCK_Dgram udp (my_addr);
  char buff[BUFSIZ];
  size_t buflen = sizeof (buff);
  ssize_t recv_cnt = udp.recv (buff, buflen, your_addr);
  if (recv_cnt > 0)
    udp.send (buff, static_cast<size_t> (buflen), your_addr);
  udp.close ();
  return;
}
// Listing 2

// Listing 3 code/ch09
#include "ace/SOCK_CODgram.h"
// Exclude 3
static void show_codgram (void)
{
  char buff[BUFSIZ];
  size_t buflen = sizeof (buff);
  // Exclude 3
  const ACE_TCHAR *peer = ACE_TEXT ("other_host:8042");
  ACE_INET_Addr peer_addr (peer);
  ACE_SOCK_CODgram udp;
  if (0 != udp.open (peer_addr))
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), peer));

  // ...

  if (-1 == udp.send (buff, buflen))
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("send")));
  // Listing 3
}

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  show_codgram ();
  return 0;
}
