// -*- C++ -*-
//
// $Id$

ACE_INLINE const char *
CIAO::DomainApplicationManager_Impl::get_uuid () const throw ()
{
  return this->uuid_;
}

ACE_INLINE void
CIAO::DomainApplicationManager_Impl::set_uuid (const char * uuid) throw ()
{
  // Copy this uuid reference
  this->uuid_ = CORBA::string_dup (uuid);
}
