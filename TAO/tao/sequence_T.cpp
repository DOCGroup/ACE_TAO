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
  if (this->release_)
    T::_release (*(this->ptr_));
}

template <class T>
TAO_Object_Manager<T>::TAO_Object_Manager (const TAO_Object_Manager &rhs)
  : ptr_ (rhs.ptr_),
    release_ (rhs.release_)
{
}

template <class T>
TAO_Object_Manager<T>&
TAO_Object_Manager<T>::operator= (const TAO_Object_Manager<T> &rhs)
{
  if (this == &rhs)
    return *this;

  if (this->release_) // need to free old one
    CORBA::release (*this->ptr_);

  this->ptr_ = rhs.ptr_;
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

// *************************************************************
// Operations for class TAO_Unbounded_Object_Sequence
// *************************************************************

// constructor for unbounded seq
template <class T>
TAO_Unbounded_Object_Sequence<T>::
TAO_Unbounded_Object_Sequence (CORBA::ULong maximum)
  : TAO_Unbounded_Base_Sequence (maximum,
				 TAO_Unbounded_Object_Sequence<T>::allocbuf (maximum))
{
}

// copy constructor
template <class T>
TAO_Unbounded_Object_Sequence<T>::
TAO_Unbounded_Object_Sequence (const TAO_Unbounded_Object_Sequence<T> &seq)
  : TAO_Unbounded_Base_Sequence (seq)
{
  if (this->release_)
    {
      T* *tmp = ACE_reinterpret_cast(T* *,this->buffer_);
      for (CORBA::ULong i = 0; i < this->length_; ++i)
	{
	  CORBA::release (tmp[i]);
	}
      if (this->maximum_ < seq.maximum_)
	{
	  this->buffer_ =
	    TAO_Unbounded_Object_Sequence<T>::allocbuf (seq.maximum_);
	}
    }
  else
    {
      this->buffer_ =
	TAO_Unbounded_Object_Sequence<T>::allocbuf (this->maximum_);
    }

  T* *tmp1 = ACE_reinterpret_cast(T* *,this->buffer_);
  T* *tmp2 = ACE_reinterpret_cast(T* *,seq.buffer_);
  for (CORBA::ULong i=0; i < seq.length_; i++)
    tmp1 [i] = T::_duplicate (tmp2 [i]);
}

// assignment operator
template <class T>
TAO_Unbounded_Object_Sequence<T>&
TAO_Unbounded_Object_Sequence<T>::
operator= (const TAO_Unbounded_Object_Sequence<T> &seq)
{
  if (this == &seq)
    return *this;

  if (this->release_)
    {
      T* *tmp = ACE_reinterpret_cast(T* *,this->buffer_);
      for (CORBA::ULong i = 0; i < this->length_; ++i)
	{
	  CORBA::release (tmp[i]);
	}
      if (this->maximum_ < seq.maximum_)
	{
	  this->buffer_ =
	    TAO_Unbounded_Object_Sequence<T>::allocbuf (seq.maximum_);
	}
    }
  else
    {
      this->buffer_ =
	TAO_Unbounded_Object_Sequence<T>::allocbuf (this->maximum_);
    }

  T* *tmp1 = ACE_reinterpret_cast(T* *,this->buffer_);
  T* *tmp2 = ACE_reinterpret_cast(T* *,seq.buffer_);
  for (CORBA::ULong i=0; i < seq.length_; i++)
    tmp1 [i] = T::_duplicate (tmp2 [i]);
  return *this;
}

template <class T> T* *
TAO_Unbounded_Object_Sequence<T>::allocbuf (CORBA::ULong nelems)
{
  T* *buf = new T*[nelems];
  for (CORBA::ULong i=0; i < nelems; i++)
    buf[i] = T::_nil ();
  return buf;
}

template <class T> void
TAO_Unbounded_Object_Sequence<T>::freebuf (T* *buffer)
{
  if (buffer == 0)
    return;

  // {orbos/97-05-15:16.11}
  // The freebuf function ensures that the destructor for each element
  // is called before the buffer is destroyed, except for string
  // elements, which are freed using string_free(), and object
  // reference elements, which are freed using release(). The freebuf
  // function will ignore null pointers passed to it.

  // @@ How are we supposed to implement that! We don't know the
  // length of the buffer here.
  //  Mark the length in the first four bytes? For the moment we let
  //  that be.

  T* *tmp = ACE_reinterpret_cast (T**,buffer);
  delete[] tmp;
}

template<class T>
void TAO_Unbounded_Object_Sequence<T>::_allocate_buffer (CORBA::ULong length)
{
  T* *tmp;
  ACE_NEW (tmp, T* [length]);

  if (this->buffer_ != 0)
    {
      T* *old = ACE_reinterpret_cast(T**,this->buffer_);
      for (CORBA::ULong i = 0; i < this->length_; ++i)
	{
	  // Only call duplicate when we did not own the previous
	  // buffer, since after this method we own it we must also
	  // own the objects.  If we already own the objects there is
	  // no need to copy them, if we did we would also have to
	  // remove the old instances.
	  if (!this->release_)
	    tmp [i] = T::_duplicate (old[i]);
	  else
	    tmp [i] = old[i];
	}
      if (this->release_)
	delete[] old;
    }
  this->buffer_ = tmp;
}

template<class T>
void TAO_Unbounded_Object_Sequence<T>::_deallocate_buffer (void)
{
  if (this->buffer_ == 0 || this->release_ == 0)
    return;
  T* *tmp = ACE_reinterpret_cast (T**,this->buffer_);
  for (CORBA::ULong i = 0;
       i < this->length_;
       ++i)
    {
      CORBA::release (tmp[i]);
    }
  delete[] tmp;
  this->buffer_ = 0;
}

template<class T> void
TAO_Unbounded_Object_Sequence<T>::_shrink_buffer (CORBA::ULong nl,
						  CORBA::ULong ol)
{
  T* *tmp = ACE_reinterpret_cast (T**,this->buffer_);
  for (CORBA::ULong i = ol; i < nl; ++i)
    CORBA::release (tmp[i]);
}

// *************************************************************
// Operations for class TAO_Bounded_Object_Sequence
// *************************************************************

template<class T, CORBA::ULong MAX>
TAO_Bounded_Object_Sequence<T,MAX>::~TAO_Bounded_Object_Sequence (void)
{
  this->_deallocate_buffer ();
}

template <class T, CORBA::ULong MAX>
TAO_Bounded_Object_Sequence<T,MAX>::
TAO_Bounded_Object_Sequence (const TAO_Bounded_Object_Sequence<T,MAX> &seq)
  : TAO_Bounded_Base_Sequence (seq)
{
  this->buffer_ = TAO_Bounded_Object_Sequence<T,MAX>::
    allocbuf (this->maximum_);
  T* *tmp1 = ACE_reinterpret_cast(T* *,this->buffer_);
  T* *tmp2 = ACE_reinterpret_cast(T* *,seq.buffer_);
  for (CORBA::ULong i=0; i < seq.length_; i++)
    tmp1 [i] = T::_duplicate (tmp2 [i]);
}

// assignment operator
template <class T, CORBA::ULong MAX>
TAO_Bounded_Object_Sequence<T,MAX>&
TAO_Bounded_Object_Sequence<T,MAX>::operator=
(const TAO_Bounded_Object_Sequence<T,MAX> &seq)
{
  if (this == &seq)
    return *this;
  if (this->release_)
    {
      T* *tmp = ACE_reinterpret_cast (T* *,this->buffer_);
      TAO_Bounded_Object_Sequence<T,MAX>::freebuf (tmp);
    }
  T* *tmp1 = ACE_reinterpret_cast(T* *,this->buffer_);
  T* *tmp2 = ACE_reinterpret_cast(T* *,seq.buffer_);
  for (CORBA::ULong i=0; i < seq.length_; i++)
    tmp1 [i] = T::_duplicate (tmp2 [i]);
  return *this;
}

template <class T, CORBA::ULong MAX> T* *
TAO_Bounded_Object_Sequence<T,MAX>::allocbuf (CORBA::ULong nelems)
{
  T* *buf = new T*[nelems]; // allocate from heap
  for (CORBA::ULong i=0; i < nelems; i++)
    buf[i] = T::_nil ();
  return buf;
}

template<class T, CORBA::ULong MAX> void
TAO_Bounded_Object_Sequence<T,MAX>::_allocate_buffer (CORBA::ULong length)
{
  T* *tmp;
  ACE_NEW (tmp, T* [length]);

  if (this->buffer_ != 0)
    {
      T* *old = ACE_reinterpret_cast(T**,this->buffer_);
      for (CORBA::ULong i = 0; i < this->length_; ++i)
	tmp [i] = old[i];
      delete[] old;
    }
  this->buffer_ = tmp;
}

template<class T, CORBA::ULong MAX>
void TAO_Bounded_Object_Sequence<T,MAX>::_deallocate_buffer (void)
{
  if (this->buffer_ == 0 || this->release_ == 0)
    return;
  T* *tmp = ACE_reinterpret_cast (T**,this->buffer_);
  // XXXASG: Do we release each object here?
  // @@ TODO add static methods to Manager to release each object here.
  delete[] tmp;
  this->buffer_ = 0;
}

template<class T, CORBA::ULong MAX> void
TAO_Bounded_Object_Sequence<T,MAX>::_shrink_buffer (CORBA::ULong nl,
						    CORBA::ULong ol)
{
  T* *tmp = ACE_reinterpret_cast (T**,this->buffer_);
  for (CORBA::ULong i = ol; i < nl; ++i)
    CORBA::release (tmp[i]);
}

#endif /* TAO_SEQUENCE_T_C */
