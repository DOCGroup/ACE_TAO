// This may look like C, but it's really -*- C++ -*-
// $Id$

#include "orbsvcs/HTIOP/HTIOP_Acceptor.h"
#include "orbsvcs/HTIOP/HTIOP_Profile.h"
#include "ace/HTBP/HTBP_Environment.h"
#include "ace/HTBP/HTBP_ID_Requestor.h"

#include "tao/MProfile.h"
#include "tao/ORB_Core.h"
#include "tao/Server_Strategy_Factory.h"
#include "tao/debug.h"
#include "tao/CDR.h"
#include "tao/Codeset_Manager.h"

#include "ace/Auto_Ptr.h"

#if !defined(__ACE_INLINE__)
#include "orbsvcs/HTIOP/HTIOP_Acceptor.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO::HTIOP::Acceptor::Acceptor (ACE::HTBP::Environment *ht_env,
                                int is_inside)
  : TAO_Acceptor (OCI_TAG_HTIOP_PROFILE),
    addrs_ (0),
    hosts_ (0),
    endpoint_count_ (0),
    hostname_in_ior_ (0),
    version_ (TAO_DEF_GIOP_MAJOR, TAO_DEF_GIOP_MINOR),
    orb_core_ (0),
    base_acceptor_ (this),
    creation_strategy_ (0),
    concurrency_strategy_ (0),
    accept_strategy_ (0),
    ht_env_ (ht_env),
    inside_ (is_inside)
{
}

TAO::HTIOP::Acceptor::~Acceptor (void)
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
TAO::HTIOP::Acceptor::create_profile (const TAO::ObjectKey &object_key,
                                    TAO_MProfile &mprofile,
                                    CORBA::Short priority)
{
  // Sanity check.
  if (this->endpoint_count_ == 0)
    return -1;

  // Check if multiple endpoints should be put in one profile or
  // if they should be spread across multiple profiles.

  if (priority == TAO_INVALID_PRIORITY
      /* && this->orb_core_->orb_params()->shared_profile () == 0 */)

    return this->create_new_profile (object_key,
                                     mprofile,
                                     priority);
  else
    return this->create_shared_profile (object_key,
                                        mprofile,
                                        priority);
}

int
TAO::HTIOP::Acceptor::create_new_profile (const TAO::ObjectKey &object_key,
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
      TAO::HTIOP::Profile *pfile = 0;
      ACE_NEW_RETURN (pfile,
                      TAO::HTIOP::Profile (this->hosts_[i],
                                         this->addrs_[i].get_port_number (),
                                         this->addrs_[i].get_htid(),
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
      // by the user not to do so, or if an TAO_HTIOP 1.0 endpoint is being
      // created (TAO_HTIOP 1.0 did not support tagged components).
      if (this->orb_core_->orb_params ()->std_profile_components () == 0
          || (this->version_.major == 1 && this->version_.minor == 0))
        continue;

      pfile->tagged_components ().set_orb_type (TAO_ORB_TYPE);

      TAO_Codeset_Manager *csm = this->orb_core_->codeset_manager();
      if (csm)
        csm->set_codeset(pfile->tagged_components());
    }

  return 0;
}

int
TAO::HTIOP::Acceptor::create_shared_profile (const TAO::ObjectKey &object_key,
                                           TAO_MProfile &mprofile,
                                           CORBA::Short priority)
{
  CORBA::ULong index = 0;
  TAO_Profile *pfile = 0;
  TAO::HTIOP::Profile *htiop_profile = 0;

  // First see if <mprofile> already contains a TAO_HTIOP profile.
  for (TAO_PHandle i = 0; i != mprofile.profile_count (); ++i)
    {
      pfile = mprofile.get_profile (i);
      if (pfile->tag () == OCI_TAG_HTIOP_PROFILE)
        {
          htiop_profile = dynamic_cast<TAO::HTIOP::Profile *> (pfile);
          break;
        }
    }

  // If <mprofile> doesn't contain a TAO::HTIOP::Profile, we need to create
  // one.
  if (htiop_profile == 0)
    {
      ACE_NEW_RETURN (htiop_profile,
                      TAO::HTIOP::Profile (this->hosts_[0],
                                         this->addrs_[0].get_port_number (),
                                         this->addrs_[0].get_htid(),
                                         object_key,
                                         this->addrs_[0],
                                         this->version_,
                                         this->orb_core_),
                      -1);
      htiop_profile->endpoint ()->priority (priority);

      if (mprofile.give_profile (htiop_profile) == -1)
        {
          htiop_profile->_decr_refcnt ();
          htiop_profile = 0;
          return -1;
        }

      // Do not add any tagged components to the profile if configured
      // by the user not to do so, or if an HTIOP 1.0 endpoint is being
      // created (HTIOP 1.0 did not support tagged components).
      if (this->orb_core_->orb_params ()->std_profile_components () != 0
          && (this->version_.major >= 1 && this->version_.minor >= 1))
        {
          htiop_profile->tagged_components ().set_orb_type (TAO_ORB_TYPE);
          TAO_Codeset_Manager *csm = this->orb_core_->codeset_manager();
          if (csm)
            csm->set_codeset(htiop_profile->tagged_components());
        }

      index = 1;
    }

  // Add any remaining acceptor endpoints to the TAO::HTIOP::Profile.
  for (;
       index < this->endpoint_count_;
       ++index)
    {
      TAO::HTIOP::Endpoint *endpoint = 0;
      ACE_NEW_RETURN (endpoint,
                      TAO::HTIOP::Endpoint (this->hosts_[index],
                                          this->addrs_[index].get_port_number (),
                                          this->addrs_[index].get_htid(),
                                          this->addrs_[index]),
                      -1);
      endpoint->priority (priority);
      htiop_profile->add_endpoint (endpoint);
    }

  return 0;
}

int
TAO::HTIOP::Acceptor::is_collocated (const TAO_Endpoint *endpoint)
{
  const TAO::HTIOP::Endpoint *endp =
    dynamic_cast<const TAO::HTIOP::Endpoint *> (endpoint);

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
      if (endp->port() == this->addrs_[i].get_port_number())
        {
          if (endp->port() == 0)
            {
              return (ACE_OS::strcmp (endp->htid(),
                                      this->addrs_[i].get_htid()) == 0);
            }
          else
            {
              return (ACE_OS::strcmp(endp->host(), this->hosts_[i]) == 0);
            }
        }
    }

  return 0;
}

int
TAO::HTIOP::Acceptor::close (void)
{
  return this->base_acceptor_.close ();
}

int
TAO::HTIOP::Acceptor::open (TAO_ORB_Core *orb_core,
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
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("TAO (%P|%t) ")
                         ACE_TEXT ("TAO::HTIOP::Acceptor::open - ")
                         ACE_TEXT ("hostname already set\n\n")),
                        -1);
    }

  ACE_CString proxy_host;
  unsigned proxy_port = 0;

  int rp = this->ht_env_->get_proxy_port(proxy_port);
  if (rp == 0 && proxy_port != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("TAO (%P|%t) ")
                       ACE_TEXT ("TAO::HTIOP::Acceptor::open - ")
                       ACE_TEXT ("explicit endpoint inside proxy, port %d\n"),
                       proxy_port),
                      -1);

  if (address == 0)
    return -1;

  if (major >=0 && minor >= 0)
    this->version_.set_version (static_cast<CORBA::Octet> (major),
                                static_cast<CORBA::Octet> (minor));
  // Parse options
  if (this->parse_options (options) == -1)
    return -1;

  ACE::HTBP::Addr addr;

  // @ Mostly the address is just a host:port but in case it is
  // host:port#token, remove the #token before this is processed
  // - priyanka
  address = ACE_OS::strtok (const_cast<char *> (address), "#");

  // In the common scenario, Acceptor is in the server that is
  // outside the firewall. It means the address that is sent to
  // us can be a ACE::HTBP::Outside_Addr which is of the form
  // host:port:token. The last :token has to be taken into
  // consideration.
  const char *port_separator_loc = ACE_OS::strchr (address, ':');

  const char *specified_hostname = 0;
  char tmp_host[MAXHOSTNAMELEN + 1];

  if (port_separator_loc == address)
    {
      // The address is a port number or port name.  No hostname was
      // specified.  The hostname for each network interface and the
      // fully qualified domain name must be obtained.

      // ie. the address can be :port:token  or just :port:

      // Check for multiple network interfaces.
      if (this->probe_interfaces (orb_core) == -1)
        return -1;

      // First convert the port into a usable form.
      if (addr.ACE_INET_Addr::set (address + sizeof (':')) != 0)
        return -1;

      // Now reset the port and set the host.
      if (addr.ACE_INET_Addr::set (addr.get_port_number (),
                                   static_cast<ACE_UINT32> (INADDR_ANY),
                                   1) != 0)
        return -1;
      else
        {
          return this->open_i (addr, reactor);
        }
    }
  else if (port_separator_loc == 0)
    {
      // The address is a hostname.  No port was specified, so assume
      // port zero (port will be chosen for us).
      if (addr.ACE_INET_Addr::set ((unsigned short) 0, address) != 0)
        return -1;

      specified_hostname = address;
    }
  else
    {
      // Host and port were specified.
      if (addr.ACE_INET_Addr::set (address) != 0)
        return -1;

      // Extract out just the host part of the address.
      size_t len = port_separator_loc - address;
      ACE_OS::memcpy (tmp_host, address, len);
      tmp_host[len] = '\0';

      specified_hostname = tmp_host;
    }

  this->endpoint_count_ = 1;  // Only one hostname to store

  ACE_NEW_RETURN (this->addrs_,
                  ACE::HTBP::Addr[this->endpoint_count_],
                  -1);

  ACE_NEW_RETURN (this->hosts_,
                  char *[this->endpoint_count_],
                  -1);

  this->hosts_[0] = 0;

  if (this->hostname_in_ior_ != 0)
    {
      if (TAO_debug_level > 2)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("Overriding address in IOR with %s\n"),
                      this->hostname_in_ior_));
        }

      if (this->hostname (orb_core,
                          addr,
                          this->hosts_[0],
                          this->hostname_in_ior_) != 0)
        return -1;
    }
  else
    {
      if (this->hostname (orb_core,
                          addr,
                          this->hosts_[0],
                          specified_hostname) != 0)
        return -1;
    }

  // Copy the addr.  The port is (re)set in
  // TAO::HTIOP::Acceptor::open_i().
  if (this->addrs_[0].ACE_INET_Addr::set (addr) != 0)
    return -1;

  return this->open_i (addr,
                       reactor);
}

int
TAO::HTIOP::Acceptor::open_default (TAO_ORB_Core *orb_core,
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
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("TAO (%P|%t) ")
                         ACE_TEXT ("TAO::HTIOP::Acceptor::open_default - ")
                         ACE_TEXT ("hostname already set\n\n")),
                        -1);
    }

  if (major >=0 && minor >= 0)
    this->version_.set_version (static_cast<CORBA::Octet> (major),
                                static_cast<CORBA::Octet> (minor));

  // Parse options
  if (this->parse_options (options) == -1)
    return -1;


  unsigned proxy_port = 0;
  if (this->inside_ == 1 ||
      (this->inside_ == -1 &&
       this->ht_env_->get_proxy_port(proxy_port) == 0 &&
       proxy_port != 0))
    {
      this->endpoint_count_ = 1;
      ACE_NEW_RETURN (this->addrs_,
                      ACE::HTBP::Addr[this->endpoint_count_],
                      -1);

      ACE_NEW_RETURN (this->hosts_,
                      char *[this->endpoint_count_],
                      -1);

      ACE_OS::memset (this->hosts_, 0,
                      sizeof (char*) * this->endpoint_count_);

      ACE::HTBP::ID_Requestor req(ht_env_);
      ACE_TCHAR *htid = req.get_HTID ();
      ACE_Auto_Array_Ptr<ACE_TCHAR> guard (htid);
      this->addrs_[0] = ACE_TEXT_ALWAYS_CHAR (htid);
      return 0;

    }

  // Check for multiple network interfaces.
  if (this->probe_interfaces (orb_core) == -1)
    return -1;

  // Now that each network interface's hostname has been cached, open
  // an endpoint on each network interface using the INADDR_ANY
  // address.
  ACE::HTBP::Addr addr;

  if (addr.ACE_INET_Addr::set (static_cast<u_short> (0),
                               static_cast<ACE_UINT32> (INADDR_ANY),
                               1) != 0)
    return -1;

  return this->open_i (addr,
                       reactor);
}

int
TAO::HTIOP::Acceptor::open_i (const ACE::HTBP::Addr& addr,
                            ACE_Reactor *reactor)
{
  ACE_NEW_RETURN (this->creation_strategy_,
                  CREATION_STRATEGY (this->orb_core_),
                  -1);

  ACE_NEW_RETURN (this->concurrency_strategy_,
                  CONCURRENCY_STRATEGY (this->orb_core_),
                  -1);

  ACE_NEW_RETURN (this->accept_strategy_,
                  ACCEPT_STRATEGY (this->orb_core_),
                  -1);

  if (this->base_acceptor_.open (addr,
                                 reactor,
                                 this->creation_strategy_,
                                 this->accept_strategy_,
                                 this->concurrency_strategy_) == -1)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%P|%t) TAO::HTIOP::Acceptor::open_i ")
                    ACE_TEXT ("- %p"),
                    ACE_TEXT ("cannot open acceptor")));
      return -1;
    }

  ACE::HTBP::Addr address;

  // We do this make sure the port number the endpoint is listening on
  // gets set in the addr.
  if (this->accept_strategy_->acceptor ().get_local_addr (address) != 0)
    {
      // @@ Should this be a catastrophic error???
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%P|%t) TAO::HTIOP::Acceptor::open_i ")
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

  (void) this->accept_strategy_->acceptor().enable (ACE_CLOEXEC);

  // This avoids having child processes acquire the listen socket thereby
  // denying the server the opportunity to restart on a well-known endpoint.
  // This does not affect the aberrent behavior on Win32 platforms.

  if (TAO_debug_level > 5)
    {
      for (CORBA::ULong i = 0; i < this->endpoint_count_; ++i)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) TAO::HTIOP::Acceptor::open_i - ")
                      ACE_TEXT ("listening on: <%C:%u>\n"),
                      this->hosts_[i],
                      this->addrs_[i].get_port_number ()));
        }
    }

  return 0;
}

int
TAO::HTIOP::Acceptor::hostname (TAO_ORB_Core *orb_core,
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
TAO::HTIOP::Acceptor::dotted_decimal_address (ACE_INET_Addr &addr,
                                            char *&host)
{
  int result = 0;
  const char *tmp = 0;

  // If the IP address in the INET_Addr is the INADDR_ANY address,
  // then force the actual IP address to be used by initializing a new
  // INET_Addr with the hostname from the original one.  If that fails
  // then something is seriously wrong with the systems networking
  // setup.
  if (addr.is_any())
    {
      ACE::HTBP::Addr new_addr;
      result = new_addr.ACE_INET_Addr::set (addr.get_port_number (),
                                            addr.get_host_name (),
                                            1,
                                            addr.get_type());
      tmp = new_addr.get_host_addr ();
    }
  else
    tmp = addr.get_host_addr ();

  if (tmp == 0 || result != 0)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("\n\nTAO (%P|%t) ")
                    ACE_TEXT ("TAO::HTIOP::Acceptor::dotted_decimal_address ")
                    ACE_TEXT ("- %p\n\n"),
                    ACE_TEXT ("cannot determine hostname")));
      return -1;
    }

  host = CORBA::string_dup (tmp);
  return 0;
}


int
TAO::HTIOP::Acceptor::probe_interfaces (TAO_ORB_Core *orb_core)
{
  // Extract the hostname for each network interface, and then cache
  // it.  The hostnames will then be used when creating a
  // TAO::HTIOP::Profile for each endpoint setup on the probed
  // network interfaces.
  size_t if_cnt = 0;

  ACE_INET_Addr *inet_addrs = 0;

  if (ACE::get_ip_interfaces (if_cnt,
                              inet_addrs) != 0
      && errno != ENOTSUP)
    {
      // In the case where errno == ENOTSUP, if_cnt and if_addrs will
      // not be modified, and will each remain equal to zero.  This
      // causes the default interface to be used.
      return -1;
    }

  if (if_cnt == 0 || inet_addrs == 0)
    {
      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_WARNING,
                      ACE_TEXT ("TAO (%P|%t) Unable to probe network ")
                      ACE_TEXT ("interfaces.  Using default.\n")));
        }

      if_cnt = 1; // Force the network interface count to be one.
      delete [] inet_addrs;
      ACE_NEW_RETURN (inet_addrs,
                      ACE_INET_Addr[if_cnt],
                      -1);
    }

  // Scan for the loopback interface since it shouldn't be included in
  // the list of cached hostnames unless it is the only interface.
  size_t lo_cnt = 0;  // Loopback interface count
  for (size_t j = 0; j < if_cnt; ++j)
    if (inet_addrs[j].get_ip_address () == INADDR_LOOPBACK)
      lo_cnt++;

  // The instantiation for this template is in
  // HTIOP/HTIOP_Connector.cpp.
  ACE_Auto_Basic_Array_Ptr<ACE_INET_Addr> safe_if_addrs (inet_addrs);

  // If the loopback interface is the only interface then include it
  // in the list of interfaces to query for a hostname, otherwise
  // exclude it from the list.
  if (if_cnt == lo_cnt)
    this->endpoint_count_ = static_cast<CORBA::ULong> (if_cnt);
  else
    this->endpoint_count_ = static_cast<CORBA::ULong> (if_cnt - lo_cnt);

  ACE_NEW_RETURN (this->addrs_,
                  ACE::HTBP::Addr[this->endpoint_count_],
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
          inet_addrs[i].get_ip_address() == INADDR_LOOPBACK)
        continue;

      if (this->hostname_in_ior_ != 0)
        {
          if (TAO_debug_level > 2)
            {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("Overriding address in IOR with %s\n"),
                          this->hostname_in_ior_));
            }
          if (this->hostname (orb_core,
                              inet_addrs[i],
                              this->hosts_[host_cnt],
                              this->hostname_in_ior_) != 0)
            return -1;
        }
      else
        {
          if (this->hostname (orb_core,
                              inet_addrs[i],
                              this->hosts_[host_cnt]) != 0)
            return -1;
        }

      // Copy the addr.  The port is (re)set in
      // HTIOP_Acceptor::open_i().
      if (this->addrs_[host_cnt].ACE_INET_Addr::set (inet_addrs[i]) != 0)
        return -1;

      host_cnt++;
    }

  return 0;
}

CORBA::ULong
TAO::HTIOP::Acceptor::endpoint_count (void)
{
  return this->endpoint_count_;
}

int
TAO::HTIOP::Acceptor::object_key (IOP::TaggedProfile &profile,
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
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) TAO::HTIOP::Acceptor::object_key")
                      ACE_TEXT (" - v%d.%d\n"),
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
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) TAO::HTIOP::Acceptor::object_key - ")
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
TAO::HTIOP::Acceptor::parse_options (const char *str)
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
      option_count++;

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
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("TAO (%P|%t) Zero length HTIOP option.\n")),
                          -1);
      else if (end != ACE_CString::npos)
        {
          ACE_CString opt = options.substring (begin, end);

          ACE_CString::size_type const slot = opt.find ("=");

          if (slot == len - 1
              || slot == ACE_CString::npos)
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("TAO (%P|%t) HTIOP option <%s> is ")
                               ACE_TEXT ("missing a value.\n"),
                               opt.c_str ()),
                              -1);

          ACE_CString name = opt.substring (0, slot);
          ACE_CString value = opt.substring (slot + 1);

          if (name.length () == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("(%P|%t) Zero length HTIOP ")
                               ACE_TEXT ("option name.\n")),
                              -1);
          else if (name == "hostname_in_ior")
            {
              this->hostname_in_ior_ = value.rep ();
            }
          else
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("(%P|%t) Invalid HTIOP option: <%s>\n"),
                               name.c_str ()),
                              -1);

          begin = end + 1;
        }
      else
        break;  // No other options.
    }
  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
