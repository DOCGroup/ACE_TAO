// -*- C++ -*-
//
// $Id$

#include "tao/TypeCode_Traits.h"


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
