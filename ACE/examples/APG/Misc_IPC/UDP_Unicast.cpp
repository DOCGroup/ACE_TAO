/**
 * Sample code from The ACE Programmer's Guide,
 * Copyright 2003 Addison-Wesley. All Rights Reserved.
 */

// Listing 1 code/ch09
#include "ace/Log_Msg.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_Dgram.h"
#include <array>

int send_unicast (ACE_INET_Addr const &to)
{
  constexpr char message[] = "this is the message!\n";
  ACE_INET_Addr my_addr (static_cast<u_short> (10101));
  ACE_SOCK_Dgram udp (my_addr);
  ssize_t const sent = udp.send (message, sizeof (message), to);
  udp.close ();
  if (sent == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"),
                       ACE_TEXT ("send")), -1);
  return 0;
}
// Listing 1

// Listing 2 code/ch09
void echo_dgram ()
{
  ACE_INET_Addr my_addr (static_cast<u_short> (10102));
  ACE_INET_Addr your_addr;
  ACE_SOCK_Dgram udp (my_addr);
  std::array<char, BUFSIZ> buff {};
  size_t buflen = buff.size ();
  ssize_t const recv_cnt = udp.recv (buff.data (), buflen, your_addr);
  if (recv_cnt > 0)
    udp.send (buff.data (), static_cast<size_t> (buflen), your_addr);
  udp.close ();
}
// Listing 2

// Listing 3 code/ch09
#include "ace/SOCK_CODgram.h"
// Exclude 3
static void show_codgram ()
{
  std::array<char, BUFSIZ> buff {};
  size_t const buflen = buff.size ();
  // Exclude 3
  ACE_TCHAR const *peer = ACE_TEXT ("other_host:8042");
  ACE_INET_Addr peer_addr (peer);
  ACE_SOCK_CODgram udp;
  if (0 != udp.open (peer_addr))
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), peer));

  // ...

  if (-1 == udp.send (buff.data (), buflen))
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("send")));
  // Listing 3
}

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  show_codgram ();
  return 0;
}
