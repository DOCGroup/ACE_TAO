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

template<class T, CORBA::ULong MAX> ACE_INLINE int
TAO_Bounded_Sequence<T,MAX>::_bounded (void) const
{
  return 0;
}

// *************************************************************
// Inline operations for class
// TAO_Unbounded_ObjectSequence<T,T_ptr>::TAO_ObjRefMngType
// *************************************************************

template <class T, class T_ptr> ACE_INLINE
TAO_Unbounded_ObjectSequence<T,T_ptr>::
TAO_ObjRefMngType::TAO_ObjRefMngType(T_ptr* buffer, CORBA::Boolean release)
  : ptr_ (buffer),
    release_ (release)
{}

template <class T, class T_ptr> ACE_INLINE
TAO_Unbounded_ObjectSequence<T,T_ptr>::
TAO_ObjRefMngType::operator const T_ptr &() const // cast
{
  return *this->ptr_;
}

template <class T, class T_ptr> ACE_INLINE
TAO_Unbounded_ObjectSequence<T,T_ptr>::
TAO_ObjRefMngType::operator T_ptr &() // cast
{
  return *this->ptr_;
}

template <class T, class T_ptr> ACE_INLINE const T_ptr
TAO_Unbounded_ObjectSequence<T,T_ptr>::
TAO_ObjRefMngType::in (void) const
{
  return *this->ptr_;
}

template <class T, class T_ptr> ACE_INLINE T_ptr&
TAO_Unbounded_ObjectSequence<T,T_ptr>::
TAO_ObjRefMngType::inout (void)
{
  return *this->ptr_;
}

// *************************************************************
// class TAO_Unbounded_ObjectSequence
// *************************************************************

template <class T, class T_ptr> ACE_INLINE void
TAO_Unbounded_ObjectSequence<T,T_ptr>::freebuf (T_ptr *seq)
{
  delete []seq;
}

//default constructor
template <class T, class T_ptr> ACE_INLINE
TAO_Unbounded_ObjectSequence<T,T_ptr>::TAO_Unbounded_ObjectSequence (void)
{}

// constructor for unbounded seq
template <class T, class T_ptr> ACE_INLINE
TAO_Unbounded_ObjectSequence<T,T_ptr>::TAO_Unbounded_ObjectSequence
(CORBA::ULong max )
  : TAO_Base_Sequence (max, TAO_Unbounded_ObjectSequence<T,T_ptr>::allocbuf (max))
{
}

// constructor from data buffer
template <class T, class T_ptr> ACE_INLINE
TAO_Unbounded_ObjectSequence<T,T_ptr>::TAO_Unbounded_ObjectSequence
(CORBA::ULong max, CORBA::ULong length,	T_ptr *value, CORBA::Boolean release)
  : TAO_Base_Sequence (max, length, value, release)
{
}

template <class T, class T_ptr> ACE_INLINE
TAO_Unbounded_ObjectSequence<T,T_ptr>::TAO_ObjRefMngType
TAO_Unbounded_ObjectSequence<T,T_ptr>::operator[] (CORBA::ULong index) // read/write
{
  T_ptr tmp = ACE_reinterpret_cast (T_ptr, this->buffer_[index]);
  return TAO_Unbounded_ObjectSequence<T,T_ptr>::TAO_ObjRefMngType (&tmp,
                                                                   this->release_);
}

template <class T, class T_ptr> ACE_INLINE const
TAO_Unbounded_ObjectSequence<T,T_ptr>::TAO_ObjRefMngType
TAO_Unbounded_ObjectSequence<T,T_ptr>::operator[] (CORBA::ULong index) const // read
{
  T_ptr tmp = ACE_reinterpret_cast (T_ptr, this->buffer_[index]);
  return TAO_Unbounded_ObjectSequence<T,T_ptr>::TAO_ObjRefMngType
  (&tmp, this->release_);
}

template<class T, class T_ptr> ACE_INLINE int
TAO_Unbounded_ObjectSequence<T,T_ptr>::_bounded (void) const
{
  return 0;
}

// *************************************************************
// Inline operations for class
// TAO_Bounded_ObjectSequence<T,T_ptr,MAX>::TAO_ObjRefMngType
// *************************************************************

template <class T, class T_ptr, CORBA::ULong MAX> ACE_INLINE
TAO_Bounded_ObjectSequence<T,T_ptr,MAX>::
TAO_ObjRefMngType::TAO_ObjRefMngType(T_ptr* buffer, CORBA::Boolean release)
  : ptr_ (buffer),
    release_ (release)
{}

template <class T, class T_ptr, CORBA::ULong MAX> ACE_INLINE
TAO_Bounded_ObjectSequence<T,T_ptr,MAX>::
TAO_ObjRefMngType::operator const T_ptr &() const // cast
{
  return *this->ptr_;
}

template <class T, class T_ptr, CORBA::ULong MAX> ACE_INLINE
TAO_Bounded_ObjectSequence<T,T_ptr,MAX>::
TAO_ObjRefMngType::operator T_ptr &() // cast
{
  return *this->ptr_;
}

template <class T, class T_ptr, CORBA::ULong MAX> ACE_INLINE const T_ptr
TAO_Bounded_ObjectSequence<T,T_ptr,MAX>::
TAO_ObjRefMngType::in (void) const
{
  return *this->ptr_;
}

template <class T, class T_ptr, CORBA::ULong MAX> ACE_INLINE T_ptr&
TAO_Bounded_ObjectSequence<T,T_ptr,MAX>::
TAO_ObjRefMngType::inout (void)
{
  return *this->ptr_;
}

// *************************************************************
// class TAO_Bounded_ObjectSequence
// *************************************************************

template <class T, class T_ptr, CORBA::ULong MAX> ACE_INLINE void
TAO_Bounded_ObjectSequence<T,T_ptr,MAX>::freebuf (T_ptr *seq)
{
  delete []seq;
}

//default constructor
template <class T, class T_ptr, CORBA::ULong MAX> ACE_INLINE
TAO_Bounded_ObjectSequence<T,T_ptr,MAX>::TAO_Bounded_ObjectSequence (void)
{}

// constructor from data buffer
template <class T, class T_ptr, CORBA::ULong MAX> ACE_INLINE
TAO_Bounded_ObjectSequence<T,T_ptr,MAX>::TAO_Bounded_ObjectSequence
(CORBA::ULong length, T_ptr *value, CORBA::Boolean release)
  : TAO_Base_Sequence (MAX, length, value, release)
{
}

template <class T, class T_ptr, CORBA::ULong MAX> ACE_INLINE
TAO_Bounded_ObjectSequence<T,T_ptr,MAX>::TAO_ObjRefMngType
TAO_Bounded_ObjectSequence<T,T_ptr,MAX>::operator[] (CORBA::ULong index) // read/write
{
  T_ptr tmp = ACE_reinterpret_cast (T_ptr, this->buffer_[index]);
  return TAO_Bounded_ObjectSequence<T,T_ptr,MAX>::TAO_ObjRefMngType (&tmp,
                                                                   this->release_);
}

template <class T, class T_ptr, CORBA::ULong MAX> ACE_INLINE const
TAO_Bounded_ObjectSequence<T,T_ptr,MAX>::TAO_ObjRefMngType
TAO_Bounded_ObjectSequence<T,T_ptr,MAX>::operator[] (CORBA::ULong index) const // read
{
  T_ptr tmp = ACE_reinterpret_cast (T_ptr, this->buffer_[index]);
  return TAO_Bounded_ObjectSequence<T,T_ptr,MAX>::TAO_ObjRefMngType
  (&tmp, this->release_);
}

template<class T, class T_ptr, CORBA::ULong MAX> ACE_INLINE int
TAO_Bounded_ObjectSequence<T,T_ptr,MAX>::_bounded (void) const
{
  return 0;
}

// *************************************************************
// Inline operations for class TAO_Bounded_StringSequence<MAX>
// ::TAO_StrMngType
// *************************************************************

template <CORBA::ULong MAX> ACE_INLINE
TAO_Bounded_StringSequence<MAX>::
TAO_StrMngType::TAO_StrMngType (char **buffer,
                                CORBA::Boolean release)
  : ptr_ (buffer),
    release_ (release)
{
}

template <CORBA::ULong MAX> ACE_INLINE
TAO_Bounded_StringSequence<MAX>::
TAO_StrMngType::operator const char *() const // cast
{
  return *this->ptr_;
}

template <CORBA::ULong MAX> ACE_INLINE
TAO_Bounded_StringSequence<MAX>::
TAO_StrMngType::operator char * () // cast
{
  return *this->ptr_;
}

template <CORBA::ULong MAX> ACE_INLINE char&
TAO_Bounded_StringSequence<MAX>::
TAO_StrMngType::operator[] (CORBA::ULong index)
{
  return *this->ptr_[index];
}

template <CORBA::ULong MAX> ACE_INLINE const char&
TAO_Bounded_StringSequence<MAX>::
TAO_StrMngType::operator[] (CORBA::ULong index) const
{
  return *this->ptr_[index];
}

template <CORBA::ULong MAX> ACE_INLINE const char *
TAO_Bounded_StringSequence<MAX>::
TAO_StrMngType::in (void) const
{
  return *this->ptr_;
}

template <CORBA::ULong MAX> ACE_INLINE char *&
TAO_Bounded_StringSequence<MAX>::
TAO_StrMngType::inout (void)
{
  return *this->ptr_;
}

// *************************************************************
// class TAO_Bounded_StringSequence<MAX>
// *************************************************************

template <CORBA::ULong MAX> ACE_INLINE void
TAO_Bounded_StringSequence<MAX>::freebuf (char **seq)
{
  delete []seq;
}

//default constructor
template <CORBA::ULong MAX> ACE_INLINE
TAO_Bounded_StringSequence<MAX>::TAO_Bounded_StringSequence (void)
{}

// constructor from data buffer
template <CORBA::ULong MAX> ACE_INLINE
TAO_Bounded_StringSequence<MAX>::TAO_Bounded_StringSequence
(CORBA::ULong length, char **value, CORBA::Boolean release)
  : TAO_Base_Sequence (MAX, length, value, release)
{
}

template <CORBA::ULong MAX> ACE_INLINE
TAO_Bounded_StringSequence<MAX>::TAO_StrMngType
TAO_Bounded_StringSequence<MAX>::operator[] (CORBA::ULong index) // read/write
{
  return TAO_Bounded_StringSequence<MAX>::TAO_StrMngType
    (&((char **)this->buffer_)[index], this->release_);
}

template <CORBA::ULong MAX> ACE_INLINE const
TAO_Bounded_StringSequence<MAX>::TAO_StrMngType
TAO_Bounded_StringSequence<MAX>::operator[] (CORBA::ULong index) const // read
{
  return TAO_Bounded_StringSequence<MAX>::TAO_StrMngType
    (&((char **)this->buffer_)[index], this->release_);
}

template <CORBA::ULong MAX> ACE_INLINE int
TAO_Bounded_StringSequence<MAX>::_bounded (void) const
{
  return 0;
}
