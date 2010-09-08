//
// Extract the address for each network interface using EXACTLY
// the same algorithm as TAO_IIOP_Acceptor, and then print
// it.  This is used by the run_test.pl to know how to set up
// "expected results" for hostnames in IORs.
//

#include "tao/corba.h"
#include "ace/ACE.h"
#include "ace/Auto_Ptr.h"
#include "ace/INET_Addr.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_stdio.h"

#if defined (ACE_HAS_IPV6) && !defined (ACE_USES_IPV4_IPV6_MIGRATION)
ACE_INET_Addr default_address (static_cast<unsigned short> (0), ACE_IPV6_ANY, AF_INET6);
#else
ACE_INET_Addr default_address (static_cast<unsigned short> (0), static_cast<ACE_UINT32> (INADDR_ANY));
#endif /* ACE_HAS_IPV6  && !ACE_USES_IPV4_IPV6_MIGRATION */

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  ACE_UNUSED_ARG (argc);
  ACE_UNUSED_ARG (argv);

  CORBA::ULong endpoint_count;
#if defined (ACE_HAS_IPV6)
  int def_type = AF_UNSPEC;
#endif /* ACE_HAS_IPV6 */

  // network interfaces.
  ACE_INET_Addr *if_addrs = 0;
  size_t if_cnt = 0;

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
      ACE_NEW_RETURN (if_addrs,
                      ACE_INET_Addr[if_cnt],
                      -1);
    }

  // Scan for the loopback interface since it shouldn't be included in
  // the list of cached hostnames unless it is the only interface.
  size_t lo_cnt = 0;  // Loopback interface count
  for (size_t j = 0; j < if_cnt; ++j)
    if (if_addrs[j].is_loopback ())
      ++lo_cnt;

#if defined (ACE_HAS_IPV6)
  size_t ipv4_cnt = 0;
  size_t ipv4_lo_cnt = 0;
  size_t ipv6_ll = 0;
  bool ipv6_non_ll = false;
  // Scan for IPv4 interfaces since these should not be included
  // when IPv6-only is selected.
  for (size_t j = 0; j < if_cnt; ++j)
    if (if_addrs[j].get_type () != AF_INET6 ||
        if_addrs[j].is_ipv4_mapped_ipv6 ())
      {
        ++ipv4_cnt;
        if (if_addrs[j].is_loopback ())
          ++ipv4_lo_cnt;  // keep track of IPv4 loopback ifs
      }
    else if (!if_addrs[j].is_linklocal () &&
             !if_addrs[j].is_loopback())
      {
        ipv6_non_ll = true; // we have at least 1 non-local IPv6 if
      }
    else if (// !orb_core->orb_params ()->use_ipv6_link_local () &&
             if_addrs[j].is_linklocal ())
      {
        ++ipv6_ll;  // count link local addrs to exclude them afterwards
      }
#endif /* ACE_HAS_IPV6 */

  ACE_Auto_Basic_Array_Ptr<ACE_INET_Addr> safe_if_addrs (if_addrs);

#if defined (ACE_HAS_IPV6)
  bool ipv4_only = def_type == AF_INET;
  bool ipv6_only = (def_type == AF_INET6); // ||
  //    orb_core->orb_params ()->connect_ipv6_only ();
#if defined (ACE_WIN32)
  if (default_address.get_type () == AF_INET)
    ipv4_only = true;
  else
    ipv6_only = true;
#endif /* ACE_WIN32 */
  // If the loopback interface is the only interface then include it
  // in the list of interfaces to query for a hostname, otherwise
  // exclude it from the list.
  bool ignore_lo;
  if (ipv6_only)
    // only exclude loopback if non-local if exists
    ignore_lo = ipv6_non_ll;
  else if (ipv4_only)
    ignore_lo = ipv4_cnt != ipv4_lo_cnt;
  else
    ignore_lo = if_cnt != lo_cnt;

  // Adjust counts for IPv6 only if required
  size_t if_ok_cnt = if_cnt;
  if (ipv6_only)
    {
      if_ok_cnt -= ipv4_cnt;
      lo_cnt -= ipv4_lo_cnt;
      ipv4_lo_cnt = 0;
    }
  else if (ipv4_only)
    {
      if_ok_cnt = ipv4_cnt;
      lo_cnt = ipv4_lo_cnt;
      ipv6_ll = 0;
    }

  // In case there are no non-local IPv6 ifs in the list only exclude
  // IPv4 loopback.
  // IPv6 loopback will be needed to successfully connect IPv6 clients
  // in a localhost environment.
  if (!ipv4_only && !ipv6_non_ll)
    lo_cnt = ipv4_lo_cnt;

  if (!ignore_lo)
    endpoint_count = static_cast<CORBA::ULong> (if_ok_cnt - ipv6_ll);
  else
    endpoint_count = static_cast<CORBA::ULong> (if_ok_cnt - ipv6_ll - lo_cnt);
#else /* ACE_HAS_IPV6 */
  // If the loopback interface is the only interface then include it
  // in the list of interfaces to query for a hostname, otherwise
  // exclude it from the list.
  bool ignore_lo;
  ignore_lo = if_cnt != lo_cnt;
  if (!ignore_lo)
    endpoint_count = static_cast<CORBA::ULong> (if_cnt);
  else
    endpoint_count = static_cast<CORBA::ULong> (if_cnt - lo_cnt);
#endif /* !ACE_HAS_IPV6 */

  // The number of hosts/interfaces we want to cache may not be the
  // same as the number of detected interfaces so keep a separate
  // count.
  size_t host_cnt = 0;

  for (size_t i = 0; i < if_cnt; ++i)
    {
#if defined (ACE_HAS_IPV6)
      // Ignore any loopback interface if there are other
      // non-loopback interfaces.
      if (ignore_lo &&
          if_addrs[i].is_loopback () &&
          (ipv4_only ||
           ipv6_non_ll ||
           if_addrs[i].get_type () != AF_INET6))
        continue;

      // Ignore any non-IPv4 interfaces when so required.
      if (ipv4_only &&
          (if_addrs[i].get_type () != AF_INET))
        continue;

      // Ignore any non-IPv6 interfaces when so required.
      if (ipv6_only &&
          (if_addrs[i].get_type () != AF_INET6 ||
           if_addrs[i].is_ipv4_mapped_ipv6 ()))
        continue;

      // Ignore all IPv6 link local interfaces when so required.
      if (// !orb_core->orb_params ()->use_ipv6_link_local () &&
          if_addrs[i].is_linklocal ())
        continue;
#else /* ACE_HAS_IPV6 */
      // Ignore any loopback interface if there are other
      // non-loopback interfaces.
      if (ignore_lo &&
          if_addrs[i].is_loopback ())
        continue;
#endif /* !ACE_HAS_IPV6 */

      // Print the address as a string.
      ACE_OS::printf ("%s\n", if_addrs[i].get_host_addr());

      ++host_cnt;
    }

  return 0;
}
