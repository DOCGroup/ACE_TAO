/* -*- C++ -*- */
// $Id$

#include "DynCommon.h"
#include "DynAnyFactory.h"
#include "DynAny_i.h"
#include "DynArray_i.h"
#include "DynEnum_i.h"
#include "DynSequence_i.h"
#include "DynStruct_i.h"
#include "DynUnion_i.h"

ACE_RCSID(DynamicAny, DynCommon, "$Id$")

// Constructor.
TAO_DynCommon::TAO_DynCommon (void)
{
}

// Destructor.
TAO_DynCommon::~TAO_DynCommon (void)
{
}

// *****************************************************************

CORBA::TypeCode_ptr
TAO_DynCommon::type (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                        CORBA::TypeCode::_nil ());
    }

  return CORBA::TypeCode::_duplicate (this->type_.in ());
}

void
TAO_DynCommon::assign (DynamicAny::DynAny_ptr dyn_any,
                       CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW (CORBA::OBJECT_NOT_EXIST ());
    }

  CORBA_TypeCode_var tc = dyn_any->type (ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::Boolean equivalent = this->type_.in ()->equivalent (tc.in (),
                                                             ACE_TRY_ENV);
  ACE_CHECK;

  if (equivalent)
    {
      CORBA_Any_ptr any = dyn_any->to_any (ACE_TRY_ENV);
      ACE_CHECK;

      this->from_any (*any,
                      ACE_TRY_ENV);
      ACE_CHECK;
    }
  else
    {
      ACE_THROW (DynamicAny::DynAny::TypeMismatch ());
    }
}

void
TAO_DynCommon::insert_boolean (CORBA::Boolean value,
                               CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW (CORBA::OBJECT_NOT_EXIST ());
    }

  if (this->has_components_)
    {
      DynamicAny::DynAny_var cc = this->check_component (ACE_TRY_ENV);
      ACE_CHECK;

      cc->insert_boolean (value,
                          ACE_TRY_ENV);
      ACE_CHECK;
    }
  else
    {
      this->check_type (CORBA::_tc_boolean,
                        ACE_TRY_ENV);
      ACE_CHECK;
      
      this->any_ <<= CORBA::Any::from_boolean (value);
    }
}

void
TAO_DynCommon::insert_octet (CORBA::Octet value,
                             CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW (CORBA::OBJECT_NOT_EXIST ());
    }

  if (this->has_components_)
    {
      DynamicAny::DynAny_var cc = this->check_component (ACE_TRY_ENV);
      ACE_CHECK;

      cc->insert_octet (value,
                        ACE_TRY_ENV);
      ACE_CHECK;
    }
  else
    {
      this->check_type (CORBA::_tc_octet,
                        ACE_TRY_ENV);
      ACE_CHECK;
      
      this->any_ <<= CORBA::Any::from_octet (value);
    }
}

void
TAO_DynCommon::insert_char (CORBA::Char value,
                            CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW (CORBA::OBJECT_NOT_EXIST ());
    }

  if (this->has_components_)
    {
      DynamicAny::DynAny_var cc = this->check_component (ACE_TRY_ENV);
      ACE_CHECK;

      cc->insert_char (value,
                       ACE_TRY_ENV);
      ACE_CHECK;
    }
  else
    {
      this->check_type (CORBA::_tc_char,
                        ACE_TRY_ENV);
      ACE_CHECK;
      
      this->any_ <<= CORBA::Any::from_char (value);
    }
}

void
TAO_DynCommon::insert_short (CORBA::Short value,
                             CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW (CORBA::OBJECT_NOT_EXIST ());
    }

  if (this->has_components_)
    {
      DynamicAny::DynAny_var cc = this->check_component (ACE_TRY_ENV);
      ACE_CHECK;

      cc->insert_short (value,
                        ACE_TRY_ENV);
      ACE_CHECK;
    }
  else
    {
      this->check_type (CORBA::_tc_short,
                        ACE_TRY_ENV);
      ACE_CHECK;
      
      this->any_ <<= value;
    }
}

void
TAO_DynCommon::insert_ushort (CORBA::UShort value,
                              CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW (CORBA::OBJECT_NOT_EXIST ());
    }

  if (this->has_components_)
    {
      DynamicAny::DynAny_var cc = this->check_component (ACE_TRY_ENV);
      ACE_CHECK;

      cc->insert_ushort (value,
                         ACE_TRY_ENV);
      ACE_CHECK;
    }
  else
    {
      this->check_type (CORBA::_tc_ushort,
                        ACE_TRY_ENV);
      ACE_CHECK;
      
      this->any_ <<= value;
    }
}

void
TAO_DynCommon::insert_long (CORBA::Long value,
                            CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW (CORBA::OBJECT_NOT_EXIST ());
    }

  if (this->has_components_)
    {
      DynamicAny::DynAny_var cc = this->check_component (ACE_TRY_ENV);
      ACE_CHECK;

      cc->insert_long (value,
                       ACE_TRY_ENV);
      ACE_CHECK;
    }
  else
    {
      this->check_type (CORBA::_tc_long,
                        ACE_TRY_ENV);
      ACE_CHECK;
      
      this->any_ <<= value;
    }
}

void
TAO_DynCommon::insert_ulong (CORBA::ULong value,
                             CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW (CORBA::OBJECT_NOT_EXIST ());
    }

  if (this->has_components_)
    {
      DynamicAny::DynAny_var cc = this->check_component (ACE_TRY_ENV);
      ACE_CHECK;

      cc->insert_ulong (value,
                        ACE_TRY_ENV);
      ACE_CHECK;
    }
  else
    {
      this->check_type (CORBA::_tc_ulong,
                        ACE_TRY_ENV);
      ACE_CHECK;
      
      this->any_ <<= value;
    }
}

void
TAO_DynCommon::insert_float (CORBA::Float value,
                             CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW (CORBA::OBJECT_NOT_EXIST ());
    }

  if (this->has_components_)
    {
      DynamicAny::DynAny_var cc = this->check_component (ACE_TRY_ENV);
      ACE_CHECK;

      cc->insert_float (value,
                        ACE_TRY_ENV);
      ACE_CHECK;
    }
  else
    {
      this->check_type (CORBA::_tc_float,
                        ACE_TRY_ENV);
      ACE_CHECK;
      
      this->any_ <<= value;
    }
}

void
TAO_DynCommon::insert_double (CORBA::Double value,
                              CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW (CORBA::OBJECT_NOT_EXIST ());
    }

  if (this->has_components_)
    {
      DynamicAny::DynAny_var cc = this->check_component (ACE_TRY_ENV);
      ACE_CHECK;

      cc->insert_double (value,
                         ACE_TRY_ENV);
      ACE_CHECK;
    }
  else
    {
      this->check_type (CORBA::_tc_double,
                        ACE_TRY_ENV);
      ACE_CHECK;
      
      this->any_ <<= value;
    }
}

void
TAO_DynCommon::insert_string (const char * value,
                              CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW (CORBA::OBJECT_NOT_EXIST ());
    }

  if (this->has_components_)
    {
      DynamicAny::DynAny_var cc = this->check_component (ACE_TRY_ENV);
      ACE_CHECK;

      cc->insert_string (value,
                         ACE_TRY_ENV);
      ACE_CHECK;
    }
  else
    {
      this->check_type (CORBA::_tc_string,
                        ACE_TRY_ENV);
      ACE_CHECK;

      CORBA::ULong length = this->type_->length (ACE_TRY_ENV);
      ACE_CHECK;

      if (length > 0 && length < ACE_OS::strlen (value))
        {
          ACE_THROW (DynamicAny::DynAny::InvalidValue ());
        }
      
      this->any_ <<= value;
    }
}

void
TAO_DynCommon::insert_reference (CORBA::Object_ptr value,
                                 CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW (CORBA::OBJECT_NOT_EXIST ());
    }

  if (this->has_components_)
    {
      DynamicAny::DynAny_var cc = this->check_component (ACE_TRY_ENV);
      ACE_CHECK;

      cc->insert_reference (value,
                            ACE_TRY_ENV);
      ACE_CHECK;
    }
  else
    {
      this->check_type (CORBA::_tc_Object,
                        ACE_TRY_ENV);
      ACE_CHECK;
      
      this->any_ <<= value;
    }
}

void
TAO_DynCommon::insert_typecode (CORBA::TypeCode_ptr value,
                                CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW (CORBA::OBJECT_NOT_EXIST ());
    }

  if (this->has_components_)
    {
      DynamicAny::DynAny_var cc = this->check_component (ACE_TRY_ENV);
      ACE_CHECK;

      cc->insert_typecode (value,
                           ACE_TRY_ENV);
      ACE_CHECK;
    }
  else
    {
      this->check_type (CORBA::_tc_TypeCode,
                        ACE_TRY_ENV);
      ACE_CHECK;
      
      this->any_ <<= value;
    }
}

void
TAO_DynCommon::insert_longlong (CORBA::LongLong value,
                                CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW (CORBA::OBJECT_NOT_EXIST ());
    }

  if (this->has_components_)
    {
      DynamicAny::DynAny_var cc = this->check_component (ACE_TRY_ENV);
      ACE_CHECK;

      cc->insert_longlong (value,
                           ACE_TRY_ENV);
      ACE_CHECK;
    }
  else
    {
      this->check_type (CORBA::_tc_longlong,
                        ACE_TRY_ENV);
      ACE_CHECK;
      
      this->any_ <<= value;
    }
}

void
TAO_DynCommon::insert_ulonglong (CORBA::ULongLong value,
                                 CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW (CORBA::OBJECT_NOT_EXIST ());
    }

  if (this->has_components_)
    {
      DynamicAny::DynAny_var cc = this->check_component (ACE_TRY_ENV);
      ACE_CHECK;

      cc->insert_ulonglong (value,
                            ACE_TRY_ENV);
      ACE_CHECK;
    }
  else
    {
      this->check_type (CORBA::_tc_ulonglong,
                        ACE_TRY_ENV);
      ACE_CHECK;
      
      this->any_ <<= value;
    }
}

void
TAO_DynCommon::insert_longdouble (CORBA::LongDouble,
                                  CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC (( 
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

void
TAO_DynCommon::insert_wchar (CORBA::WChar value,
                               CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW (CORBA::OBJECT_NOT_EXIST ());
    }

  if (this->has_components_)
    {
      DynamicAny::DynAny_var cc = this->check_component (ACE_TRY_ENV);
      ACE_CHECK;

      cc->insert_wchar (value,
                        ACE_TRY_ENV);
      ACE_CHECK;
    }
  else
    {
      this->check_type (CORBA::_tc_wchar,
                        ACE_TRY_ENV);
      ACE_CHECK;
      
      this->any_ <<= value;
    }
}

void
TAO_DynCommon::insert_wstring (const CORBA::WChar * value,
                               CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW (CORBA::OBJECT_NOT_EXIST ());
    }

  if (this->has_components_)
    {
      DynamicAny::DynAny_var cc = this->check_component (ACE_TRY_ENV);
      ACE_CHECK;

      cc->insert_wstring (value,
                          ACE_TRY_ENV);
      ACE_CHECK;
    }
  else
    {
      this->check_type (CORBA::_tc_wstring,
                        ACE_TRY_ENV);
      ACE_CHECK;

      CORBA::ULong length = this->type_->length (ACE_TRY_ENV);
      ACE_CHECK;

      if (length > 0 && length < ACE_OS::wslen (value))
        {
          ACE_THROW (DynamicAny::DynAny::InvalidValue ());
        }
      
      this->any_ <<= value;
    }
}

void
TAO_DynCommon::insert_any (const CORBA::Any &value,
                           CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW (CORBA::OBJECT_NOT_EXIST ());
    }

  if (this->has_components_)
    {
      DynamicAny::DynAny_var cc = this->check_component (ACE_TRY_ENV);
      ACE_CHECK;

      cc->insert_any (value,
                      ACE_TRY_ENV);
      ACE_CHECK;
    }
  else
    {
      this->check_type (CORBA::_tc_any,
                        ACE_TRY_ENV);
      ACE_CHECK;
      
      this->any_ <<= value;
    }
}

// @@@ (JP) TODO - optimize - this version was intended by the OMG to
// have fewer Any/DynAny conversions than insert_any, not more.
void
TAO_DynCommon::insert_dyn_any (DynamicAny::DynAny_ptr value,
                               CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW (CORBA::OBJECT_NOT_EXIST ());
    }

  CORBA_Any_var any = value->to_any (ACE_TRY_ENV);
  ACE_CHECK;

  this->insert_any (any.in (),
                    ACE_TRY_ENV);
  ACE_CHECK;
}

#ifdef TAO_HAS_VALUETYPE
void
TAO_DynCommon::insert_val (CORBA::ValueBase_ptr,
                           CORBA::Environment &ACE_TRY_ENV)
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

CORBA::Boolean
TAO_DynCommon::get_boolean (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                        0);
    }

  if (this->has_components_)
    {
      DynamicAny::DynAny_var cc = this->check_component (ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      return cc->get_boolean (ACE_TRY_ENV);
    }
  else
    {
      CORBA::Boolean retval;
      
      if ((this->any_ >>= CORBA::Any::to_boolean (retval)) == 0)
        {
          ACE_THROW_RETURN (DynamicAny::DynAny::TypeMismatch (),
                            0);
        }
          
      return retval;
    }
}

CORBA::Octet
TAO_DynCommon::get_octet (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                        0);
    }

  if (this->has_components_)
    {
      DynamicAny::DynAny_var cc = this->check_component (ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      return cc->get_octet (ACE_TRY_ENV);
    }
  else
    {
      CORBA::Octet retval;
      
      if ((this->any_ >>= CORBA::Any::to_octet (retval)) == 0)
        {
          ACE_THROW_RETURN (DynamicAny::DynAny::TypeMismatch (),
                            0);
        }
          
      return retval;
    }
}

CORBA::Char
TAO_DynCommon::get_char (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                        0);
    }

  if (this->has_components_)
    {
      DynamicAny::DynAny_var cc = this->check_component (ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      return cc->get_char (ACE_TRY_ENV);
    }
  else
    {
      CORBA::Char retval;
      
      if ((this->any_ >>= CORBA::Any::to_char (retval)) == 0)
        {
          ACE_THROW_RETURN (DynamicAny::DynAny::TypeMismatch (),
                            0);
        }
          
      return retval;
    }
}

CORBA::Short
TAO_DynCommon::get_short (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                        0);
    }

  if (this->has_components_)
    {
      DynamicAny::DynAny_var cc = this->check_component (ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      return cc->get_short (ACE_TRY_ENV);
    }
  else
    {
      CORBA::Short retval;
      
      if ((this->any_ >>= retval) == 0)
        {
          ACE_THROW_RETURN (DynamicAny::DynAny::TypeMismatch (),
                            0);
        }
          
      return retval;
    }
}

CORBA::UShort
TAO_DynCommon::get_ushort (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                        0);
    }

  if (this->has_components_)
    {
      DynamicAny::DynAny_var cc = this->check_component (ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      return cc->get_ushort (ACE_TRY_ENV);
    }
  else
    {
      CORBA::UShort retval;
      
      if ((this->any_ >>= retval) == 0)
        {
          ACE_THROW_RETURN (DynamicAny::DynAny::TypeMismatch (),
                            0);
        }
          
      return retval;
    }
}

CORBA::Long
TAO_DynCommon::get_long (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                        0);
    }

  if (this->has_components_)
    {
      DynamicAny::DynAny_var cc = this->check_component (ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      return cc->get_long (ACE_TRY_ENV);
    }
  else
    {
      CORBA::Long retval;
      
      if ((this->any_ >>= retval) == 0)
        {
          ACE_THROW_RETURN (DynamicAny::DynAny::TypeMismatch (),
                            0);
        }
          
      return retval;
    }
}

CORBA::ULong
TAO_DynCommon::get_ulong (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                        0);
    }

  if (this->has_components_)
    {
      DynamicAny::DynAny_var cc = this->check_component (ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      return cc->get_ulong (ACE_TRY_ENV);
    }
  else
    {
      CORBA::ULong retval;
      
      if ((this->any_ >>= retval) == 0)
        {
          ACE_THROW_RETURN (DynamicAny::DynAny::TypeMismatch (),
                            0);
        }
          
      return retval;
    }
}

CORBA::Float
TAO_DynCommon::get_float (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                        0);
    }

  if (this->has_components_)
    {
      DynamicAny::DynAny_var cc = this->check_component (ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      return cc->get_float (ACE_TRY_ENV);
    }
  else
    {
      CORBA::Float retval;
      
      if ((this->any_ >>= retval) == 0)
        {
          ACE_THROW_RETURN (DynamicAny::DynAny::TypeMismatch (),
                            0);
        }
          
      return retval;
    }
}

CORBA::Double
TAO_DynCommon::get_double (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                        0);
    }

  if (this->has_components_)
    {
      DynamicAny::DynAny_var cc = this->check_component (ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      return cc->get_double (ACE_TRY_ENV);
    }
  else
    {
      CORBA::Double retval;
      
      if ((this->any_ >>= retval) == 0)
        {
          ACE_THROW_RETURN (DynamicAny::DynAny::TypeMismatch (),
                            0);
        }
          
      return retval;
    }
}

char *
TAO_DynCommon::get_string (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                        0);
    }

  if (this->has_components_)
    {
      DynamicAny::DynAny_var cc = this->check_component (ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      return cc->get_string (ACE_TRY_ENV);
    }
  else
    {
      // @@@ (JP) Someday try to find a way to avoid checking for
      // type code equivalence twice without risking a throw of
      // BadKind.
      this->check_type (CORBA::_tc_string,
                        ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      char *retval;

      CORBA::ULong bound = this->type_->length (ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);
      
      (void) (this->any_ >>= CORBA::Any::to_string (retval,
                                                    bound));
          
      return CORBA::string_dup (retval);
    }
}

CORBA::Object_ptr
TAO_DynCommon::get_reference (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                        CORBA::Object::_nil ());
    }

  if (this->has_components_)
    {
      DynamicAny::DynAny_var cc = this->check_component (ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      return cc->get_reference (ACE_TRY_ENV);
    }
  else
    {
      CORBA::Object_ptr retval;
      
      if ((this->any_ >>= CORBA::Any::to_object (retval)) == 0)
        {
          ACE_THROW_RETURN (DynamicAny::DynAny::TypeMismatch (),
                            CORBA::Object::_nil ());
        }
          
      return CORBA::Object::_duplicate (retval);
    }
}

CORBA::TypeCode_ptr
TAO_DynCommon::get_typecode (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                        CORBA::TypeCode::_nil ());
    }

  if (this->has_components_)
    {
      DynamicAny::DynAny_var cc = this->check_component (ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      return cc->get_typecode (ACE_TRY_ENV);
    }
  else
    {
      CORBA::TypeCode_ptr retval;
      
      if ((this->any_ >>= retval) == 0)
        {
          ACE_THROW_RETURN (DynamicAny::DynAny::TypeMismatch (),
                            CORBA::TypeCode::_nil ());
        }
          
      return CORBA::TypeCode::_duplicate (retval);
    }
}

CORBA::LongLong
TAO_DynCommon::get_longlong (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                        0);
    }

  if (this->has_components_)
    {
      DynamicAny::DynAny_var cc = this->check_component (ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      return cc->get_longlong (ACE_TRY_ENV);
    }
  else
    {
      CORBA::LongLong retval;
      
      if ((this->any_ >>= retval) == 0)
        {
          ACE_THROW_RETURN (DynamicAny::DynAny::TypeMismatch (),
                            0);
        }
          
      return retval;
    }
}

CORBA::ULongLong
TAO_DynCommon::get_ulonglong (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                        0);
    }

  if (this->has_components_)
    {
      DynamicAny::DynAny_var cc = this->check_component (ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      return cc->get_ulonglong (ACE_TRY_ENV);
    }
  else
    {
      CORBA::ULongLong retval;
      
      if ((this->any_ >>= retval) == 0)
        {
          ACE_THROW_RETURN (DynamicAny::DynAny::TypeMismatch (),
                            0);
        }
          
      return retval;
    }
}

CORBA::LongDouble
TAO_DynCommon::get_longdouble (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  CORBA::LongDouble ret = ACE_CDR_LONG_DOUBLE_INITIALIZER;
  ACE_UNUSED_ARG (ret);
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 
                    ret);
}

CORBA::WChar
TAO_DynCommon::get_wchar (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                        0);
    }

  if (this->has_components_)
    {
      DynamicAny::DynAny_var cc = this->check_component (ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      return cc->get_wchar (ACE_TRY_ENV);
    }
  else
    {
      CORBA::WChar retval;
      
      if ((this->any_ >>= CORBA::Any::to_wchar (retval)) == 0)
        {
          ACE_THROW_RETURN (DynamicAny::DynAny::TypeMismatch (),
                            0);
        }
          
      return retval;
    }
}

CORBA::WChar *
TAO_DynCommon::get_wstring (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                        0);
    }

  if (this->has_components_)
    {
      DynamicAny::DynAny_var cc = this->check_component (ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      return cc->get_wstring (ACE_TRY_ENV);
    }
  else
    {
      // @@@ (JP) Someday try to find a way to avoid checking for
      // type code equivalence twice without risking a throw of
      // BadKind.
      this->check_type (CORBA::_tc_wstring,
                        ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      CORBA::WChar *retval;

      CORBA::ULong bound = this->type_->length (ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);
      
      (void) (this->any_ >>= CORBA::Any::to_wstring (retval,
                                                     bound));
          
      return CORBA::wstring_dup (retval);
    }
}

CORBA::Any_ptr
TAO_DynCommon::get_any (CORBA::Environment& ACE_TRY_ENV)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                        0);
    }

  if (this->has_components_)
    {
      DynamicAny::DynAny_var cc = this->check_component (ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      return cc->get_any (ACE_TRY_ENV);
    }
  else
    {
      CORBA_Any_ptr tmp = 0;

      ACE_NEW_THROW_EX (tmp,
                        CORBA_Any,
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      CORBA_Any_var val = tmp;

      if ((this->any_ >>= *tmp) == 0)
        {
          ACE_THROW_RETURN (DynamicAny::DynAny::TypeMismatch (),
                            0);
        }

      return val._retn ();
    }
}

// @@@ (JP) TODO - optimize - this version was intended by the OMG to
// have fewer Any/DynAny conversions than get_any, not more.
DynamicAny::DynAny_ptr
TAO_DynCommon::get_dyn_any (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                        0);
    }

  CORBA_Any_var any = this->get_any (ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  return TAO_DynAnyFactory::make_dyn_any (any.in (),
                                          ACE_TRY_ENV);
}

#ifdef TAO_HAS_VALUETYPE
CORBA::ValueBase_ptr
TAO_DynCommon::get_val (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 
                    0);
}
#endif /* TAO_HAS_VALUETYPE */

// ****************************************************************

CORBA::Boolean
TAO_DynCommon::seek (CORBA::Long slot,
                     CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                        0);
    }

  if (this->has_components_ == 0)
    {
      ACE_UNUSED_ARG (slot);
      this->current_position_ = -1;
      return 0;
    }
  else if (slot < 0 || slot >= ACE_static_cast (CORBA::Long, 
                                                this->component_count_))
    {
      this->current_position_ = -1;
      return 0;
    }
  else
    {
      this->current_position_ = slot;
      return 1;
    }
}

void
TAO_DynCommon::rewind (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW (CORBA::OBJECT_NOT_EXIST ());
    }

  (void) this->seek (0,
                     ACE_TRY_ENV);
  ACE_CHECK;
}

CORBA::Boolean
TAO_DynCommon::next (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                        0);
    }

  if (this->has_components_ == 0
      || this->current_position_ + 1 >= ACE_static_cast (
                                            CORBA::Long, 
                                            this->component_count_
                                          ))
    {
      this->current_position_ = -1;
      return 0;
    }
  else
    {
      ++this->current_position_;
      return 1;
    }
}

DynamicAny::DynAny_ptr
TAO_DynCommon::copy (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                        DynamicAny::DynAny::_nil ());
    }

  CORBA_Any_var any = this->to_any (ACE_TRY_ENV);
  ACE_CHECK_RETURN (DynamicAny::DynAny::_nil ());

  DynamicAny::DynAny_ptr retval = 
    TAO_DynAnyFactory::make_dyn_any (any.in (),
                                     ACE_TRY_ENV);
  ACE_CHECK_RETURN (DynamicAny::DynAny::_nil ());

  return retval;
}

CORBA::ULong
TAO_DynCommon::component_count (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                        0);
    }

  return this->component_count_;
}

void 
TAO_DynCommon::set_flag (DynamicAny::DynAny_ptr component,
                         CORBA::Boolean destroying,
                         CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  CORBA::TypeCode_var tc = component->type (ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::TCKind tk = tc->kind (ACE_TRY_ENV);
  ACE_CHECK;

  switch (tk)
  {
    case CORBA::tk_array:
      {
        TAO_DynArray_i *tmp = TAO_DynArray_i::_narrow (component,
                                                       ACE_TRY_ENV);
        ACE_CHECK;

        if (destroying)
          {
            tmp->container_is_destroying_ = 1;
          }
        else
          {
            tmp->ref_to_component_ = 1;
          }

        tmp->_remove_ref ();
        break;
      }
    case CORBA::tk_enum:
      {
        TAO_DynEnum_i *tmp = TAO_DynEnum_i::_narrow (component,
                                                     ACE_TRY_ENV);
        ACE_CHECK;

        if (destroying)
          {
            tmp->container_is_destroying_ = 1;
          }
        else
          {
            tmp->ref_to_component_ = 1;
          }

        tmp->_remove_ref ();
        break;
      }
    case CORBA::tk_sequence:
      {
        TAO_DynSequence_i *tmp = TAO_DynSequence_i::_narrow (component,
                                                             ACE_TRY_ENV);
        ACE_CHECK;

        if (destroying)
          {
            tmp->container_is_destroying_ = 1;
          }
        else
          {
            tmp->ref_to_component_ = 1;
          }

        tmp->_remove_ref ();
        break;
      }
    case CORBA::tk_struct:
      {
        TAO_DynStruct_i *tmp = TAO_DynStruct_i::_narrow (component,
                                                         ACE_TRY_ENV);
        ACE_CHECK;

        if (destroying)
          {
            tmp->container_is_destroying_ = 1;
          }
        else
          {
            tmp->ref_to_component_ = 1;
          }

        tmp->_remove_ref ();
        break;
      }
    case CORBA::tk_union:
      {
        TAO_DynUnion_i *tmp = TAO_DynUnion_i::_narrow (component,
                                                       ACE_TRY_ENV);
        ACE_CHECK;

        if (destroying)
          {
            tmp->container_is_destroying_ = 1;
          }
        else
          {
            tmp->ref_to_component_ = 1;
          }

        tmp->_remove_ref ();
        break;
      }
    default:
      {
        TAO_DynAny_i *tmp = TAO_DynAny_i::_narrow (component,
                                                   ACE_TRY_ENV);
        ACE_CHECK;

        if (destroying)
          {
            tmp->container_is_destroying_ = 1;
          }
        else
          {
            tmp->ref_to_component_ = 1;
          }

        tmp->_remove_ref ();
        break;
      }
  }
}

DynamicAny::DynAny_ptr
TAO_DynCommon::check_component (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  if (this->current_position_ == -1)
    {
      ACE_THROW_RETURN (DynamicAny::DynAny::InvalidValue (),
                        DynamicAny::DynAny::_nil ());
    }

  DynamicAny::DynAny_var cc = this->current_component (ACE_TRY_ENV);
  ACE_CHECK_RETURN (DynamicAny::DynAny::_nil ());

  CORBA::TypeCode_var tc = cc->type (ACE_TRY_ENV);
  ACE_CHECK_RETURN (DynamicAny::DynAny::_nil ());

  CORBA::TCKind kind = TAO_DynAnyFactory::unalias (tc.in (),
                                                   ACE_TRY_ENV);
  ACE_CHECK_RETURN (DynamicAny::DynAny::_nil ());

  // We are here because we are a component that is the target of
  // an insert_*() call on our container. It is
  // illegal to insert anything into a component that itself has
  // components.
  switch (kind)
  {
    case CORBA::tk_array:
    case CORBA::tk_except:
    case CORBA::tk_sequence:
    case CORBA::tk_struct:
    case CORBA::tk_union:
      ACE_THROW_RETURN (DynamicAny::DynAny::TypeMismatch (),
                        DynamicAny::DynAny::_nil ());
    default:
      return cc._retn ();
  }
}

void
TAO_DynCommon::check_type (CORBA::TypeCode_ptr tc,
                           CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch
    ))
{
  CORBA::Boolean equivalent = this->type_->equivalent (tc,
                                                       ACE_TRY_ENV);
  ACE_CHECK;

  if (!equivalent)
    {
      ACE_THROW (DynamicAny::DynAny::TypeMismatch ());
    }
}

