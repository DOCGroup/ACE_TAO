/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    sequence.i
//
// = AUTHOR
//    Aniruddha Gokhale and Carlos O'Ryan
//
// ============================================================================

// Operations on the unbounded sequence class.

ACE_INLINE
TAO_Base_Sequence::TAO_Base_Sequence (void)
  : maximum_ (0),
    length_ (0),
    buffer_ (0),
    release_ (CORBA::B_FALSE)
{
}

ACE_INLINE
TAO_Base_Sequence::TAO_Base_Sequence (CORBA::ULong maximum,
                                      CORBA::ULong length,
                                      void *buffer,
                                      CORBA::Boolean release)
  : maximum_ (maximum),
    length_ (length),
    buffer_ (buffer),
    release_ (release)
{
}

ACE_INLINE
TAO_Base_Sequence::TAO_Base_Sequence (CORBA::ULong maximum,
                                      void *data)
  : maximum_ (maximum),
    length_ (0),
    buffer_ (data),
    release_ (CORBA::B_TRUE)
{
}

ACE_INLINE
TAO_Base_Sequence::TAO_Base_Sequence (const TAO_Base_Sequence &rhs)
  : maximum_ (rhs.maximum_),
    length_ (rhs.length_),
    release_ (CORBA::B_TRUE)
{
}

ACE_INLINE CORBA::Boolean
TAO_Base_Sequence::release (void) const
{
  return this->release_;
}

ACE_INLINE TAO_Base_Sequence &
TAO_Base_Sequence::operator= (const TAO_Base_Sequence &rhs)
{
  this->maximum_ = rhs.maximum_;
  this->length_ = rhs.length_;
  this->release_ = CORBA::B_TRUE;
  return *this;
}

ACE_INLINE CORBA::ULong
TAO_Base_Sequence::maximum (void) const
{
  return this->maximum_;
}

// ****************************************************************

ACE_INLINE
TAO_Unbounded_Base_Sequence::TAO_Unbounded_Base_Sequence (void)
{
}

ACE_INLINE
TAO_Unbounded_Base_Sequence::
TAO_Unbounded_Base_Sequence (CORBA::ULong maximum,
                             CORBA::ULong length,
                             void* buffer,
                             CORBA::Boolean release)
  :  TAO_Base_Sequence (maximum, length, buffer, release)
{
}

ACE_INLINE
TAO_Unbounded_Base_Sequence::
TAO_Unbounded_Base_Sequence (CORBA::ULong maximum,
                             void* buffer)
  :  TAO_Base_Sequence (maximum, buffer)
{
}

ACE_INLINE CORBA::ULong
TAO_Unbounded_Base_Sequence::length (void) const
{
  return this->length_;
}

ACE_INLINE void
TAO_Unbounded_Base_Sequence::length (CORBA::ULong length)
{
  if (length > this->maximum_)
    {
      this->_allocate_buffer (length);
      this->maximum_ = length;
      this->release_ = CORBA::B_TRUE;
    }
  else if (length < this->length_)
    {
      this->_shrink_buffer (length, this->length_);
    }
  this->length_ = length;
}

// ****************************************************************

ACE_INLINE
TAO_Bounded_Base_Sequence::TAO_Bounded_Base_Sequence (void)
{
}

ACE_INLINE
TAO_Bounded_Base_Sequence::
TAO_Bounded_Base_Sequence (CORBA::ULong maximum,
                           CORBA::ULong length,
                           void* buffer,
                           CORBA::Boolean release)
  :  TAO_Base_Sequence (maximum, length, buffer, release)
{
}

ACE_INLINE
TAO_Bounded_Base_Sequence::
TAO_Bounded_Base_Sequence (CORBA::ULong maximum,
                           void* buffer)
  :  TAO_Base_Sequence (maximum, buffer)
{
}

ACE_INLINE CORBA::ULong
TAO_Bounded_Base_Sequence::length (void) const
{
  return this->length_;
}

ACE_INLINE void
TAO_Bounded_Base_Sequence::length (CORBA::ULong length)
{
  if (length > this->maximum_)
    {
      return;
    }
  else if (length < this->length_)
    {
      this->_shrink_buffer (length, this->length_);
    }
  this->length_ = length;
}

// ****************************************************************

ACE_INLINE
TAO_String_Manager::TAO_String_Manager (char **buffer,
                                        CORBA::Boolean release)
  :  ptr_ (buffer),
     release_ (release)
{
}

ACE_INLINE
TAO_String_Manager::TAO_String_Manager(const TAO_String_Manager &rhs)
  :  ptr_ (rhs.ptr_),
     release_ (rhs.release_)
{
}

ACE_INLINE
TAO_String_Manager::~TAO_String_Manager (void)
{
}

ACE_INLINE
TAO_String_Manager::operator const char* (void) const
{
  return *this->ptr_;
}

// ****************************************************************

//default constructor
ACE_INLINE
TAO_Unbounded_String_Sequence::TAO_Unbounded_String_Sequence (void)
{
}

ACE_INLINE
TAO_Unbounded_String_Sequence::
TAO_Unbounded_String_Sequence (CORBA::ULong maximum,
                               CORBA::ULong length,
                               char* *value,
                               CORBA::Boolean release)
  : TAO_Unbounded_Base_Sequence (maximum, length, value, release)
{
}

ACE_INLINE TAO_String_Manager
TAO_Unbounded_String_Sequence::operator[] (CORBA::ULong index) const
{
  ACE_ASSERT (index < this->maximum_);
  char ** const tmp =
    ACE_reinterpret_cast (char ** ACE_CAST_CONST, this->buffer_);
  return TAO_String_Manager (tmp + index, this->release_);
}
