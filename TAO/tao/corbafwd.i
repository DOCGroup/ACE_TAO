// $Id$

ACE_INLINE CORBA::String
CORBA::string_alloc (CORBA::ULong len)
{
  // Allocate 1 + strlen to accomodate the null terminating character.
  return new CORBA::Char[size_t (len + 1)];
}

ACE_INLINE void
CORBA::string_free (CORBA::Char *str)
{
  delete [] str;
}

ACE_INLINE CORBA::WChar*
CORBA::wstring_alloc (CORBA::ULong len)
{
  return new CORBA::WChar [(size_t) (len + 1)];
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
