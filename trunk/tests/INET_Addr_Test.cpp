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

#include <stdio.h>
#include "ace/INET_Addr.h"

int main(int argc, char *argv[])
{
  ACE_UNUSED_ARG(argc);
  ACE_UNUSED_ARG(argv);

  const char *ipv4_addresses[] = {
    "127.0.0.1", "138.38.180.251", "64.219.54.121", "192.0.0.1", "10.0.0.1", 0
  };

  ACE_INET_Addr addr;

  for(int i=0; ipv4_addresses[i] != 0; i++) {
    struct in_addr addrv4;
    ACE_UINT32 addr32;

    ACE_OS::inet_pton(AF_INET,ipv4_addresses[i],(void*)&addrv4);

    memcpy((void*)&addr32,(void*)&addrv4,sizeof(addr32));

    addr.set(80,ipv4_addresses[i]);

    /* Now check to make sure get_ip_address matches and get_host_addr
       matches. */

    if(addr.get_ip_address() != htonl(addr32)) {
      printf("Error: Address %s failed get_ip_address() check\n",ipv4_addresses[i]);
      printf("0x%x != 0x%x\n",addr.get_ip_address(), addr32);
    }
    if(0 != ACE_OS::strcmp(addr.get_host_addr(),ipv4_addresses[i])) {
      printf("Error: Address %s failed get_host_addr() check\n",ipv4_addresses[i]);
      printf("%s != %s\n",addr.get_host_addr(),ipv4_addresses[i]);
    }

    // Clear out the address by setting it to 1 and check
    addr.set((u_short)0,(ACE_UINT32)1,1);
    if(addr.get_ip_address() != 1) {
      printf("Error: failed to set address to 1\n");
    }

    // Now set the address using a 32 bit number and check that we get
    // the right string out of get_host_addr().
    addr.set(80,addr32,0); // addr32 is already in network byte order
    if(0 != ACE_OS::strcmp(addr.get_host_addr(),ipv4_addresses[i])) {
      printf("Error: Address %s failed second get_host_addr() check\n",ipv4_addresses[i]);
      printf("%s != %s\n",addr.get_host_addr(),ipv4_addresses[i]);
    }
  }

#if defined (ACE_HAS_IPV6)
  if(ACE_Sock_Connect::ipv6_enabled()) {
  char *ipv6_addresses[] = {
    "1080::8:800:200c:417a", // unicast address
    "ff01::101",       // multicast address
    "::1",                        // loopback address
    "::",            // unspecified addresses
    0
  };
  for(int i=0; ipv6_addresses[i] != 0; i++) {
    ACE_INET_Addr addr(80,ipv6_addresses[i]);

    if(0 != ACE_OS::strcmp(addr.get_host_addr(),ipv6_addresses[i])) {
      printf("IPv6 get_host_addr failed: %s != %s\n",addr.get_host_addr(),ipv6_addresses[i]);
    }
  }
  }

#endif

  return 0;
}
