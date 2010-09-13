// file      : Sender.cpp
// author    : Boris Kolpackov <boris@kolpackov.net>
// cvs-id    : $Id$

#include "ace/Log_Msg.h"

#include "ace/RMCast/Socket.h"

#include "Protocol.h"

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
    ACE_ERROR ((LM_ERROR,
                "usage: %s <IPv4 multicast address>:<port>\n", argv[0]));
  }

  return 1;
}
