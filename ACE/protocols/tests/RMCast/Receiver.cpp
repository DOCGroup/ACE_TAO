// file      : Receiver.cpp
// author    : Boris Kolpackov <boris@kolpackov.net>
#include "ace/Log_Msg.h"
#include "ace/OS_NS_string.h"

#include <vector>

#include "ace/RMCast/Socket.h"

#include "Protocol.h"

using Status_List = std::vector<unsigned char>;

class args {};

int
ACE_TMAIN (int argc, ACE_TCHAR* argv[])
{
  try
  {
    if (argc < 2) throw args ();

    ACE_INET_Addr addr (argv[1]);

    //FUZZ: disable check_for_lack_ACE_OS
    // Turn on message loss and reordering simulation.
    //
    ACE_RMCast::Socket socket (addr, false, ACE_RMCast::Parameters {true});
    //FUZZ: enable check_for_lack_ACE_OS


    Message expected_msg {};

    for (unsigned short i = 0; i < payload_size; ++i)
      expected_msg.payload[i] = i;

    Status_List received (message_count, 0);
    Status_List damaged (message_count, 0);
    Status_List duplicate (message_count, 0);

    Message msg {};

    while (true)
    {
      ssize_t s = socket.size ();

      if (s == -1 && errno == ENOENT)
      {
        ACE_ERROR ((LM_ERROR, "unavailable message detected\n"));

        // Receive it.
        //
        socket.recv (&msg, sizeof (msg));

        continue;
      }

      if (s != (ssize_t)sizeof (msg))
      {
        ACE_ERROR ((LM_ERROR, "unexpected message size %d, expected %d\n",
                    s, sizeof (msg)));
        continue;
      }

      if (socket.recv (&msg, sizeof (msg)) != s)
      {
        ACE_ERROR ((LM_ERROR,
                    "recv() reported different size than size()\n"));
        continue;
      }

      if (received[msg.sn] == 1)
      {
        duplicate[msg.sn] = 1;
      }
      else
      {
        received[msg.sn] = 1;

        if (ACE_OS::memcmp (expected_msg.payload,
                            msg.payload,
                            payload_size * sizeof(short)) != 0)
        {
          damaged[msg.sn] = 1;
        }
      }

      if (msg.sn + 1 == message_count) break;
    }

    unsigned long lost_count (0), damaged_count (0), duplicate_count (0);

    for (unsigned char const e : received)
      if (e == 0) ++lost_count;

    for (unsigned char const e : damaged)
      if (e == 1) ++damaged_count;

    for (unsigned char const e : duplicate)
      if (e == 1) ++duplicate_count;

    ACE_DEBUG ((LM_DEBUG,
                "lost      : %d\n"
                "damaged   : %d\n"
                "duplicate : %d\n",
                lost_count,
                damaged_count,
                duplicate_count));

    if (damaged_count != 0 || duplicate_count != 0)
      return 1;

    if (lost_count != 0)
    {
      // It is ok to lose some number of messages at the beginning.
      //
      unsigned long count (0);

      for (unsigned char const e : received)
      {
        if (e == 0)
          ++count;
        else
          break; // Seen first ok message.
      }

      if (lost_count != count)
        return 1;
    }

    return 0;
  }
  catch (args const&)
  {
    ACE_ERROR ((LM_ERROR,
                "usage: %s <IPv4 multicast address>:<port>\n", argv[0]));
  }

  return 1;
}
