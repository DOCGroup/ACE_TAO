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
  if (!str)
    return 0;

  size_t len = ACE_OS::strlen (str);

  // This allocates an extra byte for the '\0';
  CORBA::String copy = CORBA::string_alloc (len);

  ACE_OS::memcpy (copy, str, len + 1);
  return copy;
}

// ----------------------------------------------------------------------
// Wide strings
// ----------------------------------------------------------------------

ACE_INLINE TAO_Export CORBA::WChar*
CORBA::wstring_alloc (CORBA::ULong len)
{
  return new CORBA::WChar [(size_t) (len + 1)];
}

ACE_INLINE TAO_Export void
CORBA::wstring_free (CORBA::WChar *const str)
{
  delete [] str;
}

ACE_INLINE TAO_Export CORBA::WChar*
CORBA::wstring_dup (const WChar *const str)
{
  if (!str)
    return 0;

  CORBA::WChar* retval = CORBA::wstring_alloc (ACE_OS::wslen (str));
  return ACE_OS::wscpy (retval, str);
}

