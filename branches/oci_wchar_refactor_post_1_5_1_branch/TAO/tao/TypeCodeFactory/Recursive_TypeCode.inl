// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
TAO::TypeCodeFactory::Recursive_TypeCode::Recursive_TypeCode (char const * id)
  : ::CORBA::TypeCode (CORBA::TAO_TC_KIND_COUNT)
  , ::TAO::True_RefCount_Policy ()
  , id_ (id)
  , the_typecode_ ()
{
}

ACE_INLINE void
TAO::TypeCodeFactory::Recursive_TypeCode::the_typecode (CORBA::TypeCode_ptr tc)
{
  this->the_typecode_ = CORBA::TypeCode::_duplicate (tc);

  ACE_DECLARE_NEW_CORBA_ENV;

  // @@ Lazy, I know.  :(
  CORBA::TCKind & mutable_kind =
    const_cast<CORBA::TCKind &> (this->kind_);

  mutable_kind = tc->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

TAO_END_VERSIONED_NAMESPACE_DECL
