// -*- C++ -*-
// $Id$

ACE_INLINE const TAO_ObjectKey &
TAO_SHMIOP_Profile::object_key (void) const
{
  return this->object_key_;
}

ACE_INLINE TAO_ObjectKey *
TAO_SHMIOP_Profile::_key (void) const
{
  TAO_ObjectKey *key = 0;

  ACE_NEW_RETURN (key,
                  TAO_ObjectKey (this->object_key_),
                  0);

  return key;
}

ACE_INLINE const ACE_INET_Addr &
TAO_SHMIOP_Profile::object_addr (void) const
{
  // The object_addr_ is initialized here, rather than at IOR decode
  // time for several reasons:
  //   1. A request on the object may never be invoked.
  //   2. The DNS setup may have changed dynamically.
  //   ...etc..

  // We need to modify the object_addr_ in this method.  Do so using a
  // non-const copy of the <this> pointer.
  TAO_SHMIOP_Profile *profile =
    ACE_const_cast (TAO_SHMIOP_Profile *,
                    this);

  if (this->object_addr_.get_type () != AF_INET
      && profile->object_addr_.set (this->port_,
                                    this->host_.in ()) == -1)
    {
      // If this call fails, it most likely due a hostname lookup
      // failure caused by a DNS misconfiguration.  If a request is
      // made to the object at the given host and port, then a
      // CORBA::TRANSIENT() exception should be thrown.

      // Invalidate the ACE_INET_Addr.  This is used as a flag to
      // denote that ACE_INET_Addr initialization failed.
      profile->object_addr_.set_type (-1);
    }

  return this->object_addr_;
}

ACE_INLINE const char *
TAO_SHMIOP_Profile::host (void) const
{
  return this->host_.in ();
}

ACE_INLINE CORBA::UShort
TAO_SHMIOP_Profile::port (void) const
{
  return this->port_;
}

ACE_INLINE CORBA::UShort
TAO_SHMIOP_Profile::port (CORBA::UShort p)
{
  return this->port_ = p;
}

ACE_INLINE const TAO_GIOP_Version &
TAO_SHMIOP_Profile::version (void) const
{
  return this->version_;
}

ACE_INLINE TAO_SHMIOP_Client_Connection_Handler *&
TAO_SHMIOP_Profile::hint (void)
{
  return this->hint_;
}
