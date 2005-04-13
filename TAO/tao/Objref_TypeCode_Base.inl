// -*- C++ -*-
//
// $Id$


template <typename StringType, class RefCountPolicy>
ACE_INLINE
TAO::TypeCode::Objref_Base<StringType, RefCountPolicy>::Objref_Base (
  CORBA::TCKind kind,
  char const * id,
  char const * name)
  : CORBA::TypeCode (kind)
  , RefCountPolicy ()
  , attributes_ (id, name)
{
}
