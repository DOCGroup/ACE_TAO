// -*- C++ -*-
//
// $Id$

#include "tao/True_RefCount_Policy.h"


template <typename StringType,
          class EnumeratorArrayType,
          class RefCountPolicy>
ACE_INLINE
TAO::TypeCode::Enum<StringType, EnumeratorArrayType, RefCountPolicy>::Enum (
  char const * id,
  char const * name,
  Enumerator<StringType> const * enumerators,
  CORBA::ULong nenumerators)
  : base_attributes_ (id, name)
  , nenumerators_ (nenumerators)
  , enumerators_ (enumerators)
{
}

template <typename StringType,
          class EnumeratorArrayType,
          class RefCountPolicy>
ACE_INLINE TAO::TypeCode::Enumerator<StringType> const *
TAO::TypeCode::Enum<
  StringType,
  EnumeratorArrayType,
  RefCountPolicy>::enumerators (void) const
{
  return this->enumerators_;

}

// -------------------------------------------------------------
// Member specializations
// -------------------------------------------------------------

template<>
ACE_INLINE TAO::TypeCode::Enumerator<CORBA::String_var> const *
TAO::TypeCode::Enum<
  CORBA::String_var,
  ACE_Auto_Array_Ptr<TAO::TypeCode::Enumerator<CORBA::String_var> const>,
  TAO::True_RefCount_Policy>::enumerators (void) const
{
  return this->enumerators_.get ();
}
