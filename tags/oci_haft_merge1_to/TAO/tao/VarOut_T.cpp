// $Id$

#ifndef TAO_VAROUT_T_C
#define TAO_VAROUT_T_C

#include "tao/VarOut_T.h"

#if !defined (__ACE_INLINE__)
#include "tao/VarOut_T.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID (tao, 
           VarOut_T, 
           "$Id$")

template<typename T>
TAO_Var_Base_T<T>::TAO_Var_Base_T (const TAO_Var_Base_T<T> & p)
{
  if (p.ptr_)
    {
      ACE_NEW (this->ptr_, 
               T (*p.ptr_));
    }
  else
    {
      this->ptr_ = 0;
    }
}

// *************************************************************

template<typename T>
TAO_Fixed_Var_T<T> &
TAO_Fixed_Var_T<T>::operator= (const TAO_Fixed_Var_T<T> & p)
{
  if (this != &p)
    {
      if (p.ptr_ == 0)
        {
          delete this->ptr_;
          this->ptr_ = 0;
        }
      else
        {
          T * deep_copy = 0;
          ACE_NEW_RETURN (deep_copy,
                          T (*p.ptr_),
                          *this);
          
          if (deep_copy != 0)
            {
              T * tmp = deep_copy;
              deep_copy = this->ptr_;
              this->ptr_ = tmp;
              delete deep_copy;
            }
        }
    }
  
  return *this;
}

// Fixed-size types only.
template<typename T>
TAO_Fixed_Var_T<T> &
TAO_Fixed_Var_T<T>::operator= (const T & p)
{
  if (this->ptr_ != &p)
  {
    delete this->ptr_;
    ACE_NEW_RETURN (this->ptr_,
                    T (p),
                    *this);
  }
  
  return *this;
}

// *************************************************************

template<typename T>
TAO_Var_Var_T<T> &
TAO_Var_Var_T<T>::operator= (const TAO_Var_Var_T<T> & p)
{
  if (this != &p)
    {
      if (p.ptr_ == 0)
        {
          delete this->ptr_;
          this->ptr_ = 0;
        }
      else
        {
          T *deep_copy = 0;
          ACE_NEW_RETURN (deep_copy,
                          T (*p.ptr_),
                          *this);
          
          if (deep_copy != 0)
            {
              T * tmp = deep_copy;
              deep_copy = this->ptr_;
              this->ptr_ = tmp;
              delete deep_copy;
            }
        }
    }
  
  return *this;
}

#endif /* TAO_VAROUT_T_C */
