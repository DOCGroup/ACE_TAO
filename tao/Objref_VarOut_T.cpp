// $Id$
#ifndef TAO_OBJREF_VAROUT_T_CPP
#define TAO_OBJREF_VAROUT_T_CPP

#include "tao/Objref_VarOut_T.h"
#include "tao/Environment.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template <typename T>
TAO_Objref_Var_T<T>::TAO_Objref_Var_T (void)
  : ptr_ (TAO::Objref_Traits<T>::nil ())
{
}

template <typename T>
TAO_Objref_Var_T<T>::TAO_Objref_Var_T (
    const TAO_Objref_Var_T<T> & p
  )
  : TAO_Base_var (),
    ptr_ (TAO::Objref_Traits<T>::duplicate (p.ptr ()))
{
}

template <typename T>
TAO_Objref_Var_T<T>::~TAO_Objref_Var_T (void)
{
  TAO::Objref_Traits<T>::release (this->ptr_);
}

template <typename T>
TAO_Objref_Var_T<T> &
TAO_Objref_Var_T<T>::operator= (T * p)
{
  TAO::Objref_Traits<T>::release (this->ptr_);
  this->ptr_ = p;
  return *this;
}

template <typename T>
TAO_Objref_Var_T<T> &
TAO_Objref_Var_T<T>::operator= (
    const TAO_Objref_Var_T<T> & p
  )
{
  if (this != &p)
    {
      TAO::Objref_Traits<T>::release (this->ptr_);
      this->ptr_ = TAO::Objref_Traits<T>::duplicate (p.ptr ());
    }

  return *this;
}

template <typename T>
T *
TAO_Objref_Var_T<T>::operator-> (void) const
{
  return this->ptr_;
}

template <typename T>
TAO_Objref_Var_T<T>::operator T * const & () const
{
  return this->ptr_;
}

template <typename T>
TAO_Objref_Var_T<T>::operator T *& ()
{
  return this->ptr_;
}

template <typename T>
T *
TAO_Objref_Var_T<T>::in (void) const
{
  return this->ptr_;
}

template <typename T>
T *&
TAO_Objref_Var_T<T>::inout (void)
{
  return this->ptr_;
}

template <typename T>
T *&
TAO_Objref_Var_T<T>::out (void)
{
  TAO::Objref_Traits<T>::release (this->ptr_);
  this->ptr_ = TAO::Objref_Traits<T>::nil ();
  return this->ptr_;
}

template <typename T>
T *
TAO_Objref_Var_T<T>::_retn (void)
{
  T * val = this->ptr_;
  this->ptr_ = TAO::Objref_Traits<T>::nil ();
  return val;
}

template <typename T>
T *
TAO_Objref_Var_T<T>::ptr (void) const
{
  return this->ptr_;
}

template <typename T>
void
TAO_Objref_Var_T<T>::free (void)
{
  TAO::Objref_Traits<T>::release (this->ptr_);
}

template <typename T>
void
TAO_Objref_Var_T<T>::reset (T * p)
{
  TAO::Objref_Traits<T>::release (this->ptr_);
  this->ptr_ = p;
}

// ******************************************************

template <typename T>
TAO_Objref_Out_T<T>::TAO_Objref_Out_T (T *& p)
  : ptr_ (p)
{
  this->ptr_ = TAO::Objref_Traits<T>::nil ();
}

template <typename T>
TAO_Objref_Out_T<T>::TAO_Objref_Out_T (
    TAO_Objref_Var_T<T> & p
  )
  : ptr_ (p.out ())
{
  TAO::Objref_Traits<T>::release (this->ptr_);
  this->ptr_ = TAO::Objref_Traits<T>::nil ();
}

template <typename T>
TAO_Objref_Out_T<T>::TAO_Objref_Out_T (
    const TAO_Objref_Out_T<T> & p
  )
  : ptr_ (p.ptr_)
{
}

template <typename T>
TAO_Objref_Out_T<T> &
TAO_Objref_Out_T<T>::operator= (
    const TAO_Objref_Out_T<T> & p
  )
{
  this->ptr_ = p.ptr_;
  return *this;
}

template <typename T>
TAO_Objref_Out_T<T> &
TAO_Objref_Out_T<T>::operator= (T * p)
{
  this->ptr_ = p;
  return *this;
}

template <typename T>
T *&
TAO_Objref_Out_T<T>::ptr (void)
{
  return this->ptr_;
}

template <typename T>
TAO_Objref_Out_T<T>::operator T *& ()
{
  return this->ptr_;
}

template <typename T>
T *
TAO_Objref_Out_T<T>::operator-> (void)
{
  return this->ptr_;
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_OBJREF_VAROUT_T_CPP */
