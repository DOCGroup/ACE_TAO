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

ACE_RCSID (DynamicAny,
           DynCommon,
           "$Id$")


TAO_DynCommon::TAO_DynCommon (void)
{
}

TAO_DynCommon::~TAO_DynCommon (void)
{
}

// *****************************************************************

CORBA::TypeCode_ptr
TAO_DynCommon::type (ACE_ENV_SINGLE_ARG_DECL)
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
TAO_DynCommon::assign (DynamicAny::DynAny_ptr dyn_any
                       ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW (CORBA::OBJECT_NOT_EXIST ());
    }

  CORBA::TypeCode_var tc = dyn_any->type (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::Boolean equivalent =
    this->type_.in ()->equivalent (tc.in ()
                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (equivalent)
    {
      CORBA::Any_var any = dyn_any->to_any (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      this->from_any (any.in ()
                      ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
  else
    {
      ACE_THROW (DynamicAny::DynAny::TypeMismatch ());
    }
}

void
TAO_DynCommon::insert_boolean (CORBA::Boolean value
                               ACE_ENV_ARG_DECL)
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
      DynamicAny::DynAny_var cc = this->check_component (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      cc->insert_boolean (value
                          ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
  else
    {
      this->check_type (CORBA::_tc_boolean
                        ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      this->any_ <<= CORBA::Any::from_boolean (value);
    }
}

void
TAO_DynCommon::insert_octet (CORBA::Octet value
                             ACE_ENV_ARG_DECL)
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
      DynamicAny::DynAny_var cc = this->check_component (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      cc->insert_octet (value
                        ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
  else
    {
      this->check_type (CORBA::_tc_octet
                        ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      this->any_ <<= CORBA::Any::from_octet (value);
    }
}

void
TAO_DynCommon::insert_char (CORBA::Char value
                            ACE_ENV_ARG_DECL)
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
      DynamicAny::DynAny_var cc = this->check_component (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      cc->insert_char (value
                       ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
  else
    {
      this->check_type (CORBA::_tc_char
                        ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      this->any_ <<= CORBA::Any::from_char (value);
    }
}

void
TAO_DynCommon::insert_short (CORBA::Short value
                             ACE_ENV_ARG_DECL)
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
      DynamicAny::DynAny_var cc = this->check_component (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      cc->insert_short (value
                        ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
  else
    {
      this->check_type (CORBA::_tc_short
                        ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      this->any_ <<= value;
    }
}

void
TAO_DynCommon::insert_ushort (CORBA::UShort value
                              ACE_ENV_ARG_DECL)
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
      DynamicAny::DynAny_var cc = this->check_component (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      cc->insert_ushort (value
                         ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
  else
    {
      this->check_type (CORBA::_tc_ushort
                        ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      this->any_ <<= value;
    }
}

void
TAO_DynCommon::insert_long (CORBA::Long value
                            ACE_ENV_ARG_DECL)
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
      DynamicAny::DynAny_var cc = this->check_component (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      cc->insert_long (value
                       ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
  else
    {
      this->check_type (CORBA::_tc_long
                        ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      this->any_ <<= value;
    }
}

void
TAO_DynCommon::insert_ulong (CORBA::ULong value
                             ACE_ENV_ARG_DECL)
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
      DynamicAny::DynAny_var cc = this->check_component (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      cc->insert_ulong (value
                        ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
  else
    {
      this->check_type (CORBA::_tc_ulong
                        ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      this->any_ <<= value;
    }
}

void
TAO_DynCommon::insert_float (CORBA::Float value
                             ACE_ENV_ARG_DECL)
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
      DynamicAny::DynAny_var cc = this->check_component (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      cc->insert_float (value
                        ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
  else
    {
      this->check_type (CORBA::_tc_float
                        ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      this->any_ <<= value;
    }
}

void
TAO_DynCommon::insert_double (CORBA::Double value
                              ACE_ENV_ARG_DECL)
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
      DynamicAny::DynAny_var cc = this->check_component (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      cc->insert_double (value
                         ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
  else
    {
      this->check_type (CORBA::_tc_double
                        ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      this->any_ <<= value;
    }
}

void
TAO_DynCommon::insert_string (const char * value
                              ACE_ENV_ARG_DECL)
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
      DynamicAny::DynAny_var cc = this->check_component (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      cc->insert_string (value
                         ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
  else
    {
      CORBA::TypeCode_var unaliased_tc =
        TAO_DynAnyFactory::strip_alias (this->type_.in ()
                                        ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      CORBA::TCKind kind = unaliased_tc->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      if (kind != CORBA::tk_string)
        {
          ACE_THROW (DynamicAny::DynAny::TypeMismatch ());
        }

      CORBA::ULong bound = unaliased_tc->length (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      if (bound > 0 && bound < ACE_OS::strlen (value))
        {
          ACE_THROW (DynamicAny::DynAny::InvalidValue ());
        }

      this->any_ <<= CORBA::Any::from_string (ACE_const_cast (char *,
                                                              value),
                                              bound);
    }
}

void
TAO_DynCommon::insert_reference (CORBA::Object_ptr value
                                 ACE_ENV_ARG_DECL)
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
      DynamicAny::DynAny_var cc = this->check_component (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      cc->insert_reference (value
                            ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
  else
    {
      CORBA::Boolean good_type = 1;
      CORBA::TCKind kind = TAO_DynAnyFactory::unalias (this->type_.in ()
                                                       ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      if (kind != CORBA::tk_objref)
        {
          good_type = 0;
        }
      else if (!CORBA::is_nil (value))
        {
          const char *value_id = value->_interface_repository_id ();

          if (ACE_OS::strcmp (value_id, "IDL:omg.org/CORBA/Object:1.0") != 0)
            {
              const char *my_id = this->type_->id (ACE_ENV_SINGLE_ARG_PARAMETER);
              ACE_CHECK;

              if (ACE_OS::strcmp (value_id, my_id) != 0)
                {
                  good_type = value->_is_a (my_id
                                            ACE_ENV_ARG_PARAMETER);
                  ACE_CHECK;
                }
            }
        }

      if (good_type)
        {
          TAO_OutputCDR cdr;
          value->marshal (cdr);
          TAO::Unknown_IDL_Type *unk = 0;
          ACE_NEW (unk,
                   TAO::Unknown_IDL_Type (this->type_.in (),
                                          cdr.begin (),
                                          TAO_ENCAP_BYTE_ORDER));
          this->any_.replace (unk);
        }
      else
        {
          ACE_THROW (DynamicAny::DynAny::TypeMismatch ());
        }
    }
}

void
TAO_DynCommon::insert_typecode (CORBA::TypeCode_ptr value
                                ACE_ENV_ARG_DECL)
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
      DynamicAny::DynAny_var cc = this->check_component (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      cc->insert_typecode (value
                           ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
  else
    {
      this->check_type (CORBA::_tc_TypeCode
                        ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      this->any_ <<= value;
    }
}

void
TAO_DynCommon::insert_longlong (CORBA::LongLong value
                                ACE_ENV_ARG_DECL)
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
      DynamicAny::DynAny_var cc = this->check_component (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      cc->insert_longlong (value
                           ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
  else
    {
      this->check_type (CORBA::_tc_longlong
                        ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      this->any_ <<= value;
    }
}

void
TAO_DynCommon::insert_ulonglong (CORBA::ULongLong value
                                 ACE_ENV_ARG_DECL)
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
      DynamicAny::DynAny_var cc = this->check_component (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      cc->insert_ulonglong (value
                            ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
  else
    {
      this->check_type (CORBA::_tc_ulonglong
                        ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      this->any_ <<= value;
    }
}

void
TAO_DynCommon::insert_longdouble (CORBA::LongDouble value
                                  ACE_ENV_ARG_DECL)
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
      DynamicAny::DynAny_var cc = this->check_component (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      cc->insert_longdouble (value
                             ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
  else
    {
      this->check_type (CORBA::_tc_longdouble
                        ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      this->any_ <<= value;
    }
}

void
TAO_DynCommon::insert_wchar (CORBA::WChar value
                             ACE_ENV_ARG_DECL)
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
      DynamicAny::DynAny_var cc = this->check_component (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      cc->insert_wchar (value
                        ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
  else
    {
      this->check_type (CORBA::_tc_wchar
                        ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      this->any_ <<= CORBA::Any::from_wchar (value);
    }
}

void
TAO_DynCommon::insert_wstring (const CORBA::WChar * value
                               ACE_ENV_ARG_DECL)
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
      DynamicAny::DynAny_var cc = this->check_component (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      cc->insert_wstring (value
                          ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
  else
    {
      CORBA::TypeCode_var unaliased_tc =
        this->check_type_and_unalias (CORBA::_tc_wstring
                                      ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      CORBA::ULong bound = unaliased_tc->length (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      if (bound > 0 && bound < ACE_OS::wslen (value))
        {
          ACE_THROW (DynamicAny::DynAny::InvalidValue ());
        }

      this->any_ <<= CORBA::Any::from_wstring (ACE_const_cast (CORBA::WChar *,
                                                               value),
                                               bound);
    }
}

void
TAO_DynCommon::insert_any (const CORBA::Any &value
                           ACE_ENV_ARG_DECL)
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
      DynamicAny::DynAny_var cc = this->check_component (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      cc->insert_any (value
                      ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
  else
    {
      this->check_type (CORBA::_tc_any
                        ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      this->any_ <<= value;
    }
}

// @@@ (JP) TODO - optimize - this version was intended by the OMG to
// have fewer Any/DynAny conversions than insert_any, not more.
void
TAO_DynCommon::insert_dyn_any (DynamicAny::DynAny_ptr value
                               ACE_ENV_ARG_DECL)
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

  CORBA::Any_var any = value->to_any (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->insert_any (any.in ()
                    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_DynCommon::insert_val (CORBA::ValueBase *
                           ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

// ****************************************************************

CORBA::Boolean
TAO_DynCommon::get_boolean (ACE_ENV_SINGLE_ARG_DECL)
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
      DynamicAny::DynAny_var cc = this->check_component (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      return cc->get_boolean (ACE_ENV_SINGLE_ARG_PARAMETER);
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
TAO_DynCommon::get_octet (ACE_ENV_SINGLE_ARG_DECL)
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
      DynamicAny::DynAny_var cc = this->check_component (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      return cc->get_octet (ACE_ENV_SINGLE_ARG_PARAMETER);
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
TAO_DynCommon::get_char (ACE_ENV_SINGLE_ARG_DECL)
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
      DynamicAny::DynAny_var cc = this->check_component (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      return cc->get_char (ACE_ENV_SINGLE_ARG_PARAMETER);
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
TAO_DynCommon::get_short (ACE_ENV_SINGLE_ARG_DECL)
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
      DynamicAny::DynAny_var cc = this->check_component (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      return cc->get_short (ACE_ENV_SINGLE_ARG_PARAMETER);
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
TAO_DynCommon::get_ushort (ACE_ENV_SINGLE_ARG_DECL)
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
      DynamicAny::DynAny_var cc = this->check_component (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      return cc->get_ushort (ACE_ENV_SINGLE_ARG_PARAMETER);
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
TAO_DynCommon::get_long (ACE_ENV_SINGLE_ARG_DECL)
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
      DynamicAny::DynAny_var cc = this->check_component (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      return cc->get_long (ACE_ENV_SINGLE_ARG_PARAMETER);
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
TAO_DynCommon::get_ulong (ACE_ENV_SINGLE_ARG_DECL)
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
      DynamicAny::DynAny_var cc = this->check_component (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      return cc->get_ulong (ACE_ENV_SINGLE_ARG_PARAMETER);
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
TAO_DynCommon::get_float (ACE_ENV_SINGLE_ARG_DECL)
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
      DynamicAny::DynAny_var cc = this->check_component (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      return cc->get_float (ACE_ENV_SINGLE_ARG_PARAMETER);
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
TAO_DynCommon::get_double (ACE_ENV_SINGLE_ARG_DECL)
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
      DynamicAny::DynAny_var cc = this->check_component (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      return cc->get_double (ACE_ENV_SINGLE_ARG_PARAMETER);
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
TAO_DynCommon::get_string (ACE_ENV_SINGLE_ARG_DECL)
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
      DynamicAny::DynAny_var cc = this->check_component (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      return cc->get_string (ACE_ENV_SINGLE_ARG_PARAMETER);
    }
  else
    {
      CORBA::TypeCode_var unaliased_tc =
        TAO_DynAnyFactory::strip_alias (this->type_.in ()
                                        ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      CORBA::TCKind kind = unaliased_tc->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      if (kind != CORBA::tk_string)
        {
          ACE_THROW_RETURN (DynamicAny::DynAny::TypeMismatch (),
                            0);
        }

      char *retval = 0;

      CORBA::ULong bound = unaliased_tc->length (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      // We will have caught a type mismatch above, so if this fails,
      // it must be for some other reason.
      if ((this->any_ >>= CORBA::Any::to_string (retval, bound)) == 0)
        {
          ACE_THROW_RETURN (DynamicAny::DynAny::InvalidValue (),
                            0);
        }

      return CORBA::string_dup (retval);
    }
}

CORBA::Object_ptr
TAO_DynCommon::get_reference (ACE_ENV_SINGLE_ARG_DECL)
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
      DynamicAny::DynAny_var cc = this->check_component (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (CORBA::Object::_nil ());

      return cc->get_reference (ACE_ENV_SINGLE_ARG_PARAMETER);
    }
  else
    {
      CORBA::Object_var retval;

      if ((this->any_ >>= CORBA::Any::to_object (retval.inout ())) == 0)
        {
          ACE_THROW_RETURN (DynamicAny::DynAny::TypeMismatch (),
                            CORBA::Object::_nil ());
        }

      return retval._retn ();
    }
}

CORBA::TypeCode_ptr
TAO_DynCommon::get_typecode (ACE_ENV_SINGLE_ARG_DECL)
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
      DynamicAny::DynAny_var cc = this->check_component (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      return cc->get_typecode (ACE_ENV_SINGLE_ARG_PARAMETER);
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
TAO_DynCommon::get_longlong (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  CORBA::LongLong retval = ACE_CDR_LONGLONG_INITIALIZER;

  if (this->destroyed_)
    {
      ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                        retval);
    }

  if (this->has_components_)
    {
      DynamicAny::DynAny_var cc = this->check_component (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (retval);

      return cc->get_longlong (ACE_ENV_SINGLE_ARG_PARAMETER);
    }
  else
    {
      if ((this->any_ >>= retval) == 0)
        {
          ACE_THROW_RETURN (DynamicAny::DynAny::TypeMismatch (),
                            retval);
        }

      return retval;
    }
}

CORBA::ULongLong
TAO_DynCommon::get_ulonglong (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  CORBA::ULongLong retval;

  if (this->destroyed_)
    {
      ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                        retval);
    }

  if (this->has_components_)
    {
      DynamicAny::DynAny_var cc = this->check_component (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (retval);

      return cc->get_ulonglong (ACE_ENV_SINGLE_ARG_PARAMETER);
    }
  else
    {
      if ((this->any_ >>= retval) == 0)
        {
          ACE_THROW_RETURN (DynamicAny::DynAny::TypeMismatch (),
                            retval);
        }

      return retval;
    }
}

CORBA::LongDouble
TAO_DynCommon::get_longdouble (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  CORBA::LongDouble retval = ACE_CDR_LONG_DOUBLE_INITIALIZER;

  if (this->destroyed_)
    {
      ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                        retval);
    }

  if (this->has_components_)
    {
      DynamicAny::DynAny_var cc = this->check_component (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (retval);

      return cc->get_longdouble (ACE_ENV_SINGLE_ARG_PARAMETER);
    }
  else
    {
      if ((this->any_ >>= retval) == 0)
        {
          ACE_THROW_RETURN (DynamicAny::DynAny::TypeMismatch (),
                            retval);
        }

      return retval;
    }
}

CORBA::WChar
TAO_DynCommon::get_wchar (ACE_ENV_SINGLE_ARG_DECL)
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
      DynamicAny::DynAny_var cc = this->check_component (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      return cc->get_wchar (ACE_ENV_SINGLE_ARG_PARAMETER);
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
TAO_DynCommon::get_wstring (ACE_ENV_SINGLE_ARG_DECL)
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
      DynamicAny::DynAny_var cc = this->check_component (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      return cc->get_wstring (ACE_ENV_SINGLE_ARG_PARAMETER);
    }
  else
    {
      // @@@ (JP) Someday try to find a way to avoid checking for
      // type code equivalence twice without risking a throw of
      // BadKind.
      CORBA::TypeCode_var unaliased_tc =
        this->check_type_and_unalias (CORBA::_tc_wstring
                                      ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      CORBA::WChar *retval = 0;

      CORBA::ULong bound = unaliased_tc->length (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      (void) (this->any_ >>= CORBA::Any::to_wstring (retval,
                                                     bound));

      return CORBA::wstring_dup (retval);
    }
}

CORBA::Any_ptr
TAO_DynCommon::get_any (ACE_ENV_SINGLE_ARG_DECL)
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
      DynamicAny::DynAny_var cc = this->check_component (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      return cc->get_any (ACE_ENV_SINGLE_ARG_PARAMETER);
    }
  else
    {
      const CORBA::Any *tmp = 0;

      if ((this->any_ >>= tmp) == 0)
        {
          ACE_THROW_RETURN (DynamicAny::DynAny::TypeMismatch (),
                            0);
        }

      CORBA::Any *retval = 0;
      ACE_NEW_RETURN (retval,
                      CORBA::Any (*tmp),
                      0);
      return retval;
    }
}

// @@@ (JP) TODO - optimize - this version was intended by the OMG to
// have fewer Any/DynAny conversions than get_any, not more.
DynamicAny::DynAny_ptr
TAO_DynCommon::get_dyn_any (ACE_ENV_SINGLE_ARG_DECL)
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

  CORBA::Any_var any = this->get_any (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return TAO_DynAnyFactory::make_dyn_any (any.in ()
                                          ACE_ENV_ARG_PARAMETER);
}

CORBA::ValueBase *
TAO_DynCommon::get_val (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (),
                    0);
}

// ****************************************************************

CORBA::Boolean
TAO_DynCommon::seek (CORBA::Long slot
                     ACE_ENV_ARG_DECL)
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
TAO_DynCommon::rewind (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW (CORBA::OBJECT_NOT_EXIST ());
    }

  (void) this->seek (0
                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

CORBA::Boolean
TAO_DynCommon::next (ACE_ENV_SINGLE_ARG_DECL)
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
TAO_DynCommon::copy (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                        DynamicAny::DynAny::_nil ());
    }

  CORBA::Any_var any = this->to_any (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (DynamicAny::DynAny::_nil ());

  DynamicAny::DynAny_ptr retval =
    TAO_DynAnyFactory::make_dyn_any (any.in ()
                                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (DynamicAny::DynAny::_nil ());

  return retval;
}

CORBA::ULong
TAO_DynCommon::component_count (ACE_ENV_SINGLE_ARG_DECL)
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
                         CORBA::Boolean destroying
                         ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  CORBA::TypeCode_var tc = component->type (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::TCKind tk = TAO_DynAnyFactory::unalias (tc.in ()
                                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  switch (tk)
  {
    case CORBA::tk_array:
      {
        TAO_DynArray_i *tmp = TAO_DynArray_i::_narrow (component
                                                       ACE_ENV_ARG_PARAMETER);
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
        TAO_DynEnum_i *tmp = TAO_DynEnum_i::_narrow (component
                                                     ACE_ENV_ARG_PARAMETER);
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
        TAO_DynSequence_i *tmp = TAO_DynSequence_i::_narrow (component
                                                             ACE_ENV_ARG_PARAMETER);
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
        TAO_DynStruct_i *tmp = TAO_DynStruct_i::_narrow (component
                                                         ACE_ENV_ARG_PARAMETER);
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
        TAO_DynUnion_i *tmp = TAO_DynUnion_i::_narrow (component
                                                       ACE_ENV_ARG_PARAMETER);
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
    case CORBA::tk_fixed:
    case CORBA::tk_value:
    case CORBA::tk_value_box:
      ACE_THROW (CORBA::NO_IMPLEMENT ());
    default:
      {
        TAO_DynAny_i *tmp = TAO_DynAny_i::_narrow (component
                                                   ACE_ENV_ARG_PARAMETER);
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
TAO_DynCommon::check_component (ACE_ENV_SINGLE_ARG_DECL)
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

  DynamicAny::DynAny_var cc = this->current_component (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (DynamicAny::DynAny::_nil ());

  CORBA::TypeCode_var tc = cc->type (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (DynamicAny::DynAny::_nil ());

  CORBA::TCKind kind = TAO_DynAnyFactory::unalias (tc.in ()
                                                   ACE_ENV_ARG_PARAMETER);
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
TAO_DynCommon::check_type (CORBA::TypeCode_ptr tc
                           ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch
    ))
{
  CORBA::Boolean equivalent = this->type_->equivalent (tc
                                                       ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (!equivalent)
    {
      ACE_THROW (DynamicAny::DynAny::TypeMismatch ());
    }
}

CORBA::TypeCode_ptr
TAO_DynCommon::check_type_and_unalias (CORBA::TypeCode_ptr tc
                                       ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch
    ))
{
  this->check_type (tc
                    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  return TAO_DynAnyFactory::strip_alias (tc
                                         ACE_ENV_ARG_PARAMETER);
}
