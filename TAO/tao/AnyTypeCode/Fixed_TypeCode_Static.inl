// -*- C++ -*-
TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
TAO::TypeCode::Fixed<TAO::Null_RefCount_Policy>::Fixed (
  ::CORBA::UShort digits,
  ::CORBA::UShort scale)
  : ::CORBA::TypeCode (::CORBA::tk_fixed)
  , ::TAO::Null_RefCount_Policy ()
  , digits_ (digits)
  , scale_ (scale)
{
}

TAO_END_VERSIONED_NAMESPACE_DECL
