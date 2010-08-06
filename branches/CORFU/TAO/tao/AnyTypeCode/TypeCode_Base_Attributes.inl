// -*- C++ -*-
//
// $Id$

#include "tao/AnyTypeCode/TypeCode_Traits.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template <typename STRING_TYPE>
ACE_INLINE
TAO::TypeCode::Base_Attributes<STRING_TYPE>::Base_Attributes (
  char const * id,
  char const * name)
  : id_ (id),
    name_ (name)
{
}

template <typename STRING_TYPE>
ACE_INLINE
TAO::TypeCode::Base_Attributes<STRING_TYPE>::Base_Attributes (
  char const * id)
  : id_ (id),
    name_ ()
{
  // Only used in the dynamic (when STRING_TYPE == CORBA::String_var)
  // recursive TypeCode case.
}

template <typename STRING_TYPE>
ACE_INLINE char const *
TAO::TypeCode::Base_Attributes<STRING_TYPE>::id (void) const
{
  return Traits<STRING_TYPE>::get_string (this->id_);
}

template <typename STRING_TYPE>
ACE_INLINE char const *
TAO::TypeCode::Base_Attributes<STRING_TYPE>::name (void) const
{
  return Traits<STRING_TYPE>::get_string (this->name_);
}

template <typename STRING_TYPE>
ACE_INLINE void
TAO::TypeCode::Base_Attributes<STRING_TYPE>::name (char const * the_name)
{
  // This method will only be instantiated in the dynamic TypeCode
  // case, i.e. when STRING_TYPE is CORBA::String_var.

  this->name_ = the_name;
}

TAO_END_VERSIONED_NAMESPACE_DECL
