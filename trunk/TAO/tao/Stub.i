// -*- C++ -*-
// $Id$

// @@ Get rid of profile specific stuff, it is now in it's own class and
// file. fredk
#include <tao/debug.h>

ACE_INLINE TAO_Profile *
TAO_Stub::set_profile_in_use_i (TAO_Profile *pfile)
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

ACE_INLINE void
TAO_Stub::reset_first_locate_request (void)
{
  first_locate_request_ = 1;
}

ACE_INLINE void
TAO_Stub::reset_base (void)
{
  this->base_profiles_.rewind ();
  reset_first_locate_request ();
  profile_success_ = 0;

  set_profile_in_use_i (base_profiles_.get_next ());
}

ACE_INLINE void
TAO_Stub::forward_back_one (void)
{
  TAO_MProfile *from = forward_profiles_->forward_from ();

  delete forward_profiles_;

  // the current profile in this profile list is no
  // longer being forwarded, so set the reference to zero.
  if (from == &base_profiles_)
    {
      base_profiles_.get_current_profile ()->forward_to (0);
      forward_profiles_ = 0;
    }
  else
    {
      from->get_current_profile ()->forward_to (0);
      forward_profiles_ = from;
    }

}

ACE_INLINE void
TAO_Stub::reset_forward (void)
{
  while (this->forward_profiles_ != 0)
    forward_back_one ();
}

ACE_INLINE void
TAO_Stub::reset_profiles_i (void)
{
  reset_forward ();
  reset_base ();
}

ACE_INLINE void
TAO_Stub::reset_profiles (void)
{
  ACE_MT (ACE_GUARD (ACE_Lock,
                     guard,
                     *this->profile_lock_ptr_));
  reset_profiles_i ();
}

ACE_INLINE
TAO_Stub::~TAO_Stub (void)
{
  assert (this->refcount_ == 0);

  if (forward_profiles_)
    reset_profiles ();

  if (this->profile_in_use_ != 0)
    {
      this->profile_in_use_->reset_hint ();
      // decrease reference count on profile
      this->profile_in_use_->_decr_refcnt ();
      this->profile_in_use_ = 0;
    }

  if (this->profile_lock_ptr_)
    delete this->profile_lock_ptr_;

}

ACE_INLINE TAO_Profile *
TAO_Stub::profile_in_use (void)
{
  return this->profile_in_use_;
}

ACE_INLINE void
TAO_Stub::use_locate_requests (CORBA::Boolean use_it)
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

ACE_INLINE TAO_MProfile *
TAO_Stub::get_profiles (void)
{
  return new TAO_MProfile (base_profiles_);
}

ACE_INLINE TAO_Profile *
TAO_Stub::next_forward_profile (void)
{
  TAO_Profile *pfile_next = 0;

  while (this->forward_profiles_
         && (pfile_next = forward_profiles_->get_next ()) == 0)
    // that was the last profile.  Now we clean up our forward profiles.
    // since we own the forward MProfiles, we must delete them when done.
    forward_back_one ();

  return pfile_next;
}

ACE_INLINE TAO_Profile *
TAO_Stub::next_profile_i (void)
{

  TAO_Profile *pfile_next = 0;
  if (forward_profiles_)
    {
      pfile_next = next_forward_profile ();
      if (pfile_next == 0)
        pfile_next = base_profiles_.get_next ();
    }
  else
    pfile_next = base_profiles_.get_next ();

  if (pfile_next == 0)
    reset_base ();
  else
    set_profile_in_use_i (pfile_next);

  return pfile_next;
}

ACE_INLINE TAO_Profile *
TAO_Stub::next_profile (void)
{

  ACE_MT (ACE_GUARD_RETURN (ACE_Lock,
                            guard,
                            *this->profile_lock_ptr_,
                            0));
  return next_profile_i ();
}

ACE_INLINE CORBA::Boolean
TAO_Stub::valid_forward_profile (void)
{
  return (profile_success_ && forward_profiles_);
}

ACE_INLINE void
TAO_Stub::set_valid_profile (void)
{
  profile_success_ = 1;
}

ACE_INLINE CORBA::Boolean
TAO_Stub::valid_profile (void)
{
  return profile_success_;
}

ACE_INLINE TAO_Profile *
TAO_Stub::set_base_profiles (const TAO_MProfile &mprofiles)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_Lock,
                            guard,
                            *this->profile_lock_ptr_,
                            0));

  // first reset things so we start from scratch!
  reset_forward ();
  base_profiles_.set (mprofiles);
  reset_base ();
  return profile_in_use_;

}

ACE_INLINE CORBA::Boolean
TAO_Stub::next_profile_retry (void)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_Lock,
                            guard,
                            *this->profile_lock_ptr_,
                            0));

  if (profile_success_ && forward_profiles_)
    {
      reset_profiles_i ();
      return 1;
    }
  else if (next_profile_i ())
    {
      return 1;
    }
  else
    {
      reset_profiles_i ();
      return 0;
    }
}

ACE_INLINE const TAO_MProfile&
TAO_Stub::get_base_profiles (void) const
{
  return this->base_profiles_;
}

ACE_INLINE TAO_ORB_Core*
TAO_Stub::orb_core (void) const
{
  return this->orb_core_;
}

ACE_INLINE CORBA::ORB_ptr
TAO_Stub::servant_orb (void)
{
  // Simply pass back the ORB pointer for temporary use.
  return this->servant_orb_.ptr ();
}

ACE_INLINE void
TAO_Stub::servant_orb (CORBA::ORB_ptr orb)
{
  this->servant_orb_ = orb;
}

// Creator methods for TAO_Stub_Auto_Ptr (TAO_Stub Auto Pointer)
ACE_INLINE
TAO_Stub_Auto_Ptr::TAO_Stub_Auto_Ptr (TAO_Stub *p)
  : p_ (p)
{
  ACE_TRACE ("TAO_Stub_Auto_Ptr::TAO_Stub_Auto_Ptr");
}

ACE_INLINE TAO_Stub *
TAO_Stub_Auto_Ptr::get (void) const
{
  ACE_TRACE ("TAO_Stub_Auto_Ptr::get");
  return this->p_;
}

ACE_INLINE TAO_Stub *
TAO_Stub_Auto_Ptr::release (void)
{
  ACE_TRACE ("TAO_Stub_Auto_Ptr::release");
  TAO_Stub *old = this->p_;
  this->p_ = 0;
  return old;
}

ACE_INLINE void
TAO_Stub_Auto_Ptr::reset (TAO_Stub *p)
{
  ACE_TRACE ("TAO_Stub_Auto_Ptr::reset");
  if (this->get () != p)
    this->get ()->_decr_refcnt ();
  this->p_ = p;
}

ACE_INLINE TAO_Stub *
TAO_Stub_Auto_Ptr::operator-> () const
{
  ACE_TRACE ("auto_ptr::operator->");
  return this->get ();
}

ACE_INLINE
TAO_Stub_Auto_Ptr::TAO_Stub_Auto_Ptr (TAO_Stub_Auto_Ptr &rhs)
  : p_ (rhs.release ())
{
  ACE_TRACE ("TAO_Stub_Auto_Ptr::TAO_Stub_Auto_Ptr");
}

ACE_INLINE TAO_Stub_Auto_Ptr &
TAO_Stub_Auto_Ptr::operator= (TAO_Stub_Auto_Ptr &rhs)
{
  ACE_TRACE ("TAO_Stub_Auto_Ptr::operator=");
  if (this != &rhs)
    {
      this->reset (rhs.release ());
    }
  return *this;
}

ACE_INLINE
TAO_Stub_Auto_Ptr::~TAO_Stub_Auto_Ptr (void)
{
  ACE_TRACE ("TAO_Stub_Auto_Ptr::~TAO_Stub_Auto_Ptr");
  this->get ()->_decr_refcnt ();
}

// Accessor methods to the underlying Stub Object

ACE_INLINE TAO_Stub &
TAO_Stub_Auto_Ptr::operator *() const
{
  ACE_TRACE ("TAO_Stub_Auto_Ptr::operator *()");
  return *this->get ();
}
