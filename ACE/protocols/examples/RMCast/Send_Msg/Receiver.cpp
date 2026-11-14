// file      : Receiver.cpp
// author    : Boris Kolpackov <boris@kolpackov.net>
#include "ace/Log_Msg.h"
#include "ace/Basic_Types.h"
#include "ace/OS_NS_errno.h"
#include "ace/OS_NS_string.h"
#include "ace/Time_Value.h"
#include "ace/OS_NS_sys_time.h" // gettimeofday

#include "ace/RMCast/Socket.h"

#include "Protocol.h"

#include <vector>

using Status_List = std::vector<unsigned char>;

int
ACE_TMAIN (int argc, ACE_TCHAR *argv [])
{
  if (argc < 2)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("usage: %s <IPv4 multicast address>:<port>\n"), argv[0]));
      return 1;
    }

  ACE_INET_Addr addr (argv[1]);

  //FUZZ: disable check_for_lack_ACE_OS
  ACE_RMCast::Socket socket (addr, false);
  //FUZZ: enable check_for_lack_ACE_OS

  Message expected_msg {};
  for (size_t i {}; i < payload_size; ++i)
    {
      expected_msg.payload[i] = static_cast<unsigned short> (i);
    }

  Status_List received (message_count, 0);
  Status_List damaged (message_count, 0);
  Status_List duplicate (message_count, 0);

  Message msg {};
  bool first { true };
  ACE_Time_Value start_time {};

  for (;;)
    {
      ssize_t const s = socket.size ();

      if (first)
        {
          start_time = ACE_OS::gettimeofday ();
          first = false;
        }

      if (s == -1 && errno == ENOENT)
        {
          ACE_ERROR ((LM_ERROR, ACE_TEXT ("unavailable message detected\n")));

          // Receive it.
          socket.recv (&msg, sizeof (msg));

          continue;
        }

      if (s != static_cast<ssize_t> (sizeof (msg)))
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("unexpected message size %b, expected %B\n"),
                      s, sizeof (msg)));
          continue;
        }

      if (socket.recv (&msg, sizeof (msg)) != s)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("recv() reported different size than size()\n")));
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
                              payload_size) != 0)
            {
              damaged[msg.sn] = 1;
            }
        }

      if (msg.sn + 1 == message_count)
        {
          break;
        }
    }

  ACE_Time_Value const time = ACE_OS::gettimeofday () - start_time;

  size_t lost_count {};
  size_t damaged_count {};
  size_t duplicate_count {};

  for (unsigned char entry : received)
    {
      if (entry == 0)
        {
          ++lost_count;
        }
    }

  for (unsigned char entry : damaged)
    {
      if (entry == 1)
        {
          ++damaged_count;
        }
    }

  for (unsigned char entry : duplicate)
    {
      if (entry == 1)
        {
          ++duplicate_count;
        }
    }

  size_t const elapsed_msec =
    static_cast<size_t> (time.msec () == 0 ? 1 : time.msec ());
  size_t const throughput = (sizeof (msg) * message_count) / elapsed_msec;

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("lost       : %B\n")
              ACE_TEXT ("damaged    : %B\n")
              ACE_TEXT ("duplicate  : %B\n")
              ACE_TEXT ("throughput : %B KB/sec\n"),
              lost_count,
              damaged_count,
              duplicate_count,
              throughput));

  return 0;
}
