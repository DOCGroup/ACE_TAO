// -*- C++ -*-
//
// $Id$


ACE_INLINE TAO_IIOP_Endpoint *
TAO_SSLIOP_Endpoint::iiop_endpoint (void) const
{
  return this->iiop_endpoint_;
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

  // We need to modify the object_addr_ in this method.  Do so using a
  // non-const copy of the <this> pointer.
  ACE_INET_Addr &ssl_addr =
    ACE_const_cast (ACE_INET_Addr &,
                    this->object_addr_);

  TAO_SSLIOP_Endpoint *ssl_endpoint =
    ACE_const_cast (TAO_SSLIOP_Endpoint *, this);

  // Double checked locking optimization.
  if (this->object_addr_.get_type () != AF_INET)
    {
      ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                        guard,
                        ssl_endpoint->addr_lookup_lock_,
                        this->object_addr_);

      if (this->object_addr_.get_type () != AF_INET)
        {
          ssl_addr = this->iiop_endpoint_->object_addr ();
          ssl_addr.set_port_number (this->ssl_component_.port);
        }
    }

  return this->object_addr_;
}
