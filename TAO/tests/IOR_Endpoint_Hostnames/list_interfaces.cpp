//
// Extract the address for each network interface using EXACTLY
// the same algorithm as TAO_IIOP_Acceptor, and then print
// it.  This is used by the run_test.pl to know how to set up
// "expected results" for hostnames in IORs.
//

#include "ace/ACE.h"
#include "ace/Auto_Ptr.h"
#include "ace/INET_Addr.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_stdio.h"

ACE_RCSID (IOR_Endpoint_Hostnames, list_interfaces, "$Id$")

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  ACE_UNUSED_ARG (argc);
  ACE_UNUSED_ARG (argv);

  // network interfaces.
  ACE_INET_Addr *if_addrs = 0;
  size_t if_cnt = 0;

  unsigned long endpoint_count;

  if (ACE::get_ip_interfaces (if_cnt,
                              if_addrs) != 0
      && errno != ENOTSUP)
    {
      // In the case where errno == ENOTSUP, if_cnt and if_addrs will
      // not be modified, and will each remain equal to zero.  This
      // causes the default interface to be used.
      return -1;
    }

  if (if_cnt == 0 || if_addrs == 0)
    {
      ACE_DEBUG ((LM_WARNING,
                  ACE_TEXT ("TAO (%P|%t) Unable to probe network ")
                  ACE_TEXT ("interfaces.  Using default.\n")));

      if_cnt = 1; // Force the network interface count to be one.
      delete [] if_addrs;
      if_addrs = new ACE_INET_Addr[if_cnt];
    }

  // Scan for the loopback interface since it shouldn't be included in
  // the list of cached hostnames unless it is the only interface.
  size_t lo_cnt = 0;  // Loopback interface count
  for (size_t j = 0; j < if_cnt; ++j)
    if (if_addrs[j].get_ip_address () == INADDR_LOOPBACK)
      lo_cnt++;

  ACE_Auto_Basic_Array_Ptr<ACE_INET_Addr> safe_if_addrs (if_addrs);

  // If the loopback interface is the only interface then include it
  // in the list of interfaces to query for a hostname, otherwise
  // exclude it from the list.
  if (if_cnt == lo_cnt)
    endpoint_count = static_cast<unsigned long> (if_cnt);
  else
    endpoint_count = static_cast<unsigned long> (if_cnt - lo_cnt);

  // The number of hosts/interfaces we want to cache may not be the
  // same as the number of detected interfaces so keep a separate
  // count.
  size_t host_cnt = 0;

  for (size_t i = 0; i < if_cnt; ++i)
    {
      // Ignore any loopback interface if there are other
      // non-loopback interfaces.
      if (if_cnt != lo_cnt &&
          if_addrs[i].get_ip_address() == INADDR_LOOPBACK)
        continue;

      // Print the address as a string.
      ACE_OS::printf ("%s\n", if_addrs[i].get_host_addr());

      host_cnt++;
    }

  return 0;
}
