// This may look like C, but it's really -*- C++ -*-

// ============================================================================
//
// = LIBRARY
//    TAO
// 
// = FILENAME
//    corbacom.i
//
// = DESCRIPTION
//    CORBA C/C++/COM mapping for Win32
//
// = AUTHOR
//     Copyright 1994-1995 by Sun Microsystems Inc.
// 
// ============================================================================

// String utility support

ACE_INLINE ACE_Svc_Export CORBA::String
CORBA::string_alloc (CORBA::ULong len)
{
  // allocate 1 + strlen to accomodate the null terminating character
  return new CORBA::Char [(size_t)(len + 1)];
}

ACE_INLINE ACE_Svc_Export CORBA::String
CORBA::string_dup (const CORBA::Char *str)
{
  return CORBA::string_copy (str);
}

ACE_INLINE ACE_Svc_Export void
CORBA::string_free (CORBA::Char *str)
{
  delete [] str;
}

// ----------------------------------------------------------------------
// String_var type
// ----------------------------------------------------------------------

ACE_INLINE 
CORBA::String_var::String_var (void)
{
  this->ptr_ = 0;
}

ACE_INLINE 
CORBA::String_var::~String_var (void)
{
  if (this->ptr_ != 0)
    {
      CORBA::string_free (this->ptr_);
    }
}

ACE_INLINE 
CORBA::String_var::String_var (char *p)
  : ptr_ (p)
{
  // argument is consumed. p should never be NULL
}

ACE_INLINE
CORBA::String_var::String_var (const char *p)
  : ptr_ (CORBA::string_dup ((char *)p))
{
}

ACE_INLINE
CORBA::String_var::String_var (const CORBA::String_var& r)
{
  this->ptr_ = CORBA::string_dup (r.ptr_);
}

ACE_INLINE CORBA::Char & 
CORBA::String_var::operator[] (CORBA::ULong index)
{
  // we need to verify bounds else raise some exception
  return this->ptr_[index];
}

ACE_INLINE CORBA::Char 
CORBA::String_var::operator[] (CORBA::ULong index) const
{
  // we need to verify bounds else raise some exception
  return this->ptr_[index];
}

ACE_INLINE
CORBA::String_var::operator char *()
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

// ----------------------------------------------------------------------
// String_out type
// ----------------------------------------------------------------------

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
CORBA::String_out::String_out (CORBA::String_out &s)
  : ptr_ (s.ptr_)
{
}

ACE_INLINE CORBA::String_out &
CORBA::String_out::operator= (CORBA::String_out &s)
{
  this->ptr_ = s.ptr_;
  return *this;
}

ACE_INLINE CORBA::String_out &
CORBA::String_out::operator= (char *s)
{
  this->ptr_ = s;
  return *this;
}

ACE_INLINE CORBA::String_out &
CORBA::String_out::operator= (const char *s)
{
  this->ptr_ = CORBA::string_dup (s);
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

