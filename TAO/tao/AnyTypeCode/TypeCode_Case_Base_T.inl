// -*- C++ -*-
#include "tao/CDR.h"
#include "tao/AnyTypeCode/TypeCode_Traits.h"
#include "ace/Truncate.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template <typename StringType, typename TypeCodeType>
ACE_INLINE
TAO::TypeCode::Case<StringType, TypeCodeType>::Case (
  char const * member_name,
  TypeCodeType member_type)
  : name_ (member_name)
  , type_ (member_type)
{
}

template <typename StringType, typename TypeCodeType>
ACE_INLINE
TAO::TypeCode::Case<StringType, TypeCodeType>::Case (void)
  : name_ ()
  , type_ ()
{
  // Only used when StringType and TypeCodeType are CORBA::String_var
  // and CORBA::TypeCode_var, respectively.
}

template <typename StringType, typename TypeCodeType>
ACE_INLINE bool
TAO::TypeCode::Case<StringType, TypeCodeType>::marshal (
  TAO_OutputCDR & cdr,
  CORBA::ULong offset) const
{
  return
    this->marshal_label (cdr)
    && (cdr << TAO_OutputCDR::from_string (
         Traits<StringType>::get_string (this->name_), 0))
    && TAO::TypeCode::marshal (cdr,
                               Traits<StringType>::get_typecode (this->type_),
                               ACE_Utils::truncate_cast<CORBA::ULong> (
                                   offset + cdr.total_length ()));
}

template <typename StringType, typename TypeCodeType>
ACE_INLINE char const *
TAO::TypeCode::Case<StringType, TypeCodeType>::name (void) const
{
  return
    Traits<StringType>::get_string (this->name_);
}

template <typename StringType, typename TypeCodeType>
ACE_INLINE void
TAO::TypeCode::Case<StringType, TypeCodeType>::name (
  char const * the_name)
{
  this->name_ = the_name;
}

template <typename StringType, typename TypeCodeType>
ACE_INLINE CORBA::TypeCode_ptr
TAO::TypeCode::Case<StringType, TypeCodeType>::type (void) const
{
  return
    Traits<StringType>::get_typecode (this->type_);
}

template <typename StringType, typename TypeCodeType>
ACE_INLINE void
TAO::TypeCode::Case<StringType, TypeCodeType>::type (CORBA::TypeCode_ptr tc)
{
  // This assignment works since it is only used when TypeCodeType is
  // "CORBA::TypeCode_var", not in the "CORBA::TypeCode_ptr const *"
  // case.
  this->type_ = CORBA::TypeCode::_duplicate (tc);
}

TAO_END_VERSIONED_NAMESPACE_DECL
