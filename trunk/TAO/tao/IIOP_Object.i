// $Id$

// @@ Get rid of profile specific stuff, it is now in it's own class and
// file. fredk

ACE_INLINE
IIOP_Object::~IIOP_Object (void)
{
  // Cleanup hint
  if (profile_in_use_ != 0        &&
      profile_in_use_ != profile_ &&
      profile_in_use_ != fwd_profile_)
  {
    this->profile_in_use_->reset_hint ();
    delete this->profile_in_use_;
  }

  this->profile_in_use_ = 0;

  if (this->profile_ != 0) 
  {
    this->profile_->reset_hint ();
    delete this->profile_;
    this->profile_ = 0;
  }

  if (this->fwd_profile_)
  {
    this->fwd_profile_ ->reset_hint ();
    delete this->fwd_profile_;
    this->fwd_profile_ = 0;
  }

  assert (this->refcount_ == 0);
}

ACE_INLINE
IIOP_Object::IIOP_Object (char *repository_id)
  : STUB_Object (repository_id),
    profile_ (0),
    profile_in_use_ (0),
    fwd_profile_ (0),
    fwd_profile_lock_ptr_ (0),
    fwd_profile_success_ (0),
    // what about ACE_SYNCH_MUTEX refcount_lock_
    refcount_ (1),
    use_locate_request_ (0),
    first_locate_request_ (0)
{
  this->profile_ = this->profile_in_use_ = new TAO_IIOP_Profile;
  this->fwd_profile_lock_ptr_ = 
    TAO_ORB_Core_instance ()->client_factory ()->create_iiop_profile_lock ();  
}

ACE_INLINE
IIOP_Object::IIOP_Object (char *repository_id,
                          const TAO_Profile &profile)
  : STUB_Object (repository_id),
    profile_ (0),
    profile_in_use_ (0),
    fwd_profile_ (0),
    fwd_profile_lock_ptr_ (0),
    fwd_profile_success_ (0),
    // what about ACE_SYNCH_MUTEX refcount_lock_
    refcount_ (1),
    use_locate_request_ (0),
    first_locate_request_ (0)
{
  // @@ XXX need to verify type and deal with wrong types
  const TAO_IIOP_Profile &pfile = ACE_dynamic_cast (const TAO_IIOP_Profile &, profile);
  this->profile_ = this->profile_in_use_ = new TAO_IIOP_Profile (pfile);
  this->fwd_profile_lock_ptr_ =  
    TAO_ORB_Core_instance ()->client_factory ()->create_iiop_profile_lock ();  
}

ACE_INLINE
IIOP_Object::IIOP_Object (char *repository_id,
                          const TAO_Profile *profile)
  : STUB_Object (repository_id),
    profile_ (0),
    profile_in_use_ (0),
    fwd_profile_ (0),
    fwd_profile_lock_ptr_ (0),
    fwd_profile_success_ (0),
    // what about ACE_SYNCH_MUTEX refcount_lock_
    refcount_ (1),
    use_locate_request_ (0),
    first_locate_request_ (0)
{
  // @@ XXX need to verify type and deal with wrong types
  const TAO_IIOP_Profile *pfile = ACE_dynamic_cast (const TAO_IIOP_Profile *, profile);
  this->profile_ = this->profile_in_use_ = new TAO_IIOP_Profile (pfile);
  this->fwd_profile_lock_ptr_ =  
    TAO_ORB_Core_instance ()->client_factory ()->create_iiop_profile_lock ();  
}

ACE_INLINE
TAO_Profile *
IIOP_Object::profile_in_use (void)
{
  return this->profile_in_use_;
}

ACE_INLINE
TAO_Profile *
IIOP_Object::set_profile_in_use (TAO_Profile *pfile)
{
  if (pfile->tag () == TAO_IOP_TAG_INTERNET_IOP)
  {
    TAO_IIOP_Profile *p =
            ACE_dynamic_cast (TAO_IIOP_Profile *, pfile);
    return (this->profile_in_use_ = p);
  } else {
    return 0;
  }
}

ACE_INLINE
TAO_Profile *
IIOP_Object::set_profile_in_use (void)
{
  return this->profile_in_use_ = this->profile_;
}

ACE_INLINE
TAO_Profile *
IIOP_Object::get_profile (void)
{
  // @@ should verify type ... but the STUB object needs redoing anyway
  //    for multiple protocols so willdeal with it then.
  if (profile_in_use_)
    return new TAO_IIOP_Profile (ACE_dynamic_cast(TAO_IIOP_Profile *, profile_in_use_));
  else
    return 0;
}

ACE_INLINE
TAO_Profile *
IIOP_Object::get_fwd_profile_i (void)
{
  return this->fwd_profile_;
}

ACE_INLINE
TAO_Profile *
IIOP_Object::get_fwd_profile (void)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_Lock, 
                            guard, 
                            *this->fwd_profile_lock_ptr_, 
                            0));
  return this->get_fwd_profile_i ();
}


// set_fwd_profile is currently called with either an arg of NULL
// or with a pointer to another IIOP_Object's profile_in_use_.
ACE_INLINE
TAO_Profile *
IIOP_Object::set_fwd_profile (const TAO_Profile *new_profile)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_Lock, 
                            guard, 
                            *this->fwd_profile_lock_ptr_, 
                            0));
  TAO_Profile *old = this->fwd_profile_;
  if (new_profile != 0)
    {
      delete this->fwd_profile_;
      this->fwd_profile_ = 0;
      // @@ HACK, need to verify type here!
      const TAO_IIOP_Profile *iiop_pfile = 
                       ACE_dynamic_cast(const TAO_IIOP_Profile *, new_profile);
      ACE_NEW_RETURN (this->fwd_profile_,
                      TAO_IIOP_Profile(iiop_pfile),
                      0);
      // use the copy constructor!
    }
  return old;
}

ACE_INLINE
ACE_Lock &
IIOP_Object::get_fwd_profile_lock (void)
{
  return *this->fwd_profile_lock_ptr_;
}

ACE_INLINE
void 
IIOP_Object::reset_first_locate_request (void)
{
  first_locate_request_ = 1;
}

ACE_INLINE
void 
IIOP_Object::use_locate_requests (CORBA::Boolean use_it)
{
  if (use_it)
    {
      this->first_locate_request_ = 1;
      this->use_locate_request_ = 1;
    }
  else 
    {
      // Don't use it.
      this->first_locate_request_ = 0;
      this->use_locate_request_ = 0;
    }   
} 
