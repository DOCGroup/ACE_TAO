//
// $Id$
//

#include "tao/corba.h"

// *************************************************************
// Operations for class TAO_Base_Sequence
// *************************************************************

TAO_Base_Sequence::~TAO_Base_Sequence (void)
{
}

// *************************************************************
// Operations for class TAO_String_Manager
// *************************************************************

// destructor
TAO_String_Manager::~TAO_String_Manager (void)
{
}

// copy constructor
TAO_String_Manager::TAO_String_Manager(const TAO_String_Manager &rhs)
{
  *this->ptr_ = CORBA::string_dup (*rhs.ptr_);
  this->release_ = 1;
}

// assignment
TAO_String_Manager&
TAO_String_Manager::operator=(const TAO_String_Manager &rhs)
{
  if (this == &rhs)
    return *this;

  if (this->release_) // need to free old one
    CORBA::string_free (*this->ptr_);

  *this->ptr_ = CORBA::string_dup (*rhs.ptr_);
  this->release_ = 1;
  return *this;
}

// assignment from char*
TAO_String_Manager &
TAO_String_Manager::operator= (char * p)
{
  if (this->release_) // need to free old one
    CORBA::string_free (*this->ptr_);

  *this->ptr_ = p; // no copy
  this->release_ = 0;

  return *this;
}

TAO_String_Manager &
TAO_String_Manager::operator= (const char * p)
{
  if (this->release_) // need to free old one
    {
      CORBA::string_free (*this->ptr_);
      *this->ptr_ = CORBA::string_dup (p);
      this->release_ = 1;
    }
  else
    {
      *this->ptr_ = (char *)p;
      this->release_ = 0;
    }
  return *this;
}

// mapping for variable size - out
char *&
TAO_String_Manager::out (void)
{
  if (this->release_)
    CORBA::string_free (*this->ptr_);
  *this->ptr_ = 0;
  return *this->ptr_;
}

// _retn (gives up ownership)
char *
TAO_String_Manager::_retn (void)
{
  char *tmp = *this->ptr_;
  *this->ptr_ = 0;
  return tmp;
}
