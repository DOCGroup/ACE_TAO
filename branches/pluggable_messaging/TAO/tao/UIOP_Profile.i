// -*- C++ -*-
// $Id$


# if TAO_HAS_UIOP == 1

ACE_INLINE const TAO_ObjectKey &
TAO_UIOP_Profile::object_key (void) const
{
  return this->object_key_;
}

ACE_INLINE TAO_ObjectKey &
TAO_UIOP_Profile::object_key (TAO_ObjectKey& objkey)
{
  this->object_key_ = objkey;
  return this->object_key_;
}

ACE_INLINE TAO_ObjectKey *
TAO_UIOP_Profile::_key (void) const
{
  TAO_ObjectKey *key = 0;

  ACE_NEW_RETURN (key,
                  TAO_ObjectKey (this->object_key_),
                  0);

  return key;
}

ACE_INLINE const ACE_UNIX_Addr&
TAO_UIOP_Profile::object_addr (void) const
{
  return this->object_addr_;
}

ACE_INLINE const char *
TAO_UIOP_Profile::rendezvous_point (void) const
{
  return this->object_addr_.get_path_name ();
}

ACE_INLINE const TAO_GIOP_Version &
TAO_UIOP_Profile::version (void) const
{
  return this->version_;
}

ACE_INLINE TAO_UIOP_Client_Connection_Handler *&
TAO_UIOP_Profile::hint (void)
{
  return this->hint_;
}

#endif  /* TAO_HAS_UIOP == 1 */
