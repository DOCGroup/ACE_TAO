/* -*- C++ -*- */
// $Id$

// ===================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    DynEnum_i.cpp
//
// = AUTHOR
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ===================================================================

#include "DynEnum_i.h"

#include "DynAnyFactory.h"

ACE_RCSID(DynamicAny, DynEnum_i, "$Id$")

// Constructors and destructor

TAO_DynEnum_i::TAO_DynEnum_i (void)
{
}

TAO_DynEnum_i::~TAO_DynEnum_i (void)
{
}

void
TAO_DynEnum_i::init (const CORBA_Any &any,
                     CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::TypeCode_var tc = any.type ();

  CORBA::TCKind kind =
    TAO_DynAnyFactory::unalias (tc.in (),
                                ACE_TRY_ENV);
  ACE_CHECK;

  if (kind != CORBA::tk_enum)
    {
      ACE_THROW (DynamicAny::DynAnyFactory::InconsistentTypeCode ());
    }

  this->type_ = tc;

  ACE_Message_Block *mb = any._tao_get_cdr ();

  TAO_InputCDR cdr (mb,
                    any._tao_byte_order ());

  cdr.read_ulong (this->value_);
}

void
TAO_DynEnum_i::init (CORBA_TypeCode_ptr tc,
                     CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::TCKind kind = TAO_DynAnyFactory::unalias (tc,
                                                   ACE_TRY_ENV);
  ACE_CHECK;

  if (kind != CORBA::tk_enum)
    {
      ACE_THROW (DynamicAny::DynAnyFactory::InconsistentTypeCode ());
    }

  this->type_ = CORBA::TypeCode::_duplicate (tc);

  this->value_ = 0U;
}

// ****************************************************************

TAO_DynEnum_i *
TAO_DynEnum_i::_narrow (CORBA::Object_ptr obj,
                        CORBA::Environment &)
{
  if (CORBA::is_nil (obj))
    {
      return 0;
    }

  return ACE_reinterpret_cast (
             TAO_DynEnum_i*,
             obj->_tao_QueryInterface (
                      ACE_reinterpret_cast (
                          ptr_arith_t,
                          &TAO_DynEnum_i::_narrow
                        )
                    )
           );
}

void*
TAO_DynEnum_i::_tao_QueryInterface (ptr_arith_t type)
{
  ptr_arith_t mytype =
    ACE_reinterpret_cast (ptr_arith_t,
                          &TAO_DynEnum_i::_narrow);
  if (type == mytype)
    {
      this->_add_ref ();
      return this;
    }

  return 
    this->ACE_NESTED_CLASS (DynamicAny, DynEnum::_tao_QueryInterface) (type);
}

// ****************************************************************

char *
TAO_DynEnum_i::get_as_string (CORBA::Environment& ACE_TRY_ENV)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  const char *retval = this->type_.in ()->member_name (this->value_,
                                                       ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  return CORBA::string_dup (retval);
}

void
TAO_DynEnum_i::set_as_string (const char *value_as_string,
                              CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::InvalidValue
    ))
{
  CORBA::ULong count = this->type_.in ()->member_count (ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::ULong i;

  for (i = 0; i < count; i++)
    {
      const char *temp = this->type_.in ()->member_name (i,
                                                         ACE_TRY_ENV);
      ACE_CHECK;

      if (!ACE_OS::strcmp (value_as_string,
                           temp))
        {
          break;
        }
    }

  if (i < count)
    {
      this->value_ = i;
    }
  else
    {
      // *** NOT part of CORBA spec ***
      ACE_THROW (DynamicAny::DynAny::InvalidValue ());
    }
}

CORBA::ULong
TAO_DynEnum_i::get_as_ulong (CORBA::Environment &)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  return this->value_;
}

void
TAO_DynEnum_i::set_as_ulong (CORBA::ULong value_as_ulong,
                             CORBA::Environment& ACE_TRY_ENV)
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::InvalidValue
    ))
{
  CORBA::ULong max = this->type_.in ()->member_count (ACE_TRY_ENV);
  ACE_CHECK;

  if (value_as_ulong < max)
    {
      this->value_ = value_as_ulong;
    }
  else
    {
      // *** NOT part of CORBA spec ***
      ACE_THROW (DynamicAny::DynAny::InvalidValue ());
    }
}

// ****************************************************************

CORBA::TypeCode_ptr
TAO_DynEnum_i::type (CORBA::Environment &)
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ))
{
  return CORBA::TypeCode::_duplicate (this->type_.in ());
}

void
TAO_DynEnum_i::assign (DynamicAny::DynAny_ptr dyn_any,
                       CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch
      ))
{
  // *dyn_any->to_any raises Invalid if arg is bad.
  CORBA_TypeCode_ptr tc = dyn_any->type (ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::Boolean equal = this->type_.in ()->equal (tc,
                                                   ACE_TRY_ENV);
  ACE_CHECK;

  if (equal)
    {
      CORBA_Any_ptr any_ptr = dyn_any->to_any (ACE_TRY_ENV);
      ACE_CHECK;

      this->from_any (*any_ptr,
                      ACE_TRY_ENV);
      ACE_CHECK;
    }
  else
    {
      ACE_THROW (DynamicAny::DynAny::TypeMismatch ());
    }
}

void
TAO_DynEnum_i::from_any (const CORBA_Any& any,
                         CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  CORBA::TypeCode_var tc = any.type ();
  CORBA::TCKind kind = TAO_DynAnyFactory::unalias (tc.in (),
                                              ACE_TRY_ENV);
  ACE_CHECK;

  if (kind == CORBA::tk_enum)
    {
      // Get the CDR stream of the argument.
      ACE_Message_Block* mb = any._tao_get_cdr ();

      TAO_InputCDR cdr (mb,
                        any._tao_byte_order ());

      cdr.read_ulong (this->value_);
    }
  else
    {
      ACE_THROW (DynamicAny::DynAny::TypeMismatch ());
    }
}

CORBA::Any_ptr
TAO_DynEnum_i::to_any (CORBA::Environment& ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ))
{
  TAO_OutputCDR out_cdr;

  out_cdr.write_ulong (this->value_);

  CORBA_Any *retval;
  ACE_NEW_THROW_EX (retval,
                    CORBA_Any (this->type_.in (),
                               0,
                               TAO_ENCAP_BYTE_ORDER,
                               out_cdr.begin ()),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);
  return retval;
}

CORBA::Boolean
TAO_DynEnum_i::equal (
        DynamicAny::DynAny_ptr,
        CORBA::Environment &ACE_TRY_ENV
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

void
TAO_DynEnum_i::destroy (CORBA::Environment &)
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ))
{
}

DynamicAny::DynAny_ptr
TAO_DynEnum_i::copy (CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ))
{
  CORBA_Any_ptr a = this->to_any (ACE_TRY_ENV);
  ACE_CHECK_RETURN (DynamicAny::DynAny::_nil ());

  DynamicAny::DynAny_ptr dp =
    TAO_DynAnyFactory::make_dyn_any (*a,
                                               ACE_TRY_ENV);
  ACE_CHECK_RETURN (DynamicAny::DynAny::_nil ());

  return dp;
}

void
TAO_DynEnum_i::insert_boolean (CORBA::Boolean,
                               CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  ACE_THROW (CORBA::BAD_OPERATION ());
}

void
TAO_DynEnum_i::insert_octet (CORBA::Octet,
                             CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  ACE_THROW (CORBA::BAD_OPERATION ());
}

void
TAO_DynEnum_i::insert_char (CORBA::Char,
                            CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  ACE_THROW (CORBA::BAD_OPERATION ());
}

void
TAO_DynEnum_i::insert_short (CORBA::Short,
                             CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  ACE_THROW (CORBA::BAD_OPERATION ());
}

void
TAO_DynEnum_i::insert_ushort (CORBA::UShort,
                              CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  ACE_THROW (CORBA::BAD_OPERATION ());
}

void
TAO_DynEnum_i::insert_long (CORBA::Long,
                            CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  ACE_THROW (CORBA::BAD_OPERATION ());
}

void
TAO_DynEnum_i::insert_ulong (CORBA::ULong,
                             CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  ACE_THROW (CORBA::BAD_OPERATION ());
}

void
TAO_DynEnum_i::insert_float (CORBA::Float,
                             CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  ACE_THROW (CORBA::BAD_OPERATION ());
}

void
TAO_DynEnum_i::insert_double (CORBA::Double,
                              CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  ACE_THROW (CORBA::BAD_OPERATION ());
}

void
TAO_DynEnum_i::insert_string (const char *,
                              CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  ACE_THROW (CORBA::BAD_OPERATION ());
}

void
TAO_DynEnum_i::insert_reference (CORBA::Object_ptr,
                                 CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  ACE_THROW (CORBA::BAD_OPERATION ());
}

void
TAO_DynEnum_i::insert_typecode (CORBA::TypeCode_ptr,
                                CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  ACE_THROW (CORBA::BAD_OPERATION ());
}

void
TAO_DynEnum_i::insert_longlong (CORBA::LongLong,
                               CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  ACE_THROW (CORBA::BAD_OPERATION ());
}

void
TAO_DynEnum_i::insert_ulonglong (CORBA::ULongLong,
                                 CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  ACE_THROW (CORBA::BAD_OPERATION ());
}

void
TAO_DynEnum_i::insert_longdouble (
        CORBA::LongDouble,
        CORBA::Environment &ACE_TRY_ENV
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

void
TAO_DynEnum_i::insert_wchar (CORBA::WChar,
                             CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  ACE_THROW (CORBA::BAD_OPERATION ());
}

void
TAO_DynEnum_i::insert_wstring (
        const CORBA::WChar *,
        CORBA::Environment &ACE_TRY_ENV
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

void
TAO_DynEnum_i::insert_any (const CORBA::Any&,
                           CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  ACE_THROW (CORBA::BAD_OPERATION ());
}

CORBA::Boolean
TAO_DynEnum_i::get_boolean (CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  ACE_THROW_RETURN (CORBA::BAD_OPERATION (), 0);
}

void
TAO_DynEnum_i::insert_dyn_any (
        DynamicAny::DynAny_ptr,
        CORBA::Environment &ACE_TRY_ENV
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

#ifdef TAO_HAS_VALUETYPE
void
TAO_DynEnum_i::insert_val (
        CORBA::ValueBase_ptr,
        CORBA::Environment &ACE_TRY_ENV
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}
#endif /* TAO_HAS_VALUETYPE */

// ****************************************************************

CORBA::Octet
TAO_DynEnum_i::get_octet (CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  ACE_THROW_RETURN (CORBA::BAD_OPERATION (), 0);
}

CORBA::Char
TAO_DynEnum_i::get_char (CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  ACE_THROW_RETURN (CORBA::BAD_OPERATION (), 0);
}

CORBA::Short
TAO_DynEnum_i::get_short (CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  ACE_THROW_RETURN (CORBA::BAD_OPERATION (), 0);
}

CORBA::UShort
TAO_DynEnum_i::get_ushort (CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  ACE_THROW_RETURN (CORBA::BAD_OPERATION (), 0);
}

CORBA::Long
TAO_DynEnum_i::get_long (CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  ACE_THROW_RETURN (CORBA::BAD_OPERATION (), 0);
}

CORBA::ULong
TAO_DynEnum_i::get_ulong (CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  ACE_THROW_RETURN (CORBA::BAD_OPERATION (), 0);
}

CORBA::Float
TAO_DynEnum_i::get_float (CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  ACE_THROW_RETURN (CORBA::BAD_OPERATION (), 0);
}

CORBA::Double
TAO_DynEnum_i::get_double (CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  ACE_THROW_RETURN (CORBA::BAD_OPERATION (), 0);
}

char *
TAO_DynEnum_i::get_string (CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  ACE_THROW_RETURN (CORBA::BAD_OPERATION (), 0);
}

CORBA::Object_ptr
TAO_DynEnum_i::get_reference (CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  ACE_THROW_RETURN (CORBA::BAD_OPERATION (), 0);
}

CORBA::TypeCode_ptr
TAO_DynEnum_i::get_typecode (CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  ACE_THROW_RETURN (CORBA::BAD_OPERATION (), 0);
}

CORBA::LongLong
TAO_DynEnum_i::get_longlong (CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
#if defined (ACE_LACKS_LONGLONG_T)
  CORBA::LongLong val = {0, 0};
#else  /* ! ACE_LACKS_LONGLONG_T */
  CORBA::LongLong val = 0;
#endif /* ! ACE_LACKS_LONGLONG_T */

  ACE_UNUSED_ARG (val);
  ACE_THROW_RETURN (CORBA::BAD_OPERATION (), val;);
}

CORBA::ULongLong
TAO_DynEnum_i::get_ulonglong (CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  ACE_THROW_RETURN (CORBA::BAD_OPERATION (), 0);
}

CORBA::LongDouble
TAO_DynEnum_i::get_longdouble (
        CORBA::Environment &ACE_TRY_ENV
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  CORBA::LongDouble ret = ACE_CDR_LONG_DOUBLE_INITIALIZER;
  ACE_UNUSED_ARG (ret);
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), ret);
}

CORBA::WChar
TAO_DynEnum_i::get_wchar (CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  ACE_THROW_RETURN (CORBA::BAD_OPERATION (), 0);
}

CORBA::WChar *
TAO_DynEnum_i::get_wstring (
        CORBA::Environment &ACE_TRY_ENV
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

CORBA::Any_ptr
TAO_DynEnum_i::get_any (CORBA::Environment& ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  ACE_THROW_RETURN (CORBA::BAD_OPERATION (), 0);
}

DynamicAny::DynAny_ptr
TAO_DynEnum_i::get_dyn_any (
        CORBA::Environment &ACE_TRY_ENV
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), DynamicAny::DynAny::_nil ());
}

#ifdef TAO_HAS_VALUETYPE
CORBA::ValueBase_ptr
TAO_DynEnum_i::get_val (
        CORBA::Environment &ACE_TRY_ENV
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}
#endif /* TAO_HAS_VALUETYPE */

// ****************************************************************

CORBA::Boolean
TAO_DynEnum_i::seek (CORBA::Long slot,
                     CORBA::Environment &)
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ))
{
  if (slot == 0)
    return 1;
  else
    return 0;
}

// Nothing to do here.
void
TAO_DynEnum_i::rewind (CORBA::Environment &)
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ))
{
}

CORBA::Boolean
TAO_DynEnum_i::next (CORBA::Environment &)
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ))
{
  return 0;
}

CORBA::ULong
TAO_DynEnum_i::component_count (CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

DynamicAny::DynAny_ptr
TAO_DynEnum_i::current_component (CORBA::Environment &)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch
      ))
{
  return this;
}
