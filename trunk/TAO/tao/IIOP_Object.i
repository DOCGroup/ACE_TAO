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
}

ACE_INLINE
IIOP_Object::IIOP_Object (char *repository_id)
  : STUB_Object (repository_id),
    refcount_ (0)
{
}

ACE_INLINE
IIOP_Object::IIOP_Object (char *repository_id,
                          const IIOP::Profile &a_profile)
  : STUB_Object (repository_id),
    profile (a_profile),
    refcount_ (0)
{
}

