//
// $Id$
//

#include "tao/corba.h"

// *************************************************************
// Operations for class TAO_Base_Sequence
// *************************************************************

TAO_Base_Sequence::~TAO_Base_Sequence (void)
{
  if (this->release_)
    this->_deallocate_buffer ();
}

// *************************************************************
// Operations for class TAO_Unbounded_StringSequence::TAO_StrMngType
// *************************************************************

// destructor
TAO_Unbounded_StringSequence::TAO_StrMngType::~TAO_StrMngType (void)
{
}

// copy constructor
TAO_Unbounded_StringSequence::TAO_StrMngType::TAO_StrMngType
(const TAO_Unbounded_StringSequence::TAO_StrMngType &_tao_mng_type)
{
  *this->ptr_ = CORBA::string_dup (*_tao_mng_type.ptr_);
  this->release_ = _tao_mng_type.release_;
}

// assignment
TAO_Unbounded_StringSequence::TAO_StrMngType&
TAO_Unbounded_StringSequence::TAO_StrMngType::operator=
(const TAO_Unbounded_StringSequence::TAO_StrMngType &_tao_mng_type)
{
  if (this == &_tao_mng_type) return *this;
  if (this->release_) // need to free old one
    CORBA::string_free (*this->ptr_);
  *this->ptr_ = CORBA::string_dup (*_tao_mng_type.ptr_);
  return *this;
}

// assignment from char*
TAO_Unbounded_StringSequence::TAO_StrMngType &
TAO_Unbounded_StringSequence::TAO_StrMngType::operator= (char * p)
{
  if (this->release_) // need to free old one
    CORBA::string_free (*this->ptr_);
  *this->ptr_ = p; // no copy
  return *this;
}

TAO_Unbounded_StringSequence::TAO_StrMngType &
TAO_Unbounded_StringSequence::TAO_StrMngType::operator= (const char * p)
{
  if (this->release_) // need to free old one
  {
    CORBA::string_free (*this->ptr_);
    *this->ptr_ = CORBA::string_dup (p);// make copy
  }
  else // make no copy as we don't own it
    *this->ptr_ = (char *)p; // no copy
  return *this;
}

// mapping for variable size - out
char *&
TAO_Unbounded_StringSequence::TAO_StrMngType::out (void)
{
  if (this->release_)
    CORBA::string_free (*this->ptr_);
  *this->ptr_ = 0;
  return *this->ptr_;
}

// _retn (gives up ownership)
char *
TAO_Unbounded_StringSequence::TAO_StrMngType::_retn (void)
{
  char *tmp = *this->ptr_;
  *this->ptr_ = 0;
  return tmp;
}

// *************************************************************
// class TAO_Unbounded_StringSequence
// *************************************************************

// copy constructor
TAO_Unbounded_StringSequence::TAO_Unbounded_StringSequence
(const TAO_Unbounded_StringSequence &seq)
  : TAO_Base_Sequence (seq)
{
  this->buffer_ = TAO_Unbounded_StringSequence::allocbuf (this->maximum_);
  char **tmp = ACE_reinterpret_cast(char**,this->buffer_);
  for (CORBA::ULong i = 0; i < this->length_; ++i)
    tmp[i] = CORBA::string_dup (seq[i]);
}

// destructor
TAO_Unbounded_StringSequence::~TAO_Unbounded_StringSequence (void)
{
  if (this->release_) // we own the buffer
  {
    char **tmp = ACE_reinterpret_cast(char**,this->buffer_);
    TAO_Unbounded_StringSequence::freebuf (tmp, this->maximum_);
  }
}

// assignment operator
TAO_Unbounded_StringSequence&
TAO_Unbounded_StringSequence::operator=
(const TAO_Unbounded_StringSequence &seq)
{
  if (this == &seq) return *this;

  // first free all elements if the release flag is set
  if (this->release_)
  {
    char **tmp = ACE_reinterpret_cast (char**,this->buffer_);
    TAO_Unbounded_StringSequence::freebuf (tmp, this->maximum_);
  }

  this->TAO_Base_Sequence::operator= (seq); // set the underlying static
                                            // members
  // allocate memory according to the new maximum
  this->buffer_ = TAO_Unbounded_StringSequence::allocbuf (this->maximum_);
  char **tmp = ACE_reinterpret_cast(char**,this->buffer_);
  for (CORBA::ULong i = 0; i < this->length_; ++i)
    tmp[i] = CORBA::string_dup (seq[i]);

  return *this;
}

char **
TAO_Unbounded_StringSequence::allocbuf (CORBA::ULong nelems)
{
  char **buf = new char *[nelems]; // allocate
  // initialize to 0
  for (CORBA::ULong i=0; i < nelems; i++)
    buf[i] = (char *)0;
  return buf;
}

void
TAO_Unbounded_StringSequence::freebuf (char **buf, CORBA::ULong nelems)
{
  if (!buf) return; // null sequence
  for (CORBA::ULong i=0; i < nelems; i++)
  	CORBA::string_free (buf[i]);
  TAO_Unbounded_StringSequence::freebuf (buf);
}

void TAO_Unbounded_StringSequence::_allocate_buffer (CORBA::ULong length)
{
  char **tmp;
  ACE_NEW (tmp, char*[length]);

  if (this->buffer_ != 0)
    {
      char **old = ACE_reinterpret_cast(char**,this->buffer_);
      for (CORBA::ULong i = 0; i < this->length_; ++i)
	{
	  tmp[i] = CORBA::string_dup (old[i]);
	}
      delete[] old;
    }
  this->buffer_ = tmp;
}

void TAO_Unbounded_StringSequence::_deallocate_buffer (void)
{
  if (this->buffer_ == 0)
    return;

  char **tmp = ACE_reinterpret_cast (char**,this->buffer_);
  delete[] tmp;
  this->buffer_ = 0;
}
