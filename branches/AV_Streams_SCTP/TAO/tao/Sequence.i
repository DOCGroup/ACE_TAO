/* -*- C++ -*- */
// $Id$

// Operations on the unbounded sequence class.

ACE_INLINE
TAO_Base_Sequence::TAO_Base_Sequence (void)
  : maximum_ (0),
    length_ (0),
    buffer_ (0),
    release_ (0)
{
}

ACE_INLINE
TAO_Base_Sequence::TAO_Base_Sequence (CORBA::ULong maximum,
                                      CORBA::ULong length,
                                      void * buffer,
                                      CORBA::Boolean release)
  : maximum_ (maximum),
    length_ (length),
    buffer_ (buffer),
    release_ (release)
{
}

ACE_INLINE
TAO_Base_Sequence::TAO_Base_Sequence (CORBA::ULong maximum,
                                      void * data)
  : maximum_ (maximum),
    length_ (0),
    buffer_ (data),
    release_ (1)
{
}

ACE_INLINE
TAO_Base_Sequence::TAO_Base_Sequence (const TAO_Base_Sequence & rhs)
  : maximum_ (rhs.maximum_),
    length_ (rhs.length_),
    buffer_ (0),
    release_ (1)
{
}

ACE_INLINE 
CORBA::Boolean
TAO_Base_Sequence::release (void) const
{
  return this->release_;
}

ACE_INLINE 
TAO_Base_Sequence &
TAO_Base_Sequence::operator= (const TAO_Base_Sequence & rhs)
{
  this->maximum_ = rhs.maximum_;
  this->length_ = rhs.length_;
  this->release_ = 1;
  return *this;
}

ACE_INLINE 
CORBA::ULong
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
TAO_Unbounded_Base_Sequence::TAO_Unbounded_Base_Sequence (
    CORBA::ULong maximum,
    CORBA::ULong length,
    void * buffer,
    CORBA::Boolean release
  )
  :  TAO_Base_Sequence (maximum, length, buffer, release)
{
}

ACE_INLINE
TAO_Unbounded_Base_Sequence::TAO_Unbounded_Base_Sequence (
    CORBA::ULong maximum,
    void * buffer
  )
  :  TAO_Base_Sequence (maximum, buffer)
{
}

ACE_INLINE 
CORBA::ULong
TAO_Unbounded_Base_Sequence::length (void) const
{
  return this->length_;
}

ACE_INLINE 
void
TAO_Unbounded_Base_Sequence::length (CORBA::ULong length)
{
  if (length > this->maximum_)
    {
      this->_allocate_buffer (length);
      this->maximum_ = length;
      this->release_ = 1;
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
TAO_Bounded_Base_Sequence::TAO_Bounded_Base_Sequence (
    CORBA::ULong maximum,
    CORBA::ULong length,
    void * buffer,
    CORBA::Boolean release
  )
  :  TAO_Base_Sequence (maximum, length, buffer, release)
{
}

ACE_INLINE
TAO_Bounded_Base_Sequence::TAO_Bounded_Base_Sequence (
    CORBA::ULong maximum,
    void * buffer
  )
  :  TAO_Base_Sequence (maximum, buffer)
{
}

ACE_INLINE 
CORBA::ULong
TAO_Bounded_Base_Sequence::length (void) const
{
  return this->length_;
}

ACE_INLINE 
void
TAO_Bounded_Base_Sequence::length (CORBA::ULong length)
{
  if (this->buffer_ == 0)
    {
      this->_allocate_buffer (this->maximum_);
      this->release_ = 1;
    }

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
TAO_Unbounded_String_Sequence::TAO_Unbounded_String_Sequence (void)
{
}

ACE_INLINE
TAO_Unbounded_String_Sequence::TAO_Unbounded_String_Sequence (
    CORBA::ULong maximum,
    CORBA::ULong length,
    char ** value,
    CORBA::Boolean release
  )
  : TAO_Unbounded_Base_Sequence (maximum, length, value, release)
{
}

ACE_INLINE
TAO_Unbounded_WString_Sequence::TAO_Unbounded_WString_Sequence (void)
{
}

ACE_INLINE
TAO_Unbounded_WString_Sequence::TAO_Unbounded_WString_Sequence (
    CORBA::ULong maximum,
    CORBA::ULong length,
    CORBA::WChar ** value,
    CORBA::Boolean release
  )
  : TAO_Unbounded_Base_Sequence (maximum, length, value, release)
{
}

// ****************************************************************

ACE_INLINE 
CORBA::Octet *
TAO_Unbounded_Sequence<CORBA::Octet>::allocbuf (CORBA::ULong size)
{
  return new CORBA::Octet[size];
}

ACE_INLINE 
void
TAO_Unbounded_Sequence<CORBA::Octet>::freebuf (CORBA::Octet * buffer)
{
  delete [] buffer;
}

ACE_INLINE
TAO_Unbounded_Sequence<CORBA::Octet>::TAO_Unbounded_Sequence (void)
#if (TAO_NO_COPY_OCTET_SEQUENCES == 1)
  :  mb_ (0)
#endif /* TAO_NO_COPY_OCTET_SEQUENCES == 1 */
{
}

ACE_INLINE
TAO_Unbounded_Sequence<CORBA::Octet>::TAO_Unbounded_Sequence (
    CORBA::ULong maximum
  )
  : TAO_Unbounded_Base_Sequence (
        maximum,
        TAO_Unbounded_Sequence<CORBA::Octet>::allocbuf (maximum)
      )
#if (TAO_NO_COPY_OCTET_SEQUENCES == 1)
  ,  mb_ (0)
#endif /* TAO_NO_COPY_OCTET_SEQUENCES == 1 */
{
}

ACE_INLINE
TAO_Unbounded_Sequence<CORBA::Octet>::TAO_Unbounded_Sequence (
    CORBA::ULong maximum,
    CORBA::ULong length,
    CORBA::Octet * data,
    CORBA::Boolean release
  )
  : TAO_Unbounded_Base_Sequence (maximum, length, data, release)
#if (TAO_NO_COPY_OCTET_SEQUENCES == 1)
  , mb_ (0)
#endif /* TAO_NO_COPY_OCTET_SEQUENCES == 1 */
{
}

ACE_INLINE 
const CORBA::Octet *
TAO_Unbounded_Sequence<CORBA::Octet>::get_buffer (void) const
{
  return ACE_reinterpret_cast (const CORBA::Octet * ACE_CAST_CONST, 
                               this->buffer_);
}

ACE_INLINE 
CORBA::Octet &
TAO_Unbounded_Sequence<CORBA::Octet>::operator[] (CORBA::ULong i)
{
  TAO_SEQUENCE_ASSERT (i, this->maximum_);
  CORBA::Octet * tmp = ACE_reinterpret_cast(CORBA::Octet *, this->buffer_);
  return tmp[i];
}

ACE_INLINE 
const CORBA::Octet &
TAO_Unbounded_Sequence<CORBA::Octet>::operator[] (CORBA::ULong i) const
{
  TAO_SEQUENCE_ASSERT (i, this->maximum_);
  CORBA::Octet * const tmp = 
    ACE_reinterpret_cast (CORBA::Octet * ACE_CAST_CONST, this->buffer_);
  return tmp[i];
}

#if (TAO_NO_COPY_OCTET_SEQUENCES == 1)
ACE_INLINE 
ACE_Message_Block *
TAO_Unbounded_Sequence<CORBA::Octet>::mb (void) const
{
  return this->mb_;
}
#endif /* TAO_NO_COPY_OCTET_SEQUENCES == 1 */
