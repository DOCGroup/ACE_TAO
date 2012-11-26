// -*- C++ -*-
//
// $Id$

#include "ace/OS_Memory.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template<typename T>
ACE_INLINE
TAO_Seq_Var_Base_T<T>::TAO_Seq_Var_Base_T (void)
  : ptr_ (0)
{}

template<typename T>
ACE_INLINE
TAO_Seq_Var_Base_T<T>::TAO_Seq_Var_Base_T (T * p)
  : ptr_ (p)
{}

template<typename T>
ACE_INLINE
TAO_Seq_Var_Base_T<T>::~TAO_Seq_Var_Base_T (void)
{
  delete this->ptr_;
}

template<typename T>
ACE_INLINE
const T *
TAO_Seq_Var_Base_T<T>::operator-> (void) const
{
  return this->ptr_;
}

template<typename T>
ACE_INLINE
T *
TAO_Seq_Var_Base_T<T>::operator-> (void)
{
  return this->ptr_;
}

template<typename T>
ACE_INLINE
TAO_Seq_Var_Base_T<T>::operator const T & () const
{
  return *this->ptr_;
}

template<typename T>
ACE_INLINE
TAO_Seq_Var_Base_T<T>::operator T & ()
{
  return *this->ptr_;
}

template<typename T>
ACE_INLINE
TAO_Seq_Var_Base_T<T>::operator T & () const
{
  return *this->ptr_;
}

template<typename T>
ACE_INLINE
const T &
TAO_Seq_Var_Base_T<T>::in (void) const
{
  return *this->ptr_;
}

template<typename T>
ACE_INLINE
T &
TAO_Seq_Var_Base_T<T>::inout (void)
{
  return *this->ptr_;
}

template<typename T>
ACE_INLINE
T *&
TAO_Seq_Var_Base_T<T>::out (void)
{
  delete this->ptr_;
  this->ptr_ = 0;
  return this->ptr_;
}

template<typename T>
ACE_INLINE
T *
TAO_Seq_Var_Base_T<T>::_retn (void)
{
  T * tmp = this->ptr_;
  this->ptr_ = 0;
  return tmp;
}

template<typename T>
ACE_INLINE
T *
TAO_Seq_Var_Base_T<T>::ptr (void) const
{
  return this->ptr_;
}

template<typename T>
ACE_INLINE
TAO_Seq_Var_Base_T<T>::operator T *& ()
{
  return this->ptr_;
}

// ***************************************************************

template<typename T>
ACE_INLINE
TAO_FixedSeq_Var_T<T>::TAO_FixedSeq_Var_T (void)
{}

template<typename T>
ACE_INLINE
TAO_FixedSeq_Var_T<T>::TAO_FixedSeq_Var_T (T * p)
  : TAO_Seq_Var_Base_T<T> (p)
{}

template<typename T>
TAO_FixedSeq_Var_T<T>::TAO_FixedSeq_Var_T (
    const TAO_FixedSeq_Var_T<T> & p
  )
  : TAO_Seq_Var_Base_T<T> (p)
{
}

// Fixed-size base types only.
template<typename T>
ACE_INLINE
TAO_FixedSeq_Var_T<T>::TAO_FixedSeq_Var_T (const T & p)
{
  ACE_NEW (this->ptr_,
           T (p));
}

template<typename T>
ACE_INLINE
TAO_FixedSeq_Var_T<T> &
TAO_FixedSeq_Var_T<T>::operator= (T * p)
{
  delete this->ptr_;
  this->ptr_ = p;
  return *this;
}

template<typename T>
ACE_INLINE
typename TAO_FixedSeq_Var_T<T>::T_elem
TAO_FixedSeq_Var_T<T>::operator[] (CORBA::ULong index)
{
  return this->ptr_->operator[] (index);
}

template<typename T>
ACE_INLINE
typename TAO_FixedSeq_Var_T<T>::T_const_elem
TAO_FixedSeq_Var_T<T>::operator[] (CORBA::ULong index) const
{
  return this->ptr_->operator[] (index);
}

// ***************************************************************

template<typename T>
ACE_INLINE
TAO_VarSeq_Var_T<T>::TAO_VarSeq_Var_T (void)
{
}

template<typename T>
ACE_INLINE
TAO_VarSeq_Var_T<T>::TAO_VarSeq_Var_T (T * p)
  : TAO_Seq_Var_Base_T<T> (p)
{
}

template<typename T>
TAO_VarSeq_Var_T<T>::TAO_VarSeq_Var_T (
    const TAO_VarSeq_Var_T<T> & p)
  : TAO_Seq_Var_Base_T<T> (p)
{
}

template<typename T>
ACE_INLINE
TAO_VarSeq_Var_T<T> &
TAO_VarSeq_Var_T<T>::operator= (T * p)
{
  delete this->ptr_;
  this->ptr_ = p;
  return *this;
}

template<typename T>
ACE_INLINE
typename TAO_VarSeq_Var_T<T>::T_elem
TAO_VarSeq_Var_T<T>::operator[] (CORBA::ULong index)
{
  return this->ptr_->operator[] (index);
}

template<typename T>
ACE_INLINE
typename TAO_VarSeq_Var_T<T>::T_const_elem
TAO_VarSeq_Var_T<T>::operator[] (CORBA::ULong index) const
{
  return this->ptr_->operator[] (index);
}


TAO_END_VERSIONED_NAMESPACE_DECL
