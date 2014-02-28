// $Id$

#include "tao/Strategies/SCIOP_Acceptor.h"
#include "tao/Strategies/SCIOP_Profile.h"

#if TAO_HAS_SCIOP == 1

#include "tao/MProfile.h"
#include "tao/debug.h"
#include "tao/Protocols_Hooks.h"
#include "tao/Codeset_Manager.h"
#include "tao/Transport.h"
#include "tao/CDR.h"
#include "ace/os_include/os_netdb.h"

#if !defined(__ACE_INLINE__)
#include "tao/Strategies/SCIOP_Acceptor.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_SCIOP_Acceptor::TAO_SCIOP_Acceptor (void)
  : TAO_Acceptor (TAO_TAG_SCIOP_PROFILE),
    addrs_ (0),
    port_span_ (1),
    hosts_ (0),
    endpoint_count_ (0),
    hostname_in_ior_ (0),
    version_ (TAO_DEF_SCIOP_MAJOR, TAO_DEF_SCIOP_MINOR),
    orb_core_ (0),
    base_acceptor_ (this),
    creation_strategy_ (0),
    concurrency_strategy_ (0),
    accept_strategy_ (0)
{
}

TAO_SCIOP_Acceptor::~TAO_SCIOP_Acceptor (void)
{
  // Make sure we are closed before we start destroying the
  // strategies.
  this->close ();

  delete this->creation_strategy_;
  delete this->concurrency_strategy_;
  delete this->accept_strategy_;

  delete [] this->addrs_;

  for (CORBA::ULong i = 0; i < this->endpoint_count_; ++i)
    CORBA::string_free (this->hosts_[i]);

  delete [] this->hosts_;
}

// TODO =
//    2) For V1.[1,2] there are tagged components
int
TAO_SCIOP_Acceptor::create_profile (const TAO::ObjectKey &object_key,
                                   TAO_MProfile &mprofile,
                                   CORBA::Short priority)
{
  // Sanity check.
  if (this->endpoint_count_ == 0)
    return -1;

  // Check if multiple endpoints should be put in one profile or
  // if they should be spread across multiple profiles.
  /*
  if (priority == TAO_INVALID_PRIORITY)
    return this->create_new_profile (object_key,
                                     mprofile,
                                     priority);
  else
  */
    return this->create_shared_profile (object_key,
                                        mprofile,
                                        priority);
}

int
TAO_SCIOP_Acceptor::create_new_profile (const TAO::ObjectKey &object_key,
                                        TAO_MProfile &mprofile,
                                        CORBA::Short priority)
{
  // Adding this->endpoint_count_ to the TAO_MProfile.
  int const count = mprofile.profile_count ();
  if ((mprofile.size () - count) < this->endpoint_count_
      && mprofile.grow (count + this->endpoint_count_) == -1)
    return -1;

  // Create a profile for each acceptor endpoint.
  for (CORBA::ULong i = 0; i < this->endpoint_count_; ++i)
    {
      TAO_SCIOP_Profile *pfile = 0;
      ACE_NEW_RETURN (pfile,
                      TAO_SCIOP_Profile (this->hosts_[i],
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

      pfile->tagged_components ().set_orb_type (TAO_ORB_TYPE);

      TAO_Codeset_Manager *csm = this->orb_core_->codeset_manager();
      if (csm)
        csm->set_codeset(pfile->tagged_components());
    }

  return 0;
}

int
TAO_SCIOP_Acceptor::create_shared_profile (const TAO::ObjectKey &object_key,
                                           TAO_MProfile &mprofile,
                                           CORBA::Short priority)
{
  CORBA::ULong index = 0;
  TAO_SCIOP_Profile *sciop_profile = 0;

  // Do not check <mprofile> for the presence of an existing
  // SCIOP_Profile.  With SCIOP, there is a one-to-one relationship
  // between Acceptors and Profiles.
    {
      ACE_NEW_RETURN (sciop_profile,
                      TAO_SCIOP_Profile (this->hosts_[0],
                                        this->addrs_[0].get_port_number (),
                                        object_key,
                                        this->addrs_[0],
                                        this->version_,
                                        this->orb_core_),
                      -1);
      sciop_profile->endpoint ()->priority (priority);

      if (mprofile.give_profile (sciop_profile) == -1)
        {
          sciop_profile->_decr_refcnt ();
          sciop_profile = 0;
          return -1;
        }

      sciop_profile->tagged_components ().set_orb_type (TAO_ORB_TYPE);

      TAO_Codeset_Manager *csm = this->orb_core_->codeset_manager();
      if (csm)
        csm->set_codeset(sciop_profile->tagged_components());

      index = 1;
    }

  // Add any remaining acceptor endpoints to the SCIOP_Profile.
  for (;
       index < this->endpoint_count_;
       ++index)
    {
      TAO_SCIOP_Endpoint *endpoint = 0;
      ACE_NEW_RETURN (endpoint,
                      TAO_SCIOP_Endpoint (this->hosts_[index],
                                         this->addrs_[index].get_port_number (),
                                         this->addrs_[index]),
                      -1);
      endpoint->priority (priority);
      sciop_profile->add_endpoint (endpoint);
    }

  return 0;
}

int
TAO_SCIOP_Acceptor::is_collocated (const TAO_Endpoint *endpoint)
{
  const TAO_SCIOP_Endpoint *endp =
    dynamic_cast<const TAO_SCIOP_Endpoint *> (endpoint);

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
      if (endp->port() == this->addrs_[i].get_port_number()
          && ACE_OS::strcmp(endp->host(), this->hosts_[i]) == 0)
        return 1;
    }

  return 0;
}

int
TAO_SCIOP_Acceptor::close (void)
{
  return this->base_acceptor_.close ();
}

int
TAO_SCIOP_Acceptor::open (TAO_ORB_Core *orb_core,
                         ACE_Reactor *reactor,
                         int,
                         int,
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
                         ACE_TEXT ("SCIOP_Acceptor::open, ")
                         ACE_TEXT ("hostname already set\n\n")),
                        -1);
    }

  if (address == 0)
    return -1;

  // Parse options
  if (this->parse_options (options) == -1)
    return -1;

  ACE_Multihomed_INET_Addr addr;

  const char *port_separator_loc = ACE_OS::strchr (address, ':');
  ACE_Auto_Basic_Array_Ptr<char> tmp_host_auto;

  if (port_separator_loc == address)
    {
      // The address is a port number or port name.  No hostname was
      // specified.  The hostname for each network interface and the
      // fully qualified domain name must be obtained.

      // Check for multiple network interfaces.
      if (this->probe_interfaces (orb_core) == -1)
        return -1;

      // First convert the port into a usable form.
      ACE_INET_Addr temp_addr;
      if (temp_addr.set (address + sizeof (':')) != 0)
        return -1;

      // Now reset the port and set the host.
      if (addr.set (temp_addr.get_port_number (),
                    static_cast<ACE_UINT32> (INADDR_ANY),
                    1) != 0)
        return -1;
      else
        return this->open_i (addr,
                             reactor);
    }

  // If we've reached this point, then the address consists of one or
  // more hostnames, followed perhaps by a port.

  u_short port_number = 0;
  char *tmp_host = 0;
  size_t hostname_length = 0;

  if (port_separator_loc != 0) {

    // Port separator was found.  Check that the next character is
    // not the terminator.
    const char *port_loc = port_separator_loc;
    ++port_loc;
    if (port_loc == 0) {
      TAOLIB_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("TAO (%P|%t) ")
                         ACE_TEXT ("SCIOP_Acceptor::open - ")
                         ACE_TEXT ("no port number after the ")
                         ACE_TEXT ("colon in \"%C\"\n"),
                         address),
                        -1);
    }

    // Read the port number
    ACE_INET_Addr temp_addr;
    if (temp_addr.string_to_addr(port_loc) != 0)
      return -1;
    port_number = temp_addr.get_port_number();

    // Set the length of the hostname
    hostname_length = port_separator_loc - address;

  } else {

    // Port separator was not found.  We allow port_number to retain
    // the value of 0, which will cause the port to be chosen for us
    // in open_i.

    // Set the length of the hostname
    hostname_length = ACE_OS::strlen(address);
  }

  ACE_NEW_RETURN(tmp_host, char[hostname_length + 1], -1);
  tmp_host_auto.reset(tmp_host);
  ACE_OS::memcpy (tmp_host, address, hostname_length);
  tmp_host[hostname_length] = '\0';

  // There may be multiple hostnames.  Parse them.
  ACE_Array<ACE_CString> hostnames;
  if (parse_multiple_hostnames(tmp_host, hostnames) != 0)
    return -1;

  // Check that at least one hostname was obtained.
  if (hostnames.size() < 1) {
    TAOLIB_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("TAO (%P|%t) ")
                       ACE_TEXT ("SCIOP_Acceptor::open - ")
                       ACE_TEXT ("no hostnames in string \"%C\"\n"),
                       tmp_host),
                      -1);
  }

  // Obtain the primary ip address.
  ACE_UINT32 primary_ip_addr = 0;
  {
    // Obtain a char* for the primary hostname.
    ACE_CString & primary_hostname_obj = hostnames[0];
    ACE_Auto_Basic_Array_Ptr<char> primary_hostname_auto(primary_hostname_obj.rep());
    const char* primary_hostname = primary_hostname_auto.get();

    // Convert the primary hostname to ACE_UINT32
    ACE_INET_Addr temp_addr;
    if (temp_addr.set((u_short) 0, primary_hostname) != 0)
      return -1;

    primary_ip_addr = temp_addr.get_ip_address();
  }

  // Allocate an array of secondary ip addresses.
  ACE_UINT32 *secondary_ip_addrs = 0;
  ACE_Auto_Basic_Array_Ptr<ACE_UINT32> secondary_ip_addrs_auto;
  size_t num_secondary_ip_addrs = hostnames.size() - 1;
  if (num_secondary_ip_addrs > 0) {
    ACE_NEW_RETURN(secondary_ip_addrs,
                   ACE_UINT32[num_secondary_ip_addrs],
                   -1);
    secondary_ip_addrs_auto.reset(secondary_ip_addrs);
  }

  // Populate the array of secondary ip addresses
  size_t i = 0;
  ACE_INET_Addr temp_addr;
  while (i < num_secondary_ip_addrs) {

    // Obtain a char* for a single secondary hostname.
    ACE_CString & hostname_obj = hostnames[i + 1];
    ACE_Auto_Basic_Array_Ptr<char> hostname_auto(hostname_obj.rep());
    const char* hostname = hostname_auto.get();

    // Obtain the ip address for this secondary hostname.
    if (temp_addr.set((u_short) 0, hostname) != 0)
      return -1;

    // Put secondary ip address into the array
    secondary_ip_addrs[i++] = temp_addr.get_ip_address();
  }

  // Populate our ACE_Multihomed_INET_Addr with all the right
  // stuff.
  if (addr.set(port_number,
               primary_ip_addr,
               1,
               secondary_ip_addrs,
               num_secondary_ip_addrs) != 0)
    {
      return -1;
    }

  // Number of endpoints equals the size of the hostname array.
  this->endpoint_count_ = hostnames.size();

  ACE_NEW_RETURN (this->addrs_,
                  ACE_INET_Addr[this->endpoint_count_],
                  -1);

  ACE_NEW_RETURN (this->hosts_,
                  char *[this->endpoint_count_],
                  -1);

  // Copy the primary address to the first slot of the
  // addrs_ array.
  this->addrs_[0].set (addr);

  // Copy secondary addresses to the remaining slots of the
  // addrs_ array.
  ACE_INET_Addr *secondary_addrs = this->addrs_;
  ++secondary_addrs;
  addr.get_secondary_addresses(secondary_addrs, num_secondary_ip_addrs);

  // Set cached hostnames.
  i = 0;
  while (i < hostnames.size()) {

    // The hostname_in_ior_ field may override the FIRST hostname only.
    if (this->hostname_in_ior_ != 0 && i == 0)
    {
      if (TAO_debug_level > 2)
      {
        TAOLIB_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("Overriding address in IOR with %C\n"),
                    this->hostname_in_ior_));
      }
      if (this->hostname (orb_core,
                          this->addrs_[i],
                          this->hosts_[i],
                          this->hostname_in_ior_) != 0)
        return -1;
    }
    else
    {
      // Obtain a char* for the hostname.
      ACE_CString & hostname_obj = hostnames[i];
      ACE_Auto_Basic_Array_Ptr<char> hostname_auto(hostname_obj.rep());
      const char* hostname = hostname_auto.get();

      if (this->hostname (orb_core,
                          this->addrs_[i],
                          this->hosts_[i],
                          hostname) != 0)
        return -1;
    }

    ++i;
  }

  // Invoke open_i.
  return this->open_i (addr,
                       reactor);
}

int
TAO_SCIOP_Acceptor::open_default (TAO_ORB_Core *orb_core,
                                 ACE_Reactor *reactor,
                                 int,
                                 int,
                                 const char *options)
{
  this->orb_core_ = orb_core;

  if (this->hosts_ != 0)
    {
      // The hostname cache has already been set!
      // This is bad mojo, i.e. an internal TAO error.
      TAOLIB_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("TAO (%P|%t) ")
                         ACE_TEXT ("SCIOP_Acceptor::open_default - ")
                         ACE_TEXT ("hostname already set\n\n")),
                        -1);
    }

  // Parse options
  if (this->parse_options (options) == -1)
    return -1;

  // Check for multiple network interfaces.
  if (this->probe_interfaces (orb_core) == -1)
    return -1;

  // Now that each network interface's hostname has been cached, open
  // an endpoint on each network interface using the INADDR_ANY
  // address.
  ACE_Multihomed_INET_Addr addr;

  if (addr.set (static_cast<u_short> (0),
                static_cast<ACE_UINT32> (INADDR_ANY),
                1) != 0)
    return -1;

  return this->open_i (addr,
                       reactor);
}

int
TAO_SCIOP_Acceptor::open_i (const ACE_Multihomed_INET_Addr& addr,
                           ACE_Reactor *reactor)
{
  ACE_NEW_RETURN (this->creation_strategy_,
                  TAO_SCIOP_CREATION_STRATEGY (this->orb_core_),
                  -1);

  ACE_NEW_RETURN (this->concurrency_strategy_,
                  TAO_SCIOP_CONCURRENCY_STRATEGY (this->orb_core_),
                  -1);

  ACE_NEW_RETURN (this->accept_strategy_,
                  TAO_SCIOP_ACCEPT_STRATEGY (this->orb_core_),
                  -1);

  u_short requested_port = addr.get_port_number ();
  if (requested_port == 0)
    {
      // don't care, i.e., let the OS choose an ephemeral port
      if (this->base_acceptor_.open (addr,
                                     reactor,
                                     this->creation_strategy_,
                                     this->accept_strategy_,
                                     this->concurrency_strategy_) == -1)
        {
          if (TAO_debug_level > 0)
            TAOLIB_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("\n\nTAO (%P|%t) SCIOP_Acceptor::open_i ")
                        ACE_TEXT ("- %p\n\n"),
                        ACE_TEXT ("cannot open acceptor")));
          return -1;
        }
    }
  else
    {
      ACE_Multihomed_INET_Addr a(addr);

      int found_a_port = 0;
      ACE_UINT32 last_port = requested_port + this->port_span_ - 1;
      if (last_port > ACE_MAX_DEFAULT_PORT)
        {
          last_port = ACE_MAX_DEFAULT_PORT;
        }

      for (ACE_UINT32 p = requested_port; p <= last_port; p++)
        {
          if (TAO_debug_level > 5)
            TAOLIB_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("TAO (%P|%t) SCIOP_Acceptor::open_i() ")
                        ACE_TEXT ("trying to listen on port %d\n"), p));

          // Now try to actually open on that port
          a.set_port_number ((u_short)p);
          if (this->base_acceptor_.open (a,
                                         reactor,
                                         this->creation_strategy_,
                                         this->accept_strategy_,
                                         this->concurrency_strategy_) != -1)
            {
              found_a_port = 1;
              break;
            }
        }

      // Now, if we couldn't locate a port, we punt
      if (! found_a_port)
        {
          if (TAO_debug_level > 0)
            TAOLIB_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("\n\nTAO (%P|%t) SCIOP_Acceptor::open_i ")
                        ACE_TEXT ("cannot open acceptor in port range (%d,%d)")
                        ACE_TEXT ("- %p\n\n"),
                        requested_port, last_port, ACE_TEXT("")));
          return -1;
        }
    }

  ACE_INET_Addr address;

  // We do this make sure the port number the endpoint is listening on
  // gets set in the addr.
  if (this->base_acceptor_.acceptor ().get_local_addr (address) != 0)
    {
      // @@ Should this be a catastrophic error???
      if (TAO_debug_level > 0)
        TAOLIB_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("\n\nTAO (%P|%t) SCIOP_Acceptor::open_i ")
                    ACE_TEXT ("- %p\n\n"),
                    ACE_TEXT ("cannot get local addr")));
      return -1;
    }

  // Set the port for each addr.  If there is more than one network
  // interface then the endpoint created on each interface will be on
  // the same port.  This is how a wildcard socket bind() is supposed
  // to work.
  u_short port = address.get_port_number ();
  for (CORBA::ULong j = 0; j < this->endpoint_count_; ++j)
    this->addrs_[j].set_port_number (port, 1);

  (void) this->base_acceptor_.acceptor().enable (ACE_CLOEXEC);
  // This avoids having child processes acquire the listen socket thereby
  // denying the server the opportunity to restart on a well-known endpoint.
  // This does not affect the aberrent behavior on Win32 platforms.

  if (TAO_debug_level > 5)
    {
      for (CORBA::ULong i = 0; i < this->endpoint_count_; ++i)
        {
          TAOLIB_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("\nTAO (%P|%t) SCIOP_Acceptor::open_i - ")
                      ACE_TEXT ("listening on: <%C:%u>\n"),
                      this->hosts_[i],
                      this->addrs_[i].get_port_number ()));
        }
    }

  // In the event that an accept() fails, we can examine the reason.  If
  // the reason warrants it, we can try accepting again at a later time.
  // The amount of time we wait to accept again is governed by this orb
  // parameter.
  this->set_error_retry_delay (
    this->orb_core_->orb_params ()->accept_error_delay());

  return 0;
}

int
TAO_SCIOP_Acceptor::hostname (TAO_ORB_Core *orb_core,
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
      if (addr.get_host_name (tmp_host, sizeof (tmp_host)) != 0)
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
TAO_SCIOP_Acceptor::dotted_decimal_address (ACE_INET_Addr &addr,
                                           char *&host)
{
  int result = 0;
  const char *tmp = 0;

  // If the IP address in the INET_Addr is the INADDR_ANY address,
  // then force the actual IP address to be used by initializing a new
  // INET_Addr with the hostname from the original one.  If that fails
  // then something is seriously wrong with the systems networking
  // setup.
  if (addr.get_ip_address () == INADDR_ANY)
    {
      ACE_INET_Addr new_addr;
      result = new_addr.set (addr.get_port_number (),
                             addr.get_host_name ());
      tmp = new_addr.get_host_addr ();
    }
  else
    tmp = addr.get_host_addr ();

  if (tmp == 0 || result != 0)
    {
      if (TAO_debug_level > 0)
        TAOLIB_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("\n\nTAO (%P|%t) ")
                    ACE_TEXT ("SCIOP_Acceptor::dotted_decimal_address ")
                    ACE_TEXT ("- %p\n\n"),
                    ACE_TEXT ("cannot determine hostname")));
      return -1;
    }

  host = CORBA::string_dup (tmp);
  return 0;
}

int
TAO_SCIOP_Acceptor::probe_interfaces (TAO_ORB_Core *orb_core)
{
  // Extract the hostname for each network interface, and then cache
  // it.  The hostnames will then be used when creating a
  // TAO_SCIOP_Profile for each endpoint setup on the probed
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
                      ACE_TEXT ("TAO (%P|%t) Unable to probe network ")
                      ACE_TEXT ("interfaces.  Using default.\n")));
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
    if (if_addrs[j].get_ip_address () == INADDR_LOOPBACK)
      ++lo_cnt;

  // The instantiation for this template is in
  // tao/SCIOP_Connector.cpp.
  ACE_Auto_Basic_Array_Ptr<ACE_INET_Addr> safe_if_addrs (if_addrs);

  // If the loopback interface is the only interface then include it
  // in the list of interfaces to query for a hostname, otherwise
  // exclude it from the list.
  if (if_cnt == lo_cnt)
    this->endpoint_count_ = static_cast<CORBA::ULong> (if_cnt);
  else
    this->endpoint_count_ = static_cast<CORBA::ULong> (if_cnt - lo_cnt);

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
      // Ignore any loopback interface if there are other
      // non-loopback interfaces.
      if (if_cnt != lo_cnt &&
          if_addrs[i].get_ip_address() == INADDR_LOOPBACK)
        continue;

      if (this->hostname_in_ior_ != 0)
        {
          if (TAO_debug_level > 2)
            {
              TAOLIB_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("Overriding address in IOR with %C\n"),
                          this->hostname_in_ior_));
            }
          if (this->hostname (orb_core,
                              if_addrs[i],
                              this->hosts_[host_cnt],
                              this->hostname_in_ior_) != 0)
            return -1;
        }
      else
        {
          if (this->hostname (orb_core,
                              if_addrs[i],
                              this->hosts_[host_cnt]) != 0)
            return -1;
        }

      // Copy the addr.  The port is (re)set in
      // TAO_SCIOP_Acceptor::open_i().
      if (this->addrs_[host_cnt].set (if_addrs[i]) != 0)
        return -1;

      ++host_cnt;
    }

  return 0;
}

int
TAO_SCIOP_Acceptor::parse_multiple_hostnames (const char *hostnames,
                                              ACE_Array<ACE_CString> &hostnames_out)
{

  // Make a copy of hostnames string
  int const hostnames_string_length = ACE_OS::strlen(hostnames) + 1;
  char* hostnames_copy = 0;
  ACE_NEW_RETURN (hostnames_copy,
                  char[hostnames_string_length],
                  -1);
  ACE_Auto_Basic_Array_Ptr<char> hostnames_copy_auto(hostnames_copy);
  ACE_OS::strncpy(hostnames_copy, hostnames, hostnames_string_length);

  // Count the number of hostnames separated by "+"
  size_t num_hostnames = 0;
  char *last = 0;
  const char* hostname = ACE_OS::strtok_r (hostnames_copy, "+", &last);

  while (hostname != 0) {
    ++num_hostnames;
    hostname = ACE_OS::strtok_r (0, "+", &last);
  }

  // Set the size of the array to the number of hostnames
  if (hostnames_out.size(num_hostnames) == -1) {

    TAOLIB_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("TAO (%P|%t) Could not allocate storage ")
                       ACE_TEXT ("for %d hostnames in SCIOP endpoint\n"),
                       num_hostnames),
                      -1);
  }

  // Refresh copy of hostnames string
  ACE_NEW_RETURN (hostnames_copy,
                  char[hostnames_string_length],
                  -1);
  hostnames_copy_auto.reset(hostnames_copy);
  ACE_OS::strncpy(hostnames_copy, hostnames, hostnames_string_length);

  // Iterate over the hostnames and stuff them into the array
  size_t index = 0;
  last = 0;
  hostname = ACE_OS::strtok_r (hostnames_copy, "+", &last);

  while (index < num_hostnames) {
    ACE_CString hostname_object(hostname);
    hostnames_out.set(hostname_object, index++);

    hostname = ACE_OS::strtok_r (0, "+", &last);
  }

  return 0;
}


CORBA::ULong
TAO_SCIOP_Acceptor::endpoint_count (void)
{
  return this->endpoint_count_;
}

int
TAO_SCIOP_Acceptor::object_key (IOP::TaggedProfile &profile,
                               TAO::ObjectKey &object_key)
{
  // Create the decoding association from the encapsulation in the buffer,
#if (TAO_NO_COPY_OCTET_SEQUENCES == 1)
  TAO_InputCDR cdr (profile.profile_data.mb ());
#else
  TAO_InputCDR cdr (reinterpret_cast<char*> (profile.profile_data.get_buffer ()),
                    profile.profile_data.length ());
#endif /* TAO_NO_COPY_OCTET_SEQUENCES == 1 */

  CORBA::Octet major = 0;
  CORBA::Octet minor = 0;

  // Read the version. We just read it here. We don't*do any*
  // processing.
  if (!(cdr.read_octet (major)
        && cdr.read_octet (minor)))
    {
      if (TAO_debug_level > 0)
        {
          TAOLIB_ERROR ((LM_ERROR,
                      ACE_TEXT ("TAO (%P|%t) SCIOP_Profile::decode - v%d.%d\n"),
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
                      ACE_TEXT ("TAO (%P|%t) TAO_SCIOP_Acceptor::object_key - ")
                      ACE_TEXT ("error while decoding host/port")));
        }
      return -1;
    }

  // ... and object key.
  if (!(cdr >> object_key))
    return -1;

  // We are NOT bothered about the rest.

  return 1;
}


int
TAO_SCIOP_Acceptor::parse_options (const char *str)
{
  if (str == 0)
    return 0;  // No options to parse.  Not a problem.

  // Use an option format similar to the one used for CGI scripts in
  // HTTP URLs.
  // e.g.:  option1=foo&option2=bar

  ACE_CString options (str);

  size_t len = options.length ();

  const char option_delimiter = '&';

  // Count the number of options.

  CORBA::ULong option_count = 1;
  // Number of endpoints in the string  (initialized to 1).

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

  for (CORBA::ULong j = 0; j < option_count; ++j)
    {
      if (j < option_count - 1)
        end = options.find (option_delimiter, begin);
      else
        end = len;

      if (end == begin)
        TAOLIB_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("TAO (%P|%t) Zero length SCIOP option.\n")),
                          -1);
      else if (end != ACE_CString::npos)
        {
          ACE_CString opt = options.substring (begin, end - begin);

          ACE_CString::size_type slot = opt.find ("=");

          if (slot == len - 1
              || slot == ACE_CString::npos)
            TAOLIB_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("TAO (%P|%t) SCIOP option <%C> is ")
                               ACE_TEXT ("missing a value.\n"),
                               opt.c_str ()),
                              -1);

          ACE_CString name = opt.substring (0, slot);
          ACE_CString value = opt.substring (slot + 1);

          if (name.length () == 0)
            TAOLIB_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("TAO (%P|%t) Zero length SCIOP ")
                               ACE_TEXT ("option name.\n")),
                              -1);

          if (name == "priority")
            {
              TAOLIB_ERROR_RETURN ((LM_ERROR,
                                 ACE_TEXT ("TAO (%P|%t) Invalid SCIOP endpoint format: ")
                                 ACE_TEXT ("endpoint priorities no longer supported.\n")),
                                -1);
            }
          else if (name == "portspan")
            {
              int range = static_cast<int> (ACE_OS::atoi (value.c_str ()));
              // @@ What's the lower bound on the range?  zero, or one?
              if (range < 1 || range > ACE_MAX_DEFAULT_PORT)
                TAOLIB_ERROR_RETURN ((LM_ERROR,
                                   ACE_TEXT ("TAO (%P|%t) Invalid SCIOP endpoint ")
                                   ACE_TEXT ("portspan: <%C>\n")
                                   ACE_TEXT ("Valid range 1 -- %d\n"),
                                   value.c_str (), ACE_MAX_DEFAULT_PORT),
                                  -1);

              this->port_span_ = static_cast<u_short> (range);
            }
          else if (name == "hostname_in_ior")
            {
              this->hostname_in_ior_ = value.rep ();
            }
          else
            TAOLIB_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("TAO (%P|%t) Invalid SCIOP option: <%C>\n"),
                               name.c_str ()),
                              -1);

          begin = end + 1;
        }
      else
        {
          break; // No other options.
        }
    }

  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_SCIOP == 1 */
