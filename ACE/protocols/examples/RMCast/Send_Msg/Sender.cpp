// file      : Sender.cpp
// author    : Boris Kolpackov <boris@kolpackov.net>
#include "ace/Log_Msg.h"
#include "ace/OS_NS_unistd.h"

#include "ace/RMCast/Socket.h"

#include "Protocol.h"

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

  Message msg {};
  for (size_t i {}; i < payload_size; ++i)
    {
      msg.payload[i] = static_cast<unsigned short> (i);
    }

  for (; msg.sn < message_count; ++msg.sn)
    {
      socket.send (&msg, sizeof (msg));
    }

  // Keep running in case retransmissions are needed.
  ACE_OS::sleep (ACE_Time_Value (50, 0));

  return 0;
}
