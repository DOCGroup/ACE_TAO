// -*- C++ -*-
// $Id$

ACE_INLINE
TAO_Tagged_Profile::TAO_Tagged_Profile (TAO_ORB_Core *orb_core)
  : orb_core_ (orb_core),
    discriminator_ (0),
    object_key_ (),
    profile_ (),
    profile_index_ (0),
    type_id_ ()
{

}


ACE_INLINE TAO_ObjectKey &
TAO_Tagged_Profile::object_key (void)
{
  if (this->object_key_extracted_ == 0)
    this->object_key_extracted_ = this->extract_object_key (this->profile_);

  return this->object_key_;
}

ACE_INLINE void
TAO_Tagged_Profile::object_key (TAO_ObjectKey &object_key)
{
  this->object_key_.replace (object_key.length (),
                             object_key.length (),
                             object_key.get_buffer ());
  this->object_key_extracted_ = 1;
}

ACE_INLINE const TAO_ObjectKey &
TAO_Tagged_Profile::object_key (void) const
{
  return ACE_const_cast(TAO_Tagged_Profile *, this)->object_key ();
}

ACE_INLINE const IOP::TaggedProfile &
TAO_Tagged_Profile::tagged_profile (void) const
{
  return this->profile_;
}

ACE_INLINE CORBA::ULong
TAO_Tagged_Profile::profile_index (void) const
{
  return this->profile_index_;
}

ACE_INLINE const ACE_CString &
TAO_Tagged_Profile::type_id (void) const
{
  return this->type_id_;
}

ACE_INLINE CORBA::Short
TAO_Tagged_Profile::discriminator (void) const
{
  return this->discriminator_;
}
