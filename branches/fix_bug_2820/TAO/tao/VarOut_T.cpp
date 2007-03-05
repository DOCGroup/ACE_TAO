// $Id$

#ifndef TAO_VAROUT_T_CPP
#define TAO_VAROUT_T_CPP

#include "tao/VarOut_T.h"

#if !defined (__ACE_INLINE__)
#include "tao/VarOut_T.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template<typename T>
TAO_Var_Base_T<T>::TAO_Var_Base_T (const TAO_Var_Base_T<T> & p)
  : ptr_ (p.ptr_ ? new T (*p.ptr_) : 0)
{
}

// *************************************************************

template<typename T>
TAO_Fixed_Var_T<T> &
TAO_Fixed_Var_T<T>::operator= (const TAO_Fixed_Var_T<T> & p)
{
  // Strongly exception safe assignment using copy and non-throwing
  // swap technique.
  TAO_Fixed_Var_T<T> tmp (p);

  T * old_ptr = this->ptr_;
  this->ptr_ = tmp.ptr_;
  tmp.ptr_ = old_ptr;

  return *this;
}

// Fixed-size types only.
template<typename T>
TAO_Fixed_Var_T<T> &
TAO_Fixed_Var_T<T>::operator= (const T & p)
{
  // Strongly exception safe assignment using copy and non-throwing
  // swap technique.
  TAO_Fixed_Var_T<T> tmp (p);

  T * old_ptr = this->ptr_;
  this->ptr_ = tmp.ptr_;
  tmp.ptr_ = old_ptr;

  return *this;
}

// *************************************************************

template<typename T>
TAO_Var_Var_T<T> &
TAO_Var_Var_T<T>::operator= (const TAO_Var_Var_T<T> & p)
{
  // Strongly exception safe assignment using copy and non-throwing
  // swap technique.
  TAO_Var_Var_T<T> tmp (p);

  T * old_ptr = this->ptr_;
  this->ptr_ = tmp.ptr_;
  tmp.ptr_ = old_ptr;

  return *this;
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_VAROUT_T_CPP */
