// -*- C++ -*-
// $Id$


ACE_INLINE TAO_Profile *
TAO_Stub::set_profile_in_use_i (TAO_Profile *pfile)
{
  TAO_Profile *old = this->profile_in_use_;

  // Since we are actively using this profile we dont want
  // it to disappear, so increase the reference count by one!!
  if (pfile && (pfile->_incr_refcnt () == 0))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                        ACE_TEXT ("(%P|%t) unable to increment profile ref!\n")),
                        0);
    }

  this->profile_in_use_ = pfile;

  if (old)
    old->_decr_refcnt ();

  return this->profile_in_use_;
}

ACE_INLINE void
TAO_Stub::reset_base (void)
{
  this->base_profiles_.rewind ();
  this->profile_success_ = 0;

  this->set_profile_in_use_i (base_profiles_.get_next ());
}


ACE_INLINE ACE_Lock*
TAO_Stub::profile_lock (void) const
{
  return this->profile_lock_ptr_;
}

ACE_INLINE CORBA::Boolean
TAO_Stub::service_profile_selection (void)
{
  // @@todo: This method is depreacted. Needs to be removed after
  // 1.2.1 goes out.
  TAO_Profile *profile = 0;

  this->orb_core_->service_profile_selection (this->base_profiles_,
                                              profile);
  if (profile)
    {
      this->set_profile_in_use_i (profile);
      return 1;
    }

  return 0;
}

ACE_INLINE void
TAO_Stub::forward_back_one (void)
{
  TAO_MProfile *from = forward_profiles_->forward_from ();

  delete this->forward_profiles_;

  // the current profile in this profile list is no
  // longer being forwarded, so set the reference to zero.
  if (from == &this->base_profiles_)
    {
      this->base_profiles_.get_current_profile ()->forward_to (0);
      this->forward_profiles_ = 0;
    }
  else
    {
      from->get_current_profile ()->forward_to (0);
      this->forward_profiles_ = from;
    }

}

ACE_INLINE void
TAO_Stub::reset_forward (void)
{
  while (this->forward_profiles_ != 0)
    this->forward_back_one ();
}

ACE_INLINE void
TAO_Stub::reset_profiles_i (void)
{
  this->reset_forward ();
  this->reset_base ();
}

ACE_INLINE void
TAO_Stub::reset_profiles (void)
{
  ACE_MT (ACE_GUARD (ACE_Lock,
                     guard,
                     *this->profile_lock_ptr_));
  this->reset_profiles_i ();
}

ACE_INLINE TAO_Profile *
TAO_Stub::profile_in_use (void)
{
  return this->profile_in_use_;
}

ACE_INLINE TAO_MProfile *
TAO_Stub::make_profiles (void)
{
  TAO_MProfile *mp = 0;

  ACE_NEW_RETURN (mp,
                  TAO_MProfile (base_profiles_),
                  0);

  return mp;
}

ACE_INLINE TAO_Profile *
TAO_Stub::next_forward_profile (void)
{
  TAO_Profile *pfile_next = 0;

  while (this->forward_profiles_
         && (pfile_next = this->forward_profiles_->get_next ()) == 0)
    // that was the last profile.  Now we clean up our forward profiles.
    // since we own the forward MProfiles, we must delete them when done.
    this->forward_back_one ();

  return pfile_next;
}

ACE_INLINE TAO_Profile *
TAO_Stub::next_profile_i (void)
{

  TAO_Profile *pfile_next = 0;
  if (this->forward_profiles_)
    {
      pfile_next = this->next_forward_profile ();
      if (pfile_next == 0)
        pfile_next = this->base_profiles_.get_next ();
    }
  else
    pfile_next = this->base_profiles_.get_next ();

  if (pfile_next == 0)
    this->reset_base ();
  else
    this->set_profile_in_use_i (pfile_next);

  return pfile_next;
}

ACE_INLINE TAO_Profile *
TAO_Stub::next_profile (void)
{

  ACE_MT (ACE_GUARD_RETURN (ACE_Lock,
                            guard,
                            *this->profile_lock_ptr_,
                            0));
  return this->next_profile_i ();
}

ACE_INLINE CORBA::Boolean
TAO_Stub::valid_forward_profile (void)
{
  return (this->profile_success_ && this->forward_profiles_);
}

ACE_INLINE void
TAO_Stub::set_valid_profile (void)
{
  this->profile_success_ = 1;
}

ACE_INLINE CORBA::Boolean
TAO_Stub::valid_profile (void)
{
  return (CORBA::Boolean) this->profile_success_;
}

ACE_INLINE TAO_Profile *
TAO_Stub::base_profiles (const TAO_MProfile &mprofiles)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_Lock,
                            guard,
                            *this->profile_lock_ptr_,
                            0));

  // first reset things so we start from scratch!
  this->reset_forward ();
  this->base_profiles_.set (mprofiles);
  this->reset_base ();
  return this->profile_in_use_;

}

ACE_INLINE CORBA::Boolean
TAO_Stub::next_profile_retry (void)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_Lock,
                            guard,
                            *this->profile_lock_ptr_,
                            0));

  if (this->profile_success_ && this->forward_profiles_)
    {
      this->reset_profiles_i ();
      return 1;
    }
  else if (this->next_profile_i ())
    {
      return 1;
    }

  return 0;
#if 0
  else
    {
      // Check whether the loaded services have something to say about
      // this condition
      TAO_Profile *prof = 0;
      this->orb_core_->service_profile_reselection (this,
                                                    prof);

      // If the service is loaded and has a profile then try it.
      if (prof)
        {
          return 1;
        }
      this->reset_profiles_i ();
      return 0;
    }
#endif /*If 0 */
}

ACE_INLINE const TAO_MProfile&
TAO_Stub::base_profiles (void) const
{
  return this->base_profiles_;
}

ACE_INLINE TAO_MProfile&
TAO_Stub::base_profiles (void)
{
  return this->base_profiles_;
}

ACE_INLINE const TAO_MProfile *
TAO_Stub::forward_profiles (void) const
{
  return this->forward_profiles_;
}

ACE_INLINE TAO_ORB_Core*
TAO_Stub::orb_core (void) const
{
  return this->orb_core_.get ();
}

ACE_INLINE CORBA::ORB_var &
TAO_Stub::servant_orb_var (void)
{
  // Simply pass back the ORB pointer for temporary use.
  return this->servant_orb_;
}

ACE_INLINE CORBA::ORB_ptr
TAO_Stub::servant_orb_ptr (void)
{
  // Simply pass back the ORB pointer for temporary use.
  return CORBA::ORB::_duplicate (this->servant_orb_.in ());
}

ACE_INLINE void
TAO_Stub::servant_orb (CORBA::ORB_ptr orb)
{
  this->servant_orb_ = CORBA::ORB::_duplicate (orb);
}


ACE_INLINE void
TAO_Stub::addressing_mode (CORBA::Short addr)
{
  this->addressing_mode_ = addr;
}

ACE_INLINE CORBA::Short
TAO_Stub::addressing_mode (void)
{
  return this->addressing_mode_;
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
  if (this->get () != p && this->get () != 0)
    this->get ()->_decr_refcnt ();
  this->p_ = p;
}

ACE_INLINE TAO_Stub *
TAO_Stub_Auto_Ptr::operator-> () const
{
  ACE_TRACE ("TAO_Stub_Auto_Ptr::operator->");
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
  if (this->get() != 0)
    this->get ()->_decr_refcnt ();
}

// Accessor methods to the underlying Stub Object

ACE_INLINE TAO_Stub &
TAO_Stub_Auto_Ptr::operator *() const
{
  ACE_TRACE ("TAO_Stub_Auto_Ptr::operator *()");
  // @@ Potential problem if this->p_ is zero!
  return *this->get ();
}
