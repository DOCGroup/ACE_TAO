ACE_INLINE
IIOP::Version::Version (CORBA::Octet maj, CORBA::Octet min)
  : major (maj),
    minor (min)
{}

ACE_INLINE
IIOP::Profile::Profile (void)
{}

ACE_INLINE
IIOP::Profile::~Profile (void)
{
  ACE_OS::free (host);
  //  ACE_OS::free (object_key.buffer);
  delete [] object_key.buffer;
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
