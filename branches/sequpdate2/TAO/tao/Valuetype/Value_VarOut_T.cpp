// $Id$

#ifndef TAO_VALUE_VAROUT_T_CPP
#define TAO_VALUE_VAROUT_T_CPP

#include "tao/Valuetype/Value_VarOut_T.h"
#include "tao/Valuetype/Value_CORBA_methods.h"

#include <algorithm>  /* For std::swap<>() */

template <typename T>
TAO_Value_Var_T<T>::TAO_Value_Var_T (void)
  : ptr_ (0)
{}

template <typename T>
TAO_Value_Var_T<T>::TAO_Value_Var_T (T * p)
  : ptr_ (p)
{}

template <typename T>
TAO_Value_Var_T<T>::TAO_Value_Var_T (const T * p)
  : ptr_ (const_cast<T *> (p))
{}

template <typename T>
T *
TAO_Value_Var_T<T>::ptr (void) const
{
  return this->ptr_;
}

template <typename T>
TAO_Value_Var_T<T>::TAO_Value_Var_T (const TAO_Value_Var_T<T> & p)
  : TAO_Base_var ()
{
  TAO::Value_Traits<T>::add_ref (p.ptr ());
  this->ptr_ = p.ptr ();
}

template <typename T>
TAO_Value_Var_T<T>::~TAO_Value_Var_T (void)
{
  TAO::Value_Traits<T>::remove_ref (this->ptr_);
}

template <typename T>
TAO_Value_Var_T<T> &
TAO_Value_Var_T<T>::operator= (T * p)
{
  TAO_Value_Var_T<T> tmp (p);
  std::swap (this->ptr_, tmp.ptr_);

  return *this;
}

template <typename T>
TAO_Value_Var_T<T> &
TAO_Value_Var_T<T>::operator= (const TAO_Value_Var_T<T> & p)
{
  TAO_Value_Var_T<T> tmp (p);
  std::swap (this->ptr_, tmp.ptr_);

  return *this;
}

template <typename T>
TAO_Value_Var_T<T>::operator const T * () const
{
  return this->ptr_;
}

template <typename T>
TAO_Value_Var_T<T>::operator T *& ()
{
  return this->ptr_;
}

template <typename T>
T *
TAO_Value_Var_T<T>::operator-> (void) const
{
  return this->ptr_;
}

template <typename T>
T *
TAO_Value_Var_T<T>::in (void) const
{
  return this->ptr_;
}

template <typename T>
T *&
TAO_Value_Var_T<T>::inout (void)
{
  return this->ptr_;
}

template <typename T>
T *&
TAO_Value_Var_T<T>::out (void)
{
  TAO::Value_Traits<T>::remove_ref (this->ptr_);
  this->ptr_ = 0;
  return this->ptr_;
}

template <typename T>
T *
TAO_Value_Var_T<T>::_retn (void)
{
  T * tmp = this->ptr_;
  this->ptr_ = 0;
  return tmp;
}

// *************************************************************

template <typename T>
TAO_Value_Out_T<T>::TAO_Value_Out_T (T *& p)
  : ptr_ (p)
{
  this->ptr_ = 0;
}

template <typename T>
TAO_Value_Out_T<T>::TAO_Value_Out_T (TAO_Value_Var_T<T> & p)
  : ptr_ (p.out ())
{
  TAO::Value_Traits<T>::remove_ref (this->ptr_);
  this->ptr_ = 0;
}

template <typename T>
TAO_Value_Out_T<T>::TAO_Value_Out_T (const TAO_Value_Out_T<T> & p)
  : ptr_ (const_cast<THIS_OUT_TYPE &> (p).ptr_)
{}

template <typename T>
TAO_Value_Out_T<T> &
TAO_Value_Out_T<T>::operator= (const TAO_Value_Out_T<T> & p)
{
  this->ptr_ = const_cast<THIS_OUT_TYPE &> (p).ptr_;
  return *this;
}

template <typename T>
TAO_Value_Out_T<T> &
TAO_Value_Out_T<T>::operator= (const TAO_Value_Var_T<T> & p)
{
  T * tmp = p.ptr ();
  TAO::Value_Traits<T>::add_ref (tmp);
  this->ptr_ = tmp;
  return *this;
}

template <typename T>
TAO_Value_Out_T<T> &
TAO_Value_Out_T<T>::operator= (T * p)
{
  this->ptr_ = p;
  return *this;
}

template <typename T>
TAO_Value_Out_T<T>::operator T *& ()
{
  return this->ptr_;
}

template <typename T>
T *&
TAO_Value_Out_T<T>::ptr (void)
{
  return this->ptr_;
}

template <typename T>
T *
TAO_Value_Out_T<T>::operator-> (void)
{
  return this->ptr_;
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_VALUE_VAROUT_T_CPP */
