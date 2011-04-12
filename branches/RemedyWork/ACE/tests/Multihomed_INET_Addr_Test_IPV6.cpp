//=============================================================================
/**
 *  @file    Multihomed_INET_Addr_Test_IPV6.cpp
 *
 *  $Id$
 *
 *   Performs several tests on the Multihomed_ACE_INET_Addr class.
 *   It creates several IPv6 addresses and checks that the
 *   address formed by the class is valid.
 *
 *
 *  @author Edward Mulholland (emulholl@atl.lmco.com) Brian Buesker (bbuesker@qualcomm.com) - Added testing of                                         ACE_Multihomed_INET_Addr class                                         using IPv6 addresses based on                                         Multihomed_INET_Addr_Test.
 */
//=============================================================================


#include "test_config.h"
#include "ace/OS_NS_string.h"
#include "ace/Multihomed_INET_Addr.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_arpa_inet.h"

int run_main (int argc, ACE_TCHAR *argv[])
{
  ACE_UNUSED_ARG (argc);
  ACE_UNUSED_ARG (argv);

  ACE_START_TEST (ACE_TEXT ("Multihomed_INET_Addr_Test_IPV6"));

  int status = 0;     // Innocent until proven guilty

#if defined (ACE_HAS_IPV6)
  // loop variables
  size_t i, j;
  sockaddr_in6 *pointer6;

  const ACE_TCHAR *primary_ipv6 = ACE_TEXT("3ffe::123:4567:89ab:cdef");

  const ACE_TCHAR *secondary_ipv6[] = {
    ACE_IPV6_LOCALHOST,
    ACE_TEXT("fe80::0123:4567:89ab:cdef"),
    ACE_TEXT("fec0::0123:4567:89ab:cdef"),
    ACE_TEXT("3ffe::1:0123:4567:89ab:cdef"),
    ACE_TEXT("2002:3e02:5473::")
  };

  // The port will always be this
  u_short port = 80;

  // ... and as you can see, there are 5 of them
  const size_t num_secondaries = 5;

  // Test subject
  ACE_Multihomed_INET_Addr addr;

  // Array of INET_Addrs that will repeatedly be passed into the
  // get_secondary_addresses accessor of Multihomed_INET_Addr
  ACE_INET_Addr in_out[5];

  // Array of INET_Addrs against which the above array will be tested.
  ACE_INET_Addr stay_out[5];

  // Array of sockaddrs that will repeatedly be passed into the
  // get_addresses accessor of Multihomed_INET_Addr
  const size_t num_sockaddrs = 6;
  sockaddr_in6 in_out_sockaddr6[num_sockaddrs];

  for (i = 0; i <= num_secondaries; ++i) {


    /**** Test set (u_short, const char[], int, int, const char *([]), size_t) ****/


    addr.set(port,
             primary_ipv6,
             1,
             AF_INET6,
             secondary_ipv6,
             i);

    // Check the port number
    if (addr.get_port_number() != port) {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Failed second get_port_number check\n")
                  ACE_TEXT ("%d != %d\n"),
                  addr.get_port_number(),
                  port));
      status = 1;
    }

    // Check the primary address
    if (0 != ACE_OS::strcmp (ACE_TEXT_CHAR_TO_TCHAR(addr.get_host_addr()), primary_ipv6))
      {
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("%s failed get_host_addr() check\n")
                    ACE_TEXT ("%s != %s\n"),
                    primary_ipv6,
                    addr.get_host_addr (),
                    primary_ipv6));
        status = 1;
      }

    // Check that the test subject reports the correct number of
    // secondary addresses.
    size_t returned_num_secondaries = addr.get_num_secondary_addresses();
    if (returned_num_secondaries == i) {

      // Initialize the stay_out array with the secondary addresses
      for (j = 0; j < i; ++j) {
        stay_out[j].set(port, secondary_ipv6[j]);
      }

      // Pass the in_out array to the accessor
      addr.get_secondary_addresses(in_out, i);

      // Check that the in_out array matches stay_out array
      for (j = 0; j < i; ++j) {

        if (in_out[j] != stay_out[j]) {

          ACE_TCHAR in_out_string[100];
          ACE_TCHAR stay_out_string[100];

          in_out[j].addr_to_string(in_out_string, 100);
          stay_out[j].addr_to_string(stay_out_string, 100);

          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("Failed get_secondary_addresses check\n")
                      ACE_TEXT ("%s != %s\n"),
                      in_out_string,
                      stay_out_string));

          status = 1;
        }
      }

      // Pass the in_out_sockaddr array to the accessor
      addr.get_addresses(in_out_sockaddr6, i + 1);

      // Check that the primary address in the in_out_sockaddr array
      // matches the primary address reported by the superclass
      if (ACE_OS::memcmp(in_out_sockaddr6, addr.get_addr(),
                         sizeof(sockaddr_in6))) {

          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("Failed second get_addresses check ")
                      ACE_TEXT ("(for primary address)\n")));

          status = 1;

      }

      // Check that the secondary addresses in the in_out_sockaddr
      // array match the stay_out array
      for (j = 1, pointer6 = &in_out_sockaddr6[1];
           j < i + 1;
           ++j, ++pointer6) {

        if (ACE_OS::memcmp(pointer6, stay_out[j-1].get_addr(),
                           sizeof(sockaddr_in6))) {

          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("Failed get_addresses check ")
                      ACE_TEXT ("(for secondary addresses)\n")));

          status = 1;
        }
      }
    } else {

      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Failed get_num_secondary_addresses check\n")
                  ACE_TEXT ("%d != %d\n"),
                  returned_num_secondaries,
                  i));
      status = 1;
    }
  }
#endif /* ACE_HAS_IPV6 */

  ACE_END_TEST;
  return status;

}
