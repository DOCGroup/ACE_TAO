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
  // delete [] this->object_key.buffer;
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
}

ACE_INLINE
IIOP_Object::IIOP_Object (char *repository_id)
  : STUB_Object (repository_id),
    base (this),
    refcount_ (1),
    fwd_profile_ (0)
{
}

ACE_INLINE
IIOP_Object::IIOP_Object (char *repository_id,
			  const IIOP::Profile &a_profile)
  : STUB_Object (repository_id),
    profile (a_profile),
    base (this),
    refcount_ (1),
    fwd_profile_ (0)
{
}

ACE_INLINE
IIOP::Profile *
IIOP_Object::fwd_profile_i (void)
{
  return this->fwd_profile_;
}

ACE_INLINE
IIOP::Profile *
IIOP_Object::fwd_profile_i (IIOP::Profile *new_profile)
{
  IIOP::Profile *old = this->fwd_profile_;
  this->fwd_profile_ = new_profile;
  return old;
}

ACE_INLINE
IIOP::Profile *
IIOP_Object::fwd_profile (void)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, guard, this->fwd_profile_lock_, 0));
  return this->fwd_profile_i ();
}

ACE_INLINE
IIOP::Profile *
IIOP_Object::fwd_profile (IIOP::Profile *new_profile)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, guard, this->fwd_profile_lock_, 0));
  return this->fwd_profile_i (new_profile);
}

ACE_INLINE
ACE_SYNCH_MUTEX &
IIOP_Object::fwd_profile_lock (void)
{
  return fwd_profile_lock_;
}
