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

  char *ipv4_addresses[] = {
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

    if(addr.get_ip_address() != addr32) {
      printf("Error: Address %s failed get_ip_address() check\n",ipv4_addresses[i]);
      printf("%u != %u\n",addr.get_ip_address(), addr32);
    }
    if(0 != ACE_OS::strcmp(addr.get_host_addr(),ipv4_addresses[i])) {
      printf("Error: Address %s failed get_host_addr() check\n",ipv4_addresses[i]);
      printf("%s != %s\n",addr.get_host_addr(),ipv4_addresses[i]);
    }
  }

  return 0;
}
