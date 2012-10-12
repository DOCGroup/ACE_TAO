// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE CORBA::ULong
TAO_Profile::tag (void) const
{
  return this->tag_;
}

ACE_INLINE const TAO_GIOP_Message_Version &
TAO_Profile::version (void) const
{
  return this->version_;
}

ACE_INLINE TAO_ORB_Core *
TAO_Profile::orb_core (void) const
{
  return this->orb_core_;
}

ACE_INLINE void
TAO_Profile::forward_to (TAO_MProfile *mprofiles)
{
  this->forward_to_ = mprofiles;
}

ACE_INLINE TAO_MProfile *
TAO_Profile::forward_to (void)
{
  return this->forward_to_;
}

ACE_INLINE TAO_MProfile *
TAO_Profile::forward_to_i (void)
{
  return this->forward_to_;
}

ACE_INLINE const TAO_Tagged_Components&
TAO_Profile::tagged_components (void) const
{
  return this->tagged_components_;
}

ACE_INLINE TAO_Tagged_Components&
TAO_Profile::tagged_components (void)
{
  return this->tagged_components_;
}

ACE_INLINE CORBA::Short
TAO_Profile::addressing_mode (void) const
{
  return this->addressing_mode_;
}

ACE_INLINE const TAO::ObjectKey &
TAO_Profile::object_key (void) const
{
  return this->ref_object_key_->object_key ();
}

ACE_INLINE unsigned long
TAO_Profile::_incr_refcnt (void)
{
  return ++this->refcount_;
}

ACE_INLINE unsigned long
TAO_Profile::_decr_refcnt (void)
{
  unsigned long count = --this->refcount_;
  if (count == 0)
    {
      // refcount is 0, so delete us!
      // delete will call our ~ destructor which in turn deletes stuff.
      delete this;
    }
  return count;
}



TAO_END_VERSIONED_NAMESPACE_DECL
