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

#include <array>
#include <cstring>

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
struct Diffserv_Codepoint
{
  int value;
  char const *name;
};

constexpr std::array<Diffserv_Codepoint, 21> dscp_values
  {{
    {IPDSFIELD_DSCP_DEFAULT, "Normal"},
    {IPDSFIELD_DSCP_CS1, "CS1"},
    {IPDSFIELD_DSCP_CS2, "CS2"},
    {IPDSFIELD_DSCP_CS3, "CS3"},
    {IPDSFIELD_DSCP_CS4, "CS4"},
    {IPDSFIELD_DSCP_CS5, "CS5"},
    {IPDSFIELD_DSCP_CS6, "CS6"},
    {IPDSFIELD_DSCP_CS7, "CS7"},
    {IPDSFIELD_DSCP_AF11, "Assured Forwarding 11"},
    {IPDSFIELD_DSCP_AF12, "Assured Forwarding 12"},
    {IPDSFIELD_DSCP_AF13, "Assured Forwarding 13"},
    {IPDSFIELD_DSCP_AF21, "Assured Forwarding 21"},
    {IPDSFIELD_DSCP_AF22, "Assured Forwarding 22"},
    {IPDSFIELD_DSCP_AF23, "Assured Forwarding 23"},
    {IPDSFIELD_DSCP_AF31, "Assured Forwarding 31"},
    {IPDSFIELD_DSCP_AF32, "Assured Forwarding 32"},
    {IPDSFIELD_DSCP_AF33, "Assured Forwarding 33"},
    {IPDSFIELD_DSCP_AF41, "Assured Forwarding 41"},
    {IPDSFIELD_DSCP_AF42, "Assured Forwarding 42"},
    {IPDSFIELD_DSCP_AF43, "Assured Forwarding 43"},
    {IPDSFIELD_DSCP_EF, "Expedited Forwarding"}
  }};

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  if (argc != 3)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Usage:\n  %s [destination host] [destination port]\n\n",
                  argv[0]));
      return 1;
    }

  char const buf[] = "Hello";

  ACE_SOCK_CODgram sock;
  ACE_INET_Addr const raddr (ACE_OS::atoi (argv[2]), argv[1]);

  if (sock.open (raddr, ACE_Addr::sap_any, PF_INET, 0, 1) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "open datagram socket"), 1);
    }

  for (Diffserv_Codepoint const &codepoint : dscp_values)
    {
      int opt = codepoint.value << 2;
      int const ret = sock.set_option (IPPROTO_IP,
                                       IP_TOS,
                                       &opt,
                                       static_cast<int> (sizeof (opt)));
      if (ret == -1)
        ACE_DEBUG ((LM_DEBUG, "setsockopt error: %m\n"));

      for (int j = 0; j < 3; ++j)
        {
          int const ret2 = sock.send (buf, std::strlen (buf));
          if (ret2 == -1)
            ACE_DEBUG ((LM_DEBUG, "send error: %m\n"));

          ACE_OS::printf ("opt: %d dscp: %d, %s, setsockopt returned: %d, send returned: %d\n",
                          opt,
                          codepoint.value,
                          codepoint.name,
                          ret,
                          ret2);
          ACE_OS::sleep (1);
        }
    }

  return 0;
}
