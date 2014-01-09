// This may look like C, but it's really -*- C++ -*-
// $Id$

#include "tao/Strategies/DIOP_Endpoint.h"

#if defined (TAO_HAS_DIOP) && (TAO_HAS_DIOP != 0)

#include "tao/Strategies/DIOP_Connection_Handler.h"
#include "tao/debug.h"
#include "tao/ORB_Constants.h"

#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"

#if !defined (__ACE_INLINE__)
# include "tao/Strategies/DIOP_Endpoint.inl"
#endif /* __ACE_INLINE__ */

#include "ace/os_include/os_netdb.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_DIOP_Endpoint::TAO_DIOP_Endpoint (const ACE_INET_Addr &addr,
                                      int use_dotted_decimal_addresses)

  : TAO_Endpoint (TAO_TAG_DIOP_PROFILE)
    , host_ ()
    , port_ (0)
#if defined (ACE_HAS_IPV6)
    , is_ipv6_decimal_ (false)
#endif /* ACE_HAS_IPV6 */
    , object_addr_ (addr)
    , object_addr_set_ (false)
    , next_ (0)
{
  this->set (addr, use_dotted_decimal_addresses);
}

TAO_DIOP_Endpoint::TAO_DIOP_Endpoint (const char *host,
                                      CORBA::UShort port,
                                      const ACE_INET_Addr &addr,
                                      CORBA::Short priority)
  : TAO_Endpoint (TAO_TAG_DIOP_PROFILE,
                  priority)
    , host_ ()
    , port_ (port)
#if defined (ACE_HAS_IPV6)
    , is_ipv6_decimal_ (false)
#endif /* ACE_HAS_IPV6 */
    , object_addr_ (addr)
    , object_addr_set_ (false)
    , next_ (0)
{
  this->host (host); // With IPv6 performs check for decimal address
}

TAO_DIOP_Endpoint::TAO_DIOP_Endpoint (void)
  : TAO_Endpoint (TAO_TAG_DIOP_PROFILE),
    host_ (),
    port_ (0),
#if defined (ACE_HAS_IPV6)
    is_ipv6_decimal_ (false),
#endif /* ACE_HAS_IPV6 */
    object_addr_ (),
    object_addr_set_ (false),
    next_ (0)
{
}

TAO_DIOP_Endpoint::TAO_DIOP_Endpoint (const char *host,
                                      CORBA::UShort port,
                                      CORBA::Short priority)
  : TAO_Endpoint (TAO_TAG_DIOP_PROFILE, priority),
    host_ (),
    port_ (port),
#if defined (ACE_HAS_IPV6)
    is_ipv6_decimal_ (false),
#endif /* ACE_HAS_IPV6 */
    object_addr_ (),
    object_addr_set_ (false),
    next_ (0)
{
  this->host (host); // With IPv6 performs check for decimal address
}

TAO_DIOP_Endpoint::~TAO_DIOP_Endpoint (void)
{
}

int
TAO_DIOP_Endpoint::set (const ACE_INET_Addr &addr,
                        int use_dotted_decimal_addresses)
{
  char tmp_host[MAXHOSTNAMELEN + 1];

#if defined (ACE_HAS_IPV6)
  this->is_ipv6_decimal_ = false; // Reset
#endif /* ACE_HAS_IPV6 */

  if (use_dotted_decimal_addresses
      || addr.get_host_name (tmp_host, sizeof (tmp_host)) != 0)
    {
      if (use_dotted_decimal_addresses == 0 && TAO_debug_level > 5)
        {
          TAOLIB_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) - DIOP_Endpoint::set, ")
                      ACE_TEXT ("%p\n"),
                      ACE_TEXT ("cannot determine hostname")));
        }

      const char *tmp = addr.get_host_addr ();
      if (tmp == 0)
        {
          if (TAO_debug_level > 0)
            TAOLIB_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("TAO (%P|%t) - ")
                        ACE_TEXT ("DIOP_Endpoint::set, ")
                        ACE_TEXT ("%p\n"),
                        ACE_TEXT ("cannot determine hostname\n")));
          return -1;
        }
      else
        {
          this->host_ = tmp;
#if defined (ACE_HAS_IPV6)
          if (addr.get_type () == PF_INET6)
            this->is_ipv6_decimal_ = true;
#endif /* ACE_HAS_IPV6 */
        }
    }
  else
    this->host_ = CORBA::string_dup (tmp_host);

  this->port_ = addr.get_port_number ();

  return 0;
}

int
TAO_DIOP_Endpoint::addr_to_string (char *buffer, size_t length)
{
  size_t actual_len =
    ACE_OS::strlen (this->host_.in ()) // chars in host name
    + sizeof (':')                     // delimiter
    + ACE_OS::strlen ("65536")         // max port
    + sizeof ('\0');

#if defined (ACE_HAS_IPV6)
  if (this->is_ipv6_decimal_)
    actual_len += 2; // '[' + ']'
#endif /* ACE_HAS_IPV6 */

  if (length < actual_len)
    return -1;

#if defined (ACE_HAS_IPV6)
  if (this->is_ipv6_decimal_)
    ACE_OS::sprintf (buffer, "[%s]:%d",
                     this->host_.in (), this->port_);
  else
#endif /* ACE_HAS_IPV6 */
  ACE_OS::sprintf (buffer, "%s:%d",
                   this->host_.in (), this->port_);

  return 0;
}

const char *
TAO_DIOP_Endpoint::host (const char *h)
{
  this->host_ = h;
#if defined (ACE_HAS_IPV6)
  if (ACE_OS::strchr (h, ':') != 0)
    this->is_ipv6_decimal_ = true;
#endif /* ACE_HAS_IPV6 */

  return this->host_.in ();
}

TAO_Endpoint *
TAO_DIOP_Endpoint::next (void)
{
  return this->next_;
}

TAO_Endpoint *
TAO_DIOP_Endpoint::duplicate (void)
{
  TAO_DIOP_Endpoint *endpoint = 0;

  ACE_NEW_RETURN (endpoint,
                  TAO_DIOP_Endpoint (this->host_.in (),
                                     this->port_,
                                     this->object_addr_,
                                     this->priority ()),
                  0);

  return endpoint;
}

CORBA::Boolean
TAO_DIOP_Endpoint::is_equivalent (const TAO_Endpoint *other_endpoint)
{
  TAO_Endpoint *endpt = const_cast<TAO_Endpoint *> (other_endpoint);

  TAO_DIOP_Endpoint *endpoint = dynamic_cast<TAO_DIOP_Endpoint *> (endpt);
  if (endpoint == 0)
    return 0;

  return (this->port () == endpoint->port ()
          && ACE_OS::strcmp (this->host (), endpoint->host ()) == 0);
}

CORBA::ULong
TAO_DIOP_Endpoint::hash (void)
{
  if (this->hash_val_ != 0)
    return this->hash_val_;

  {
    ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                      guard,
                      this->addr_lookup_lock_,
                      this->hash_val_);
    // .. DCL
    if (this->hash_val_ != 0)
      return this->hash_val_;

    this->hash_val_ =
      ACE::hash_pjw (this->host ()) + this->port ();
  }

  return this->hash_val_;
}

const ACE_INET_Addr &
TAO_DIOP_Endpoint::object_addr (void) const
{
  // The object_addr_ is initialized here, rather than at IOR decode
  // time for several reasons:
  //   1. A request on the object may never be invoked.
  //   2. The DNS setup may have changed dynamically.
  //   ...etc..

  // Double checked locking optimization.
  if (!this->object_addr_set_)
    {
      ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                        guard,
                        this->addr_lookup_lock_,
                        this->object_addr_ );

      if (!this->object_addr_set_)
        {
          (void) this->object_addr_i ();
        }
    }
  return this->object_addr_;
}

void
TAO_DIOP_Endpoint::object_addr_i (void) const
{
  // We should have already held the lock

#if defined (ACE_HAS_IPV6)
  bool is_ipv4_decimal_ = false;
  if (!this->is_ipv6_decimal_)
    is_ipv4_decimal_ =
      ACE_OS::strspn (this->host_.in (), ".0123456789") ==
      ACE_OS::strlen (this->host_.in ());

  // If this is *not* an IPv4 decimal address at first try to
  // resolve the address as an IPv6 address; if that fails
  // (or it's an IPv4 address) and the address is *not* an IPv6
  // decimal address try to resolve it as an IPv4 address.
  if ((is_ipv4_decimal_ ||
        this->object_addr_.set (this->port_,
                                this->host_.in (),
                                1,
                                AF_INET6) == -1) &&
      (this->is_ipv6_decimal_ ||
        this->object_addr_.set (this->port_,
                              this->host_.in (),
                              1,
                              AF_INET) == -1))
#else
  if (this->object_addr_.set (this->port_,
                              this->host_.in ()) == -1)
#endif /* ACE_HAS_IPV6 */
    {
      // If this call fails, it most likely due a hostname
      // lookup failure caused by a DNS misconfiguration.  If
      // a request is made to the object at the given host and
      // port, then a CORBA::TRANSIENT() exception should be
      // thrown.

      // Invalidate the ACE_INET_Addr.  This is used as a flag
      // to denote that ACE_INET_Addr initialization failed.
      this->object_addr_.set_type (-1);
    }
  else
    {
      this->object_addr_set_ = true;
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_DIOP && TAO_HAS_DIOP != 0 */
