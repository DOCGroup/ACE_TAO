// file      : Receiver.cpp
// author    : Boris Kolpackov <boris@kolpackov.net>
// cvs-id    : $Id$

#include "ace/OS_NS_string.h"
#include "ace/RMCast/Socket.h"

#include "Protocol.h"

#include <vector>
#include <iostream>

using std::cout;
using std::cerr;
using std::endl;

typedef
std::vector<unsigned char>
Status_List;

class args {};

int
ACE_TMAIN (int argc, ACE_TCHAR* argv[])
{
  try
  {
    if (argc < 2) throw args ();

    ACE_RMCast::Address addr (argv[1]);

    ACE_RMCast::Socket socket (addr);


    Message expected_msg;
    expected_msg.sn = 0;

    // VC6 does not know about new rules.
    //
    {
      for (unsigned short i = 0; i < payload_size; i++)
      {
        expected_msg.payload[i] = i;
      }
    }

    Status_List received (message_count, 0);
    Status_List damaged (message_count, 0);
    Status_List duplicate (message_count, 0);

    Message msg;

    while (true)
    {
      socket.recv (&msg, sizeof (msg));

      if (received[msg.sn] == 1)
      {
        duplicate[msg.sn] = 1;
      }
      else
      {
        received[msg.sn] = 1;

        if (ACE_OS::memcmp (expected_msg.payload, msg.payload, payload_size) != 0)
        {
          damaged[msg.sn] = 1;
        }
      }

      if (msg.sn + 1 == message_count) break;
    }

    unsigned long lost_count (0), damaged_count (0), duplicate_count (0);

    {
      for (Status_List::iterator i (received.begin ()), end (received.end ());
           i != end;
           ++i) if (*i == 0) ++lost_count;
    }


    {
      for (Status_List::iterator i (damaged.begin ()), end (damaged.end ());
           i != end;
           ++i) if (*i == 1) ++damaged_count;
    }


    {
      for (Status_List::iterator i (duplicate.begin ()), end (duplicate.end ());
           i != end;
           ++i) if (*i == 1) ++duplicate_count;
    }


    cout << "lost      : " << lost_count << endl
         << "damaged   : " << damaged_count << endl
         << "duplicate : " << duplicate_count << endl << endl;

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

  }
  catch (args const&)
  {
    cerr << "usage: " << argv[0] << " <IPv4 Multicast Address>" << endl;
  }
}
