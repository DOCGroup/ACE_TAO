// This may look like C, but it's really -*- C++ -*-
// $Id$

// *************************************************************
// Inline operations for class TAO_Valuetype_Manager
// *************************************************************

template <typename T, typename T_var> 
ACE_INLINE
TAO_Valuetype_Manager<T,T_var>::~TAO_Valuetype_Manager (void)
{
}

template <typename T, typename T_var> 
ACE_INLINE
TAO_Valuetype_Manager<T,T_var>::TAO_Valuetype_Manager (
    const TAO_Valuetype_Manager<T,T_var> & rhs
  )
  : ptr_ (rhs.ptr_),
    release_ (rhs.release_)
{
}

template <typename T, typename T_var> 
ACE_INLINE
TAO_Valuetype_Manager<T,T_var>::TAO_Valuetype_Manager(
    T ** buffer, 
    CORBA::Boolean release
  )
  : ptr_ (buffer),
    release_ (release)
{
}

template <typename T, typename T_var> 
ACE_INLINE 
T *
TAO_Valuetype_Manager<T,T_var>::operator-> (void) const
{
  return *this->ptr_;
}

template <typename T, typename T_var> 
ACE_INLINE
TAO_Valuetype_Manager<T,T_var>::operator const T_var () const
{
  TAO::Value_Traits<T>::tao_add_ref (*this->ptr_);
  return *this->ptr_;
}

template <typename T, typename T_var> 
ACE_INLINE
TAO_Valuetype_Manager<T,T_var>::operator const T * () const
{
  return *this->ptr_;
}

template <typename T, typename T_var> 
ACE_INLINE
TAO_Valuetype_Manager<T,T_var>::operator T *& ()
{
  return *this->ptr_;
}

template <typename T, typename T_var> 
ACE_INLINE 
T *
TAO_Valuetype_Manager<T,T_var>::in (void) const
{
  return *this->ptr_;
}

template <typename T, typename T_var> 
ACE_INLINE 
T *&
TAO_Valuetype_Manager<T,T_var>::inout (void)
{
  return *this->ptr_;
}

template <typename T, typename T_var> 
ACE_INLINE 
T *&
TAO_Valuetype_Manager<T,T_var>::out (void)
{
  TAO::Value_Traits<T>::tao_remove_ref (*this->ptr_);
  *this->ptr_ = 0;
  return *this->ptr_;
}

template <typename T, typename T_var> 
ACE_INLINE 
T *
TAO_Valuetype_Manager<T,T_var>::_retn (void)
{
  T * temp = *this->ptr_;
  *this->ptr_ = 0;
  return temp;
}

// *************************************************************
// Inline operations for class TAO_Abstract_Manager
// *************************************************************

template<typename T, typename T_var>
ACE_INLINE
TAO_Abstract_Manager<T,T_var>::~TAO_Abstract_Manager (void)
{
}

template<typename T, typename T_var>
ACE_INLINE
TAO_Abstract_Manager<T,T_var>::TAO_Abstract_Manager (
    const TAO_Abstract_Manager<T,T_var> & rhs
  )
  : ptr_ (rhs.ptr_),
    release_ (rhs.release_)
{
}

template<typename T, typename T_var>
ACE_INLINE
TAO_Abstract_Manager<T,T_var>::TAO_Abstract_Manager(
    T ** buffer, 
    CORBA::Boolean release
  )
  : ptr_ (buffer),
    release_ (release)
{
}

template<typename T, typename T_var>
ACE_INLINE 
T *
TAO_Abstract_Manager<T,T_var>::operator-> (void) const
{
  return *this->ptr_;
}

template<typename T, typename T_var>
ACE_INLINE
TAO_Abstract_Manager<T,T_var>::operator const T * () const
{
  return *this->ptr_;
}

template<typename T, typename T_var>
ACE_INLINE
TAO_Abstract_Manager<T,T_var>::operator T *& ()
{
  return *this->ptr_;
}

template<typename T, typename T_var>
ACE_INLINE 
T *
TAO_Abstract_Manager<T,T_var>::in (void) const
{
  return *this->ptr_;
}

template<typename T, typename T_var>
ACE_INLINE 
T *&
TAO_Abstract_Manager<T,T_var>::inout (void)
{
  return *this->ptr_;
}

template<typename T, typename T_var>
ACE_INLINE 
T *
TAO_Abstract_Manager<T,T_var>::_retn (void)
{
  T * temp = *this->ptr_;
  *this->ptr_ = 0;
  return temp;
}

// *************************************************************
// class TAO_Unbounded_Valuetype_Sequence
// *************************************************************

template <typename T, typename T_var> 
ACE_INLINE
TAO_Unbounded_Valuetype_Sequence<T,T_var>::TAO_Unbounded_Valuetype_Sequence (
    void
  )
{
}

template <typename T, typename T_var> 
ACE_INLINE
TAO_Unbounded_Valuetype_Sequence<T,T_var>::TAO_Unbounded_Valuetype_Sequence (
    CORBA::ULong maximum,
    CORBA::ULong length,
    T ** value,
    CORBA::Boolean release
  )
  : TAO_Unbounded_Base_Sequence (maximum, length, value, release)
{
}

template <typename T, typename T_var> 
ACE_INLINE 
TAO_Valuetype_Manager<T,T_var>
TAO_Unbounded_Valuetype_Sequence<T,T_var>::operator[] (
    CORBA::ULong slot
  ) const
{
  ACE_ASSERT (slot < this->maximum_);
  T ** const tmp = ACE_reinterpret_cast (T ** ACE_CAST_CONST, this->buffer_);
  return TAO_Valuetype_Manager<T,T_var> (tmp + slot, this->release_);
}

// *************************************************************
// class TAO_Bounded_Valuetype_Sequence
// *************************************************************

template<typename T, typename T_var, size_t MAX> 
ACE_INLINE
TAO_Bounded_Valuetype_Sequence<T,T_var,MAX>::~TAO_Bounded_Valuetype_Sequence (
    void
  )
{
  this->_deallocate_buffer ();
}

template<typename T, typename T_var, size_t MAX> 
ACE_INLINE
TAO_Bounded_Valuetype_Sequence<T,T_var,MAX>::TAO_Bounded_Valuetype_Sequence (
    CORBA::ULong length,
    T **value,
    CORBA::Boolean release
  )
  : TAO_Bounded_Base_Sequence (MAX, length, value, release)
{
}

template<typename T, typename T_var, size_t MAX> 
ACE_INLINE 
TAO_Valuetype_Manager<T,T_var>
TAO_Bounded_Valuetype_Sequence<T,T_var,MAX>::operator[] (
    CORBA::ULong slot
  ) const
{
  ACE_ASSERT (slot < this->maximum_);
  T ** const tmp = ACE_reinterpret_cast (T ** ACE_CAST_CONST, this->buffer_);
  return TAO_Valuetype_Manager<T,T_var> (tmp + slot, this->release_);
}

// *************************************************************
// class TAO_Unbounded_Abstract_Sequence
// *************************************************************

template <typename T, typename T_var> 
ACE_INLINE
TAO_Unbounded_Abstract_Sequence<T,T_var>::TAO_Unbounded_Abstract_Sequence (
    void
  )
{
}

template <typename T, typename T_var> 
ACE_INLINE
TAO_Unbounded_Abstract_Sequence<T,T_var>::TAO_Unbounded_Abstract_Sequence (
    CORBA::ULong maximum,
    CORBA::ULong length,
    T ** value,
    CORBA::Boolean release
  )
  : TAO_Unbounded_Base_Sequence (maximum, length, value, release)
{
}

template <typename T, typename T_var> 
ACE_INLINE 
TAO_Abstract_Manager<T,T_var>
TAO_Unbounded_Abstract_Sequence<T,T_var>::operator[] (
    CORBA::ULong slot
  ) const
{
  ACE_ASSERT (slot < this->maximum_);
  T ** const tmp = ACE_reinterpret_cast (T ** ACE_CAST_CONST, this->buffer_);
  return TAO_Abstract_Manager<T,T_var> (tmp + slot, this->release_);
}

// *************************************************************
// class TAO_Bounded_Abstract_Sequence
// *************************************************************

template<typename T, typename T_var, size_t MAX>
ACE_INLINE
TAO_Bounded_Abstract_Sequence<T,T_var,MAX>::~TAO_Bounded_Abstract_Sequence (
    void
  )
{
  this->_deallocate_buffer ();
}

template<typename T, typename T_var, size_t MAX>
ACE_INLINE
TAO_Bounded_Abstract_Sequence<T,T_var,MAX>::TAO_Bounded_Abstract_Sequence (
    CORBA::ULong length,
    T ** value,
    CORBA::Boolean release
  )
  : TAO_Bounded_Base_Sequence (MAX, length, value, release)
{
}

template<typename T, typename T_var, size_t MAX>
ACE_INLINE 
TAO_Abstract_Manager<T,T_var>
TAO_Bounded_Abstract_Sequence<T,T_var,MAX>::operator[] (
    CORBA::ULong slot
  ) const
{
  ACE_ASSERT (slot < this->maximum_);
  T ** const tmp = ACE_reinterpret_cast (T ** ACE_CAST_CONST, this->buffer_);
  return TAO_Abstract_Manager<T,T_var> (tmp + slot, this->release_);
}
