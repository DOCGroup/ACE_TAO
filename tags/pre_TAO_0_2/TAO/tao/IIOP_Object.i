// $Id$

ACE_INLINE
IIOP::Version::Version (CORBA::Octet maj, CORBA::Octet min)
  : major (maj),
    minor (min)
{
}

ACE_INLINE
IIOP::Profile::Profile (void)
  : host (0)
{
}

ACE_INLINE
IIOP::Profile::~Profile (void)
{
  delete [] this->host;
}

ACE_INLINE void
IIOP::Profile::object_addr (const ACE_INET_Addr *addr)
{
  if (addr != 0)
    this->object_addr_ = *addr;
  else if (this->host)
    this->object_addr_.set (this->port, this->host);
}

ACE_INLINE ACE_INET_Addr &
IIOP::Profile::object_addr (void)
{
  return this->object_addr_;
}

ACE_INLINE
IIOP_Object::~IIOP_Object (void)
{
  assert (this->refcount_ == 0);
  delete this->fwd_profile_;
  delete this->fwd_profile_lock_ptr_;
}

ACE_INLINE
IIOP_Object::IIOP_Object (char *repository_id)
  : STUB_Object (repository_id),
    fwd_profile_ (0),
    refcount_ (1),
    use_locate_request_ (CORBA::B_FALSE),
    first_locate_request_ (CORBA::B_FALSE)
{
  this->fwd_profile_lock_ptr_ =  TAO_ORB_Core_instance ()
                                ->client_factory ()
                                  ->create_iiop_profile_lock ();  
}

ACE_INLINE
IIOP_Object::IIOP_Object (char *repository_id,
                          const IIOP::Profile &a_profile)
  : STUB_Object (repository_id),
    profile (a_profile),
    fwd_profile_ (0),
    refcount_ (1),
    use_locate_request_ (CORBA::B_FALSE),
    first_locate_request_ (CORBA::B_FALSE)
{
  this->fwd_profile_lock_ptr_ =  TAO_ORB_Core_instance ()
                                ->client_factory ()
                                  ->create_iiop_profile_lock ();  
}


ACE_INLINE
IIOP::Profile *
IIOP_Object::get_fwd_profile_i (void)
{
  return this->fwd_profile_;
}

ACE_INLINE
IIOP::Profile *
IIOP_Object::get_fwd_profile (void)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_Lock, 
                            guard, 
                            *this->fwd_profile_lock_ptr_, 
                            0));
  return this->get_fwd_profile_i ();
}


ACE_INLINE
IIOP::Profile *
IIOP_Object::set_fwd_profile (IIOP::Profile *new_profile)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_Lock, 
                            guard, 
                            *this->fwd_profile_lock_ptr_, 
                            0));
  IIOP::Profile *old = this->fwd_profile_;
  if (new_profile != 0)
    {
      delete this->fwd_profile_;
      ACE_NEW_RETURN (this->fwd_profile_,
                      IIOP::Profile(),
                      0);
      *this->fwd_profile_ = *new_profile;
      // use the copy operator on IIOP_Profile
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
  first_locate_request_ = CORBA::B_TRUE;
}

ACE_INLINE
void 
IIOP_Object::use_locate_requests (CORBA::Boolean use_it)
{
  if (use_it)
    {
      this->first_locate_request_ = CORBA::B_TRUE;
      this->use_locate_request_ = CORBA::B_TRUE;
    }
  else 
    // don't use it
    {
      this->first_locate_request_ = CORBA::B_TRUE;
      this->use_locate_request_ = CORBA::B_TRUE;
    }   
} 
