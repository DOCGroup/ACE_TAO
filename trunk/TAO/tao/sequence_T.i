// $Id$
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
//    Aniruddha Gokhale and Carlos O'Ryan
//
// ============================================================================

// ***************************************************
// operations on the generic unbounded sequence class
// ***************************************************

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

template <class T> ACE_INLINE T &
TAO_Unbounded_Sequence<T>::operator[] (CORBA::ULong i)
{
  ACE_ASSERT (i < this->length_);
  T* tmp = ACE_reinterpret_cast(T*,this->buffer_);
  return tmp[i];
}

template <class T> ACE_INLINE const T &
TAO_Unbounded_Sequence<T>::operator[] (CORBA::ULong i) const
{
  ACE_ASSERT (i < this->length_);
  T* tmp = ACE_reinterpret_cast(T*,this->buffer_);
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

template<class T> ACE_INLINE int
TAO_Unbounded_Sequence<T>::_bounded (void) const
{
  return 0;
}

// ***************************************************
// operations on the generic Bounded sequence class
// ***************************************************

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
  T* tmp = ACE_reinterpret_cast(T*,this->buffer_);
  for (CORBA::ULong i = 0; i < this->length_; ++i)
    tmp[i] = rhs[i];
}

template <class T, CORBA::ULong MAX> ACE_INLINE TAO_Bounded_Sequence<T,MAX> &
TAO_Bounded_Sequence<T,MAX>::operator= (const TAO_Bounded_Sequence<T,MAX> &rhs)
{
  if (this != &rhs)
    {
      this->TAO_Base_Sequence::operator= (rhs);
      T* tmp = ACE_reinterpret_cast(T*,this->buffer_);
      for (CORBA::ULong i = 0; i < this->length_; ++i)
	tmp[i] = seq[i];
    }
  return *this;
}

template <class T, CORBA::ULong MAX> ACE_INLINE T &
TAO_Bounded_Sequence<T,MAX>::operator[] (CORBA::ULong i)
{
  ACE_ASSERT (i < this->length_);
  T* tmp = ACE_reinterpret_cast(T*,this->buffer_);
  return tmp[i];
}

template <class T, CORBA::ULong MAX> ACE_INLINE const T &
TAO_Bounded_Sequence<T,MAX>::operator[] (CORBA::ULong i) const
{
  ACE_ASSERT (i < this->length_);
  T* tmp = ACE_reinterpret_cast(T*,this->buffer_);
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

template<class T, CORBA::ULong MAX> ACE_INLINE int
TAO_Bounded_Sequence<T,MAX>::_bounded (void) const
{
  return 0;
}

// *************************************************************
// Inline operations for class TAO_Object_Manager<T>
// *************************************************************

template <class T> ACE_INLINE
TAO_Object_Manager<T>::TAO_Object_Manager(T** buffer, CORBA::Boolean release)
  : ptr_ (buffer),
    release_ (release)
{
}

template <class T> ACE_INLINE
TAO_Object_Manager<T>::operator const T* &() const // cast
{
  return *this->ptr_;
}

template <class T> ACE_INLINE
TAO_Object_Manager<T>::operator T* &() // cast
{
  return *this->ptr_;
}

template <class T> ACE_INLINE const T*
TAO_Object_Manager<T>::in (void) const
{
  return *this->ptr_;
}

template <class T> ACE_INLINE T*&
TAO_Object_Manager<T>::inout (void)
{
  return *this->ptr_;
}

// *************************************************************
// class TAO_Unbounded_Managed_Sequence
// *************************************************************

template <class T, class Manager> ACE_INLINE void
TAO_Unbounded_Managed_Sequence<T,Manager>::freebuf (T* *seq)
{
  delete []seq;
}

//default constructor
template <class T, class Manager> ACE_INLINE
TAO_Unbounded_Managed_Sequence<T,Manager>::TAO_Unbounded_Managed_Sequence (void)
{}

// constructor for unbounded seq
template <class T, class Manager> ACE_INLINE
TAO_Unbounded_Managed_Sequence<T,Manager>::TAO_Unbounded_Managed_Sequence
(CORBA::ULong max )
  : TAO_Base_Sequence (max, TAO_Unbounded_Managed_Sequence<T,Manager>::allocbuf (max))
{
}

// constructor from data buffer
template <class T, class Manager> ACE_INLINE
TAO_Unbounded_Managed_Sequence<T,Manager>::TAO_Unbounded_Managed_Sequence
(CORBA::ULong max, CORBA::ULong length,	T* *value, CORBA::Boolean release)
  : TAO_Base_Sequence (max, length, value, release)
{
}

template <class T, class Manager> ACE_INLINE Manager
TAO_Unbounded_Managed_Sequence<T,Manager>::operator[] (CORBA::ULong index)
{
  T* tmp = ACE_reinterpret_cast (T*, this->buffer_[index]);
  return Manager (&tmp, this->release_);
}

template <class T, class Manager> ACE_INLINE const Manager
TAO_Unbounded_Managed_Sequence<T,Manager>::operator[] (CORBA::ULong index) const
{
  T* tmp = ACE_reinterpret_cast (T*, this->buffer_[index]);
  return Manager (&tmp, this->release_);
}

template<class T, class Manager> ACE_INLINE int
TAO_Unbounded_Managed_Sequence<T,Manager>::_bounded (void) const
{
  return 0;
}

// *************************************************************
// class TAO_Bounded_Managed_Sequence
// *************************************************************

template <class T, class Manager, CORBA::ULong MAX> ACE_INLINE void
TAO_Bounded_Managed_Sequence<T,Manager,MAX>::freebuf (T* *seq)
{
  delete []seq;
}

template <class T, class Manager, CORBA::ULong MAX> ACE_INLINE
TAO_Bounded_Managed_Sequence<T,Manager,MAX>::TAO_Bounded_Managed_Sequence (void)
{}

// constructor from data buffer
template <class T, class Manager, CORBA::ULong MAX> ACE_INLINE
TAO_Bounded_Managed_Sequence<T,Manager,MAX>::TAO_Bounded_Managed_Sequence
(CORBA::ULong length, T* *value, CORBA::Boolean release)
  : TAO_Base_Sequence (MAX, length, value, release)
{
}

template <class T, class Manager, CORBA::ULong MAX> ACE_INLINE Manager
TAO_Bounded_Managed_Sequence<T,Manager,MAX>::operator[] (CORBA::ULong index)
{
  T* tmp = ACE_reinterpret_cast (T*, this->buffer_[index]);
  return Manager (&tmp, this->release_);
}

template <class T, class Manager, CORBA::ULong MAX> ACE_INLINE const Manager
TAO_Bounded_Managed_Sequence<T,Manager,MAX>::operator[] (CORBA::ULong index) const
{
  T* tmp = ACE_reinterpret_cast (T*, this->buffer_[index]);
  return Manager(&tmp, this->release_);
}

template<class T, class Manager, CORBA::ULong MAX> ACE_INLINE int
TAO_Bounded_Managed_Sequence<T,Manager,MAX>::_bounded (void) const
{
  return 0;
}
