// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    managed_types.i
//
// = DESCRIPTION
//    C++ mapping of sequences of strings/objrefs or struct/union members with
//    strings/objrefs require a special managed type. These types are define
//    din this header file.

// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

// ----------------------------------------------------------------------
// TAO_String_ManagedType type
// ----------------------------------------------------------------------

// default constructor
ACE_INLINE
TAO_String_ManagedType::TAO_String_ManagedType (void)
  : ptr_ (0),
    release_ (1)
{
}

// destructor
ACE_INLINE
TAO_String_ManagedType::~TAO_String_ManagedType (void)
{
  if ((this->ptr_ != 0) && this->release_)
    CORBA::string_free (this->ptr_);
}

// construct from char *
ACE_INLINE
TAO_String_ManagedType::TAO_String_ManagedType (char *p, CORBA::Boolean release)
  : ptr_ (p),
    release_ (release)
{
}

// construct from const char*. Will make a copy. So if release is false, memory
// will leak
ACE_INLINE
TAO_String_ManagedType::TAO_String_ManagedType (const char *p, CORBA::Boolean release)
  : ptr_ (CORBA::string_dup ((char *) p)),
    release_ (release)
{
}

// copy constructor
ACE_INLINE
TAO_String_ManagedType::TAO_String_ManagedType (const TAO_String_ManagedType &r,
                                                CORBA::Boolean release)
{
  this->ptr_ = CORBA::string_dup (r.ptr_);
  this->release_ = release;
}

// accesor - read/write
ACE_INLINE CORBA::Char &
TAO_String_ManagedType::operator[] (CORBA::ULong index)
{
  // we need to verify bounds else raise some exception
  return this->ptr_[index];
}

// read only
ACE_INLINE CORBA::Char
TAO_String_ManagedType::operator[] (CORBA::ULong index) const
{
  // we need to verify bounds else raise some exception
  return this->ptr_[index];
}

// cast operator (read/write)
ACE_INLINE
TAO_String_ManagedType::operator char *()
{
  return this->ptr_;
}

// cast operator (read only)
ACE_INLINE
TAO_String_ManagedType::operator const char *() const
{
  return this->ptr_;
}

// the in, inout, out and _ retn methods for parameter passing

ACE_INLINE const char *
TAO_String_ManagedType::in (void) const
{
  return this->ptr_;
}

ACE_INLINE char *&
TAO_String_ManagedType::inout (void)
{
  return this->ptr_;
}

ACE_INLINE char *&
TAO_String_ManagedType::out (void)
{
  if (this->release_)
    CORBA::string_free (this->ptr_);
  this->ptr_ = 0;
  return this->ptr_;
}

ACE_INLINE char *
TAO_String_ManagedType::_retn (void)
{
  char *temp = this->ptr_;
  this->ptr_ = 0;
  return temp;
}
