// file      : Sender.cpp
// author    : Boris Kolpackov <boris@kolpackov.net>
// cvs-id    : $Id$


#include <ace/OS.h>
#include <ace/RMCast/Socket.h>

#include <iostream>

#include "Protocol.h"

using std::cerr;
using std::endl;

class args {};

int
ACE_TMAIN (int argc, ACE_TCHAR* argv[])
{
  try
  {
    if (argc < 2) throw args ();

    ACE_INET_Addr addr (argv[1]);

    ACE_RMCast::Socket socket (addr);

    Message msg;
    msg.sn = 0;

    for (unsigned short i = 0; i < payload_size; i++)
    {
      msg.payload[i] = i;
    }

    for (; msg.sn < message_count; msg.sn++)
    {
      socket.send (&msg, sizeof (msg));
    }

    // Keep running in case retransmissions are needed.
    //
    ACE_OS::sleep (ACE_Time_Value (50, 0));

    return 0;
  }
  catch (args const&)
  {
    cerr << "usage: " << argv[0] << " <IPv4 Multicast Address>" << endl;
  }

  return 1;
}
