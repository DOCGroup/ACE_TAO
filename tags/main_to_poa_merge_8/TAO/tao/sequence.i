/* -*- C++ -*- */

// ============================================================================
//
// = LIBRARY
//    TAO
// 
// = FILENAME
//    sequence.i
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems Inc.
//
//    Aniruddha Gokhale
// 
// ============================================================================

// operations on the unbounded sequence class

ACE_INLINE
TAO_Base_Sequence::TAO_Base_Sequence (void)
  : maximum_ (0),
    length_ (0),
    release_ (0),
    buffer_ (0)
{
}

ACE_INLINE
TAO_Base_Sequence::TAO_Base_Sequence (CORBA::ULong maximum,
				      CORBA::ULong length,
				      void* buffer,
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

ACE_INLINE TAO_Base_Sequence &
TAO_Base_Sequence::operator= (const TAO_Base_Sequence &rhs)
{
  if (this != &rhs)
    {
      this->maximum_ = rhs.maximum_;
      this->length_ = rhs.length_;
      this->release_ = CORBA::B_TRUE;
    }
  return *this;
}

ACE_INLINE CORBA::ULong
TAO_Base_Sequence::maximum (void) const
{
  return this->maximum_;
}

ACE_INLINE void
TAO_Base_Sequence::length (CORBA::ULong length)
{
  if (length > this->maximum_)
    {
      if (this->_bounded ())
	{
	  // @@ TODO maybe we should throw?
	  return;
	}
      this->_allocate_buffer (length);
      this->maximum_ = length;
    }
  this->length_ = length;
}

ACE_INLINE CORBA::ULong
TAO_Base_Sequence::length (void) const
{
  return this->length_;
}
