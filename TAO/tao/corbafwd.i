// $Id$

ACE_INLINE TAO_Export CORBA::String
CORBA::string_alloc (CORBA::ULong len)
{
  // Allocate 1 + strlen to accomodate the null terminating character.
  return new CORBA::Char[size_t (len + 1)];
}

ACE_INLINE TAO_Export void
CORBA::string_free (CORBA::Char *str)
{
  delete [] str;
}

ACE_INLINE TAO_Export CORBA::String
CORBA::string_dup (const CORBA::Char *str)
{
  return CORBA::string_copy (str);
}

// ----------------------------------------------------------------------
// Wide strings
// ----------------------------------------------------------------------

ACE_INLINE TAO_Export CORBA::WString
CORBA::wstring_dup (const WChar *const str)
{
  return CORBA::wstring_copy (str);
}

