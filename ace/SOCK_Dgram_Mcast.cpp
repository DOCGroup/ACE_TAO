// $Id$

#include "ace/SOCK_Dgram_Mcast.h"
#include "ace/INET_Addr.h"

#if defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "ace/SOCK_Dgram_Mcast.i"
#endif /* ACE_LACKS_INLINE_FUNCTIONS */

// This is a workaround for platforms with non-standard
// definitions of the ip_mreq structure
#if ! defined (IMR_MULTIADDR)
#define IMR_MULTIADDR imr_multiaddr
#endif /* ! defined (IMR_MULTIADDR) */

ACE_RCSID(ace, SOCK_Dgram_Mcast, "$Id$")

ACE_ALLOC_HOOK_DEFINE(ACE_SOCK_Dgram_Mcast)

void
ACE_SOCK_Dgram_Mcast::dump (void) const
{
  ACE_TRACE ("ACE_SOCK_Dgram_Mcast::dump");
}

// Dummy default constructor...

ACE_SOCK_Dgram_Mcast::ACE_SOCK_Dgram_Mcast (void)
{
  ACE_TRACE ("ACE_SOCK_Dgram_Mcast::ACE_SOCK_Dgram_Mcast");
}

int
ACE_SOCK_Dgram_Mcast::open (const ACE_Addr &mcast_addr,
                            int protocol_family,
                            int protocol,
                            int reuse_addr)
{
  ACE_TRACE ("ACE_SOCK_Dgram_Mcast::open");

  // Make a copy of address to use in the <send> methods.  Note: Sun
  // C++ 4.2 needs the useless const_cast.
  this->mcast_addr_.set (ACE_reinterpret_cast (const ACE_INET_Addr &,
                                               ACE_const_cast (ACE_Addr &,
                                                               mcast_addr)));
  // Only perform the <open> initialization if we haven't been opened
  // earlier.
  if (this->get_handle () == ACE_INVALID_HANDLE)
    {
      if (ACE_SOCK::open (SOCK_DGRAM,
                          protocol_family,
                          protocol,
                          reuse_addr) == -1)
        return -1;

      int one = 1;
      if (reuse_addr
          && this->ACE_SOCK::set_option (SOL_SOCKET,
                                         SO_REUSEADDR,
                                         &one,
                                         sizeof one) == -1)
        return -1;
#if defined (SO_REUSEPORT)
      else if (this->ACE_SOCK::set_option (SOL_SOCKET,
                                           SO_REUSEPORT,
                                           &one,
                                           sizeof one) == -1)
        return -1;
#endif /* SO_REUSEPORT */

      // Create an address to bind the socket to.
      ACE_INET_Addr local;

#if defined (linux)
      local = this->mcast_addr_;
#else /* linux */
      if (local.set (this->mcast_addr_.get_port_number ()) == -1)
        return -1;
#endif /* linux */

      if (ACE_SOCK_Dgram::shared_open (local,
                                       protocol_family) == -1)
        return -1;
    }

  return 0;
}

int
ACE_SOCK_Dgram_Mcast::subscribe_ifs (const ACE_INET_Addr &mcast_addr,
                                     const ACE_TCHAR *net_if,
                                     int protocol_family,
                                     int protocol,
                                     int reuse_addr)
{
  ACE_TRACE ("ACE_SOCK_Dgram_Mcast::subscribe_ifs");
#if defined (ACE_WIN32)
  // Windows NT's winsock has trouble with multicast subscribes in the
  // presence of multiple network interfaces when the IP address is
  // given as INADDR_ANY.  It will pick the first interface and only
  // accept mcast there.  So, to work around this, cycle through all
  // of the interfaces known and subscribe to all the non-loopback
  // ones.
  //
  // Note that this only needs to be done on NT, but there's no way to
  // tell at this point if the code will be running on NT - only if it
  // is compiled for NT-only or for NT/95, and that doesn't really
  // help us.  It doesn't hurt to do this on Win95, it's just a little
  // slower than it normally would be.
  //
  // NOTE - <ACE::get_ip_interfaces> doesn't always get all of the
  // interfaces.  In particular, it may not get a PPP interface.  This
  // is a limitation of the way <ACE::get_ip_interfaces> works with
  // MSVC.  The reliable way of getting the interface list is
  // available only with MSVC 5.

  if (net_if == 0)
    {
      ACE_INET_Addr *if_addrs = 0;
      size_t if_cnt;

      if (ACE::get_ip_interfaces (if_cnt,
                                  if_addrs) != 0)
        return -1;

      size_t nr_subscribed = 0;

      if (if_cnt < 2)
        {
          if (this->subscribe (mcast_addr,
                               reuse_addr,
                               ACE_LIB_TEXT ("0.0.0.0"),
                               protocol_family,
                               protocol) == 0)
            ++nr_subscribed;
        }
      else
        // Iterate through all the interfaces, figure out which ones
        // offer multicast service, and subscribe to them.
        while (if_cnt > 0)
          {
            --if_cnt;

            // Convert to 0-based for indexing, next loop check.
            if (if_addrs[if_cnt].get_ip_address() == INADDR_LOOPBACK)
              continue;
            if (this->subscribe (mcast_addr,
                                 reuse_addr,
                                 ACE_TEXT_CHAR_TO_TCHAR
                                   (if_addrs[if_cnt].get_host_addr()),
                                 protocol_family,
                                 protocol) == 0)
              ++nr_subscribed;
          }

      delete [] if_addrs;

      if (nr_subscribed == 0)
        {
          errno = ENODEV;
          return -1;
        }
      else
        // 1 indicates a "short-circuit" return.  This handles the
        // rather bizarre semantics of checking all the interfaces on
        // NT.
        return 1;
    }
#else
  ACE_UNUSED_ARG (mcast_addr);
  ACE_UNUSED_ARG (net_if);
  ACE_UNUSED_ARG (protocol_family);
  ACE_UNUSED_ARG (protocol);
  ACE_UNUSED_ARG (reuse_addr);
#endif /* ACE_WIN32 */
  // Otherwise, do it like everyone else...

  return 0;
}

int
ACE_SOCK_Dgram_Mcast::subscribe (const ACE_INET_Addr &mcast_addr,
                                 int reuse_addr,
                                 const ACE_TCHAR *net_if,
                                 int protocol_family,
                                 int protocol)
{
  ACE_TRACE ("ACE_SOCK_Dgram_Mcast::subscribe");

  if (this->open (mcast_addr,
                  protocol_family,
                  protocol,
                  reuse_addr) == -1)
    return -1;

  int result = this->subscribe_ifs (mcast_addr,
                                    net_if,
                                    protocol_family,
                                    protocol,
                                    reuse_addr);
  // Check for the "short-circuit" return value of 1 (for NT).  This
  // handles the rather bizarre semantics of checking all the
  // interfaces on NT.
  if (result != 0)
    return result;

  ip_mreq multicast_address;

  // Create multicast request.
  result = this->make_multicast_address_i (mcast_addr,
                                           multicast_address,
                                           net_if);
  if (result != 0)
    return result;

  // Tell network device driver to read datagrams with a
  // multicast_address IP interface.
  result = this->ACE_SOCK::set_option (IPPROTO_IP,
                                       IP_ADD_MEMBERSHIP,
                                       &multicast_address,
                                       sizeof multicast_address);
  if (result != 0)
    return result;

  // Set the <mcast_request_if_> field.  This is mostly a convenience
  // for applications that subscribe to a single multicast group,
  // using the internal field allows them to simply call the
  // unsubscribe() operation.
  // Notice that such applications are using a non-reentrant interface
  // of the class, and thus should use proper synchronization for it.
  this->mcast_request_if_ = multicast_address;

  return 0;
}

int
ACE_SOCK_Dgram_Mcast::unsubscribe_ifs (const ACE_INET_Addr &mcast_addr,
                                       const ACE_TCHAR *net_if,
                                       int protocol_family,
                                       int protocol)
{
  ACE_TRACE ("ACE_SOCK_Dgram_Mcast::unsubscribe_ifs");
#if defined (ACE_WIN32)
  // Windows NT's winsock has trouble with multicast subscribes in the
  // presence of multiple network interfaces when the IP address is
  // given as INADDR_ANY.  It will pick the first interface and only
  // accept mcast there.  So, to work around this, cycle through all
  // of the interfaces known and subscribe to all the non-loopback
  // ones.
  //
  // Note that this only needs to be done on NT, but there's no way to
  // tell at this point if the code will be running on NT - only if it
  // is compiled for NT-only or for NT/95, and that doesn't really
  // help us.  It doesn't hurt to do this on Win95, it's just a little
  // slower than it normally would be.
  //
  // NOTE - <ACE::get_ip_interfaces> doesn't always get all of the
  // interfaces.  In particular, it may not get a PPP interface.  This
  // is a limitation of the way <ACE::get_ip_interfaces> works with
  // MSVC.  The reliable way of getting the interface list is
  // available only with MSVC 5.

  if (net_if == 0)
    {
      ACE_INET_Addr *if_addrs = 0;
      size_t if_cnt;

      if (ACE::get_ip_interfaces (if_cnt,
                                  if_addrs) != 0)
        return -1;

      size_t nr_unsubscribed = 0;

      if (if_cnt < 2)
        {
          if (this->unsubscribe (mcast_addr,
                                 ACE_LIB_TEXT ("0.0.0.0"),
                                 protocol_family,
                                 protocol) == 0)
            ++nr_unsubscribed;
        }
      else
        while (if_cnt > 0)
          {
            --if_cnt;
            // Convert to 0-based for indexing, next loop check
            if (if_addrs[if_cnt].get_ip_address() == INADDR_LOOPBACK)
              continue;
            if (this->unsubscribe (mcast_addr,
                                   ACE_TEXT_CHAR_TO_TCHAR
                                     (if_addrs[if_cnt].get_host_addr()),
                                   protocol_family,
                                   protocol) == 0)
              ++nr_unsubscribed;
          }

      delete [] if_addrs;

      if (nr_unsubscribed == 0)
        {
          errno = ENODEV;
          return -1;
        }

      return 1;
    }
#else
  ACE_UNUSED_ARG (mcast_addr);
  ACE_UNUSED_ARG (net_if);
  ACE_UNUSED_ARG (protocol_family);
  ACE_UNUSED_ARG (protocol);
#endif /* ACE_WIN32 */

  return 0;
}

int
ACE_SOCK_Dgram_Mcast::unsubscribe (const ACE_INET_Addr &mcast_addr,
                                   const ACE_TCHAR *net_if,
                                   int protocol_family,
                                   int protocol)
{
  ACE_TRACE ("ACE_SOCK_Dgram_Mcast::unsubscribe");
  int result = this->unsubscribe_ifs (mcast_addr,
                                      net_if,
                                      protocol_family,
                                      protocol);

  // Check for the "short-circuit" return value of 1 (for NT).
  if (result != 0)
    return result;

  // Otherwise, do it like everyone else.

  ip_mreq multicast_address;

  // Create multicast request.
  result = this->make_multicast_address_i (mcast_addr,
                                           multicast_address,
                                           net_if);
  if (result != 0)
    return result;

  // Tell network device driver to stop reading datagrams with the
  // <mcast_addr>.
  return ACE_SOCK::set_option (IPPROTO_IP,
                               IP_DROP_MEMBERSHIP,
                               &multicast_address,
                               sizeof multicast_address);
}

int
ACE_SOCK_Dgram_Mcast::unsubscribe (void)
{
  ACE_TRACE ("ACE_SOCK_Dgram_Mcast::unsubscribe");
  return this->ACE_SOCK::set_option (IPPROTO_IP,
                                     IP_DROP_MEMBERSHIP,
                                     &this->mcast_request_if_,
                                     sizeof this->mcast_request_if_);
}

int
ACE_SOCK_Dgram_Mcast::make_multicast_address_i (const ACE_INET_Addr &mcast_addr,
                                                ip_mreq &multicast_address ,
                                                const ACE_TCHAR *net_if)
{
  if (net_if != 0)
    {
#if defined (ACE_WIN32)
      // This port number is not necessary, just convenient
      ACE_INET_Addr interface_addr;
      if (interface_addr.set (mcast_addr.get_port_number (),
                              net_if) == -1)
        return -1;
      multicast_address.imr_interface.s_addr =
        htonl (interface_addr.get_ip_address ());
#else
      ifreq if_address;

#if defined (ACE_PSOS)
      // Look up the interface by number, not name.
      if_address.ifr_ifno = ACE_OS::atoi (net_if);
#else
      ACE_OS::strcpy (if_address.ifr_name, net_if);
#endif /* defined (ACE_PSOS) */

      if (ACE_OS::ioctl (this->get_handle (),
                         SIOCGIFADDR,
                         &if_address) == -1)
        return -1;

      sockaddr_in *socket_address;
      socket_address = ACE_reinterpret_cast(sockaddr_in *,
                                            &if_address.ifr_addr);
      multicast_address.imr_interface.s_addr = socket_address->sin_addr.s_addr;
#endif /* ACE_WIN32 */
    }
  else
    multicast_address.imr_interface.s_addr = INADDR_ANY;

  multicast_address.IMR_MULTIADDR.s_addr = htonl (mcast_addr.get_ip_address ());
  return 0;
}

int
ACE_SOCK_Dgram_Mcast::make_multicast_address (const ACE_INET_Addr &mcast_addr,
                                              const ACE_TCHAR *net_if)
{
  ACE_TRACE ("ACE_SOCK_Dgram_Mcast::make_multicast_address");

  return this->make_multicast_address_i (mcast_addr,
                                         this->mcast_request_if_,
                                         net_if );
}
