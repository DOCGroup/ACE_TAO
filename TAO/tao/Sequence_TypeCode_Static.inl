// -*- C++ -*-
//
// $Id$

ACE_INLINE
TAO::TypeCode::Sequence<CORBA::TypeCode_ptr const *,
                        TAO::Null_RefCount_Policy>::Sequence (
  CORBA::TCKind kind,
  CORBA::TypeCode_ptr const * content_type,
  CORBA::ULong length)
  : CORBA::TypeCode (kind)
  , ACE_NESTED_CLASS (TAO, Null_RefCount_Policy) ()
  , content_type_ (content_type)
  , length_ (length)
{
}
