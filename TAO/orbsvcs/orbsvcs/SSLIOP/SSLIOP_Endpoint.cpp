#include "SSLIOP_Endpoint.h"

#include "tao/IIOP_Endpoint.h"

#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"


ACE_RCSID (SSLIOP,
           SSLIOP_Endpoint,
           "$Id$")


#if !defined (__ACE_INLINE__)
# include "SSLIOP_Endpoint.i"
#endif /* __ACE_INLINE__ */

TAO_SSLIOP_Endpoint::TAO_SSLIOP_Endpoint (const ::SSLIOP::SSL *ssl_component,
                                          TAO_IIOP_Endpoint *iiop_endp)
  : TAO_Endpoint (IOP::TAG_INTERNET_IOP),
    object_addr_ (),
    next_ (0),
    iiop_endpoint_ (iiop_endp),
    destroy_iiop_endpoint_ (false),
    qop_ (::Security::SecQOPIntegrityAndConfidentiality),
#if !defined (VXWORKS) && !defined (__QNX__)
    // Some compilers don't like the initialization
    trust_ (),
#endif /* !VXWORKS && !__QNX__ */
    credentials_ (),
    credentials_set_ (0)
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
                    ::Security::Integrity
                    | ::Security::Confidentiality
                    | ::Security::NoDelegation);

      // SSLIOP supports these Security::AssociationOptions by
      // default.
      //
      // Note that the Security::NoProtection bit is set since we
      // can't be sure if the server supports SSL, and TAO's SSLIOP
      // implementation must support IIOP over SSL and plain IIOP.
      ACE_SET_BITS (this->ssl_component_.target_supports,
                    ::Security::Integrity
                    | ::Security::Confidentiality
                    | ::Security::EstablishTrustInTarget
                    | ::Security::NoProtection
                    | ::Security::NoDelegation);

      // Initialize the default SSL port to zero, not the IANA
      // assigned IIOP over SSL port (684).  We usually only get here
      // if we're creating a profile on the client side using an IOR
      // that does not contain an SSLIOP tagged component.
      this->ssl_component_.port = 0;
    }

  // Invalidate the Addr until the first attempt to use it is made.
  this->object_addr_.set_type (-1);

  this->trust_.trust_in_target = 1;
  this->trust_.trust_in_client = 1;
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


TAO_Endpoint *
TAO_SSLIOP_Endpoint::next (void)
{
  return this->next_;
}

CORBA::Boolean
TAO_SSLIOP_Endpoint::is_equivalent (const TAO_Endpoint *other_endpoint)
{
  TAO_Endpoint *endpt = const_cast<TAO_Endpoint *> (other_endpoint);

  TAO_SSLIOP_Endpoint *endpoint =
    dynamic_cast<TAO_SSLIOP_Endpoint *> (endpt);

  if (endpoint == 0)
    return 0;

  ::Security::EstablishTrust t = endpoint->trust ();

  if ((this->ssl_component_.port != 0
       && endpoint->ssl_component_.port != 0
       && this->ssl_component_.port != endpoint->ssl_component_.port)
      || this->qop_ != endpoint->qop ()
      || this->trust_.trust_in_target != t.trust_in_target
      || this->trust_.trust_in_client != t.trust_in_client
      || (!CORBA::is_nil (this->credentials_.in ())
          && !(*this->credentials_.in () == *endpoint->credentials ())))
    return 0;

  return
    this->iiop_endpoint_->is_equivalent (endpoint->iiop_endpoint_);
}

TAO_Endpoint *
TAO_SSLIOP_Endpoint::duplicate (void)
{
  TAO_SSLIOP_Endpoint *endpoint = 0;

  // @@ We need to set the priority of the newly formed endpoint. It
  // shouldnt be a problem as long as SSL is not used with RTCORBA.
  ACE_NEW_RETURN (endpoint,
                  TAO_SSLIOP_Endpoint (&this->ssl_component_,
                                         0),
                  0);

  if (this->credentials_set_)
    endpoint->set_sec_attrs (this->qop_,this->trust_, this->credentials_.in());

  endpoint->iiop_endpoint (this->iiop_endpoint_, true);
  endpoint->hash_val_ = this->hash_val_;
  return endpoint;
}

CORBA::ULong
TAO_SSLIOP_Endpoint::hash (void)
{
  // there is actually the potential for a race of the inverse case,
  // since setting the security attributes will reset the hash_val_,
  // it is possible this test to pass, but then have the hash reset
  // before the value is returned.
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
      this->iiop_endpoint_->hash ()
      + this->ssl_component_.port
      + this->qop_
      + this->trust_.trust_in_target
      + this->trust_.trust_in_client
      + (CORBA::is_nil (this->credentials_.in ())
         ? 0
         : this->credentials_->hash ());
  }

  return this->hash_val_;
}


const ACE_INET_Addr &
TAO_SSLIOP_Endpoint::object_addr (void) const
{
  // The object_addr_ is initialized here, rather than at IOR decode
  // time for several reasons:
  //   1. A request on the object may never be invoked.
  //   2. The DNS setup may have changed dynamically.
  //   ...etc..

  // Double checked locking optimization.
  if (this->object_addr_.get_type () != AF_INET)
    {
      const ACE_INET_Addr &iiop_addr = this->iiop_endpoint_->object_addr ();

      ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                        guard,
                        this->addr_lookup_lock_,
                        this->object_addr_);

      if (this->object_addr_.get_type () != AF_INET)
        {
          this->object_addr_ = iiop_addr;
          this->object_addr_.set_port_number (this->ssl_component_.port);
        }
    }

  return this->object_addr_;
}

void
TAO_SSLIOP_Endpoint::set_sec_attrs (::Security::QOP q,
                                    const ::Security::EstablishTrust &t,
                                    const TAO::SSLIOP::OwnCredentials_ptr c)
{
  if (this->credentials_set_)
    return;

  ACE_GUARD (TAO_SYNCH_MUTEX,
             guard,
             this->addr_lookup_lock_);

  // double-check
  if (this->credentials_set_)
    return;

  this->qop_ = q;
  this->trust_ = t;
  this->credentials_ = TAO::SSLIOP::OwnCredentials::_duplicate (c);
  this->credentials_set_ = 1;

  // reset the hash value to force a recomputation.
  this->hash_val_ = 0;
}
