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
#include "ace/SOCK_Dgram_Bcast.h"

int send_broadcast (u_short to_port)
{
  const char *message = "this is the message!\n";
  ACE_INET_Addr my_addr (static_cast<u_short> (10101));
  ACE_SOCK_Dgram_Bcast udp (my_addr);
  ssize_t sent = udp.send (message,
                           ACE_OS::strlen (message) + 1,
                           to_port);
  udp.close ();
  if (sent == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"),
                       ACE_TEXT ("send")), -1);
  return 0;
}
// Listing 1

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  send_broadcast (10);
  return 0;
}
