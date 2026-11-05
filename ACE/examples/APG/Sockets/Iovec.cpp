#include "ace/INET_Addr.h"
#include "ace/SOCK_Stream.h"
#include "ace/SOCK_Connector.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Malloc_Base.h"
#include <array>
#include <memory>

constexpr char UPTIME[] = "uptime";
constexpr char HUMIDITY[] = "humidity";
constexpr char TEMPERATURE[] = "temperature";

void addCommand (iovec [], char const *)
{}

struct Iovec_Buffer_Deleter
{
  void operator() (char *buffer) const
  {
#if defined (ACE_HAS_ALLOC_HOOKS)
    ACE_Allocator::instance ()->free (buffer);
#else
    delete [] buffer;
#endif
  }
};

using Iovec_Buffer_Owner = std::unique_ptr<char, Iovec_Buffer_Deleter>;

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_INET_Addr srvr (50000, ACE_LOCALHOST);
  ACE_SOCK_Connector connector;
  ACE_SOCK_Stream peer;

  ACE_ASSERT (connector.connect (peer, srvr) != -1);

  ssize_t bc {};

  // Listing 1 code/ch06
  char up[] = "up";
  char time[] = "time";
  char newline[] = "\n";
  std::array<iovec, 4> send {};
  send[0].iov_base = up;
  send[0].iov_len  = 2;
  send[1].iov_base = time;
  send[1].iov_len  = 4;
  send[2].iov_base = newline;
  send[2].iov_len  = 1;

  peer.sendv (send.data (), 3);
  // Listing 1

  //
  // A more clever approach would use something like this:
  // Where the addCommand() method allocates and populates
  // the query array from a set of global commands.
  //
  // Listing 2 code/ch06
  std::array<iovec, 3> query {};
  addCommand (query.data (), UPTIME);
  addCommand (query.data (), HUMIDITY);
  addCommand (query.data (), TEMPERATURE);
  peer.sendv (query.data (), 3);
  // Listing 2

  // Listing 3 code/ch06
  std::array<char, 32> receive_buffer_0 {};
  std::array<char, 64> receive_buffer_1 {};
  std::array<iovec, 2> receive {};
  receive[0].iov_base = receive_buffer_0.data ();
  receive[0].iov_len  = 32;
  receive[1].iov_base = receive_buffer_1.data ();
  receive[1].iov_len  = 64;

  bc = peer.recvv (receive.data (), 2);
  // Listing 3

  // Listing 4 code/ch06
  for (int i = 0; i < 2 && bc > 0; ++i)
    {
      size_t wc = receive[i].iov_len;
      if (static_cast<size_t> (bc) < wc)
        wc = static_cast<size_t> (bc);
      ACE_OS::write (ACE_STDOUT, receive[i].iov_base, wc);
      bc -= receive[i].iov_len;
    }
  // Listing 4

  // Listing 5 code/ch06
  peer.send_n ("uptime\n", 7);
  iovec response {};
  peer.recvv (&response);
  Iovec_Buffer_Owner response_buffer (static_cast<char *> (response.iov_base));
  ACE_OS::write (ACE_STDOUT, response_buffer.get (), response.iov_len);
  // Listing 5

  peer.close ();

  return (0);
}
