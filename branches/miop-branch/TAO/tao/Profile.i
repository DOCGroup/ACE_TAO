// -*- C++ -*-
// $Id$

ACE_INLINE
TAO_Profile::TAO_Profile (CORBA::ULong tag,
                          TAO_ORB_Core *orb_core,
                          const TAO_GIOP_Message_Version &version)
  : version_ (version),
    are_policies_parsed_ (0),
    stub_ (0),
    policy_list_ (0),
    tag_ (tag),
    orb_core_ (orb_core),
    forward_to_ (0),
    addressing_mode_ (0),
    refcount_ (1)
{
}

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

ACE_INLINE CORBA::ULong
TAO_Profile::_incr_refcnt (void)
{
  // OK, think I got it.  When this object is created (guard) the
  // lock is automatically acquired (refcount_lock_).  Then when
  // we leave this method the destructir for guard is called which
  // releases the lock!
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, guard, this->refcount_lock_, 0);

  return this->refcount_++;
}

ACE_INLINE CORBA::ULong
TAO_Profile::_decr_refcnt (void)
{
  {
    ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, mon, this->refcount_lock_, 0);
    this->refcount_--;
    if (this->refcount_ != 0)
      return this->refcount_;
  }

  // refcount is 0, so delete us!
  // delete will call our ~ destructor which in turn deletes stuff.
  delete this;
  return 0;
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
