// $Id$

#include "tao/DynamicAny/DynValueBox_i.h"

ACE_RCSID (DynamicAny,
           DynValueBox_i,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_DynValueBox_i::TAO_DynValueBox_i (void)
{
}

TAO_DynValueBox_i::~TAO_DynValueBox_i (void)
{
}

CORBA::Any *
TAO_DynValueBox_i::get_boxed_value (

  )
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::InvalidValue
    ))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

void
TAO_DynValueBox_i::set_boxed_value (
    const CORBA::Any & /* boxed */

  )
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  throw ::CORBA::NO_IMPLEMENT ();
}

DynamicAny::DynAny_ptr
TAO_DynValueBox_i::get_boxed_value_as_dyn_any (

  )
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::InvalidValue
    ))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (),
                    DynamicAny::DynAny::_nil ());
}

void
TAO_DynValueBox_i::set_boxed_value_as_dyn_any (
    DynamicAny::DynAny_ptr /* boxed */

  )
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch
    ))
{
  throw ::CORBA::NO_IMPLEMENT ();
}

TAO_END_VERSIONED_NAMESPACE_DECL

