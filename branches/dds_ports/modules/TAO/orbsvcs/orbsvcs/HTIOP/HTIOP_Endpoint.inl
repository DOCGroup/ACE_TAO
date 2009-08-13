// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE const ACE::HTBP::Addr &
TAO::HTIOP::Endpoint::object_addr (void) const
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
      // using a non-const copy of the <this> pointer.
      TAO::HTIOP::Endpoint *endpoint =
        const_cast<TAO::HTIOP::Endpoint *> (this);

      ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                        guard,
                        endpoint->addr_lookup_lock_,
                        this->object_addr_ );

      if (!this->object_addr_set_)
        {
          if (endpoint->object_addr_.set (this->port_,
                                          this->host_.in (),
                                          this->htid_.in ()) == -1)
            {
              // If this call fails, it most likely due a hostname
              // lookup failure caused by a DNS misconfiguration.  If
              // a request is made to the object at the given host and
              // port, then a CORBA::TRANSIENT() exception should be
              // thrown.

              // Invalidate the ACE::HTBP::Addr.  This is used as a flag
              // to denote that ACE::HTBP::Addr initialization failed.
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

ACE_INLINE const char *
TAO::HTIOP::Endpoint::host (void) const
{
  return this->host_.in ();
}

ACE_INLINE const char *
TAO::HTIOP::Endpoint::htid (void) const
{
  return this->htid_.in ();
}

ACE_INLINE CORBA::UShort
TAO::HTIOP::Endpoint::port (void) const
{
  return this->port_;
}

ACE_INLINE CORBA::UShort
TAO::HTIOP::Endpoint::port (CORBA::UShort p)
{
  return this->port_ = p;
}

TAO_END_VERSIONED_NAMESPACE_DECL
