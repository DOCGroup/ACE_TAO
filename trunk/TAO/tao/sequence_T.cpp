// $Id$

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
  T *tmp1 = TAO_Unbounded_Sequence<T>::allocbuf (this->maximum_);
  T *tmp2 = ACE_reinterpret_cast (T *, rhs.buffer_);

  for (CORBA::ULong i = 0; i < this->length_; ++i)
    tmp1[i] = tmp2[i];

  this->buffer_ = tmp1;
}

template <class T> TAO_Unbounded_Sequence<T> &
TAO_Unbounded_Sequence<T>::operator=
(const TAO_Unbounded_Sequence<T> &rhs)
{
  if (this == &rhs)
    return *this;

  if (this->release_)
    {
      if (this->maximum_ < rhs.maximum_)
        {
          // free the old buffer
          T *tmp = ACE_reinterpret_cast (T *, this->buffer_);
          TAO_Unbounded_Sequence<T>::freebuf (tmp);
          this->buffer_ =
            TAO_Unbounded_Sequence<T>::allocbuf (rhs.maximum_);
        }
    }
  else
    this->buffer_ =
      TAO_Unbounded_Sequence<T>::allocbuf (rhs.maximum_);

  TAO_Unbounded_Base_Sequence::operator= (rhs);

  T *tmp1 = ACE_reinterpret_cast (T *, this->buffer_);
  T *tmp2 = ACE_reinterpret_cast (T *, rhs.buffer_);

  for (CORBA::ULong i = 0; i < this->length_; ++i)
    tmp1[i] = tmp2[i];

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
  T* tmp = TAO_Unbounded_Sequence<T>::allocbuf (length);

  if (this->buffer_ != 0)
    {
      T *old = ACE_reinterpret_cast (T *, this->buffer_);

      for (CORBA::ULong i = 0; i < this->length_; ++i)
        tmp[i] = old[i];

      if (this->release_)
        delete[] old;
    }

  this->buffer_ = tmp;
}

template<class T>
void TAO_Unbounded_Sequence<T>::_deallocate_buffer (void)
{
  if (this->buffer_ == 0 || this->release_ == 0)
    return;
  T *tmp = ACE_reinterpret_cast (T *, this->buffer_);
  TAO_Unbounded_Sequence<T>::freebuf (tmp);
  this->buffer_ = 0;
}

// ****************************************************************
// Bounded_Sequence
// ****************************************************************

template <class T, CORBA::ULong MAX>
TAO_Bounded_Sequence<T, MAX>::
TAO_Bounded_Sequence (const TAO_Bounded_Sequence<T, MAX> &rhs)
  : TAO_Bounded_Base_Sequence (rhs)
{
  T *tmp1 = TAO_Bounded_Sequence<T, MAX>::allocbuf (MAX);

  T *tmp2 = ACE_reinterpret_cast(T *, rhs.buffer_);

  for (CORBA::ULong i = 0; i < this->length_; ++i)
    tmp1[i] = tmp2[i];

  this->buffer_ = tmp1;
}

template <class T, CORBA::ULong MAX> TAO_Bounded_Sequence<T, MAX> &
TAO_Bounded_Sequence<T, MAX>::
operator= (const TAO_Bounded_Sequence<T, MAX> &rhs)
{
  if (this == &rhs)
    return *this;

  if (this->release_)
    {
#if 0
      // We never need to reallocate the buffer because they are
      // always of size MAX.
      if (this->maximum_ < rhs.maximum_)
        {
          // free the old buffer
          T *tmp = ACE_reinterpret_cast(T *, this->buffer_);
          TAO_Bounded_Sequence<T>::freebuf (tmp);
          this->buffer_ =
            TAO_Bounded_Sequence<T>::allocbuf (rhs.maximum_);
        }
#endif /* 0 */
    }
  else
    this->buffer_ =
      TAO_Unbounded_Sequence<T>::allocbuf (rhs.maximum_);

  TAO_Bounded_Base_Sequence::operator= (rhs);

  T* tmp1 = ACE_reinterpret_cast (T *, this->buffer_);
  T* tmp2 = ACE_reinterpret_cast (T *, rhs.buffer_);

  for (CORBA::ULong i = 0; i < this->length_; ++i)
    tmp1[i] = tmp2[i];

  return *this;
}

template<class T, CORBA::ULong MAX>
TAO_Bounded_Sequence<T, MAX>::~TAO_Bounded_Sequence (void)
{
  this->_deallocate_buffer ();
}

template<class T, CORBA::ULong MAX>
void TAO_Bounded_Sequence<T, MAX>::_allocate_buffer (CORBA::ULong)
{
  // For this class memory is never reallocated so the implementation
  // is *really* simple.
  this->buffer_ = TAO_Bounded_Sequence<T, MAX>::allocbuf (MAX);
}

template<class T, CORBA::ULong MAX>
void TAO_Bounded_Sequence<T, MAX>::_deallocate_buffer (void)
{
  if (this->buffer_ == 0 || this->release_ == 0)
    return;
  T *tmp = ACE_reinterpret_cast (T *, this->buffer_);
  TAO_Bounded_Sequence<T, MAX>::freebuf (tmp);
  this->buffer_ = 0;
}

// *************************************************************
// class TAO_Object_Manager
// *************************************************************

template <class T>
TAO_Object_Manager<T>&
TAO_Object_Manager<T>::operator= (const TAO_Object_Manager<T> &rhs)
{
  if (this == &rhs)
    return *this;

  if (this->release_)
    {
      CORBA::release (*this->ptr_);
      *this->ptr_ = T::_duplicate (*rhs.ptr_);
    }
  else
    *this->ptr_ = *rhs.ptr_;

  return *this;
}

template <class T>
TAO_Object_Manager<T> &
TAO_Object_Manager<T>::operator=(T* p)
{
  if (this->release_)
    {
      // The semantics of the elements of a sequence are the same as
      // that of a var variable.  Therefore we will not duplicate the
      // user provided pointer before assigning it to the internal
      // variable.  However, we will release it.
      CORBA::release (*this->ptr_);
      *this->ptr_ = p;
    }
  else
    *this->ptr_ = p;

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

template <class T>
TAO_Unbounded_Object_Sequence<T>::
TAO_Unbounded_Object_Sequence (const TAO_Unbounded_Object_Sequence<T> &rhs)
  : TAO_Unbounded_Base_Sequence (rhs)
{
  T **tmp1 = TAO_Unbounded_Object_Sequence<T>::allocbuf (this->maximum_);
  T **tmp2 = ACE_reinterpret_cast (T **, rhs.buffer_);

  for (CORBA::ULong i = 0; i < rhs.length_; ++i)
    tmp1[i] = T::_duplicate (tmp2[i]);

  this->buffer_ = tmp1;
}

template<class T>
TAO_Unbounded_Object_Sequence<T>::~TAO_Unbounded_Object_Sequence (void)
{
  this->_deallocate_buffer ();
}

// assignment operator
template <class T>
TAO_Unbounded_Object_Sequence<T>&
TAO_Unbounded_Object_Sequence<T>::
operator= (const TAO_Unbounded_Object_Sequence<T> &rhs)
{
  if (this == &rhs)
    return *this;

  if (this->release_)
    {
      T **tmp = ACE_reinterpret_cast (T **, this->buffer_);

      for (CORBA::ULong i = 0; i < this->length_; ++i)
        {
          CORBA::release (tmp[i]);
          tmp[i] = T::_nil ();
        }
      if (this->maximum_ < rhs.maximum_)
        {
          TAO_Unbounded_Object_Sequence<T>::freebuf (tmp);
          this->buffer_ =
            TAO_Unbounded_Object_Sequence<T>::allocbuf (rhs.maximum_);
        }
    }
  else
    this->buffer_ =
      TAO_Unbounded_Object_Sequence<T>::allocbuf (rhs.maximum_);

  TAO_Unbounded_Base_Sequence::operator= (rhs);

  T **tmp1 = ACE_reinterpret_cast (T **, this->buffer_);
  T **tmp2 = ACE_reinterpret_cast (T **, rhs.buffer_);

  for (CORBA::ULong i = 0; i < rhs.length_; ++i)
    tmp1[i] = T::_duplicate (tmp2[i]);

  return *this;
}

template <class T> T* *
TAO_Unbounded_Object_Sequence<T>::allocbuf (CORBA::ULong nelems)
{
  T **buf = new T*[nelems];

  for (CORBA::ULong i = 0; i < nelems; i++)
    buf[i] = T::_nil ();

  return buf;
}

template <class T> void
TAO_Unbounded_Object_Sequence<T>::freebuf (T **buffer)
{
  if (buffer == 0)
    return;

  // {orbos/97-05-15:16.11} The freebuf function ensures that the
  // destructor for each element is called before the buffer is
  // destroyed, except for string elements, which are freed using
  // string_free(), and object reference elements, which are freed
  // using release(). The freebuf function will ignore null pointers
  // passed to it.

  // @@ How are we supposed to implement that! We don't know the
  // length of the buffer here.
  // Mark the length in the first four bytes? For the moment we let
  // that be.

  delete[] buffer;
}

template<class T>
void TAO_Unbounded_Object_Sequence<T>::_allocate_buffer (CORBA::ULong length)
{
  T **tmp = TAO_Unbounded_Object_Sequence<T>::allocbuf (length);

  if (this->buffer_ != 0)
    {
      T **old = ACE_reinterpret_cast (T**, this->buffer_);
      for (CORBA::ULong i = 0; i < this->length_; ++i)
        // Only call duplicate when we did not own the previous
        // buffer, since after this method we own it we must also own
        // the objects.  If we already own the objects there is no
        // need to copy them, if we did we would also have to remove
        // the old instances.
        if (!this->release_)
          tmp[i] = T::_duplicate (old[i]);
        else
          tmp[i] = old[i];

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
  T **tmp = ACE_reinterpret_cast (T**, this->buffer_);
  for (CORBA::ULong i = 0; i < this->length_; ++i)
    {
      CORBA::release (tmp[i]);
      tmp[i] = T::_nil ();
    }
  TAO_Unbounded_Object_Sequence<T>::freebuf (tmp);
  this->buffer_ = 0;
}

template<class T> void
TAO_Unbounded_Object_Sequence<T>::_shrink_buffer (CORBA::ULong nl,
                                                  CORBA::ULong ol)
{
  T **tmp = ACE_reinterpret_cast (T**, this->buffer_);

  for (CORBA::ULong i = nl; i < ol; ++i)
    {
      CORBA::release (tmp[i]);
      tmp[i] = T::_nil ();
    }
}

// *************************************************************
// Operations for class TAO_Bounded_Object_Sequence
// *************************************************************

template <class T, CORBA::ULong MAX>
TAO_Bounded_Object_Sequence<T, MAX>::
TAO_Bounded_Object_Sequence (void)
  :  TAO_Bounded_Base_Sequence (MAX,
                                TAO_Bounded_Sequence<T, MAX>::allocbuf (MAX))
{
}

template <class T, CORBA::ULong MAX>
TAO_Bounded_Object_Sequence<T, MAX>::
TAO_Bounded_Object_Sequence (const TAO_Bounded_Object_Sequence<T, MAX> &rhs)
  : TAO_Bounded_Base_Sequence (rhs)
{
  T **tmp1 =
    TAO_Bounded_Object_Sequence<T, MAX>::allocbuf (MAX);
  T **tmp2 = ACE_reinterpret_cast (T**, rhs.buffer_);
  for (CORBA::ULong i = 0; i < rhs.length_; i++)
    tmp1[i] = T::_duplicate (tmp2[i]);
  this->buffer_ = tmp1;
}

template <class T, CORBA::ULong MAX>
TAO_Bounded_Object_Sequence<T, MAX>&
TAO_Bounded_Object_Sequence<T, MAX>::operator=
(const TAO_Bounded_Object_Sequence<T, MAX> &rhs)
{
  if (this == &rhs)
    return *this;

  if (this->release_)
    {
      T **tmp = ACE_reinterpret_cast (T **, this->buffer_);

      for (CORBA::ULong i = 0; i < this->length_; ++i)
        {
          CORBA::release (tmp[i]);
          tmp[i] = T::_nil ();
        }
      // No need to reallocate the buffer since it is always of size
      // MAX
#if 0
      if (this->maximum_ < rhs.maximum_)
        {
          TAO_Bounded_Object_Sequence<T, MAX>::freebuf (tmp);
          this->buffer_ =
            TAO_Bounded_Object_Sequence<T, MAX>::allocbuf (rhs.maximum_);
        }
#endif /* 0 */
    }
  else
    this->buffer_ =
      TAO_Bounded_Object_Sequence<T, MAX>::allocbuf (rhs.maximum_);

  TAO_Bounded_Base_Sequence::operator= (rhs);

  T **tmp1 = ACE_reinterpret_cast (T **, this->buffer_);
  T **tmp2 = ACE_reinterpret_cast (T **, rhs.buffer_);
  for (CORBA::ULong i=0; i < rhs.length_; ++i)
    tmp1[i] = T::_duplicate (tmp2[i]);
  return *this;
}

template <class T, CORBA::ULong MAX> T* *
TAO_Bounded_Object_Sequence<T, MAX>::allocbuf (CORBA::ULong)
{
  T **buf = new T*[MAX];
  for (CORBA::ULong i = 0; i < MAX; i++)
    buf[i] = T::_nil ();
  return buf;
}

template <class T, CORBA::ULong MAX> void
TAO_Bounded_Object_Sequence<T, MAX>::freebuf (T **buffer)
{
  // How much do we deallocate? Easy! allocbuf() always creates MAX
  // elements and initialize them to T::_nil().  So we can be
  // complaint and call CORBA::release() on each one.
  for (CORBA::ULong i = 0; i < MAX; ++i)
    if (buffer[i] != T::_nil ())
      {
        CORBA::release (buffer[i]);
        buffer[i] = T::_nil ();
      }

  delete[] buffer;
}

template<class T, CORBA::ULong MAX> void
TAO_Bounded_Object_Sequence<T, MAX>::_allocate_buffer (CORBA::ULong length)
{
  // For this class memory is never reallocated so the implementation
  // is *really* simple.
  this->buffer_ =
    TAO_Bounded_Object_Sequence<T, MAX>::allocbuf (length);
}

template<class T, CORBA::ULong MAX>
void TAO_Bounded_Object_Sequence<T, MAX>::_deallocate_buffer (void)
{
  if (this->buffer_ == 0 || this->release_ == 0)
    return;
  T **tmp = ACE_reinterpret_cast (T **, this->buffer_);
  TAO_Bounded_Object_Sequence<T, MAX>::freebuf (tmp);
  this->buffer_ = 0;
}

template<class T, CORBA::ULong MAX> void
TAO_Bounded_Object_Sequence<T, MAX>::_shrink_buffer (CORBA::ULong nl,
                                                     CORBA::ULong ol)
{
  T **tmp = ACE_reinterpret_cast (T **, this->buffer_);

  for (CORBA::ULong i = nl; i < ol; ++i)
    {
      CORBA::release (tmp[i]);
      tmp[i] = T::_nil ();
    }
}

// *************************************************************
// Operations for class TAO_Bounded_String_Sequence
// *************************************************************

template<CORBA::ULong MAX>
TAO_Bounded_String_Sequence<MAX>::
TAO_Bounded_String_Sequence (void)
  :  TAO_Bounded_Base_Sequence (MAX,
                                TAO_Bounded_String_Sequence<MAX>::allocbuf(MAX))
{
}

template<CORBA::ULong MAX>
TAO_Bounded_String_Sequence<MAX>::
TAO_Bounded_String_Sequence (const TAO_Bounded_String_Sequence<MAX> &rhs)
  : TAO_Bounded_Base_Sequence (rhs)
{
  char **tmp1 =
    TAO_Bounded_String_Sequence<MAX>::allocbuf (this->maximum_);
  char **tmp2 = ACE_reinterpret_cast (char **, rhs.buffer_);

  for (CORBA::ULong i=0; i < rhs.length_; i++)
    tmp1[i] = CORBA::string_dup (tmp2[i]);

  this->buffer_ = tmp1;
}

template<CORBA::ULong MAX>
TAO_Bounded_String_Sequence<MAX>&
TAO_Bounded_String_Sequence<MAX>::operator=
(const TAO_Bounded_String_Sequence<MAX> &seq)
{
  if (this == &seq)
    return *this;

  if (this->release_)
    {
      char **tmp = ACE_reinterpret_cast (char **, this->buffer_);

      for (CORBA::ULong i = 0; i < this->length_; ++i)
        {
          CORBA::string_free (tmp[i]);
          tmp[i] = 0;
        }
#if 0
      if (this->maximum_ < seq.maximum_)
        {
          TAO_Bounded_String_Sequence<T, MAX>::freebuf (tmp);
          this->buffer_ =
            TAO_Unbounded_String_Sequence<T, MAX>::allocbuf (rhs.maximum_);
        }
#endif /* 0 */
    }
  else
    this->buffer_ =
      TAO_Bounded_String_Sequence<T>::allocbuf (rhs.maximum_);

  TAO_Bounded_Base_Sequence::operator= (rhs);

  char **tmp1 = ACE_reinterpret_cast (char **, this->buffer_);
  char **tmp2 = ACE_reinterpret_cast (char **, seq.buffer_);

  for (CORBA::ULong i=0; i < seq.length_; i++)
    tmp1[i] = CORBA::string_dup (tmp2[i]);
  return *this;
}

template<CORBA::ULong MAX> char* *
TAO_Bounded_String_Sequence<MAX>::allocbuf (CORBA::ULong)
{
  char **buf = new char*[MAX];

  for (CORBA::ULong i = 0; i < MAX; i++)
    buf[i] = 0;

  return buf;
}

template<CORBA::ULong MAX> void
TAO_Bounded_String_Sequence<MAX>::freebuf (char* *buffer)
{
  // How much do we deallocate? Easy! allocbuf() always creates MAX
  // elements and initialize them to 0 (they say NULL, yuck!).  So we
  // can be complaint and call CORBA::string_free() on each one.
  for (CORBA::ULong i = 0; i < MAX; ++i)
    {
      if (buffer[i] != 0)
        {
          CORBA::string_free (buffer[i]);
          buffer[i] = 0;
        }
    }

  delete[] buffer;
}

template<CORBA::ULong MAX> void
TAO_Bounded_String_Sequence<MAX>::_allocate_buffer (CORBA::ULong length)
{
  // For this class memory is never reallocated so the implementation
  // is *really* simple.
  this->buffer_ =
    TAO_Bounded_String_Sequence<MAX>::allocbuf (MAX);
}

template<CORBA::ULong MAX>
void TAO_Bounded_String_Sequence<MAX>::_deallocate_buffer (void)
{
  if (this->buffer_ == 0 || this->release_ == 0)
    return;
  char **tmp = ACE_reinterpret_cast (char **, this->buffer_);
  TAO_Bounded_String_Sequence<MAX>::freebuf (tmp);
  this->buffer_ = 0;
}

template<CORBA::ULong MAX> void
TAO_Bounded_String_Sequence<MAX>::_shrink_buffer (CORBA::ULong nl,
                                                  CORBA::ULong ol)
{
  char **tmp = ACE_reinterpret_cast (char **, this->buffer_);
  for (CORBA::ULong i = nl; i < ol; ++i)
    {
      CORBA::string_free (tmp[i]);
      tmp[i] = 0;
    }
}

#endif /* TAO_SEQUENCE_T_C */
