#include "tao/CORBA_String.h"

#if !defined (__ACE_INLINE__)
# include "tao/CORBA_String.inl"
#endif /* ! __ACE_INLINE__ */

#include "ace/OS.h"
#include "ace/streams.h"


ACE_RCSID (tao,
           CORBA_String,
           "$Id$")


// String utility support; this needs to be integrated with the ORB's
// own memory allocation subsystem.

CORBA::String_var::String_var (char *p)
  : ptr_ (p)
{
  // NOTE: According to the CORBA spec this string must *not* be
  // copied, but it is non-compliant to use it/release it in the
  // calling code.  argument is consumed. p should never be NULL
}

CORBA::String_var::String_var (const CORBA::String_var& r)
{
  this->ptr_ = CORBA::string_dup (r.ptr_);
}

CORBA::String_var::~String_var (void)
{
  CORBA::string_free (this->ptr_);
  this->ptr_ = 0;
}

CORBA::String_var &
CORBA::String_var::operator= (char *p)
{
  if (this->ptr_ != p)
    {
      CORBA::string_free (this->ptr_);
      this->ptr_ = p;
    }
  return *this;
}

CORBA::String_var &
CORBA::String_var::operator= (const char *p)
{
  CORBA::string_free (this->ptr_);

  this->ptr_ = CORBA::string_dup (p);
  return *this;
}

CORBA::String_var &
CORBA::String_var::operator= (const CORBA::String_var& r)
{
  if (this != &r)
    {
      CORBA::string_free (this->ptr_);
      this->ptr_ = CORBA::string_dup (r.ptr_);
    }
  return *this;
}

// ****************************************************************

CORBA::WString_var::WString_var (CORBA::WChar *p)
  : ptr_ (p)
{
  // NOTE: According to the CORBA spec this string must *not* be
  // copied, but it is non-compliant to use it/release it in the
  // calling code.  argument is consumed. p should never be NULL
}

CORBA::WString_var::WString_var (const CORBA::WString_var& r)
{
  this->ptr_ = CORBA::wstring_dup (r.ptr_);
}

CORBA::WString_var::~WString_var (void)
{
  CORBA::wstring_free (this->ptr_);
  this->ptr_ = 0;
}

CORBA::WString_var &
CORBA::WString_var::operator= (CORBA::WChar *p)
{
  if (this->ptr_ != p)
    {
      CORBA::wstring_free (this->ptr_);
      this->ptr_ = p;
    }
  return *this;
}

CORBA::WString_var &
CORBA::WString_var::operator= (const CORBA::WChar *p)
{
  CORBA::wstring_free (this->ptr_);

  this->ptr_ = CORBA::wstring_dup (p);
  return *this;
}

CORBA::WString_var &
CORBA::WString_var::operator= (const CORBA::WString_var& r)
{
  if (this != &r)
    {
      CORBA::wstring_free (this->ptr_);
      this->ptr_ = CORBA::wstring_dup (r.ptr_);
    }
  return *this;
}

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
  const CORBA::ULong len = ACE_OS::wslen (wsv.in ());

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
  const CORBA::ULong len = is.tellg ();
  wsv = CORBA::wstring_alloc (len);
  is.seekg (0, ios::beg);

  for (CORBA::ULong i = 0; i < len; ++i)
    {
      CORBA::WChar wc = 0;

      // Unformatted input is used to work around overloaded
      // extraction operator (>>) ambiguities on some platforms.
      is.read (ACE_reinterpret_cast (char *, &wc), sizeof (wc));

      wsv[i] = wc;
    }

  wsv[len] = 0;  // NULL terminate

  return is;
}

ostream &
operator<< (ostream &os, CORBA::WString_out &wso)
{
  CORBA::WChar *tmp = wso.ptr ();
  const CORBA::ULong len = ACE_OS::wslen (tmp);

  for (CORBA::ULong i = 0; i < len; ++i)
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
      is.read (ACE_reinterpret_cast (char *, &wc), sizeof (wc));

      wso.ptr ()[i] = wc;
    }

  wso.ptr ()[len] = 0;  // NULL terminate

  return is;
}

#endif /* ACE_LACKS_IOSTREAM_TOTALLY */
