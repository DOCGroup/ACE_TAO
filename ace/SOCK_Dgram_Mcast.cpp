// SOCK_Dgram_Mcast.cpp
// $Id$

#define ACE_BUILD_DLL
#include "ace/SOCK_Dgram_Mcast.h"
#include "ace/INET_Addr.h"

#if defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "ace/SOCK_Dgram_Mcast.i"
#endif

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
ACE_SOCK_Dgram_Mcast::subscribe (const ACE_INET_Addr &mcast_addr,
				 int reuse_addr,
				 const ASYS_TCHAR *net_if,
				 int protocol_family,
				 int protocol)
{
  ACE_TRACE ("ACE_SOCK_Dgram_Mcast::subscribe");

  // Make a local copy of address to use in sends.
  this->mcast_addr_.set (mcast_addr);

  // Only perform the open() initialization if we haven't been opened
  // previously.
  if (this->get_handle () == ACE_INVALID_HANDLE)
    {
      if (ACE_SOCK::open (SOCK_DGRAM, protocol_family, 
		      protocol, reuse_addr) == -1)
        return -1;

      int one = 1;
      if (reuse_addr 
          && this->ACE_SOCK::set_option (SOL_SOCKET,
                                         SO_REUSEADDR,
                                         &one,
                                         sizeof one) == -1)
        return -1;
#if defined (SO_REUSEPORT)
      if (this->ACE_SOCK::set_option (SOL_SOCKET,
                                      SO_REUSEPORT,
                                      &one,
                                      sizeof one) == -1)
        return -1;
#endif /* SO_REUSEPORT */

      // Create an address to bind the socket to.
  
      ACE_INET_Addr local;
  
      if (local.set (this->mcast_addr_.get_port_number ()) == -1)
        return -1;
      else if (ACE_SOCK_Dgram::shared_open (local, protocol_family) == -1)
        return -1;
    }
#if defined (ACE_WIN32)
  // Windows NT's winsock has trouble with multicast subscribes in the presence
  // of multiple network interfaces when the IP address is given as INADDR_ANY.
  // It will pick the first interface and only accept mcast there.  So, to work
  // around this, cycle through all of the interfaces known and subscribe to
  // all the non-loopback ones.
  // Note that this only needs to be done on NT, but there's no way to tell
  // at this point if the code will be running on NT - only if it is compiled
  // for NT-only or for NT/95, and that doesn't really help us.  It doesn't
  // hurt to do this on Win95, it's just a little slower than it normally
  // would be.
  //
  // NOTE - get_ip_interfaces doesn't always get all of the interfaces.  In
  // particular, it may not get a PPP interface.  This is a limitation of the
  // way get_ip_interfaces works with MSVC.  The reliable way of getting the
  // interface list is available only with MSVC 5.

  if (net_if == 0)
    {
      ACE_INET_Addr *if_addrs;
      size_t         if_cnt, nr_subscribed;

      if (ACE::get_ip_interfaces(if_cnt, if_addrs) != 0)
	return -1;

      for (nr_subscribed = 0; if_cnt > 0; )
	{
	  --if_cnt;    // Convert to 0-based for indexing, and next loop check
	  if (if_addrs[if_cnt].get_ip_address() == INADDR_LOOPBACK)
	    continue;
	  if (this->subscribe(mcast_addr,
			      reuse_addr,
			      ASYS_WIDE_STRING (if_addrs[if_cnt].get_host_addr()),
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
      return 0;
    }
  // else do it like everyone else...
#endif /* ACE_WIN32 */

  // Create multicast request.
  if (this->make_multicast_address (this->mcast_addr_, net_if) == -1)
    return -1;

  // Tell network device driver to read datagrams with a
  // multicast_address address.
  else if (this->ACE_SOCK::set_option (IPPROTO_IP, 
                                       IP_ADD_MEMBERSHIP,
                                       &multicast_address_, 
                                       sizeof multicast_address_) == -1)
    return -1;
  return 0;
}

int
ACE_SOCK_Dgram_Mcast::unsubscribe (void)
{
  ACE_TRACE ("ACE_SOCK_Dgram_Mcast::unsubscribe");
  return this->ACE_SOCK::set_option (IPPROTO_IP, 
				     IP_DROP_MEMBERSHIP,
				     &multicast_address_, 
				     sizeof multicast_address_);
}

int
ACE_SOCK_Dgram_Mcast::make_multicast_address (const ACE_INET_Addr &mcast_addr,
					      const ASYS_TCHAR *net_if)
{
  ACE_TRACE ("ACE_SOCK_Dgram_Mcast::make_multicast_address");

  if (net_if != 0)
    {
#if !defined (ACE_WIN32)
      struct ifreq if_address;

      ACE_OS::strcpy (if_address.ifr_name, net_if);

      if (ACE_OS::ioctl (this->get_handle (), SIOCGIFADDR, &if_address) == -1)
	return -1;

      struct sockaddr_in *socket_address;
      socket_address = (sockaddr_in *) &if_address.ifr_addr;
      multicast_address_.imr_interface.s_addr = socket_address->sin_addr.s_addr;
#else
      // This port number is not necessary, just convenient
      ACE_INET_Addr interface_addr;
      if (interface_addr.set (mcast_addr.get_port_number (),
			      net_if) == -1)
	return -1;
      multicast_address_.imr_interface.s_addr = 
	htonl (interface_addr.get_ip_address ());      
#endif /* ACE_WIN32 */      
    }
  else
    multicast_address_.imr_interface.s_addr = INADDR_ANY;
  
  multicast_address_.imr_multiaddr.s_addr = htonl (mcast_addr.get_ip_address ());
  return 0;
}  
