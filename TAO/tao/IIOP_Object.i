// $Id$

ACE_INLINE
IIOP::Version::Version (CORBA::Octet maj, CORBA::Octet min)
  : major (maj),
    minor (min)
{
}

ACE_INLINE
IIOP::Profile::Profile (const char *h,
                        CORBA::UShort p,
                        const TAO_opaque &key,
                        const ACE_INET_Addr &addr)
  : host (0)
{
  this->set (h, p, key, addr);
}

ACE_INLINE
IIOP::Profile::Profile (void)
  : host (0),
    port (0)
{
}

ACE_INLINE
IIOP::Profile::Profile (const IIOP::Profile &src)
  : host (0)
{
  this->set (src.host,
             src.port,
             src.object_key,
             src.object_addr_);
}

ACE_INLINE
IIOP::Profile::~Profile (void)
{
  delete [] this->host;
}

ACE_INLINE IIOP::Profile &
IIOP::Profile::operator= (const IIOP::Profile &src)
{
  this->set (src.host,
             src.port,
             src.object_key,
             src.object_addr_);
  return *this;
}

ACE_INLINE int
IIOP::Profile::operator== (const IIOP::Profile &rhs)
{
  return 
    this->object_key == rhs.object_key && 
    this->port == rhs.port && 
    ACE_OS::strcmp (this->host, rhs.host) == 0 && 
    this->iiop_version.minor == rhs.iiop_version.minor && 
    this->iiop_version.major == rhs.iiop_version.major;
}

ACE_INLINE void
IIOP::Profile::reset_object_addr (void)
{
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
  
  // Cleanup hint
  if (this->handler_ != 0)
    this->handler_->cleanup_hint ();
}

ACE_INLINE
IIOP_Object::IIOP_Object (char *repository_id)
  : STUB_Object (repository_id),
    fwd_profile_ (0),
    fwd_profile_success_ (0),
    refcount_ (1),
    use_locate_request_ (0),
    first_locate_request_ (0),
    handler_ (0)
{
  this->fwd_profile_lock_ptr_ = 
    TAO_ORB_Core_instance ()->client_factory ()->create_iiop_profile_lock ();  
}

ACE_INLINE
IIOP_Object::IIOP_Object (char *repository_id,
                          const char *host,
                          CORBA::UShort port,
                          const TAO_opaque &object_key,
                          const ACE_INET_Addr &addr)
  : STUB_Object (repository_id),
    profile (host, port, object_key, addr),
    fwd_profile_ (0),
    fwd_profile_success_ (0),
    refcount_ (1),
    use_locate_request_ (0),
    first_locate_request_ (0),
    handler_ (0)
{
  this->fwd_profile_lock_ptr_ =  
    TAO_ORB_Core_instance ()->client_factory ()->create_iiop_profile_lock ();  
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
                      IIOP::Profile (),
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

ACE_INLINE TAO_Client_Connection_Handler *&
IIOP_Object::handler (void)
{
  return this->handler_;
}

ACE_INLINE void 
IIOP_Object::reset_handler (void)
{
  this->handler_->cleanup_hint ();
  this->handler_ = 0;
}

