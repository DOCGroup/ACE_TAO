// -*- C++ -*-
//
// $Id$

ACE_INLINE const ACE_INET_Addr &
TAO_IIOP_Endpoint::object_addr (void) const
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
                        this->object_addr_);

      if (!this->object_addr_set_)
        {
          if (this->object_addr_.set (this->port_,
                                      this->host_.in ()) == -1)
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
              this->object_addr_set_ = 1;
            }
        }
    }

  return this->object_addr_;
}

ACE_INLINE const char *
TAO_IIOP_Endpoint::host (void) const
{
  return this->host_.in ();
}

ACE_INLINE CORBA::UShort
TAO_IIOP_Endpoint::port (void) const
{
  return this->port_;
}

ACE_INLINE CORBA::UShort
TAO_IIOP_Endpoint::port (CORBA::UShort p)
{
  return this->port_ = p;
}
