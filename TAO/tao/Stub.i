// $Id$

// @@ Get rid of profile specific stuff, it is now in it's own class and
// file. fredk
#include <tao/debug.h>

ACE_INLINE
STUB_Object::~STUB_Object (void)
{
  assert (this->refcount_ == 0);

  // Cleanup hint

  if (this->profile_in_use_ != 0) 
  {
    this->profile_in_use_->reset_hint ();
    // decrease reference count on profile
    this->profile_in_use_->_decr_refcnt ();   
    this->profile_in_use_ = 0;
  }

  if (this->fwd_profile_)
  {
    this->fwd_profile_->reset_hint ();
    this->fwd_profile_->_decr_refcnt ();
    this->fwd_profile_ = 0;
  }

  if (fwd_profiles_)
    delete fwd_profiles_;

  if (this->profile_lock_ptr_)
    delete this->profile_lock_ptr_; 

}

ACE_INLINE
TAO_Profile *
STUB_Object::set_profile_in_use_i (TAO_Profile *pfile)
{
  TAO_Profile *old = this->profile_in_use_;

  // Since we are actively using this profile we dont want
  // it to disappear, so increase the reference count by one!!
  if (pfile && (pfile->_incr_refcnt () == 0))
  {
    ACE_ERROR_RETURN ((LM_ERROR,
                      "(%P|%t) unable to increment profile ref!\n"),
                      0);
  }

  this->profile_in_use_ = pfile;

  if (old)
    old->_decr_refcnt ();

  return this->profile_in_use_;
}

ACE_INLINE
TAO_Profile * 
STUB_Object::set_profiles (TAO_MProfile *mprofiles)
{

  ACE_MT (ACE_GUARD_RETURN (ACE_Lock, 
                            guard, 
                            *this->profile_lock_ptr_, 
                            0));

  base_profiles_.set (mprofiles);

  return this->set_profile_in_use_i (this->base_profiles_.get_next ());  

}

ACE_INLINE
TAO_Profile *
STUB_Object::profile_in_use (void)
{
  return this->profile_in_use_;
}

ACE_INLINE
TAO_Profile *
STUB_Object::set_profile_in_use (TAO_Profile *pfile)
{

  ACE_MT (ACE_GUARD_RETURN (ACE_Lock, 
                            guard, 
                            *this->profile_lock_ptr_, 
                            0));

  return set_profile_in_use_i (pfile);

}


ACE_INLINE
TAO_Profile *
STUB_Object::get_fwd_profile_i (void)
{
  return this->fwd_profile_;
}

ACE_INLINE
TAO_Profile *
STUB_Object::get_fwd_profile (void)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_Lock, 
                            guard, 
                            *this->profile_lock_ptr_, 
                            0));

  return this->get_fwd_profile_i ();
}


// set_fwd_profile is currently called with either an arg of NULL
// or with a pointer to another STUB_Object's profile_in_use_.
// Change in strategy from previous version:  Since we assume this 
// forward profile comes from am MProfile list, we just increment the
// reference count on the profile.  We do not copy it and we assume
// the user will not modify!
ACE_INLINE
TAO_Profile *
STUB_Object::set_fwd_profile (TAO_Profile *new_profile)
{
 
  TAO_Profile *old = this->fwd_profile_;

  // show that we are using this!
  // @@ could be an inline => reduce code, common
  if (new_profile && (new_profile->_incr_refcnt () == 0))
  {
    ACE_ERROR_RETURN ((LM_ERROR, 
                       "(%P|%t) unable to increment profile ref!\n"), 
                       0);
  }

  this->fwd_profile_ = new_profile;

  if (old) 
    old->_decr_refcnt ();

  return this->fwd_profile_;
}

ACE_INLINE
ACE_Lock &
STUB_Object::get_profile_lock (void)
{
  return *this->profile_lock_ptr_;
}

ACE_INLINE
void 
STUB_Object::reset_first_locate_request (void)
{
  first_locate_request_ = 1;
}

ACE_INLINE
void 
STUB_Object::use_locate_requests (CORBA::Boolean use_it)
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

ACE_INLINE
TAO_MProfile * 
STUB_Object::get_profiles (void)
{
  return new TAO_MProfile (&base_profiles_);
}

ACE_INLINE
void
STUB_Object::reset_fwd_profiles (void)
{

  if (fwd_profiles_)
  {
    // @@ Assume on one level deep FRED
    TAO_MProfile *old = fwd_profiles_;
    TAO_MProfile *prev = old->fwded_mprofile ();
    // this should be base_profiles_
    if (prev->get_current_profile ())
      prev->get_current_profile ()->fwd_profiles (0);
      // it better be!  this should be profile_in_use_
  }
    
  set_fwd_profile (0);
}

ACE_INLINE
TAO_Profile *
STUB_Object::reset_profiles (void)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_Lock, 
                            guard, 
                            *this->profile_lock_ptr_, 
                            0));

  // first get rid of any forwarding profile list and fwd_profile
  reset_fwd_profiles ();

  // then rewind profile list, back to the beginning
  this->base_profiles_.rewind ();

  reset_first_locate_request ();
  // resets the flag of the first call locate request to true
  
  return this->set_profile_in_use_i (this->base_profiles_.get_next ());
}

ACE_INLINE
TAO_Profile *
STUB_Object::next_profile (void)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_Lock, 
                            guard, 
                            *this->profile_lock_ptr_, 
                            0));

  return this->set_profile_in_use_i (this->base_profiles_.get_next ());
}

ACE_INLINE
TAO_MProfile * 
STUB_Object::get_fwd_profiles (void)
{
  if (fwd_profiles_)
    return new TAO_MProfile (this->fwd_profiles_);
  return 0;
}

ACE_INLINE
void
STUB_Object::set_fwd_profiles (TAO_MProfile *mprofiles)
{

  if (! profile_in_use_)
    return ;

  // profile_in_use_ will copy the object but delegate
  // to this Stub_Obj responsibility for manipulating list!
  TAO_MProfile *tmp = new TAO_MProfile (mprofiles);

  // now add a pointer back the the profile list hat had been 
  // forwarded.  Note, since the profile list is like
  // a state machine, the current poitner refers to the
  // actual profile that was forwarded!

  {
    ACE_MT (ACE_GUARD (ACE_Lock, 
                       guard, 
                       *this->profile_lock_ptr_));
  
    if (this->fwd_profiles_)
    {
      if (TAO_orbdebug)
        ACE_DEBUG ((LM_DEBUG, "** Overwriting fwd profiles!\n"));
      TAO_MProfile *old = this->fwd_profiles_;
      TAO_MProfile *prev = old->fwded_mprofile ();
      if (prev->get_current_profile ())
        prev->get_current_profile ()->fwd_profiles (0);
      // @@ FRED: UGLY but for now just one level!
      delete this->fwd_profiles_;
    }
      
    this->fwd_profiles_ = tmp;
  
    // @@ if (this->fwd_profiles_)
    // @@ // add a link in the chain!
    // @@ tmp->fwded_mprofile (this->fwd_profiles_);

    this->fwd_profiles_->fwded_mprofile (&this->base_profiles_);
    profile_in_use_->fwd_profiles (this->fwd_profiles_);

    set_fwd_profile ( this->fwd_profiles_->get_next ());
  }
 
}

ACE_INLINE
TAO_Profile *
STUB_Object::next_fwd_profile (void)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_Lock, 
                            guard, 
                            *this->profile_lock_ptr_, 
                            0));

  if (fwd_profiles_)
    return this->set_fwd_profile (this->fwd_profiles_->get_next ());

  return this->set_fwd_profile (0);
}

