// This may look like C, but it's really -*- C++ -*-
// $Id$

// ***************************************************
// Operations on the generic unbounded sequence class.
// ***************************************************

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

template <class T> ACE_INLINE
TAO_Unbounded_Sequence<T>::TAO_Unbounded_Sequence (void)
{
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

template <class T> ACE_INLINE T *
TAO_Unbounded_Sequence<T>::get_buffer (CORBA::Boolean orphan)
{
  T *result = 0;
  if (orphan == 0)
    {
      // We retain ownership.
      if (this->buffer_ == 0)
        {
          if (this->length_ > 0)
            {
              result = TAO_Unbounded_Sequence<T>::allocbuf (this->length_);
              this->buffer_ = result;
	      this->release_ = 1;
            }
        }
      else
        {
          result =
            ACE_reinterpret_cast (T*, 
	                          this->buffer_);
        }
    }
  else // if (orphan == 1)
    {
      result = ACE_reinterpret_cast (T*,
	                             this->buffer_);

      if (this->release_ != 0)
        {
          // We set the state back to default and relinquish
          // ownership.
          this->maximum_ = 0;
          this->length_ = 0;
          this->buffer_ = 0;
          this->release_ = 0;
        }
    }

  return result;
}

template <class T> ACE_INLINE const T *
TAO_Unbounded_Sequence<T>::get_buffer (void) const
{
  return ACE_reinterpret_cast (const T * ACE_CAST_CONST, this->buffer_);
}

template <class T> ACE_INLINE void
TAO_Unbounded_Sequence<T>::replace (CORBA::ULong max,
                                    CORBA::ULong length,
                                    T *data,
                                    CORBA::Boolean release)
{
  this->maximum_ = max;
  this->length_ = length;
  if (this->buffer_ && this->release_ == 1)
    {
      T *tmp = ACE_reinterpret_cast(T*,this->buffer_);
      TAO_Unbounded_Sequence<T>::freebuf (tmp);
    }
  this->buffer_ = data;
  this->release_ = release;
}

// ***************************************************
// operations on the generic Bounded sequence class
// ***************************************************

template <class T, size_t MAX> ACE_INLINE T *
TAO_Bounded_Sequence<T, MAX>::allocbuf (CORBA::ULong)
{
  return new T[MAX];
}

template <class T, size_t MAX> ACE_INLINE void
TAO_Bounded_Sequence<T, MAX>::freebuf (T *buffer)
{
  delete [] buffer;
}

template <class T, size_t MAX> ACE_INLINE
TAO_Bounded_Sequence<T, MAX>::TAO_Bounded_Sequence (void)
{
  this->maximum_ = MAX;
}

template <class T, size_t MAX> ACE_INLINE
TAO_Bounded_Sequence<T, MAX>::TAO_Bounded_Sequence (CORBA::ULong length,
                                                    T *data,
                                                    CORBA::Boolean release)
  : TAO_Bounded_Base_Sequence (MAX, length, data, release)
{
}

template <class T, size_t MAX> ACE_INLINE T *
TAO_Bounded_Sequence<T, MAX>::get_buffer (CORBA::Boolean orphan)
{
  T *result = 0;
  if (orphan == 0)
    {
      // We retain ownership.

      if (this->buffer_ == 0)
        {
          result = TAO_Bounded_Sequence<T,MAX>::allocbuf (this->maximum_);
          this->buffer_ = result;
	  this->release_ = 1;
        }
      else
        {
          result =
            ACE_reinterpret_cast (T*, this->buffer_);
        }
    }
  else // if (orphan == 1)
    {
      if (this->release_ != 0)
        {
          // We set the state back to default and relinquish
          // ownership.
          result = ACE_reinterpret_cast(T*,this->buffer_);
          this->maximum_ = 0;
          this->length_ = 0;
          this->buffer_ = 0;
          this->release_ = 0;
        }
    }
  return result;
}

template <class T, size_t MAX> ACE_INLINE const T *
TAO_Bounded_Sequence<T, MAX>::get_buffer (void) const
{
  return ACE_reinterpret_cast(const T * ACE_CAST_CONST, this->buffer_);
}

template <class T, size_t MAX> ACE_INLINE void
TAO_Bounded_Sequence<T, MAX>::replace (CORBA::ULong max,
                                       CORBA::ULong length,
                                       T *data,
                                       CORBA::Boolean release)
{
  this->maximum_ = max;
  this->length_ = length;
  if (this->buffer_ && this->release_ == 1)
    {
      T* tmp = ACE_reinterpret_cast(T*, this->buffer_);
      TAO_Bounded_Sequence<T, MAX>::freebuf (tmp);
    }
  this->buffer_ = data;
  this->release_ = release;
}

template <class T, size_t MAX> ACE_INLINE T &
TAO_Bounded_Sequence<T, MAX>::operator[] (CORBA::ULong i)
{
  ACE_ASSERT (i < this->maximum_);
  T *tmp = ACE_reinterpret_cast (T*,this->buffer_);
  return tmp[i];
}

template <class T, size_t MAX> ACE_INLINE const T &
TAO_Bounded_Sequence<T, MAX>::operator[] (CORBA::ULong i) const
{
  ACE_ASSERT (i < this->maximum_);
  const T* tmp = ACE_reinterpret_cast (const T* ACE_CAST_CONST,this->buffer_);
  return tmp[i];
}

// *************************************************************
// Inline operations for class TAO_Object_Manager<T>
// *************************************************************

template <class T, class T_var> ACE_INLINE
TAO_Object_Manager<T,T_var>::~TAO_Object_Manager (void)
{
}

template <class T, class T_var> ACE_INLINE
TAO_Object_Manager<T,T_var>::TAO_Object_Manager (const TAO_Object_Manager<T,T_var> &rhs)
  : ptr_ (rhs.ptr_),
    release_ (rhs.release_)
{
}

template <class T, class T_var> ACE_INLINE
TAO_Object_Manager<T,T_var>::TAO_Object_Manager(T** buffer, CORBA::Boolean release)
  : ptr_ (buffer),
    release_ (release)
{
}

template <class T, class T_var> ACE_INLINE T *
TAO_Object_Manager<T,T_var>::operator-> (void) const
{
  return *this->ptr_;
}

template <class T, class T_var> ACE_INLINE
TAO_Object_Manager<T,T_var>::operator const T_var () const
{
  return T_var::tao_duplicate (*this->ptr_);
}

template <class T, class T_var> ACE_INLINE
TAO_Object_Manager<T,T_var>::operator const T* () const // cast
{
  return *this->ptr_;
}

template <class T, class T_var> ACE_INLINE
TAO_Object_Manager<T,T_var>::operator T* &() // cast
{
  return *this->ptr_;
}

template <class T, class T_var> ACE_INLINE T *
TAO_Object_Manager<T,T_var>::in (void) const
{
  return *this->ptr_;
}

template <class T, class T_var> ACE_INLINE T *&
TAO_Object_Manager<T,T_var>::inout (void)
{
  return *this->ptr_;
}

// *************************************************************
// Inline operations for class TAO_Pseudo_Object_Manager<T>
// *************************************************************

template <class T, class T_var> ACE_INLINE
TAO_Pseudo_Object_Manager<T,T_var>::~TAO_Pseudo_Object_Manager (void)
{
}

template <class T, class T_var> ACE_INLINE
TAO_Pseudo_Object_Manager<T,T_var>::
    TAO_Pseudo_Object_Manager (const TAO_Pseudo_Object_Manager<T,T_var> &rhs)
  : ptr_ (rhs.ptr_),
    release_ (rhs.release_)
{
}

template <class T, class T_var> ACE_INLINE
TAO_Pseudo_Object_Manager<T,T_var>::
    TAO_Pseudo_Object_Manager(T** buffer, CORBA::Boolean release)
  : ptr_ (buffer),
    release_ (release)
{
}

template <class T, class T_var> ACE_INLINE T *
TAO_Pseudo_Object_Manager<T,T_var>::operator-> (void) const
{
  return *this->ptr_;
}

template <class T, class T_var> ACE_INLINE
TAO_Pseudo_Object_Manager<T,T_var>::operator const T* () const // cast
{
  return *this->ptr_;
}

template <class T, class T_var> ACE_INLINE
TAO_Pseudo_Object_Manager<T,T_var>::operator T* &() // cast
{
  return *this->ptr_;
}

template <class T, class T_var> ACE_INLINE T *
TAO_Pseudo_Object_Manager<T,T_var>::in (void) const
{
  return *this->ptr_;
}

template <class T, class T_var> ACE_INLINE T *&
TAO_Pseudo_Object_Manager<T,T_var>::inout (void)
{
  return *this->ptr_;
}

template <class T, class T_var> ACE_INLINE T *&
TAO_Pseudo_Object_Manager<T,T_var>::out (void)
{
  CORBA::release (*this->ptr_);
  *this->ptr_ = T::_nil ();
  return *this->ptr_;
}

template <class T, class T_var> ACE_INLINE T *
TAO_Pseudo_Object_Manager<T,T_var>::_retn (void)
{
  T *temp = *this->ptr_;
  *this->ptr_ = T::_nil ();
  return temp;
}

// *************************************************************
// Inline operations for class TAO_Valuetype_Manager
// *************************************************************

template <class T, class T_var> ACE_INLINE
TAO_Valuetype_Manager<T,T_var>::~TAO_Valuetype_Manager (void)
{
}

template <class T, class T_var> ACE_INLINE
TAO_Valuetype_Manager<T,T_var>::TAO_Valuetype_Manager (const TAO_Valuetype_Manager<T,T_var> &rhs)
  : ptr_ (rhs.ptr_),
    release_ (rhs.release_)
{
}

template <class T, class T_var> ACE_INLINE
TAO_Valuetype_Manager<T,T_var>::TAO_Valuetype_Manager(T** buffer, CORBA::Boolean release)
  : ptr_ (buffer),
    release_ (release)
{
}

template <class T, class T_var> ACE_INLINE T *
TAO_Valuetype_Manager<T,T_var>::operator-> (void) const
{
  return *this->ptr_;
}

template <class T, class T_var> ACE_INLINE
TAO_Valuetype_Manager<T,T_var>::operator const T_var () const
{
  if (*this->ptr_ != 0)
    (*this->ptr_)->_add_ref ();
  return *this->ptr_;
}

template <class T, class T_var> ACE_INLINE
TAO_Valuetype_Manager<T,T_var>::operator const T* () const // cast
{
  return *this->ptr_;
}

template <class T, class T_var> ACE_INLINE
TAO_Valuetype_Manager<T,T_var>::operator T* &() // cast
{
  return *this->ptr_;
}

template <class T, class T_var> ACE_INLINE T *
TAO_Valuetype_Manager<T,T_var>::in (void) const
{
  return *this->ptr_;
}

template <class T, class T_var> ACE_INLINE T *&
TAO_Valuetype_Manager<T,T_var>::inout (void)
{
  return *this->ptr_;
}

template <class T, class T_var> ACE_INLINE T *&
TAO_Valuetype_Manager<T,T_var>::out (void)
{
  if (*this->ptr_ != 0)
    (*this->ptr_)->_remove_ref ();
  *this->ptr_ = 0;
  return *this->ptr_;
}

template <class T, class T_var> ACE_INLINE T *
TAO_Valuetype_Manager<T,T_var>::_retn (void)
{
  T *temp = *this->ptr_;
  *this->ptr_ = 0;
  return temp;
}

// *************************************************************
// class TAO_Unbounded_Object_Sequence
// *************************************************************

//default constructor
template <class T, class T_var> ACE_INLINE
TAO_Unbounded_Object_Sequence<T,T_var>::TAO_Unbounded_Object_Sequence (void)
{
}

template <class T, class T_var> ACE_INLINE
TAO_Unbounded_Object_Sequence<T,T_var>::
TAO_Unbounded_Object_Sequence (CORBA::ULong maximum,
                               CORBA::ULong length,
                               T* *value,
                               CORBA::Boolean release)
  : TAO_Unbounded_Base_Sequence (maximum, length, value, release)
{
}

template <class T, class T_var> ACE_INLINE TAO_Object_Manager<T,T_var>
TAO_Unbounded_Object_Sequence<T,T_var>::operator[] (CORBA::ULong slot) const
{
  ACE_ASSERT (slot < this->maximum_);
  T ** const tmp = ACE_reinterpret_cast (T ** ACE_CAST_CONST, this->buffer_);
  return TAO_Object_Manager<T,T_var> (tmp + slot, this->release_);
}

// *************************************************************
// class TAO_Bounded_Object_Sequence
// *************************************************************

template<class T, class T_var, size_t MAX> ACE_INLINE
TAO_Bounded_Object_Sequence<T,T_var,MAX>::~TAO_Bounded_Object_Sequence (void)
{
  this->_deallocate_buffer ();
}

template <class T, class T_var, size_t MAX> ACE_INLINE
TAO_Bounded_Object_Sequence<T,T_var,MAX>::
TAO_Bounded_Object_Sequence (CORBA::ULong length,
                             T **value,
                             CORBA::Boolean release)
  : TAO_Bounded_Base_Sequence (MAX, length, value, release)
{
}

template <class T, class T_var, size_t MAX> ACE_INLINE TAO_Object_Manager<T,T_var>
TAO_Bounded_Object_Sequence<T, T_var,MAX>::operator[] (CORBA::ULong slot) const
{
  ACE_ASSERT (slot < this->maximum_);
  T **const tmp = ACE_reinterpret_cast (T ** ACE_CAST_CONST, this->buffer_);
  return TAO_Object_Manager<T,T_var> (tmp + slot, this->release_);
}

// *************************************************************
// class TAO_Unbounded_Pseudo_Sequence
// *************************************************************

//default constructor
template <class T, class T_var> ACE_INLINE
TAO_Unbounded_Pseudo_Sequence<T,T_var>::TAO_Unbounded_Pseudo_Sequence (void)
{
}

template <class T, class T_var> ACE_INLINE
TAO_Unbounded_Pseudo_Sequence<T,T_var>::
TAO_Unbounded_Pseudo_Sequence (CORBA::ULong maximum,
                               CORBA::ULong length,
                               T* *value,
                               CORBA::Boolean release)
  : TAO_Unbounded_Base_Sequence (maximum, length, value, release)
{
}

template <class T, class T_var> ACE_INLINE TAO_Pseudo_Object_Manager<T,T_var>
TAO_Unbounded_Pseudo_Sequence<T,T_var>::operator[] (CORBA::ULong slot) const
{
  ACE_ASSERT (slot < this->maximum_);
  T ** const tmp = ACE_reinterpret_cast (T ** ACE_CAST_CONST, this->buffer_);
  return TAO_Pseudo_Object_Manager<T,T_var> (tmp + slot, this->release_);
}

// *************************************************************
// class TAO_Bounded_Pseudo_Sequence
// *************************************************************

template<class T, class T_var, size_t MAX> ACE_INLINE
TAO_Bounded_Pseudo_Sequence<T,T_var,MAX>::~TAO_Bounded_Pseudo_Sequence (void)
{
  this->_deallocate_buffer ();
}

template <class T, class T_var, size_t MAX> ACE_INLINE
TAO_Bounded_Pseudo_Sequence<T,T_var,MAX>::
TAO_Bounded_Pseudo_Sequence (CORBA::ULong length,
                             T **value,
                             CORBA::Boolean release)
  : TAO_Bounded_Base_Sequence (MAX, length, value, release)
{
}

template <class T, class T_var, size_t MAX> ACE_INLINE TAO_Pseudo_Object_Manager<T,T_var>
TAO_Bounded_Pseudo_Sequence<T, T_var,MAX>::operator[] (CORBA::ULong slot) const
{
  ACE_ASSERT (slot < this->maximum_);
  T **const tmp = ACE_reinterpret_cast (T ** ACE_CAST_CONST,
                                        this->buffer_);
  return TAO_Pseudo_Object_Manager<T,T_var> (tmp + slot,
                                             this->release_);
}

// *************************************************************
// class TAO_Unbounded_Valuetype_Sequence
// *************************************************************

//default constructor
template <class T, class T_var> ACE_INLINE
TAO_Unbounded_Valuetype_Sequence<T,T_var>::TAO_Unbounded_Valuetype_Sequence (void)
{
}

template <class T, class T_var> ACE_INLINE
TAO_Unbounded_Valuetype_Sequence<T,T_var>::
TAO_Unbounded_Valuetype_Sequence (CORBA::ULong maximum,
                                  CORBA::ULong length,
                                  T* *value,
                                  CORBA::Boolean release)
  : TAO_Unbounded_Base_Sequence (maximum, length, value, release)
{
}

template <class T, class T_var> ACE_INLINE TAO_Valuetype_Manager<T,T_var>
TAO_Unbounded_Valuetype_Sequence<T,T_var>::operator[] (CORBA::ULong slot) const
{
  ACE_ASSERT (slot < this->maximum_);
  T ** const tmp = ACE_reinterpret_cast (T ** ACE_CAST_CONST, this->buffer_);
  return TAO_Valuetype_Manager<T,T_var> (tmp + slot, this->release_);
}

// *************************************************************
// class TAO_Bounded_Valuetype_Sequence
// *************************************************************

template<class T, class T_var, size_t MAX> ACE_INLINE
TAO_Bounded_Valuetype_Sequence<T,T_var,MAX>::~TAO_Bounded_Valuetype_Sequence (void)
{
  this->_deallocate_buffer ();
}

template <class T, class T_var, size_t MAX> ACE_INLINE
TAO_Bounded_Valuetype_Sequence<T,T_var,MAX>::
TAO_Bounded_Valuetype_Sequence (CORBA::ULong length,
                                T **value,
                                CORBA::Boolean release)
  : TAO_Bounded_Base_Sequence (MAX, length, value, release)
{
}

template <class T, class T_var, size_t MAX> ACE_INLINE TAO_Valuetype_Manager<T,T_var>
TAO_Bounded_Valuetype_Sequence<T, T_var,MAX>::operator[] (CORBA::ULong slot) const
{
  ACE_ASSERT (slot < this->maximum_);
  T **const tmp = ACE_reinterpret_cast (T ** ACE_CAST_CONST, this->buffer_);
  return TAO_Valuetype_Manager<T,T_var> (tmp + slot, this->release_);
}

// *************************************************************
// class TAO_Unbounded_Array_Sequence
// *************************************************************

template <class T, class T_var> ACE_INLINE void
TAO_Unbounded_Array_Sequence<T, T_var>::freebuf (T *buffer)
{
  if (buffer == 0)
    return;

  delete [] buffer;
}

//default constructor
template <class T, class T_var> ACE_INLINE
TAO_Unbounded_Array_Sequence<T, T_var>::TAO_Unbounded_Array_Sequence (void)
{
}

template <class T, class T_var> ACE_INLINE
TAO_Unbounded_Array_Sequence<T, T_var>::
TAO_Unbounded_Array_Sequence (CORBA::ULong maximum)
  : TAO_Unbounded_Base_Sequence (maximum,
                                 TAO_Unbounded_Array_Sequence<T, T_var>::allocbuf (maximum))
{
}

template <class T, class T_var> ACE_INLINE
TAO_Unbounded_Array_Sequence<T, T_var>::
TAO_Unbounded_Array_Sequence (CORBA::ULong maximum,
                              CORBA::ULong length,
                              T *value,
                              CORBA::Boolean release)
  : TAO_Unbounded_Base_Sequence (maximum, length, value, release)
{
}

template <class T, class T_var> ACE_INLINE T &
TAO_Unbounded_Array_Sequence<T, T_var>::operator[] (CORBA::ULong i)
{
  ACE_ASSERT (i < this->maximum_);

#if defined (__SUNPRO_CC) && (__SUNPRO_CC < 0x500)
  return ((T*) this->buffer_)[i];
#else /* (__SUNPRO_CC) && (__SUNPRO_CC < 0x500) */
  return (ACE_reinterpret_cast (T*, this->buffer_))[i];
#endif /* (__SUNPRO_CC) && (__SUNPRO_CC < 0x500) */
}

template <class T, class T_var> ACE_INLINE const T &
TAO_Unbounded_Array_Sequence<T, T_var>::operator[] (CORBA::ULong i) const
{
  ACE_ASSERT (i < this->maximum_);

#if defined (__SUNPRO_CC) && (__SUNPRO_CC < 0x500)
  return ((const T* ACE_CAST_CONST) this->buffer_)[i];
#else /* (__SUNPRO_CC) && (__SUNPRO_CC < 0x500) */
  return (ACE_reinterpret_cast (const T* ACE_CAST_CONST, this->buffer_))[i];
#endif /* (__SUNPRO_CC) && (__SUNPRO_CC < 0x500) */
}

template <class T, class T_var> ACE_INLINE T *
TAO_Unbounded_Array_Sequence<T, T_var>::get_buffer (CORBA::Boolean orphan)
{
  T *result = 0;
  if (orphan == 0)
    {
      // We retain ownership.

      if (this->buffer_ == 0)
        {
          if (this->length_ > 0)
            {
              result = 
	        TAO_Unbounded_Array_Sequence<T, T_var>::allocbuf (
                    this->length_
                  );
              this->buffer_ = result;
	      this->release_ = 1;
            }
        }
      else
        {
          result =
            ACE_reinterpret_cast (T*, 
	                          this->buffer_);
        }
    }
  else // if (orphan == 1)
    {
      if (this->release_ != 0)
        {
          // We set the state back to default and relinquish
          // ownership.
          result = ACE_reinterpret_cast (T*,
	                                 this->buffer_);
          this->maximum_ = 0;
          this->length_ = 0;
          this->buffer_ = 0;
          this->release_ = 0;
        }
    }
  return result;
}

template <class T, class T_var> ACE_INLINE const T *
TAO_Unbounded_Array_Sequence<T, T_var>::get_buffer (void) const
{
  return ACE_reinterpret_cast (const T * ACE_CAST_CONST, 
                               this->buffer_);
}

template <class T, class T_var> ACE_INLINE void
TAO_Unbounded_Array_Sequence<T, T_var>::replace (CORBA::ULong max,
				                   CORBA::ULong length,
                                                   T *data,
                                                   CORBA::Boolean release)
{
  this->maximum_ = max;
  this->length_ = length;
  if (this->buffer_ && this->release_ == 1)
    {
      T *tmp = ACE_reinterpret_cast (T*,
                                     this->buffer_);
      TAO_Unbounded_Array_Sequence<T, T_var>::freebuf (tmp);
    }
  this->buffer_ = data;
  this->release_ = release;
}

// *************************************************************
// class TAO_Bounded_Array_Sequence
// *************************************************************

template <class T, class T_var, size_t MAX> ACE_INLINE void
TAO_Bounded_Array_Sequence<T, T_var, MAX>::freebuf (T *buffer)
{
  if (buffer == 0)
    return;

  delete [] buffer;
}

template <class T, class T_var, size_t MAX> ACE_INLINE
TAO_Bounded_Array_Sequence<T, T_var, MAX>::
TAO_Bounded_Array_Sequence (void)
  : TAO_Bounded_Base_Sequence (MAX, 0, allocbuf (MAX), 1)
{
}

template <class T, class T_var, size_t MAX> ACE_INLINE
TAO_Bounded_Array_Sequence<T, T_var, MAX>::
TAO_Bounded_Array_Sequence (CORBA::ULong length,
                            T *data,
                            CORBA::Boolean release)
  : TAO_Bounded_Base_Sequence (MAX, length, data, release)
{
}

template <class T, class T_var, size_t MAX> ACE_INLINE T &
TAO_Bounded_Array_Sequence<T, T_var, MAX>::operator[] (CORBA::ULong i)
{
  ACE_ASSERT (i < this->maximum_);

#if defined (__SUNPRO_CC) && (__SUNPRO_CC < 0x500)
  return ((T*) this->buffer_)[i];
#else /* (__SUNPRO_CC) && (__SUNPRO_CC < 0x500) */
  return (ACE_reinterpret_cast (T*, this->buffer_))[i];
#endif /* (__SUNPRO_CC) && (__SUNPRO_CC < 0x500) */
}

template <class T, class T_var, size_t MAX> ACE_INLINE const T &
TAO_Bounded_Array_Sequence<T, T_var, MAX>::operator[] (CORBA::ULong i) const
{
  ACE_ASSERT (i < this->maximum_);

#if defined (__SUNPRO_CC) && (__SUNPRO_CC < 0x500)
  return ((const T* ACE_CAST_CONST) this->buffer_)[i];
#else /* (__SUNPRO_CC) && (__SUNPRO_CC < 0x500) */
  return (ACE_reinterpret_cast (const T* ACE_CAST_CONST, this->buffer_))[i];
#endif /* (__SUNPRO_CC) && (__SUNPRO_CC < 0x500) */
}

template <class T, class T_var, size_t MAX> ACE_INLINE T *
TAO_Bounded_Array_Sequence<T, T_var, MAX>::get_buffer (CORBA::Boolean orphan)
{
  T *result = 0;
  if (orphan == 0)
    {
      // We retain ownership.

      if (this->buffer_ == 0)
        {
          result = 
	    TAO_Bounded_Array_Sequence<T, T_var, MAX>::allocbuf (this->maximum_);
          this->buffer_ = result;
	  this->release_ = 1;
        }
      else
        {
          result =
            ACE_reinterpret_cast (T*, this->buffer_);
        }
    }
  else // if (orphan == 1)
    {
      if (this->release_ != 0)
        {
          // We set the state back to default and relinquish
          // ownership.
          result = ACE_reinterpret_cast(T*,this->buffer_);
          this->maximum_ = 0;
          this->length_ = 0;
          this->buffer_ = 0;
          this->release_ = 0;
        }
    }
  return result;
}

template <class T, class T_var, size_t MAX> ACE_INLINE const T *
TAO_Bounded_Array_Sequence<T, T_var, MAX>::get_buffer (void) const
{
  return ACE_reinterpret_cast (const T * ACE_CAST_CONST, 
                               this->buffer_);
}

template <class T, class T_var, size_t MAX> ACE_INLINE void
TAO_Bounded_Array_Sequence<T, T_var, MAX>::replace (CORBA::ULong max,
                                                      CORBA::ULong length,
                                                      T *data,
                                                      CORBA::Boolean release)
{
  this->maximum_ = max;
  this->length_ = length;
  if (this->buffer_ && this->release_ == 1)
    {
      T* tmp = ACE_reinterpret_cast (T*, 
                                     this->buffer_);
      TAO_Bounded_Array_Sequence<T, T_var, MAX>::freebuf (tmp);
    }
  this->buffer_ = data;
  this->release_ = release;
}

// *************************************************************
// class TAO_Bounded_String_Sequence
// *************************************************************

template<size_t MAX> ACE_INLINE
TAO_Bounded_String_Sequence<MAX>::~TAO_Bounded_String_Sequence (void)
{
  this->_deallocate_buffer ();
}

template<size_t MAX> ACE_INLINE
TAO_Bounded_String_Sequence<MAX>::
TAO_Bounded_String_Sequence (CORBA::ULong length,
                             char **value,
                             CORBA::Boolean release)
  : TAO_Bounded_Base_Sequence (MAX, length, value, release)
{
}

// *************************************************************
// class TAO_Bounded_WString_Sequence
// *************************************************************

template<size_t MAX> ACE_INLINE
TAO_Bounded_WString_Sequence<MAX>::~TAO_Bounded_WString_Sequence (void)
{
  this->_deallocate_buffer ();
}

template<size_t MAX> ACE_INLINE
TAO_Bounded_WString_Sequence<MAX>::
TAO_Bounded_WString_Sequence (CORBA::ULong length,
                              CORBA::WChar **value,
                              CORBA::Boolean release)
  : TAO_Bounded_Base_Sequence (MAX, length, value, release)
{
}
