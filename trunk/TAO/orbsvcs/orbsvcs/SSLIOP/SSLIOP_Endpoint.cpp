// This may look like C, but it's really -*- C++ -*-
//
// $Id$

#include "SSLIOP_Endpoint.h"
#include "SSLIOP_Connection_Handler.h"
#include "tao/IIOP_Endpoint.h"

ACE_RCSID (SSLIOP,
           SSLIOP_Endpoint,
           "$Id$")

#if !defined (__ACE_INLINE__)
# include "SSLIOP_Endpoint.i"
#endif /* __ACE_INLINE__ */

TAO_SSLIOP_Endpoint::TAO_SSLIOP_Endpoint (const SSLIOP::SSL *ssl_component,
                                          TAO_IIOP_Endpoint *iiop_endp)
  : TAO_Endpoint (IOP::TAG_INTERNET_IOP),
    object_addr_ (),
    next_ (0),
    iiop_endpoint_ (iiop_endp),
    destroy_iiop_endpoint_ (0)
{
  if (ssl_component != 0)
    {
      // Copy the security association options in the IOR's SSL tagged
      // component.
      this->ssl_component_.target_supports = ssl_component->target_supports;
      this->ssl_component_.target_requires = ssl_component->target_requires;
      this->ssl_component_.port = ssl_component->port;
    }
  else
    {
      // No SSL tagged component is available so construct a default
      // set of security association options, in addition to the IANA
      // assigned IIOP over SSL port (684).  This is generally a
      // client side issue.

      // Clear all bits in the SSLIOP::SSL association option fields.
      this->ssl_component_.target_supports = 0;
      this->ssl_component_.target_requires = 0;

      // SSLIOP requires these Security::AssociationOptions by default.
      ACE_SET_BITS (this->ssl_component_.target_requires,
                    Security::Integrity
                    | Security::Confidentiality
                    | Security::NoDelegation);

      // SSLIOP supports these Security::AssociationOptions by
      // default.
      //
      // Note that the Security::NoProtection bit is set since we
      // can't be sure if the server supports SSL, and TAO's SSLIOP
      // implementation must support IIOP over SSL and plain IIOP.
      ACE_SET_BITS (this->ssl_component_.target_supports,
                    Security::Integrity
                    | Security::Confidentiality
                    | Security::EstablishTrustInTarget
                    | Security::NoProtection
                    | Security::NoDelegation);

      // Initialize the default SSL port to zero, not the IANA
      // assigned IIOP over SSL port (684).  We usually only get here
      // if we're creating a profile on the client side using an IOR
      // that does not contain an SSLIOP tagged component.
      this->ssl_component_.port = 0;
    }

  // Invalidate the Addr until the first attempt to use it is made.
  this->object_addr_.set_type (-1);
}

TAO_SSLIOP_Endpoint::~TAO_SSLIOP_Endpoint (void)
{
  if (this->destroy_iiop_endpoint_)
    delete this->iiop_endpoint_;
}

int
TAO_SSLIOP_Endpoint::addr_to_string (char *buffer, size_t length)
{
  size_t actual_len =
    ACE_OS::strlen (this->iiop_endpoint_->host ()) // chars in host name
    + sizeof (':')                                 // delimiter
    + ACE_OS::strlen ("65536")                     // max port
    + sizeof ('\0');

  if (length < actual_len)
    return -1;

  ACE_OS::sprintf (buffer,
                   "%s:%d",
                   this->iiop_endpoint_->host (),
                   this->ssl_component_.port);

  return 0;
}

void
TAO_SSLIOP_Endpoint::reset_hint (void)
{
  this->iiop_endpoint_->reset_hint ();

  // @@ Who is doing the locking here!
  /*if (this->ssl_hint_)
    {
      this->ssl_hint_->cleanup_hint ();
      this->ssl_hint_ = 0;
      }*/
}

TAO_Endpoint *
TAO_SSLIOP_Endpoint::next (void)
{
  return this->next_;
}

CORBA::Boolean
TAO_SSLIOP_Endpoint::is_equivalent (const TAO_Endpoint *other_endpoint)
{
  TAO_Endpoint *endpt = ACE_const_cast (TAO_Endpoint *,
                                        other_endpoint);

  TAO_SSLIOP_Endpoint *endpoint =
    ACE_dynamic_cast (TAO_SSLIOP_Endpoint *, endpt);

  if (endpoint == 0)
    return 0;

  if (this->ssl_component_.port != 0
      && endpoint->ssl_component_.port != 0
      && this->ssl_component_.port != endpoint->ssl_component_.port)
    return 0;

  return
    this->iiop_endpoint_->is_equivalent (endpoint->iiop_endpoint_);
}

TAO_Endpoint *
TAO_SSLIOP_Endpoint::duplicate (void)
{
  TAO_SSLIOP_Endpoint *endpoint = 0;

  ACE_NEW_RETURN (endpoint,
                  TAO_SSLIOP_Endpoint (&this->ssl_component_,
                                       0),
                  0);

  endpoint->iiop_endpoint (this->iiop_endpoint_, 1);

  return endpoint;
}

CORBA::ULong
TAO_SSLIOP_Endpoint::hash (void)
{
  return this->iiop_endpoint_->hash () + this->ssl_component_.port;
}
