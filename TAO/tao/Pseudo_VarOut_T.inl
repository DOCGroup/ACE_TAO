// -*- C++ -*-
#include "tao/CORBA_methods.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template <typename T>
ACE_INLINE
TAO_Pseudo_Var_T<T>::TAO_Pseudo_Var_T ()
  : ptr_ (T::_nil ())
{}

template <typename T>
ACE_INLINE
TAO_Pseudo_Var_T<T>::TAO_Pseudo_Var_T (typename T::_ptr_type p)
  : ptr_ (p)
{}

template <typename T>
ACE_INLINE
TAO_Pseudo_Var_T<T>::TAO_Pseudo_Var_T (const TAO_Pseudo_Var_T<T> & p)
  : TAO_Base_var ()
  , ptr_ (T::_duplicate (p.ptr ()))
{}

template <typename T>
ACE_INLINE
TAO_Pseudo_Var_T<T>::~TAO_Pseudo_Var_T ()
{
  CORBA::release (this->ptr_);
}

template <typename T>
ACE_INLINE
TAO_Pseudo_Var_T<T> &
TAO_Pseudo_Var_T<T>::operator= (typename T::_ptr_type p)
{
  CORBA::release (this->ptr_);
  this->ptr_ = p;
  return *this;
}

template <typename T>
ACE_INLINE
TAO_Pseudo_Var_T<T>::operator typename T::_ptr_type const & () const
{
  return this->ptr_;
}

template <typename T>
ACE_INLINE
TAO_Pseudo_Var_T<T>::operator typename T::_ptr_type & ()
{
  return this->ptr_;
}

template <typename T>
ACE_INLINE
typename T::_ptr_type
TAO_Pseudo_Var_T<T>::operator-> () const
{
  return this->ptr_;
}

template <typename T>
ACE_INLINE
typename T::_ptr_type
TAO_Pseudo_Var_T<T>::in () const
{
  return this->ptr_;
}

template <typename T>
ACE_INLINE
typename T::_ptr_type &
TAO_Pseudo_Var_T<T>::inout ()
{
  return this->ptr_;
}

template <typename T>
ACE_INLINE
typename T::_ptr_type &
TAO_Pseudo_Var_T<T>::out ()
{
  CORBA::release (this->ptr_);
  this->ptr_ = T::_nil ();
  return this->ptr_;
}

template <typename T>
ACE_INLINE
typename T::_ptr_type
TAO_Pseudo_Var_T<T>::_retn ()
{
  typename T::_ptr_type val = this->ptr_;
  this->ptr_ = T::_nil ();
  return val;
}

template <typename T>
ACE_INLINE
typename T::_ptr_type
TAO_Pseudo_Var_T<T>::ptr () const
{
  return this->ptr_;
}

// *************************************************************

template <typename T>
ACE_INLINE
TAO_Pseudo_Out_T<T>::TAO_Pseudo_Out_T (typename T::_ptr_type & p)
  : ptr_ (p)
{
  this->ptr_ = T::_nil ();
}

template <typename T>
ACE_INLINE
TAO_Pseudo_Out_T<T>::TAO_Pseudo_Out_T (typename T::_var_type & p)
  : ptr_ (p.out ())
{
  CORBA::release (this->ptr_);
  this->ptr_ = T::_nil ();
}

template <typename T>
ACE_INLINE
TAO_Pseudo_Out_T<T>::TAO_Pseudo_Out_T (
    const TAO_Pseudo_Out_T<T> & p)
  : ptr_ (p.ptr_)
{}

template <typename T>
ACE_INLINE
TAO_Pseudo_Out_T<T> &
TAO_Pseudo_Out_T<T>::operator= (const TAO_Pseudo_Out_T<T> & p)
{
  this->ptr_ = p.ptr_;
  return *this;
}

template <typename T>
ACE_INLINE
TAO_Pseudo_Out_T<T> &
TAO_Pseudo_Out_T<T>::operator= (typename T::_ptr_type p)
{
  this->ptr_ = p;
  return *this;
}

template <typename T>
ACE_INLINE
typename T::_ptr_type &
TAO_Pseudo_Out_T<T>::ptr ()
{
  return this->ptr_;
}

template <typename T>
ACE_INLINE
TAO_Pseudo_Out_T<T>::operator typename T::_ptr_type & ()
{
  return this->ptr_;
}

template <typename T>
ACE_INLINE
typename T::_ptr_type
TAO_Pseudo_Out_T<T>::operator-> ()
{
  return this->ptr_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
