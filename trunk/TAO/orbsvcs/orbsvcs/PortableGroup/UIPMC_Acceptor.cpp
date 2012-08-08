// This may look like C, but it's really -*- C++ -*-
// $Id$

#include "orbsvcs/PortableGroup/UIPMC_Profile.h"
#include "orbsvcs/PortableGroup/UIPMC_Acceptor.h"

#include "tao/MProfile.h"
#include "tao/ORB_Core.h"
#include "tao/debug.h"
#include "tao/Protocols_Hooks.h"
#include "tao/ORB_Constants.h"

#include "ace/Auto_Ptr.h"
#include "ace/os_include/os_netdb.h"

#if !defined(__ACE_INLINE__)
#include "orbsvcs/PortableGroup/UIPMC_Acceptor.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_UIPMC_Acceptor::TAO_UIPMC_Acceptor (bool listen_on_all_ifs)
  : TAO_Acceptor (IOP::TAG_UIPMC),
    addrs_ (0),
    hosts_ (0),
    endpoint_count_ (0),
    version_ (TAO_DEF_GIOP_MAJOR, TAO_DEF_GIOP_MINOR),
    orb_core_ (0),
    connection_handler_ (0),
    listen_on_all_(listen_on_all_ifs)
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
TAO_UIPMC_Acceptor::create_profile (const TAO::ObjectKey &,
  TAO_MProfile &,
  CORBA::Short)
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
TAO_UIPMC_Acceptor::is_collocated (const TAO_Endpoint *)
{
  // @@ Not clear how
  // to best handle collation.  For example, one servant could
  // be collocated, but we still need to send the request out on
  // the network to see if there are any other servants in the
  // group.
  return 0;  // Not collocated
}

int
TAO_UIPMC_Acceptor::close (void)
{
  return 0;
}

int
TAO_UIPMC_Acceptor::open (TAO_ORB_Core *orb_core,
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
                         ACE_TEXT ("UIPMC_Acceptor::open - ")
                         ACE_TEXT ("hostname already set\n\n")),
                        -1);
    }

  if (address == 0)
    return -1;

  if (major >= 0 && minor >= 0)
    this->version_.set_version (static_cast<CORBA::Octet> (major),
                                static_cast<CORBA::Octet> (minor));
  // Parse options
  if (this->parse_options (options) == -1)
    return -1;

  ACE_INET_Addr addr;

  const char *port_separator_loc = ACE_OS::strchr (address, ':');
  const char *specified_hostname = 0;
  char tmp_host[MAXHOSTNAMELEN + 1];

#if defined (ACE_HAS_IPV6)
  // Check if this is a (possibly) IPv6 supporting profile containing a
  // numeric IPv6 address representation.
  if ((this->version_.major > TAO_MIN_IPV6_IIOP_MAJOR ||
        this->version_.minor >= TAO_MIN_IPV6_IIOP_MINOR) &&
      address[0] == '[')
    {
      // In this case we have to find the end of the numeric address and
      // start looking for the port separator from there.
      const char *cp_pos = ACE_OS::strchr(address, ']');
      if (cp_pos == 0)
        {
          // No valid IPv6 address specified.
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("TAO (%P|%t) - ")
                             ACE_TEXT ("UIPMC_Acceptor::open, ")
                             ACE_TEXT ("Invalid IPv6 decimal address specified\n\n")),
                            -1);
        }
      else
        {
          if (cp_pos[1] == ':')    // Look for a port
            port_separator_loc = cp_pos + 1;
          else
            port_separator_loc = 0;
          // Extract out just the host part of the address.
          const size_t len = cp_pos - (address + 1);
          ACE_OS::memcpy (tmp_host, address + 1, len);
          tmp_host[len] = '\0';
        }
    }
  else
    {
#endif /* ACE_HAS_IPV6 */
      // Extract out just the host part of the address.
      size_t len = port_separator_loc - address;
      ACE_OS::memcpy (tmp_host, address, len);
      tmp_host[len] = '\0';
#if defined (ACE_HAS_IPV6)
    }
#endif /* ACE_HAS_IPV6 */

  // Both host and port have to be specified.
  if (port_separator_loc == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("TAO (%P|%t) - ")
                         ACE_TEXT ("UIPMC_Acceptor::open, ")
                         ACE_TEXT ("port is not specified\n\n")),
                        -1);
    }

  if (addr.set (address) != 0)
    return -1;

  specified_hostname = tmp_host;

#if defined (ACE_HAS_IPV6)
  // Check for violation of ORBConnectIPV6Only option
  if (this->orb_core_->orb_params ()->connect_ipv6_only () &&
      (addr.get_type () != AF_INET6 ||
       addr.is_ipv4_mapped_ipv6 ()))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("TAO (%P|%t) - ")
                         ACE_TEXT ("UIPMC_Acceptor::open, ")
                         ACE_TEXT ("non-IPv6 endpoints not allowed when ")
                         ACE_TEXT ("connect_ipv6_only is set\n\n")),
                        -1);
    }
#endif /* ACE_HAS_IPV6 */

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
  // TAO_UIPMC_Acceptor::open_i().
  if (this->addrs_[0].set (addr) != 0)
    return -1;

  return this->open_i (addr,
                       reactor);
}

int
TAO_UIPMC_Acceptor::open_default (TAO_ORB_Core *,
                                  ACE_Reactor *,
                                  int,
                                  int,
                                  const char *)
{
  // There is no such thing as a default multicast listen
  // port.  The mechanism for choosing these ports is done
  // when creating the group ids.  (I.e. not here).
  return -1;
}

int
TAO_UIPMC_Acceptor::open_i (const ACE_INET_Addr& addr,
                            ACE_Reactor *reactor)
{
  ACE_NEW_RETURN (this->connection_handler_,
                  TAO_UIPMC_Mcast_Connection_Handler (this->orb_core_),
                  -1);

  this->connection_handler_->local_addr (addr);
  this->connection_handler_->listen_on_all (this->listen_on_all_);
  if (this->connection_handler_->open (0))
    {
        ACE_DEBUG ((LM_ERROR,
                      ACE_TEXT("TAO (%P|%t) - TAO_UIPMC_Acceptor::open_i, ")
                      ACE_TEXT("failed to open connection handler.\n")
                  ));
        return -1;
    }

  int result =
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
                      ACE_TEXT ("TAO (%P|%t) - UIPMC_Acceptor::open_i ")
                      ACE_TEXT ("listening on: <%s:%u>\n"),
                      this->hosts_[i],
                      this->addrs_[i].get_port_number ()));
        }
    }

  return 0;
}

int
TAO_UIPMC_Acceptor::hostname (TAO_ORB_Core *,
                              ACE_INET_Addr &addr,
                              char *&host,
                              const char *)
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

CORBA::ULong
TAO_UIPMC_Acceptor::endpoint_count (void)
{
  return this->endpoint_count_;
}

int
TAO_UIPMC_Acceptor::object_key (IOP::TaggedProfile &,
                                TAO::ObjectKey &)
{
  // No object key to extract.  Just return success.
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
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("TAO (%P|%t) Zero length UIPMC option.\n")),
                          -1);
      else if (end != ACE_CString::npos)
        {
          ACE_CString opt = options.substring (begin, end);

          ACE_CString::size_type const slot = opt.find ("=");

          if (slot == len - 1
              || slot == ACE_CString::npos)
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("TAO (%P|%t) UIPMC option <%s> is ")
                               ACE_TEXT ("missing a value.\n"),
                               opt.c_str ()),
                              -1);

          ACE_CString name = opt.substring (0, slot);
          ACE_CString value = opt.substring (slot + 1);

          begin = end + 1;

          if (name.length () == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("TAO (%P|%t) Zero length UIPMC ")
                               ACE_TEXT ("option name.\n")),
                              -1);

          if (name == "priority")
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 ACE_TEXT ("TAO (%P|%t) Invalid UIPMC endpoint format: ")
                                 ACE_TEXT ("endpoint priorities no longer supported.\n"),
                                 value.c_str ()),
                                -1);
            }
          else
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("TAO (%P|%t) Invalid UIPMC option: <%s>\n"),
                               name.c_str ()),
                              -1);
        }
      else
        break;  // No other options.
    }
  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
