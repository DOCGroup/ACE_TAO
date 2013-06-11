// This may look like C, but it's really -*- C++ -*-
// $Id$

#include "tao/Strategies/DIOP_Acceptor.h"

#if defined (TAO_HAS_DIOP) && (TAO_HAS_DIOP != 0)

#include "tao/Strategies/DIOP_Profile.h"
#include "tao/MProfile.h"
#include "tao/ORB_Core.h"
#include "tao/debug.h"
#include "tao/Protocols_Hooks.h"
#include "tao/Codeset_Manager.h"
#include "tao/CDR.h"

#include "ace/Auto_Ptr.h"
#include "ace/OS_NS_string.h"

#if !defined(__ACE_INLINE__)
#include "tao/Strategies/DIOP_Acceptor.inl"
#endif /* __ACE_INLINE__ */

#include "ace/os_include/os_netdb.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_DIOP_Acceptor::TAO_DIOP_Acceptor (void)
  : TAO_Acceptor (TAO_TAG_DIOP_PROFILE),
    addrs_ (0),
    hosts_ (0),
    endpoint_count_ (0),
    version_ (TAO_DEF_GIOP_MAJOR, TAO_DEF_GIOP_MINOR),
    orb_core_ (0),
#if defined (ACE_HAS_IPV6)
    default_address_ (static_cast<unsigned short> (0), ACE_IPV6_ANY, AF_INET6),
#else
    default_address_ (static_cast<unsigned short> (0), static_cast<ACE_UINT32> (INADDR_ANY)),
#endif /* ACE_HAS_IPV6 */
    connection_handler_ (0)
{
}

TAO_DIOP_Acceptor::~TAO_DIOP_Acceptor (void)
{
  // Make sure we are closed before we start destroying the
  // strategies.
  this->close ();

  delete [] this->addrs_;

  for (CORBA::ULong i = 0; i < this->endpoint_count_; ++i)
    CORBA::string_free (this->hosts_[i]);

  delete [] this->hosts_;
}

// TODO =
//    2) For V1.[1,2] there are tagged components

int
TAO_DIOP_Acceptor::create_profile (const TAO::ObjectKey & object_key,
                                   TAO_MProfile &mprofile,
                                   CORBA::Short priority)
{
  // Sanity check.
  if (this->endpoint_count_ == 0)
    return -1;

  // Check if multiple endpoints should be put in one profile or if
  // they should be spread across multiple profiles.
  if (priority == TAO_INVALID_PRIORITY &&
      this->orb_core_->orb_params ()->shared_profile () == 0)
    return this->create_new_profile (object_key, mprofile, priority);
  else
    return this->create_shared_profile (object_key, mprofile, priority);
}

int
TAO_DIOP_Acceptor::create_new_profile (const TAO::ObjectKey &object_key,
                                       TAO_MProfile &mprofile,
                                       CORBA::Short priority)
{
  // Adding this->endpoint_count_ to the TAO_MProfile.
  int count = mprofile.profile_count ();
  if ((mprofile.size () - count) < this->endpoint_count_
      && mprofile.grow (count + this->endpoint_count_) == -1)
    return -1;

  // Create a profile for each acceptor endpoint.
  for (CORBA::ULong i = 0; i < this->endpoint_count_; ++i)
    {
      // Skip if the host name
      if (i > 0
          && (this->addrs_[i].get_port_number () == this->addrs_[0].get_port_number ())
          && ACE_OS::strcmp (this->hosts_[i], this->hosts_[0]) == 0)
        continue;

      TAO_DIOP_Profile *pfile = 0;
      ACE_NEW_RETURN (pfile,
                      TAO_DIOP_Profile (this->hosts_[i],
                                        this->addrs_[i].get_port_number (),
                                        object_key,
                                        this->addrs_[i],
                                        this->version_,
                                        this->orb_core_),
                      -1);
      pfile->endpoint ()->priority (priority);

      if (mprofile.give_profile (pfile) == -1)
        {
          pfile->_decr_refcnt ();
          pfile = 0;
          return -1;
        }

      // Do not add any tagged components to the profile if configured
      // by the user not to do so, or if an IIOP 1.0 endpoint is being
      // created (IIOP 1.0 did not support tagged components).
      if (this->orb_core_->orb_params ()->std_profile_components () == 0
          || (this->version_.major == 1 && this->version_.minor == 0))
        continue;

      pfile->tagged_components ().set_orb_type (TAO_ORB_TYPE);

      TAO_Codeset_Manager *csm = this->orb_core_->codeset_manager ();
      if (csm)
        csm->set_codeset (pfile->tagged_components ());
    }

  return 0;
}

int
TAO_DIOP_Acceptor::create_shared_profile (const TAO::ObjectKey &object_key,
                                          TAO_MProfile &mprofile,
                                          CORBA::Short priority)
{
  CORBA::ULong index = 0;
  TAO_DIOP_Profile *diop_profile = 0;

  // First see if <mprofile> already contains a DIOP profile.
  for (TAO_PHandle i = 0; i != mprofile.profile_count (); ++i)
    {
      TAO_Profile *pfile = mprofile.get_profile (i);
      if (pfile->tag () == TAO_TAG_DIOP_PROFILE)
      {
        diop_profile = dynamic_cast<TAO_DIOP_Profile *> (pfile);
        break;
      }
    }

  // If <mprofile> doesn't contain a DIOP_Profile, we need to create
  // one.
  if (diop_profile == 0)
    {
      ACE_NEW_RETURN (diop_profile,
                      TAO_DIOP_Profile (this->hosts_[0],
                                        this->addrs_[0].get_port_number (),
                                        object_key,
                                        this->addrs_[0],
                                        this->version_,
                                        this->orb_core_),
                      -1);
      diop_profile->endpoint ()->priority (priority);

      if (mprofile.give_profile (diop_profile) == -1)
        {
          diop_profile->_decr_refcnt ();
          diop_profile = 0;
          return -1;
        }

      if (this->orb_core_->orb_params ()->std_profile_components () != 0
          && (this->version_.major >= 1 && this->version_.minor >= 1))
        {
          diop_profile->tagged_components ().set_orb_type (TAO_ORB_TYPE);
          TAO_Codeset_Manager *csm = this->orb_core_->codeset_manager ();
          if (csm)
            csm->set_codeset (diop_profile->tagged_components ());
        }

      index = 1;
    }

  // Add any remaining acceptor endpoints to the DIOP_Profile.
  for (;
       index < this->endpoint_count_;
       ++index)
    {
      if (index > 0 &&
          this->addrs_[index].get_port_number () == this->addrs_[0].get_port_number () &&
          ACE_OS::strcmp (this->hosts_[index], this->hosts_[0]) == 0)
        continue;

      TAO_DIOP_Endpoint *endpoint = 0;
      ACE_NEW_RETURN (endpoint,
                      TAO_DIOP_Endpoint (this->hosts_[index],
                                         this->addrs_[index].get_port_number (),
                                         this->addrs_[index]),
                      -1);
      endpoint->priority (priority);
      diop_profile->add_endpoint (endpoint);
    }

  return 0;
}

int
TAO_DIOP_Acceptor::is_collocated (const TAO_Endpoint *endpoint)
{
  const TAO_DIOP_Endpoint *endp =
    dynamic_cast<const TAO_DIOP_Endpoint *> (endpoint);

  // Make sure the dynamically cast pointer is valid.
  if (endp == 0)
    return 0;

  for (CORBA::ULong i = 0; i < this->endpoint_count_; ++i)
    {
      // compare the port and host name.  Please do *NOT* optimize
      // this code by comparing the IP address instead.  That would
      // trigger the following bug:
      //
      // http://deuce.doc.wustl.edu/bugzilla/show_bug.cgi?id=1220
      //
      if (endp->port () == this->addrs_[i].get_port_number ()
          && ACE_OS::strcmp (endp->host (), this->hosts_[i]) == 0)
        return 1;  // Collocated
    }

  return 0;  // Not collocated
}

int
TAO_DIOP_Acceptor::close (void)
{
  return 0;
}

int
TAO_DIOP_Acceptor::open (TAO_ORB_Core *orb_core,
                         ACE_Reactor *reactor,
                         int major,
                         int minor,
                         const char *address,
                         const char *options)
{
  this->orb_core_ = orb_core;

  if (this->hosts_ != 0)
    {
      // The hostname cache has already been set!
      // This is bad mojo, i.e. an internal TAO error.
      TAOLIB_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("TAO (%P|%t) - ")
                         ACE_TEXT ("DIOP_Acceptor::open, ")
                         ACE_TEXT ("hostname already set\n\n")),
                        -1);
    }

  if (address == 0)
    return -1;

  if (major >=0 && minor >= 0)
    this->version_.set_version (static_cast<CORBA::Octet> (major),
                                static_cast<CORBA::Octet> (minor));
  // Parse options
  if (this->parse_options (options) == -1)
    return -1;

  ACE_CString specified_hostname;
  ACE_INET_Addr addr;
  int def_type = AF_UNSPEC;

  if (this->parse_address (address,
                           addr,
                           specified_hostname,
                           &def_type) == -1)
    return -1;

  if (specified_hostname.length () == 0)
    {
      // The address is a port number or port name.  No hostname was
      // specified.  The hostname for each network interface and the
      // fully qualified domain name must be obtained.

      // Check for multiple network interfaces.
      if (this->probe_interfaces (orb_core, def_type) == -1)
        return -1;

      // Probe interfaces has a side effect of potentially modifying
      // the default address, since that is where the address family
      // is considered.
      addr.set (this->default_address_);

      return this->open_i (addr,
                           reactor);
    }

#if defined (ACE_HAS_IPV6)
  // Check for violation of ORBConnectIPV6Only option
  if (this->orb_core_->orb_params ()->connect_ipv6_only () &&
      (addr.get_type () != AF_INET6 ||
       addr.is_ipv4_mapped_ipv6 ()))
    {
      TAOLIB_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("TAO (%P|%t) - ")
                         ACE_TEXT ("DIOP_Acceptor::open, ")
                         ACE_TEXT ("non-IPv6 endpoints not allowed when ")
                         ACE_TEXT ("connect_ipv6_only is set\n\n")),
                        -1);
    }
#endif /* ACE_HAS_IPV6 */

  if (TAO_debug_level > 2)
    {
      TAOLIB_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) - ")
                  ACE_TEXT ("DIOP_Acceptor::open, specified host=%C:%d\n"),
                  (specified_hostname.length () == 0 ? "<null>" : specified_hostname.c_str ()),
                  addr.get_port_number ()));
    }

  this->endpoint_count_ = 1;  // Only one hostname to store

  ACE_NEW_RETURN (this->addrs_,
                  ACE_INET_Addr[this->endpoint_count_],
                  -1);

  ACE_NEW_RETURN (this->hosts_,
                  char *[this->endpoint_count_],
                  -1);

  this->hosts_[0] = 0;

  if (this->hostname (orb_core,
                      addr,
                      this->hosts_[0],
                      specified_hostname.c_str ()) != 0)
    return -1;

  // Copy the addr.  The port is (re)set in
  // TAO_DIOP_Acceptor::open_i().
  if (this->addrs_[0].set (addr) != 0)
    return -1;

  return this->open_i (addr,
                       reactor);
}

int
TAO_DIOP_Acceptor::open_default (TAO_ORB_Core *orb_core,
                                 ACE_Reactor *reactor,
                                 int major,
                                 int minor,
                                 const char *options)
{
  this->orb_core_ = orb_core;

  if (this->hosts_ != 0)
    {
      // The hostname cache has already been set!
      // This is bad mojo, i.e. an internal TAO error.
      TAOLIB_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("TAO (%P|%t) - ")
                         ACE_TEXT ("DIOP_Acceptor::open_default, ")
                         ACE_TEXT ("hostname already set\n\n")),
                        -1);
    }

  if (major >= 0 && minor >= 0)
    this->version_.set_version (static_cast<CORBA::Octet> (major),
                                static_cast<CORBA::Octet> (minor));

  // Parse options
  if (this->parse_options (options) == -1)
    return -1;

  // Check for multiple network interfaces.
  if (this->probe_interfaces (orb_core) == -1)
    return -1;

  // Now that each network interface's hostname has been cached, open
  // an endpoint on each network interface using the INADDR_ANY
  // address.
  ACE_INET_Addr addr;

  if (addr.set (this->default_address_) != 0)
    return -1;

  return this->open_i (addr, reactor);
}

int
TAO_DIOP_Acceptor::open_i (const ACE_INET_Addr& addr,
                           ACE_Reactor *reactor)
{
  ACE_NEW_RETURN (this->connection_handler_,
                  TAO_DIOP_Connection_Handler (this->orb_core_),
                  -1);

  this->connection_handler_->local_addr (addr);
  int result = this->connection_handler_->open_server ();
  if (result == -1)
    {
      delete this->connection_handler_;
      return result;
    }

  // Register only with a valid handle
  result =
    reactor->register_handler (this->connection_handler_,
                               ACE_Event_Handler::READ_MASK);
  if (result == -1)
    {
      // Close the handler (this will also delete connection_handler_).
      this->connection_handler_->close ();
      return result;
    }

  // Connection handler ownership now belongs to the Reactor.
  this->connection_handler_->remove_reference ();

  ACE_INET_Addr address;

  // We do this make sure the port number the endpoint is listening on
  // gets set in the addr.
  if (this->connection_handler_->peer ().get_local_addr (address) != 0)
    {
      if (TAO_debug_level > 0)
        TAOLIB_ERROR ((LM_ERROR,
                    ACE_TEXT ("TAO (%P|%t) DIOP_Acceptor::open_i, ")
                    ACE_TEXT ("%p"),
                    ACE_TEXT ("cannot get local addr\n\n")));
      return -1;
    }

  // Set the port for each addr.  If there is more than one network
  // interface then the endpoint created on each interface will be on
  // the same port.  This is how a wildcard socket bind() is supposed
  // to work.
  unsigned short port = address.get_port_number ();
  for (CORBA::ULong j = 0; j < this->endpoint_count_; ++j)
    this->addrs_[j].set_port_number (port, 1);

  this->default_address_.set_port_number (port);

  if (TAO_debug_level > 5)
    {
      for (CORBA::ULong i = 0; i < this->endpoint_count_; ++i)
        {
          TAOLIB_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) - DIOP_Acceptor::open_i, ")
                      ACE_TEXT ("listening on: <%C:%u>\n"),
                      this->hosts_[i],
                      this->addrs_[i].get_port_number ()));
        }
    }

  return 0;
}

int
TAO_DIOP_Acceptor::hostname (TAO_ORB_Core *orb_core,
                             ACE_INET_Addr &addr,
                             char *&host,
                             const char *specified_hostname)
{
  if (orb_core->orb_params ()->use_dotted_decimal_addresses ())
    {
      // If dotted decimal addresses are enabled,
      // just return ours.
      return this->dotted_decimal_address (addr, host);
    }
  else if (specified_hostname != 0)
    {
      // If the user specified a hostname, pass it back
      // blindly as it overrides our choice of hostname.
      host = CORBA::string_dup (specified_hostname);
    }
  else
    {
      char tmp_host[MAXHOSTNAMELEN + 1];

      // Get the hostname associated with our address
#if defined (ACE_HAS_IPV6)
      // If we have a IPv4-compatible IPv6 address don't do hostname lookup
      // because that gets us into trouble. Most likely we get the same hostname
      // returned as for the actual IPv4 address but resolving that into an IPv6
      // address at the client will fail.
      if (addr.is_ipv4_compat_ipv6 () ||
          addr.get_host_name (tmp_host, sizeof (tmp_host)) != 0)
#else /* ACE_HAS_IPV6 */
      if (addr.get_host_name (tmp_host, sizeof (tmp_host)) != 0)
#endif /* !ACE_HAS_IPV6 */
        {
          // On failure, just return the decimal address.
          return this->dotted_decimal_address (addr, host);
        }
      else
        {
          host = CORBA::string_dup (tmp_host);
        }
    }

  return 0;
}

int
TAO_DIOP_Acceptor::parse_address (const char *address,
                                  ACE_INET_Addr &addr,
                                  ACE_CString &specified_hostname,
                                  int *def_type)
{
  {
    ACE_INET_Addr tmp;
    addr.set (tmp);
    specified_hostname.clear();
  }

  const char *port_separator_loc = ACE_OS::strchr (address, ':');
  char tmp_host[MAXHOSTNAMELEN + 1];
  tmp_host[0] = '\0';
  bool host_defaulted = port_separator_loc == address;
  bool ipv6_in_host = false;
  if (def_type)
    *def_type = AF_UNSPEC;

#if defined (ACE_HAS_IPV6)
  // Check if this is a (possibly) IPv6 supporting profile containing a
  // numeric IPv6 address representation.
  if ((this->version_.major > TAO_MIN_IPV6_IIOP_MAJOR ||
       this->version_.minor >= TAO_MIN_IPV6_IIOP_MINOR) &&
      address[0] == '[')
    {
      // In this case we have to find the end of the numeric address and
      // start looking for the port separator from there.
      char const * const cp_pos = ACE_OS::strchr (address, ']');
      if (cp_pos == 0)
        {
          // No valid IPv6 address specified.
          TAOLIB_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("TAO (%P|%t) - ")
                             ACE_TEXT ("DIOP_Acceptor::open, ")
                             ACE_TEXT ("invalid IPv6 decimal address specified\n\n")),
                            -1);
        }
      else
        {
          // Extract out just the host part of the address.
          size_t const len = cp_pos - (address + 1);

          if (len >= sizeof (tmp_host))
            return -1;

          ipv6_in_host = true;
          host_defaulted = (cp_pos == address+1) ||
            (cp_pos == address+3 && address[1] == ':' && address[2] == ':');
          if (cp_pos[1] == ':')    // Look for a port
            port_separator_loc = cp_pos + 1;
          else
            port_separator_loc = 0;
          if (def_type)
            *def_type = AF_INET6;

          ACE_OS::memcpy (tmp_host, address + 1, len);
          tmp_host[len] = '\0';
        }
    }
  else
#endif /* ACE_HAS_IPV6 */
    if (!host_defaulted)
      {
        if (port_separator_loc != 0)
          {
            // Extract out just the host part of the address.
            size_t const len = port_separator_loc - address;

            if (len >= sizeof (tmp_host))
              return -1;

            ACE_OS::memcpy (tmp_host, address, len);
            tmp_host[len] = '\0';
          }
        else
          ACE_OS::strcpy (tmp_host, address);
      }

  if (!ipv6_in_host && !host_defaulted)
    {
      if (addr.set ((unsigned short) 0, tmp_host) != 0)
        return -1;
      this->default_address_.set (addr);
      host_defaulted = addr.is_any ();
      if (def_type)
        *def_type = AF_INET;
    }

  if (host_defaulted)
    {
      // First convert the port into a usable form.
      unsigned short portno = 0;
      if (port_separator_loc != 0)
        {
          portno =
            static_cast<u_short> (ACE_OS::atoi (port_separator_loc +
                                                sizeof (':')));
        }
      this->default_address_.set_port_number (portno);

      // Now reset the port and set the host.
      if (addr.set (this->default_address_) != 0)
        return -1;
    }
  else if (port_separator_loc == 0)
    {
      // The address is a hostname.  No port was specified, so assume
      // port zero (port will be chosen for us).
      specified_hostname = tmp_host[0] == '\0' ? address : tmp_host;
      if (addr.set ((unsigned short) 0,
                    specified_hostname.c_str ()) != 0)
        return -1;
    }
  else
    {
      // Host and port were specified.
      if (addr.set (address) != 0)
        return -1;
      if (tmp_host[0] == '\0')
        {
          // Extract out just the host part of the address.
          size_t const len = port_separator_loc - address;

          if (len >= sizeof (tmp_host))
            return -1;

          ACE_OS::memcpy (tmp_host, address, len);
          tmp_host[len] = '\0';
        }
      specified_hostname = tmp_host;
    }

  return 1;
}

int
TAO_DIOP_Acceptor::dotted_decimal_address (ACE_INET_Addr &addr,
                                           char *&host)
{
  int result = 0;
  const char *tmp = 0;

  // If the IP address in the INET_Addr is the IN(6)ADDR_ANY address,
  // then force the actual IP address to be used by initializing a new
  // INET_Addr with the hostname from the original one.  If that fails
  // then something is seriously wrong with the systems networking
  // setup.
  if (addr.is_any ())
    {
      ACE_INET_Addr new_addr;
#if defined (ACE_HAS_IPV6)
      result = new_addr.set (addr.get_port_number (),
                             addr.get_host_name (),
                             1, /* encode */
                             addr.get_type ());
#else /* ACE_HAS_IPV6 */
      result = new_addr.set (addr.get_port_number (),
                             addr.get_host_name ());
#endif /* !ACE_HAS_IPV6 */
      tmp = new_addr.get_host_addr ();
    }
  else
    tmp = addr.get_host_addr ();

  if (tmp == 0 || result != 0)
    {
      if (TAO_debug_level > 0)
        TAOLIB_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%P|%t) - ")
                    ACE_TEXT ("DIOP_Acceptor::dotted_decimal_address, ")
                    ACE_TEXT ("%p\n\n"),
                    ACE_TEXT ("cannot determine hostname")));
      return -1;
    }

  host = CORBA::string_dup (tmp);
  return 0;
}

int
TAO_DIOP_Acceptor::probe_interfaces (TAO_ORB_Core *orb_core, int def_type)
{
  // Extract the hostname for each network interface, and then cache
  // it.  The hostnames will then be used when creating a
  // TAO_DIOP_Profile for each endpoint setup on the probed
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
      if (TAO_debug_level > 0)
        {
          TAOLIB_DEBUG ((LM_WARNING,
                      ACE_TEXT ("TAO (%P|%t) - Unable to probe network ")
                      ACE_TEXT ("interfaces.  Using default.")));
        }

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
      lo_cnt++;

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
    else if (!orb_core->orb_params ()->use_ipv6_link_local () &&
             if_addrs[j].is_linklocal ())
      {
        ++ipv6_ll;  // count link local addrs to exclude them afterwards
      }
#endif /* ACE_HAS_IPV6 */

  // The instantiation for this template is in
  // tao/DIOP_Connector.cpp.
  ACE_Auto_Basic_Array_Ptr<ACE_INET_Addr> safe_if_addrs (if_addrs);

#if defined (ACE_HAS_IPV6)
  bool ipv4_only = def_type == AF_INET;
  bool ipv6_only = (def_type == AF_INET6) ||
    orb_core->orb_params ()->connect_ipv6_only ();
#if defined (ACE_WIN32)
  if (this->default_address_.get_type () == AF_INET)
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
    }

  // In case there are no non-local IPv6 ifs in the list only exclude
  // IPv4 loopback.
  // IPv6 loopback will be needed to successfully connect IPv6 clients
  // in a localhost environment.
  if (!ipv4_only && !ipv6_non_ll)
    lo_cnt = ipv4_lo_cnt;

  if (!ignore_lo)
    this->endpoint_count_ = static_cast<CORBA::ULong> (if_ok_cnt - ipv6_ll);
  else
    this->endpoint_count_ = static_cast<CORBA::ULong> (if_ok_cnt - ipv6_ll - lo_cnt);
#else /* ACE_HAS_IPV6 */
  // If the loopback interface is the only interface then include it
  // in the list of interfaces to query for a hostname, otherwise
  // exclude it from the list.
  bool ignore_lo;
  ignore_lo = if_cnt != lo_cnt;
  if (!ignore_lo)
    this->endpoint_count_ = static_cast<CORBA::ULong> (if_cnt);
  else
    this->endpoint_count_ = static_cast<CORBA::ULong> (if_cnt - lo_cnt);
#endif /* !ACE_HAS_IPV6 */

  if (this->endpoint_count_ == 0)
    {
      if (TAO_debug_level > 0)
        TAOLIB_DEBUG ((LM_DEBUG,
                    ACE_TEXT("(%P|%t) - DIOP_Acceptor::probe_interfaces, ")
                    ACE_TEXT("found no usable addresses, def_type = %d\n"),
                    def_type));
      return -1;
    }

  ACE_NEW_RETURN (this->addrs_,
                  ACE_INET_Addr[this->endpoint_count_],
                  -1);

  ACE_NEW_RETURN (this->hosts_,
                  char *[this->endpoint_count_],
                  -1);

  ACE_OS::memset (this->hosts_, 0, sizeof (char*) * this->endpoint_count_);

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
      if (!orb_core->orb_params ()->use_ipv6_link_local () &&
          if_addrs[i].is_linklocal ())
        continue;
#else /* ACE_HAS_IPV6 */
      // Ignore any loopback interface if there are other
      // non-loopback interfaces.
      if (ignore_lo &&
          if_addrs[i].is_loopback ())
        continue;
#endif /* !ACE_HAS_IPV6 */

      if (this->hostname (orb_core,
                          if_addrs[i],
                          this->hosts_[host_cnt]) != 0)
        return -1;

      // Copy the addr.  The port is (re)set in
      // TAO_DIOP_Acceptor::open_i().
      if (this->addrs_[host_cnt].set (if_addrs[i]) != 0)
        return -1;

      host_cnt++;
    }

  return 0;
}

CORBA::ULong
TAO_DIOP_Acceptor::endpoint_count (void)
{
  return this->endpoint_count_;
}

int
TAO_DIOP_Acceptor::object_key (IOP::TaggedProfile &profile,
                               TAO::ObjectKey &object_key)
{
  // Create the decoding stream from the encapsulation in the buffer,
#if (TAO_NO_COPY_OCTET_SEQUENCES == 1)
  TAO_InputCDR cdr (profile.profile_data.mb ());
#else
  TAO_InputCDR cdr (reinterpret_cast<char*> (profile.profile_data.get_buffer ()),
                    profile.profile_data.length ());
#endif /* TAO_NO_COPY_OCTET_SEQUENCES == 1 */

  CORBA::Octet major;
  CORBA::Octet minor = CORBA::Octet();

  // Read the version. We just read it here. We don't*do any*
  // processing.
  if (!(cdr.read_octet (major)
        && cdr.read_octet (minor)))
  {
    if (TAO_debug_level > 0)
      {
        TAOLIB_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%P|%t) - DIOP_Profile::object_key, v%d.%d\n"),
                    major,
                    minor));
      }
    return -1;
  }

  CORBA::String_var host;
  CORBA::UShort port = 0;

  // Get host and port. No processing here too..
  if (cdr.read_string (host.out ()) == 0
      || cdr.read_ushort (port) == 0)
    {
      if (TAO_debug_level > 0)
        {
          TAOLIB_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) - DIOP_Acceptor::object_key, ")
                      ACE_TEXT ("error while decoding host/port")));
        }
      return -1;
    }

  // ... and object key.
  if ((cdr >> object_key) == 0)
    return -1;

  // We are NOT bothered about the rest.

  return 1;
}

int
TAO_DIOP_Acceptor::parse_options (const char *str)
{
  if (str == 0)
    return 0;  // No options to parse.  Not a problem.

  // Use an option format similar to the one used for CGI scripts in
  // HTTP URLs.
  // e.g.:  option1=foo&option2=bar

  const ACE_CString options (str);

  const size_t len = options.length ();

  static const char option_delimiter = '&';

  // Count the number of options.
  CORBA::ULong option_count = 1;

  // Only check for endpoints after the protocol specification and
  // before the object key.
  for (size_t i = 0; i < len; ++i)
    if (options[i] == option_delimiter)
      ++option_count;

  // The idea behind the following loop is to split the options into
  // (option, name) pairs.
  // For example,
  //    `option1=foo&option2=bar'
  // will be parsed into:
  //    `option1=foo'
  //    `option2=bar'

  ACE_CString::size_type begin = 0;
  ACE_CString::size_type end = 0;

  for (CORBA::ULong j = 0; j < option_count;)
    {
      if (j < option_count - 1)
        end = options.find (option_delimiter, begin);
      else
        end = len;

      ++j;  // In this way we fight MS VS warning about unreachable code.

      if (end == begin)
        {
          TAOLIB_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("TAO (%P|%t) - Zero length DIOP option.\n")),
                            -1);
        }
      else if (end != ACE_CString::npos)
        {
          ACE_CString opt = options.substring (begin, end - begin);

          ACE_CString::size_type const slot = opt.find ("=");

          if (slot == len - 1
              || slot == ACE_CString::npos)
            TAOLIB_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("TAO (%P|%t) - DIOP option <%C> is ")
                               ACE_TEXT ("missing a value.\n"),
                               opt.c_str ()),
                              -1);

          ACE_CString name = opt.substring (0, slot);
          ACE_CString value = opt.substring (slot + 1);

          begin = end + 1;

          if (name.length () == 0)
            TAOLIB_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("TAO (%P|%t) - Zero length DIOP ")
                               ACE_TEXT ("option name.\n")),
                              -1);

          if (name == "priority")
            {
              TAOLIB_ERROR_RETURN ((LM_ERROR,
                                 ACE_TEXT ("TAO (%P|%t) - Invalid DIOP endpoint format: ")
                                 ACE_TEXT ("endpoint priorities no longer supported.\n")),
                                -1);
            }
          else
            {
              TAOLIB_ERROR_RETURN ((LM_ERROR,
                                 ACE_TEXT ("TAO (%P|%t) - Invalid DIOP option: <%C>\n"),
                                 name.c_str ()),
                                -1);
            }
        }
      else
        {
          break;  // No other options.
        }
    }
  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_DIOP && TAO_HAS_DIOP != 0 */
