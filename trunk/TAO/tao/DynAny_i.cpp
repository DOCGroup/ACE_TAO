/* -*- C++ -*- */
// $Id$
// =================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    DynAny_i.cpp
//
// = AUTHOR
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// =================================================================

#include "tao/DynAny_i.h"

#if !defined (TAO_HAS_MINIMUM_CORBA)

#include "tao/DynStruct_i.h"
#include "tao/DynSequence_i.h"
#include "tao/DynEnum_i.h"
#include "tao/DynArray_i.h"
#include "tao/DynUnion_i.h"
#include "tao/InconsistentTypeCodeC.h"

// Constructor from typecode
TAO_DynAny_i::TAO_DynAny_i (CORBA_TypeCode_ptr tc)
  : value_ (CORBA::TypeCode::_duplicate (tc))
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // Check to see if it's a simple type.
      switch (unalias (tc, ACE_TRY_ENV))
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
            break;
          default:
            ACE_THROW (CORBA_ORB_InconsistentTypeCode ());
            break;
        }
    }
  ACE_CATCHANY
    {
      // do nothing
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

// Constructor from Any
TAO_DynAny_i::TAO_DynAny_i (const CORBA_Any& any)
  : value_ (any)
{
  // Check to see if it's a simple type.
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      switch (unalias (any.type (), ACE_TRY_ENV))
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
            break;
          default:
            ACE_THROW (CORBA_DynAny::Invalid ());
            break;
        }
    }
  ACE_CATCHANY
    {
      // do nothing
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

// Destructor
TAO_DynAny_i::~TAO_DynAny_i (void)
{
}

// Common functions

CORBA::TypeCode_ptr
TAO_DynAny_i::type (CORBA::Environment &)
{
  return CORBA_TypeCode::_duplicate (this->value_.type ());
}

void
TAO_DynAny_i::assign (CORBA_DynAny_ptr dyn_any,
                      CORBA::Environment &ACE_TRY_ENV)
{
  if (this->value_.type ()->equal (dyn_any->type (ACE_TRY_ENV),
                                   ACE_TRY_ENV))
    this->value_ = *dyn_any->to_any (ACE_TRY_ENV);
  else
    ACE_THROW (CORBA_DynAny::Invalid ());
}

CORBA_DynAny_ptr
TAO_DynAny_i::copy (CORBA::Environment &ACE_TRY_ENV)
{
  TAO_DynAny_i* tmp = new TAO_DynAny_i (this->value_);
  return tmp->_this (ACE_TRY_ENV);
}

void
TAO_DynAny_i::destroy (CORBA::Environment &)
{
  delete this;
}

void
TAO_DynAny_i::from_any (const CORBA_Any& any,
                        CORBA::Environment &ACE_TRY_ENV)
{
  if (!this->value_.type ()->equal (any.type (),
                                    ACE_TRY_ENV)
      || any._tao_get_cdr () == 0)
    {
      ACE_THROW (CORBA_DynAny::Invalid ());
    }

  this->value_ = any;
}

CORBA::Any_ptr
TAO_DynAny_i::to_any (CORBA::Environment &ACE_TRY_ENV)
{
  if (this->value_._tao_get_cdr ())
    return new CORBA::Any (this->value_);
  else
    {
      ACE_THROW (CORBA_DynAny::Invalid ());
      return 0;
    }
}

CORBA_DynAny_ptr
TAO_DynAny_i::current_component (CORBA::Environment &ACE_TRY_ENV)
{
  return this->_this (ACE_TRY_ENV);
}

CORBA::Boolean
TAO_DynAny_i::next (CORBA::Environment &)
{
  return 0;
}

CORBA::Boolean
TAO_DynAny_i::seek (CORBA::Long slot,
                    CORBA::Environment &)
{
  if (slot == 0)
    return 1;
  else
    return 0;
}

// Nothing to rewind in this class.
void
TAO_DynAny_i::rewind (CORBA::Environment &)
{
}

// Insert functions

void
TAO_DynAny_i::insert_boolean (CORBA::Boolean value,
                              CORBA::Environment &ACE_TRY_ENV)
{
  if (unalias (this->value_.type (), ACE_TRY_ENV) == CORBA::tk_boolean)
    this->value_ <<= CORBA::Any::from_boolean (value);
  else
    ACE_THROW (CORBA_DynAny::InvalidValue ());
}

void
TAO_DynAny_i::insert_octet (CORBA::Octet value,
                            CORBA::Environment &ACE_TRY_ENV)
{
  if (this->value_.type ()->kind (ACE_TRY_ENV) == CORBA::tk_octet)
    this->value_ <<= CORBA::Any::from_octet (value);
  else
    ACE_THROW (CORBA_DynAny::InvalidValue ());
}

void
TAO_DynAny_i::insert_char (CORBA::Char value,
                           CORBA::Environment &ACE_TRY_ENV)
{
  if (this->value_.type ()->kind (ACE_TRY_ENV) == CORBA::tk_char)
    this->value_ <<= CORBA::Any::from_char (value);
  else
    ACE_THROW (CORBA_DynAny::InvalidValue ());
}

void
TAO_DynAny_i::insert_short (CORBA::Short value,
                            CORBA::Environment &ACE_TRY_ENV)
{
  if (this->value_.type ()->kind (ACE_TRY_ENV) == CORBA::tk_short)
    this->value_ <<= value;
  else
    ACE_THROW (CORBA_DynAny::InvalidValue ());
}

void
TAO_DynAny_i::insert_ushort (CORBA::UShort value,
                             CORBA::Environment &ACE_TRY_ENV)
{
  if (this->value_.type ()->kind (ACE_TRY_ENV) == CORBA::tk_ushort)
    this->value_ <<= value;
  else
    ACE_THROW (CORBA_DynAny::InvalidValue ());
}

void
TAO_DynAny_i::insert_long (CORBA::Long value,
                           CORBA::Environment &ACE_TRY_ENV)
{
  if (this->value_.type ()->kind (ACE_TRY_ENV) == CORBA::tk_long)
    this->value_ <<= value;
  else
    ACE_THROW (CORBA_DynAny::InvalidValue ());
}

void
TAO_DynAny_i::insert_ulong (CORBA::ULong value,
                            CORBA::Environment &ACE_TRY_ENV)
{
  if (this->value_.type ()->kind (ACE_TRY_ENV) == CORBA::tk_ulong)
    this->value_ <<= value;
  else
    ACE_THROW (CORBA_DynAny::InvalidValue ());
}

void
TAO_DynAny_i::insert_float (CORBA::Float value,
                            CORBA::Environment &ACE_TRY_ENV)
{
  if (this->value_.type ()->kind (ACE_TRY_ENV) == CORBA::tk_float)
    this->value_ <<= value;
  else
    ACE_THROW (CORBA_DynAny::InvalidValue ());
}

void
TAO_DynAny_i::insert_double (CORBA::Double value,
                             CORBA::Environment &ACE_TRY_ENV)
{
  if (this->value_.type ()->kind (ACE_TRY_ENV) == CORBA::tk_double)
    this->value_ <<= value;
  else
    ACE_THROW (CORBA_DynAny::InvalidValue ());
}

void
TAO_DynAny_i::insert_string (const char * value,
                             CORBA::Environment &ACE_TRY_ENV)
{
  if (this->value_.type ()->kind (ACE_TRY_ENV) == CORBA::tk_string)
    this->value_ <<= CORBA::Any::from_string (CORBA::string_dup (value),
                                              0,
                                              1);
  else
    ACE_THROW (CORBA_DynAny::InvalidValue ());
}

void
TAO_DynAny_i::insert_reference (CORBA::Object_ptr value,
                                CORBA::Environment &ACE_TRY_ENV)
{
  if (this->value_.type ()->kind (ACE_TRY_ENV) == CORBA::tk_objref)
    this->value_ <<= value;
  else
    ACE_THROW (CORBA_DynAny::InvalidValue ());
}

void
TAO_DynAny_i::insert_typecode (CORBA::TypeCode_ptr value,
                               CORBA::Environment &ACE_TRY_ENV)
{
  if (this->value_.type ()->kind (ACE_TRY_ENV) == CORBA::tk_TypeCode)
    this->value_ <<= value;
  else
    ACE_THROW (CORBA_DynAny::InvalidValue ());
}

void
TAO_DynAny_i::insert_longlong (CORBA::LongLong value,
                               CORBA::Environment &ACE_TRY_ENV)
{
  if (this->value_.type ()->kind (ACE_TRY_ENV) == CORBA::tk_longlong)
    this->value_ <<= value;
  else
    ACE_THROW (CORBA_DynAny::InvalidValue ());
}

void
TAO_DynAny_i::insert_ulonglong (CORBA::ULongLong value,
                                CORBA::Environment &ACE_TRY_ENV)
{
  if (this->value_.type ()->kind (ACE_TRY_ENV) == CORBA::tk_ulonglong)
    this->value_ <<= value;
  else
    ACE_THROW (CORBA_DynAny::InvalidValue ());
}

void
TAO_DynAny_i::insert_wchar (CORBA::WChar value,
                            CORBA::Environment &ACE_TRY_ENV)
{
  if (this->value_.type ()->kind (ACE_TRY_ENV) == CORBA::tk_wchar)
    this->value_ <<= CORBA::Any::from_wchar (value);
  else
    ACE_THROW (CORBA_DynAny::InvalidValue ());
}

void
TAO_DynAny_i::insert_any (const CORBA::Any& value,
                          CORBA::Environment &ACE_TRY_ENV)
{
  if (this->value_.type ()->kind (ACE_TRY_ENV) == CORBA::tk_any)
    this->value_ <<= value;
  else
    ACE_THROW (CORBA_DynAny::InvalidValue ());
}

// Get functions

CORBA::Boolean
TAO_DynAny_i::get_boolean (CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::Boolean val;

  if (!(this->value_ >>= CORBA_Any::to_boolean (val)))
    ACE_THROW (CORBA_DynAny::TypeMismatch ());

  return val;
}

CORBA::Octet
TAO_DynAny_i::get_octet (CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::Octet val;

  if (!(this->value_ >>= CORBA_Any::to_octet (val)))
    ACE_THROW (CORBA_DynAny::TypeMismatch ());

  return val;
}

CORBA::Char
TAO_DynAny_i::get_char (CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::Char val;

  if (!(this->value_ >>= CORBA_Any::to_char (val)))
    ACE_THROW (CORBA_DynAny::TypeMismatch ());

  return val;
}

CORBA::Short
TAO_DynAny_i::get_short (CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::Short val;

  if (!(this->value_ >>= val))
    ACE_THROW (CORBA_DynAny::TypeMismatch ());

  return val;
}

CORBA::UShort
TAO_DynAny_i::get_ushort (CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::UShort val;

  if (!(this->value_ >>= val))
    ACE_THROW (CORBA_DynAny::TypeMismatch ());

  return val;
}

CORBA::Long
TAO_DynAny_i::get_long (CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::Long val;

  if (!(this->value_ >>= val))
    ACE_THROW (CORBA_DynAny::TypeMismatch ());

  return val;
}

CORBA::ULong
TAO_DynAny_i::get_ulong (CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::ULong val;

  if (!(this->value_ >>= val))
    ACE_THROW (CORBA_DynAny::TypeMismatch ());

  return val;
}

CORBA::Float
TAO_DynAny_i::get_float (CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::Float val;

  if (!(this->value_ >>= val))
    ACE_THROW (CORBA_DynAny::TypeMismatch ());

  return val;
}

CORBA::Double
TAO_DynAny_i::get_double (CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::Double val;

  if (!(this->value_ >>= val))
    ACE_THROW (CORBA_DynAny::TypeMismatch ());

  return val;
}

char *
TAO_DynAny_i::get_string (CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::Char *val;
  CORBA::Any::to_string ts (val, 0);

  if (!(this->value_ >>= ts))
    ACE_THROW (CORBA_DynAny::TypeMismatch ());

  return ts.val_;
}

CORBA::Object_ptr
TAO_DynAny_i::get_reference (CORBA::Environment &ACE_TRY_ENV)
{
  CORBA_Object_ptr val;
  CORBA_Any_var a = this->to_any (ACE_TRY_ENV);

  if (!(this->value_ >>= CORBA_Any::to_object (val)))
    ACE_THROW (CORBA_DynAny::TypeMismatch ());

  return val;
}

CORBA::TypeCode_ptr
TAO_DynAny_i::get_typecode (CORBA::Environment &ACE_TRY_ENV)
{
  CORBA_TypeCode_ptr val;

  if (!(this->value_ >>= val))
    ACE_THROW (CORBA_DynAny::TypeMismatch ());

  return val;
}

CORBA::LongLong
TAO_DynAny_i::get_longlong (CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::LongLong val;

  if (!(this->value_ >>= val))
    ACE_THROW (CORBA_DynAny::TypeMismatch ());

  return val;
}

CORBA::ULongLong
TAO_DynAny_i::get_ulonglong (CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::ULongLong val;

  if (!(this->value_ >>= val))
    ACE_THROW (CORBA_DynAny::TypeMismatch ());

  return val;
}

CORBA::WChar
TAO_DynAny_i::get_wchar (CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::WChar val;

  if (!(this->value_ >>= CORBA_Any::to_wchar (val)))
    ACE_THROW (CORBA_DynAny::TypeMismatch ());

  return val;
}

CORBA::Any_ptr
TAO_DynAny_i::get_any (CORBA::Environment& ACE_TRY_ENV)
{
  CORBA_Any_var val;

  ACE_NEW_THROW_EX (val.out (),
                    CORBA_Any,
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  if (!(this->value_ >>= *val.out ()))
    {
      val = 0;
      ACE_THROW_RETURN (CORBA_DynAny::TypeMismatch (),
                        0);
    }

  return val._retn ();
}

// The factory functions

CORBA_DynAny_ptr
TAO_DynAny_i::create_dyn_any (const CORBA_Any &any,
                              CORBA::Environment &ACE_TRY_ENV)
{
  switch (TAO_DynAny_i::unalias (any.type (), ACE_TRY_ENV))
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
        {
          TAO_DynAny_i *dp;
          ACE_NEW_THROW_EX (dp,
                            TAO_DynAny_i (any),
                            CORBA::NO_MEMORY ());
          ACE_CHECK_RETURN (CORBA_DynAny::_nil ());
          return dp->_this (ACE_TRY_ENV);
        }
      case CORBA::tk_struct:
      case CORBA::tk_except:
        {
          TAO_DynStruct_i *dp;
          ACE_NEW_THROW_EX (dp,
                            TAO_DynStruct_i (any),
                            CORBA::NO_MEMORY ());
          ACE_CHECK_RETURN (CORBA_DynStruct::_nil ());
          return dp->_this (ACE_TRY_ENV);
        }
      case CORBA::tk_sequence:
        {
          TAO_DynSequence_i *dp;
          ACE_NEW_THROW_EX (dp,
                            TAO_DynSequence_i (any),
                            CORBA::NO_MEMORY ());
          ACE_CHECK_RETURN (CORBA_DynSequence::_nil ());
          return dp->_this (ACE_TRY_ENV);
        }
      case CORBA::tk_union:
        {
          TAO_DynUnion_i *dp;
          ACE_NEW_THROW_EX (dp,
                            TAO_DynUnion_i (any),
                            CORBA::NO_MEMORY ());
          ACE_CHECK_RETURN (CORBA_DynUnion::_nil ());
          return dp->_this (ACE_TRY_ENV);
        }
      case CORBA::tk_enum:
        {
          TAO_DynEnum_i *dp;
          ACE_NEW_THROW_EX (dp,
                            TAO_DynEnum_i (any),
                            CORBA::NO_MEMORY ());
          ACE_CHECK_RETURN (CORBA_DynEnum::_nil ());
          return dp->_this (ACE_TRY_ENV);
        }
      case CORBA::tk_array:
        {
          TAO_DynArray_i *dp;
          ACE_NEW_THROW_EX (dp,
                            TAO_DynArray_i (any),
                            CORBA::NO_MEMORY ());
          ACE_CHECK_RETURN (CORBA_DynArray::_nil ());
          return dp->_this (ACE_TRY_ENV);
        }
      default:
        break;
    }

  return CORBA_DynAny::_nil ();
}

CORBA_DynAny_ptr
TAO_DynAny_i::create_basic_dyn_any (CORBA_TypeCode_ptr tc,
                                    CORBA::Environment& ACE_TRY_ENV)
{
  TAO_DynAny_i *dp;
  ACE_NEW_THROW_EX (dp,
                    TAO_DynAny_i (tc),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (CORBA_DynAny::_nil ());
  return dp->_this (ACE_TRY_ENV);
}

CORBA_DynStruct_ptr
TAO_DynAny_i::create_dyn_struct (CORBA_TypeCode_ptr tc,
                                 CORBA::Environment& ACE_TRY_ENV)
{
  TAO_DynStruct_i *dp;
  ACE_NEW_THROW_EX (dp,
                    TAO_DynStruct_i (tc),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (CORBA_DynStruct::_nil ());
  return dp->_this (ACE_TRY_ENV);
}

CORBA_DynSequence_ptr
TAO_DynAny_i::create_dyn_sequence (CORBA_TypeCode_ptr tc,
                                   CORBA::Environment& ACE_TRY_ENV)
{
  TAO_DynSequence_i *dp;
  ACE_NEW_THROW_EX (dp,
                        TAO_DynSequence_i (tc),
                        CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (CORBA_DynSequence::_nil ());
  return dp->_this (ACE_TRY_ENV);
}

CORBA_DynArray_ptr
TAO_DynAny_i::create_dyn_array (CORBA_TypeCode_ptr tc,
                                CORBA::Environment& ACE_TRY_ENV)
{
  TAO_DynArray_i *dp;
  ACE_NEW_THROW_EX (dp,
                        TAO_DynArray_i (tc),
                        CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (CORBA_DynArray::_nil ());
  return dp->_this (ACE_TRY_ENV);
}

CORBA_DynUnion_ptr
TAO_DynAny_i::create_dyn_union (CORBA_TypeCode_ptr tc,
                                CORBA::Environment& ACE_TRY_ENV)
{
  TAO_DynUnion_i *dp;
  ACE_NEW_THROW_EX (dp,
                        TAO_DynUnion_i (tc),
                        CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (CORBA_DynUnion::_nil ());
  return dp->_this (ACE_TRY_ENV);
}

CORBA_DynEnum_ptr
TAO_DynAny_i::create_dyn_enum (CORBA_TypeCode_ptr tc,
                               CORBA::Environment& ACE_TRY_ENV)
{
  TAO_DynEnum_i *dp;
  ACE_NEW_THROW_EX (dp,
                        TAO_DynEnum_i (tc),
                        CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (CORBA_DynEnum::_nil ());
  return dp->_this (ACE_TRY_ENV);
}

CORBA_DynAny_ptr
TAO_DynAny_i::create_dyn_any (CORBA_TypeCode_ptr tc,
                              CORBA::Environment& ACE_TRY_ENV)
{
  switch (TAO_DynAny_i::unalias (tc, ACE_TRY_ENV))
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
        return create_basic_dyn_any (tc, ACE_TRY_ENV);
      case CORBA::tk_struct:
      case CORBA::tk_except:
        return create_dyn_struct (tc, ACE_TRY_ENV);
      case CORBA::tk_sequence:
        return create_dyn_sequence (tc, ACE_TRY_ENV);
      case CORBA::tk_union:
        return create_dyn_union (tc, ACE_TRY_ENV);
      case CORBA::tk_enum:
        return create_dyn_enum (tc, ACE_TRY_ENV);
      case CORBA::tk_array:
        return create_dyn_array (tc, ACE_TRY_ENV);
      default:
        break;
    }

  ACE_THROW_RETURN (CORBA_ORB_InconsistentTypeCode (),
                    CORBA_DynAny::_nil ());
}

// Utility function called by all the DynAny classes
// to extract the TCKind of possibly aliased types.
CORBA::TCKind
TAO_DynAny_i::unalias (CORBA_TypeCode_ptr tc,
                       CORBA::Environment& ACE_TRY_ENV)
{
  CORBA::TCKind tck = tc->kind (ACE_TRY_ENV);

  while (tck == CORBA::tk_alias)
    tck = TAO_DynAny_i::unalias (tc->content_type (ACE_TRY_ENV),
                                 ACE_TRY_ENV);

  return tck;
}

#endif /* TAO_HAS_MINIMUM_CORBA */
