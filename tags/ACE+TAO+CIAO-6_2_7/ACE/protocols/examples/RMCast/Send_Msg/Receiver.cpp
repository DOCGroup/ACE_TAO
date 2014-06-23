// file      : Receiver.cpp
// author    : Boris Kolpackov <boris@kolpackov.net>
// cvs-id    : $Id$

#include "ace/Vector_T.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_string.h"
#include "ace/Time_Value.h"     // ACE_Time_Value
#include "ace/OS_NS_sys_time.h" // gettimeofday

#include "ace/RMCast/Socket.h"

#include "Protocol.h"

typedef
ACE_Vector<unsigned char, ACE_VECTOR_DEFAULT_SIZE>
Status_List;

class args {};

int
ACE_TMAIN (int argc, ACE_TCHAR* argv[])
{
  try
  {
    if (argc < 2) throw args ();

    ACE_INET_Addr addr (argv[1]);

    //FUZZ: disable check_for_lack_ACE_OS
    ACE_RMCast::Socket socket (addr, false);
    //FUZZ: enable check_for_lack_ACE_OS

    Message expected_msg;
    expected_msg.sn = 0;

    // VC6 does not know about the new rules.
    //
    {
      for (unsigned short i = 0; i < payload_size; i++)
      {
        expected_msg.payload[i] = i;
      }
    }

    Status_List received (message_count);
    Status_List damaged (message_count);
    Status_List duplicate (message_count);

    // VC6 does not know about new rules.
    //
    {
      for (unsigned int i = 0; i < message_count; ++i)
      {
        received.push_back (0);
        damaged.push_back (0);
        duplicate.push_back (0);
      }
    }


    Message msg;
    bool first (true);
    ACE_Time_Value start_time, time;

    while (true)
    {
      ssize_t s = socket.size ();

      if (first)
      {
        start_time = ACE_OS::gettimeofday ();
        first = false;
      }

      if (s == -1 && errno == ENOENT)
      {
        ACE_ERROR ((LM_ERROR, "unavailable message detected\n"));

        // Receive it.
        //
        socket.recv (&msg, sizeof (msg));

        continue;
      }

      if (s != (ssize_t) sizeof (msg))
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
                            payload_size) != 0)
        {
          damaged[msg.sn] = 1;
        }
      }

      if (msg.sn + 1 == message_count) break;
    }

    time = ACE_OS::gettimeofday () - start_time;

    unsigned int lost_count (0), damaged_count (0), duplicate_count (0);

    {
      for (Status_List::Iterator i (received); !i.done (); i.advance ())
      {
        unsigned char* e;
        i.next (e);

        if (*e == 0) ++lost_count;
      }
    }


    {
      for (Status_List::Iterator i (damaged); !i.done (); i.advance ())
      {
        unsigned char* e;
        i.next (e);

        if (*e == 1) ++damaged_count;
      }
    }


    {
      for (Status_List::Iterator i (duplicate); !i.done (); i.advance ())
      {
        unsigned char* e;
        i.next (e);

        if (*e == 1) ++duplicate_count;
      }
    }

    unsigned long tput =
      (sizeof (msg) * message_count) / (time.msec () == 0 ? 1 : time.msec ());

    ACE_DEBUG ((LM_DEBUG,
                "lost       : %d\n"
                "damaged    : %d\n"
                "duplicate  : %d\n"
                "throughput : %d KB/sec\n",
                lost_count,
                damaged_count,
                duplicate_count,
                tput));

    /*
    cout << "lost message dump:" << endl;

    unsigned long total = 0;

    for (Status_List::iterator
           begin (received.begin ()), i (begin), end (received.end ());
         i != end;)
    {
      if (*i == 0)
      {
        unsigned long count = 1;

        for (Status_List::iterator j = i + 1;
             j < end && *j == 0;
             j++, count++);

        cout << '\t' << i - begin << " : " << count << endl;

        i += count;
        total += count;
      }
      else ++i;
    }

    if (total != lost_count) cerr << "trouble" << endl;
    */

    return 0;
  }
  catch (args const&)
  {
    ACE_ERROR ((LM_ERROR,
                "usage: %s <IPv4 multicast address>:<port>\n", argv[0]));
  }

  return 1;
}
