// $Id$

#include "ace/INET_Addr.h"
#include "ace/SOCK_Stream.h"
#include "ace/SOCK_Connector.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_unistd.h"

const char *UPTIME = "uptime";
const char *HUMIDITY = "humidity";
const char *TEMPERATURE = "temperature";

void addCommand (iovec [], const char *)
{}

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_INET_Addr srvr (50000, ACE_LOCALHOST);
  ACE_SOCK_Connector connector;
  ACE_SOCK_Stream peer;

  ACE_ASSERT (connector.connect (peer, srvr) != -1);

  ssize_t bc;

  // Listing 1 code/ch06
  iovec send[4];
  send[0].iov_base = const_cast<char *> ("up");
  send[0].iov_len  = 2;
  send[1].iov_base = const_cast<char *> ("time");
  send[1].iov_len  = 4;
  send[2].iov_base = const_cast<char *> ("\n");
  send[2].iov_len  = 1;

  peer.sendv (send, 3);
  // Listing 1

  //
  // A more clever approach would use something like this:
  // Where the addCommand() method allocates and populates
  // the query array from a set of global commands.
  //
  // Listing 2 code/ch06
  iovec query[3];
  addCommand (query, UPTIME);
  addCommand (query, HUMIDITY);
  addCommand (query, TEMPERATURE);
  peer.sendv (query, 3);
  // Listing 2

  // Listing 3 code/ch06
  iovec receive[2];
  receive[0].iov_base = new char [32];
  receive[0].iov_len  = 32;
  receive[1].iov_base = new char [64];
  receive[1].iov_len  = 64;

  bc = peer.recvv (receive, 2);
  // Listing 3

  // Listing 4 code/ch06
  for (int i = 0; i < 2 && bc > 0; ++i)
    {
      size_t wc = receive[i].iov_len;
      if (static_cast<size_t> (bc) < wc)
        wc = static_cast<size_t> (bc);
      ACE_OS::write (ACE_STDOUT, receive[i].iov_base, wc);
      bc -= receive[i].iov_len;
      delete []
        (reinterpret_cast<char *> (receive[i].iov_base));
    }
  // Listing 4

  // Listing 5 code/ch06
  peer.send_n ("uptime\n", 7);
  iovec response;
  peer.recvv (&response);
  ACE_OS::write (ACE_STDOUT, response.iov_base, response.iov_len);
  delete [] reinterpret_cast<char *> (response.iov_base);
  // Listing 5

  peer.close ();

  return (0);
}
