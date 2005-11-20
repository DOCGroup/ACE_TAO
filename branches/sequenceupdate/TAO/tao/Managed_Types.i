// -*- C++ -*-
//
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    Managed_Types.i
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

// ****************************************************************

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// default CTOR initializes to empty string
ACE_INLINE
TAO_String_Manager::TAO_String_Manager (void)
  :  ptr_ (CORBA::string_dup (""))
{
}

// copy ctor copies storage
ACE_INLINE
TAO_String_Manager::TAO_String_Manager (const TAO_String_Manager &rhs)
  :  ptr_ (CORBA::string_dup (rhs.ptr_))
{
}

ACE_INLINE
TAO_String_Manager::TAO_String_Manager (const char *s)
  : ptr_ (CORBA::string_dup (s))
{
}

// destructor
ACE_INLINE
TAO_String_Manager::~TAO_String_Manager (void)
{
  CORBA::string_free (this->ptr_);
}

// assignment
ACE_INLINE TAO_String_Manager&
TAO_String_Manager::operator= (const TAO_String_Manager &rhs)
{
  // Strongly exception safe by means of copy and non-throwing swap
  // technique.
  TAO_String_Manager tmp (rhs);

  char * old_ptr = this->ptr_;
  this->ptr_ = tmp.ptr_;
  tmp.ptr_  = old_ptr;

  return *this;
}

// assignment from const char* makes a copy
ACE_INLINE TAO_String_Manager &
TAO_String_Manager::operator= (const char * p)
{
  // Strongly exception safe by means of copy and non-throwing swap
  // technique.
  TAO_String_Manager tmp (p);

  char * old_ptr = this->ptr_;
  this->ptr_ = tmp.ptr_;
  tmp.ptr_ = old_ptr;

  return *this;
}

// assignment from char* owns the string
ACE_INLINE TAO_String_Manager &
TAO_String_Manager::operator= (char * p)
{
  CORBA::string_free (this->ptr_);
  this->ptr_ = p;
  return *this;
}

ACE_INLINE
TAO_String_Manager::operator const char* (void) const
{
  return this->ptr_;
}

ACE_INLINE const char *
TAO_String_Manager::in (void) const
{
  return this->ptr_;
}

ACE_INLINE char *&
TAO_String_Manager::inout (void)
{
  return this->ptr_;
}

ACE_INLINE char *&
TAO_String_Manager::out (void)
{
  CORBA::string_free (this->ptr_);
  this->ptr_ = 0;
  return this->ptr_;
}

ACE_INLINE char *
TAO_String_Manager::_retn (void)
{
  char *temp = this->ptr_;
  this->ptr_ = 0;
  return temp;
}

// ****************************************************************

// default CTOR initializes to empty string
ACE_INLINE
TAO_WString_Manager::TAO_WString_Manager (void)
  :  ptr_ (0) // @@ CORBA::wstring_dup (""))
{
}

// copy ctor copies storage
ACE_INLINE
TAO_WString_Manager::TAO_WString_Manager (const TAO_WString_Manager &rhs)
  :  ptr_ (CORBA::wstring_dup (rhs.ptr_))
{
}

ACE_INLINE
TAO_WString_Manager::TAO_WString_Manager (const CORBA::WChar *ws)
  : ptr_ (CORBA::wstring_dup (ws))
{
}

// destructor
ACE_INLINE
TAO_WString_Manager::~TAO_WString_Manager (void)
{
  CORBA::wstring_free (this->ptr_);
}

// assignment
ACE_INLINE TAO_WString_Manager&
TAO_WString_Manager::operator= (const TAO_WString_Manager &rhs)
{
  // Strongly exception safe by means of copy and non-throwing swap
  // technique.
  TAO_WString_Manager tmp (rhs);

  CORBA::WChar * old_ptr = this->ptr_;
  this->ptr_ = tmp.ptr_;
  tmp.ptr_  = old_ptr;

  return *this;
}

// assignment from const CORBA::WChar* makes a copy
ACE_INLINE TAO_WString_Manager &
TAO_WString_Manager::operator= (const CORBA::WChar * p)
{
  // Strongly exception safe by means of copy and non-throwing swap
  // technique.
  TAO_WString_Manager tmp (p);

  CORBA::WChar * old_ptr = this->ptr_;
  this->ptr_ = tmp.ptr_;
  tmp.ptr_ = old_ptr;

  return *this;
}

// assignment from CORBA::WChar* owns the string
ACE_INLINE TAO_WString_Manager &
TAO_WString_Manager::operator= (CORBA::WChar * p)
{
  CORBA::wstring_free (this->ptr_);
  this->ptr_ = p;
  return *this;
}

ACE_INLINE
TAO_WString_Manager::operator const CORBA::WChar* (void) const
{
  return this->ptr_;
}

ACE_INLINE const CORBA::WChar *
TAO_WString_Manager::in (void) const
{
  return this->ptr_;
}

ACE_INLINE CORBA::WChar *&
TAO_WString_Manager::inout (void)
{
  return this->ptr_;
}

ACE_INLINE CORBA::WChar *&
TAO_WString_Manager::out (void)
{
  CORBA::wstring_free (this->ptr_);
  this->ptr_ = 0;
  return this->ptr_;
}

ACE_INLINE CORBA::WChar *
TAO_WString_Manager::_retn (void)
{
  CORBA::WChar *temp = this->ptr_;
  this->ptr_ = 0;
  return temp;
}

TAO_END_VERSIONED_NAMESPACE_DECL
