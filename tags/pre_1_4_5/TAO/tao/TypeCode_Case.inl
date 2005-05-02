// -*- C++ -*-
//
// $Id$

#include "tao/CDR.h"


template <typename StringType, typename TypeCodeType>
ACE_INLINE
TAO::TypeCode::Case<StringType, TypeCodeType>::Case (
  char const * member_name,
  TypeCodeType const & member_type)
  : name_ (member_name)
  , type_ (member_type)
{
}

template <typename StringType, typename TypeCodeType>
ACE_INLINE bool
TAO::TypeCode::Case<StringType, TypeCodeType>::marshal (
  TAO_OutputCDR & cdr) const
{
  return
    this->marshal_label (cdr)
    && (cdr << TAO_OutputCDR::from_string (
         Traits<StringType>::get_string (this->name_), 0))
    && (cdr << Traits<StringType>::get_typecode (this->type_));
}

template <typename StringType, typename TypeCodeType>
ACE_INLINE char const *
TAO::TypeCode::Case<StringType, TypeCodeType>::name (void) const
{
  return
    Traits<StringType>::get_string (this->name_);
}

template <typename StringType, typename TypeCodeType>
ACE_INLINE CORBA::TypeCode_ptr
TAO::TypeCode::Case<StringType, TypeCodeType>::type (void) const
{
  return
    Traits<StringType>::get_typecode (this->type_);
}
