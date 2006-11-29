//=============================================================================
/**
 *  @file    DynAnyFactory.cpp
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan <coryan@uci.edu>
 */
//=============================================================================


#include "tao/AnyTypeCode/AnyTypeCode_methods.h"

#include "tao/DynamicAny/DynAnyFactory.h"

#include "tao/DynamicAny/DynAny_i.h"
#include "tao/DynamicAny/DynStruct_i.h"
#include "tao/DynamicAny/DynSequence_i.h"
#include "tao/DynamicAny/DynEnum_i.h"
#include "tao/DynamicAny/DynArray_i.h"
#include "tao/DynamicAny/DynUnion_i.h"
#include "tao/DynamicAny/DynAnyUtils_T.h"

#include "ace/Auto_Ptr.h"

ACE_RCSID (DynamicAny,
           DynAnyFactory,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Constructor from typecode
TAO_DynAnyFactory::TAO_DynAnyFactory (void)
{
}

DynamicAny::DynAny_ptr
TAO_DynAnyFactory::create_dyn_any (
      const CORBA::Any & value
      ACE_ENV_ARG_DECL
    )
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAnyFactory::InconsistentTypeCode
    ))
{
  return
    TAO::MakeDynAnyUtils<const CORBA::Any&>::make_dyn_any_t (
      value._tao_get_typecode (),
      value);
}

DynamicAny::DynAny_ptr
TAO_DynAnyFactory::create_dyn_any_from_type_code (
      CORBA::TypeCode_ptr type
      ACE_ENV_ARG_DECL
    )
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAnyFactory::InconsistentTypeCode
    ))
{
  // Second arg is typed in the template parameter, repeating it
  // this way allows cleaner template code.
  return
    TAO::MakeDynAnyUtils<CORBA::TypeCode_ptr>::make_dyn_any_t (
      type,
      type);
}

DynamicAny::DynAny_ptr
TAO_DynAnyFactory::create_dyn_any_without_truncation (
    const CORBA::Any & /* value */
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAnyFactory::InconsistentTypeCode,
      DynamicAny::MustTruncate
    ))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (),
                    DynamicAny::DynAny::_nil ());
}

DynamicAny::DynAnySeq *
TAO_DynAnyFactory::create_multiple_dyn_anys (
    const DynamicAny::AnySeq & /* values */,
    ::CORBA::Boolean /* allow_truncate */
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAnyFactory::InconsistentTypeCode,
      DynamicAny::MustTruncate
    ))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

DynamicAny::AnySeq *
TAO_DynAnyFactory::create_multiple_anys (
    const DynamicAny::DynAnySeq & /* values */
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}
      
// Utility function called by all the DynAny classes
// to extract the TCKind of possibly aliased types.
CORBA::TCKind
TAO_DynAnyFactory::unalias (CORBA::TypeCode_ptr tc
                            ACE_ENV_ARG_DECL)
{
  CORBA::TCKind tck = tc->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::tk_null);

  while (tck == CORBA::tk_alias)
    {
      CORBA::TypeCode_var temp =
        tc->content_type (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (CORBA::tk_null);

      tck = TAO_DynAnyFactory::unalias (temp.in ()
                                        ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (CORBA::tk_null);
    }

  return tck;
}

// Same as above, but returns the type code.
CORBA::TypeCode_ptr
TAO_DynAnyFactory::strip_alias (CORBA::TypeCode_ptr tc
                                ACE_ENV_ARG_DECL)
{
  CORBA::TypeCode_var retval = CORBA::TypeCode::_duplicate (tc);
  CORBA::TCKind tck = retval->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  while (tck == CORBA::tk_alias)
    {
      retval = retval->content_type (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

      tck = retval->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());
    }

  return retval._retn ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
