// $Id$
#ifndef TAO_OBJREF_VAROUT_T_C
#define TAO_OBJREF_VAROUT_T_C

#include "tao/Objref_VarOut_T.h"
#include "tao/Environment.h"

#if !defined (__ACE_INLINE__)
#include "tao/Objref_VarOut_T.inl"
#endif /* __ACE_INLINE__ */



ACE_RCSID (tao,
           Objref_VarOut_T,
           "$Id$")

// @@@ (JP) This needs to be here temporarily until we regenerate the
// hand-crafted files.

template<typename T>
T *
TAO::Objref_Traits<T>::tao_duplicate (T * p)
{
  return T::_duplicate (p);
}

template<typename T>
void
TAO::Objref_Traits<T>::tao_release (T * p)
{
  CORBA::release (p);
}

template<typename T>
T *
TAO::Objref_Traits<T>::tao_nil (void)
{
  return T::_nil ();
}

template<typename T>
CORBA::Boolean
TAO::Objref_Traits<T>::tao_marshal (T * p,
                                    TAO_OutputCDR & cdr)
{
  return p->marshal (cdr);
}

// =================================================================

template <typename T, typename T_life>
TAO_Objref_Var_T<T, T_life>::TAO_Objref_Var_T (void)
  : ptr_ (TAO::Objref_Traits<T>::tao_nil ())
{
}

template <typename T, typename T_life>
TAO_Objref_Var_T<T, T_life>::TAO_Objref_Var_T (
    const TAO_Objref_Var_T<T, T_life> & p
  )
  : TAO_Base_var (),
    ptr_ (TAO::Objref_Traits<T>::tao_duplicate (p.ptr ()))
{
}

template <typename T, typename T_life>
TAO_Objref_Var_T<T, T_life>::~TAO_Objref_Var_T (void)
{
  TAO::Objref_Traits<T>::tao_release (this->ptr_);
}

template <typename T, typename T_life>
TAO_Objref_Var_T<T, T_life> &
TAO_Objref_Var_T<T, T_life>::operator= (T * p)
{
  TAO::Objref_Traits<T>::tao_release (this->ptr_);
  this->ptr_ = p;
  return *this;
}

template <typename T, typename T_life>
TAO_Objref_Var_T<T, T_life> &
TAO_Objref_Var_T<T, T_life>::operator= (
    const TAO_Objref_Var_T<T, T_life> & p
  )
{
  if (this != &p)
    {
      TAO::Objref_Traits<T>::tao_release (this->ptr_);
      this->ptr_ = TAO::Objref_Traits<T>::tao_duplicate (p.ptr ());
    }

  return *this;
}

template <typename T, typename T_life>
T *
TAO_Objref_Var_T<T, T_life>::operator-> (void) const
{
  return this->ptr_;
}

template <typename T, typename T_life>
TAO_Objref_Var_T<T, T_life>::operator T * const & () const
{
  return this->ptr_;
}

template <typename T, typename T_life>
TAO_Objref_Var_T<T, T_life>::operator T *& ()
{
  return this->ptr_;
}

template <typename T, typename T_life>
T *
TAO_Objref_Var_T<T, T_life>::in (void) const
{
  return this->ptr_;
}

template <typename T, typename T_life>
T *&
TAO_Objref_Var_T<T, T_life>::inout (void)
{
  return this->ptr_;
}

template <typename T, typename T_life>
T *&
TAO_Objref_Var_T<T, T_life>::out (void)
{
  TAO::Objref_Traits<T>::tao_release (this->ptr_);
  this->ptr_ = TAO::Objref_Traits<T>::tao_nil ();
  return this->ptr_;
}

template <typename T, typename T_life>
T *
TAO_Objref_Var_T<T, T_life>::_retn (void)
{
  T * val = this->ptr_;
  this->ptr_ = TAO::Objref_Traits<T>::tao_nil ();
  return val;
}

template <typename T, typename T_life>
T *
TAO_Objref_Var_T<T, T_life>::ptr (void) const
{
  return this->ptr_;
}

template <typename T, typename T_life>
void
TAO_Objref_Var_T<T, T_life>::free (void)
{
  TAO::Objref_Traits<T>::tao_release (this->ptr_);
}

template <typename T, typename T_life>
void
TAO_Objref_Var_T<T, T_life>::reset (T * p)
{
  TAO::Objref_Traits<T>::tao_release (this->ptr_);
  this->ptr_ = p;
}

// ******************************************************

template <typename T, typename T_life>
TAO_Objref_Out_T<T, T_life>::TAO_Objref_Out_T (T *& p)
  : ptr_ (p)
{
  this->ptr_ = T_life::tao_nil ();
}

template <typename T, typename T_life>
TAO_Objref_Out_T<T, T_life>::TAO_Objref_Out_T (
    TAO_Objref_Var_T<T, T_life> & p
  )
  : ptr_ (p.out ())
{
  TAO::Objref_Traits<T>::tao_release (this->ptr_);
  this->ptr_ = TAO::Objref_Traits<T>::tao_nil ();
}

template <typename T, typename T_life>
TAO_Objref_Out_T<T, T_life>::TAO_Objref_Out_T (
    const TAO_Objref_Out_T<T, T_life> & p
  )
  : ptr_ (p.ptr_)
{
}

template <typename T, typename T_life>
TAO_Objref_Out_T<T, T_life> &
TAO_Objref_Out_T<T, T_life>::operator= (
    const TAO_Objref_Out_T<T, T_life> & p
  )
{
  this->ptr_ = p.ptr_;
  return *this;
}

template <typename T, typename T_life>
TAO_Objref_Out_T<T, T_life> &
TAO_Objref_Out_T<T, T_life>::operator= (
    const TAO_Objref_Var_T<T, T_life> & p
  )
{
  this->ptr_ = TAO::Objref_Traits<T>::tao_duplicate (p.ptr ());
  return *this;
}

template <typename T, typename T_life>
TAO_Objref_Out_T<T, T_life> &
TAO_Objref_Out_T<T, T_life>::operator= (T * p)
{
  this->ptr_ = p;
  return *this;
}

template <typename T, typename T_life>
TAO_Objref_Out_T<T, T_life>::operator T *& ()
{
  return this->ptr_;
}

template <typename T, typename T_life>
T *&
TAO_Objref_Out_T<T, T_life>::ptr (void)
{
  return this->ptr_;
}

template <typename T, typename T_life>
T *
TAO_Objref_Out_T<T, T_life>::operator-> (void)
{
  return this->ptr_;
}

#endif /* TAO_OBJREF_VAROUT_T_C */
