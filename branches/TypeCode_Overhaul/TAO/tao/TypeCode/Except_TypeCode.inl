// -*- C++ -*-
//
// $Id$

template <class AttrType, class RefCountPolicy>
ACE_INLINE
TAO::TypeCode::Except<AttrType, RefCountPolicy>::Except (
  char const * id,
  char const * name,
  TAO::TypeCode::Field const * fields,
  CORBA::ULong nfields)
  : Struct<AttrType, RefCountPolicy> (id, name, fields, nfields)
{
}
