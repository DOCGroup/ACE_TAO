// -*- C++ -*-
// $Id$

ACE_INLINE const TAO_ObjectKey &
TAO_IIOP_Profile::object_key (void) const
{
  return this->object_key_;
}

ACE_INLINE TAO_ObjectKey &
TAO_IIOP_Profile::object_key (TAO_ObjectKey& objkey)
{
  this->object_key_ = objkey;
  return this->object_key_;
}

ACE_INLINE TAO_ObjectKey *
TAO_IIOP_Profile::_key (void) const
{
  TAO_ObjectKey *key = 0;

  ACE_NEW_RETURN (key,
                  TAO_ObjectKey (this->object_key_),
                  0);

  return key;
}

ACE_INLINE const ACE_INET_Addr&
TAO_IIOP_Profile::object_addr (void) const
{
  return this->object_addr_;
}

ACE_INLINE const char *
TAO_IIOP_Profile::host (void)
{
  return this->host_.in ();
}

ACE_INLINE CORBA::UShort
TAO_IIOP_Profile::port (void)
{
  return this->port_;
}

ACE_INLINE CORBA::UShort
TAO_IIOP_Profile::port (CORBA::UShort p)
{
  return this->port_ = p;
}

ACE_INLINE const TAO_GIOP_Version &
TAO_IIOP_Profile::version (void) const
{
  return this->version_;
}

ACE_INLINE TAO_IIOP_Client_Connection_Handler *&
TAO_IIOP_Profile::hint(void)
{
  return this->hint_;
}

ACE_INLINE const TAO_Tagged_Components&
TAO_IIOP_Profile::tagged_components (void) const
{
  return this->tagged_components_;
}

ACE_INLINE TAO_Tagged_Components&
TAO_IIOP_Profile::tagged_components (void)
{
  return this->tagged_components_;
}
