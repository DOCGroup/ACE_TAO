// -*- C++ -*-
//
// $Id$

ACE_INLINE void
CORBA::string_free (char *str)
{
  delete [] str;
}

ACE_INLINE void
CORBA::wstring_free (CORBA::WChar *const str)
{
  delete [] str;
}

ACE_INLINE CORBA::Environment&
CORBA::default_environment (void)
{
  return TAO_default_environment ();
}
