/* -*- C++ -*- */

//
// $Id$
//

#if !defined (TAO_SEQUENCE_T_C)
#define TAO_SEQUENCE_T_C

#include "tao/sequence_T.h"

#if !defined (__ACE_INLINE__)
#include "tao/sequence_T.i"
#endif /* __ACE_INLINE__ */

template <class T>
TAO_Unbounded_Sequence<T>::
TAO_Unbounded_Sequence (const TAO_Unbounded_Sequence<T> &rhs)
  : TAO_Unbounded_Base_Sequence (rhs)
{
  this->buffer_ = TAO_Unbounded_Sequence<T>::allocbuf (this->maximum_);
  T* tmp = ACE_reinterpret_cast(T*,this->buffer_);
  for (CORBA::ULong i = 0; i < this->length_; ++i)
    tmp[i] = rhs[i];
}

template <class T> TAO_Unbounded_Sequence<T> &
TAO_Unbounded_Sequence<T>::operator=
(const TAO_Unbounded_Sequence<T> &rhs)
{
  if (this != &rhs)
    {
      if (this->release_)
        {
          // free the old buffer
          T* tmp = ACE_reinterpret_cast(T*,this->buffer_);
          TAO_Unbounded_Sequence<T>::freebuf (tmp);
        }

      this->TAO_Base_Sequence::operator= (rhs);
      this->buffer_ = TAO_Unbounded_Sequence<T>::allocbuf (this->maximum_);
      this->release_ = 1;
      T* tmp = ACE_reinterpret_cast(T*,this->buffer_);
      for (CORBA::ULong i = 0; i < this->length_; ++i)
	tmp[i] = rhs[i];
    }
  return *this;
}

template<class T>
TAO_Unbounded_Sequence<T>::~TAO_Unbounded_Sequence (void)
{
  this->_deallocate_buffer ();
}

template<class T>
void TAO_Unbounded_Sequence<T>::_allocate_buffer (CORBA::ULong length)
{
  T* tmp;
  ACE_NEW (tmp, T[length]);

  if (this->buffer_ != 0)
    {
      T* old = ACE_reinterpret_cast(T*,this->buffer_);
      for (CORBA::ULong i = 0; i < this->length_; ++i)
	{
	  tmp[i] = old[i];
	}
      delete[] old;
    }
  this->buffer_ = tmp;
}

template<class T>
void TAO_Unbounded_Sequence<T>::_deallocate_buffer (void)
{
  if (this->buffer_ == 0 || this->release_ == 0)
    return;
  T* tmp = ACE_reinterpret_cast (T*,this->buffer_);
  delete[] tmp;
  this->buffer_ = 0;
}

// ****************************************************************
// Bounded_Sequence
// ****************************************************************

template <class T, CORBA::ULong MAX>
TAO_Bounded_Sequence<T,MAX>::
TAO_Bounded_Sequence (const TAO_Bounded_Sequence<T,MAX> &rhs)
  : TAO_Bounded_Base_Sequence (rhs)
{
  this->buffer_ = TAO_Bounded_Sequence<T,MAX>::allocbuf (MAX);
  T* tmp = ACE_reinterpret_cast(T*,this->buffer_);
  for (CORBA::ULong i = 0; i < this->length_; ++i)
    tmp[i] = rhs[i];
}

template <class T, CORBA::ULong MAX> TAO_Bounded_Sequence<T,MAX> &
TAO_Bounded_Sequence<T,MAX>::operator= (const TAO_Bounded_Sequence<T,MAX> &rhs)
{
  if (this != &rhs)
    {
      this->TAO_Base_Sequence::operator= (rhs);
      this->buffer_ = TAO_Unbounded_Sequence<T>::allocbuf (this->maximum_);
      this->release_ = 1;
      T* tmp = ACE_reinterpret_cast(T*,this->buffer_);
      for (CORBA::ULong i = 0; i < this->length_; ++i)
	tmp[i] = seq[i];
    }
  return *this;
}

template<class T, CORBA::ULong MAX>
TAO_Bounded_Sequence<T,MAX>::~TAO_Bounded_Sequence (void)
{
  this->_deallocate_buffer ();
}

template<class T, CORBA::ULong MAX>
void TAO_Bounded_Sequence<T,MAX>::_allocate_buffer (CORBA::ULong)
{
  // For this class memory is never reallocated so the implementation
  // is *really* simple.
  ACE_NEW (this->buffer_, T[MAX]);
  this->maximum_ = MAX;
}

template<class T, CORBA::ULong MAX>
void TAO_Bounded_Sequence<T,MAX>::_deallocate_buffer (void)
{
  if (this->buffer_ == 0 || this->release_ == 0)
    return;
  T* tmp = ACE_reinterpret_cast (T*,this->buffer_);
  delete[] tmp;
  this->buffer_ = 0;
}

// *************************************************************
// class TAO_Object_Manager
// *************************************************************

// destructor
template <class T>
TAO_Object_Manager<T>::~TAO_Object_Manager (void)
{
}

template <class T>
TAO_Object_Manager<T>::TAO_Object_Manager (const TAO_Object_Manager &rhs)
{
  *this->ptr_ = T::_duplicate (*rhs.ptr_);
  this->release_ = _tao_mng_type.release_;
}

template <class T>
TAO_Object_Manager<T>&
TAO_Object_Manager<T>::operator= (const TAO_Object_Manager &rhs)
{
  if (this == &rhs)
    return *this;

  if (this->release_) // need to free old one
    CORBA::release (*this->ptr_);

  *this->ptr_ = T::_duplicate (*rhs.ptr_);
  return *this;
}

template <class T>
TAO_Object_Manager<T> &
TAO_Object_Manager<T>::operator=(T* p)
{
  if (this->release_) // need to free old one
    CORBA::release (*this->ptr_);
  *this->ptr_ = p; // no copy
  return *this;
}

// mapping for variable size
template <class T> T* &
TAO_Object_Manager<T>::out (void)
{
  if (this->release_)
    CORBA::release (*this->ptr_);
  *this->ptr_ = T::_nil ();
  return *this->ptr_;
}

template <class T> T*
TAO_Object_Manager<T>::_retn (void)
{
  T* tmp = *this->ptr_;
  *this->ptr_ = T::_nil ();
  return tmp;
}

// *************************************************************
// Operations for class TAO_Unbounded_Managed_Sequence
// *************************************************************

template<class T, class Manager>
TAO_Unbounded_Managed_Sequence<T,Manager>::~TAO_Unbounded_Managed_Sequence (void)
{
  this->_deallocate_buffer ();
}

// copy constructor
template <class T, class Manager>
TAO_Unbounded_Managed_Sequence<T,Manager>::TAO_Unbounded_Managed_Sequence
(const TAO_Unbounded_Managed_Sequence<T,Manager> &seq)
  : TAO_Unbounded_Base_Sequence (seq)
{
  this->buffer_ = TAO_Unbounded_Managed_Sequence<T,Manager>::
    allocbuf (this->maximum_);
  T* *tmp1 = ACE_reinterpret_cast(T* *,this->buffer_);
  T* *tmp2 = ACE_reinterpret_cast(T* *,seq.buffer_);
  for (CORBA::ULong i=0; i < seq.length_; i++)
    tmp [i] = T::_duplicate (tmp2 [i]);
}

// assignment operator
template <class T, class Manager>
TAO_Unbounded_Managed_Sequence<T,Manager>&
TAO_Unbounded_Managed_Sequence<T,Manager>::operator=
(const TAO_Unbounded_Managed_Sequence<T,Manager> &seq)
{
  if (this == &seq) return *this;
  if (this->release_)
  {
    T* *tmp = ACE_reinterpret_cast (T* *,this->buffer_);
    TAO_Unbounded_Managed_Sequence<T,Manager>::freebuf (tmp,
                                                    this->maximum_);
  }
  T* *tmp1 = ACE_reinterpret_cast(T* *,this->buffer_);
  T* *tmp2 = ACE_reinterpret_cast(T* *,seq.buffer_);
  for (CORBA::ULong i=0; i < seq.length_; i++)
    tmp [i] = T::_duplicate (tmp2 [i]);
  return *this;
}

template <class T, class Manager> T* *
TAO_Unbounded_Managed_Sequence<T,Manager>::allocbuf (CORBA::ULong nelems)
{
  T* *buf = new T*[nelems]; // allocate from heap
  for (CORBA::ULong i=0; i < nelems; i++)
  	buf[i] = T::_nil ();
  return buf;
}

template <class T, class Manager> void
TAO_Unbounded_Managed_Sequence<T,Manager>::freebuf (T* *seq,
						    CORBA::ULong nelems)
{
  if (!seq) return; // null sequence
  for (CORBA::ULong i=0; i < nelems; i++)
  	CORBA::release (seq[i]);
  TAO_Unbounded_Managed_Sequence<T,Manager>::freebuf (seq);
}

template<class T, class Manager>
void TAO_Unbounded_Managed_Sequence<T,Manager>::_allocate_buffer (CORBA::ULong length)
{
  T* *tmp;
  ACE_NEW (tmp, T* [length]);

  if (this->buffer_ != 0)
    {
      T* *old = ACE_reinterpret_cast(T**,this->buffer_);
      for (CORBA::ULong i = 0; i < this->length_; ++i)
	{
	  tmp [i] = T::_duplicate (old[i]);
	}
      delete[] old;
    }
  this->buffer_ = tmp;
}

template<class T, class Manager>
void TAO_Unbounded_Managed_Sequence<T,Manager>::_deallocate_buffer (void)
{
  if (this->buffer_ == 0 || this->release_ == 0)
    return;
  T* *tmp = ACE_reinterpret_cast (T**,this->buffer_);
  // XXXASG: Do we release each object here?
  // @@ TODO add static methods to Manager to release each object.
  delete[] tmp;
  this->buffer_ = 0;
}

// *************************************************************
// Operations for class TAO_Bounded_Managed_Sequence
// *************************************************************

template<class T, class Manager, CORBA::ULong MAX>
TAO_Bounded_Managed_Sequence<T,Manager,MAX>::~TAO_Bounded_Managed_Sequence (void)
{
  this->_deallocate_buffer ();
}

// copy constructor
template <class T, class Manager, CORBA::ULong MAX>
TAO_Bounded_Managed_Sequence<T,Manager,MAX>::TAO_Bounded_Managed_Sequence
(const TAO_Bounded_Managed_Sequence<T,Manager,MAX> &seq)
  : TAO_Unbounded_Base_Sequence (seq)
{
  this->buffer_ = TAO_Bounded_Managed_Sequence<T,Manager,MAX>::
    allocbuf (this->maximum_);
  T* *tmp1 = ACE_reinterpret_cast(T* *,this->buffer_);
  T* *tmp2 = ACE_reinterpret_cast(T* *,seq.buffer_);
  for (CORBA::ULong i=0; i < seq.length_; i++)
    tmp [i] = T::_duplicate (tmp2 [i]);
}

// assignment operator
template <class T, class Manager, CORBA::ULong MAX>
TAO_Bounded_Managed_Sequence<T,Manager,MAX>&
TAO_Bounded_Managed_Sequence<T,Manager,MAX>::operator=
(const TAO_Bounded_Managed_Sequence<T,Manager,MAX> &seq)
{
  if (this == &seq) return *this;
  if (this->release_)
  {
    T* *tmp = ACE_reinterpret_cast (T* *,this->buffer_);
    TAO_Bounded_Managed_Sequence<T,Manager,MAX>::freebuf (tmp,
                                                    this->maximum_);
  }
  T* *tmp1 = ACE_reinterpret_cast(T* *,this->buffer_);
  T* *tmp2 = ACE_reinterpret_cast(T* *,seq.buffer_);
  for (CORBA::ULong i=0; i < seq.length_; i++)
    tmp [i] = T::_duplicate (tmp2 [i]);
  return *this;
}

template <class T, class Manager, CORBA::ULong MAX> T* *
TAO_Bounded_Managed_Sequence<T,Manager,MAX>::allocbuf (CORBA::ULong nelems)
{
  T* *buf = new T*[nelems]; // allocate from heap
  for (CORBA::ULong i=0; i < nelems; i++)
  	buf[i] = T::_nil ();
  return buf;
}

template <class T, class Manager, CORBA::ULong MAX> void
TAO_Bounded_Managed_Sequence<T,Manager,MAX>::freebuf (T* *seq,
						      CORBA::ULong nelems)
{
  if (!seq) return; // null sequence
  for (CORBA::ULong i=0; i < nelems; i++)
  	CORBA::release (seq[i]);
  TAO_Bounded_Managed_Sequence<T,Manager,MAX>::freebuf (seq);
}

template<class T, class Manager, CORBA::ULong MAX> void
TAO_Bounded_Managed_Sequence<T,Manager,MAX>::_allocate_buffer (CORBA::ULong length)
{
  T* *tmp;
  ACE_NEW (tmp, T* [length]);

  if (this->buffer_ != 0)
    {
      T* *old = ACE_reinterpret_cast(T**,this->buffer_);
      for (CORBA::ULong i = 0; i < this->length_; ++i)
	{
	  tmp [i] = T::_duplicate (old[i]);
	}
      delete[] old;
    }
  this->buffer_ = tmp;
}

template<class T, class Manager, CORBA::ULong MAX>
void TAO_Bounded_Managed_Sequence<T,Manager,MAX>::_deallocate_buffer (void)
{
  if (this->buffer_ == 0 || this->release_ == 0)
    return;
  T* *tmp = ACE_reinterpret_cast (T**,this->buffer_);
  // XXXASG: Do we release each object here?
  // @@ TODO add static methods to Manager to release each object here.
  delete[] tmp;
  this->buffer_ = 0;
}

#endif /* TAO_SEQUENCE_T_C */
