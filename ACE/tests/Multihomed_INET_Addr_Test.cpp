
//=============================================================================
/**
 *  @file    Multihomed_INET_Addr_Test.cpp
 *
 *  $Id$
 *
 *   Performs several tests on the Multihomed_ACE_INET_Addr class.
 *   It creates several IPv4 addresses and checks that the
 *   address formed by the class is valid.
 *
 *
 *  @author Edward Mulholland (emulholl@atl.lmco.com)
 */
//=============================================================================


#include "test_config.h"
#include "ace/OS_NS_string.h"
#include "ace/Multihomed_INET_Addr.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_arpa_inet.h"

int run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Multihomed_INET_Addr_Test"));

  int status = 0;     // Innocent until proven guilty

  // loop variables
  size_t i, j;
  sockaddr_in *pointer;

  // The port will always be this
  u_short port = 80;

  // The primary address will always be this
  const char *primary_dotted_decimal = "138.38.180.251";

  // The secondary addresses will always be these...
  const char *secondary_dotted_decimals[] = {
    "64.219.54.121",
    "127.0.0.1",
    "21.242.14.51",
    "53.141.124.24",
    "42.12.44.9"
  };

  // ... and as you can see, there are 5 of them
  const size_t num_secondaries = 5;

  // We also need the primary address and the secondary addresses
  // in ACE_UINT32 format in host byte order
  ACE_UINT32 primary_addr32;
  ACE_UINT32 secondary_addr32[5];

  {
    struct in_addr addrv4;
    ACE_OS::memset ((void *) &addrv4, 0, sizeof addrv4);
    ACE_OS::inet_pton (AF_INET, primary_dotted_decimal, &addrv4);
    ACE_OS::memcpy (&primary_addr32, &addrv4, sizeof (primary_addr32));
    primary_addr32 = ACE_NTOHL(primary_addr32);
  }

  for (i = 0; i < num_secondaries; ++i) {
    struct in_addr addrv4;
    ACE_OS::memset ((void *) &addrv4, 0, sizeof addrv4);
    ACE_OS::inet_pton (AF_INET, secondary_dotted_decimals[i], &addrv4);
    ACE_OS::memcpy (&secondary_addr32[i], &addrv4, sizeof (primary_addr32));
    secondary_addr32[i] = ACE_NTOHL(secondary_addr32[i]);
  }

  // Test subject
  ACE_Multihomed_INET_Addr addr;

  // Array of ones (used to clear the secondary addresses of the test
  // subject)
  ACE_UINT32 array_of_threes[5] = { ACE_UINT32 (3),
                                    ACE_UINT32 (3),
                                    ACE_UINT32 (3),
                                    ACE_UINT32 (3),
                                    ACE_UINT32 (3) };

  // Array of INET_Addrs that will repeatedly be passed into the
  // get_secondary_addresses accessor of Multihomed_INET_Addr
  ACE_INET_Addr in_out[5];

  // Array of INET_Addrs against which the above array will be tested.
  ACE_INET_Addr stay_out[5];

  // Array of sockaddrs that will repeatedly be passed into the
  // get_addresses accessor of Multihomed_INET_Addr
  const size_t num_sockaddrs = 6;
  sockaddr_in in_out_sockaddr[num_sockaddrs];

  // Run the test with a varying number of secondary addresses
  for (i = 0; i <= num_secondaries; ++i)  {


    /****** Clear the in_out array and test subject ******/


    // Clear the in_out array by setting every port to 0 and every
    // address to 1
    for (j = 0; j < num_secondaries; ++j)  {
      in_out[j].set(0, ACE_UINT32 (1), 1);
    }

    // Clear the in_out_sockaddr array by setting every port to 0 and
    // every address to 1
    ACE_OS::memset(in_out_sockaddr, 0, num_sockaddrs * sizeof(sockaddr));

    // Clear the test subject by setting the port to 2 and every
    // address (both the primary and the secondaries) to 3
    addr.set (2, ACE_UINT32 (3), 1, array_of_threes, num_secondaries);

    // Check that the port is 2
    if (addr.get_port_number() != 2) {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Failed get_port_number check\n")
                  ACE_TEXT ("%d != %d\n"),
                  addr.get_port_number(),
                  2));
      status = 1;
    }

    // Check that the primary address is 3
    if (addr.get_ip_address() != ACE_UINT32 (3)) {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Failed get_ip_address check\n")
                  ACE_TEXT ("0x%x != 0x%x\n"),
                  addr.get_ip_address(),
                  ACE_UINT32 (3)));
      status = 1;
    }

    // Check that the test subject reports the correct number of
    // secondary addresses.
    size_t returned_num_secondaries = addr.get_num_secondary_addresses();
    if (returned_num_secondaries == num_secondaries) {

      // Set a stay_out element to the state that we expect to see
      // from every in_out element after the in_out array is passed to
      // the accessor of the test subject.
      stay_out[0].set(2, ACE_UINT32 (3), 1);

      // Pass the in_out array to the accessor
      addr.get_secondary_addresses(in_out, num_secondaries);

      // Check that the in_out array matches stay_out element
      for (j = 0; j < num_secondaries; ++j) {

        if (in_out[j] != stay_out[0]) {

          ACE_TCHAR in_out_string[100];
          ACE_TCHAR stay_out_string[100];

          in_out[j].addr_to_string(in_out_string, 100);
          stay_out[0].addr_to_string(stay_out_string, 100);

          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("Failed get_secondary_addresses check\n")
                      ACE_TEXT ("%s != %s\n"),
                      in_out_string,
                      stay_out_string));

          status = 1;
        }
      }

      // Pass the in_out_sockaddr array to the accessor
      addr.get_addresses(in_out_sockaddr, num_secondaries + 1);

      // Check that the in_out_sockaddr array matches stay_out element
      for (j = 0, pointer = in_out_sockaddr;
           j < num_secondaries + 1;
           ++j, ++pointer) {

        if (ACE_OS::memcmp(pointer, stay_out[0].get_addr(), sizeof(sockaddr))) {

          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("Failed get_addresses check\n")));

          status = 1;
        }
      }

    } else {

        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("Failed get_num_secondary_addresses check\n")
                    ACE_TEXT ("%d != %d\n"),
                    returned_num_secondaries,
                    num_secondaries));
        status = 1;

    }


    /**** Test set (u_short, const char[], int, int, const char *([]), size_t) ****/


    addr.set(port,
             primary_dotted_decimal,
             1,
             AF_INET,
             secondary_dotted_decimals,
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
    if (0 != ACE_OS::strcmp (addr.get_host_addr(), primary_dotted_decimal))
      {
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("%s failed get_host_addr() check\n")
                    ACE_TEXT ("%s != %s\n"),
                    primary_dotted_decimal,
                    addr.get_host_addr (),
                    primary_dotted_decimal));
        status = 1;
      }

    // Check that the test subject reports the correct number of
    // secondary addresses.
    returned_num_secondaries = addr.get_num_secondary_addresses();
    if (returned_num_secondaries == i) {

      // Initialize the stay_out array with the secondary addresses
      for (j = 0; j < i; ++j) {
        stay_out[j].set(port, secondary_dotted_decimals[j]);
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
                      ACE_TEXT ("Failed second get_secondary_addresses check\n")
                      ACE_TEXT ("%s != %s\n"),
                      in_out_string,
                      stay_out_string));

          status = 1;
        }
      }

      // Pass the in_out_sockaddr array to the accessor
      addr.get_addresses(in_out_sockaddr, i + 1);

      // Check that the primary address in the in_out_sockaddr array
      // matches the primary address reported by the superclass
      if (ACE_OS::memcmp(in_out_sockaddr, addr.get_addr(), sizeof(sockaddr))) {

          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("Failed second get_addresses check ")
                      ACE_TEXT ("(for primary address)\n")));

          status = 1;

      }

      // Check that the secondary addresses in the in_out_sockaddr
      // array match the stay_out array
      for (j = 1, pointer = &in_out_sockaddr[1];
           j < i + 1;
           ++j, ++pointer) {

        if (ACE_OS::memcmp(pointer, stay_out[j-1].get_addr(), sizeof(sockaddr))) {

          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("Failed second get_addresses check ")
                      ACE_TEXT ("(for secondary addresses)\n")));

          status = 1;
        }
      }

    } else {

      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Failed second get_num_secondary_addresses check\n")
                  ACE_TEXT ("%d != %d\n"),
                  returned_num_secondaries,
                  i));
      status = 1;
    }


    /****** Clear the in_out array and test subject AGAIN ******/


    // Clear the in_out array by setting every port to 0 and every
    // address to 1
    for (j = 0; j < num_secondaries; ++j)  {
      in_out[j].set(0, ACE_UINT32 (1), 1);
    }

    // Clear the test subject by setting the port to 2 and every
    // address (both the primary and the secondaries) to 3
    addr.set (2, ACE_UINT32 (3), 1, array_of_threes, num_secondaries);

    // Check that the port is 2
    if (addr.get_port_number() != 2) {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Failed third get_port_number check\n")
                  ACE_TEXT ("%d != %d\n"),
                  addr.get_port_number(),
                  2));
      status = 1;
    }

    // Check that the primary address is 3
    if (addr.get_ip_address() != ACE_UINT32 (3)) {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Failed third get_ip_address check\n")
                  ACE_TEXT ("0x%x != 0x%x\n"),
                  addr.get_ip_address(),
                  ACE_UINT32 (3)));
      status = 1;
    }

    // Check that the test subject reports the correct number of
    // secondary addresses.
    returned_num_secondaries = addr.get_num_secondary_addresses();
    if (returned_num_secondaries == num_secondaries) {

      // Set a stay_out element to the state that we expect to see
      // from every in_out element after the in_out array is passed to
      // the accessor of the test subject.
      stay_out[0].set(2, ACE_UINT32 (3), 1);

      // Pass the in_out array to the accessor
      addr.get_secondary_addresses(in_out, num_secondaries);

      // Check that the in_out array matches stay_out array
      for (j = 0; j < num_secondaries; ++j) {

        if (in_out[j] != stay_out[0]) {

          ACE_TCHAR in_out_string[100];
          ACE_TCHAR stay_out_string[100];

          in_out[j].addr_to_string(in_out_string, 100);
          stay_out[0].addr_to_string(stay_out_string, 100);

          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("Failed third get_secondary_addresses check\n")
                      ACE_TEXT ("%s != %s\n"),
                      in_out_string,
                      stay_out_string));

          status = 1;
        }
      }

    } else {

        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("Failed third get_num_secondary_addresses check\n")
                    ACE_TEXT ("%d != %d\n"),
                    returned_num_secondaries,
                    num_secondaries));
        status = 1;

    }


    /**** Test set (u_short, ACE_UINT32, int, const ACE_UINT32 *, size_t) ****/

    addr.set(port,
             primary_addr32,
             1,
             secondary_addr32,
             i);

    // Check the port number
    if (addr.get_port_number() != port) {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Failed forth get_port_number check\n")
                  ACE_TEXT ("%d != %d\n"),
                  addr.get_port_number(),
                  port));
      status = 1;
    }

    // Check the primary address
    if (0 != ACE_OS::strcmp (addr.get_host_addr(), primary_dotted_decimal))
      {
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("%s failed second get_ip_address() check\n")
                    ACE_TEXT ("%s != %s\n"),
                    primary_dotted_decimal,
                    addr.get_host_addr (),
                    primary_dotted_decimal));
        status = 1;
      }

    // Check that the test subject reports the correct number of
    // secondary addresses.
    returned_num_secondaries = addr.get_num_secondary_addresses();
    if (returned_num_secondaries == i) {

      // Initialize the stay_out array with the secondary addresses
      for (j = 0; j < i; ++j) {
        stay_out[j].set(port, secondary_addr32[j]);
      }

      // Pass the in_out array to the accessor
      addr.get_secondary_addresses(in_out, j);

      // Check that the in_out array matches stay_out array
      for (j = 0; j < i; ++j) {

        if (in_out[j] != stay_out[j]) {

          ACE_TCHAR in_out_string[100];
          ACE_TCHAR stay_out_string[100];

          in_out[j].addr_to_string(in_out_string, 100);
          stay_out[j].addr_to_string(stay_out_string, 100);

          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("Failed forth get_secondary_addresses check\n")
                      ACE_TEXT ("%s != %s\n"),
                      in_out_string,
                      stay_out_string));

          status = 1;
        }
      }

    } else {

      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Failed forth get_num_secondary_addresses check\n")
                  ACE_TEXT ("%d != %d\n"),
                  returned_num_secondaries,
                  i));
      status = 1;
    }

  }

  ACE_END_TEST;
  return status;
}
