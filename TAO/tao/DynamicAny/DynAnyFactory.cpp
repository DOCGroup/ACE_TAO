// $Id$

// =================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    DynAnyFactory.cpp
//
// = AUTHOR
//    Carlos O'Ryan <coryan@uci.edu>
//
// =================================================================

#include "DynAnyFactory.h"

#include "DynAny_i.h"
#include "DynStruct_i.h"
#include "DynSequence_i.h"
#include "DynEnum_i.h"
#include "DynArray_i.h"
#include "DynUnion_i.h"

#include "ace/Auto_Ptr.h"

ACE_RCSID (DynamicAny,
           DynAnyFactory,
           "$Id$")

// Constructor from typecode
TAO_DynAnyFactory::TAO_DynAnyFactory (void)
{
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
  return TAO_DynAnyFactory::make_dyn_any (value ACE_ENV_ARG_PARAMETER);
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
  return TAO_DynAnyFactory::make_dyn_any (type ACE_ENV_ARG_PARAMETER);
}

DynamicAny::DynAny_ptr
TAO_DynAnyFactory::make_dyn_any (const CORBA::Any &any
                                 ACE_ENV_ARG_DECL)
{
  CORBA::TypeCode_var tc = any.type ();
  CORBA::TCKind kind = TAO_DynAnyFactory::unalias (tc.in ()
                                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (DynamicAny::DynAny::_nil ());

  switch (kind)
    {
      case CORBA::tk_null:
      case CORBA::tk_void:
      case CORBA::tk_short:
      case CORBA::tk_long:
      case CORBA::tk_ushort:
      case CORBA::tk_ulong:
      case CORBA::tk_float:
      case CORBA::tk_double:
      case CORBA::tk_longlong:
      case CORBA::tk_ulonglong:
      case CORBA::tk_boolean:
      case CORBA::tk_char:
      case CORBA::tk_wchar:
      case CORBA::tk_octet:
      case CORBA::tk_any:
      case CORBA::tk_TypeCode:
      case CORBA::tk_objref:
      case CORBA::tk_string:
      case CORBA::tk_wstring:
        {
          TAO_DynAny_i *p = 0;

          ACE_NEW_THROW_EX (p,
                            TAO_DynAny_i,
                            CORBA::NO_MEMORY ());
          ACE_CHECK_RETURN (0);

          ACE_Auto_Basic_Ptr<TAO_DynAny_i> dp (p);
          p->init (any ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (0);

          return dp.release ();
        }
      case CORBA::tk_struct:
      case CORBA::tk_except:
        {
          TAO_DynStruct_i *p = 0;

          ACE_NEW_THROW_EX (p,
                            TAO_DynStruct_i,
                            CORBA::NO_MEMORY ());
          ACE_CHECK_RETURN (0);

          ACE_Auto_Basic_Ptr<TAO_DynStruct_i> dp (p);
          p->init (any ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (0);

          return dp.release ();
        }
      case CORBA::tk_sequence:
        {
          TAO_DynSequence_i *p = 0;

          ACE_NEW_THROW_EX (p,
                            TAO_DynSequence_i,
                            CORBA::NO_MEMORY ());
          ACE_CHECK_RETURN (0);

          ACE_Auto_Basic_Ptr<TAO_DynSequence_i> dp (p);
          p->init (any ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (0);

          return dp.release ();
        }
      case CORBA::tk_union:
        {
          TAO_DynUnion_i *p = 0;

          ACE_NEW_THROW_EX (p,
                            TAO_DynUnion_i,
                            CORBA::NO_MEMORY ());
          ACE_CHECK_RETURN (0);

          ACE_Auto_Basic_Ptr<TAO_DynUnion_i> dp (p);
          p->init (any ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (0);

          return dp.release ();
        }
      case CORBA::tk_enum:
        {
          TAO_DynEnum_i *p = 0;

          ACE_NEW_THROW_EX (p,
                            TAO_DynEnum_i,
                            CORBA::NO_MEMORY ());
          ACE_CHECK_RETURN (0);

          ACE_Auto_Basic_Ptr<TAO_DynEnum_i> dp (p);
          p->init (any ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (0);

          return dp.release ();
        }
      case CORBA::tk_array:
        {
          TAO_DynArray_i *p = 0;

          ACE_NEW_THROW_EX (p,
                            TAO_DynArray_i,
                            CORBA::NO_MEMORY ());
          ACE_CHECK_RETURN (0);

          ACE_Auto_Basic_Ptr<TAO_DynArray_i> dp (p);
          p->init (any ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (0);

          return dp.release ();
        }

    case CORBA::tk_fixed:
    case CORBA::tk_value:
    case CORBA::tk_value_box:
    case CORBA::tk_abstract_interface:
    case CORBA::tk_component:
    case CORBA::tk_home:
      ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (),
                        DynamicAny::DynAny::_nil ());
    case CORBA::tk_native:
      ACE_THROW_RETURN (DynamicAny::DynAnyFactory::InconsistentTypeCode (),
                        DynamicAny::DynAny::_nil ());
    default:
      break;
    }

  return DynamicAny::DynAny::_nil ();
}

DynamicAny::DynAny_ptr
TAO_DynAnyFactory::make_dyn_any (CORBA::TypeCode_ptr tc
                                 ACE_ENV_ARG_DECL)
{
  CORBA::TCKind kind =
    TAO_DynAnyFactory::unalias (tc ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (DynamicAny::DynAny::_nil ());

  switch (kind)
    {
      case CORBA::tk_null:
      case CORBA::tk_void:
      case CORBA::tk_short:
      case CORBA::tk_long:
      case CORBA::tk_ushort:
      case CORBA::tk_ulong:
      case CORBA::tk_float:
      case CORBA::tk_double:
      case CORBA::tk_longlong:
      case CORBA::tk_ulonglong:
      case CORBA::tk_boolean:
      case CORBA::tk_char:
      case CORBA::tk_wchar:
      case CORBA::tk_octet:
      case CORBA::tk_any:
      case CORBA::tk_TypeCode:
      case CORBA::tk_objref:
      case CORBA::tk_string:
      case CORBA::tk_wstring:
        {
          TAO_DynAny_i *p = 0;

          ACE_NEW_THROW_EX (p,
                            TAO_DynAny_i,
                            CORBA::NO_MEMORY ());
          ACE_CHECK_RETURN (0);

          ACE_Auto_Basic_Ptr<TAO_DynAny_i> dp (p);
          p->init (tc ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (0);

          return dp.release ();
        }
      case CORBA::tk_struct:
      case CORBA::tk_except:
        {
          TAO_DynStruct_i *p = 0;

          ACE_NEW_THROW_EX (p,
                            TAO_DynStruct_i,
                            CORBA::NO_MEMORY ());
          ACE_CHECK_RETURN (0);

          ACE_Auto_Basic_Ptr<TAO_DynStruct_i> dp (p);
          p->init (tc ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (0);

          return dp.release ();
        }
      case CORBA::tk_sequence:
        {
          TAO_DynSequence_i *p = 0;

          ACE_NEW_THROW_EX (p,
                            TAO_DynSequence_i,
                            CORBA::NO_MEMORY ());
          ACE_CHECK_RETURN (0);

          ACE_Auto_Basic_Ptr<TAO_DynSequence_i> dp (p);
          p->init (tc ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (0);

          return dp.release ();
        }
      case CORBA::tk_union:
        {
          TAO_DynUnion_i *p = 0;

          ACE_NEW_THROW_EX (p,
                            TAO_DynUnion_i,
                            CORBA::NO_MEMORY ());
          ACE_CHECK_RETURN (0);

          ACE_Auto_Basic_Ptr<TAO_DynUnion_i> dp (p);
          p->init (tc ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (0);

          return dp.release ();
        }
      case CORBA::tk_enum:
        {
          TAO_DynEnum_i *p = 0;

          ACE_NEW_THROW_EX (p,
                            TAO_DynEnum_i,
                            CORBA::NO_MEMORY ());
          ACE_CHECK_RETURN (0);

          ACE_Auto_Basic_Ptr<TAO_DynEnum_i> dp (p);
          p->init (tc ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (0);

          return dp.release ();
        }
      case CORBA::tk_array:
        {
          TAO_DynArray_i *p = 0;

          ACE_NEW_THROW_EX (p,
                            TAO_DynArray_i,
                            CORBA::NO_MEMORY ());
          ACE_CHECK_RETURN (0);

          ACE_Auto_Basic_Ptr<TAO_DynArray_i> dp (p);
          p->init (tc ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (0);

          return dp.release ();
        }

    case CORBA::tk_fixed:
    case CORBA::tk_value:
    case CORBA::tk_value_box:
    case CORBA::tk_abstract_interface:
    case CORBA::tk_component:
    case CORBA::tk_home:
      ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (),
                        DynamicAny::DynAny::_nil ());
    case CORBA::tk_native:
      ACE_THROW_RETURN (DynamicAny::DynAnyFactory::InconsistentTypeCode (),
                        DynamicAny::DynAny::_nil ());
    default:
      break;
    }

  return DynamicAny::DynAny::_nil ();
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Array_Base<DynamicAny::DynAny_var>;

template class ACE_Auto_Basic_Ptr<TAO_DynAny_i>;
template class ACE_Auto_Basic_Ptr<TAO_DynArray_i>;
template class ACE_Auto_Basic_Ptr<TAO_DynEnum_i>;
template class ACE_Auto_Basic_Ptr<TAO_DynSequence_i>;
template class ACE_Auto_Basic_Ptr<TAO_DynStruct_i>;
template class ACE_Auto_Basic_Ptr<TAO_DynUnion_i>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Array_Base<DynamicAny::DynAny_var>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_DynAny_i>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_DynArray_i>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_DynEnum_i>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_DynSequence_i>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_DynStruct_i>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_DynUnion_i>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
