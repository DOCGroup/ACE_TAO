// This may look like C, but it's really -*- C++ -*-
// $Id$


#include "UIPMC_Acceptor.h"
#include "UIPMC_Profile.h"
#include "tao/MProfile.h"
#include "tao/ORB_Core.h"
#include "tao/Server_Strategy_Factory.h"
#include "tao/debug.h"
#include "tao/Protocols_Hooks.h"

#include "ace/Auto_Ptr.h"

#if !defined(__ACE_INLINE__)
#include "UIPMC_Acceptor.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(tao, UIPMC_Acceptor, "$Id$")


TAO_UIPMC_Acceptor::TAO_UIPMC_Acceptor (CORBA::Boolean /*flag*/)
  : TAO_Acceptor (TAO_TAG_UIPMC_PROFILE),
    addrs_ (0),
    hosts_ (0),
    endpoint_count_ (0),
    version_ (TAO_DEF_GIOP_MAJOR, TAO_DEF_GIOP_MINOR),
    orb_core_ (0)
{
}

TAO_UIPMC_Acceptor::~TAO_UIPMC_Acceptor (void)
{
  // Make sure we are closed before we start destroying the
  // strategies.
  this->close ();

  delete [] this->addrs_;

  for (size_t i = 0; i < this->endpoint_count_; ++i)
    CORBA::string_free (this->hosts_[i]);

  delete [] this->hosts_;
}

int
TAO_UIPMC_Acceptor::create_mprofile (const TAO_ObjectKey &object_key,
                                     TAO_MProfile &mprofile)
{
  // The standard mechanism for adding profiles to object references
  // for each pluggable protocol doesn't apply to UIPMC profiles, so
  // this function just returns success without doing anything.  The
  // appropiate mechanism for getting UIPMC profiles is to call the
  // multicast group manager to get a Group reference.  Invocations
  // sent to this group reference will be dispatched to the servants
  // that belong to that group.
  return 0;
}

int
TAO_UIPMC_Acceptor::is_collocated (const TAO_Endpoint *endpoint)
{
#if 0
  const TAO_UIPMC_Endpoint *endp =
    ACE_dynamic_cast (const TAO_UIPMC_Endpoint *, endpoint);

  // Make sure the dynamically cast pointer is valid.
  if (endp == 0)
    return 0;

  for (size_t i = 0; i < this->endpoint_count_; ++i)
    {
      // compare the port and sin_addr (numeric host address)
      if (endp->object_addr () == this->addrs_[i])
        return 1;  // Collocated
    }
#endif

  // @@ Frank: Comment this out for now since its not clear how
  // to best handle collation.  For example, one servant could
  // be collocated, but we still need to send the request out on
  // the network to see if there are any other servants in the
  // group.
  return 0;  // Not collocated
}

int
TAO_UIPMC_Acceptor::close (void)
{
/*
  // @@ Frank: Commented out since it seems like the reactor always is the one to 
  //    remove the connection handler.
  if (this->connection_handler_)
    {
      // Remove the connection handler from the reactor in the case
      // of a valid handle, or close it yourself, if the handle is invalid.
      // Either way it will cause the connection handler to be destructed.
      if (this->connection_handler_->get_handle () != ACE_INVALID_HANDLE)
        {
          this->orb_core_->reactor ()->remove_handler (this->connection_handler_,
                                                       ACE_Event_Handler::READ_MASK);
        }
      else
        {
          this->connection_handler_->handle_close ();
        }
      this->connection_handler_ = 0;
    }
  */
  return 0;
}

int
TAO_UIPMC_Acceptor::open (TAO_ORB_Core *orb_core,
                         int major,
                         int minor,
                         const char *address,
                         const char *options)
{
  this->orb_core_ = orb_core;

  if (this->init_uipmc_properties () != 0)
    return -1;

  if (this->hosts_ != 0)
    {
      // The hostname cache has already been set!
      // This is bad mojo, i.e. an internal TAO error.
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("TAO (%P|%t) ")
                         ACE_TEXT ("UIPMC_Acceptor::open - "),
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

  // Both host and port have to be specified.
  if (addr.set (address) != 0)
    return -1;

  // Extract out just the host part of the address.
  size_t len = port_separator_loc - address;
  ACE_OS::memcpy (tmp_host, address, len);
  tmp_host[len] = '\0';

  specified_hostname = tmp_host;

  this->endpoint_count_ = 1;  // Only one hostname to store

  ACE_NEW_RETURN (this->addrs_,
                  ACE_INET_Addr[this->endpoint_count_],
                  -1);

  ACE_NEW_RETURN (this->hosts_,
                  char *[this->endpoint_count_],
                  -1);

  if (this->hostname (orb_core,
                      addr,
                      this->hosts_[0],
                      specified_hostname) != 0)
    return -1;

  // Copy the addr.  The port is (re)set in
  // TAO_UIPMC_Acceptor::open_i().
  if (this->addrs_[0].set (addr) != 0)
    return -1;

  return this->open_i (addr);
}

int
TAO_UIPMC_Acceptor::open_default (TAO_ORB_Core *,
                                  int,
                                  int,
                                  const char *)
{
  // There is no such thing as a default multicast listen
  // port.  The mechanism for choosing these ports is done
  // when creating the group ids.  (Not here).
  return -1;
}

int
TAO_UIPMC_Acceptor::open_i (const ACE_INET_Addr& addr)
{
  ACE_NEW_RETURN (this->connection_handler_,
                  TAO_UIPMC_Connection_Handler (this->orb_core_,
                                               0 /* TAO_UIPMC_Properties */),
                  -1);

  this->connection_handler_->local_addr (addr);
  this->connection_handler_->open_server ();


  // Register only with a valid handle
  if (this->connection_handler_->get_handle () != ACE_INVALID_HANDLE)
    {
      this->orb_core_->reactor ()->register_handler (this->connection_handler_,
                                                     ACE_Event_Handler::READ_MASK);

      // Add the connection handler to cache
      this->connection_handler_->add_transport_to_cache ();

      // Set the flag in the Connection Handler and in the Wait Strategy
      this->connection_handler_->transport ()->wait_strategy ()->is_registered (1);
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
                      ACE_TEXT ("\nTAO (%P|%t) UIPMC_Acceptor::open_i - ")
                      ACE_TEXT ("listening on: <%s:%u>\n"),
                      this->hosts_[i],
                      this->addrs_[i].get_port_number ()));
        }
    }

  return 0;
}

int
TAO_UIPMC_Acceptor::hostname (TAO_ORB_Core *orb_core,
                             ACE_INET_Addr &addr,
                             char *&host,
                             const char *specified_hostname)
{
  // Only have dotted decimal addresses for multicast.
  return this->dotted_decimal_address (addr, host);
}

int
TAO_UIPMC_Acceptor::dotted_decimal_address (ACE_INET_Addr &addr,
                                           char *&host)
{
  const char *tmp = addr.get_host_addr ();
  if (tmp == 0)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("\n\nTAO (%P|%t) ")
                    ACE_TEXT ("UIPMC_Acceptor::dotted_decimal_address ")
                    ACE_TEXT ("- %p\n\n"),
                    ACE_TEXT ("cannot determine hostname")));
      return -1;
    }

  host = CORBA::string_dup (tmp);
  return 0;
}

int
TAO_UIPMC_Acceptor::probe_interfaces (TAO_ORB_Core *orb_core)
{
  // Extract the hostname for each network interface, and then cache
  // it.  The hostnames will then be used when creating a
  // TAO_UIPMC_Profile for each endpoint setup on the probed
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
  // tao/UIPMC_Connector.cpp.
  ACE_Auto_Basic_Array_Ptr<ACE_INET_Addr> safe_if_addrs (if_addrs);

  // If the loopback interface is the only interface then include it
  // in the list of interfaces to query for a hostname, otherwise
  // exclude it from the list.
  if (if_cnt == lo_cnt)
    this->endpoint_count_ = if_cnt;
  else
    this->endpoint_count_ = if_cnt - lo_cnt;

  ACE_NEW_RETURN (this->addrs_,
                  ACE_INET_Addr[this->endpoint_count_],
                  -1);

  ACE_NEW_RETURN (this->hosts_,
                  char *[this->endpoint_count_],
                  -1);

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
      // TAO_UIPMC_Acceptor::open_i().
      if (this->addrs_[host_cnt].set (if_addrs[i]) != 0)
        return -1;

      host_cnt++;
    }

  return 0;
}

CORBA::ULong
TAO_UIPMC_Acceptor::endpoint_count (void)
{
  return this->endpoint_count_;
}

int
TAO_UIPMC_Acceptor::object_key (IOP::TaggedProfile &profile,
                                TAO_ObjectKey &object_key)
{
  // @@ Frank - This is an unused function that should always
  //  fail since there is no object key.

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
                    ACE_TEXT ("TAO (%P|%t) UIPMC_Profile::decode - v%d.%d\n"),
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
                      ACE_TEXT ("TAO (%P|%t) TAO_Tagged_Profile::decode - ")
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
TAO_UIPMC_Acceptor::parse_options (const char *str)
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
        end = len - begin;  // Handle last endpoint differently

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
              CORBA::Short corba_priority =
                ACE_static_cast (CORBA::Short,
                                 ACE_OS::atoi (value.c_str ()));

              if (corba_priority >= 0
                  /* && corba_priority < 32768 */)
                // priority_ and corba_priority will always be less
                // than 32768 since CORBA::Short is a signed 16 bit
                // integer.
                this->priority_ = corba_priority;
              else
                ACE_ERROR_RETURN ((LM_ERROR,
                                   ACE_TEXT ("TAO (%P|%t) Invalid DIOP endpoint ")
                                   ACE_TEXT ("priority: <%s>\n"),
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
TAO_UIPMC_Acceptor::init_uipmc_properties (void)
{
  // @@ Michael: We use UDP, so we do not set TCP settings.
  return 0;
}
