// -*- C++ -*-
//
// $Id$

#include "tao/CDR.h"


template <typename STRING_TYPE>
ACE_INLINE char const *
TAO::TypeCode::Case_Base<STRING_TYPE>::Case_Base (
  char const * member_name,
  CORBA::TypeCode_ptr * member_type)
  : name_ (member_name)
  , type_ (member_type)
{
}

template <typename STRING_TYPE>
ACE_INLINE char const *
TAO::TypeCode::Case_Base<STRING_TYPE>::name (void) const
{
  return this->name_;
}

template <typename STRING_TYPE>
ACE_INLINE CORBA::TypeCode_ptr
TAO::TypeCode::Case_Base<STRING_TYPE>::type (void) const
{
  return *this->type_;
}

template <typename DISCRIMINATOR, typename STRING_TYPE>
ACE_INLINE bool
TAO::TypeCode::Case<DISCRIMINATOR, STRING_TYPE>::marshal (
  TAO_OutputCDR & cdr) const
{
  return
    this->marshal_label ()
    && (cdr << this->name ())
    && (cdr << this->type ());
}


// -----------------------------------------------------------------
// Some compilers exhibit warnings about better conversion sequence
// from a CORBA::String_var to a char const *.  This member
// specialization works around them by performing explicit
// conversions.
// -----------------------------------------------------------------

ACE_INLINE char const *
TAO::TypeCode::Case_Base<CORBA::String_var>::name (void) const
{
  return this->name_.in ();
}
