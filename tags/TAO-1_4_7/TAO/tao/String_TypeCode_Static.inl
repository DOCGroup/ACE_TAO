// -*- C++ -*-
//
// $Id$

ACE_INLINE
TAO::TypeCode::String<TAO::Null_RefCount_Policy>::String (
  CORBA::TCKind kind,
  CORBA::ULong length)
  : CORBA::TypeCode (kind)
  , ACE_NESTED_CLASS (TAO, Null_RefCount_Policy) ()
  , length_ (length)
{
  // ACE_ASSERT (kind == CORBA::tk_string || kind == CORBA::tk_wstring);
}
