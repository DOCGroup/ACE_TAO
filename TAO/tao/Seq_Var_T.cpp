// $Id$

#ifndef TAO_SEQ_VAR_T_C
#define TAO_SEQ_VAR_T_C

#include "tao/Seq_Var_T.h"

#if !defined (__ACE_INLINE__)
#include "tao/Seq_Var_T.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID (tao,
           Seq_VarOut_T,
           "$Id$")

template<typename T, typename T_elem>
TAO_Seq_Var_Base_T<T,T_elem>::TAO_Seq_Var_Base_T (
    const TAO_Seq_Var_Base_T<T,T_elem> & p
  )
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

// ****************************************************************************

template<typename T, typename T_elem>
TAO_FixedSeq_Var_T<T,T_elem> &
TAO_FixedSeq_Var_T<T,T_elem>::operator= (
    const TAO_FixedSeq_Var_T<T,T_elem> & p
  )
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
          ACE_NEW_RETURN (
              deep_copy,
              T (*p.ptr_),
              *this
            );

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
template<typename T, typename T_elem>
TAO_FixedSeq_Var_T<T,T_elem> &
TAO_FixedSeq_Var_T<T,T_elem>::operator= (const T & p)
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

// ****************************************************************************

template<typename T, typename T_elem>
TAO_VarSeq_Var_T<T,T_elem> &
TAO_VarSeq_Var_T<T,T_elem>::operator= (
    const TAO_VarSeq_Var_T<T,T_elem> & p
  )
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
          ACE_NEW_RETURN (
              deep_copy,
              T (*p.ptr_),
              *this
            );

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

// ****************************************************************************

template<typename T, typename T_elem>
TAO_MngSeq_Var_T<T,T_elem> &
TAO_MngSeq_Var_T<T,T_elem>::operator= (
    const TAO_MngSeq_Var_T<T,T_elem> & p
  )
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
          ACE_NEW_RETURN (
              deep_copy,
              T (*p.ptr_),
              *this
            );

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

#endif /* TAO_SEQ_VAR_T_C */
