// -*- C++ -*-
//
// $Id$


#include "tao/CORBA_String.h"

#if !defined (__ACE_INLINE__)
# include "tao/CORBA_String.inl"
#endif /* ! __ACE_INLINE__ */


ACE_RCSID(tao, CORBA_String, "$Id$")


// String utility support; this needs to be integrated with the ORB's
// own memory allocation subsystem.

CORBA_String_var::CORBA_String_var (char *p)
  : ptr_ (p)
{
  // NOTE: According to the CORBA spec this string must *not* be
  // copied, but it is non-compliant to use it/release it in the
  // calling code.  argument is consumed. p should never be NULL
}

CORBA_String_var::CORBA_String_var (const CORBA_String_var& r)
{
  this->ptr_ = CORBA::string_dup (r.ptr_);
}

CORBA_String_var::~CORBA_String_var (void)
{
  CORBA::string_free (this->ptr_);
  this->ptr_ = 0;
}

CORBA_String_var &
CORBA_String_var::operator= (char *p)
{
  if (this->ptr_ != p)
    {
      CORBA::string_free (this->ptr_);
      this->ptr_ = p;
    }
  return *this;
}

CORBA_String_var &
CORBA_String_var::operator= (const char *p)
{
  CORBA::string_free (this->ptr_);

  this->ptr_ = CORBA::string_dup (p);
  return *this;
}

CORBA_String_var &
CORBA_String_var::operator= (const CORBA_String_var& r)
{
  if (this != &r)
    {
      CORBA::string_free (this->ptr_);
      this->ptr_ = CORBA::string_dup (r.ptr_);
    }
  return *this;
}

// ****************************************************************

CORBA_WString_var::CORBA_WString_var (CORBA::WChar *p)
  : ptr_ (p)
{
  // NOTE: According to the CORBA spec this string must *not* be
  // copied, but it is non-compliant to use it/release it in the
  // calling code.  argument is consumed. p should never be NULL
}

CORBA_WString_var::CORBA_WString_var (const CORBA_WString_var& r)
{
  this->ptr_ = CORBA::wstring_dup (r.ptr_);
}

CORBA_WString_var::~CORBA_WString_var (void)
{
  CORBA::wstring_free (this->ptr_);
  this->ptr_ = 0;
}

CORBA_WString_var &
CORBA_WString_var::operator= (CORBA::WChar *p)
{
  if (this->ptr_ != p)
    {
      CORBA::wstring_free (this->ptr_);
      this->ptr_ = p;
    }
  return *this;
}

CORBA_WString_var &
CORBA_WString_var::operator= (const CORBA::WChar *p)
{
  CORBA::wstring_free (this->ptr_);

  this->ptr_ = CORBA::wstring_dup (p);
  return *this;
}

CORBA_WString_var &
CORBA_WString_var::operator= (const CORBA_WString_var& r)
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
  CORBA::ULong len = ACE_OS::wslen (wsv.in ());

  for (CORBA::ULong i = 0; i < len; ++i)
    {
      os << wsv[i];
    }

  return os;
}

istream &
operator>> (istream &is, CORBA::WString_var &wsv)
{
  CORBA::ULong i = 0;
  CORBA::WChar wc;
  is >> wc;

  while (wc)
    {
      wsv[i] = wc;
      ++i;
      is >> wc;
    }

  wsv[i] = 0;
  return is;
}

ostream &
operator<< (ostream &os, CORBA::WString_out &wso)
{
  CORBA::WChar *tmp = wso.ptr ();
  CORBA::ULong len = ACE_OS::wslen (tmp);

  for (CORBA::ULong i = 0; i < len; ++i)
    {
      os << tmp[i];
    }

  return os;
}

istream &
operator>> (istream &is, CORBA::WString_out &wso)
{
  CORBA::ULong i = 0;
  CORBA::WChar wc;
  is >> wc;

  while (wc)
    {
      wso.ptr ()[i] = wc;
      ++i;
      is >> wc;
    }

  wso.ptr ()[i] = 0;
  return is;
}

#endif /* ACE_LACKS_IOSTREAM_TOTALLY */
