// -*- C++ -*-
// $Id$

ACE_INLINE const ACE_INET_Addr &
TAO_SHMIOP_Endpoint::object_addr (void) const
{
  // The object_addr_ is initialized here, rather than at IOR decode
  // time for several reasons:
  //   1. A request on the object may never be invoked.
  //   2. The DNS setup may have changed dynamically.
  //   ...etc..

  // We need to modify the object_addr_ in this method.  Do so using a
  // non-const copy of the <this> pointer.
  TAO_SHMIOP_Endpoint *endpoint =
    ACE_const_cast (TAO_SHMIOP_Endpoint *,
                    this);

  if (this->object_addr_.get_type () != AF_INET
      && endpoint->object_addr_.set (this->port_,
                                    this->host_.in ()) == -1)
    {
      // If this call fails, it most likely due a hostname lookup
      // failure caused by a DNS misconfiguration.  If a request is
      // made to the object at the given host and port, then a
      // CORBA::TRANSIENT() exception should be thrown.

      // Invalidate the ACE_INET_Addr.  This is used as a flag to
      // denote that ACE_INET_Addr initialization failed.
      endpoint->object_addr_.set_type (-1);
    }

  return this->object_addr_;
}

ACE_INLINE const char *
TAO_SHMIOP_Endpoint::host (void) const
{
  return this->host_.in ();
}

ACE_INLINE CORBA::UShort
TAO_SHMIOP_Endpoint::port (void) const
{
  return this->port_;
}

ACE_INLINE CORBA::UShort
TAO_SHMIOP_Endpoint::port (CORBA::UShort p)
{
  return this->port_ = p;
}

ACE_INLINE TAO_SHMIOP_Client_Connection_Handler *&
TAO_SHMIOP_Endpoint::hint (void)
{
  return this->hint_;
}

ACE_INLINE CORBA::ULong
TAO_SHMIOP_Endpoint::hash (void)
{
  return
    ACE::hash_pjw (this->host_.in ())
    + this->port_;
}

ACE_INLINE CORBA::Boolean
TAO_SHMIOP_Endpoint::is_equivalent (const TAO_SHMIOP_Endpoint *other_endpoint)
{
  return
    this->port_ == other_endpoint->port_
    && ACE_OS::strcmp (this->host_.in (), other_endpoint->host_.in ()) == 0;
}
