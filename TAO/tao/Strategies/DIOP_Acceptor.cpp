// This may look like C, but it's really -*- C++ -*-
// $Id$

#include "DIOP_Acceptor.h"

#if defined (TAO_HAS_DIOP) && (TAO_HAS_DIOP != 0)

#include "DIOP_Profile.h"
#include "tao/MProfile.h"
#include "tao/ORB_Core.h"
#include "tao/debug.h"
#include "tao/Protocols_Hooks.h"
#include "tao/Codeset_Manager.h"

#include "ace/Auto_Ptr.h"

#if !defined(__ACE_INLINE__)
#include "DIOP_Acceptor.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID (tao,    
           DIOP_Acceptor, 
           "$Id$")

TAO_DIOP_Acceptor::TAO_DIOP_Acceptor (CORBA::Boolean flag)
  : TAO_Acceptor (TAO_TAG_UDP_PROFILE),
    addrs_ (0),
    hosts_ (0),
    endpoint_count_ (0),
    version_ (TAO_DEF_GIOP_MAJOR, TAO_DEF_GIOP_MINOR),
    orb_core_ (0),
    lite_flag_ (flag),
    connection_handler_ (0)
{
}

TAO_DIOP_Acceptor::~TAO_DIOP_Acceptor (void)
{
  // Make sure we are closed before we start destroying the
  // strategies.
  this->close ();

  delete [] this->addrs_;

  for (size_t i = 0; i < this->endpoint_count_; ++i)
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
  if (priority == TAO_INVALID_PRIORITY)
    return this->create_new_profile (object_key,
                                     mprofile,
                                     priority);
  else
    return this->create_shared_profile (object_key,
                                        mprofile,
                                        priority);
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
  for (size_t i = 0; i < this->endpoint_count_; ++i)
    {
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

      this->orb_core_->codeset_manager()->
        set_codeset(pfile->tagged_components());
    }

  return 0;
}

int
TAO_DIOP_Acceptor::create_shared_profile (const TAO::ObjectKey &object_key,
                                          TAO_MProfile &mprofile,
                                          CORBA::Short priority)
{
  size_t index = 0;
  TAO_Profile *pfile = 0;
  TAO_DIOP_Profile *iiop_profile = 0;

  // First see if <mprofile> already contains a DIOP profile.
  for (TAO_PHandle i = 0; i != mprofile.profile_count (); ++i)
    {
      pfile = mprofile.get_profile (i);
      if (pfile->tag () == TAO_TAG_UDP_PROFILE)
      {
        iiop_profile = ACE_dynamic_cast (TAO_DIOP_Profile *,
                                         pfile);
        break;
      }
    }

  // If <mprofile> doesn't contain a DIOP_Profile, we need to create
  // one.
  if (iiop_profile == 0)
    {
      ACE_NEW_RETURN (iiop_profile,
                      TAO_DIOP_Profile (this->hosts_[0],
                                        this->addrs_[0].get_port_number (),
                                        object_key,
                                        this->addrs_[0],
                                        this->version_,
                                        this->orb_core_),
                      -1);
      iiop_profile->endpoint ()->priority (priority);

      if (mprofile.give_profile (iiop_profile) == -1)
        {
          iiop_profile->_decr_refcnt ();
          iiop_profile = 0;
          return -1;
        }

      if (this->orb_core_->orb_params ()->std_profile_components () != 0
          && (this->version_.major >= 1 && this->version_.minor >= 1))
        {
          iiop_profile->tagged_components ().set_orb_type (TAO_ORB_TYPE);
          this->orb_core_->codeset_manager()->
            set_codeset(iiop_profile->tagged_components());
        }

      index = 1;
    }

  // Add any remaining acceptor endpoints to the DIOP_Profile.
  for (;
       index < this->endpoint_count_;
       ++index)
    {
      TAO_DIOP_Endpoint *endpoint = 0;
      ACE_NEW_RETURN (endpoint,
                      TAO_DIOP_Endpoint (this->hosts_[index],
                                         this->addrs_[index].get_port_number (),
                                         this->addrs_[index]),
                      -1);
      endpoint->priority (priority);
      iiop_profile->add_endpoint (endpoint);
    }

  return 0;
}

int
TAO_DIOP_Acceptor::is_collocated (const TAO_Endpoint *endpoint)
{
  const TAO_DIOP_Endpoint *endp =
    ACE_dynamic_cast (const TAO_DIOP_Endpoint *, endpoint);

  // Make sure the dynamically cast pointer is valid.
  if (endp == 0)
    return 0;

  for (size_t i = 0; i < this->endpoint_count_; ++i)
    {
      // compare the port and host name.  Please do *NOT* optimize
      // this code by comparing the IP address instead.  That would
      // trigger the following bug:
      //
      // http://deuce.doc.wustl.edu/bugzilla/show_bug.cgi?id=1220
      //
      if (endp->port() == this->addrs_[i].get_port_number()
          && ACE_OS::strcmp(endp->host(), this->hosts_[i]) == 0)
        return 1;  // Collocated
    }

  return 0;  // Not collocated
}

int
TAO_DIOP_Acceptor::close (void)
{
  if (this->connection_handler_)
    {
      // Remove the connection handler from the reactor in the case
      // of a valid handle, or close it yourself, if the handle is invalid.
      // Either way it will cause the connection handler to be destructed.
      if (this->connection_handler_->get_handle () != ACE_INVALID_HANDLE)
        {
          this->connection_handler_->reactor ()->remove_handler (this->connection_handler_,
                                                                 ACE_Event_Handler::READ_MASK);
        }
      else
        {
          this->connection_handler_->handle_close (ACE_INVALID_HANDLE,
                                                   ACE_Event_Handler::ALL_EVENTS_MASK);
        }
      this->connection_handler_ = 0;
    }
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

  if (this->init_tcp_properties () != 0)
    return -1;

  if (this->hosts_ != 0)
    {
      // The hostname cache has already been set!
      // This is bad mojo, i.e. an internal TAO error.
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("TAO (%P|%t) ")
                         ACE_TEXT ("DIOP_Acceptor::open - "),
                         ACE_TEXT ("hostname already set\n\n")),
                        -1);
    }

  if (address == 0)
    return -1;

  if (major >=0 && minor >= 0)
    this->version_.set_version (ACE_static_cast (CORBA::Octet,
                                                 major),
                                ACE_static_cast (CORBA::Octet,
                                                 minor));
  // Parse options
  if (this->parse_options (options) == -1)
    return -1;

  ACE_INET_Addr addr;

  const char *port_separator_loc = ACE_OS::strchr (address, ':');
  const char *specified_hostname = 0;
  char tmp_host[MAXHOSTNAMELEN + 1];

  if (port_separator_loc == address)
    {
      // The address is a port number or port name.  No hostname was
      // specified.  The hostname for each network interface and the
      // fully qualified domain name must be obtained.

      // Check for multiple network interfaces.
      if (this->probe_interfaces (orb_core) == -1)
        return -1;

      // First convert the port into a usable form.
      if (addr.set (address + sizeof (':')) != 0)
        return -1;

      // Now reset the port and set the host.
      if (addr.set (addr.get_port_number (),
                    ACE_static_cast (ACE_UINT32, INADDR_ANY),
                    1) != 0)
        return -1;
      else
        return this->open_i (addr,
                             reactor);
    }
  else if (port_separator_loc == 0)
    {
      // The address is a hostname.  No port was specified, so assume
      // port zero (port will be chosen for us).
      if (addr.set ((unsigned short) 0, address) != 0)
        return -1;

      specified_hostname = address;
    }
  else
    {
      // Host and port were specified.
      if (addr.set (address) != 0)
        return -1;

      // Extract out just the host part of the address.
      size_t len = port_separator_loc - address;
      ACE_OS::memcpy (tmp_host, address, len);
      tmp_host[len] = '\0';

      specified_hostname = tmp_host;
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
                      specified_hostname) != 0)
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

  if (this->init_tcp_properties () != 0)
    return -1;

  if (this->hosts_ != 0)
    {
      // The hostname cache has already been set!
      // This is bad mojo, i.e. an internal TAO error.
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("TAO (%P|%t) ")
                         ACE_TEXT ("DIOP_Acceptor::open_default - "),
                         ACE_TEXT ("hostname already set\n\n")),
                        -1);
    }

  if (major >=0 && minor >= 0)
    this->version_.set_version (ACE_static_cast (CORBA::Octet,
                                                 major),
                                ACE_static_cast (CORBA::Octet,
                                                 minor));

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

  if (addr.set (ACE_DEFAULT_SERVER_PORT,
                ACE_static_cast(ACE_UINT32, INADDR_ANY),
                1) != 0)
    return -1;

  return this->open_i (addr,
                       reactor);
}

int
TAO_DIOP_Acceptor::open_i (const ACE_INET_Addr& addr,
                           ACE_Reactor *reactor)
{
  ACE_NEW_RETURN (this->connection_handler_,
                  TAO_DIOP_Connection_Handler (this->orb_core_,
                                               this->lite_flag_,
                                               0 /* TAO_DIOP_Properties */),
                  -1);

  this->connection_handler_->local_addr (addr);
  this->connection_handler_->open_server ();

  // Register only with a valid handle
  if (this->connection_handler_->get_handle () != ACE_INVALID_HANDLE)
    {
      reactor->register_handler (this->connection_handler_,
                                 ACE_Event_Handler::READ_MASK);
    }

  // Set the port for each addr.  If there is more than one network
  // interface then the endpoint created on each interface will be on
  // the same port.  This is how a wildcard socket bind() is supposed
  // to work.
  u_short port = addr.get_port_number ();
  for (size_t j = 0; j < this->endpoint_count_; ++j)
    this->addrs_[j].set_port_number (port, 1);

  if (TAO_debug_level > 5)
    {
      for (size_t i = 0; i < this->endpoint_count_; ++i)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("\nTAO (%P|%t) DIOP_Acceptor::open_i - ")
                      ACE_TEXT ("listening on: <%s:%u>\n"),
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
  else
  if (specified_hostname != 0)
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
TAO_DIOP_Acceptor::dotted_decimal_address (ACE_INET_Addr &addr,
                                           char *&host)
{
  const char *tmp = addr.get_host_addr ();
  if (tmp == 0)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("\n\nTAO (%P|%t) ")
                    ACE_TEXT ("DIOP_Acceptor::dotted_decimal_address ")
                    ACE_TEXT ("- %p\n\n"),
                    ACE_TEXT ("cannot determine hostname")));
      return -1;
    }

  host = CORBA::string_dup (tmp);
  return 0;
}

int
TAO_DIOP_Acceptor::probe_interfaces (TAO_ORB_Core *orb_core)
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
          ACE_DEBUG ((LM_WARNING,
                      ACE_TEXT ("TAO (%P|%t) Unable to probe network ")
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
    if (if_addrs[j].get_ip_address () == INADDR_LOOPBACK)
      lo_cnt++;

  // The instantiation for this template is in
  // tao/DIOP_Connector.cpp.
  ACE_Auto_Basic_Array_Ptr<ACE_INET_Addr> safe_if_addrs (if_addrs);

  // If the loopback interface is the only interface then include it
  // in the list of interfaces to query for a hostname, otherwise
  // exclude it from the list.
  if (if_cnt == lo_cnt)
    this->endpoint_count_ = ACE_static_cast (CORBA::ULong, if_cnt);
  else
    this->endpoint_count_ = ACE_static_cast (CORBA::ULong, if_cnt - lo_cnt);

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
  TAO_InputCDR cdr (ACE_reinterpret_cast(char*,profile.profile_data.get_buffer ()),
                    profile.profile_data.length ());
#endif /* TAO_NO_COPY_OCTET_SEQUENCES == 1 */

  CORBA::Octet major, minor;

  // Read the version. We just read it here. We don't*do any*
  // processing.
  if (!(cdr.read_octet (major)
        && cdr.read_octet (minor)))
  {
    if (TAO_debug_level > 0)
      {
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%P|%t) DIOP_Profile::decode - v%d.%d\n"),
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
                      ACE_TEXT ("TAO (%P|%t) TAO_DIOP_Acceptor::object_key - ")
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

  int begin = 0;
  int end = -1;

  for (CORBA::ULong j = 0; j < option_count; ++j)
    {
      begin += end + 1;

      if (j < option_count - 1)
        end = options.find (option_delimiter, begin);
      else
        end = ACE_static_cast (int,
                               len - begin); // Handle last endpoint differently

      if (end == begin)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("TAO (%P|%t) Zero length DIOP option.\n")),
                          -1);
      else if (end != ACE_CString::npos)
        {
          ACE_CString opt = options.substring (begin, end);

          int slot = opt.find ("=");

          if (slot == ACE_static_cast (int, len - 1)
              || slot == ACE_CString::npos)
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("TAO (%P|%t) DIOP option <%s> is ")
                               ACE_TEXT ("missing a value.\n"),
                               opt.c_str ()),
                              -1);

          ACE_CString name = opt.substring (0, slot);
          ACE_CString value = opt.substring (slot + 1);

          if (name.length () == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("TAO (%P|%t) Zero length DIOP ")
                               ACE_TEXT ("option name.\n")),
                              -1);

          if (name == "priority")
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 ACE_TEXT ("TAO (%P|%t) Invalid DIOP endpoint format: ")
                                 ACE_TEXT ("endpoint priorities no longer supported. \n"),
                                 value.c_str ()),
                                -1);
            }
          else
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("TAO (%P|%t) Invalid DIOP option: <%s>\n"),
                               name.c_str ()),
                              -1);
        }
    }
  return 0;
}

int
TAO_DIOP_Acceptor::init_tcp_properties (void)
{
  // @@ Michael: We use UDP, so we do not set TCP settings.
  return 0;
}

#endif /* TAO_HAS_DIOP && TAO_HAS_DIOP != 0 */
