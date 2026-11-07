//=============================================================================
/**
  *  @file   diffserv_test.cpp
  *
  *
  *
  *  @author Craig Rodrigues <crodrigu@bbn.com>
  *  @brief  Send UDP packets to a destination host and port.
  *          Change the Diffserv field to various values.
  */
//=============================================================================

#include "ace/Log_Msg.h"
#include "ace/SOCK_CODgram.h"
#include "ace/INET_Addr.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_stdio.h"

constexpr int IPDSFIELD_DSCP_DEFAULT = 0x00;
constexpr int IPDSFIELD_DSCP_CS1 = 0x08;
constexpr int IPDSFIELD_DSCP_CS2 = 0x10;
constexpr int IPDSFIELD_DSCP_CS3 = 0x18;
constexpr int IPDSFIELD_DSCP_CS4 = 0x20;
constexpr int IPDSFIELD_DSCP_CS5 = 0x28;
constexpr int IPDSFIELD_DSCP_CS6 = 0x30;
constexpr int IPDSFIELD_DSCP_CS7 = 0x38;
constexpr int IPDSFIELD_DSCP_AF11 = 0x0A;
constexpr int IPDSFIELD_DSCP_AF12 = 0x0C;
constexpr int IPDSFIELD_DSCP_AF13 = 0x0E;
constexpr int IPDSFIELD_DSCP_AF21 = 0x12;
constexpr int IPDSFIELD_DSCP_AF22 = 0x14;
constexpr int IPDSFIELD_DSCP_AF23 = 0x16;
constexpr int IPDSFIELD_DSCP_AF31 = 0x1A;
constexpr int IPDSFIELD_DSCP_AF32 = 0x1C;
constexpr int IPDSFIELD_DSCP_AF33 = 0x1E;
constexpr int IPDSFIELD_DSCP_AF41 = 0x22;
constexpr int IPDSFIELD_DSCP_AF42 = 0x24;
constexpr int IPDSFIELD_DSCP_AF43 = 0x26;
constexpr int IPDSFIELD_DSCP_EF = 0x2E;
constexpr int IPDSFIELD_ECT_MASK = 0x02;
constexpr int IPDSFIELD_CE_MASK = 0x01;

constexpr int dscp[] =
  {
    IPDSFIELD_DSCP_DEFAULT ,
    IPDSFIELD_DSCP_CS1     ,
    IPDSFIELD_DSCP_CS2     ,
    IPDSFIELD_DSCP_CS3     ,
    IPDSFIELD_DSCP_CS4     ,
    IPDSFIELD_DSCP_CS5     ,
    IPDSFIELD_DSCP_CS6     ,
    IPDSFIELD_DSCP_CS7     ,
    IPDSFIELD_DSCP_AF11    ,
    IPDSFIELD_DSCP_AF12    ,
    IPDSFIELD_DSCP_AF13    ,
    IPDSFIELD_DSCP_AF21    ,
    IPDSFIELD_DSCP_AF22    ,
    IPDSFIELD_DSCP_AF23    ,
    IPDSFIELD_DSCP_AF31    ,
    IPDSFIELD_DSCP_AF32    ,
    IPDSFIELD_DSCP_AF33    ,
    IPDSFIELD_DSCP_AF41    ,
    IPDSFIELD_DSCP_AF42    ,
    IPDSFIELD_DSCP_AF43    ,
    IPDSFIELD_DSCP_EF
  };

constexpr char const * dscp_char[] =
  {
    "Normal",
    "CS1",
    "CS2",
    "CS3",
    "CS4",
    "CS5",
    "CS6",
    "CS7",
    "Assured Forwarding 11",
    "Assured Forwarding 12",
    "Assured Forwarding 13",
    "Assured Forwarding 21",
    "Assured Forwarding 22",
    "Assured Forwarding 23",
    "Assured Forwarding 31",
    "Assured Forwarding 32",
    "Assured Forwarding 33",
    "Assured Forwarding 41",
    "Assured Forwarding 42",
    "Assured Forwarding 43",
    "Expedited Forwarding"
  };

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  if(argc != 3)
    {
      ACE_DEBUG((LM_DEBUG, "Usage:\n  %s [destination host] [destination port]\n\n", argv[0]));
      ACE_OS::exit(1);
    }


  char buf[] = "Hello";

  ACE_SOCK_CODgram sock;
  ACE_INET_Addr raddr (ACE_OS::atoi (argv[2]), argv[1]);

  sock.open (raddr, ACE_Addr::sap_any, PF_INET, 0, 1);
  int opt = 0;
  int ret = 0;
  int ret2 = 0;
  unsigned int i;
  for (i = 0; i < 21; i++)
    {
      opt = dscp[i] << 2;
      ret = sock.set_option (IPPROTO_IP, IP_TOS, &opt, static_cast<int> (sizeof (opt)));
      if (ret == -1)
        ACE_DEBUG ((LM_DEBUG, "setsockopt error: %m\n"));

      for (int j = 0; j < 3; j++)
        {
          ret2 = sock.send (buf, ACE_OS::strlen (buf));
          if (ret2 == -1)
            ACE_DEBUG ((LM_DEBUG, "send error: %m\n"));
          ACE_OS::printf ("opt: %u dscp: %u, %s, setsockopt returned: %d, send returned: %d\n",
                          opt, opt >> 2, dscp_char[i], ret, ret2);
          ACE_OS::sleep (1);
        }
    }

  return 0;
}
