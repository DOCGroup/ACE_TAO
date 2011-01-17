// $Id$

#include "tao/Strategies/SHMIOP_Endpoint.h"

#if defined (TAO_HAS_SHMIOP) && (TAO_HAS_SHMIOP != 0)

#include "tao/Strategies/SHMIOP_Connection_Handler.h"
#include "tao/debug.h"
#include "tao/ORB_Constants.h"

#include "ace/OS_NS_stdio.h"

#if !defined (__ACE_INLINE__)
# include "tao/Strategies/SHMIOP_Endpoint.inl"
#endif /* __ACE_INLINE__ */

#include "ace/os_include/os_netdb.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_SHMIOP_Endpoint::TAO_SHMIOP_Endpoint (const ACE_MEM_Addr &addr,
                                          int use_dotted_decimal_addresses)
  : TAO_Endpoint (TAO_TAG_SHMEM_PROFILE)
    , host_ ()
    , port_ (0)
    , object_addr_ (addr.get_remote_addr ())
    , object_addr_set_ (0)
    , next_ (0)
{
  this->set (addr.get_remote_addr (), use_dotted_decimal_addresses);
}

TAO_SHMIOP_Endpoint::TAO_SHMIOP_Endpoint (const ACE_INET_Addr &addr,
                                          int use_dotted_decimal_addresses)
  : TAO_Endpoint (TAO_TAG_SHMEM_PROFILE)
    , host_ ()
    , port_ (0)
    , object_addr_ (addr)
    , object_addr_set_ (0)
    , next_ (0)
{
  this->set (addr, use_dotted_decimal_addresses);
}

TAO_SHMIOP_Endpoint::TAO_SHMIOP_Endpoint (const char *host,
                                          CORBA::UShort port,
                                          const ACE_INET_Addr &addr,
                                          CORBA::Short priority)
  : TAO_Endpoint (TAO_TAG_SHMEM_PROFILE, priority)
    , host_ ()
    , port_ (port)
    , object_addr_ (addr)
    , object_addr_set_ (0)
    , next_ (0)
{
  if (host != 0)
    this->host_ = host;
}

TAO_SHMIOP_Endpoint::TAO_SHMIOP_Endpoint (void)
  : TAO_Endpoint (TAO_TAG_SHMEM_PROFILE)
    , host_ ()
    , port_ (0)
    , object_addr_ ()
    , object_addr_set_ (0)
    , next_ (0)
{
}

TAO_SHMIOP_Endpoint::TAO_SHMIOP_Endpoint (const char *host,
                                          CORBA::UShort port,
                                          CORBA::Short priority)
  : TAO_Endpoint (TAO_TAG_SHMEM_PROFILE)
    , host_ ()
    , port_ (port)
    , object_addr_ ()
    , object_addr_set_ (0)
    , next_ (0)
{
  if (host != 0)
    this->host_ = host;

  this->priority (priority);
}

TAO_SHMIOP_Endpoint::~TAO_SHMIOP_Endpoint (void)
{
}

int
TAO_SHMIOP_Endpoint::set (const ACE_INET_Addr &addr,
                          int use_dotted_decimal_addresses)
{
  char tmp_host[MAXHOSTNAMELEN + 1];

  if (use_dotted_decimal_addresses
      || addr.get_host_name (tmp_host, sizeof (tmp_host)) != 0)
    {
      if (use_dotted_decimal_addresses == 0 && TAO_debug_level > 5)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) - SHMIOP_Endpoint::set, ")
                      ACE_TEXT ("- %p cannot determine hostname\n")));
        }

      const char *tmp = addr.get_host_addr ();
      if (tmp == 0)
        {
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("TAO (%P|%t) - ")
                        ACE_TEXT ("SHMIOP_Endpoint::set ")
                        ACE_TEXT ("- %p\n"),
                        ACE_TEXT ("cannot determine hostname")));
          return -1;
        }
      else
        this->host_ = tmp;
    }
  else
    this->host_ = CORBA::string_dup (tmp_host);

  this->port_ = addr.get_port_number();

  return 0;
}

int
TAO_SHMIOP_Endpoint::addr_to_string (char *buffer, size_t length)
{
  size_t actual_len =
    ACE_OS::strlen (this->host_.in ()) // chars in host name
    + sizeof (':')                     // delimiter
    + ACE_OS::strlen ("65536")         // max port
    + sizeof ('\0');

  if (length < actual_len)
    return -1;

  ACE_OS::sprintf (buffer, "%s:%d",
                   this->host_.in (), this->port_);

  return 0;
}

const char *
TAO_SHMIOP_Endpoint::host (const char *h)
{
  this->host_ = h;

  return this->host_.in ();
}

TAO_Endpoint *
TAO_SHMIOP_Endpoint::next (void)
{
  return this->next_;
}

TAO_Endpoint *
TAO_SHMIOP_Endpoint::duplicate (void)
{
  TAO_SHMIOP_Endpoint *endpoint = 0;
  ACE_NEW_RETURN (endpoint,
                  TAO_SHMIOP_Endpoint (this->host_.in (),
                                       this->port_,
                                       this->object_addr_,
                                       this->priority ()),
                  0);

  return endpoint;
}


CORBA::Boolean
TAO_SHMIOP_Endpoint::is_equivalent (const TAO_Endpoint *other_endpoint)
{
  const TAO_SHMIOP_Endpoint *endpoint = dynamic_cast <const TAO_SHMIOP_Endpoint *>
                                                     (other_endpoint);

  if (endpoint == 0)
    return 0;

  return
    this->port_ == endpoint->port_
    && ACE_OS::strcmp (this->host_.in (), endpoint->host_.in ()) == 0;
}

CORBA::ULong
TAO_SHMIOP_Endpoint::hash (void)
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
TAO_SHMIOP_Endpoint::object_addr (void) const
{
  // The object_addr_ is initialized here, rather than at IOR decode
  // time for several reasons:
  //   1. A request on the object may never be invoked.
  //   2. The DNS setup may have changed dynamically.
  //   ...etc..

  // Double checked locking optimization.
  if (!this->object_addr_set_)
    {
      // We need to modify the object_addr_ in this method.  Do so
      // using a  non-const copy of the <this> pointer.
      TAO_SHMIOP_Endpoint *endpoint =
        const_cast <TAO_SHMIOP_Endpoint *>(this);

      ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                        guard,
                        endpoint->addr_lookup_lock_,
                        this->object_addr_ );

      if (!this->object_addr_set_)
        {
          if (endpoint->object_addr_.set (this->port_,
                                          this->host_.in ()) == -1)
            {
              // If this call fails, it most likely due a hostname
              // lookup failure caused by a DNS misconfiguration.  If
              // a request is made to the object at the given host and
              // port, then a CORBA::TRANSIENT() exception should be
              // thrown.

              // Invalidate the ACE_INET_Addr.  This is used as a flag
              // to denote that ACE_INET_Addr initialization failed.
              endpoint->object_addr_.set_type (-1);
            }
          else
            {
              endpoint->object_addr_set_ = 1;
            }
        }
    }

  return this->object_addr_;
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_SHMIOP && TAO_HAS_SHMIOP != 0 */
