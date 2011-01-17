// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
TAO_Tagged_Profile::TAO_Tagged_Profile (TAO_ORB_Core *orb_core)
  : orb_core_ (orb_core),
    discriminator_ (0),
    object_key_extracted_ (false),
    object_key_ (),
    profile_ (),
    profile_index_ (0),
    type_id_ (0)
{

}

ACE_INLINE TAO::ObjectKey &
TAO_Tagged_Profile::object_key (void)
{
  if (!this->object_key_extracted_)
    this->object_key_extracted_ = this->extract_object_key (this->profile_);

  return this->object_key_;
}

ACE_INLINE void
TAO_Tagged_Profile::object_key (TAO::ObjectKey &object_key)
{
  this->object_key_.replace (object_key.length (),
                             object_key.length (),
                             object_key.get_buffer ());
  this->object_key_extracted_ = true;
}

ACE_INLINE const TAO::ObjectKey &
TAO_Tagged_Profile::object_key (void) const
{
  return const_cast<TAO_Tagged_Profile *> (this)->object_key ();
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

ACE_INLINE const char*
TAO_Tagged_Profile::type_id (void) const
{
  return this->type_id_ == 0 ? "" : this->type_id_;
}

ACE_INLINE CORBA::Short
TAO_Tagged_Profile::discriminator (void) const
{
  return this->discriminator_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
