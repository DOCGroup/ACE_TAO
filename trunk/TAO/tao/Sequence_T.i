// This may look like C, but it's really -*- C++ -*-
// $Id$

// ***************************************************
// Operations on the generic unbounded sequence class.
// ***************************************************

template <class T> ACE_INLINE
TAO_Unbounded_Sequence<T>::TAO_Unbounded_Sequence (void)
{
}

template <class T> ACE_INLINE T *
TAO_Unbounded_Sequence<T>::get_buffer (CORBA::Boolean orphan)
{
  if (orphan == CORBA::B_FALSE)
    {
      // We retain ownership.

      if (this->buffer_ == 0)
        this->buffer_ = (void *) new T[this->length_];
    }
  else // if (orphan == CORBA::B_TRUE)
    {
      if (this->release_ == CORBA::B_FALSE)
        // Oops, it's not our buffer to relinquish...
        return 0;
      else
        {
          // We set the state back to default and relinquish
          // ownership.
          this->maximum_ = 0;
          this->length_ = 0;
          this->buffer_ = 0;
          this->release_ = CORBA::B_FALSE;
        }
    }
  return (T *) this->buffer_;
}

template <class T> ACE_INLINE const T *
TAO_Unbounded_Sequence<T>::get_buffer (void) const
{
  return (const T *) this->buffer_;
}

template <class T> ACE_INLINE void
TAO_Unbounded_Sequence<T>::replace (CORBA::ULong max,
                                    CORBA::ULong length,
                                    T *data,
                                    CORBA::Boolean release)
{
  this->maximum_ = max;
  this->length_ = length;
  if (this->buffer_ && this->release_ == CORBA::B_TRUE)
    TAO_Unbounded_Sequence<T>::freebuf ((T *) this->buffer_);
  this->buffer_ = data;
  this->release_ = release;
}

template <class T> ACE_INLINE
TAO_Unbounded_Sequence<T>::TAO_Unbounded_Sequence (CORBA::ULong maximum)
  : TAO_Unbounded_Base_Sequence (maximum,
                                 TAO_Unbounded_Sequence<T>::allocbuf (maximum))
{
}

template <class T> ACE_INLINE
TAO_Unbounded_Sequence<T>::TAO_Unbounded_Sequence (CORBA::ULong maximum,
                                                   CORBA::ULong length,
                                                   T *data,
                                                   CORBA::Boolean release)
  : TAO_Unbounded_Base_Sequence (maximum, length, data, release)
{
}

template <class T> ACE_INLINE T &
TAO_Unbounded_Sequence<T>::operator[] (CORBA::ULong i)
{
  ACE_ASSERT (i < this->maximum_);
  T* tmp = ACE_reinterpret_cast(T*,this->buffer_);
  return tmp[i];
}

template <class T> ACE_INLINE const T &
TAO_Unbounded_Sequence<T>::operator[] (CORBA::ULong i) const
{
  ACE_ASSERT (i < this->maximum_);
  T * const tmp = ACE_reinterpret_cast (T* const, this->buffer_);
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

// ***************************************************
// operations on the generic Bounded sequence class
// ***************************************************

template <class T, CORBA::ULong MAX> ACE_INLINE T *
TAO_Bounded_Sequence<T, MAX>::get_buffer (CORBA::Boolean orphan)
{
  if (orphan == CORBA::B_FALSE)
    {
      // We retain ownership.

      if (this->buffer_ == 0)
        this->buffer_ = (void *) new T[this->maximum_];
    }
  else // if (orphan == CORBA::B_TRUE)
    {
      if (this->release_ == CORBA::B_FALSE)
        // Oops, it's not our buffer to relinquish...
        return 0;
      else
        {
          // We set the state back to default and relinquish
          // ownership.
          this->maximum_ = 0;
          this->length_ = 0;
          this->buffer_ = 0;
          this->release_ = CORBA::B_FALSE;
        }
    }
  return (T *) this->buffer_;
}

template <class T, CORBA::ULong MAX> ACE_INLINE const T *
TAO_Bounded_Sequence<T, MAX>::get_buffer (void) const
{
  return (const T *) this->buffer_;
}

template <class T, CORBA::ULong MAX> ACE_INLINE void
TAO_Bounded_Sequence<T, MAX>::replace (CORBA::ULong max,
                                       CORBA::ULong length,
                                       T *data,
                                       CORBA::Boolean release)
{
  this->maximum_ = max;
  this->length_ = length;
  if (this->buffer_ && this->release_ == CORBA::B_TRUE)
    TAO_Bounded_Sequence<T, MAX>::freebuf ((T *) this->buffer_);
  this->buffer_ = data;
  this->release_ = release;
}

template <class T, CORBA::ULong MAX> ACE_INLINE
TAO_Bounded_Sequence<T, MAX>::TAO_Bounded_Sequence (void)
{
}

template <class T, CORBA::ULong MAX> ACE_INLINE
TAO_Bounded_Sequence<T, MAX>::TAO_Bounded_Sequence (CORBA::ULong length,
                                                    T *data,
                                                    CORBA::Boolean release)
  : TAO_Bounded_Base_Sequence (length, MAX, data, release)
{
}

template <class T, CORBA::ULong MAX> ACE_INLINE T &
TAO_Bounded_Sequence<T, MAX>::operator[] (CORBA::ULong i)
{
  ACE_ASSERT (i < this->maximum_);
  T *tmp = ACE_reinterpret_cast (T*,this->buffer_);
  return tmp[i];
}

template <class T, CORBA::ULong MAX> ACE_INLINE const T &
TAO_Bounded_Sequence<T, MAX>::operator[] (CORBA::ULong i) const
{
  ACE_ASSERT (i < this->maximum_);
  const T* tmp = ACE_reinterpret_cast (const T*,this->buffer_);
  return tmp[i];
}

template <class T, CORBA::ULong MAX> ACE_INLINE T *
TAO_Bounded_Sequence<T, MAX>::allocbuf (CORBA::ULong)
{
  return new T[MAX];
}

template <class T, CORBA::ULong MAX> ACE_INLINE void
TAO_Bounded_Sequence<T, MAX>::freebuf (T *buffer)
{
  delete [] buffer;
}

// *************************************************************
// Inline operations for class TAO_Object_Manager<T>
// *************************************************************

template <class T> ACE_INLINE
TAO_Object_Manager<T>::~TAO_Object_Manager (void)
{
}

template <class T> ACE_INLINE
TAO_Object_Manager<T>::TAO_Object_Manager (const TAO_Object_Manager &rhs)
  : ptr_ (rhs.ptr_),
    release_ (rhs.release_)
{
}

template <class T> ACE_INLINE
TAO_Object_Manager<T>::TAO_Object_Manager(T** buffer, CORBA::Boolean release)
  : ptr_ (buffer),
    release_ (release)
{
}

template <class T> ACE_INLINE
TAO_Object_Manager<T>::operator const T* () const // cast
{
  return *this->ptr_;
}

template <class T> ACE_INLINE
TAO_Object_Manager<T>::operator T* &() // cast
{
  return *this->ptr_;
}

// *************************************************************
// class TAO_Unbounded_Object_Sequence
// *************************************************************

//default constructor
template <class T> ACE_INLINE
TAO_Unbounded_Object_Sequence<T>::TAO_Unbounded_Object_Sequence (void)
{
}

template <class T> ACE_INLINE
TAO_Unbounded_Object_Sequence<T>::
TAO_Unbounded_Object_Sequence (CORBA::ULong maximum,
                               CORBA::ULong length,
                               T* *value,
                               CORBA::Boolean release)
  : TAO_Unbounded_Base_Sequence (maximum, length, value, release)
{
}

template <class T> ACE_INLINE TAO_Object_Manager<T>
TAO_Unbounded_Object_Sequence<T>::operator[] (CORBA::ULong index) const
{
  ACE_ASSERT (index < this->maximum_);
  T ** const tmp = ACE_reinterpret_cast (T ** const, this->buffer_);
  return TAO_Object_Manager<T> (tmp + index, this->release_);
}

// *************************************************************
// class TAO_Bounded_Object_Sequence
// *************************************************************

template<class T, CORBA::ULong MAX> ACE_INLINE
TAO_Bounded_Object_Sequence<T,MAX>::~TAO_Bounded_Object_Sequence (void)
{
  this->_deallocate_buffer ();
}

template <class T, CORBA::ULong MAX> ACE_INLINE
TAO_Bounded_Object_Sequence<T,MAX>::
TAO_Bounded_Object_Sequence (CORBA::ULong length,
                             T **value,
                             CORBA::Boolean release)
  : TAO_Bounded_Base_Sequence (MAX, length, value, release)
{
}

template <class T, CORBA::ULong MAX> ACE_INLINE TAO_Object_Manager<T>
TAO_Bounded_Object_Sequence<T, MAX>::operator[] (CORBA::ULong index) const
{
  ACE_ASSERT (index < this->maximum_);
  T **const tmp = ACE_reinterpret_cast (T **const, this->buffer_);
  return TAO_Object_Manager<T> (tmp + index, this->release_);
}

// *************************************************************
// class TAO_Bounded_String_Sequence
// *************************************************************

template<CORBA::ULong MAX> ACE_INLINE
TAO_Bounded_String_Sequence<MAX>::~TAO_Bounded_String_Sequence (void)
{
  this->_deallocate_buffer ();
}

template<CORBA::ULong MAX> ACE_INLINE
TAO_Bounded_String_Sequence<MAX>::
TAO_Bounded_String_Sequence (CORBA::ULong length,
                             char **value,
                             CORBA::Boolean release)
  : TAO_Bounded_Base_Sequence (MAX, length, value, release)
{
}

template<CORBA::ULong MAX> ACE_INLINE TAO_String_Manager
TAO_Bounded_String_Sequence<MAX>::operator[] (CORBA::ULong index) const
{
  ACE_ASSERT (index < this->maximum_);
  char **const tmp = ACE_reinterpret_cast (char **const, this->buffer_);
  return TAO_String_Manager (tmp + index, this->release_);
}
