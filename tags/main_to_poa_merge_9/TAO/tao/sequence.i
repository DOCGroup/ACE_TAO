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

// operations on the unbounded sequence class

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
    release_ (1)
{
}

ACE_INLINE
TAO_Base_Sequence::TAO_Base_Sequence (const TAO_Base_Sequence &rhs)
  : maximum_ (rhs.maximum_),
    length_ (rhs.length_),
    release_ (1)
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

#if 0
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
#endif

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
    }
  this->length_ = length;
}



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
  this->length_ = length;
}
