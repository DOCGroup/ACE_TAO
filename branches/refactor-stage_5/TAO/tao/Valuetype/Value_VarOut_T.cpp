// $Id$

#ifndef TAO_VALUE_VAROUT_T_C
#define TAO_VALUE_VAROUT_T_C

#include "Value_VarOut_T.h"

ACE_RCSID (tao, 
           Value_VarOut_T, 
           "$Id$")

template<typename T>
void
TAO::Value_Traits<T>::tao_add_ref (T * p)
{
  CORBA::add_ref (p);
}

template<typename T>
void
TAO::Value_Traits<T>::tao_remove_ref (T * p)
{
  CORBA::remove_ref (p);
}

// ===============================================================

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
  : ptr_ (ACE_const_cast (T *, p))
{}

template <typename T>
T *
TAO_Value_Var_T<T>::ptr (void) const
{
  return this->ptr_;
}

template <typename T>
TAO_Value_Var_T<T>::TAO_Value_Var_T (
    const TAO_Value_Var_T<T> & p
  )
{
  TAO::Value_Traits<T>::tao_add_ref (p.ptr ());
  this->ptr_ = p.ptr ();
}

template <typename T>
TAO_Value_Var_T<T>::~TAO_Value_Var_T (void)
{
  TAO::Value_Traits<T>::tao_remove_ref (this->ptr_);
}

template <typename T>
TAO_Value_Var_T<T> &
TAO_Value_Var_T<T>::operator= (T * p)
{
  TAO::Value_Traits<T>::tao_remove_ref (this->ptr_);
  this->ptr_ = p;
  return *this;
}

template <typename T>
TAO_Value_Var_T<T> &
TAO_Value_Var_T<T>::operator= (const TAO_Value_Var_T & p)
{
  if (this != &p)
  {
    TAO::Value_Traits<T>::tao_remove_ref (this->ptr_);
    T * tmp = p.ptr ();
    TAO::Value_Traits<T>::tao_add_ref (tmp);
    this->ptr_ = tmp;
  }
  
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
  TAO::Value_Traits<T>::tao_remove_ref (this->ptr_);
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
  TAO::Value_Traits<T>::tao_remove_ref (this->ptr_);
  this->ptr_ = 0;
}

template <typename T>
TAO_Value_Out_T<T>::TAO_Value_Out_T (const TAO_Value_Out_T<T> & p)
  : ptr_ (ACE_const_cast (THIS_OUT_TYPE &, p).ptr_)
{}

template <typename T>
TAO_Value_Out_T<T> &
TAO_Value_Out_T<T>::operator= (const TAO_Value_Out_T<T> & p)
{
  this->ptr_ = ACE_const_cast (THIS_OUT_TYPE &, p).ptr_;
  return *this;
}

template <typename T>
TAO_Value_Out_T<T> &
TAO_Value_Out_T<T>::operator= (const TAO_Value_Var_T<T> & p)
{
  T * tmp = p.ptr ();
  TAO::Value_Traits<T>::tao_add_ref (tmp);
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

#endif /* TAO_VALUE_VAROUT_T_C */
