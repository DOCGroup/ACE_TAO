ACE_INLINE
IIOP::Version::Version (CORBA::Octet maj, CORBA::Octet min)
  : major (maj),
    minor (min)
{}

ACE_INLINE
IIOP::ProfileBody::ProfileBody (void)
  : host (0)
{}

ACE_INLINE
IIOP::ProfileBody::~ProfileBody (void)
{
  ACE_OS::free (host);
  //  ACE_OS::free (object_key.buffer);
  delete [] object_key.buffer;
}

ACE_INLINE
IIOP_Object::~IIOP_Object (void)
{
  assert (refcount_ == 0);
  delete this->fwd_profile_;
}

ACE_INLINE
IIOP_Object::IIOP_Object (char *repository_id)
  : STUB_Object (repository_id),
    base (this),
    refcount_ (1),
    fwd_profile_ (0)
{}

ACE_INLINE
IIOP_Object::IIOP_Object (char *repository_id,
			  const IIOP::ProfileBody &a_profile)
  : STUB_Object (repository_id),
    profile (a_profile),
    base (this),
    refcount_ (1),
    fwd_profile_ (0)
{}

ACE_INLINE
IIOP::ProfileBody *
IIOP_Object::fwd_profile_i (void)
{
  return this->fwd_profile_;
}

ACE_INLINE
IIOP::ProfileBody *
IIOP_Object::fwd_profile_i (IIOP::ProfileBody *new_profile)
{
  IIOP::ProfileBody *old = this->fwd_profile_;
  this->fwd_profile_ = new_profile;
  return old;
}

ACE_INLINE
IIOP::ProfileBody *
IIOP_Object::fwd_profile (void)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, guard, this->fwd_profile_lock_, 0));
  return this->fwd_profile_i ();
}

ACE_INLINE
IIOP::ProfileBody *
IIOP_Object::fwd_profile (IIOP::ProfileBody *new_profile)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, guard, this->fwd_profile_lock_, 0));
  return this->fwd_profile_i (new_profile);
}

ACE_INLINE
ACE_Thread_Mutex &
IIOP_Object::fwd_profile_lock (void)
{
  return fwd_profile_lock_;
}

