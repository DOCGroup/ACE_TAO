// @(#)corbacom.cpp	1.1 95/08/31
// Copyright 1994-1995 by Sun Microsystems Inc.
// All Rights Reserved
//
// ORB:		support for primitive data types

#include "tao/corba.h"

// @@ Chris, do we really need to use HAVE_WIDEC_H anymore?  Isn't
// this handled by ACE?  
#if defined (HAVE_WIDEC_H)
#	include <widec.h>
#endif /* HAVE_WIDEC_H */

// String utility support; this can need to be integrated with the
// ORB's own memory allocation subsystem.

ACE_Svc_Export CORBA::String
CORBA::string_alloc (CORBA::ULong len)
{
  // allocate 1 + strlen to accomodate the null terminating character
  return new CORBA::Char [(size_t)(len + 1)];
}

CORBA::String
CORBA::string_copy (const CORBA::Char *str)
{
  if (!str)
    return 0;

  CORBA::String	retval = CORBA::string_alloc (ACE_OS::strlen (str));
  // clear the contents of the allocated string
  ACE_OS::memset(retval, '\0', ACE_OS::strlen (str));

  return ACE_OS::strcpy (retval, str);
}

CORBA::String
CORBA::string_dup (const CORBA::Char *str)
{
  return CORBA::string_copy (str);
}

ACE_Svc_Export void
CORBA::string_free (CORBA::Char *str)
{
  delete [] str;
}

// ----------------------------------------------------------------------
// String_var type
// ----------------------------------------------------------------------

CORBA::String_var::String_var (void)
{
  this->ptr_ = 0;
}

CORBA::String_var::~String_var (void)
{
  if (this->ptr_ != 0)
    {
      CORBA::string_free (this->ptr_);
    }
}

CORBA::String_var::String_var (char *p)
  : ptr_ (p)
{
  // argument is consumed. p should never be NULL
}

CORBA::String_var::String_var (const char *p)
  : ptr_ (CORBA::string_dup ((char *)p))
{
}

CORBA::String_var::String_var (const CORBA::String_var& r)
{
  this->ptr_ = CORBA::string_dup (r.ptr_);
}

CORBA::String_var &
CORBA::String_var::operator= (char *p)
{
  if (this->ptr_ != p)
    {
      if (this->ptr_ != 0)
	CORBA::string_free (this->ptr_);
      this->ptr_ = p;
    }
  return *this;
}

CORBA::String_var &
CORBA::String_var::operator= (const char *p)
{
  if (this->ptr_ != 0)
    CORBA::string_free (this->ptr_);

  this->ptr_ = CORBA::string_dup (p);
  return *this;
}

CORBA::String_var &
CORBA::String_var::operator= (const CORBA::String_var& r)
{
  if (this->ptr_ != 0)
    CORBA::string_free (this->ptr_);
  this->ptr_ = CORBA::string_dup (r.ptr_);
  return *this;
}

CORBA::Char & 
CORBA::String_var::operator[] (CORBA::ULong index)
{
  // we need to verify bounds else raise some exception
  return this->ptr_[index];
}

CORBA::Char 
CORBA::String_var::operator[] (CORBA::ULong index) const
{
  // we need to verify bounds else raise some exception
  return this->ptr_[index];
}

#if	!defined (HAVE_WIDEC_H)
// NOTE: assuming that these don't exist unless they're declared in
// that header file ...

extern "C" unsigned
wslen (const CORBA::WChar *str)
{
  u_int len = 0;

  while (*str++)
    len++;
  return len;
}

extern "C" CORBA::WChar *
wscpy (CORBA::WChar *dest,
       const CORBA::WChar *src)
{
    CORBA::WChar	*retval = dest;

    while ((*dest++ = *src++) != 0)
	continue;
    return retval;
}
#endif	/* HAVE_WIDEC_H */

// Wide Character string utility support; this can need to be
// integrated with the ORB's own memory allocation subsystem.

CORBA::WString
CORBA::wstring_alloc (CORBA::ULong len)
{
  return new CORBA::WChar [(size_t) (len + 1)];
}

CORBA::WString
CORBA::wstring_copy (const CORBA::WChar *const str)
{
  if (*str)
    return 0;

  CORBA::WString	retval = CORBA::wstring_alloc (wslen (str));
  return wscpy (retval, str);
}

void
CORBA::wstring_free (CORBA::WChar *const str)
{
  delete str;
}

