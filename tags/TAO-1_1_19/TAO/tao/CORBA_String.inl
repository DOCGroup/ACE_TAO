// -*- C++ -*-

// $Id$

// ----------------------------------------------------------------------
// String_var type
// ----------------------------------------------------------------------

ACE_INLINE
CORBA_String_var::CORBA_String_var (void)
  : ptr_ (0)
{
}

ACE_INLINE
CORBA_String_var::CORBA_String_var (const char *p)
  : ptr_ (CORBA::string_dup (p))
{
}

ACE_INLINE CORBA::Char &
CORBA_String_var::operator[] (CORBA::ULong slot)
{
  // We need to verify bounds else raise some exception.
  return this->ptr_[slot];
}

ACE_INLINE CORBA::Char
CORBA_String_var::operator[] (CORBA::ULong slot) const
{
  // We need to verify bounds else raise some exception.
  return this->ptr_[slot];
}

ACE_INLINE
CORBA_String_var::operator char *()
{
  return this->ptr_;
}

ACE_INLINE
CORBA_String_var::operator const char *() const
{
  return this->ptr_;
}

ACE_INLINE const char *
CORBA_String_var::in (void) const
{
  return this->ptr_;
}

ACE_INLINE char *&
CORBA_String_var::inout (void)
{
  return this->ptr_;
}

ACE_INLINE char *&
CORBA_String_var::out (void)
{
  CORBA::string_free (this->ptr_);
  this->ptr_ = 0;
  return this->ptr_;
}

ACE_INLINE char *
CORBA_String_var::_retn (void)
{
  char *temp = this->ptr_;
  this->ptr_ = 0;
  return temp;
}

// ----------------------------------------------------
//  String_out type
// ----------------------------------------------------

ACE_INLINE
CORBA_String_out::CORBA_String_out (char *&s)
  : ptr_ (s)
{
  this->ptr_ = 0;
}

ACE_INLINE
CORBA_String_out::CORBA_String_out (CORBA_String_var &s)
  : ptr_ (s.out ())
{
}

ACE_INLINE
CORBA_String_out::CORBA_String_out (const CORBA_String_out &s)
  : ptr_ (s.ptr_)
{
}

ACE_INLINE CORBA_String_out &
CORBA_String_out::operator= (const CORBA_String_out &s)
{
  this->ptr_ = s.ptr_;
  return *this;
}

ACE_INLINE CORBA_String_out &
CORBA_String_out::operator= (char *s)
{
  this->ptr_ = s;
  return *this;
}

ACE_INLINE CORBA_String_out &
CORBA_String_out::operator= (const char *s)
{
  this->ptr_ = CORBA::string_dup (s);
  return *this;
}

ACE_INLINE
CORBA_String_out::operator char *&()
{
  return this->ptr_;
}

ACE_INLINE char *&
CORBA_String_out::ptr (void)
{
  return this->ptr_;
}

// ****************************************************************

// ----------------------------------------------------------------------
// String_var type
// ----------------------------------------------------------------------

ACE_INLINE
CORBA_WString_var::CORBA_WString_var (void)
  : ptr_ (0)
{
}

ACE_INLINE
CORBA_WString_var::CORBA_WString_var (const CORBA::WChar *p)
  : ptr_ (CORBA::wstring_dup (p))
{
}

ACE_INLINE CORBA::WChar &
CORBA_WString_var::operator[] (CORBA::ULong slot)
{
  // @@ We need to verify bounds else raise some exception.
  return this->ptr_[slot];
}

ACE_INLINE CORBA::WChar
CORBA_WString_var::operator[] (CORBA::ULong slot) const
{
  // @@ We need to verify bounds else raise some exception.
  return this->ptr_[slot];
}

ACE_INLINE
CORBA_WString_var::operator CORBA::WChar *()
{
  return this->ptr_;
}

ACE_INLINE
CORBA_WString_var::operator const CORBA::WChar *() const
{
  return this->ptr_;
}

ACE_INLINE const CORBA::WChar *
CORBA_WString_var::in (void) const
{
  return this->ptr_;
}

ACE_INLINE CORBA::WChar *&
CORBA_WString_var::inout (void)
{
  return this->ptr_;
}

ACE_INLINE CORBA::WChar *&
CORBA_WString_var::out (void)
{
  CORBA::wstring_free (this->ptr_);
  this->ptr_ = 0;
  return this->ptr_;
}

ACE_INLINE CORBA::WChar *
CORBA_WString_var::_retn (void)
{
  CORBA::WChar *temp = this->ptr_;
  this->ptr_ = 0;
  return temp;
}

// ----------------------------------------------------
//  String_out type
// ----------------------------------------------------

ACE_INLINE
CORBA_WString_out::CORBA_WString_out (CORBA::WChar *&s)
  : ptr_ (s)
{
  this->ptr_ = 0;
}

ACE_INLINE
CORBA_WString_out::CORBA_WString_out (CORBA_WString_var &s)
  : ptr_ (s.out ())
{
}

ACE_INLINE
CORBA_WString_out::CORBA_WString_out (const CORBA_WString_out &s)
  : ptr_ (s.ptr_)
{
}

ACE_INLINE CORBA_WString_out &
CORBA_WString_out::operator= (const CORBA_WString_out &s)
{
  this->ptr_ = s.ptr_;
  return *this;
}

ACE_INLINE CORBA_WString_out &
CORBA_WString_out::operator= (CORBA::WChar *s)
{
  this->ptr_ = s;
  return *this;
}

ACE_INLINE
CORBA_WString_out::operator CORBA::WChar *&()
{
  return this->ptr_;
}

ACE_INLINE CORBA::WChar *&
CORBA_WString_out::ptr (void)
{
  return this->ptr_;
}
