// $Id$

ACE_INLINE CORBA::ULong
TAO_IIOP_Profile::tag (void) const
{
  return this->tag_;
}

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
TAO_IIOP_Profile::_key (CORBA::Environment &)
{
  return new TAO_ObjectKey (this->object_key_);
}

ACE_INLINE const TAO_opaque&
TAO_IIOP_Profile::body (void) const
{
  return this->body_;
}

ACE_INLINE const ACE_INET_Addr&
TAO_IIOP_Profile::object_addr (void) const
{
  return this->object_addr_;
}

ACE_INLINE const char *
TAO_IIOP_Profile::host (void)
{
  return this->host_;
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

ACE_INLINE const TAO_IOP_Version *
TAO_IIOP_Profile::version (void)
{
  return &this->version_;
}

ACE_INLINE const TAO_IOP_Version *
TAO_IIOP_Profile::version (TAO_IOP_Version *v)
{
  this->version_ = *v;
  return &this->version_;
}

ACE_INLINE TAO_Client_Connection_Handler *&
TAO_IIOP_Profile::hint(void)
{
  return this->hint_;
}

ACE_INLINE TAO_Profile *
TAO_IIOP_Profile::_nil (void)
{
  return (TAO_IIOP_Profile *)0;
}
