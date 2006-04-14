// -*- C++ -*-
//
// $Id$


template <typename StringType,
          class EnumeratorArrayType,
          class RefCountPolicy>
ACE_INLINE
TAO::TypeCode::Enum<StringType, EnumeratorArrayType, RefCountPolicy>::Enum (
  char const * id,
  char const * name,
  EnumeratorArrayType const & enumerators,
  CORBA::ULong nenumerators)
  : CORBA::TypeCode (CORBA::tk_enum)
  , RefCountPolicy ()
  , base_attributes_ (id, name)
  , nenumerators_ (nenumerators)
  , enumerators_ (enumerators)
{
}
