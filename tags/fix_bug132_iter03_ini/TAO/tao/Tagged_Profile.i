// -*- C++ -*-
// $Id$

ACE_INLINE
TAO_Tagged_Profile::TAO_Tagged_Profile (TAO_ORB_Core *orb_core)
  : orb_core_ (orb_core),
    object_key_ (),
    profile_ (),
    profile_index_ (0),
    type_id_ ()
{

}


ACE_INLINE TAO_ObjectKey &
TAO_Tagged_Profile::object_key (void)
{
  return this->object_key_;
}

ACE_INLINE const TAO_ObjectKey &
TAO_Tagged_Profile::object_key (void) const
{
  return this->object_key_;
}

ACE_INLINE void
TAO_Tagged_Profile::object_key (TAO_ObjectKey &object_key)
{
  this->object_key_.replace (object_key.length (),
                             object_key.length (),
                             object_key.get_buffer ());

}

ACE_INLINE IOP::TaggedProfile &
TAO_Tagged_Profile::tagged_profile (void)
{
  return this->profile_;
}

ACE_INLINE CORBA::ULong
TAO_Tagged_Profile::profile_index (void)
{
  return this->profile_index_;
}
ACE_INLINE void
TAO_Tagged_Profile::profile_index (CORBA::ULong ind)
{
  this->profile_index_ = ind;
}

ACE_INLINE ACE_CString &
TAO_Tagged_Profile::type_id (void)
{
  return this->type_id_;
}
