// $Id$

#include "tao/DynamicAny/DynValue_i.h"

ACE_RCSID (DynamicAny,
           DynValue_i,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_DynValue_i::TAO_DynValue_i (void)
{
}

TAO_DynValue_i::~TAO_DynValue_i (void)
{
}

char *
TAO_DynValue_i::current_member_name (
    ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

CORBA::TCKind
TAO_DynValue_i::current_member_kind (
    ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (),
                    CORBA::tk_null);
}

DynamicAny::NameValuePairSeq *
TAO_DynValue_i::get_members (
    ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

void
TAO_DynValue_i::set_members (
    const DynamicAny::NameValuePairSeq & /* value */
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

DynamicAny::NameDynAnyPairSeq *
TAO_DynValue_i::get_members_as_dyn_any (
    ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

void
TAO_DynValue_i::set_members_as_dyn_any (
    const DynamicAny::NameDynAnyPairSeq & /* value */
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

TAO_END_VERSIONED_NAMESPACE_DECL

