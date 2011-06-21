// $Id$

#include "orbsvcs/SSLIOP/SSLIOP_Endpoint.h"

#include "tao/IIOP_Endpoint.h"

#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"
#include "ace/os_include/os_netdb.h"

#include "tao/debug.h"

#if !defined (__ACE_INLINE__)
# include "orbsvcs/SSLIOP/SSLIOP_Endpoint.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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


TAO_SSLIOP_Endpoint &
TAO_SSLIOP_Endpoint::operator= (const TAO_SSLIOP_Endpoint &other)
{
  this->object_addr_ = other.object_addr_;
  this->qop_ = other.qop_;
  this->trust_ = other.trust_;
  this->ssl_component_ = other.ssl_component_;

  this->next_ = 0; // do not copy list membership, since we are only cloning the values

  this->iiop_endpoint(other.iiop_endpoint_,
                      other.destroy_iiop_endpoint_);
  return *this;
}

TAO_SSLIOP_Endpoint::~TAO_SSLIOP_Endpoint (void)
{
  if (this->destroy_iiop_endpoint_)
    delete this->iiop_endpoint_;
}

#if 0
static void
dump_endpoint (const char* msg, const TAO_Endpoint *other_endpoint)
{

  TAO_Endpoint *endpt = const_cast<TAO_Endpoint *> (other_endpoint);

  TAO_SSLIOP_Endpoint *endpoint =
    dynamic_cast<TAO_SSLIOP_Endpoint *> (endpt);

  if (endpoint == 0)
  {
    ACE_DEBUG ((LM_DEBUG, "TAO (%P|%t) endpoint - %s: Unable to cast an endpoint to SSLIOP_Endpoint\n", msg));
    return;
  }

  char hostaddr[MAXHOSTNAMELEN + 16];
  int gothost = endpoint->addr_to_string (hostaddr, sizeof hostaddr);

  ACE_DEBUG ((LM_INFO, "TAO (%P|%t) SSLIOPEndpoint %s - %@ {%s, ssl=%d, iiop=%d,"
                " qop=%d, trst=(%d,%d), c=%@, crdh=0x%x}, h=0x%x\n",
                msg,
                endpoint,
                (gothost == 0 ? hostaddr : "*UNKNOWN*"),
                endpoint->ssl_component ().port ,
                endpoint->iiop_endpoint ()->port (),
                endpoint->qop() ,
                endpoint->trust().trust_in_target ,
                endpoint->trust().trust_in_client ,
                endpoint->credentials() ,
                (endpoint->credentials_set () ? endpoint->credentials()->hash () : 0) ,
                endpoint->hash ()));
}
#endif /* 0 */

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
  {
    return 0;
  }

  // Comparing the underlying iiop endpoints is wrong, as their port
  // numbers often may not make sense. Or may not being used anyway.
  // Therefore, we only need to directly compare the hosts. See also the
  // comments in the hash() method.
  if (this->iiop_endpoint() == 0 || endpoint->iiop_endpoint() == 0)
    return 0;

  if ((ACE_OS::strcmp (this->iiop_endpoint()->host (),
                       endpoint->iiop_endpoint()->host ()) != 0))
    return 0;

  return 1;
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

  // Do this with no locks held, as it may try to acquire it, too.
  const ACE_INET_Addr &oaddr = this->object_addr();

  { // nested scope for the lock
    ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                      guard,
                      this->addr_lookup_lock_,
                      this->hash_val_);
    // .. DCL
    if (this->hash_val_ != 0)
      return this->hash_val_;


    // Note that we are not using the underlying IIOP endpoint's hash
    // value in order to avoid the influence of the IIOP port number,
    // since it is ignored anyway. When it features a
    // purely fictional port number, as when accepting an SSL
    // connection, the unsecured port is undefined and
    // had we used it in computing the hash it would have broken the
    // bi-directional support - as the 'guessed' IIOP port value will
    // hardly match the one specified in the bi-dir service context.
    this->hash_val_ =
      oaddr.hash ()
      + this->ssl_component_.port;
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
  if (this->object_addr_.get_type () != AF_INET
#if defined (ACE_HAS_IPV6)
      && this->object_addr_.get_type () != AF_INET6
#endif /* ACE_HAS_IPV6 */
     )
    {
      const ACE_INET_Addr &iiop_addr = this->iiop_endpoint_->object_addr ();

      ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                        guard,
                        this->addr_lookup_lock_,
                        this->object_addr_);

      if (this->object_addr_.get_type () != AF_INET
#if defined (ACE_HAS_IPV6)
          && this->object_addr_.get_type () != AF_INET6
#endif /* ACE_HAS_IPV6 */
     )
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




TAO_SSLIOP_Synthetic_Endpoint::~TAO_SSLIOP_Synthetic_Endpoint ()
{
}

TAO_SSLIOP_Synthetic_Endpoint::TAO_SSLIOP_Synthetic_Endpoint (const ::SSLIOP::SSL *ssl)
  : TAO_SSLIOP_Endpoint (ssl, 0)
{
}


TAO_SSLIOP_Synthetic_Endpoint::TAO_SSLIOP_Synthetic_Endpoint (TAO_IIOP_Endpoint *iiop_endp)
  : TAO_SSLIOP_Endpoint ((const ::SSLIOP::SSL *)0, iiop_endp)
{
   this->ssl_component_.port = iiop_endp->port ();
}


CORBA::Boolean
TAO_SSLIOP_Synthetic_Endpoint::is_equivalent (const TAO_Endpoint *other_endpoint)
{
  TAO_Endpoint *endpt = const_cast<TAO_Endpoint *> (other_endpoint);

  TAO_SSLIOP_Endpoint *endpoint =
    dynamic_cast<TAO_SSLIOP_Endpoint *> (endpt);

  if (endpoint == 0)
    return 0;

  if ((this->ssl_component ().port != 0
       && endpoint->ssl_component ().port != 0
       && this->ssl_component ().port != endpoint->ssl_component ().port)
       || this->qop () < endpoint->qop ())
  {
    return 0;
  }

  // Comparing the underlying iiop endpoints is wrong, as their port
  // numbers often may not make sense, or are not being used anyway.
  // Therefore, directly comparing the hosts at this point. See also the
  // comments in the hash() method
  if (this->iiop_endpoint() == 0 || endpoint->iiop_endpoint() == 0)
    return 0;

  if ((ACE_OS::strcmp (this->iiop_endpoint()->host (),
                       endpoint->iiop_endpoint()->host ()) != 0))
    return 0;

  return 1;
}

TAO_Endpoint *
TAO_SSLIOP_Synthetic_Endpoint::duplicate (void)
{
  TAO_SSLIOP_Synthetic_Endpoint *endpoint = 0;

  // @@ We need to set the priority of the newly formed endpoint. It
  // shouldnt be a problem as long as SSL is not used with RTCORBA.
  ACE_NEW_RETURN (endpoint,
                  TAO_SSLIOP_Synthetic_Endpoint (&(this->ssl_component ())),
                  0);

  if (this->credentials_set())
    endpoint->set_sec_attrs (this->qop (),this->trust (), this->credentials ());

  endpoint->iiop_endpoint (this->iiop_endpoint (), true);
  endpoint->hash_val_ = this->hash ();
  return endpoint;
}

TAO_END_VERSIONED_NAMESPACE_DECL
