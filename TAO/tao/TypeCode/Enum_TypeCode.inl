// -*- C++ -*-
//
// $Id$

template <typename StringType, class AttrType, class RefCountPolicy>
ACE_INLINE
TAO::TypeCode::Enum<StringType, AttrType, RefCountPolicy>::Enum (
  char const * id,
  char const * name,
  Enumerator<StringType> const * enumerators,
  CORBA::ULong nenumerators)
  : base_attributes_ (id, name)
  , nenumerators_ (nenumerators)
  , enumerators_ (enumerators)
{
}
