// -*- C++ -*-
// $Id$

ACE_INLINE char *
CORBA::string_alloc (CORBA::ULong len)
{
  // Allocate 1 + strlen to accomodate the null terminating character.

  char *s = 0;
  ACE_NEW_RETURN (s,
                  char[size_t (len + 1)],
                  0);

  return s;
}

ACE_INLINE void
CORBA::string_free (char *str)
{
  delete [] str;
}

ACE_INLINE CORBA::WChar*
CORBA::wstring_alloc (CORBA::ULong len)
{
  CORBA::WChar *s = 0;
  ACE_NEW_RETURN (s,
                  CORBA::WChar [(size_t) (len + 1)],
                  0);

  return s;
}

ACE_INLINE void
CORBA::wstring_free (CORBA::WChar *const str)
{
  delete [] str;
}

ACE_INLINE CORBA_Environment&
CORBA::default_environment ()
{
  return TAO_default_environment ();
}
