// $Id$
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

// *************************************************************
// Inline operations for class TAO_Unbounded_StringSequence
// ::TAO_StrMngType
// *************************************************************

ACE_INLINE
TAO_Unbounded_StringSequence::
TAO_StrMngType::TAO_StrMngType (char **buffer,
                                CORBA::Boolean release)
  : ptr_ (buffer),
    release_ (release)
{
}

ACE_INLINE
TAO_Unbounded_StringSequence::
TAO_StrMngType::operator const char *() const // cast
{
  return *this->ptr_;
}

ACE_INLINE
TAO_Unbounded_StringSequence::
TAO_StrMngType::operator char * () // cast
{
  return *this->ptr_;
}

ACE_INLINE char&
TAO_Unbounded_StringSequence::
TAO_StrMngType::operator[] (CORBA::ULong index)
{
  return *this->ptr_[index];
}

ACE_INLINE const char&
TAO_Unbounded_StringSequence::
TAO_StrMngType::operator[] (CORBA::ULong index) const
{
  return *this->ptr_[index];
}

ACE_INLINE const char *
TAO_Unbounded_StringSequence::
TAO_StrMngType::in (void) const
{
  return *this->ptr_;
}

ACE_INLINE char *&
TAO_Unbounded_StringSequence::
TAO_StrMngType::inout (void)
{
  return *this->ptr_;
}

// *************************************************************
// class TAO_Unbounded_StringSequence
// *************************************************************

ACE_INLINE void
TAO_Unbounded_StringSequence::freebuf (char **seq)
{
  delete []seq;
}

//default constructor
ACE_INLINE
TAO_Unbounded_StringSequence::TAO_Unbounded_StringSequence (void)
{}

// constructor for unbounded seq taking the "max" value
ACE_INLINE
TAO_Unbounded_StringSequence::TAO_Unbounded_StringSequence (CORBA::ULong max)
  : TAO_Base_Sequence (max,
                       TAO_Unbounded_StringSequence::allocbuf (max))
{
}

// constructor from data buffer
ACE_INLINE
TAO_Unbounded_StringSequence::TAO_Unbounded_StringSequence
(CORBA::ULong max, CORBA::ULong length,
 char **value, CORBA::Boolean release)
  : TAO_Base_Sequence (max, length, value, release)
{
}

ACE_INLINE TAO_Unbounded_StringSequence::TAO_StrMngType
TAO_Unbounded_StringSequence::operator[] (CORBA::ULong index) // read/write
{
  return TAO_Unbounded_StringSequence::TAO_StrMngType
    (&((char **)this->buffer_)[index], this->release_);
}

ACE_INLINE const TAO_Unbounded_StringSequence::TAO_StrMngType
TAO_Unbounded_StringSequence::operator[] (CORBA::ULong index) const // read
{
  return TAO_Unbounded_StringSequence::TAO_StrMngType
    (&((char **)this->buffer_)[index], this->release_);
}

ACE_INLINE int
TAO_Unbounded_StringSequence::_bounded (void) const
{
  return 0;
}
