// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    INET_Addr_Test.cpp
//
// = DESCRIPTION
//     Performs several tests on the ACE_INET_Addr class.  It creates several
//     IPv4 and IPv6 addresses and checks that the address formed by the
//     class is valid.
//
// = AUTHOR
//    John Aughey (jha@aughey.com)
//
// ============================================================================

#include "test_config.h"
#include "ace/OS.h"
#include "ace/INET_Addr.h"
#include "ace/Log_Msg.h"


int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_UNUSED_ARG (argc);
  ACE_UNUSED_ARG (argv);

  ACE_START_TEST (ACE_TEXT ("INET_Addr_Test"));

  int status = 0;     // Innocent until proven guilty

  const char *ipv4_addresses[] = {
    "127.0.0.1", "138.38.180.251", "64.219.54.121", "192.0.0.1", "10.0.0.1", 0
  };

  ACE_INET_Addr addr;

  for (int i=0; ipv4_addresses[i] != 0; i++) {
    struct in_addr addrv4;
    ACE_UINT32 addr32;

    ACE_OS::inet_pton (AF_INET, ipv4_addresses[i], &addrv4);

    ACE_OS::memcpy (&addr32, &addrv4, sizeof (addr32));

    addr.set (80, ipv4_addresses[i]);

    /*
    ** Now check to make sure get_ip_address matches and get_host_addr
    ** matches.
    */
    if (addr.get_ip_address () != ACE_HTONL (addr32))
      {
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("Error: %s failed get_ip_address() check\n")
                    ACE_TEXT ("0x%x != 0x%x\n"),
                    ipv4_addresses[i],
                    addr.get_ip_address (),
                    addr32));
        status = 1;
      }

    if (0 != ACE_OS::strcmp (addr.get_host_addr(), ipv4_addresses[i]))
      {
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("%s failed get_host_addr() check\n")
                    ACE_TEXT ("%s != %s\n"),
                    ipv4_addresses[i],
                    addr.get_host_addr (),
                    ipv4_addresses[i]));
        status = 1;
      }

    // Clear out the address by setting it to 1 and check
    addr.set (0, ACE_UINT32 (1), 1);
    if (addr.get_ip_address () != 1)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("Failed to set address to 1\n")));
        status = 1;
      }

    // Now set the address using a 32 bit number and check that we get
    // the right string out of get_host_addr().
    addr.set (80, addr32, 0); // addr32 is already in network byte order
    if (0 != ACE_OS::strcmp (addr.get_host_addr (), ipv4_addresses[i]))
      {
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("%s failed second get_host_addr() check\n")
                    ACE_TEXT ("%s != %s\n"),
                    ipv4_addresses[i],
                    addr.get_host_addr (),
                    ipv4_addresses[i]));
        status = 1;
      }
  }

#if defined (ACE_HAS_IPV6)
  if (ACE_Sock_Connect::ipv6_enabled ())
    {
      const char *ipv6_addresses[] = {
        "1080::8:800:200c:417a", // unicast address
        "ff01::101",             // multicast address
        "::1",                   // loopback address
        "::",                    // unspecified addresses
        0
      };

      for (int i=0; ipv6_addresses[i] != 0; i++)
        {
          ACE_INET_Addr addr (80, ipv6_addresses[i]);

          if (0 != ACE_OS::strcmp (addr.get_host_addr (), ipv6_addresses[i]))
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("IPv6 get_host_addr failed: %s != %s\n"),
                          addr.get_host_addr (),
                          ipv6_addresses[i]));
              status = 1;
            }
        }
    }

#endif

  ACE_END_TEST;

  return status;
}
