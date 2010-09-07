// $Id$

#include "tao/CORBA_String.h"
#include "tao/String_Manager_T.h"

#include "ace/OS_NS_string.h"
#include "ace/OS_NS_wchar.h"
#include "ace/OS_Memory.h"

// FUZZ: disable check_for_streams_include
#include "ace/streams.h"

#if !defined (__ACE_INLINE__)
# include "tao/CORBA_String.inl"
#endif /* ! __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// *************************************************************
// C++ iostream operators for (W)String_var and (W)String_out
// *************************************************************

#if !defined (ACE_LACKS_IOSTREAM_TOTALLY)

ostream &
operator<< (ostream &os, const CORBA::String_var &sv)
{
  os << sv.in ();
  return os;
}

istream &
operator>> (istream &is, CORBA::String_var &sv)
{
  is.seekg (0, ios::end);
  sv = CORBA::string_alloc (is.tellg ());
  is.seekg (0, ios::beg);
  is >> sv.inout ();
  return is;
}

ostream &
operator<< (ostream &os, CORBA::String_out &so)
{
  os << so.ptr ();
  return os;
}

istream &
operator>> (istream &is, CORBA::String_out &so)
{
  is.seekg (0, ios::end);
  so = CORBA::string_alloc (is.tellg ());
  is.seekg (0, ios::beg);
  is >> so.ptr ();
  return is;
}

// Until we implement WString support for platforms with a
// 4-byte wchar_t, we just define the following to emit
// the CORBA::WChars one by one.

ostream &
operator<< (ostream &os, const CORBA::WString_var &wsv)
{
  CORBA::ULong const len =
    static_cast <CORBA::ULong> (ACE_OS::strlen (wsv.in ()));

  for (CORBA::ULong i = 0; i < len; ++i)
    {
      os << wsv[i];
    }

  return os;
}

istream &
operator>> (istream &is, CORBA::WString_var &wsv)
{
  is.seekg (0, ios::end);
  // @@ is.tellg()/sizeof(CORBA::WChar) instead?
  CORBA::ULong const len = is.tellg ();
  wsv = CORBA::wstring_alloc (len);
  is.seekg (0, ios::beg);

  for (CORBA::ULong i = 0; i < len; ++i)
    {
      CORBA::WChar wc = 0;

      // Unformatted input is used to work around overloaded
      // extraction operator (>>) ambiguities on some platforms.
      is.read (reinterpret_cast<char *> (&wc), sizeof (wc));

      wsv[i] = wc;
    }

  wsv[len] = 0;  // NULL terminate

  return is;
}

ostream &
operator<< (ostream &os, CORBA::WString_out &wso)
{
  CORBA::WChar *tmp = wso.ptr ();
  const size_t len = ACE_OS::strlen (tmp);

  for (size_t i = 0; i < len; ++i)
    {
      os << tmp[i];
    }

  return os;
}

istream &
operator>> (istream &is, CORBA::WString_out &wso)
{
  is.seekg (0, ios::end);
  // @@ is.tellg()/sizeof(CORBA::WChar) instead?
  const CORBA::ULong len = is.tellg ();
  wso = CORBA::wstring_alloc (len);
  is.seekg (0, ios::beg);

  for (CORBA::ULong i = 0; i < len; ++i)
    {
      CORBA::WChar wc = 0;

      // Unformatted input is used to work around overloaded
      // extraction operator (>>) ambiguities on some platforms.
      is.read (reinterpret_cast<char *> (&wc), sizeof (wc));

      wso.ptr ()[i] = wc;
    }

  wso.ptr ()[len] = 0;  // NULL terminate

  return is;
}

#endif /* ACE_LACKS_IOSTREAM_TOTALLY */

TAO_END_VERSIONED_NAMESPACE_DECL
