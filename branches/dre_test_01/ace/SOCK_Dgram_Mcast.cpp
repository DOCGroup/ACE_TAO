// SOCK_Dgram_Mcast.cpp

#include "ace/SOCK_Dgram_Mcast.h"

#if defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "ace/SOCK_Dgram_Mcast.i"
#endif /* ACE_LACKS_INLINE_FUNCTIONS */

ACE_RCSID (ace,
           SOCK_Dgram_Mcast,
           "$Id$")

#include "ace/Log_Msg.h"

// This is a workaround for platforms with non-standard
// definitions of the ip_mreq structure
#if ! defined (IMR_MULTIADDR)
#define IMR_MULTIADDR imr_multiaddr
#endif /* ! defined (IMR_MULTIADDR) */

// Helper (inline) functions.
class ACE_SDM_helpers
{
public:
  // Convert ACE_INET_Addr to string, using local formatting rules.

  static void addr_to_string (const ACE_INET_Addr &ip_addr,
                              ACE_TCHAR *ret_string,  // results here.
                              size_t len,
                              int clip_portnum)       // clip port# info?
    {
      if (ip_addr.addr_to_string (ret_string, len, 1) == -1)
        {
          ACE_OS_String::strcpy (ret_string, ACE_LIB_TEXT ("<?>"));
        }
      else
        {
          ACE_TCHAR *pc;
          if (clip_portnum
              && (pc = ACE_OS_String::strchr (ret_string, ACE_LIB_TEXT (':'))))
            *pc = ACE_LIB_TEXT ('\0'); // clip port# info.
        }
    }
    // op== for ip_mreq structs.
    static int is_equal (const ip_mreq &m1, const ip_mreq &m2)
      {
        return m1.IMR_MULTIADDR.s_addr == m2.IMR_MULTIADDR.s_addr
          && m1.imr_interface.s_addr == m2.imr_interface.s_addr;
      }
};

ACE_ALLOC_HOOK_DEFINE (ACE_SOCK_Dgram_Mcast)

void
ACE_SOCK_Dgram_Mcast::dump (void) const
{
  ACE_TRACE ("ACE_SOCK_Dgram_Mcast::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));

# if defined (ACE_SOCK_DGRAM_MCAST_DUMPABLE)
  ACE_TCHAR addr_string[MAXNAMELEN + 1];

  ACE_DEBUG ((LM_DEBUG, 
              ACE_LIB_TEXT ("\nOptions: bindaddr=%s, nulliface=%s\n"),
              ACE_BIT_ENABLED (this->opts_, OPT_BINDADDR_YES) ?
                ACE_LIB_TEXT ("<Bound>") : ACE_LIB_TEXT ("<Not Bound>"),
              ACE_BIT_ENABLED (this->opts_, OPT_NULLIFACE_ALL) ? 
                ACE_LIB_TEXT ("<All Ifaces>") : ACE_LIB_TEXT ("<Default Iface>")));

  // Show default send addr, port#, and interface.
  ACE_SDM_helpers::addr_to_string (this->send_addr_, addr_string, 
                                   sizeof addr_string, 0);
  ACE_DEBUG ((LM_DEBUG, 
              ACE_LIB_TEXT ("Send addr=%s iface=%s\n"),
              addr_string,
              this->send_net_if_ ? this->send_net_if_ 
                                 : ACE_LIB_TEXT ("<default>")));

  // Show list of subscribed addresses.
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("Subscription list:\n")));

  ACE_MT (ACE_GUARD (ACE_SDM_LOCK, guard, this->subscription_list_lock_));
  subscription_list_iter_t  iter (this->subscription_list_);
  for ( ; !iter.done (); iter.advance ())
    {
      ACE_TCHAR iface_string[MAXNAMELEN + 1];
      ip_mreq *pm = iter.next ();

      // Get subscribed address (w/out port# info - not relevant).
      ACE_INET_Addr ip_addr (ACE_static_cast (u_short, 0),
                             ACE_NTOHL (pm->IMR_MULTIADDR.s_addr));
      ACE_SDM_helpers::addr_to_string (ip_addr, addr_string, 
                                       sizeof addr_string, 1);

      // Get interface address/specification.
      ACE_INET_Addr if_addr (ACE_static_cast (u_short, 0),
                             ACE_NTOHL (pm->imr_interface.s_addr));
      ACE_SDM_helpers::addr_to_string (if_addr, iface_string, 
                                       sizeof iface_string, 1);
      if (ACE_OS_String::strcmp (iface_string, ACE_LIB_TEXT ("0.0.0.0")) == 0)
        // Receives on system default iface. (Note that null_iface_opt_
        // option processing has already occurred.)
        ACE_OS_String::strcpy (iface_string, ACE_LIB_TEXT ("<default>"));

      // Dump info.
      ACE_DEBUG ((LM_DEBUG, 
                  ACE_LIB_TEXT ("\taddr=%s iface=%s\n"),
                  addr_string, 
                  iface_string));
    }
# endif /* ACE_SOCK_DGRAM_MCAST_DUMPABLE */
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

// Constructor.
ACE_SOCK_Dgram_Mcast::ACE_SOCK_Dgram_Mcast
  (ACE_SOCK_Dgram_Mcast::options opts)
  :  opts_ (opts),
     send_net_if_ (0)
{
  ACE_TRACE ("ACE_SOCK_Dgram_Mcast::ACE_SOCK_Dgram_Mcast");
}

// Destructor.
ACE_SOCK_Dgram_Mcast::~ACE_SOCK_Dgram_Mcast (void)
{
  ACE_TRACE ("ACE_SOCK_Dgram_Mcast::~ACE_SOCK_Dgram_Mcast");

  // Free memory and optionally unsubscribe from currently subscribed group(s).
  delete [] this->send_net_if_;
  this->clear_subs_list ();
}

int
ACE_SOCK_Dgram_Mcast::open (const ACE_INET_Addr &mcast_addr,
                            const ACE_TCHAR *net_if,
                            int reuse_addr)
{
  ACE_TRACE ("ACE_SOCK_Dgram_Mcast::open");

  // Only perform the <open> initialization if we haven't been opened
  // earlier.
  // No sanity check?  We should probably flag an error if the user 
  // makes multiple calls to open().
  if (this->get_handle () != ACE_INVALID_HANDLE)
    return 0;

  // Invoke lower-layer ::open.
  if (ACE_SOCK::open (SOCK_DGRAM,
                      mcast_addr.get_type (),
                      0, // always use 0
                      reuse_addr) == -1)
    return -1;

  return open_i (mcast_addr, net_if, reuse_addr);
}

int
ACE_SOCK_Dgram_Mcast::open_i (const ACE_INET_Addr &mcast_addr,
                              const ACE_TCHAR *net_if,
                              int reuse_addr)
{
  ACE_TRACE ("ACE_SOCK_Dgram_Mcast::open_i");
  // ACE_SOCK::open calls this if reuse_addr is set, so we only need to
  // process port reuse option.
  if (reuse_addr)
    {
#if defined (SO_REUSEPORT)
      int one = 1;
      if (this->ACE_SOCK::set_option (SOL_SOCKET,
                                      SO_REUSEPORT,
                                      &one,
                                      sizeof one) == -1)
        return -1;
#endif /* SO_REUSEPORT */
    }

  // Create an address/port# to bind the socket to. Use mcast_addr to
  // initialize bind_addy to pick up the correct protocol family. If
  // OPT_BINDADDR_YES is set, then we're done. Else use mcast_addr's
  // port number and use the "any" address.
  ACE_INET_Addr bind_addy (mcast_addr);
  if (ACE_BIT_DISABLED (this->opts_, OPT_BINDADDR_YES))
    {
      // Bind to "any" address and explicit port#.
      if (bind_addy.set (mcast_addr.get_port_number ()) == -1)
        return -1;
    }

  // Bind to the address (which may be INADDR_ANY) and port# (which may be 0)
  if (ACE_SOCK_Dgram::shared_open (bind_addy, bind_addy.get_type ()) == -1)
    return -1;

  // Cache the actual bound address (which may be INADDR_ANY)
  // and the actual bound port# (which will be a valid, non-zero port#).
  ACE_INET_Addr bound_addy;
  if (this->get_local_addr (bound_addy) == -1)
    {
      // (Unexpected failure - should be bound to something)
      if (bound_addy.set (bind_addy) == -1)
        {
          // (Shouldn't happen - bind_addy is a valid addy; punt.)
          return -1;
        }
    }

  this->send_addr_ = mcast_addr;
  this->send_addr_.set_port_number (bound_addy.get_port_number ());
  if (net_if)
    {
#if defined (IP_MULTICAST_IF) && (IP_MULTICAST_IF != 0)
      ip_mreq  send_mreq;
      if (this->make_multicast_ifaddr (&send_mreq,
                                       mcast_addr,
                                       net_if) == -1)
        return -1;
      if (this->ACE_SOCK::set_option (IPPROTO_IP,
                                      IP_MULTICAST_IF,
                                      &(send_mreq.imr_interface),
                                      sizeof send_mreq.imr_interface) == -1)
        return -1;
      this->send_net_if_ = new ACE_TCHAR[ACE_OS_String::strlen (net_if) + 1];
      ACE_OS_String::strcpy (this->send_net_if_, net_if);
#else
      // Send interface option not supported - ignore it.
      // (We may have been invoked by ::subscribe, so we have to allow
      // a non-null interface parameter in this function.)
      ACE_DEBUG ((LM_DEBUG, 
                  ACE_LIB_TEXT ("Send interface specification not ")
                  ACE_LIB_TEXT ("supported - IGNORED.\n")));
#endif // IP_MULTICAST_IF
    }

  return 0;
}

int
ACE_SOCK_Dgram_Mcast::subscribe_ifs (const ACE_INET_Addr &mcast_addr,
                                     const ACE_TCHAR *net_if,
                                     int reuse_addr)
{
  ACE_TRACE ("ACE_SOCK_Dgram_Mcast::subscribe_ifs");

  if (ACE_BIT_ENABLED (this->opts_, OPT_NULLIFACE_ALL)
      && net_if == 0)
    {
      // Subscribe on all local multicast-capable network interfaces, by
      // doing recursive calls with specific interfaces.

      ACE_INET_Addr *if_addrs = 0;
      size_t if_cnt;

      if (ACE_Sock_Connect::get_ip_interfaces (if_cnt, if_addrs) != 0)
        return -1;

      size_t nr_subscribed = 0;

      if (if_cnt < 2)
        {
          if (this->subscribe (mcast_addr,
                               reuse_addr,
                               ACE_LIB_TEXT ("0.0.0.0")) == 0)
            ++nr_subscribed;
        }
      else
        {
          // Iterate through all the interfaces, figure out which ones
          // offer multicast service, and subscribe to them.
          while (if_cnt > 0)
            {
              --if_cnt;

              // Convert to 0-based for indexing, next loop check.
              if (if_addrs[if_cnt].get_ip_address () == INADDR_LOOPBACK)
                continue;
              if (this->subscribe (mcast_addr,
                                   reuse_addr,
                                   ACE_TEXT_CHAR_TO_TCHAR
                                     (if_addrs[if_cnt].get_host_addr ())) == 0)
                ++nr_subscribed;
            }
        }

      delete [] if_addrs;

      if (nr_subscribed == 0)
        {
          errno = ENODEV;
          return -1;
        }

      // 1 indicates a "short-circuit" return.  This handles the
      // recursive behavior of checking all the interfaces.
      return 1;
    }

  // Validate passed multicast addr and iface specifications.
  if (this->make_multicast_ifaddr (0,
                                   mcast_addr,
                                   net_if) == -1)
    return -1;

  return 0;
}

// Subscribe and add address/iface to subscription list if successful.
int
ACE_SOCK_Dgram_Mcast::subscribe (const ACE_INET_Addr &mcast_addr,
                                 int reuse_addr,
                                 const ACE_TCHAR *net_if,
                                 int protocol_family,
                                 int protocol)
{
  ACE_TRACE ("ACE_SOCK_Dgram_Mcast::subscribe");

  ACE_UNUSED_ARG (protocol_family);
  ACE_UNUSED_ARG (protocol);

  return this->join (mcast_addr,reuse_addr, net_if);
}

int
ACE_SOCK_Dgram_Mcast::join (const ACE_INET_Addr &mcast_addr,
                            int reuse_addr,
                            const ACE_TCHAR *net_if)
{
  ACE_TRACE ("ACE_SOCK_Dgram_Mcast::join");
  ACE_INET_Addr subscribe_addr = mcast_addr;

  // If port# is 0, insert bound port# if it is set. (To satisfy lower-level
  // port# validation.)
  u_short def_port_number = this->send_addr_.get_port_number ();
  if (subscribe_addr.get_port_number () == 0 
      && def_port_number != 0)
    {
      subscribe_addr.set_port_number (def_port_number);
    }

  // Check for port# different than bound port#.
  u_short sub_port_number = mcast_addr.get_port_number ();
  if (sub_port_number != 0
      && def_port_number != 0
      && sub_port_number != def_port_number)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_LIB_TEXT ("Subscribed port# (%u) different than bound ")
                  ACE_LIB_TEXT ("port# (%u).\n"),
                  (u_int) sub_port_number,
                  (u_int) def_port_number));
      errno = ENXIO;
      return -1;
    }

  // If bind_addr_opt_ is enabled, check for address different than
  // bound address.
  if (ACE_BIT_ENABLED (this->opts_, OPT_BINDADDR_YES)
      && this->send_addr_.get_ip_address () != INADDR_ANY
      && this->send_addr_.get_ip_address () != mcast_addr.get_ip_address ())
    {
      ACE_TCHAR sub_addr_string[MAXNAMELEN + 1];
      ACE_TCHAR bound_addr_string[MAXNAMELEN + 1];
      ACE_SDM_helpers::addr_to_string (mcast_addr, sub_addr_string, 
                                       sizeof sub_addr_string, 1);
      ACE_SDM_helpers::addr_to_string (this->send_addr_, bound_addr_string, 
                                       sizeof bound_addr_string, 1);
      ACE_ERROR ((LM_ERROR,
                  ACE_LIB_TEXT ("Subscribed address (%s) different than ")
                  ACE_LIB_TEXT ("bound address (%s).\n"),
                  sub_addr_string,
                  bound_addr_string));
      errno = ENXIO;
      return -1;
    }

  // Attempt subscription.
  int  result = this->subscribe_i (subscribe_addr, reuse_addr, net_if);

#if defined (ACE_SOCK_DGRAM_MCAST_DUMPABLE)
  if (result == 0)
    {
      // Add this addr/iface info to the list of subscriptions.
      // (Assumes this is unique addr/iface combo - most systems don't allow
      // re-sub to same addr/iface.)
      ip_mreq  *pmreq = new ip_mreq;
      // (should not fail)
      if (this->make_multicast_ifaddr (pmreq, subscribe_addr, net_if) != -1)
        {
          ACE_MT (ACE_GUARD_RETURN (ACE_SDM_LOCK, guard,
				    this->subscription_list_lock_, -1));
          this->subscription_list_.insert_tail (pmreq);
          return 0;
        }
      // this still isn't really right. If ACE_GUARD_RETURN fails, we leak.
      // Need to add one of Chris' fancy ace auto pointers (bound?).     
      delete pmreq;
    }
#endif /* ACE_SOCK_DGRAM_MCAST_DUMPABLE */

  return result >= 0 ? 0 : result;
}

// Attempt subscribe and return status.
int
ACE_SOCK_Dgram_Mcast::subscribe_i (const ACE_INET_Addr &mcast_addr,
                                   int reuse_addr,
                                   const ACE_TCHAR *net_if)
{
  ACE_TRACE ("ACE_SOCK_Dgram_Mcast::subscribe_i");
  ip_mreq  mreq;

  // Open the socket IFF this is the first ::subscribe and ::open
  // was not explicitly invoked.
  if (this->open (mcast_addr,
                  net_if,
                  reuse_addr) == -1)
    return -1;

  // Only do this if net_if == 0, i.e., INADDR_ANY
  if (net_if == 0)
    {
      int result = this->subscribe_ifs (mcast_addr,
                                        net_if,
                                        reuse_addr);
      // Check for error or "short-circuit" return.
      if (result != 0)
        return result;
    }

  // Create multicast addr/if struct.
  if (this->make_multicast_ifaddr (&mreq, mcast_addr, net_if) == -1)
    return -1;

  // Tell IP stack to pass messages sent to this group.
  // Note, this is not IPv6 compliant.
  else if (this->ACE_SOCK::set_option (IPPROTO_IP,
                                       IP_ADD_MEMBERSHIP,
                                       &mreq,
                                       sizeof mreq) == -1)
    return -1;

  return 0;
}

int
ACE_SOCK_Dgram_Mcast::unsubscribe_ifs (const ACE_INET_Addr &mcast_addr,
                                       const ACE_TCHAR *net_if)
{
  ACE_TRACE ("ACE_SOCK_Dgram_Mcast::unsubscribe_ifs");


  if (ACE_BIT_ENABLED (this->opts_, OPT_NULLIFACE_ALL)
      && net_if == 0)
    {
      // Unsubscribe on all local multicast-capable network interfaces, by
      // doing recursive calls with specific interfaces.

      ACE_INET_Addr *if_addrs = 0;
      size_t if_cnt;

      // NOTE - <get_ip_interfaces> doesn't always get all of the
      // interfaces.  In particular, it may not get a PPP interface.  This
      // is a limitation of the way <get_ip_interfaces> works with
      // old versions of MSVC.  The reliable way of getting the interface list
      // is available only with MSVC 5 and newer.
      if (ACE_Sock_Connect::get_ip_interfaces (if_cnt, if_addrs) != 0)
        return -1;

      size_t nr_unsubscribed = 0;

      if (if_cnt < 2)
        {
          if (this->leave (mcast_addr,
                           ACE_LIB_TEXT ("0.0.0.0")) == 0)
            ++nr_unsubscribed;
        }
      else
        {
          while (if_cnt > 0)
            {
              --if_cnt;
              // Convert to 0-based for indexing, next loop check
              if (if_addrs[if_cnt].get_ip_address () == INADDR_LOOPBACK)
                continue;
              if (this->leave (mcast_addr,
                               ACE_TEXT_CHAR_TO_TCHAR
                               (if_addrs[if_cnt].get_host_addr ())) == 0)
                ++nr_unsubscribed;
            }
        }

      delete [] if_addrs;

      if (nr_unsubscribed == 0)
        {
          errno = ENODEV;
          return -1;
        }

      return 1;
    }

  return 0;
}


// Unsubscribe, and remove address from subscription list.
// Note: If there are duplicate entries, only finds the first in the list (this
// is a defined restriction - most environments don't allow duplicates to be
// created.)
int
ACE_SOCK_Dgram_Mcast::unsubscribe (const ACE_INET_Addr &mcast_addr,
                                   const ACE_TCHAR *net_if,
                                   int protocol_family,
                                   int protocol)
{
  ACE_TRACE ("ACE_SOCK_Dgram_Mcast::unsubscribe");

  ACE_UNUSED_ARG (protocol_family);
  ACE_UNUSED_ARG (protocol);

  return this->leave (mcast_addr, net_if);
}

int
ACE_SOCK_Dgram_Mcast::leave (const ACE_INET_Addr &mcast_addr,
                             const ACE_TCHAR *net_if)
{
  ACE_TRACE ("ACE_SOCK_Dgram_Mcast::leave");
  // Unsubscribe.
  int result = this->unsubscribe_i (mcast_addr,
                                    net_if);

#if defined (ACE_SOCK_DGRAM_MCAST_DUMPABLE)
  // (Unconditionally) Remove this addr/if from subscription list.
  // (Addr/if is removed even if unsubscribe failed)
  ip_mreq  tgt_mreq;
  if (this->make_multicast_ifaddr (&tgt_mreq,
                                   mcast_addr,
                                   net_if) != -1)
    {
      ACE_MT (ACE_GUARD_RETURN (ACE_SDM_LOCK, guard,
				this->subscription_list_lock_, -1));
      subscription_list_iter_t iter (this->subscription_list_);
      for (; !iter.done (); iter.advance ())
        {
          ip_mreq  *pm = iter.next ();
          if (ACE_SDM_helpers::is_equal (*pm, tgt_mreq))
            {
              iter.remove ();
              delete pm;
              break;
            }
        }
    }
#endif /* ACE_SOCK_DGRAM_MCAST_DUMPABLE */

  return result >= 0 ? 0 : result;
}

// Attempt unsubscribe and return status.
int
ACE_SOCK_Dgram_Mcast::unsubscribe_i (const ACE_INET_Addr &mcast_addr,
                                     const ACE_TCHAR *net_if)
{
  ACE_TRACE ("ACE_SOCK_Dgram_Mcast::unsubscribe_i");

  int result = this->unsubscribe_ifs (mcast_addr,
                                      net_if);

  // Check for error or "short-circuit" return.
  if (result != 0)
    return result;

  // Validate addr/if specifications and create addr/if struct.
  ip_mreq  mreq;
  if (this->make_multicast_ifaddr (&mreq, mcast_addr, net_if) == -1)
    {
      return -1;
    }
  // Tell network device driver to stop reading datagrams with the
  // <mcast_addr>.  
  // Note, this is not IPv6 friendly...
  else if (ACE_SOCK::set_option (IPPROTO_IP,
                                 IP_DROP_MEMBERSHIP,
                                 &mreq,
                                 sizeof mreq) == -1)
    {
      return -1;
    }

  return 0;
}

int
ACE_SOCK_Dgram_Mcast::unsubscribe (void)
{
  ACE_TRACE ("ACE_SOCK_Dgram_Mcast::unsubscribe");

  // Can't implement this reliably without keeping an expensive list,
  // and can't close the socket since the caller may want to continue
  // using the socket to send() or join() new groups.  Even if we
  // wanted to be clever and reopen the socket, we'd need to know what
  // options had been set, and reset them--and we have no way of doing
  // that either. :-(
  // Should this return -1?
  ACE_ERROR_RETURN ((LM_INFO,
                     ACE_LIB_TEXT ("ACE_SOCK_Dgram_Mcast::unsubscribe (void) ")
                     ACE_LIB_TEXT ("has been deprecated. You must either ")
                     ACE_LIB_TEXT ("close to socket to unsubscribe to all ")
                     ACE_LIB_TEXT ("or unsubscribe to each individually.\n")),
                     0);
}

int
ACE_SOCK_Dgram_Mcast::clear_subs_list (void)
{
  ACE_TRACE ("ACE_SOCK_Dgram_Mcast::clear_subs_list");
  int  result = 0;

#if defined (ACE_SOCK_DGRAM_MCAST_DUMPABLE)
  ACE_MT (ACE_GUARD_RETURN (ACE_SDM_LOCK, guard,
                            this->subscription_list_lock_, -1));
  subscription_list_iter_t iter (this->subscription_list_);
  for (; !iter.done (); /*Hack: Do _not_ ::advance after remove*/)
    {
      ip_mreq  *pm = iter.next ();
      iter.remove ();
      delete pm;
    }
#endif /* ACE_SOCK_DGRAM_MCAST_DUMPABLE */
  return result;
}

int
ACE_SOCK_Dgram_Mcast::make_multicast_ifaddr (ip_mreq *ret_mreq,
                                             const ACE_INET_Addr &mcast_addr,
                                             const ACE_TCHAR *net_if)
{
  ACE_TRACE ("ACE_SOCK_Dgram_Mcast::make_multicast_ifaddr");
  ip_mreq  lmreq;       // Scratch copy.
  if (net_if != 0)
    {
#if defined (ACE_WIN32)
      // This port number is not necessary, just convenient
      ACE_INET_Addr interface_addr;
      if (interface_addr.set (mcast_addr.get_port_number (), net_if) == -1)
        return -1;
      lmreq.imr_interface.s_addr =
        ACE_HTONL (interface_addr.get_ip_address ());
#else
      ifreq if_address;

#if defined (ACE_PSOS)
      // Look up the interface by number, not name.
      if_address.ifr_ifno = ACE_OS::atoi (net_if);
#else
      ACE_OS_String::strcpy (if_address.ifr_name, net_if);
#endif /* defined (ACE_PSOS) */

      if (ACE_OS::ioctl (this->get_handle (),
                         SIOCGIFADDR,
                         &if_address) == -1)
        return -1;

      sockaddr_in *socket_address;
      socket_address = ACE_reinterpret_cast (sockaddr_in*,
                                             &if_address.ifr_addr);
      lmreq.imr_interface.s_addr = socket_address->sin_addr.s_addr;
#endif /* ACE_WIN32 */
    }
  else
    lmreq.imr_interface.s_addr = INADDR_ANY;

  lmreq.IMR_MULTIADDR.s_addr = ACE_HTONL (mcast_addr.get_ip_address ());

  // Set return info, if requested.
  if (ret_mreq)
    *ret_mreq = lmreq;

  return 0;
}

