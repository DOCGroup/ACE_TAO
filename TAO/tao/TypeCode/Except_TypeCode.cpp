// $Id$

#ifndef TAO_EXCEPT_TYPECODE_CPP
#define TAO_EXCEPT_TYPECODE_CPP

#include "tao/Except_TypeCode.h"

#ifndef __ACE_INLINE__
# include "tao/Except_TypeCode.inl"
#endif  /* !__ACE_INLINE__ */


template <class AttrType, class RefCountPolicy>
CORBA::TCKind
TAO::TypeCode::Except<AttrType, RefCountPolicy>::kind_i (
  ACE_ENV_SINGLE_ARG_DECL_NOT_USED) const
{
  return CORBA::tk_except;
}

#endif  /* TAO_EXCEPT_TYPECODE_CPP */
