// -*- C++ -*-
//
// $Id$

// @@ (OO) To improve const correctness, this method should be
//         declared as const, i.e.:
//
//           ACE_INLINE const char *
//           CIAO::DomainApplicationManager_Impl::get_uuid () const

ACE_INLINE const char *
CIAO::DomainApplicationManager_Impl::get_uuid ()
{
  return this->uuid_;
}

ACE_INLINE void
CIAO::DomainApplicationManager_Impl::set_uuid (const char * uuid)
{
  // Copy this uuid reference
  this->uuid_ = CORBA::string_dup (uuid);
}
