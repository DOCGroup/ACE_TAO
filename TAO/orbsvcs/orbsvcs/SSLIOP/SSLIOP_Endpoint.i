// -*- C++ -*-
//
// $Id$


ACE_INLINE TAO_IIOP_Endpoint *
TAO_SSLIOP_Endpoint::iiop_endpoint (void) const
{
  return this->iiop_endpoint_;
}

ACE_INLINE void
TAO_SSLIOP_Endpoint::iiop_endpoint (TAO_IIOP_Endpoint *iiop_endpoint,
                                    int destroy)
{
  if (iiop_endpoint != 0)
    {
      TAO_IIOP_Endpoint *new_endpoint = 0;

      if (destroy)
        {
          TAO_Endpoint *endpoint = iiop_endpoint->duplicate ();

          new_endpoint = ACE_dynamic_cast (TAO_IIOP_Endpoint *,
                                           endpoint);

        }
      else
        new_endpoint = iiop_endpoint;

      if (this->destroy_iiop_endpoint_)
        delete this->iiop_endpoint_;

      this->iiop_endpoint_ = new_endpoint;
      this->destroy_iiop_endpoint_ = destroy;
    }
}

ACE_INLINE const SSLIOP::SSL &
TAO_SSLIOP_Endpoint::ssl_component (void) const
{
  return this->ssl_component_;
}

ACE_INLINE const ACE_INET_Addr &
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
      // We need to modify the object_addr_ in this method.  Do so
      // using a non-const copy of the <this> pointer.
      ACE_INET_Addr &ssl_addr =
        ACE_const_cast (ACE_INET_Addr &, this->object_addr_);

      const ACE_INET_Addr &iiop_addr = this->iiop_endpoint_->object_addr ();

      TAO_SSLIOP_Endpoint *ssl_endpoint =
        ACE_const_cast (TAO_SSLIOP_Endpoint *, this);

      ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                        guard,
                        ssl_endpoint->addr_lookup_lock_,
                        this->object_addr_);

      if (this->object_addr_.get_type () != AF_INET)
        {
          ssl_addr = iiop_addr;
          ssl_addr.set_port_number (this->ssl_component_.port);
        }
    }

  return this->object_addr_;
}

ACE_INLINE void
TAO_SSLIOP_Endpoint::qop (Security::QOP q)
{
  this->qop_ = q;
}

ACE_INLINE Security::QOP
TAO_SSLIOP_Endpoint::qop (void) const
{
  return this->qop_;
}

ACE_INLINE void
TAO_SSLIOP_Endpoint::trust (const Security::EstablishTrust &t)
{
  this->trust_ = t;
}

ACE_INLINE Security::EstablishTrust
TAO_SSLIOP_Endpoint::trust (void) const
{
  return this->trust_;
}
