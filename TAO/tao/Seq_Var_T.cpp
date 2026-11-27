#ifndef TAO_SEQ_VAR_T_CPP
#define TAO_SEQ_VAR_T_CPP

#include "tao/Seq_Var_T.h"

#if !defined (__ACE_INLINE__)
#include "tao/Seq_Var_T.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template<typename T>
TAO_Seq_Var_Base_T<T>::TAO_Seq_Var_Base_T (TAO_Seq_Var_Base_T<T> const & p)
  : ptr_ (p.ptr_ ? new T (*p.ptr_) : nullptr)
{
}

// ****************************************************************************

template<typename T>
TAO_FixedSeq_Var_T<T> &
TAO_FixedSeq_Var_T<T>::operator= (TAO_FixedSeq_Var_T<T> const & p)
{
  // Strongly exception safe assignment using copy and non-throwing
  // swap technique.
  TAO_FixedSeq_Var_T<T> tmp (p);

  T * old_ptr = this->ptr_;
  this->ptr_ = tmp.ptr_;
  tmp.ptr_ = old_ptr;

  return *this;
}

// Fixed-size types only.
template<typename T>
TAO_FixedSeq_Var_T<T> &
TAO_FixedSeq_Var_T<T>::operator= (T const & p)
{
  // Strongly exception safe assignment using copy and non-throwing
  // swap technique.
  TAO_FixedSeq_Var_T<T> tmp (p);

  T * old_ptr = this->ptr_;
  this->ptr_ = tmp.ptr_;
  tmp.ptr_ = old_ptr;

  return *this;
}

// ****************************************************************************

template<typename T>
TAO_VarSeq_Var_T<T> &
TAO_VarSeq_Var_T<T>::operator= (TAO_VarSeq_Var_T<T> const & p)
{
  // Strongly exception safe assignment using copy and non-throwing
  // swap technique.
  TAO_VarSeq_Var_T<T> tmp (p);

  T * old_ptr = this->ptr_;
  this->ptr_ = tmp.ptr_;
  tmp.ptr_ = old_ptr;

  return *this;
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_SEQ_VAR_T_CPP */
