ACE_INLINE
IIOP::Version::Version (CORBA_Octet maj, CORBA_Octet min)
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
  ACE_OS::free (object_key.buffer);
}

ACE_INLINE
IIOP_Object::~IIOP_Object (void)
{
  assert (_refcount == 0);
  delete fwd_profile;
}

ACE_INLINE
IIOP_Object::IIOP_Object (char *repository_id)
  : fwd_profile (0),
    base (this),
    STUB_Object (repository_id),
    _refcount (1)
{}

ACE_INLINE
IIOP_Object::IIOP_Object (char *repository_id, IIOP::ProfileBody& a_profile)
  : fwd_profile (0),
    base (this),
    STUB_Object (repository_id),
    _refcount (1),
    profile(a_profile)
{}
