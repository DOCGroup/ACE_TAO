//
// $Id$
//

#include "tao/corba.h"

#if !defined (__ACE_INLINE__)
#include "tao/sequence.i"
#endif /* __ACE_INLINE__ */

// *************************************************************
// Operations for class TAO_Base_Sequence
// *************************************************************

TAO_Base_Sequence::~TAO_Base_Sequence (void)
{
}

void TAO_Base_Sequence::_shrink_buffer (CORBA::ULong, CORBA::ULong)
{
  // default is no op.
}

// *************************************************************
// Operations for class TAO_String_Manager
// *************************************************************

// assignment
TAO_String_Manager&
TAO_String_Manager::operator=(const TAO_String_Manager &rhs)
{
  if (this == &rhs)
    return *this;

  if (this->release_) // need to free old one
    {
      CORBA::string_free (*this->ptr_);
      *this->ptr_ = CORBA::string_dup (*rhs.ptr_);
    }
  else
    {
      *this->ptr_ = *rhs.ptr_;
    }
  return *this;
}

// assignment from char*
TAO_String_Manager &
TAO_String_Manager::operator= (const char * p)
{
  if (this->release_)
    {
      CORBA::string_free (*this->ptr_);
      *this->ptr_ = CORBA::string_dup (p);
    }
  else
    {
      *this->ptr_ = ACE_const_cast(char*,p);
    }
  return *this;
}

// *************************************************************

// constructor for unbounded seq
TAO_Unbounded_String_Sequence::
TAO_Unbounded_String_Sequence (CORBA::ULong maximum)
  : TAO_Unbounded_Base_Sequence (maximum,
				 TAO_Unbounded_String_Sequence::allocbuf (maximum))
{
}

TAO_Unbounded_String_Sequence::
TAO_Unbounded_String_Sequence (const TAO_Unbounded_String_Sequence &seq)
  : TAO_Unbounded_Base_Sequence (seq)
{
  this->buffer_ =
    TAO_Unbounded_String_Sequence::allocbuf (this->maximum_);

  char* *tmp1 = ACE_reinterpret_cast(char* *,this->buffer_);
  char* *tmp2 = ACE_reinterpret_cast(char* *,seq.buffer_);
  for (CORBA::ULong i = 0; i < seq.length_; ++i)
    tmp1 [i] = CORBA::string_dup (tmp2 [i]);

  this->release_ = 1;
}

TAO_Unbounded_String_Sequence::~TAO_Unbounded_String_Sequence (void)
{
  this->_deallocate_buffer ();
}

TAO_Unbounded_String_Sequence&
TAO_Unbounded_String_Sequence::
operator= (const TAO_Unbounded_String_Sequence &seq)
{
  if (this == &seq)
    return *this;

  if (this->release_)
    {
      char* *tmp = ACE_reinterpret_cast(char* *,this->buffer_);
      for (CORBA::ULong i = 0; i < this->length_; ++i)
	{
	  CORBA::string_free (tmp[i]);
	}
      if (this->maximum_ < seq.maximum_)
	{
	  this->buffer_ =
	    TAO_Unbounded_String_Sequence::allocbuf (seq.maximum_);
	}
    }
  else
    {
      this->buffer_ =
	TAO_Unbounded_String_Sequence::allocbuf (this->maximum_);
    }

  char* *tmp1 = ACE_reinterpret_cast(char* *,this->buffer_);
  char* *tmp2 = ACE_reinterpret_cast(char* *,seq.buffer_);
  CORBA::ULong i=0;
  for (; i < seq.length_; ++i)
    tmp1 [i] = CORBA::string_dup (tmp2 [i]);

  return *this;
}

char* *
TAO_Unbounded_String_Sequence::allocbuf (CORBA::ULong nelems)
{
  char* *buf = new char*[nelems];
  for (CORBA::ULong i=0; i < nelems; i++)
    buf[i] = 0;
  return buf;
}

void
TAO_Unbounded_String_Sequence::freebuf (char* *buffer)
{
  if (buffer == 0)
    return;

  // {orbos/97-05-15:16.11}
  // The freebuf function ensures that the destructor for each element
  // is called before the buffer is destroyed, except for string
  // elements, which are freed using string_free(), and object
  // reference elements, which are freed using release(). The freebuf
  // function will ignore null pointers passed to it.

  // @@ How are we supposed to implement that! We don't know the
  // length of the buffer here.
  //  Mark the length in the first four bytes? For the moment we let
  //  that be.

  char* *tmp = ACE_reinterpret_cast (char**,buffer);
  delete[] tmp;
}

void
TAO_Unbounded_String_Sequence::_allocate_buffer (CORBA::ULong length)
{
  char* *tmp;
  ACE_NEW (tmp, char* [length]);

  if (this->buffer_ != 0)
    {
      char* *old = ACE_reinterpret_cast(char**,this->buffer_);
      for (CORBA::ULong i = 0; i < this->length_; ++i)
	{
	  // Only call duplicate when we did not own the previous
	  // buffer, since after this method we own it we must also
	  // own the objects.  If we already own the objects there is
	  // no need to copy them, if we did we would also have to
	  // remove the old instances.
	  if (!this->release_)
	    tmp [i] = CORBA::string_dup (old[i]);
	  else
	    tmp [i] = old[i];
	}
      if (this->release_)
	delete[] old;
    }
  this->buffer_ = tmp;
}

void
TAO_Unbounded_String_Sequence::_deallocate_buffer (void)
{
  if (this->buffer_ == 0 || this->release_ == 0)
    return;
  char* *tmp = ACE_reinterpret_cast (char**,this->buffer_);
  for (CORBA::ULong i = 0;
       i < this->length_;
       ++i)
    {
      CORBA::string_free (tmp[i]);
    }
  delete[] tmp;
  this->buffer_ = 0;
}

void
TAO_Unbounded_String_Sequence::_shrink_buffer (CORBA::ULong nl,
					       CORBA::ULong ol)
{
  char* *tmp = ACE_reinterpret_cast (char**,this->buffer_);
  for (CORBA::ULong i = ol; i < nl; ++i)
    CORBA::string_free (tmp[i]);
}
