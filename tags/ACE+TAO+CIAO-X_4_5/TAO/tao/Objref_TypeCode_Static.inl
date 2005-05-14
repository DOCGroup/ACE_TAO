// -*- C++ -*-
//
// $Id$


ACE_INLINE
TAO::TypeCode::Objref<char const *, TAO::Null_RefCount_Policy>::Objref (
  CORBA::TCKind kind,
  char const * id,
  char const * name)
  : CORBA::TypeCode (kind)
  , ACE_NESTED_CLASS (TAO, Null_RefCount_Policy) ()
  , attributes_ (id, name)
{
}
