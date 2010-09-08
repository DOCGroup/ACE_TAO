// $Id$

#include "orbsvcs/PortableGroup/UIPMC_Endpoint.h"
#include "orbsvcs/PortableGroup/UIPMC_Profile.h"

#include "tao/debug.h"
#include "ace/Guard_T.h"
#include "tao/ORB_Constants.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_Memory.h"

#if !defined (__ACE_INLINE__)
# include "orbsvcs/PortableGroup/UIPMC_Endpoint.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_UIPMC_Endpoint::TAO_UIPMC_Endpoint (void)
  : TAO_Endpoint (IOP::TAG_UIPMC),
    host_ (),
    port_ (0),
    object_addr_ (),
    next_ (0)
{
}

TAO_UIPMC_Endpoint::TAO_UIPMC_Endpoint (const ACE_INET_Addr &addr)
  : TAO_Endpoint (IOP::TAG_UIPMC),
    host_ (),
    port_ (0),
    object_addr_ (addr),
    next_ (0)
{
  this->object_addr (addr);
}

// Use of this ctr must be avoided
TAO_UIPMC_Endpoint::TAO_UIPMC_Endpoint (const CORBA::Octet class_d_address[4],
                                        CORBA::UShort port)
  : TAO_Endpoint (IOP::TAG_UIPMC),
    port_ (port),
    next_ (0)
{
  for (int i = 0; i<4; i++)
    this->class_d_address_[i] = class_d_address[i];

  this->update_object_addr ();
}

TAO_UIPMC_Endpoint::~TAO_UIPMC_Endpoint (void)
{
}

void
TAO_UIPMC_Endpoint::object_addr (const ACE_INET_Addr &addr)
{
  this->port_ = addr.get_port_number();
  this->host_ = CORBA::string_dup (addr.get_host_addr ());

  this->object_addr_.set (addr);
}

const char *
TAO_UIPMC_Endpoint::host (void) const
{
  return this->host_.in ();
}

int
TAO_UIPMC_Endpoint::addr_to_string (char *buffer, size_t length)
{
  size_t actual_len =
    ACE_OS::strlen (this->object_addr_.get_host_addr ()) // chars in host name
    + sizeof (':')               // delimiter
    + 5                          // max port
    + sizeof ('\0');

#if defined (ACE_HAS_IPV6)
  if (this->object_addr_.get_type () == AF_INET6)
    actual_len += 2; // '[' + ']'
#endif /* ACE_HAS_IPV6 */

  if (length < actual_len)
    return -1;

#if defined (ACE_HAS_IPV6)
  if (this->object_addr_.get_type () == AF_INET6)
    ACE_OS::sprintf (buffer, "[%s]:%d",
                     this->object_addr_.get_host_addr (),
                     this->port_);
  else
#endif /* ACE_HAS_IPV6 */
  ACE_OS::sprintf (buffer, "%s:%d",
                   this->object_addr_.get_host_addr (),
                   this->port_);

  return 0;
}

TAO_Endpoint *
TAO_UIPMC_Endpoint::next (void)
{
  return this->next_;
}

TAO_Endpoint *
TAO_UIPMC_Endpoint::duplicate (void)
{
  TAO_UIPMC_Endpoint *endpoint = 0;

  ACE_NEW_RETURN (endpoint,
                  TAO_UIPMC_Endpoint (this->object_addr_),
                  0);

  return endpoint;
}

CORBA::Boolean
TAO_UIPMC_Endpoint::is_equivalent (const TAO_Endpoint *other_endpoint)
{
  const TAO_UIPMC_Endpoint *endpoint =
    dynamic_cast<const TAO_UIPMC_Endpoint *> (other_endpoint);

  if (endpoint == 0)
    return 0;

  return
    (this->port_ == endpoint->port_
     && ACE_OS::strcmp (this->host (), endpoint->host ()) == 0);
}

CORBA::ULong
TAO_UIPMC_Endpoint::hash (void)
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

    this->hash_val_ = this->object_addr_.hash ();
  }

  return this->hash_val_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
