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
TAO_Unbounded_Sequence<T>::TAO_Unbounded_Sequence (void)
{
}

template <class T> ACE_INLINE
TAO_Unbounded_Sequence<T>::TAO_Unbounded_Sequence (CORBA::ULong maximum)
  : TAO_Base_Sequence (maximum,
		       TAO_Unbounded_Sequence<T>::allocbuf (maximum))
{
}

template <class T> ACE_INLINE
TAO_Unbounded_Sequence<T>::TAO_Unbounded_Sequence (CORBA::ULong maximum,
						   CORBA::ULong length,
						   T *data,
						   CORBA::Boolean release)
  : TAO_Base_Sequence (maximum, length, data, release)
{
}

template <class T> ACE_INLINE
TAO_Unbounded_Sequence<T>::TAO_Unbounded_Sequence (const TAO_Unbounded_Sequence<T> &rhs)
  : TAO_Base_Sequence (rhs),
{
  this->buffer_ = TAO_Unbounded_Sequence<T>::allocbuf (this->maximum_);
  T* tmp = ACE_reinterpret_cast(T*,this->buffer);
  for (CORBA::ULong i = 0; i < this->length_; ++i)
    tmp[i] = rhs[i];
}

template <class T> ACE_INLINE TAO_Unbounded_Sequence<T> &
TAO_Unbounded_Sequence<T>::operator= (const TAO_Unbounded_Sequence<T> &rhs)
{
  if (this != &rhs)
    {
      this->TAO_Base_Sequence::operator= (rhs);
      T* tmp = ACE_reinterpret_cast(T*,this->buffer);
      for (CORBA::ULong i = 0; i < this->length_; ++i)
	tmp[i] = rhs[i];
    }
  return *this;
}

template <class T> ACE_INLINE T &
TAO_Unbounded_Sequence<T>::operator[] (CORBA::ULong i)
{
  ACE_ASSERT (i < this->length_);
  T* tmp = ACE_reinterpret_cast(T*,this->buffer);
  return tmp[i];
}

template <class T> ACE_INLINE const T &
TAO_Unbounded_Sequence<T>::operator[] (CORBA::ULong i) const
{
  ACE_ASSERT (i < this->length_);
  T* tmp = ACE_reinterpret_cast(T*,this->buffer);
  return tmp[i];
}

template <class T> ACE_INLINE T *
TAO_Unbounded_Sequence<T>::allocbuf (CORBA::ULong size)
{
  return new T[size];
}

template <class T> ACE_INLINE void
TAO_Unbounded_Sequence<T>::freebuf (T *buffer)
{
  delete [] buffer;
}

// operations on the Bounded sequence class

template <class T, CORBA::ULong MAX> ACE_INLINE
TAO_Bounded_Sequence<T,MAX>::TAO_Bounded_Sequence (void)
{
}

template <class T, CORBA::ULong MAX> ACE_INLINE
TAO_Bounded_Sequence<T,MAX>::TAO_Bounded_Sequence (CORBA::ULong length,
						   T *data,
						   CORBA::Boolean release) 
  : TAO_Base_Sequence (length, MAX, data, release)
{
}

template <class T, CORBA::ULong MAX> ACE_INLINE
TAO_Bounded_Sequence<T,MAX>::TAO_Bounded_Sequence (const TAO_Bounded_Sequence<T,MAX> &rhs)
  : TAO_Base_Sequence (rhs)
{
  this->buffer_ = TAO_Bounded_Sequence<T,MAX>::allocbuf (MAX);
  T* tmp = ACE_reinterpret_cast(T*,this->buffer);
  for (CORBA::ULong i = 0; i < this->length_; ++i)
    tmp[i] = rhs[i];
}

template <class T, CORBA::ULong MAX> ACE_INLINE TAO_Bounded_Sequence<T,MAX> &
TAO_Bounded_Sequence<T,MAX>::operator= (const TAO_Bounded_Sequence<T,MAX> &rhs)
{
  if (this != &rhs)
    {
      this->TAO_Base_Sequence::operator= (rhs);
      T* tmp = ACE_reinterpret_cast(T*,this->buffer);
      for (CORBA::ULong i = 0; i < this->length_; ++i)
	tmp[i] = seq[i];
    }
  return *this;
}

template <class T, CORBA::ULong MAX> ACE_INLINE T &
TAO_Bounded_Sequence<T,MAX>::operator[] (CORBA::ULong i)
{
  ACE_ASSERT (i < this->length_);
  T* tmp = ACE_reinterpret_cast(T*,this->buffer);
  return tmp[i];
}

template <class T, CORBA::ULong MAX> ACE_INLINE const T &
TAO_Bounded_Sequence<T,MAX>::operator[] (CORBA::ULong i) const
{
  ACE_ASSERT (i < this->length_);
  T* tmp = ACE_reinterpret_cast(T*,this->buffer);
  return tmp[i];
}

template <class T, CORBA::ULong MAX> ACE_INLINE T *
TAO_Bounded_Sequence<T,MAX>::allocbuf (CORBA::ULong size)
{
  return new T[size];
}

template <class T, CORBA::ULong MAX> ACE_INLINE void
TAO_Bounded_Sequence<T,MAX>::freebuf (T *buffer)
{
  delete [] buffer;
}
