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

template <typename T, typename T_life>
TAO_Value_Var_T<T,T_life>::TAO_Value_Var_T (void)
  : ptr_ (0)
{}

template <typename T, typename T_life>
TAO_Value_Var_T<T,T_life>::TAO_Value_Var_T (T * p)
  : ptr_ (p)
{}

template <typename T, typename T_life>
TAO_Value_Var_T<T,T_life>::TAO_Value_Var_T (const T * p)
  : ptr_ (ACE_const_cast (T *, p))
{}

template <typename T, typename T_life>
T *
TAO_Value_Var_T<T,T_life>::ptr (void) const
{
  return this->ptr_;
}

template <typename T, typename T_life>
TAO_Value_Var_T<T,T_life>::TAO_Value_Var_T (
    const TAO_Value_Var_T<T,T_life> & p
  )
{
  TAO::Value_Traits<T>::tao_add_ref (p.ptr ());
  this->ptr_ = p.ptr ();
}

template <typename T, typename T_life>
TAO_Value_Var_T<T,T_life>::~TAO_Value_Var_T (void)
{
  TAO::Value_Traits<T>::tao_remove_ref (this->ptr_);
}

template <typename T, typename T_life>
TAO_Value_Var_T<T,T_life> &
TAO_Value_Var_T<T,T_life>::operator= (T * p)
{
  TAO::Value_Traits<T>::tao_remove_ref (this->ptr_);
  this->ptr_ = p;
  return *this;
}

template <typename T, typename T_life>
TAO_Value_Var_T<T,T_life> &
TAO_Value_Var_T<T,T_life>::operator= (const TAO_Value_Var_T & p)
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

template <typename T, typename T_life>
TAO_Value_Var_T<T,T_life>::operator const T * () const
{
  return this->ptr_;
}

template <typename T, typename T_life>
TAO_Value_Var_T<T,T_life>::operator T *& ()
{
  return this->ptr_;
}

template <typename T, typename T_life>
T *
TAO_Value_Var_T<T,T_life>::operator-> (void) const
{
  return this->ptr_;
}

template <typename T, typename T_life>
T *
TAO_Value_Var_T<T,T_life>::in (void) const
{
  return this->ptr_;
}

template <typename T, typename T_life>
T *&
TAO_Value_Var_T<T,T_life>::inout (void)
{
  return this->ptr_;
}

template <typename T, typename T_life>
T *&
TAO_Value_Var_T<T,T_life>::out (void)
{
  TAO::Value_Traits<T>::tao_remove_ref (this->ptr_);
  this->ptr_ = 0;
  return this->ptr_;
}

template <typename T, typename T_life>
T *
TAO_Value_Var_T<T,T_life>::_retn (void)
{
  T * tmp = this->ptr_;
  this->ptr_ = 0;
  return tmp;
}

// *************************************************************

template <typename T, typename T_life>
TAO_Value_Out_T<T,T_life>::TAO_Value_Out_T (T *& p)
  : ptr_ (p)
{
  this->ptr_ = 0;
}

template <typename T, typename T_life>
TAO_Value_Out_T<T,T_life>::TAO_Value_Out_T (TAO_Value_Var_T<T,T_life> & p)
  : ptr_ (p.out ())
{
  TAO::Value_Traits<T>::tao_remove_ref (this->ptr_);
  this->ptr_ = 0;
}

template <typename T, typename T_life>
TAO_Value_Out_T<T,T_life>::TAO_Value_Out_T (const TAO_Value_Out_T<T,T_life> & p)
  : ptr_ (ACE_const_cast (THIS_OUT_TYPE &, p).ptr_)
{}

template <typename T, typename T_life>
TAO_Value_Out_T<T,T_life> &
TAO_Value_Out_T<T,T_life>::operator= (const TAO_Value_Out_T<T,T_life> & p)
{
  this->ptr_ = ACE_const_cast (THIS_OUT_TYPE &, p).ptr_;
  return *this;
}

template <typename T, typename T_life>
TAO_Value_Out_T<T,T_life> &
TAO_Value_Out_T<T,T_life>::operator= (const TAO_Value_Var_T<T,T_life> & p)
{
  T * tmp = p.ptr ();
  TAO::Value_Traits<T>::tao_add_ref (tmp);
  this->ptr_ = tmp;
  return *this;
}

template <typename T, typename T_life>
TAO_Value_Out_T<T,T_life> &
TAO_Value_Out_T<T,T_life>::operator= (T * p)
{
  this->ptr_ = p;
  return *this;
}

template <typename T, typename T_life>
TAO_Value_Out_T<T,T_life>::operator T *& ()
{
  return this->ptr_;
}

template <typename T, typename T_life>
T *&
TAO_Value_Out_T<T,T_life>::ptr (void)
{
  return this->ptr_;
}

template <typename T, typename T_life>
T *
TAO_Value_Out_T<T,T_life>::operator-> (void)
{
  return this->ptr_;
}

#endif /* TAO_VALUE_VAROUT_T_C */
