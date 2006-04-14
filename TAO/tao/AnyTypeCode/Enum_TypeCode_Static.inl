// -*- C++ -*-
//
// $Id$


ACE_INLINE
TAO::TypeCode::Enum<char const *,
                    char const * const *,
                    TAO::Null_RefCount_Policy>::Enum (
  char const * id,
  char const * name,
  char const * const * enumerators,
  CORBA::ULong nenumerators)
  : CORBA::TypeCode (CORBA::tk_enum)
  , ACE_NESTED_CLASS (TAO, Null_RefCount_Policy) ()
  , base_attributes_ (id, name)
  , nenumerators_ (nenumerators)
  , enumerators_ (enumerators)
{
}
