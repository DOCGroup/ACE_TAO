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
#include "ace/SOCK_Dgram_Mcast.h"

int send_multicast (const ACE_INET_Addr &mcast_addr)
{
  const char *message = "this is the message!\n";
  ACE_SOCK_Dgram_Mcast udp;
  if (-1 == udp.join (mcast_addr))
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"),
                       ACE_TEXT ("join")), -1);

  ssize_t sent = udp.send (message,
                           ACE_OS::strlen (message) + 1);
  udp.close ();
  if (sent == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"),
                       ACE_TEXT ("send")), -1);
  return 0;
}
// Listing 1

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_INET_Addr nop;
  send_multicast (nop);
  return 0;
}
