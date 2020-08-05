/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Enum_Interfaces_Test.cpp
 *
 *   This is a simple test of <ACE::get_ip_interfaces>.  This call
 *   retrieves the IP addresses assigned to the host by
 *   interrogating the kernel.  Network applications typically
 *   assume gethostbyname(uname()) will work, but this is just a
 *   convention. It is also problematic if the resolver code
 *   (DNS/NIS+...) is misconfigured. This happens more than
 *   programmers realize. It is better to find out by asking the
 *   kernel for local address assignments. This API is similar to
 *   what netstat -ni or ifconfig -a produces on UNIX or ipconfig on
 *   Windows NT. In fact, it was by reverse engineering these tools
 *   that this api was created.
 *
 *  @author Michael R. MacFaden <mrm@cisco.com>
 */
//=============================================================================

#include "test_config.h"

#include "ace/OS_Memory.h"
#include "ace/OS_NS_sys_utsname.h"
#include "ace/SOCK_Dgram_Mcast.h"
#include "ace/SString.h"

#include <set>
#include <string>

typedef std::set<ACE_TString> nameset;

#ifdef ACE_WIN32
void get_valid_ipv4_interface_names_win32(nameset& names) {
  names.clear();

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
      if (!pAddrs->NoMulticast)
        {
          PIP_ADAPTER_UNICAST_ADDRESS_LH pUnicast = pAddrs->FirstUnicastAddress;
          LPSOCKADDR sa = pUnicast->Address.lpSockaddr;
          if (sa->sa_family == AF_INET)
            {
              names.insert(ACE_TEXT_WCHAR_TO_TCHAR(pAddrs->FriendlyName));
            }
        }
      pAddrs = pAddrs->Next;
    }

  delete[] buf;
}
#elif defined ACE_HAS_GETIFADDRS
void get_valid_ipv4_interface_names_getifaddrs(nameset& names) {
}
#endif

void get_valid_ipv4_interface_names(nameset& names) {
#ifdef ACE_WIN32
get_valid_ipv4_interface_names_win32(names);
#elif defined ACE_HAS_GETIFADDRS
get_valid_ipv4_interface_names_getifaddrs(names);
#endif
}

#ifdef ACE_HAS_IPV6
#ifdef ACE_WIN32
void get_valid_ipv6_interface_names_win32(nameset& names) {
  names.clear();

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
      if (!pAddrs->NoMulticast)
        {
          PIP_ADAPTER_UNICAST_ADDRESS_LH pUnicast = pAddrs->FirstUnicastAddress;
          LPSOCKADDR sa = pUnicast->Address.lpSockaddr;
          if (sa->sa_family == AF_INET6)
            {
              names.insert(ACE_TEXT_WCHAR_TO_TCHAR(pAddrs->FriendlyName));
            }
        }
      pAddrs = pAddrs->Next;
    }

  delete[] buf;
}
#elif defined ACE_HAS_GETIFADDRS
void get_valid_ipv6_interface_names_getifaddrs(nameset& names) {
}
#endif

void get_valid_ipv6_interface_names(nameset& names) {
#ifdef ACE_WIN32
get_valid_ipv6_interface_names_win32(names);
#elif defined ACE_HAS_GETIFADDRS
get_valid_ipv6_interface_names_getifaddrs(names);
#endif
}
#endif /* ACE_HAS_IPV6 */

int create_socket_and_join_multicast(const ACE_INET_Addr& mc_addr, const ACE_TString& if_name) {
  int result = 0;
  ACE_SOCK_Dgram_Mcast sock;
  sock.opts(ACE_SOCK_Dgram_Mcast::OPT_BINDADDR_NO | ACE_SOCK_Dgram_Mcast::DEFOPT_NULLIFACE);
  result = sock.join(mc_addr, 1, if_name.empty() ? 0 : if_name.c_str());
  result |= sock.close();
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

  get_valid_ipv4_interface_names(names);
  ACE_INET_Addr ipv4_mc_addr("239.255.0.7:1234", AF_INET);
  result |= create_socket_and_join_multicast(ipv4_mc_addr, ACE_TString());
  for (nameset::const_iterator it = names.begin(); result == 0 && it != names.end(); ++it) {
    result |= create_socket_and_join_multicast(ipv4_mc_addr, *it);
  }

#ifdef ACE_HAS_IPV6
  get_valid_ipv6_interface_names(names);
  ACE_INET_Addr ipv6_mc_addr("ff03::7:4321", AF_INET6);
  result |= create_socket_and_join_multicast(ipv6_mc_addr, ACE_TString());
  for (nameset::const_iterator it = names.begin(); result == 0 && it != names.end(); ++it) {
    result |= create_socket_and_join_multicast(ipv6_mc_addr, *it);
  }
#endif /* ACE_HAS_IPV6 */

  ACE_END_TEST;
  return result;
}
