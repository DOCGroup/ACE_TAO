// $Id$

#include "ace/Sock_Connect.h"
#include "ace/INET_Addr.h"
#include "ace/Log_Msg.h"
#include "ace/Handle_Set.h"
#include "ace/Auto_Ptr.h"
#include "ace/SString.h"

#if defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "ace/Sock_Connect.i"
#endif /* ACE_LACKS_INLINE_FUNCTIONS */

ACE_RCSID(ace, Sock_Connect, "$Id$")

#if defined (ACE_WIN32) && \
(!defined (ACE_HAS_WINSOCK2) || (defined (ACE_HAS_WINSOCK2) && (ACE_HAS_WINSOCK2 == 0)))
// Return value in buffer for a key/name pair from the Windows
// Registry up to buf_len size.

static int
get_reg_value (const ACE_TCHAR *key,
               const ACE_TCHAR *name,
               ACE_TCHAR *buffer,
               DWORD &buf_len)
{
  HKEY hk;
  DWORD buf_type;
  LONG rc = ACE_TEXT_RegOpenKeyEx (HKEY_LOCAL_MACHINE,
                                   key,
                                   0,
                                   KEY_READ,
                                   &hk);
  // 1. open key that defines the interfaces used for TCP/IP?
  if (rc != ERROR_SUCCESS)
    // print_error_string(ACE_LIB_TEXT ("RegOpenKeyEx"), rc);
    return -1;

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

  ::RegCloseKey (hk);
  return 0;
}
#endif //(ACE_WIN32) && !(ACE_HAS_WINSOCK2) || (ACE_HAS_WINSOCK2) && (ACE_HAS_WINSOCK2 == 0)

// Bind socket to an unused port.

int
ACE_Sock_Connect::bind_port (ACE_HANDLE handle,
                ACE_UINT32 ip_addr)
{
  ACE_TRACE ("ACE_Sock_Connect::bind_port");

  sockaddr_in sock_addr;

  ACE_OS::memset ((void *) &sock_addr, 0, sizeof sock_addr);
  sock_addr.sin_family = AF_INET;
#if defined (ACE_HAS_SIN_LEN)
  sock_addr.sin_len = sizeof sock_addr;
#endif /* ACE_HAS_SIN_LEN */
  sock_addr.sin_addr.s_addr = ip_addr;

#if !defined (ACE_LACKS_WILDCARD_BIND)
  // The OS kernel should select a free port for us.
  sock_addr.sin_port = 0;
  return ACE_OS::bind (handle,
                       ACE_reinterpret_cast(sockaddr *, &sock_addr),
                       sizeof sock_addr);
#else
  static u_short upper_limit = ACE_MAX_DEFAULT_PORT;
  int round_trip = upper_limit;
  int lower_limit = IPPORT_RESERVED;

  // We have to select the port explicitly.

  for (;;)
    {
      sock_addr.sin_port = htons (upper_limit);

      if (ACE_OS::bind (handle,
                        ACE_reinterpret_cast(sockaddr *, &sock_addr),
                        sizeof sock_addr) >= 0)
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
ACE_Sock_Connect::get_bcast_addr (ACE_UINT32 &bcast_addr,
                     const ACE_TCHAR *host_name,
                     ACE_UINT32 host_addr,
                     ACE_HANDLE handle)
{
  ACE_TRACE ("ACE_Sock_Connect::get_bcast_addr");

#if !defined(ACE_WIN32)
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
#if defined (AIX)
  int cmd = CSIOCGIFCONF;
#else
  int cmd = SIOCGIFCONF;
#endif /* AIX */
  if (ACE_OS::ioctl (s, cmd, (char *) &ifc) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_LIB_TEXT ("%p\n"),
                       ACE_LIB_TEXT ("ACE_Sock_Connect::get_bcast_addr:")
                       ACE_LIB_TEXT ("ioctl (get interface configuration)")),
                      -1);

  struct ifreq *ifr = ifc.ifc_req;

  struct sockaddr_in ip_addr;

  // Get host ip address if necessary.
  if (host_name)
    {
      hostent *hp = ACE_OS::gethostbyname (host_name);

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

  for (int n = ifc.ifc_len / sizeof (struct ifreq);
       n > 0;
#if !defined(CHORUS_4)
       n--, ifr++)
#else
       n--,
           ((ifr->ifr_addr.sa_len <= sizeof (struct sockaddr)) ?
             ifr++ :
             ifr = (struct ifreq *)
             (ifr->ifr_addr.sa_len + (caddr_t) &ifr->ifr_addr)))
#endif /* CHORUS_4 */
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
                      ACE_LIB_TEXT ("ACE_Sock_Connect::get_bcast_addr:")
                      ACE_LIB_TEXT ("Not AF_INET")));
          continue;
        }

      struct ifreq flags = *ifr;
      struct ifreq if_req = *ifr;

      if (ACE_OS::ioctl (s, SIOCGIFFLAGS, (char *) &flags) == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_LIB_TEXT ("%p\n"),
                      ACE_LIB_TEXT ("ACE_Sock_Connect::get_bcast_addr:")
                      ACE_LIB_TEXT (" ioctl (get interface flags)")));
          continue;
        }

      if (ACE_BIT_DISABLED (flags.ifr_flags, IFF_UP))
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_LIB_TEXT ("%p\n"),
                      ACE_LIB_TEXT ("ACE_Sock_Connect::get_bcast_addr:")
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
                        ACE_LIB_TEXT ("ACE_Sock_Connect::get_bcast_addr:")
                        ACE_LIB_TEXT ("ioctl (get broadaddr)")));
          else
            {
              ACE_OS::memcpy (ACE_reinterpret_cast(sockaddr_in *, &ip_addr),
                              ACE_reinterpret_cast(sockaddr_in *, &if_req.ifr_broadaddr),
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
                    ACE_LIB_TEXT ("ACE_Sock_Connect::get_bcast_addr:")
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
#endif /* !ACE_WIN32 */
}

// return an array of all configured IP interfaces on this host, count
// rc = 0 on success (count == number of interfaces else -1 caller is
// responsible for calling delete [] on parray

int
ACE_Sock_Connect::get_ip_interfaces (size_t &count,
                                     ACE_INET_Addr *&addrs)
{
  ACE_TRACE ("ACE_Sock_Connect::get_ip_interfaces");

  count = 0;
  addrs = 0;

#if defined (ACE_WIN32)
  // Win32 can do this by a simple API call if MSVC 5 or later is the compiler.
  // Not sure if Borland supplies the needed header/lib, but it might.
# if defined (ACE_HAS_WINSOCK2) && (ACE_HAS_WINSOCK2 != 0)
#if 0
  // If this also needs to be predicated on MSVC 5 or later, add the
  // following condition to the #if above.  It tests ok at Riverace w/ 4.2,
  // but this isn't a virgin install of 4.2 so there's a minimal risk that
  // it may need work later.
  defined (_MSC_VER) && (_MSC_VER >= 1100)
#endif /* 0 */

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
  if (n_interfaces == 0)
    return 0;

  ACE_NEW_RETURN (addrs,
                  ACE_INET_Addr[n_interfaces],
                  -1);

  // Now go through the list and transfer the good ones to the list of
  // because they're down or don't have an IP address.
  for (count = 0, i = 0; i < n_interfaces; i++)
    {
      LPINTERFACE_INFO lpii;
      struct sockaddr_in *addrp;

      lpii = &info[i];
      if (!(lpii->iiFlags & IFF_UP))
        continue;

      // We assume IPv4 addresses here
      addrp = ACE_reinterpret_cast(struct sockaddr_in *, &(lpii->iiAddress));
      if (addrp->sin_addr.s_addr == INADDR_ANY)
        continue;

      // Set the address for the caller.
      addrs[count].set(addrp, sizeof(sockaddr_in));
      ++count;
    }

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

  const ACE_TCHAR *SVCS_KEY1 =
    ACE_LIB_TEXT ("SYSTEM\\CurrentControlSet\\Services\\");
  const ACE_TCHAR *LINKAGE_KEY1 =
    ACE_LIB_TEXT ("SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Linkage");
  const ACE_TCHAR *TCP_PARAM_SUBKEY = ACE_LIB_TEXT ("\\Parameters\\Tcpip");
  const ACE_TCHAR *BIND_NAME_ID =  ACE_LIB_TEXT ("Bind");
  const ACE_TCHAR *IPADDR_NAME_ID = ACE_LIB_TEXT ("IPAddress");
  const ACE_TCHAR *INVALID_TCPIP_DEVICE_ADDR = ACE_LIB_TEXT ("0.0.0.0");

  ACE_TCHAR raw_buffer[ACE_MAX_FULLY_QUALIFIED_NAME_LEN + 1];
  DWORD raw_buflen = ACE_MAX_FULLY_QUALIFIED_NAME_LEN + 1;
  ACE_TCHAR buffer[ACE_MAX_FULLY_QUALIFIED_NAME_LEN + 1];
  DWORD buf_len = ACE_MAX_FULLY_QUALIFIED_NAME_LEN + 1;

  if (::get_reg_value (LINKAGE_KEY1,
                       BIND_NAME_ID,
                       raw_buffer,
                       raw_buflen))
    return -1;
  // return buffer contains 0 delimited strings

  ACE_Tokenizer dev_names (raw_buffer);
  dev_names.delimiter (ACE_LIB_TEXT ('\0'));
  int n_interfaces = 0;

  // Count the number of interfaces
  while (dev_names.next () != 0)
    n_interfaces ++;

  // case 1. no interfaces present, empty string? OS version change?
  if (n_interfaces == 0)
    return 0;

  ACE_NEW_RETURN (addrs,
                  ACE_INET_Addr[n_interfaces],
                  -2);
  count = 0;
  for (int i = 0; i < n_interfaces; i++)
    {
      // a. construct name to access IPAddress for this interface
      ACE_TString ifdevkey (SVCS_KEY1);
      ACE_TString the_dev = dev_names.next ();

      // chop off the "\Device" and keep last name.
      if (the_dev.length() < 8)
        return -3;              // Something's wrong
      else
        {
          // rest of string from offset 8
          the_dev = the_dev.substring (8);
          ifdevkey += the_dev;
          ifdevkey += TCP_PARAM_SUBKEY;

          // b. extract value
          // Gets overwritten on each call
          buf_len = sizeof (buffer);
          if (get_reg_value (ifdevkey.fast_rep (),
                             IPADDR_NAME_ID,
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

#elif defined (__unix) || defined (__Lynx__) || defined (_AIX)
  // COMMON (SVR4 and BSD) UNIX CODE

  size_t num_ifs;

  // Call specific routine as necessary.
  ACE_HANDLE handle = get_handle();

  if (handle == ACE_INVALID_HANDLE)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_LIB_TEXT ("%p\n"),
                       ACE_LIB_TEXT ("ACE_Sock_Connect::get_ip_interfaces:open")),
                      -1);
  if (ACE_Sock_Connect::count_interfaces (handle, num_ifs))
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

#if defined (AIX)
  int cmd = CSIOCGIFCONF;
#else
  int cmd = SIOCGIFCONF;
#endif /* AIX */
  if (ACE_OS::ioctl (handle,
                     cmd,
                     (caddr_t) &ifcfg) == -1)
    {
      ACE_OS::close (handle);
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_LIB_TEXT ("%p\n"),
                         ACE_LIB_TEXT ("is_address_local:")
                         ACE_LIB_TEXT ("ioctl - SIOCGIFCONF failed")),
                        -1);
    }

  ACE_OS::close (handle);

  // Now create and initialize output array.

  ACE_NEW_RETURN (addrs,
                  ACE_INET_Addr[num_ifs],
                  -1); // caller must free

  struct ifreq *pcur = p_ifs.get ();
  // Pull the address out of each INET interface.  Not every interface
  // is for IP, so be careful to count properly.  When setting the
  // INET_Addr, note that the 3rd arg (0) says to leave the byte order
  // (already in net byte order from the interface structure) as is.
  count = 0;

  for (size_t i = 0;
       i < num_ifs;
       i++)
    {
      if (pcur->ifr_addr.sa_family == AF_INET)
        {
#if !defined(_UNICOS)
          struct sockaddr_in *addr =
            ACE_reinterpret_cast(sockaddr_in *, &pcur->ifr_addr);

          // Sometimes the kernel returns 0.0.0.0 as the interface
          // address, skip those...
          if (addr->sin_addr.s_addr != 0)
            {
              addrs[count].set ((u_short) 0,
                                addr->sin_addr.s_addr,
                                0);
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
              count++;
            }
#endif /* ! _UNICOS */
        }

#if !defined(CHORUS_4)
      pcur++;
#else
      if (pcur->ifr_addr.sa_len <= sizeof (struct sockaddr))
        {
           pcur++;
        }
      else
        {
           pcur = (struct ifreq *)
               (pcur->ifr_addr.sa_len + (caddr_t) &pcur->ifr_addr);
        }
#endif
    }
  return 0;
#else
  ACE_UNUSED_ARG (count);
  ACE_UNUSED_ARG (addrs);
  ACE_NOTSUP_RETURN (-1);;                      // no implementation
#endif /* ACE_WIN32 */
}

// Helper routine for get_ip_interfaces, differs by UNIX platform so
// put into own subroutine.  perform some ioctls to retrieve ifconf
// list of ifreq structs.

int
ACE_Sock_Connect::count_interfaces (ACE_HANDLE handle,
                       size_t &how_many)
{
#if defined (sparc) && defined (SIOCGIFNUM)
  int tmp_how_many; // For 64 bit Solaris
  if (ACE_OS::ioctl (handle,
                     SIOCGIFNUM,
                     (caddr_t) &tmp_how_many) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_LIB_TEXT ("ACE_Sock_Connect::get_ip_interfaces:")
                       ACE_LIB_TEXT ("ioctl - SIOCGIFNUM failed")),
                      -1);
  how_many = (size_t) tmp_how_many;
  return 0;
#elif defined (__unix) || defined (__Lynx__) || defined (_AIX)
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

#if defined (AIX)
  int cmd = CSIOCGIFCONF;
#else
  int cmd = SIOCGIFCONF;
#endif /* AIX */
  if (ACE_OS::ioctl (handle,
                     cmd,
                     (caddr_t) &ifcfg) == -1)
    {
      ACE_OS::free (ifcfg.ifc_req);
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_LIB_TEXT ("count_interfaces:ioctl:")
                         ACE_LIB_TEXT ("SIOCGIFCONF failed")),
                        -1);
    }

  int if_count = 0, i;

  // get if address out of ifreq buffers.  ioctl puts a blank-named
  // interface to mark the end of the returned interfaces.
  for (i = 0;
       i < num_ifs;
       i++)
    {
      if (p_ifs->ifr_name[0] == '\0')
        break;

      if_count++;
#if !defined(CHORUS_4)
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
#endif /* CHORUS_4 */
    }

  ACE_OS::free (ifcfg.ifc_req);
  how_many = if_count;
  return 0;
#else
  ACE_UNUSED_ARG (handle);
  ACE_UNUSED_ARG (how_many);
  ACE_NOTSUP_RETURN (-1);; // no implmentation
#endif /* sparc && SIOCGIFNUM */
}

// Routine to return a handle from which ioctl() requests can be made.

ACE_HANDLE
ACE_Sock_Connect::get_handle (void)
{
  // Solaris 2.x
  ACE_HANDLE handle = ACE_INVALID_HANDLE;
#if defined (sparc) && ! defined (CHORUS)
  handle = ACE_OS::open ("/dev/udp", O_RDONLY);
#elif defined (__unix) || defined (__Lynx__) || defined (_AIX)
  // Note: DEC CXX doesn't define "unix" BSD compatible OS: HP UX,
  // AIX, SunOS 4.x

  handle = ACE_OS::socket (PF_INET, SOCK_DGRAM, 0);
#endif /* sparc */
  return handle;
}
