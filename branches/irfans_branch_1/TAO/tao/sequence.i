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

template <class T> ACE_INLINE
TAO_UnboundedSeq<T>::TAO_UnboundedSeq (void)
  : maximum_ (0),
    length_ (0),
    release_ (0),
    buffer_ (0)
{
}

template <class T> ACE_INLINE
TAO_UnboundedSeq<T>::TAO_UnboundedSeq (CORBA::ULong max)
  : maximum_ (max),
    length_ (0),
    release_ (1) // we own it
{
  buffer_ = TAO_UnboundedSeq<T>::allocbuf (max);
}

template <class T> ACE_INLINE
TAO_UnboundedSeq<T>::TAO_UnboundedSeq (CORBA::ULong max, CORBA::ULong length, T
                                    *data, CORBA::Boolean release)
  : maxium_ (max),
    length_ (length),
    buffer_ (data),
    release_ (release)
{
}

template <class T> ACE_INLINE
TAO_UnboundedSeq<T>::TAO_UnboundedSeq (const TAO_UnboundedSeq<T> &seq)
  : maximum_ (seq.maximum ()),
    length_ (seq.length ()),
    release_ (1) // we own this
{
  this->buffer_ = TAO_UnboundedSeq<T>::allocbuf (this->maximum_);
  for (CORBA::ULong i=0; i < this->length_; i++)
    this->buffer_[i] = seq[i];
}

template <class T> ACE_INLINE
TAO_UnboundedSeq<T>::~TAO_UnboundedSeq (void)
{
  if (this->release_)
    TAO_UnboundedSeq<T>::freebuf (this->buffer_);

}

template <class T> ACE_INLINE TAO_UnboundedSeq<T> &
TAO_UnboundedSeq<T>::operator= (const TAO_UnboundedSeq<T> &seq)
{
  this->maximum_ = seq.maximum_;
  this->length_ = seq.length_;
  this->release_ = 1;
  this->buffer_ = TAO_UnboundedSeq<T>::allocbuf (this->maximum_);
  for (CORBA::ULong i=0; i < this->length_; i++)
    this->buffer_[i] = seq[i];
}

template <class T> ACE_INLINE CORBA::ULong
TAO_UnboundedSeq<T>::maximum (void) const
{
  return this->maximum_;
}

template <class T> ACE_INLINE void
TAO_UnboundedSeq<T>::length (CORBA::ULong length)
{
  this->length_ = length;
}

template <class T> ACE_INLINE CORBA::ULong
TAO_UnboundedSeq<T>::length (void) const
{
  return this->length_;
}

template <class T> ACE_INLINE T &
TAO_UnboundedSeq<T>::operator[] (CORBA::ULong i)
{
  ACE_ASSERT (i < this->length_);
  return this->buffer_[i];
}

template <class T> ACE_INLINE const T &
TAO_UnboundedSeq<T>::operator[] (CORBA::ULong i) const
{
  ACE_ASSERT (i < this->length_);
  return this->buffer_[i];
}

template <class T> ACE_INLINE T *
TAO_UnboundedSeq<T>::allocbuf (CORBA::ULong size)
{
  return new T[size];
}

template <class T> ACE_INLINE void
TAO_UnboundedSeq<T>::freebuf (T *buffer)
{
  delete [] buffer;
}


// operations on the Bounded sequence class

template <class T, CORBA::ULong size> ACE_INLINE
TAO_BoundedSeq<T,size>::TAO_BoundedSeq (void)
  : length_ (0),
    release_ (0),
    buffer_ (0)
{
}

template <class T, CORBA::ULong size> ACE_INLINE
TAO_BoundedSeq<T,size>::TAO_BoundedSeq (CORBA::ULong length, T *data, CORBA::Boolean
                                   release) 
  : length_ (length),
    buffer_ (data),
    release_ (release)
{
}

template <class T, CORBA::ULong size> ACE_INLINE
TAO_BoundedSeq<T,size>::TAO_BoundedSeq (const TAO_BoundedSeq<T,size> &seq)
  : length_ (seq.length ()),
    release_ (1) // we own this
{
  this->buffer_ = TAO_BoundedSeq<T,size>::allocbuf (this->length_);
  for (CORBA::ULong i=0; i < this->length_; i++)
    this->buffer_[i] = seq[i];
}

template <class T, CORBA::ULong size> ACE_INLINE
TAO_BoundedSeq<T,size>::~TAO_BoundedSeq (void)
{
  if (this->release_)
    TAO_BoundedSeq<T,size>::freebuf (this->buffer_);

}

template <class T, CORBA::ULong size> ACE_INLINE TAO_BoundedSeq<T,size> &
TAO_BoundedSeq<T,size>::operator= (const TAO_BoundedSeq<T,size> &seq)
{
  this->length_ = seq.length_;
  this->release_ = 1;
  this->buffer_ = TAO_BoundedSeq<T,size>::allocbuf (this->length_);
  for (CORBA::ULong i=0; i < this->length_; i++)
    this->buffer_[i] = seq[i];
}

template <class T, CORBA::ULong size> ACE_INLINE CORBA::ULong
TAO_BoundedSeq<T,size>::maximum (void) const
{
  return this->length_;
}

template <class T, CORBA::ULong size> ACE_INLINE void
TAO_BoundedSeq<T,size>::length (CORBA::ULong length)
{
  this->length_ = length;
}

template <class T, CORBA::ULong size> ACE_INLINE CORBA::ULong
TAO_BoundedSeq<T,size>::length (void) const
{
  return this->length_;
}

template <class T, CORBA::ULong size> ACE_INLINE T &
TAO_BoundedSeq<T,size>::operator[] (CORBA::ULong i)
{
  ACE_ASSERT (i < this->length_);
  return this->buffer_[i];
}

template <class T, CORBA::ULong size> ACE_INLINE const T &
TAO_BoundedSeq<T,size>::operator[] (CORBA::ULong i) const
{
  ACE_ASSERT (i < this->length_);
  return this->buffer_[i];
}

template <class T, CORBA::ULong size> ACE_INLINE T *
TAO_BoundedSeq<T,size>::allocbuf (CORBA::ULong size)
{
  return new T[size];
}

template <class T, CORBA::ULong size> ACE_INLINE void
TAO_BoundedSeq<T,size>::freebuf (T *buffer)
{
  delete [] buffer;
}

