/* -*- C++ -*- */
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
  if (this == &rhs)
    return *this;

  CORBA::string_free (this->ptr_);
  this->ptr_ = CORBA::string_dup (rhs.ptr_);
  return *this;
}

// assignment from const char* makes a copy
ACE_INLINE TAO_String_Manager &
TAO_String_Manager::operator= (const char * p)
{
  CORBA::string_free (this->ptr_);
  this->ptr_ = CORBA::string_dup (p);
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

// copy ctor
ACE_INLINE
TAO_SeqElem_String_Manager::
TAO_SeqElem_String_Manager (const TAO_SeqElem_String_Manager &rhs)
  :  ptr_ (rhs.ptr_),
     release_ (rhs.release_)
{
}

ACE_INLINE
TAO_SeqElem_String_Manager::TAO_SeqElem_String_Manager (char **buffer,
                                                        CORBA::Boolean release)
  :  ptr_ (buffer),
     release_ (release)
{
}

ACE_INLINE
TAO_SeqElem_String_Manager::~TAO_SeqElem_String_Manager (void)
{
}

// assignment
ACE_INLINE TAO_SeqElem_String_Manager&
TAO_SeqElem_String_Manager::operator= (const TAO_SeqElem_String_Manager &rhs)
{
  if (this == &rhs)
    return *this;

  if (this->release_)
    CORBA::string_free (*this->ptr_);
  *this->ptr_ = CORBA::string_dup (*rhs.ptr_);
  return *this;
}

// assignment from const char* will make copy
ACE_INLINE TAO_SeqElem_String_Manager &
TAO_SeqElem_String_Manager::operator= (const char *p)
{
  if (this->release_)
    CORBA::string_free (*this->ptr_);
  *this->ptr_ = CORBA::string_dup (p);
  return *this;
}

// assignment from char* will own it
ACE_INLINE TAO_SeqElem_String_Manager &
TAO_SeqElem_String_Manager::operator= (char *p)
{
  if (this->release_)
    CORBA::string_free (*this->ptr_);
  *this->ptr_ = p;
  return *this;
}

ACE_INLINE
TAO_SeqElem_String_Manager::operator const char* (void) const
{
  return *this->ptr_;
}

ACE_INLINE const char *
TAO_SeqElem_String_Manager::in (void) const
{
  return *this->ptr_;
}

ACE_INLINE char *&
TAO_SeqElem_String_Manager::inout (void)
{
  return *this->ptr_;
}

ACE_INLINE char *&
TAO_SeqElem_String_Manager::out (void)
{
  CORBA::string_free (*this->ptr_);
  *this->ptr_ = 0;
  return *this->ptr_;
}

ACE_INLINE char *
TAO_SeqElem_String_Manager::_retn (void)
{
  char *temp = *this->ptr_;
  *this->ptr_ = 0;
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
  if (this == &rhs)
    return *this;

  CORBA::wstring_free (this->ptr_);
  this->ptr_ = CORBA::wstring_dup (rhs.ptr_);
  return *this;
}

// assignment from const CORBA::WChar* makes a copy
ACE_INLINE TAO_WString_Manager &
TAO_WString_Manager::operator= (const CORBA::WChar * p)
{
  CORBA::wstring_free (this->ptr_);
  this->ptr_ = CORBA::wstring_dup (p);
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

// ****************************************************************

// copy ctor
ACE_INLINE
TAO_SeqElem_WString_Manager::
TAO_SeqElem_WString_Manager (const TAO_SeqElem_WString_Manager &rhs)
  :  ptr_ (rhs.ptr_),
     release_ (rhs.release_)
{
}

ACE_INLINE
TAO_SeqElem_WString_Manager::
    TAO_SeqElem_WString_Manager (CORBA::WChar **buffer,
                                 CORBA::Boolean release)
  :  ptr_ (buffer),
     release_ (release)
{
}

ACE_INLINE
TAO_SeqElem_WString_Manager::~TAO_SeqElem_WString_Manager (void)
{
}

// assignment
ACE_INLINE TAO_SeqElem_WString_Manager&
TAO_SeqElem_WString_Manager::operator= (const TAO_SeqElem_WString_Manager &rhs)
{
  if (this == &rhs)
    return *this;

  if (this->release_)
    CORBA::wstring_free (*this->ptr_);
  *this->ptr_ = CORBA::wstring_dup (*rhs.ptr_);
  return *this;
}

// assignment from const CORBA::WChar* will make copy
ACE_INLINE TAO_SeqElem_WString_Manager &
TAO_SeqElem_WString_Manager::operator= (const CORBA::WChar *p)
{
  if (this->release_)
    CORBA::wstring_free (*this->ptr_);
  *this->ptr_ = CORBA::wstring_dup (p);
  return *this;
}

// assignment from CORBA::WChar* will own it
ACE_INLINE TAO_SeqElem_WString_Manager &
TAO_SeqElem_WString_Manager::operator= (CORBA::WChar *p)
{
  if (this->release_)
    CORBA::wstring_free (*this->ptr_);
  *this->ptr_ = p;
  return *this;
}

ACE_INLINE
TAO_SeqElem_WString_Manager::operator const CORBA::WChar* (void) const
{
  return *this->ptr_;
}

ACE_INLINE const CORBA::WChar *
TAO_SeqElem_WString_Manager::in (void) const
{
  return *this->ptr_;
}

ACE_INLINE CORBA::WChar *&
TAO_SeqElem_WString_Manager::inout (void)
{
  return *this->ptr_;
}

ACE_INLINE CORBA::WChar *&
TAO_SeqElem_WString_Manager::out (void)
{
  CORBA::wstring_free (*this->ptr_);
  *this->ptr_ = 0;
  return *this->ptr_;
}

ACE_INLINE CORBA::WChar *
TAO_SeqElem_WString_Manager::_retn (void)
{
  CORBA::WChar *temp = *this->ptr_;
  *this->ptr_ = 0;
  return temp;
}
