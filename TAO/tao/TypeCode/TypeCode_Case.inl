// -*- C++ -*-
//
// $Id$

#include "tao/CDR.h"


template <typename STRING_TYPE>
ACE_INLINE char const *
TAO::TypeCode::Case<STRING_TYPE>::Case (
  char const * member_name,
  CORBA::TypeCode_ptr * member_type)
  : name_ (member_name)
  , type_ (member_type)
{
}

template <typename STRING_TYPE>
ACE_INLINE char const *
TAO::TypeCode::Case<STRING_TYPE>::name (void) const
{
  return this->name_;
}

template <typename STRING_TYPE>
ACE_INLINE CORBA::TypeCode_ptr
TAO::TypeCode::Case<STRING_TYPE>::type (void) const
{
  return *this->type_;
}

template <typename STRING_TYPE>
ACE_INLINE bool
TAO::TypeCode::Case<STRING_TYPE>::marshal (
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
TAO::TypeCode::Case<CORBA::String_var>::name (void) const
{
  return this->name_.in ();
}
