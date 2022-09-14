/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Multicast_Interfaces_Test.cpp
 *
 *   This is a sanity-check test of ACE_SOCK_Dgram_Mcast::join by
 *   interface name on platforms that support it.
 *   It retrieves the valid local interface names and attempts to
 *   perform a multicast join on all interfaces and then on each
 *   individual interface separately. If IPv6 is enabled, it will
 *   attempt to join on an IPv6 multicast address as well.
 *
 *  @author Timothy Simpson <simpsont@objectcomputing.com>
 */
//=============================================================================

#include "test_config.h"

#include "ace/OS_Memory.h"
#include "ace/OS_NS_sys_utsname.h"
#include "ace/SOCK_Dgram_Mcast.h"
#include "ace/SString.h"

#if defined (ACE_HAS_GETIFADDRS)
#  include "ace/os_include/os_ifaddrs.h"
#endif /* ACE_HAS_GETIFADDRS */

#include <set>

using nameset = std::set<ACE_TString>;

#if defined (ACE_WIN32)
void
get_valid_ipv4_interface_names_win32 (nameset &names)
{
  names.clear ();

  // Initial call to determine actual memory size needed
  IP_ADAPTER_ADDRESSES tmp_addrs;
  ULONG bufLen = 0;
  if (GetAdaptersAddresses (AF_INET, 0, 0, &tmp_addrs, &bufLen) != ERROR_BUFFER_OVERFLOW)
    {
      return;
    }

  // Get required output buffer and retrieve info for real.
  char *buf = 0;
  ACE_NEW (buf, char[bufLen]);
  PIP_ADAPTER_ADDRESSES pAddrs = reinterpret_cast<PIP_ADAPTER_ADDRESSES> (buf);
  if (::GetAdaptersAddresses (AF_INET, 0, 0, pAddrs, &bufLen) != NO_ERROR)
    {
      delete[] buf;
      return;
    }

  while (pAddrs)
    {
      if (pAddrs->OperStatus == IfOperStatusUp && !(pAddrs->Flags & IP_ADAPTER_NO_MULTICAST))
        {
          PIP_ADAPTER_UNICAST_ADDRESS pUnicast = pAddrs->FirstUnicastAddress;
          LPSOCKADDR sa = pUnicast->Address.lpSockaddr;
          if (sa->sa_family == AF_INET)
            {
              names.insert (ACE_TEXT_WCHAR_TO_TCHAR (pAddrs->FriendlyName));
            }
        }
      pAddrs = pAddrs->Next;
    }

  delete[] buf;
}
#elif defined (ACE_HAS_GETIFADDRS)
void
get_valid_ipv4_interface_names_getifaddrs (nameset &names)
{
  ifaddrs *ifap = 0;
  ifaddrs *p_if = 0;

  if (::getifaddrs (&ifap) != 0)
    return;

  for (p_if = ifap; p_if != 0; p_if = p_if->ifa_next)
    {
      if (p_if->ifa_flags & IFF_MULTICAST &&
          p_if->ifa_addr->sa_family == AF_INET)
        {
          sockaddr_in *addr = reinterpret_cast<sockaddr_in *> (p_if->ifa_addr);

          if (addr->sin_addr.s_addr != INADDR_ANY)
            {
              names.insert (ACE_TEXT_CHAR_TO_TCHAR (p_if->ifa_name));
            }
        }
    }

  ::freeifaddrs (ifap);
}
#endif /* ACE_WIN32 */

void
get_valid_ipv4_interface_names (nameset &names)
{
#if defined (ACE_WIN32)
  get_valid_ipv4_interface_names_win32 (names);
#elif defined (ACE_HAS_GETIFADDRS)
  get_valid_ipv4_interface_names_getifaddrs (names);
#endif /* ACE_WIN32 */
}

#if defined (ACE_HAS_IPV6)
#if defined (ACE_WIN32)
void
get_valid_ipv6_interface_names_win32 (nameset &names)
{
  names.clear ();

  // Initial call to determine actual memory size needed
  IP_ADAPTER_ADDRESSES tmp_addrs;
  ULONG bufLen = 0;
  if (GetAdaptersAddresses (AF_INET, 0, 0, &tmp_addrs, &bufLen) != ERROR_BUFFER_OVERFLOW)
    {
      return;
    }

  // Get required output buffer and retrieve info for real.
  char *buf = 0;
  ACE_NEW (buf, char[bufLen]);
  PIP_ADAPTER_ADDRESSES pAddrs = reinterpret_cast<PIP_ADAPTER_ADDRESSES> (buf);
  if (::GetAdaptersAddresses (AF_INET6, 0, 0, pAddrs, &bufLen) != NO_ERROR)
    {
      delete[] buf;
      return;
    }

  while (pAddrs)
    {
      if (pAddrs->OperStatus == IfOperStatusUp && !(pAddrs->Flags & IP_ADAPTER_NO_MULTICAST))
        {
          PIP_ADAPTER_UNICAST_ADDRESS pUnicast = pAddrs->FirstUnicastAddress;
          LPSOCKADDR sa = pUnicast->Address.lpSockaddr;
          if (sa->sa_family == AF_INET6)
            {
              names.insert (ACE_TEXT_WCHAR_TO_TCHAR (pAddrs->FriendlyName));
            }
        }
      pAddrs = pAddrs->Next;
    }

  delete[] buf;
}
#elif defined (ACE_HAS_GETIFADDRS)
void
get_valid_ipv6_interface_names_getifaddrs (nameset &names)
{
  ifaddrs *ifap = 0;
  ifaddrs *p_if = 0;

  if (::getifaddrs (&ifap) != 0)
    return;

  for (p_if = ifap; p_if != 0; p_if = p_if->ifa_next)
    {
      if (p_if->ifa_flags & IFF_MULTICAST &&
          p_if->ifa_addr->sa_family == AF_INET6)
        {
          sockaddr_in6 *addr = reinterpret_cast<sockaddr_in6 *> (p_if->ifa_addr);

          if (!IN6_IS_ADDR_UNSPECIFIED (&addr->sin6_addr))
            {
              names.insert (ACE_TEXT_CHAR_TO_TCHAR (p_if->ifa_name));
            }
        }
    }

  ::freeifaddrs (ifap);
}
#endif /*ACE_WIN32 */

void
get_valid_ipv6_interface_names (nameset &names)
{
#if defined (ACE_WIN32)
  get_valid_ipv6_interface_names_win32 (names);
#elif defined (ACE_HAS_GETIFADDRS)
  get_valid_ipv6_interface_names_getifaddrs (names);
#endif /* ACE_WIN32 */
}
#endif /* ACE_HAS_IPV6 */

int
create_socket_and_join_multicast (const ACE_INET_Addr &mc_addr, const ACE_TString &if_name)
{
  int result = 0;
  ACE_SOCK_Dgram_Mcast sock;
  sock.opts (ACE_SOCK_Dgram_Mcast::OPT_BINDADDR_NO | ACE_SOCK_Dgram_Mcast::DEFOPT_NULLIFACE);
  result = sock.join (mc_addr, 1, if_name.empty () ? 0 : if_name.c_str ());
  result |= sock.close ();
  return result;
}

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Multicast_Interfaces_Test"));

  int result = 0;

  ACE_utsname uname;
  ACE_OS::uname (&uname);
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Machine: %C running on %C\n"),
              uname.nodename, uname.machine ));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Platform: %C, %C, %C\n"),
              uname.sysname, uname.release, uname.version ));

  nameset names;

  get_valid_ipv4_interface_names (names);
  ACE_INET_Addr ipv4_mc_addr ("239.255.0.7:1234", AF_INET);
  result |= create_socket_and_join_multicast (ipv4_mc_addr, ACE_TString ());
  for (nameset::const_iterator it = names.begin (); result == 0 && it != names.end (); ++it)
    {
      result |= create_socket_and_join_multicast (ipv4_mc_addr, *it);
    }

#if defined (ACE_HAS_IPV6)
  names.clear ();
  get_valid_ipv6_interface_names (names);
  ACE_INET_Addr ipv6_mc_addr ("ff03::7:4321", AF_INET6);
  result |= create_socket_and_join_multicast (ipv6_mc_addr, ACE_TString ());
  for (nameset::const_iterator it = names.begin (); result == 0 && it != names.end (); ++it)
    {
      result |= create_socket_and_join_multicast (ipv6_mc_addr, *it);
    }
#endif /* ACE_HAS_IPV6 */

  ACE_END_TEST;
  return result;
}
