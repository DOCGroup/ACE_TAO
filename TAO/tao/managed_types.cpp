// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    managed_types.cpp
//
// = DESCRIPTION
//    C++ mapping of sequences of strings/objrefs or struct/union members with
//    strings/objrefs require a special managed type. These types are define
//    din this header file.

// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include "tao/corba.h"

TAO_String_ManagedType &
TAO_String_ManagedType::operator= (char *p)
{
  if (this->ptr_ != p)
    {
      if ((this->ptr_ != 0) && this->release_)
        CORBA::string_free (this->ptr_);
      this->ptr_ = p;
    }
  return *this;
}

TAO_String_ManagedType &
TAO_String_ManagedType::operator= (const char *p)
{
  if ((this->ptr_ != 0) && this->release_)
    CORBA::string_free (this->ptr_);

  this->ptr_ = CORBA::string_dup (p);
  return *this;
}

TAO_String_ManagedType &
TAO_String_ManagedType::operator= (const TAO_String_ManagedType& r)
{
  if ((this->ptr_ != 0) && this->release_)
    CORBA::string_free (this->ptr_);
  this->ptr_ = CORBA::string_dup (r.ptr_);
  return *this;
}
