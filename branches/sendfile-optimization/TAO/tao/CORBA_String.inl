// -*- C++ -*-
//
// $Id$

// ----------------------------------------------------------------------
// String_var type
// ----------------------------------------------------------------------

#include "ace/OS_NS_string.h"
// #include "ace/OS_NS_wchar.h"


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
CORBA::String_var::String_var (void)
  : ptr_ (0)
{
}

ACE_INLINE
CORBA::String_var::String_var (const char *p)
  : ptr_ (CORBA::string_dup (p))
{
}

ACE_INLINE CORBA::Char &
CORBA::String_var::operator[] (CORBA::ULong slot)
{
  // We need to verify bounds else raise some exception.
  return this->ptr_[slot];
}

ACE_INLINE CORBA::Char
CORBA::String_var::operator[] (CORBA::ULong slot) const
{
  // We need to verify bounds else raise some exception.
  return this->ptr_[slot];
}

ACE_INLINE
CORBA::String_var::operator char *&()
{
  return this->ptr_;
}

ACE_INLINE
CORBA::String_var::operator const char *() const
{
  return this->ptr_;
}

ACE_INLINE const char *
CORBA::String_var::in (void) const
{
  return this->ptr_;
}

ACE_INLINE char *&
CORBA::String_var::inout (void)
{
  return this->ptr_;
}

ACE_INLINE char *&
CORBA::String_var::out (void)
{
  CORBA::string_free (this->ptr_);
  this->ptr_ = 0;
  return this->ptr_;
}

ACE_INLINE char *
CORBA::String_var::_retn (void)
{
  char *temp = this->ptr_;
  this->ptr_ = 0;
  return temp;
}

/// TAO extension.
ACE_INLINE char *
CORBA::String_var::ptr (void)
{
  return this->ptr_;
}

// ----------------------------------------------------
//  String_out type
// ----------------------------------------------------

ACE_INLINE
CORBA::String_out::String_out (char *&s)
  : ptr_ (s)
{
  this->ptr_ = 0;
}

ACE_INLINE
CORBA::String_out::String_out (CORBA::String_var &s)
  : ptr_ (s.out ())
{
}

ACE_INLINE
CORBA::String_out::String_out (const CORBA::String_out &s)
  : ptr_ (s.ptr_)
{
}

ACE_INLINE CORBA::String_out &
CORBA::String_out::operator= (const CORBA::String_out &s)
{
  this->ptr_ = s.ptr_;
  return *this;
}

ACE_INLINE CORBA::String_out &
CORBA::String_out::operator= (char *p)
{
  this->ptr_ = p;
  return *this;
}

ACE_INLINE CORBA::String_out &
CORBA::String_out::operator= (const char *p)
{
  this->ptr_ = CORBA::string_dup (p);
  return *this;
}

ACE_INLINE
CORBA::String_out::operator char *&()
{
  return this->ptr_;
}

ACE_INLINE char *&
CORBA::String_out::ptr (void)
{
  return this->ptr_;
}

// ****************************************************************

// ----------------------------------------------------------------------
// String_var type
// ----------------------------------------------------------------------

ACE_INLINE
CORBA::WString_var::WString_var (void)
  : ptr_ (0)
{
}

ACE_INLINE
CORBA::WString_var::WString_var (const CORBA::WChar *p)
  : ptr_ (CORBA::wstring_dup (p))
{
}

ACE_INLINE CORBA::WChar &
CORBA::WString_var::operator[] (CORBA::ULong slot)
{
  // @@ We need to verify bounds else raise some exception.
  return this->ptr_[slot];
}

ACE_INLINE CORBA::WChar
CORBA::WString_var::operator[] (CORBA::ULong slot) const
{
  // @@ We need to verify bounds else raise some exception.
  return this->ptr_[slot];
}

ACE_INLINE
CORBA::WString_var::operator CORBA::WChar *&()
{
  return this->ptr_;
}

ACE_INLINE
CORBA::WString_var::operator const CORBA::WChar *() const
{
  return this->ptr_;
}

ACE_INLINE const CORBA::WChar *
CORBA::WString_var::in (void) const
{
  return this->ptr_;
}

ACE_INLINE CORBA::WChar *&
CORBA::WString_var::inout (void)
{
  return this->ptr_;
}

ACE_INLINE CORBA::WChar *&
CORBA::WString_var::out (void)
{
  CORBA::wstring_free (this->ptr_);
  this->ptr_ = 0;
  return this->ptr_;
}

ACE_INLINE CORBA::WChar *
CORBA::WString_var::_retn (void)
{
  CORBA::WChar *temp = this->ptr_;
  this->ptr_ = 0;
  return temp;
}

/// TAO extension.
ACE_INLINE CORBA::WChar *
CORBA::WString_var::ptr (void)
{
  return this->ptr_;
}

// ----------------------------------------------------
//  String_out type
// ----------------------------------------------------

ACE_INLINE
CORBA::WString_out::WString_out (CORBA::WChar *&s)
  : ptr_ (s)
{
  this->ptr_ = 0;
}

ACE_INLINE
CORBA::WString_out::WString_out (CORBA::WString_var &s)
  : ptr_ (s.out ())
{
}

ACE_INLINE
CORBA::WString_out::WString_out (const CORBA::WString_out &s)
  : ptr_ (s.ptr_)
{
}

ACE_INLINE CORBA::WString_out &
CORBA::WString_out::operator= (const CORBA::WString_out &s)
{
  this->ptr_ = s.ptr_;
  return *this;
}

ACE_INLINE CORBA::WString_out &
CORBA::WString_out::operator= (CORBA::WChar *p)
{
  this->ptr_ = p;
  return *this;
}

ACE_INLINE CORBA::WString_out &
CORBA::WString_out::operator= (const CORBA::WChar *p)
{
  this->ptr_ = CORBA::wstring_dup (p);
  return *this;
}

ACE_INLINE
CORBA::WString_out::operator CORBA::WChar *&()
{
  return this->ptr_;
}

ACE_INLINE CORBA::WChar *&
CORBA::WString_out::ptr (void)
{
  return this->ptr_;
}

// ----------------------------------------------------------------------

ACE_INLINE bool
TAO::String_Var_Equal_To::operator() (CORBA::String_var const & lhs,
                                      CORBA::String_var const & rhs) const
{
  return (ACE_OS::strcmp (lhs.in (), rhs.in ()) == 0);
}

ACE_INLINE bool
TAO::String_Var_Equal_To::operator() (CORBA::WString_var const & lhs,
                                      CORBA::WString_var const & rhs) const
{
  return (ACE_OS::strcmp (lhs.in (), rhs.in ()) == 0);
}

TAO_END_VERSIONED_NAMESPACE_DECL
