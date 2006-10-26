// $Id$

#include "ace/Sock_Connect.h"
#include "ace/INET_Addr.h"
#include "ace/Log_Msg.h"
#include "ace/Handle_Set.h"
#include "ace/Auto_Ptr.h"
#include "ace/SString.h"
#include "ace/OS_Memory.h"
#include "ace/OS_NS_stdio.h"

#if defined (sparc)
#  include "ace/OS_NS_fcntl.h"
#endif  // sparc

#include "ace/OS_NS_stdlib.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_sys_socket.h"
#include "ace/OS_NS_netdb.h"
#include "ace/OS_NS_unistd.h"
#include "ace/os_include/net/os_if.h"

#if defined (ACE_HAS_IPV6)
#  include "ace/Guard_T.h"
#  include "ace/Recursive_Thread_Mutex.h"
#endif /* ACE_HAS_IPV6 */

# if defined (ACE_HAS_GETIFADDRS)
#   if defined (ACE_VXWORKS)
#     include /**/ <net/ifaddrs.h>
#   else
#     include /**/ <ifaddrs.h>
#   endif
# endif /* ACE_HAS_GETIFADDRS */

#if defined (ACE_VXWORKS) && (ACE_VXWORKS < 0x600)
#include /**/ <inetLib.h>
#include /**/ <netinet/in_var.h>
extern "C" {
  extern struct in_ifaddr* in_ifaddr;
}
#include "ace/OS_NS_stdio.h"
#endif /* ACE_VXWORKS < 0x600 */

#if defined (ACE_HAS_WINCE)
#include /**/ <Iphlpapi.h>
#endif  // ACE_HAS_WINCE

#if defined (ACE_WIN32) && defined (ACE_HAS_PHARLAP)
# include "ace/OS_NS_stdio.h"
#endif

#if defined (ACE_HAS_IPV6)
#  if defined (ACE_HAS_THREADS)
#    include "ace/Object_Manager.h"
#  endif /* ACE_HAS_THREADS */

namespace
{
  // private:
  //  Used internally so not exported.

  // Does this box have ipv4 turned on?
  int ace_ipv4_enabled = -1;

  // Does this box have ipv6 turned on?
  int ace_ipv6_enabled = -1;

}
#endif /* ACE_HAS_IPV6 */

// This is a hack to work around a problem with Visual Age C++ 5 and 6 on AIX.
// Without this, the compiler auto-instantiates the ACE_Auto_Array_Ptr for
// ifreq (contained in this module) but only adds the #include for <net/if.h>
// and not the one for <sys/socket.h> which is also needed.  Although we
// don't need the template defined here, it makes the compiler pull in
// <sys/socket.h> and the build runs clean.
#if defined (AIX) && defined (__IBMCPP__) && (__IBMCPP__ >= 500) && (__IBMCPP__ < 700)
static ACE_Auto_Array_Ptr<sockaddr> force_compiler_to_include_socket_h;
#endif /* AIX && __IBMCPP__ >= 500 */


ACE_RCSID (ace,
           Sock_Connect,
           "$Id$")


#if defined (ACE_WIN32) && \
    (!defined (ACE_HAS_WINSOCK2) \
     || (defined (ACE_HAS_WINSOCK2) && (ACE_HAS_WINSOCK2 == 0)))

static int
get_reg_subkeys (const ACE_TCHAR *key,
                 ACE_TCHAR *buffer,
                 DWORD &buf_len)
{
  HKEY hk;
  LONG rc = ACE_TEXT_RegOpenKeyEx (HKEY_LOCAL_MACHINE,
                                   key,
                                   0,
                                   KEY_READ,
                                   &hk);

  if (rc != ERROR_SUCCESS)
    return -1;

  ACE_TCHAR subkeyname[ACE_MAX_FULLY_QUALIFIED_NAME_LEN + 1];
  DWORD subkeyname_len = ACE_MAX_FULLY_QUALIFIED_NAME_LEN;
  FILETIME update_dummy;

  DWORD total = 0;

  for (int i = 0;
       (rc = ACE_TEXT_RegEnumKeyEx (hk, i,
                                    subkeyname,
                                    &subkeyname_len,
                                    0, 0, 0,
                                    &update_dummy)) != ERROR_NO_MORE_ITEMS;
        ++i)
    {
      if (subkeyname_len < buf_len - total)
        {
          ACE_OS::strcpy(buffer + total, subkeyname);
          total += subkeyname_len + 1;
          // Reset: was changed by RegEnumKeyEx call.
          subkeyname_len = ACE_MAX_FULLY_QUALIFIED_NAME_LEN + 1;
        }
      else
        return -1;
    }

  buf_len = total;

  ::RegCloseKey (hk);
  return 0;
}

// Return value in buffer for a key/name pair from the Windows
// Registry up to buf_len size.
// If all_subkeys == 1, look for name under all subkeys of key.

static int
get_reg_value (const ACE_TCHAR *key,
               const ACE_TCHAR *name,
               ACE_TCHAR *buffer,
               DWORD &buf_len,
               int all_subkeys = 0)
{
  HKEY hk;
  DWORD buf_type;
  LONG rc = ACE_TEXT_RegOpenKeyEx (HKEY_LOCAL_MACHINE,
                                   key,
                                   0,
                                   KEY_READ,
                                   &hk);

  if (rc != ERROR_SUCCESS)
    // print_error_string(ACE_LIB_TEXT ("RegOpenKeyEx"), rc);
    return -1;

  if (all_subkeys)
    {
      ACE_TCHAR ifname[ACE_MAX_FULLY_QUALIFIED_NAME_LEN + 1];
      DWORD ifname_len = ACE_MAX_FULLY_QUALIFIED_NAME_LEN + 1;
      FILETIME update_dummy;

      DWORD total = 0;
      DWORD size = buf_len;

      for (int i = 0;
           (rc = ACE_TEXT_RegEnumKeyEx (hk, i, ifname, &ifname_len,
                                        0, 0, 0,
                                        &update_dummy)) != ERROR_NO_MORE_ITEMS;
           ++i)
        {
          HKEY ifkey;
          if (rc != ERROR_SUCCESS
              || ACE_TEXT_RegOpenKeyEx (hk, ifname, 0,
                                        KEY_READ, &ifkey) != ERROR_SUCCESS)
            continue;

          if (ACE_TEXT_RegQueryValueEx (ifkey, name, 0, 0,
                                        (u_char*) (buffer + total),
                                        &size) != ERROR_SUCCESS)
            {
              RegCloseKey(ifkey);
              continue;
            }
          else
            {
              total += size;
              size = buf_len - total;
            }
          // Needs to be reset.
          ifname_len = ACE_MAX_FULLY_QUALIFIED_NAME_LEN + 1;
        }

      if (total == 0)
        {
          ::RegCloseKey (hk);
          return -2;
        }
      else
        {
          buf_len = total;
        }
    }
  else
    {

      rc = ACE_TEXT_RegQueryValueEx (hk,
                                     name,
                                     0,
                                     &buf_type,
                                     (u_char *) buffer,
                                     &buf_len);
      if (rc != ERROR_SUCCESS)
        {
          // print_error_string(ACE_LIB_TEXT ("RegEnumKeyEx"), rc);
          RegCloseKey (hk);
          return -2;
        }
    }

  ::RegCloseKey (hk);
  return 0;
}

enum ACE_WINDOWS_VERSION {
  ACE_WINDOWS_IS_UNKNOWN,
  ACE_WINDOWS_IS_WIN95,
  ACE_WINDOWS_IS_WIN98,
  ACE_WINDOWS_IS_WINME,
  ACE_WINDOWS_IS_WINNT,
  ACE_WINDOWS_IS_WIN2K,
  ACE_WINDOWS_IS_WINCE
};

static ACE_WINDOWS_VERSION
get_windows_version()
{
  OSVERSIONINFO vinfo;
  vinfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
  if (::GetVersionEx(&vinfo) == 0)
    {
      return ACE_WINDOWS_IS_UNKNOWN;
    }

  switch (vinfo.dwPlatformId)
    {
    case VER_PLATFORM_WIN32_NT:
      if (vinfo.dwMajorVersion <= 4)
        return ACE_WINDOWS_IS_WINNT;
      else
        return ACE_WINDOWS_IS_WIN2K;
    case VER_PLATFORM_WIN32_WINDOWS:
      if (vinfo.dwMajorVersion == 4)
        {
          if (vinfo.dwMinorVersion == 0)
            return ACE_WINDOWS_IS_WIN95;
          else if (vinfo.dwMinorVersion == 10)
            return ACE_WINDOWS_IS_WIN98;
          else if (vinfo.dwMinorVersion == 90)
            return ACE_WINDOWS_IS_WINME;
        }
#if defined (VER_PLATFORM_WIN32_CE)
    case VER_PLATFORM_WIN32_CE:
        if (vinfo.dwMajorVersion >= 3) {
            return ACE_WINDOWS_IS_WINCE;
        }
        else {
            return ACE_WINDOWS_IS_UNKNOWN;
        }
#endif /* VER_PLATFORM_WIN32_CE */
      // If no match we fall throu.
    default:
      return ACE_WINDOWS_IS_UNKNOWN;
    }
}

#endif //(ACE_WIN32) && !(ACE_HAS_WINSOCK2) || (ACE_HAS_WINSOCK2) && (ACE_HAS_WINSOCK2 == 0)

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

// Bind socket to an unused port.

int
ACE::bind_port (ACE_HANDLE handle, ACE_UINT32 ip_addr, int address_family)
{
  ACE_TRACE ("ACE::bind_port");

  ACE_INET_Addr addr;

#if defined (ACE_HAS_IPV6)
  if (address_family != PF_INET6)
    // What do we do if it is PF_"INET6?  Since it's 4 bytes, it must be an
    // IPV4 address. Is there a difference?  Why is this test done? dhinton
#else /* ACE_HAS_IPV6 */
    ACE_UNUSED_ARG (address_family);
#endif /* !ACE_HAS_IPV6 */
    addr = ACE_INET_Addr ((u_short)0, ip_addr);
#if defined (ACE_HAS_IPV6)
 else if (ip_addr != INADDR_ANY)
 // address_family == PF_INET6 and a non default IP address means to bind
 // to the IPv4-mapped IPv6 address
   addr.set ((u_short)0, ip_addr, 1, 1);
#endif /* ACE_HAS_IPV6 */

#if !defined (ACE_LACKS_WILDCARD_BIND)
  // The OS kernel should select a free port for us.
  return ACE_OS::bind (handle,
                       (sockaddr*)addr.get_addr(),
                       addr.get_size());
#else
  static u_short upper_limit = ACE_MAX_DEFAULT_PORT;
  int round_trip = upper_limit;
  int lower_limit = IPPORT_RESERVED;

  // We have to select the port explicitly.

  for (;;)
    {
      addr.set((u_short)upper_limit,ip_addr);

      if (ACE_OS::bind (handle,
                        (sockaddr*)addr.get_addr()
                        addr.get_size()) >= 0)
        {
#if defined (ACE_WIN32)
          upper_limit--;
#endif /* ACE_WIN32 */
          return 0;
        }
      else if (errno != EADDRINUSE)
        return -1;
      else
        {
          upper_limit--;

          // Wrap back around when we reach the bottom.
          if (upper_limit <= lower_limit)
            upper_limit = ACE_MAX_DEFAULT_PORT;

          // See if we have already gone around once!
          if (upper_limit == round_trip)
            {
              errno = EAGAIN;
              return -1;
            }
        }
    }
#endif /* ACE_HAS_WILDCARD_BIND */
}

int
ACE::get_bcast_addr (ACE_UINT32 &bcast_addr,
                     const ACE_TCHAR *host_name,
                     ACE_UINT32 host_addr,
                     ACE_HANDLE handle)
{
  ACE_TRACE ("ACE::get_bcast_addr");

#if defined (ACE_LACKS_GET_BCAST_ADDR)
  ACE_UNUSED_ARG (bcast_addr);
  ACE_UNUSED_ARG (host_name);
  ACE_UNUSED_ARG (host_addr);
  ACE_UNUSED_ARG (handle);
  ACE_NOTSUP_RETURN (-1);
#elif !defined(ACE_WIN32) && !defined(__INTERIX)
  ACE_HANDLE s = handle;

  if (s == ACE_INVALID_HANDLE)
    s = ACE_OS::socket (AF_INET, SOCK_STREAM, 0);

  if (s == ACE_INVALID_HANDLE)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_LIB_TEXT ("%p\n"),
                       ACE_LIB_TEXT ("ACE_OS::socket")),
                      -1);

  struct ifconf ifc;
  char buf[BUFSIZ];

  ifc.ifc_len = sizeof buf;
  ifc.ifc_buf = buf;

  // Get interface structure and initialize the addresses using UNIX
  // techniques
  int cmd = SIOCGIFCONF;
  if (ACE_OS::ioctl (s, cmd, (char *) &ifc) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_LIB_TEXT ("%p\n"),
                       ACE_LIB_TEXT ("ACE::get_bcast_addr:")
                       ACE_LIB_TEXT ("ioctl (get interface configuration)")),
                      -1);

  struct ifreq *ifr = ifc.ifc_req;

  struct sockaddr_in ip_addr;

  // Get host ip address if necessary.
  if (host_name)
    {
      hostent *hp = ACE_OS::gethostbyname (ACE_TEXT_ALWAYS_CHAR (host_name));

      if (hp == 0)
        return -1;
      else
#if !defined(_UNICOS)
        ACE_OS::memcpy ((char *) &ip_addr.sin_addr.s_addr,
                        (char *) hp->h_addr,
                        hp->h_length);
#else /* _UNICOS */
      {
        ACE_UINT64 haddr;  // a place to put the address
        char * haddrp = (char *) &haddr;  // convert to char pointer
        ACE_OS::memcpy(haddrp,(char *) hp->h_addr,hp->h_length);
        ip_addr.sin_addr.s_addr = haddr;
      }
#endif /* ! _UNICOS */
    }
  else
    {
      ACE_OS::memset ((void *) &ip_addr, 0, sizeof ip_addr);
#if !defined(_UNICOS)
      ACE_OS::memcpy ((void *) &ip_addr.sin_addr,
                      (void*) &host_addr,
                      sizeof ip_addr.sin_addr);
#else /* _UNICOS */
      ip_addr.sin_addr.s_addr = host_addr;   // just copy to the bitfield
#endif /* ! _UNICOS */
    }

#if !defined(AIX) && !defined (__QNX__) && !defined (__FreeBSD__) && !defined(__NetBSD__)
  for (int n = ifc.ifc_len / sizeof (struct ifreq) ; n > 0;
       n--, ifr++)
#else
  // see mk_broadcast@SOCK_Dgram_Bcast.cpp
  for (int nbytes = ifc.ifc_len; nbytes >= (int) sizeof (struct ifreq) &&
        ((ifr->ifr_addr.sa_len > sizeof (struct sockaddr)) ?
          (nbytes >= (int) sizeof (ifr->ifr_name) + ifr->ifr_addr.sa_len) : 1);
        ((ifr->ifr_addr.sa_len > sizeof (struct sockaddr)) ?
          (nbytes -= sizeof (ifr->ifr_name) + ifr->ifr_addr.sa_len,
            ifr = (struct ifreq *)
              ((caddr_t) &ifr->ifr_addr + ifr->ifr_addr.sa_len)) :
          (nbytes -= sizeof (struct ifreq), ifr++)))
#endif /* !defined(AIX) && !defined (__QNX__) && !defined (__FreeBSD__) && !defined(__NetBSD__) */
    {
      struct sockaddr_in if_addr;

      // Compare host ip address with interface ip address.
      ACE_OS::memcpy (&if_addr,
                      &ifr->ifr_addr,
                      sizeof if_addr);

      if (ip_addr.sin_addr.s_addr != if_addr.sin_addr.s_addr)
        continue;

      if (ifr->ifr_addr.sa_family != AF_INET)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_LIB_TEXT ("%p\n"),
                      ACE_LIB_TEXT ("ACE::get_bcast_addr:")
                      ACE_LIB_TEXT ("Not AF_INET")));
          continue;
        }

      struct ifreq flags = *ifr;
      struct ifreq if_req = *ifr;

      if (ACE_OS::ioctl (s, SIOCGIFFLAGS, (char *) &flags) == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_LIB_TEXT ("%p\n"),
                      ACE_LIB_TEXT ("ACE::get_bcast_addr:")
                      ACE_LIB_TEXT (" ioctl (get interface flags)")));
          continue;
        }

      if (ACE_BIT_DISABLED (flags.ifr_flags, IFF_UP))
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_LIB_TEXT ("%p\n"),
                      ACE_LIB_TEXT ("ACE::get_bcast_addr:")
                      ACE_LIB_TEXT ("Network interface is not up")));
          continue;
        }

      if (ACE_BIT_ENABLED (flags.ifr_flags, IFF_LOOPBACK))
        continue;

      if (ACE_BIT_ENABLED (flags.ifr_flags, IFF_BROADCAST))
        {
          if (ACE_OS::ioctl (s,
                             SIOCGIFBRDADDR,
                             (char *) &if_req) == -1)
            ACE_ERROR ((LM_ERROR,
                        ACE_LIB_TEXT ("%p\n"),
                        ACE_LIB_TEXT ("ACE::get_bcast_addr:")
                        ACE_LIB_TEXT ("ioctl (get broadaddr)")));
          else
            {
              ACE_OS::memcpy (reinterpret_cast<sockaddr_in *> (&ip_addr),
                              reinterpret_cast<sockaddr_in *> (&if_req.ifr_broadaddr),
                              sizeof if_req.ifr_broadaddr);

              ACE_OS::memcpy ((void *) &host_addr,
                              (void *) &ip_addr.sin_addr,
                              sizeof host_addr);

              if (handle == ACE_INVALID_HANDLE)
                ACE_OS::close (s);

              bcast_addr = host_addr;
              return 0;
            }
        }
      else
        ACE_ERROR ((LM_ERROR,
                    ACE_LIB_TEXT ("%p\n"),
                    ACE_LIB_TEXT ("ACE::get_bcast_addr:")
                    ACE_LIB_TEXT ("Broadcast is not enable for this interface.")));

      if (handle == ACE_INVALID_HANDLE)
        ACE_OS::close (s);

      bcast_addr = host_addr;
      return 0;
    }

  return 0;
#else
  ACE_UNUSED_ARG (handle);
  ACE_UNUSED_ARG (host_addr);
  ACE_UNUSED_ARG (host_name);
  bcast_addr = (ACE_UINT32 (INADDR_BROADCAST));
  return 0;
#endif /* !ACE_WIN32 && !__INTERIX */
}

int
ACE::get_fqdn (ACE_INET_Addr const & addr,
               char hostname[],
               size_t len)
{
  int h_error;  // Not the same as errno!
  hostent hentry;
  ACE_HOSTENT_DATA buf;

  char * ip_addr = 0;
  int ip_addr_size = 0;
  if (addr.get_type () == AF_INET)
    {
      sockaddr_in * const sock_addr =
        reinterpret_cast<sockaddr_in *> (addr.get_addr ());
      ip_addr_size = sizeof sock_addr->sin_addr;
      ip_addr = (char*) &sock_addr->sin_addr;
    }
#ifdef ACE_HAS_IPV6
  else
    {
      sockaddr_in6 * sock_addr =
        reinterpret_cast<sockaddr_in6 *> (addr.get_addr ());

      ip_addr_size = sizeof sock_addr->sin6_addr;
      ip_addr = (char*) &sock_addr->sin6_addr;
    }
#endif  /* ACE_HAS_IPV6 */

   // get the host entry for the address in question
   hostent * const hp = ACE_OS::gethostbyaddr_r (ip_addr,
                                                 ip_addr_size,
                                                 addr.get_type (),
                                                 &hentry,
                                                 buf,
                                                 &h_error);

   // if it's not found in the host file or the DNS datase, there is nothing
   // much we can do. embed the IP address
   if (hp == 0 || hp->h_name == 0)
     return -1;

   if (ACE::debug())
     ACE_DEBUG ((LM_DEBUG,
                 ACE_TEXT ("(%P|%t) - ACE::get_fqdn, ")
                 ACE_TEXT ("canonical host name is %s\n"),
                 ACE_TEXT_CHAR_TO_TCHAR (hp->h_name)));

   // check if the canonical name is the FQDN
   if (!ACE_OS::strchr(hp->h_name, '.'))
     {
       // list of address
       char** p;
       // list of aliases
       char** q;

       // for every address and for every alias within the address, check and
       // see if we can locate a FQDN
       for (p = hp->h_addr_list; *p != 0; ++p)
         {
           for (q = hp->h_aliases; *q != 0; ++q)
             {
               if (ACE_OS::strchr(*q, '.'))
                 {
                   // we got an FQDN from an alias. use this
                   if (ACE_OS::strlen (*q) >= len)
                     // the hostname is too huge to fit into a
                     // buffer of size MAXHOSTNAMELEN
                     // should we check other aliases as well
                     // before bailing out prematurely?
                     // for right now, let's do it. this (short name)
                     // is atleast better than embedding the IP
                     // address in the profile
                     continue;

                   if (ACE::debug ())
                     ACE_DEBUG ((LM_DEBUG,
                                 ACE_TEXT ("(%P|%t) - ACE::get_fqdn, ")
                                 ACE_TEXT ("found fqdn within alias as %s\n"),
                                 ACE_TEXT_CHAR_TO_TCHAR(*q)));
                   ACE_OS::strcpy (hostname, *q);

                   return 0;
                 }
             }
         }
     }

   // The canonical name may be an FQDN when we reach here.
   // Alternatively, the canonical name (a non FQDN) may be the best
   // we can do.
   if (ACE_OS::strlen (hp->h_name) >= len)
     {
       // The hostname is too large to fit into a buffer of size
       // MAXHOSTNAMELEN.
       return -2;
     }
   else
     {
       ACE_OS::strcpy (hostname, hp->h_name);
     }

   return 0;
}

// return an array of all configured IP interfaces on this host, count
// rc = 0 on success (count == number of interfaces else -1 caller is
// responsible for calling delete [] on parray

int
ACE::get_ip_interfaces (size_t &count,
                        ACE_INET_Addr *&addrs)
{
  ACE_TRACE ("ACE::get_ip_interfaces");

  count = 0;
  addrs = 0;

#if defined (ACE_WIN32)
  // Win32 can do this by a simple API call if MSVC 5 or later is the compiler.
  // Not sure if Borland supplies the needed header/lib, but it might.
# if defined (ACE_HAS_WINSOCK2) && (ACE_HAS_WINSOCK2 != 0)
  int i, n_interfaces, status;

  INTERFACE_INFO info[64];
  SOCKET sock;

  // Get an (overlapped) DGRAM socket to test with
  sock = socket (AF_INET, SOCK_DGRAM, 0);
  if (sock == INVALID_SOCKET)
    return -1;

  DWORD bytes;
  status = WSAIoctl(sock,
                    SIO_GET_INTERFACE_LIST,
                    0,
                    0,
                    info,
                    sizeof(info),
                    &bytes,
                    0,
                    0);
  closesocket (sock);
  if (status == SOCKET_ERROR)
    return -1;

  n_interfaces = bytes / sizeof(INTERFACE_INFO);

  // SIO_GET_INTERFACE_LIST does not work for IPv6
  // Instead recent versions of Winsock2 add the new opcode
  // SIO_ADDRESS_LIST_QUERY.
  // If this is not available forget about IPv6 local interfaces:-/
  int n_v6_interfaces = 0;

# if defined (ACE_HAS_IPV6) && defined (SIO_ADDRESS_LIST_QUERY)

  LPSOCKET_ADDRESS_LIST v6info;
  char *buffer;
  DWORD buflen = sizeof (SOCKET_ADDRESS_LIST) + (63 * sizeof (SOCKET_ADDRESS));
  ACE_NEW_RETURN (buffer,
                  char[buflen],
                  -1);
  v6info = reinterpret_cast<LPSOCKET_ADDRESS_LIST> (buffer);

  // Get an (overlapped) DGRAM socket to test with.
  // If it fails only return IPv4 interfaces.
  sock = socket (AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
  if (sock != INVALID_SOCKET)
    {
      status = WSAIoctl(sock,
                        SIO_ADDRESS_LIST_QUERY,
                        0,
                        0,
                        v6info,
                        buflen,
                        &bytes,
                        0,
                        0);
      closesocket (sock);
      if (status != SOCKET_ERROR)
        n_v6_interfaces = v6info->iAddressCount;
    }
# endif /* ACE_HAS_IPV6 */

  ACE_NEW_RETURN (addrs,
                  ACE_INET_Addr[n_interfaces + n_v6_interfaces],
                  -1);

  // Now go through the list and transfer the good ones to the list of
  // because they're down or don't have an IP address.
  for (count = 0, i = 0; i < n_interfaces; ++i)
    {
      LPINTERFACE_INFO lpii;
      struct sockaddr_in *addrp = 0;

      lpii = &info[i];
      if (!(lpii->iiFlags & IFF_UP))
        continue;

      // We assume IPv4 addresses here
      addrp = reinterpret_cast<struct sockaddr_in *> (&lpii->iiAddress.AddressIn);
      if (addrp->sin_addr.s_addr == INADDR_ANY)
        continue;

      // Set the address for the caller.
      addrs[count].set(addrp, sizeof(sockaddr_in));
      ++count;
    }

# if defined (ACE_HAS_IPV6) && defined (SIO_ADDRESS_LIST_QUERY)
  // Now go through the list and transfer the good ones to the list of
  // because they're down or don't have an IP address.
  for (i = 0; i < n_v6_interfaces; i++)
    {
      struct sockaddr_in6 *addr6p;

      if (v6info->Address[i].lpSockaddr->sa_family != AF_INET6)
        continue;

      addr6p = reinterpret_cast<struct sockaddr_in6 *> (v6info->Address[i].lpSockaddr);
      if (IN6_IS_ADDR_UNSPECIFIED(&addr6p->sin6_addr))  // IN6ADDR_ANY?
        continue;

      // Set the address for the caller.
      addrs[count].set(reinterpret_cast<struct sockaddr_in *> (addr6p), sizeof(sockaddr_in6));
      ++count;
    }

  delete [] buffer; // Clean up
# endif /* ACE_HAS_IPV6 */

  if (count == 0)
    {
      delete [] addrs;
      addrs = 0;
    }

  return 0;

#else /* Winsock 2 && MSVC 5 or later */

  // PharLap ETS has kernel routines to rummage through the device
  // configs and extract the interface info. Sort of a pain in the
  // butt, but better than trying to figure out where it moved to in
  // the registry... :-|
#  if defined (ACE_HAS_PHARLAP)
#    if !defined (ACE_HAS_PHARLAP_RT)
  ACE_NOTSUP_RETURN (-1);
#    endif /* ACE_HAS_PHARLAP_RT */

  // Locate all of the IP devices in the system, saving a DEVHANDLE
  // for each. Then allocate the ACE_INET_Addrs needed and fetch all
  // the IP addresses.  To locate the devices, try the available
  // device name roots and increment the device number until the
  // kernel says there are no more of that type.
  const size_t ACE_MAX_ETS_DEVICES = 64;  // Arbitrary, but should be enough.
  DEVHANDLE ip_dev[ACE_MAX_ETS_DEVICES];
  EK_TCPIPCFG *devp;
  size_t i, j;
  ACE_TCHAR dev_name[16];

  count = 0;
  for (i = 0; count < ACE_MAX_ETS_DEVICES; i++, ++count)
    {
      // Ethernet.
      ACE_OS::sprintf (dev_name,
                       "ether%d",
                       i);
      ip_dev[count] = EtsTCPGetDeviceHandle (dev_name);
      if (ip_dev[count] == 0)
        break;
    }
  for (i = 0; count < ACE_MAX_ETS_DEVICES; i++, ++count)
    {
      // SLIP.
      ACE_OS::sprintf (dev_name,
                       "sl%d",
                       i);
      ip_dev[count] = EtsTCPGetDeviceHandle (dev_name);
      if (ip_dev[count] == 0)
        break;
    }
  for (i = 0; count < ACE_MAX_ETS_DEVICES; i++, ++count)
    {
      // PPP.
      ACE_OS::sprintf (dev_name,
                       "ppp%d",
                       i);
      ip_dev[count] = EtsTCPGetDeviceHandle (dev_name);
      if (ip_dev[count] == 0)
        break;
    }

  if (count > 0)
    ACE_NEW_RETURN (addrs,
                    ACE_INET_Addr[count],
                    -1);
  else
    addrs = 0;

  for (i = 0, j = 0; i < count; i++)
    {
      devp = EtsTCPGetDeviceCfg (ip_dev[i]);
      if (devp != 0)
        {
          addrs[j].set (0,
                        devp->nwIPAddress,
                        0); // Already in net order.
          j++;
        }
      // There's no call to close the DEVHANDLE.
    }

  count = j;
  if (count == 0 && addrs != 0)
    {
      delete [] addrs;
      addrs = 0;
    }

  return 0;

#  else /* ACE_HAS_PHARLAP */

#   if defined (ACE_HAS_WINCE)

  // CE does not support Winsock2 (yet) and has many variations on registry setting.
  // Thus, it is better to use GetAdapterInfo defined in iphlpapi.h, which is a
  // standard library for CE.  iphlpapi.lib should come with the CE SDK and should
  // be included in the machine.
  // Note: This call is supported only in WinCE 3.0 or later.  Also, even though
  //       "iphlpapi.dll" may not be found in the /Windows directory on some machines,
  //       it will (must) support iphlpapi API's because it is part of the standard
  //       library for WinCE.

    IP_ADAPTER_INFO* adapterInfo = 0;
    ULONG sz = 0;
    DWORD result = ::GetAdaptersInfo(adapterInfo, &sz);

    while (result != ERROR_SUCCESS)
    {
        switch (result)
        {
        case ERROR_BUFFER_OVERFLOW:  // MUST come here at the first run because sz = 0
            adapterInfo = (PIP_ADAPTER_INFO)(new char[sz]);  // I know, I know, this is ugly.

            result = ::GetAdaptersInfo(adapterInfo, &sz);
            if (result == ERROR_SUCCESS) {
                const char* invalid_IP = "0.0.0.0";

                // find out how many interfaces are there
                {
                    IP_ADAPTER_INFO* tempAdapterInfo = adapterInfo;
                    int n_interfaces = 0;
                    while (tempAdapterInfo != 0) {
                        IP_ADDR_STRING* addr = &tempAdapterInfo->IpAddressList;
                        while (addr != 0) {
                            if (ACE_OS::strcmp(addr->IpAddress.String, invalid_IP) != 0) {
                                // skip invalid IP address
                                ++n_interfaces;
                            }
                            addr = addr->Next;
                        }
                        tempAdapterInfo = tempAdapterInfo->Next;
                    }
                    if (n_interfaces == 0) {
                        ACE_ERROR_RETURN ((LM_ERROR,
                            ACE_LIB_TEXT ("%p\nACE::get_ip_interfaces - "),
                            ACE_LIB_TEXT ("No adapter found.")),
                            -1);
                    }

                    ACE_NEW_RETURN (addrs, ACE_INET_Addr[n_interfaces], -2);
                }

                // find out valid IP addresses and put them into the addr
                while (adapterInfo != 0) {
                    IP_ADDR_STRING* addr = &adapterInfo->IpAddressList;
                    while (addr != 0) {
                        if (ACE_OS::strcmp(addr->IpAddress.String, invalid_IP) != 0) {
                            addrs[count++] = ACE_INET_Addr((u_short) 0, addr->IpAddress.String);
                        }
                        addr = addr->Next;
                    }
                    adapterInfo = adapterInfo->Next;
                }
            }
            // if second GetAdaptersInfo call fails, let other cases take care of it
            break;

        case ERROR_NOT_SUPPORTED: // OS does not support this method
            ACE_ERROR_RETURN ((LM_ERROR,
                ACE_LIB_TEXT ("%p\nACE::get_ip_interfaces - "),
                ACE_LIB_TEXT ("This version of WinCE does not support GetAdapterInfo.")),
                -1);
            break;

        case ERROR_NO_DATA:  // no adapter installed
            ACE_ERROR_RETURN ((LM_ERROR,
                ACE_LIB_TEXT ("%p\nACE::get_ip_interfaces - "),
                ACE_LIB_TEXT ("No network adapter installed.")),
                -1);
            break;

        case ERROR_INVALID_PARAMETER:
            ACE_ERROR_RETURN ((LM_ERROR,
                ACE_LIB_TEXT ("%p\nACE::get_ip_interfaces - "),
                ACE_LIB_TEXT ("Invalid parameter.")),
                -1);
            break;

        default:
            ACE_ERROR_RETURN ((LM_ERROR,
                ACE_LIB_TEXT ("%p\nACE::get_ip_interfaces - "),
                ACE_LIB_TEXT ("Adapter info access permission denied.")),
                -1);
            break;
        }
    }

    delete [] adapterInfo;
    return 0;

#   endif  // ACE_HAS_WINCE

  //
  // No Winsock2.
  // Get interface information from the registry.
  // As this information is in different locations of the registry
  // on different windows versions, we need to ask at runtime.
  //

  // Normally we have to look under one key for interfaces name,
  // and under a second key for ip address of those interfaces.
  // Exact values and the way to search depend on windows version.

  // This is the first key we have to look for.
  const ACE_TCHAR *BASE_KEY1;

  // This is the name we have to look for under the first key.
  // If this is == 0, we need to look for subkeys, not the values from
  // a name.
  const ACE_TCHAR *KEY1_NAME_ID;

  // The second key is normally constructed concatenating a prefix,
  // the value found on KEY1_NAME_ID stripped from the first s_offset
  // characters, and a suffix.
  unsigned int s_offset;
  const ACE_TCHAR *PREFFIX_KEY2;
  const ACE_TCHAR *SUFFIX_KEY2;

  // If != 0, look for the value of KEY1_NAME_ID not directly under
  // BASE_KEY1, but on every subkey of BASE_KEY1.
  int use_subkeys;

  // When we search for IP Addresses below, we look for a key with a
  // name in this array (null terminated).
  // For some windows versions, there is an
  // aditional key for ppp interfaces that will be stored on [1].
  const ACE_TCHAR *IPADDR_NAME_ID[3] = {
    ACE_LIB_TEXT ("IPAddress"), 0, 0
  };

  // Skip addresses that match this.
  const ACE_TCHAR *INVALID_TCPIP_DEVICE_ADDR = ACE_LIB_TEXT ("0.0.0.0");

  ACE_WINDOWS_VERSION winver = get_windows_version();

  switch (winver)
    {
      case ACE_WINDOWS_IS_WINNT:
        PREFFIX_KEY2 = ACE_LIB_TEXT ("SYSTEM\\CurrentControlSet\\Services\\");
        BASE_KEY1 =
          ACE_LIB_TEXT ("SYSTEM\\CurrentControlSet\\Services")
          ACE_LIB_TEXT ("\\Tcpip\\Linkage");
        SUFFIX_KEY2 = ACE_LIB_TEXT ("\\Parameters\\Tcpip");
        KEY1_NAME_ID = ACE_LIB_TEXT ("Bind");
        s_offset = 8;
        use_subkeys = 0;
        break;

      case ACE_WINDOWS_IS_WIN2K:
        BASE_KEY1 =
          ACE_LIB_TEXT ("SYSTEM\\CurrentControlSet\\Services")
          ACE_LIB_TEXT ("\\Tcpip\\Parameters\\Interfaces\\");
        PREFFIX_KEY2 = BASE_KEY1;
        SUFFIX_KEY2 = ACE_LIB_TEXT ("");
        KEY1_NAME_ID = 0;
        s_offset = 0;
        use_subkeys = 1;
        // PPP.
        IPADDR_NAME_ID[1] = ACE_LIB_TEXT ("DhcpIPAddress");
        break;

      // If ACE_HAS_WINNT4 we can safely assume the ones below will
      // not be needed.
#   if !defined(ACE_HAS_WINNT4) || (ACE_HAS_WINNT4 == 0)
      case ACE_WINDOWS_IS_WIN95:
      case ACE_WINDOWS_IS_WIN98:
      case ACE_WINDOWS_IS_WINME:
        PREFFIX_KEY2 =
          ACE_LIB_TEXT ("SYSTEM\\CurrentControlSet\\Services\\Class\\");
        BASE_KEY1 = ACE_LIB_TEXT ("Enum\\Network\\MSTCP");
        SUFFIX_KEY2 = ACE_LIB_TEXT ("");
        KEY1_NAME_ID =  ACE_LIB_TEXT ("Driver");
        use_subkeys = 1;
        s_offset = 0;
        break;
#   endif /* !ACE_HAS_WINNT4 */

      default:
        return -1;
    }

  ACE_TCHAR raw_buffer[ACE_MAX_FULLY_QUALIFIED_NAME_LEN + 1];
  DWORD raw_buflen = ACE_MAX_FULLY_QUALIFIED_NAME_LEN + 1;

  if (KEY1_NAME_ID == 0)
    {
      if (::get_reg_subkeys (BASE_KEY1,
                             raw_buffer,
                             raw_buflen))
        return -1;
    }
  else
    {
      if (::get_reg_value (BASE_KEY1,
                           KEY1_NAME_ID,
                           raw_buffer,
                           raw_buflen,
                           use_subkeys))
        return -1;
    }
  // return buffer contains 0 delimited strings

  ACE_Tokenizer dev_names (raw_buffer);
  dev_names.delimiter (ACE_LIB_TEXT ('\0'));
  int n_interfaces = 0;

  // Count the number of interfaces
  while (dev_names.next () != 0)
    ++n_interfaces;

  // case 1. no interfaces present, empty string? OS version change?
  if (n_interfaces == 0)
    return 0;

  ACE_NEW_RETURN (addrs,
                  ACE_INET_Addr[n_interfaces],
                  -2);

  ACE_TCHAR buffer[ACE_MAX_FULLY_QUALIFIED_NAME_LEN + 1];
  DWORD buf_len = ACE_MAX_FULLY_QUALIFIED_NAME_LEN + 1;

  count = 0;
  for (int i = 0; i < n_interfaces; i++)
    {
      for (const ACE_TCHAR **ipaddr_name_id = IPADDR_NAME_ID;
           *ipaddr_name_id != 0;
           ++ipaddr_name_id)
        {
          // a. construct name to access IP Address for this interface
          ACE_TString ifdevkey (PREFFIX_KEY2);
          ACE_TString the_dev = dev_names.next ();

          if (the_dev.length() < s_offset)
            {
              return -3;              // Something's wrong
            }

          // rest of string from offset.
          the_dev = the_dev.substring (s_offset);

          ifdevkey += the_dev;
          ifdevkey += SUFFIX_KEY2;

          // b. extract value
          // Gets overwritten on each call
          buf_len = sizeof (buffer);
          if (get_reg_value (ifdevkey.fast_rep (),
                             *ipaddr_name_id,
                             buffer,
                             buf_len))
            continue; // Skip unknown devices.

          if (ACE_OS::strcmp (buffer,
                              INVALID_TCPIP_DEVICE_ADDR) == 0)
            continue; // Don't count this device

          // c. store in hostinfo object array and up the counter
          addrs[count++] =
            ACE_INET_Addr ((u_short) 0, buffer);
        }
    }

  return 0;
#  endif /* ACE_HAS_PHARLAP */
# endif /* Winsock 2 && MSVC 5 or later */

#elif defined (ACE_HAS_GETIFADDRS)
  // Take advantage of the BSD getifaddrs function that simplifies
  // access to connected interfaces.
  struct ifaddrs *ifap;
  struct ifaddrs *p_if;

  if (::getifaddrs (&ifap) != 0)
    return -1;

  // Count number of interfaces.
  size_t num_ifs = 0;
  for (p_if = ifap; p_if != 0; p_if = p_if->ifa_next)
    ++num_ifs;

  // Now create and initialize output array.
  ACE_NEW_RETURN (addrs,
                  ACE_INET_Addr[num_ifs],
                  -1); // caller must free

  // Pull the address out of each INET interface.  Not every interface
  // is for IP, so be careful to count properly.  When setting the
  // INET_Addr, note that the 3rd arg (0) says to leave the byte order
  // (already in net byte order from the interface structure) as is.
  count = 0;

  for (p_if = ifap;
       p_if != 0;
       p_if = p_if->ifa_next)
    {
      if (p_if->ifa_addr &&
          p_if->ifa_addr->sa_family == AF_INET)
        {
          struct sockaddr_in *addr =
            reinterpret_cast<sockaddr_in *> (p_if->ifa_addr);

          // Sometimes the kernel returns 0.0.0.0 as the interface
          // address, skip those...
          if (addr->sin_addr.s_addr != INADDR_ANY)
            {
              addrs[count].set ((u_short) 0,
                                addr->sin_addr.s_addr,
                                0);
              ++count;
            }
        }
# if defined (ACE_HAS_IPV6)
      else if (p_if->ifa_addr &&
               p_if->ifa_addr->sa_family == AF_INET6)
        {
          struct sockaddr_in6 *addr =
            reinterpret_cast<sockaddr_in6 *> (p_if->ifa_addr);

          // Skip the ANY address
          if (!IN6_IS_ADDR_UNSPECIFIED(&addr->sin6_addr))
            {
              addrs[count].set(reinterpret_cast<struct sockaddr_in *> (addr),
                               sizeof(sockaddr_in6));
              ++count;
            }
        }
# endif /* ACE_HAS_IPV6 */
    }

  ::freeifaddrs (ifap);

  return 0;

#elif defined (__unix) || defined (__unix__) || defined (__Lynx__) || defined (_AIX) || defined (ACE_OPENVMS)
  // COMMON (SVR4 and BSD) UNIX CODE

  size_t num_ifs, num_ifs_found;

  // Call specific routine as necessary.
  ACE_HANDLE handle = get_handle();

  if (handle == ACE_INVALID_HANDLE)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_LIB_TEXT ("%p\n"),
                       ACE_LIB_TEXT ("ACE::get_ip_interfaces:open")),
                      -1);
  if (ACE::count_interfaces (handle, num_ifs))
    {
      ACE_OS::close (handle);
      return -1;
    }

  // ioctl likes to have an extra ifreq structure to mark the end of
  // what it returned, so increase the num_ifs by one.
  ++num_ifs;

  struct ifreq *ifs = 0;
  ACE_NEW_RETURN (ifs,
                  struct ifreq[num_ifs],
                  -1);
  ACE_OS::memset (ifs, 0, num_ifs * sizeof (struct ifreq));

  ACE_Auto_Array_Ptr<struct ifreq> p_ifs (ifs);

  if (p_ifs.get() == 0)
    {
      ACE_OS::close (handle);
      errno = ENOMEM;
      return -1;
    }

  struct ifconf ifcfg;
  ACE_OS::memset (&ifcfg, 0, sizeof (struct ifconf));
  ifcfg.ifc_req = p_ifs.get ();
  ifcfg.ifc_len = num_ifs * sizeof (struct ifreq);

  int cmd = SIOCGIFCONF;
  if (ACE_OS::ioctl (handle,
                     cmd,
                     (caddr_t) &ifcfg) == -1)
    {
      ACE_OS::close (handle);
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_LIB_TEXT ("%p\n"),
                         ACE_LIB_TEXT ("ACE::get_ip_interfaces:")
                         ACE_LIB_TEXT ("ioctl - SIOCGIFCONF failed")),
                        -1);
    }

  ACE_OS::close (handle);

  // Now create and initialize output array.

  ACE_NEW_RETURN (addrs,
                  ACE_INET_Addr[num_ifs],
                  -1); // caller must free

  struct ifreq *pcur = p_ifs.get ();
  num_ifs_found = ifcfg.ifc_len / sizeof (struct ifreq); // get the number of returned ifs

  // Pull the address out of each INET interface.  Not every interface
  // is for IP, so be careful to count properly.  When setting the
  // INET_Addr, note that the 3rd arg (0) says to leave the byte order
  // (already in net byte order from the interface structure) as is.
  count = 0;

  for (size_t i = 0;
       i < num_ifs_found;
       i++)
    {
      if (pcur->ifr_addr.sa_family == AF_INET
#if defined (ACE_HAS_IPV6)
          || pcur->ifr_addr.sa_family == AF_INET6
#endif
          )
        {
#if !defined(_UNICOS)
          struct sockaddr_in *addr =
            reinterpret_cast<sockaddr_in *> (&pcur->ifr_addr);

          // Sometimes the kernel returns 0.0.0.0 as an IPv4 interface
          // address; skip those...
          if (addr->sin_addr.s_addr != 0
#  if defined (ACE_HAS_IPV6)
              || addr->sin_family == AF_INET6
#  endif
              )
            {
	      int addrlen = static_cast<int> (sizeof (struct sockaddr_in));
#  if defined (ACE_HAS_IPV6)
              if (addr->sin_family == AF_INET6)
                addrlen = static_cast<int> (sizeof (struct sockaddr_in6));
#  endif
              addrs[count].set (addr, addrlen);
              count++;
            }
#else /* ! _UNICOS */
          // need to explicitly copy on the Cray, since the bitfields kinda
          // screw things up here
          struct sockaddr_in inAddr;

          inAddr.sin_len = pcur->ifr_addr.sa_len;
          inAddr.sin_family = pcur->ifr_addr.sa_family;
          memcpy((void *)&(inAddr.sin_addr),
                 (const void *)&(pcur->ifr_addr.sa_data[8]),
                 sizeof(struct in_addr));

          if (inAddr.sin_addr.s_addr != 0)
            {
              addrs[count].set(&inAddr, sizeof(struct sockaddr_in));
              ++count;
            }
#endif /* ! _UNICOS */
        }

#if !defined(AIX) && !defined (__QNX__) && !defined (__FreeBSD__) && !defined(__NetBSD__)
      ++pcur;
#else
      if (pcur->ifr_addr.sa_len <= sizeof (struct sockaddr))
        {
           ++pcur;
        }
      else
        {
           pcur = (struct ifreq *)
               (pcur->ifr_addr.sa_len + (caddr_t) &pcur->ifr_addr);
        }
#endif /* !defined(AIX) && !defined (__QNX__) && !defined (__FreeBSD__) && !defined(__NetBSD__) */
    }

# if defined (ACE_HAS_IPV6)
  // Retrieve IPv6 local interfaces by scanning /proc/net/if_inet6 if it exists.
  // If we cannot open it then ignore possible IPv6 interfaces, we did our best;-)
  FILE* fp;
  char addr_p[8][5];
  char s_ipaddr[64];
  int scopeid;
  struct addrinfo hints, *res0;
  int error;

  ACE_OS::memset (&hints, 0, sizeof (hints));
  hints.ai_flags = AI_NUMERICHOST;
  hints.ai_family = AF_INET6;

  if ((fp = ACE_OS::fopen (ACE_LIB_TEXT ("/proc/net/if_inet6"), ACE_LIB_TEXT ("r"))) != NULL)
    {
      while (fscanf (fp,
                     "%4s%4s%4s%4s%4s%4s%4s%4s %02x %*02x %*02x %*02x %*8s\n",
                     addr_p[0], addr_p[1], addr_p[2], addr_p[3],
                     addr_p[4], addr_p[5], addr_p[6], addr_p[7], &scopeid) != EOF)
        {
          // Format the address intoa proper IPv6 decimal address specification and
          // resolve the resulting text using getaddrinfo().

          const char* ip_fmt = "%s:%s:%s:%s:%s:%s:%s:%s%%%d";
          ACE_OS::sprintf (s_ipaddr,
                           ip_fmt,
                           addr_p[0], addr_p[1], addr_p[2], addr_p[3],
                           addr_p[4], addr_p[5], addr_p[6], addr_p[7], scopeid);

          error = getaddrinfo (s_ipaddr, 0, &hints, &res0);
          if (error)
            continue;

          if (res0->ai_family == AF_INET6 &&
                !IN6_IS_ADDR_UNSPECIFIED (&reinterpret_cast<sockaddr_in6 *> (res0->ai_addr)->sin6_addr))
            {
              addrs[count].set(reinterpret_cast<sockaddr_in *> (res0->ai_addr), res0->ai_addrlen);
              ++count;
            }
          freeaddrinfo (res0);

        }
      ACE_OS::fclose (fp);
    }
# endif /* ACE_HAS_IPV6 */

  return 0;
#elif defined (ACE_VXWORKS) && (ACE_VXWORKS < 0x600)
  count = 0;
  // Loop through each address structure
  for (struct in_ifaddr* ia = in_ifaddr; ia != 0; ia = ia->ia_next)
    {
      ++count;
    }

  // Now create and initialize output array.
  ACE_NEW_RETURN (addrs,
                  ACE_INET_Addr[count],
                  -1); // caller must free
  count = 0;
  for (struct in_ifaddr* ia = in_ifaddr; ia != 0; ia = ia->ia_next)
    {
      struct ifnet* ifp = ia->ia_ifa.ifa_ifp;
      if (ifp != 0)
        {
          // Get the current interface name
          char interface[64];
          ACE_OS::sprintf(interface, "%s%d", ifp->if_name, ifp->if_unit);

          // Get the address for the current interface
          char address [INET_ADDR_LEN];
          STATUS status = ifAddrGet(interface, address);

          if (status == OK)
            {
              // Concatenate a ':' at the end. This is because in
              // ACE_INET_Addr::string_to_addr, the ip_address is
              // obtained using ':' as the delimiter. Since, using
              // ifAddrGet(), we just get the IP address, I am adding
              // a ":" to get with the general case.
              ACE_OS::strcat (address, ":");
              addrs[count].set (address);
            }
          else
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 ACE_LIB_TEXT ("ACE::get_ip_interface failed\n")
                                 ACE_LIB_TEXT ("Couldnt get the IP Address\n")),
                                 -1);
            }
          ++count;
        }
    }
  return 0;
#else
  ACE_UNUSED_ARG (count);
  ACE_UNUSED_ARG (addrs);
  ACE_NOTSUP_RETURN (-1);                      // no implementation
#endif /* ACE_WIN32 */
}

// Helper routine for get_ip_interfaces, differs by UNIX platform so
// put into own subroutine.  perform some ioctls to retrieve ifconf
// list of ifreq structs.

int
ACE::count_interfaces (ACE_HANDLE handle, size_t &how_many)
{
#if defined (sparc) && defined (SIOCGIFNUM)
  int tmp_how_many; // For 64 bit Solaris
  if (ACE_OS::ioctl (handle,
                     SIOCGIFNUM,
                     (caddr_t) &tmp_how_many) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_LIB_TEXT ("%p\n"),
                       ACE_LIB_TEXT ("ACE::count_interfaces:")
                       ACE_LIB_TEXT ("ioctl - SIOCGIFNUM failed")),
                      -1);
  how_many = (size_t) tmp_how_many;
  return 0;
#elif defined (ACE_HAS_GETIFADDRS)
  ACE_UNUSED_ARG (handle);

  struct ifaddrs *ifap;
  struct ifaddrs *p_if;

  if (::getifaddrs (&ifap) != 0)
    return -1;

  // Count number of interfaces.
  size_t num_ifs = 0;
  for (p_if = ifap; p_if != 0; p_if = p_if->ifa_next)
    ++num_ifs;

  ::freeifaddrs (ifap);

  how_many = num_ifs;
  return 0;
#elif defined (__unix) || defined (__unix__) || defined (__Lynx__) || defined (_AIX) || defined (ACE_OPENVMS)
  // Note: DEC CXX doesn't define "unix".  BSD compatible OS: HP UX,
  // AIX, SunOS 4.x perform some ioctls to retrieve ifconf list of
  // ifreq structs no SIOCGIFNUM on SunOS 4.x, so use guess and scan
  // algorithm

  // Probably hard to put this many ifs in a unix box..
  const int MAX_IF = 50;

  // HACK - set to an unreasonable number
  int num_ifs = MAX_IF;

  struct ifconf ifcfg;
  size_t ifreq_size = num_ifs * sizeof (struct ifreq);
  struct ifreq *p_ifs =
    (struct ifreq *) ACE_OS::malloc (ifreq_size);

  if (!p_ifs)
    {
      errno = ENOMEM;
      return -1;
    }

  ACE_OS::memset (p_ifs, 0, ifreq_size);
  ACE_OS::memset (&ifcfg, 0, sizeof (struct ifconf));

  ifcfg.ifc_req = p_ifs;
  ifcfg.ifc_len = ifreq_size;

  int cmd = SIOCGIFCONF;
  if (ACE_OS::ioctl (handle,
                     cmd,
                     (caddr_t) &ifcfg) == -1)
    {
      ACE_OS::free (ifcfg.ifc_req);
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_LIB_TEXT ("%p\n"),
                         ACE_LIB_TEXT ("ACE::count_interfaces:")
                         ACE_LIB_TEXT ("ioctl - SIOCGIFCONF failed")),
                        -1);
    }

  int if_count = 0, i;

  // get if address out of ifreq buffers.  ioctl puts a blank-named
  // interface to mark the end of the returned interfaces.
  for (i = 0;
       i < num_ifs;
       i++)
    {
      /* In OpenBSD, the length of the list is returned. */
      ifcfg.ifc_len -= sizeof (struct ifreq);
      if (ifcfg.ifc_len < 0)
        break;

      if_count++;
#if !defined(AIX) && !defined (__QNX__) && !defined (__FreeBSD__) && !defined(__NetBSD__)
      p_ifs++;
#else
     if (p_ifs->ifr_addr.sa_len <= sizeof (struct sockaddr))
       {
          p_ifs++;
       }
       else
       {
          p_ifs = (struct ifreq *)
              (p_ifs->ifr_addr.sa_len + (caddr_t) &p_ifs->ifr_addr);
       }
#endif /* !defined(AIX) && !defined (__QNX__) && !defined (__FreeBSD__) && !defined(__NetBSD__) */
    }

  ACE_OS::free (ifcfg.ifc_req);

# if defined (ACE_HAS_IPV6)
  FILE* fp;

  if ((fp = ACE_OS::fopen (ACE_LIB_TEXT ("/proc/net/if_inet6"), ACE_LIB_TEXT ("r"))) != NULL)
    {
      // Scan the lines according to the expected format but don't really read any input
      while (fscanf (fp, "%*32s %*02x %*02x %*02x %*02x %*8s\n") != EOF)
        {
          if_count++;
        }
      ACE_OS::fclose (fp);
    }
# endif /* ACE_HAS_IPV6 */

  how_many = if_count;
  return 0;
#else
  ACE_UNUSED_ARG (handle);
  ACE_UNUSED_ARG (how_many);
  ACE_NOTSUP_RETURN (-1); // no implementation
#endif /* sparc && SIOCGIFNUM */
}

// Routine to return a handle from which ioctl() requests can be made.

ACE_HANDLE
ACE::get_handle (void)
{
  // Solaris 2.x
  ACE_HANDLE handle = ACE_INVALID_HANDLE;
#if defined (sparc)
  handle = ACE_OS::open ("/dev/udp", O_RDONLY);
#elif defined (__unix) || defined (__unix__) || defined (__Lynx__) || defined (_AIX) || (defined (ACE_VXWORKS) && (ACE_VXWORKS >= 0x600)) || defined (ACE_OPENVMS)
  // Note: DEC CXX doesn't define "unix" BSD compatible OS: HP UX,
  // AIX, SunOS 4.x

  handle = ACE_OS::socket (PF_INET, SOCK_DGRAM, 0);
#endif /* sparc */
  return handle;
}

bool
ACE::ipv4_enabled (void)
{
#if defined (ACE_HAS_IPV6)
  if (ace_ipv4_enabled == -1)
    {
      // Perform Double-Checked Locking Optimization.
      ACE_MT (ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, ace_mon,
                                *ACE_Static_Object_Lock::instance (), false));

      if (ace_ipv4_enabled == -1)
        {
          // Determine if the kernel has IPv6 support by attempting to
          // create a PF_INET socket and see if it fails.
          ACE_HANDLE const s = ACE_OS::socket (PF_INET, SOCK_DGRAM, 0);
          if (s == ACE_INVALID_HANDLE)
            {
              ace_ipv4_enabled = 0;
            }
          else
            {
              ace_ipv4_enabled = 1;
              ACE_OS::closesocket (s);
            }
        }
    }

  return static_cast<bool> (ace_ipv4_enabled);
#else
 // Assume it's always enabled since ACE requires some version of
 // TCP/IP to exist.
  return true;
#endif  /* ACE_HAS_IPV6*/
}

int
ACE::ipv6_enabled (void)
{
#if defined (ACE_HAS_IPV6)
  if (ace_ipv6_enabled == -1)
    {
      // Perform Double-Checked Locking Optimization.
      ACE_MT (ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, ace_mon,
                                *ACE_Static_Object_Lock::instance (), 0));

      if (ace_ipv6_enabled == -1)
        {
          // Determine if the kernel has IPv6 support by attempting to
          // create a PF_INET6 socket and see if it fails.
          ACE_HANDLE s = ACE_OS::socket (PF_INET6, SOCK_DGRAM, 0);
          if (s == ACE_INVALID_HANDLE)
            {
              ace_ipv6_enabled = 0;
            }
          else
            {
              ace_ipv6_enabled = 1;
              ACE_OS::closesocket (s);
            }
        }
    }

  return ace_ipv6_enabled;
#else /* ACE_HAS_IPV6 */
  return 0;
#endif /* !ACE_HAS_IPV6 */
}

ACE_END_VERSIONED_NAMESPACE_DECL
