// -*- C++ -*-
// $Id$

#include "ace/OS_NS_arpa_inet.h"

ACE_RCSID(ace, OS_NS_arpa_inet, "$Id$")

#if !defined (ACE_HAS_INLINED_OSCALLS)
# include "ace/OS_NS_arpa_inet.inl"
#endif /* ACE_HAS_INLINED_OS_CALLS */

int
ACE_OS::inet_aton (const char *host_name, struct in_addr *addr)
{
#if defined (ACE_LACKS_INET_ATON)
  ACE_UINT32 ip_addr = ACE_OS::inet_addr (host_name);

  if (ip_addr == INADDR_NONE
      // Broadcast addresses are weird...
      && ACE_OS::strcmp (host_name, "255.255.255.255") != 0)
    return 0;
  else if (addr == 0)
    return 0;
  else
    {
      addr->s_addr = ip_addr;  // Network byte ordered
      return 1;
    }
#elif defined (VXWORKS)
  // inet_aton() returns OK (0) on success and ERROR (-1) on failure.
  // Must reset errno first. Refer to WindRiver SPR# 34949, SPR# 36026
  ::errnoSet(0);
  int result = ERROR;
  ACE_OSCALL (::inet_aton ((char*)host_name, addr), int, ERROR, result);
  return (result == ERROR) ? 0 : 1;
#else
  // inet_aton() returns 0 upon failure, not -1 since -1 is a valid
  // address (255.255.255.255).
  ACE_OSCALL_RETURN (::inet_aton (host_name, addr), int, 0);
#endif  /* ACE_LACKS_INET_ATON */
}

// All other platforms have this inlined in OS_NS_arpa_inet.inl
#if defined (ACE_PSOS)
char *
ACE_OS::inet_ntoa (const struct in_addr addr)
{
  ACE_OS_TRACE ("ACE_OS::inet_ntoa");

  static char addrstr[INET_ADDRSTRLEN + 1] = { 0 };
  ACE_UINT32 ipaddr = ntohl (addr.s_addr);
  //printf("Socket address %X, IP address %X.\n",addr.s_addr,ipaddr);
  sprintf(addrstr, "%d.%d.%d.%d",
          ((ipaddr & 0xff000000) >> 24) & 0x000000ff,
          (ipaddr & 0x00ff0000) >> 16,
          (ipaddr & 0x0000ff00) >> 8,
          (ipaddr & 0x000000ff));
  return addrstr;
}
#endif /* defined (ACE_PSOS) */
