#include "ace/SOCK_Dgram_Mcast.h"
#include "ace/INET_Addr.h"

#ifndef ACE_SOCK_DGRAM_MCAST_T_C
#define ACE_SOCK_DGRAM_MCAST_T_C

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

ACE_RCSID (ace,
           SOCK_Dgram_Mcast_T,
           "$Id$")

// This is a workaround for platforms with non-standard
// definitions of the ip_mreq structure
#if ! defined (IMR_MULTIADDR)
#define IMR_MULTIADDR imr_multiaddr
#endif /* ! defined (IMR_MULTIADDR) */

// Short-hand for scoping the options enumeration.
typedef ACE_SOCK_Dgram_Mcast_Ex<ACE_SDM_DEFOPT_LOCK> ACE_SDM_OPTIONS;

// Helper (inline) functions.
class ACE_SDM_helpers
{
public:
  // Convert ACE_INET_Addr to string, using local formatting rules.
  enum
    {
      ADDR_STRING_MAX = 255     // max strlen of ip or interface address string.
                                // (interface string may be a UNIX pathname)
    };

  static void addr_to_string (const ACE_INET_Addr &ip_addr,
                              ACE_TCHAR *ret_string,  // results here.
                              int clip_portnum)       // clip port# info?
    {
      // Text displayed in case of conversion error.
      static const ACE_TCHAR *cvt_err_string = ACE_LIB_TEXT("<?>");

      if (ip_addr.addr_to_string (ret_string,
                                  ADDR_STRING_MAX,
                                  1) == -1)
        {
          ACE_OS_String::strcpy (ret_string, 
				 cvt_err_string);
        }
      else
        {
          ACE_TCHAR *pc;
          if (clip_portnum
              && (pc = ACE_OS_String::strchr (ret_string, ':')))
            *pc = '\0'; // clip port# info.
        }
    }
    // op== for ip_mreq structs.
    static int is_equal (const ip_mreq &m1,
                  const ip_mreq &m2)
      {
        return m1.IMR_MULTIADDR.s_addr == m2.IMR_MULTIADDR.s_addr
          && m1.imr_interface.s_addr == m2.imr_interface.s_addr;
      }
};

ACE_ALLOC_HOOK_DEFINE(ACE_SOCK_Dgram_Mcast_Ex)

template <class ACE_SDMOPT_LOCK>
void ACE_SOCK_Dgram_Mcast_Ex<ACE_SDMOPT_LOCK>::dump (void) ACE_CONST_WHEN_MUTABLE
{
  ACE_TRACE ("ACE_SOCK_Dgram_Mcast_Ex::dump");

  ACE_TCHAR addr_string[ACE_SDM_helpers::ADDR_STRING_MAX+1];

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT( \
      "\nOptions: bindaddr=%s, nulliface=%s, dtorunsub=%s\n"),
             ((bind_addr_opt_ == ACE_SDM_OPTIONS::OPT_BINDADDR_YES) ? \
                "<Bound>" : "<Not Bound>"),
             ((null_iface_opt_ == ACE_SDM_OPTIONS::OPT_NULLIFACE_ALL) ? \
                "<All Ifaces>" : "<Default Iface>"),
             ((dtor_unsub_opt_ == ACE_SDM_OPTIONS::OPT_DTORUNSUB_YES) ? \
                "<Enabled>" : "<Disabled>")));

  // Show bound addr and port#.
  ACE_SDM_helpers::addr_to_string (this->bound_addr_, addr_string, 0);
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("Bound address=%s\n"),
             addr_string));

  // Show default send addr, port#, and interface.
  ACE_SDM_helpers::addr_to_string (this->send_addr_, addr_string, 0);
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("Send addr=%s iface=%s\n"),
             addr_string,
             (this->send_net_if_ ? this->send_net_if_ : ACE_LIB_TEXT("<default>"))));

  // Show list of subscribed addresses.
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("Subscription list:\n"), ""));
  {
    ACE_GUARD (ACE_SDMOPT_LOCK, guard, this->subscription_list_lock_);
    subscription_list_iter_t  iter (this->subscription_list_);
    for ( ; !iter.done (); iter.advance ())
      {
        ACE_TCHAR iface_string[ACE_SDM_helpers::ADDR_STRING_MAX+1];
        ip_mreq *pm = iter.next ();

        // Get subscribed address (w/out port# info - not relevant).
        ACE_INET_Addr ip_addr (ACE_static_cast (u_short, 0),
                               ntohl (pm->IMR_MULTIADDR.s_addr));
        ACE_SDM_helpers::addr_to_string (ip_addr, addr_string, 1);

        // Get interface address/specification.
        ACE_INET_Addr if_addr (ACE_static_cast (u_short, 0),
                               ntohl (pm->imr_interface.s_addr));
        ACE_SDM_helpers::addr_to_string (if_addr, iface_string, 1);
        if (ACE_OS_String::strcmp (iface_string, ACE_LIB_TEXT("0.0.0.0")) == 0)
          // Receives on system default iface. (Note that null_iface_opt_
          // option processing has already occurred.)
          ACE_OS_String::strcpy (iface_string, ACE_LIB_TEXT("<default>"));

        // Dump info.
        ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("\taddr=%s iface=%s\n"),
                   addr_string, iface_string));
      }
  }
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

// Constructor.
template <class ACE_SDMOPT_LOCK>
ACE_SOCK_Dgram_Mcast_Ex<ACE_SDMOPT_LOCK>::ACE_SOCK_Dgram_Mcast_Ex
  (ACE_TYPENAME ACE_SOCK_Dgram_Mcast_Ex<ACE_SDMOPT_LOCK>::options bind_addr_opt,
   ACE_TYPENAME ACE_SOCK_Dgram_Mcast_Ex<ACE_SDMOPT_LOCK>::options null_iface_opt,
   ACE_TYPENAME ACE_SOCK_Dgram_Mcast_Ex<ACE_SDMOPT_LOCK>::options dtor_unsub_opt)
  :  bind_addr_opt_ (bind_addr_opt),
     null_iface_opt_ (null_iface_opt),
     dtor_unsub_opt_ (dtor_unsub_opt),
     send_net_if_ (NULL)
{
  ACE_TRACE ("ACE_SOCK_Dgram_Mcast_Ex::ACE_SOCK_Dgram_Mcast_Ex");
}

// Destructor.
template <class ACE_SDMOPT_LOCK>
ACE_SOCK_Dgram_Mcast_Ex<ACE_SDMOPT_LOCK>::~ACE_SOCK_Dgram_Mcast_Ex (void)
{
  ACE_TRACE ("ACE_SOCK_Dgram_Mcast_Ex::~ACE_SOCK_Dgram_Mcast_Ex");

  // Free memory and optionally unsubscribe from currently subscribed group(s).
  delete [] send_net_if_;
  this->clear_subs_list (dtor_unsub_opt_ == ACE_SDM_OPTIONS::OPT_DTORUNSUB_YES);
}

// Overloaded (public) method.
template <class ACE_SDMOPT_LOCK>
int ACE_SOCK_Dgram_Mcast_Ex<ACE_SDMOPT_LOCK>::open (const ACE_INET_Addr &mcast_addr,
                                                    const ACE_TCHAR *net_if,
                                                    int reuse_addr)
{
  ACE_TRACE ("ACE_SOCK_Dgram_Mcast_Ex::open [public]");

  // Only perform the <open> initialization if we haven't been opened
  // earlier.
  if (this->get_handle () == ACE_INVALID_HANDLE)
    {

      // Invoke lower-layer ::open.
      if (ACE_SOCK::open (SOCK_DGRAM,
                          PF_INET,
                          0,
                          reuse_addr) == -1)
        return -1;

      // Process addr/port reuse option.
      if (reuse_addr)
        {
          int one = 1;
          if (this->ACE_SOCK::set_option (SOL_SOCKET,
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
        }

      // Create an address/port# to bind the socket to.
      ACE_INET_Addr bind_addy;
      if (bind_addr_opt_ == ACE_SDM_OPTIONS::OPT_BINDADDR_YES)
        {
          // Bind to explicit addr and port#.
          if (bind_addy.set (mcast_addr) == -1)
            return -1;
        }
      else
        {
          // Bind to "any" address and explicit port#.
          if (bind_addy.set (mcast_addr.get_port_number ()) == -1)
            return -1;
        }

      // Bind to the address (which may be INADDR_ANY) and port# (which may
      // be 0)
      if (ACE_SOCK_Dgram::shared_open (bind_addy,
                                       PF_INET) == -1)
        return -1;

      // Cache the actual bound address (which may be INADDR_ANY)
      // and the actual bound port# (which will be a valid, non-zero port#).
      ACE_INET_Addr bound_addy;
      if (this->get_local_addr (bound_addy) == -1)
        // (Unexpected failure - should be bound to something)
        if (bound_addy.set (bind_addy) == -1)
          // (Shouldn't happen - bind_addy is a valid addy; punt.)
          return -1;
      if (this->bound_addr_.set (bound_addy) == -1)
        // (Shouldn't happen - bound_addy is a valid addy; punt.)
        return -1;

      // Cache the address to use in the <send> methods (the passed mcast
      // address and the actual bound port#), and set the interface to be
      // used for sends if it was specified.
      this->send_addr_ = mcast_addr;
      this->send_addr_.set_port_number (this->bound_addr_.get_port_number ());
      if (net_if)
        {
#if defined (IP_MULTICAST_IF) && (IP_MULTICAST_IF != 0)
          ip_mreq  send_mreq;
          if (make_multicast_ifaddr (&send_mreq,
                                     mcast_addr,
                                     net_if) == -1)
            return -1;
          if (this->ACE_SOCK::set_option (IPPROTO_IP,
                                          IP_MULTICAST_IF,
                                          &(send_mreq.imr_interface),
                                          sizeof send_mreq.imr_interface) == -1)
            return -1;
          this->send_net_if_ = new ACE_TCHAR[ACE_OS_String::strlen (net_if)+1];
          ACE_OS_String::strcpy (this->send_net_if_, 
                                 net_if);
#else
          // Send interface option not supported - ignore it.
          // (We may have been invoked by ::subscribe, so we have to allow
          // a non-null interface parameter in this function.)
          ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ( \
              "Send interface specification not supported - IGNORED.\n")));
#endif // IP_MULTICAST_IF
        }
    }

  return 0;
}

// This is a hidden, non-virtual base-class method - it shouldn't be invoked
// ... but it is forwarded to the public interface, with no send interface
// specification, just in case.
template <class ACE_SDMOPT_LOCK>
int ACE_SOCK_Dgram_Mcast_Ex<ACE_SDMOPT_LOCK>::open (const ACE_Addr &mcast_addr,
                                                    int protocol_family,
                                                    int protocol,
                                                    int reuse_addr)
{
  ACE_TRACE ("ACE_SOCK_Dgram_Mcast_Ex::open [legacy]");
  // Note: Sun C++ 4.2 needs the useless const_cast.
  return ACE_SOCK_Dgram_Mcast_Ex<ACE_SDMOPT_LOCK>
                     ::open (ACE_reinterpret_cast (const ACE_INET_Addr&,
                               ACE_const_cast (ACE_Addr &, mcast_addr)),
                             ACE_static_cast (ACE_TCHAR*, NULL),
                             reuse_addr);
}

template <class ACE_SDMOPT_LOCK>
int ACE_SOCK_Dgram_Mcast_Ex<ACE_SDMOPT_LOCK>::subscribe_ifs (const ACE_INET_Addr &mcast_addr,
                                                             const ACE_TCHAR *net_if,
                                                             int protocol_family,
                                                             int protocol,
                                                             int reuse_addr)
{
  ACE_TRACE ("ACE_SOCK_Dgram_Mcast_Ex::subscribe_ifs");

  if (null_iface_opt_ == ACE_SDM_OPTIONS::OPT_NULLIFACE_ALL
      && net_if == 0)
    {
      // Subscribe on all local multicast-capable network interfaces, by
      // doing recursive calls with specific interfaces.

      ACE_INET_Addr *if_addrs = 0;
      size_t if_cnt;

      if (ACE_Sock_Connect::get_ip_interfaces (if_cnt,
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
                                     (if_addrs[if_cnt].get_host_addr ()),
                                   protocol_family,
                                   protocol) == 0)
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
  if (this->make_multicast_ifaddr (NULL,
                                   mcast_addr,
                                   net_if) == -1)
    return -1;

  // Check for port# different than bound port#.
  u_short def_port_number = this->bound_addr_.get_port_number (),
                 sub_port_number = mcast_addr.get_port_number ();
  if (sub_port_number != 0
      && def_port_number != 0
      && sub_port_number != def_port_number)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_LIB_TEXT ("Subscribed port# (%u) different than bound port# (%u).\n"),
                  (u_int) sub_port_number,
                  (u_int) def_port_number));
      errno = ENXIO;
      return -1;
    }

  // If bind_addr_opt_ is enabled, check for address different than
  // bound address.
  if (bind_addr_opt_ == ACE_SDM_OPTIONS::OPT_BINDADDR_YES
      && mcast_addr != this->bound_addr_)
    {
      ACE_TCHAR sub_addr_string[ACE_SDM_helpers::ADDR_STRING_MAX + 1],
                bound_addr_string[ACE_SDM_helpers::ADDR_STRING_MAX + 1];
      ACE_SDM_helpers::addr_to_string (mcast_addr, sub_addr_string, 1);
      ACE_SDM_helpers::addr_to_string (this->bound_addr_,
                                       bound_addr_string, 1);
      ACE_DEBUG ((LM_DEBUG,
                  ACE_LIB_TEXT ("Subscribed address (%s) different than bound address (%s).\n"),
                  sub_addr_string,
                  bound_addr_string));
      errno = ENXIO;
      return -1;
    }

  // Addressing seems to be ok.
  return 0;
}

// Subscribe and add address/iface to subscription list if successful.
template <class ACE_SDMOPT_LOCK>
int ACE_SOCK_Dgram_Mcast_Ex<ACE_SDMOPT_LOCK>::subscribe (const ACE_INET_Addr &mcast_addr,
                                                         int reuse_addr,
                                                         const ACE_TCHAR *net_if,
                                                         int protocol_family,
                                                         int protocol)
{
  ACE_TRACE ("ACE_SOCK_Dgram_Mcast_Ex::subscribe");
  ACE_INET_Addr subscribe_addr = mcast_addr;

  // If port# is 0, insert bound port# if it is set. (To satisfy lower-level
  // port# validation.)
  u_short def_port_number = this->bound_addr_.get_port_number ();
  if (subscribe_addr.get_port_number () == 0
      && def_port_number != 0)
    subscribe_addr.set_port_number (def_port_number);

  // Attempt subscription.
  int  result = subscribe_2 (subscribe_addr,
                             reuse_addr,
                             net_if,
                             protocol_family,
                             protocol);

  if (result == 0)
    {
      // Add this addr/iface info to the list of subscriptions.
      // (Assumes this is unique addr/iface combo - most systems don't allow
      // re-sub to same addr/iface.)
      ip_mreq  *pmreq = new ip_mreq;
      if (this->make_multicast_ifaddr (pmreq,
                                       subscribe_addr,
                                       net_if) != -1) // (should not fail)
        {
          ACE_GUARD_RETURN (ACE_SDMOPT_LOCK, guard,
                            this->subscription_list_lock_, -1);
          subscription_list_.insert_tail (pmreq);
        }
        else
          delete pmreq;
    }

  return result >= 0 ? 0 : result;
}

// Attempt subscribe and return status.
template <class ACE_SDMOPT_LOCK>
int ACE_SOCK_Dgram_Mcast_Ex<ACE_SDMOPT_LOCK>::subscribe_2 (const ACE_INET_Addr &mcast_addr,
                                                           int reuse_addr,
                                                           const ACE_TCHAR *net_if,
                                                           int protocol_family,
                                                           int protocol)
{
  ACE_TRACE ("ACE_SOCK_Dgram_Mcast_Ex::subscribe_2");
  ip_mreq  mreq;

  // Open the socket IFF this is the first ::subscribe and ::open
  // was not explicitly invoked.
  if (this->open (mcast_addr,
                  net_if,
                  reuse_addr) == -1)
    return -1;

  int result = this->subscribe_ifs (mcast_addr,
                                    net_if,
                                    protocol_family,
                                    protocol,
                                    reuse_addr);
  // Check for error or "short-circuit" return.
  if (result != 0)
    return result;

  // Create multicast addr/if struct.
  else if (this->make_multicast_ifaddr (&mreq,
                                        mcast_addr,
                                        net_if) == -1)
    return -1;

  // Tell IP stack to pass messages sent to this group.
  else if (this->ACE_SOCK::set_option (IPPROTO_IP,
                                       IP_ADD_MEMBERSHIP,
                                       &mreq,
                                       sizeof mreq) == -1)
    return -1;
  else
    return 0;
}

template <class ACE_SDMOPT_LOCK>
int ACE_SOCK_Dgram_Mcast_Ex<ACE_SDMOPT_LOCK>::unsubscribe_ifs (const ACE_INET_Addr &mcast_addr,
                                                               const ACE_TCHAR *net_if,
                                                               int protocol_family,
                                                               int protocol)
{
  ACE_TRACE ("ACE_SOCK_Dgram_Mcast_Ex::unsubscribe_ifs");


  if (null_iface_opt_ == ACE_SDM_OPTIONS::OPT_NULLIFACE_ALL
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
      if (ACE_Sock_Connect::get_ip_interfaces (if_cnt,
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
            if (if_addrs[if_cnt].get_ip_address () == INADDR_LOOPBACK)
              continue;
            if (this->unsubscribe (mcast_addr,
                                   ACE_TEXT_CHAR_TO_TCHAR
                                     (if_addrs[if_cnt].get_host_addr ()),
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

  return 0;
}


// Unsubscribe, and remove address from subscription list.
// Note: If there are duplicate entries, only finds the first in the list (this
// is a defined restriction - most environments don't allow duplicates to be
// created.)
template <class ACE_SDMOPT_LOCK>
int ACE_SOCK_Dgram_Mcast_Ex<ACE_SDMOPT_LOCK>::unsubscribe (const ACE_INET_Addr &mcast_addr,
                                                           const ACE_TCHAR *net_if,
                                                           int protocol_family,
                                                           int protocol)
{
  ACE_TRACE ("ACE_SOCK_Dgram_Mcast_Ex::unsubscribe");

  // Unsubscribe.
  int result = this->unsubscribe_2 (mcast_addr,
                                    net_if,
                                    protocol_family,
                                    protocol);

  // (Unconditionally) Remove this addr/if from subscription list.
  // (Addr/if is removed even if unsubscribe failed)
  ip_mreq  tgt_mreq;
  if (this->make_multicast_ifaddr (&tgt_mreq,
                                   mcast_addr,
                                   net_if) != -1)
    {
      ACE_GUARD_RETURN (ACE_SDMOPT_LOCK, guard,
                        this->subscription_list_lock_, -1);
      for (subscription_list_iter_t iter (subscription_list_);
           !iter.done ();
           iter.advance ())
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

  return result >= 0 ? 0 : result;
}

// Attempt unsubscribe and return status.
template <class ACE_SDMOPT_LOCK>
int ACE_SOCK_Dgram_Mcast_Ex<ACE_SDMOPT_LOCK>::unsubscribe_2 (const ACE_INET_Addr &mcast_addr,
                                                             const ACE_TCHAR *net_if,
                                                             int protocol_family,
                                                             int protocol)
{
  ACE_TRACE ("ACE_SOCK_Dgram_Mcast_Ex::unsubscribe_2");

  int result = this->unsubscribe_ifs (mcast_addr,
                                      net_if,
                                      protocol_family,
                                      protocol);

  // Check for error or "short-circuit" return.
  if (result != 0)
    return result;

  // Validate addr/if specifications and create addr/if struct.
  ip_mreq  mreq;
  if (this->make_multicast_ifaddr (&mreq,
                                   mcast_addr,
                                   net_if) == -1)
    return -1;

  // Tell network device driver to stop reading datagrams with the
  // <mcast_addr>.
  else if (ACE_SOCK::set_option (IPPROTO_IP,
                                 IP_DROP_MEMBERSHIP,
                                 &mreq,
                                 sizeof mreq) == -1)
    return -1;
  else
    return 0;
}

template <class ACE_SDMOPT_LOCK>
int ACE_SOCK_Dgram_Mcast_Ex<ACE_SDMOPT_LOCK>::unsubscribe (void)
{
  ACE_TRACE ("ACE_SOCK_Dgram_Mcast_Ex::unsubscribe");
  return this->clear_subs_list (1);
}

template <class ACE_SDMOPT_LOCK>
int ACE_SOCK_Dgram_Mcast_Ex<ACE_SDMOPT_LOCK>::clear_subs_list (int do_unsubscribe)
{
  ACE_TRACE ("ACE_SOCK_Dgram_Mcast_Ex::clear_subs_list");
  int  result = 0;
  {
    ACE_GUARD_RETURN (ACE_SDMOPT_LOCK, guard,
                      this->subscription_list_lock_, -1);
    for (subscription_list_iter_t iter (subscription_list_);
         !iter.done ();
         /*Hack: Do _not_ ::advance after remove*/)
      {
        ip_mreq  *pm = iter.next ();
        if (do_unsubscribe)
          {
          int  rc = this->ACE_SOCK::set_option (IPPROTO_IP,
                                                IP_DROP_MEMBERSHIP,
                                                pm,
                                                sizeof *pm);
          if (rc != 0)
            result = rc;
          }
        iter.remove ();
        delete pm;
      }
  }
  return result;
}

template <class ACE_SDMOPT_LOCK>
int ACE_SOCK_Dgram_Mcast_Ex<ACE_SDMOPT_LOCK>::make_multicast_ifaddr (ip_mreq *ret_mreq,
                                                                     const ACE_INET_Addr &mcast_addr,
                                                                     const ACE_TCHAR *net_if)
{
  ACE_TRACE ("ACE_SOCK_Dgram_Mcast_Ex::make_multicast_ifaddr");
  ip_mreq  lmreq;       // Scratch copy.
  if (net_if != 0)
    {
#if defined (ACE_WIN32)
      // This port number is not necessary, just convenient
      ACE_INET_Addr interface_addr;
      if (interface_addr.set (mcast_addr.get_port_number (),
                              net_if) == -1)
        return -1;
      lmreq.imr_interface.s_addr =
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

#endif /* ACE_SOCK_DGRAM_MCAST_T_C */
