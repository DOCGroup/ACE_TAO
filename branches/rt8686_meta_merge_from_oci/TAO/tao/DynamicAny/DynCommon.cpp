// $Id$

#include "tao/DynamicAny/DynCommon.h"

#include "tao/DynamicAny/DynAnyFactory.h"
#include "tao/DynamicAny/DynAny_i.h"
#include "tao/DynamicAny/DynArray_i.h"
#include "tao/DynamicAny/DynEnum_i.h"
#include "tao/DynamicAny/DynSequence_i.h"
#include "tao/DynamicAny/DynStruct_i.h"
#include "tao/DynamicAny/DynUnion_i.h"
#include "tao/DynamicAny/DynAnyUtils_T.h"

#include "tao/AnyTypeCode/Any_Unknown_IDL_Type.h"
#include "tao/AnyTypeCode/AnyTypeCode_methods.h"

#include "tao/CDR.h"

#include "ace/OS_NS_wchar.h"
#include "ace/OS_NS_string.h"

ACE_RCSID (DynamicAny,
           DynCommon,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_DynCommon::TAO_DynCommon (void)
{
}

TAO_DynCommon::~TAO_DynCommon (void)
{
}

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
                               ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  TAO::DynAnyBasicTypeUtils<CORBA::Boolean>::insert_value (value,
                                                           this);
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
  TAO::DynAnyBasicTypeUtils<CORBA::Octet>::insert_value (value,
                                                         this);
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
  TAO::DynAnyBasicTypeUtils<CORBA::Char>::insert_value (value,
                                                        this);
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
  TAO::DynAnyBasicTypeUtils<CORBA::Short>::insert_value (value,
                                                         this);
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
  TAO::DynAnyBasicTypeUtils<CORBA::UShort>::insert_value (value,
                                                          this);
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
  TAO::DynAnyBasicTypeUtils<CORBA::Long>::insert_value (value,
                                                        this);
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
  TAO::DynAnyBasicTypeUtils<CORBA::ULong>::insert_value (value,
                                                         this);
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
  TAO::DynAnyBasicTypeUtils<CORBA::Float>::insert_value (value,
                                                         this);
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
  TAO::DynAnyBasicTypeUtils<CORBA::Double>::insert_value (value,
                                                          this);
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
      DynamicAny::DynAny_var cc =
        this->check_component (ACE_ENV_SINGLE_ARG_PARAMETER);
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

      CORBA::TCKind kind =
        unaliased_tc->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      if (kind != CORBA::tk_string)
        {
          ACE_THROW (DynamicAny::DynAny::TypeMismatch ());
        }

      CORBA::ULong bound =
        unaliased_tc->length (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      if (bound > 0 && bound < ACE_OS::strlen (value))
        {
          ACE_THROW (DynamicAny::DynAny::InvalidValue ());
        }

      this->any_ <<= CORBA::Any::from_string (const_cast<char *> (value),
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
      DynamicAny::DynAny_var cc =
        this->check_component (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      cc->insert_reference (value
                            ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
  else
    {
      CORBA::Boolean good_type = true;
      CORBA::TCKind kind =
        TAO_DynAnyFactory::unalias (this->type_.in ()
                                    ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      if (kind != CORBA::tk_objref)
        {
          good_type = false;
        }
      else if (!CORBA::is_nil (value))
        {
          const char *value_id = value->_interface_repository_id ();

          if (ACE_OS::strcmp (value_id, "IDL:omg.org/CORBA/Object:1.0") != 0)
            {
              const char *my_id =
                this->type_->id (ACE_ENV_SINGLE_ARG_PARAMETER);
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

          if (CORBA::is_nil (value))
            {
              // Empty type hint, no profile.
              cdr.write_ulong (1);
              cdr.write_char ('\0');
              cdr.write_ulong (0);
            }
          else
            {
              if (!value->marshal (cdr))
                {
                  ACE_THROW (DynamicAny::DynAny::InvalidValue ());
                }
            }

          TAO_InputCDR in (cdr);
          TAO::Unknown_IDL_Type *unk = 0;
          ACE_NEW (unk,
                   TAO::Unknown_IDL_Type (this->type_.in (),
                                          in));
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
  TAO::DynAnyBasicTypeUtils<CORBA::TypeCode_ptr>::insert_value (value,
                                                                this);
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
  TAO::DynAnyBasicTypeUtils<CORBA::LongLong>::insert_value (value,
                                                            this);
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
  TAO::DynAnyBasicTypeUtils<CORBA::ULongLong>::insert_value (value,
                                                             this);
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
  TAO::DynAnyBasicTypeUtils<CORBA::LongDouble>::insert_value (value,
                                                              this);
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
  TAO::DynAnyBasicTypeUtils<CORBA::WChar>::insert_value (value,
                                                         this);
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
      DynamicAny::DynAny_var cc =
        this->check_component (ACE_ENV_SINGLE_ARG_PARAMETER);
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

      this->any_ <<= CORBA::Any::from_wstring (const_cast<CORBA::WChar *> (value),
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
  TAO::DynAnyBasicTypeUtils<CORBA::Any>::insert_value (value,
                                                       this);
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
TAO_DynCommon::insert_val (CORBA::ValueBase *value
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
      DynamicAny::DynAny_var cc =
        this->check_component (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      cc->insert_val (value
                      ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
  else
    {
      CORBA::TCKind kind =
        TAO_DynAnyFactory::unalias (this->type_.in ()
                                    ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      if (kind != CORBA::tk_value)
        {
          ACE_THROW (DynamicAny::DynAny::TypeMismatch ());
        }
      
      // If the arg is 0 we can't do this kind of type checking,
      // and the call to _tao_marshal() below will handle the
      // null value correctly.
      if (value != 0)
        {
          const char *value_id = value->_tao_obv_repository_id ();
          const char *my_id =
            this->type_->id (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK;

          // Valuetypes, unlike interfaces, don't have a virtual
          // method to check for a more derived type when the
          // repo ids don't match. Valuetypes have only
          // a static _downcast() method, which can't be used
          // here, so if the repo ids don't match, we're hosed.
          if (ACE_OS::strcmp (value_id, my_id) != 0)
            {
              ACE_THROW (DynamicAny::DynAny::TypeMismatch ());
            }
        }

      // This will handle null values correctly, or otherwise
      // make a virtual call to marshal the state.
      TAO_OutputCDR out;
      CORBA::Boolean good_insert =
        CORBA::ValueBase::_tao_marshal (out, value);
      
      if (!good_insert)
        {
          ACE_THROW (DynamicAny::DynAny::InvalidValue ());
        }

      TAO_InputCDR in (out);
      TAO::Unknown_IDL_Type *unk = 0;
      ACE_NEW (unk,
               TAO::Unknown_IDL_Type (this->type_.in (),
                                      in));
      this->any_.replace (unk);
    }
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
  return TAO::DynAnyBasicTypeUtils<CORBA::Boolean>::get_value (this);
}

CORBA::Octet
TAO_DynCommon::get_octet (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  return TAO::DynAnyBasicTypeUtils<CORBA::Octet>::get_value (this);
}

CORBA::Char
TAO_DynCommon::get_char (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  return TAO::DynAnyBasicTypeUtils<CORBA::Char>::get_value (this);
}

CORBA::Short
TAO_DynCommon::get_short (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  return TAO::DynAnyBasicTypeUtils<CORBA::Short>::get_value (this);
}

CORBA::UShort
TAO_DynCommon::get_ushort (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  return TAO::DynAnyBasicTypeUtils<CORBA::UShort>::get_value (this);
}

CORBA::Long
TAO_DynCommon::get_long (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  return TAO::DynAnyBasicTypeUtils<CORBA::Long>::get_value (this);
}

CORBA::ULong
TAO_DynCommon::get_ulong (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  return TAO::DynAnyBasicTypeUtils<CORBA::ULong>::get_value (this);
}

CORBA::Float
TAO_DynCommon::get_float (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  return TAO::DynAnyBasicTypeUtils<CORBA::Float>::get_value (this);
}

CORBA::Double
TAO_DynCommon::get_double (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  return TAO::DynAnyBasicTypeUtils<CORBA::Double>::get_value (this);
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
      DynamicAny::DynAny_var cc =
        this->check_component (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      return cc->get_string (ACE_ENV_SINGLE_ARG_PARAMETER);
    }
  else
    {
      CORBA::TypeCode_var unaliased_tc =
        TAO_DynAnyFactory::strip_alias (this->type_.in ()
                                        ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      CORBA::TCKind kind =
        unaliased_tc->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      if (kind != CORBA::tk_string)
        {
          ACE_THROW_RETURN (DynamicAny::DynAny::TypeMismatch (),
                            0);
        }

      char *retval = 0;

      CORBA::ULong bound =
        unaliased_tc->length (ACE_ENV_SINGLE_ARG_PARAMETER);
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
      DynamicAny::DynAny_var cc =
        this->check_component (ACE_ENV_SINGLE_ARG_PARAMETER);
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
      DynamicAny::DynAny_var cc =
        this->check_component (ACE_ENV_SINGLE_ARG_PARAMETER);
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
  return TAO::DynAnyBasicTypeUtils<CORBA::LongLong>::get_value (this);
}

CORBA::ULongLong
TAO_DynCommon::get_ulonglong (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  return TAO::DynAnyBasicTypeUtils<CORBA::ULongLong>::get_value (this);
}

CORBA::LongDouble
TAO_DynCommon::get_longdouble (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  return TAO::DynAnyBasicTypeUtils<CORBA::LongDouble>::get_value (this);
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
      DynamicAny::DynAny_var cc =
        this->check_component (ACE_ENV_SINGLE_ARG_PARAMETER);
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
      DynamicAny::DynAny_var cc =
        this->check_component (ACE_ENV_SINGLE_ARG_PARAMETER);
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

      CORBA::ULong bound =
        unaliased_tc->length (ACE_ENV_SINGLE_ARG_PARAMETER);
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
      DynamicAny::DynAny_var cc =
        this->check_component (ACE_ENV_SINGLE_ARG_PARAMETER);
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

  return 
    TAO::MakeDynAnyUtils<const CORBA::Any&>::make_dyn_any_t (
      any.in ()._tao_get_typecode (),
      any.in ());
}

CORBA::ValueBase *
TAO_DynCommon::get_val (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (), 0);
    }

  if (this->has_components_)
    {
      DynamicAny::DynAny_var cc =
        this->check_component (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      return cc->get_val (ACE_ENV_SINGLE_ARG_PARAMETER);
    }
  else
    {
      CORBA::TCKind kind =
        TAO_DynAnyFactory::unalias (this->type_.in ()
                                    ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
      
      if (kind != CORBA::tk_value)
        {
          ACE_THROW_RETURN (DynamicAny::DynAny::TypeMismatch (), 0);
        }

      CORBA::ValueBase_var retval;
      TAO::Any_Impl *any_impl = this->any_.impl ();
      
      if (any_impl == 0)
        {
          ACE_THROW_RETURN (DynamicAny::DynAny::InvalidValue (), 0);
        }
      
      // This should always be non-zero for dynamic anys.  
      TAO::Unknown_IDL_Type *unk =
        dynamic_cast<TAO::Unknown_IDL_Type *> (any_impl);
        
      // Demarshal from a copy so we can retain ownership of our contents.  
      TAO_InputCDR for_reading (unk->_tao_get_cdr ());
        
      CORBA::Boolean good_extract =
        CORBA::ValueBase::_tao_unmarshal (for_reading,
                                          retval.inout ());
        
      if (!good_extract)
        {
          ACE_THROW_RETURN (DynamicAny::DynAny::InvalidValue (), 0);
        }  
      
      return retval._retn ();
    }
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
  else if (slot < 0 || slot >= static_cast<CORBA::Long> (this->component_count_))
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
      || this->current_position_ + 1 >= static_cast<CORBA::Long> (this->component_count_
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
    TAO::MakeDynAnyUtils<const CORBA::Any&>::make_dyn_any_t (
      any.in ()._tao_get_typecode (),
      any.in ());

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

// ****************************************************************

void
TAO_DynCommon::insert_abstract (CORBA::AbstractBase_ptr value
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
      DynamicAny::DynAny_var cc =
        this->check_component (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      cc->insert_abstract (value
                           ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
  else
    {
      CORBA::Boolean good_type = true;
      CORBA::TCKind kind =
        TAO_DynAnyFactory::unalias (this->type_.in ()
                                    ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      if (kind != CORBA::tk_abstract_interface)
        {
          good_type = false;
        }
      else if (!CORBA::is_nil (value))
        {
          const char *value_id = value->_interface_repository_id ();
          int cmp =
            ACE_OS::strcmp (value_id, "IDL:omg.org/CORBA/AbstractBase:1.0");

          if (cmp != 0)
            {
              const char *my_id =
                this->type_->id (ACE_ENV_SINGLE_ARG_PARAMETER);
              ACE_CHECK;

              if (ACE_OS::strcmp (value_id, my_id) != 0)
                {
                  // If 'value' is an objref, this will be a virtual
                  // call. If not, it will just compare to the repo id
                  // above, which we already know won't match. We see
                  // in insert_val() (see comment there) that there
                  // is no way to check for derived valuetypes w/o
                  // type knowledge at compile time.
                  good_type = value->_is_a (my_id
                                            ACE_ENV_ARG_PARAMETER);
                  ACE_CHECK;
                }
            }
        }

      if (good_type)
        {
          TAO_OutputCDR out;
          
          // The CDR insertion operator for AbstractBase handles
          // all cases - nil, objref and valuetype - polymorphically.
          if (!(out << value))
            {
              ACE_THROW (DynamicAny::DynAny::InvalidValue ());
            }

          TAO_InputCDR in (out);
          TAO::Unknown_IDL_Type *unk = 0;
          ACE_NEW (unk,
                   TAO::Unknown_IDL_Type (this->type_.in (),
                                          in));
          this->any_.replace (unk);
        }
      else
        {
          ACE_THROW (DynamicAny::DynAny::TypeMismatch ());
        }
    }
}

CORBA::AbstractBase_ptr
TAO_DynCommon::get_abstract (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                        CORBA::AbstractBase::_nil ());
    }

  if (this->has_components_)
    {
      DynamicAny::DynAny_var cc =
        this->check_component (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (CORBA::AbstractBase::_nil ());

      return cc->get_abstract (ACE_ENV_SINGLE_ARG_PARAMETER);
    }
  else
    {
      CORBA::TCKind kind =
        TAO_DynAnyFactory::unalias (this->type_.in ()
                                    ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
      
      if (kind != CORBA::tk_abstract_interface)
        {
          ACE_THROW_RETURN (DynamicAny::DynAny::TypeMismatch (), 0);
        }

      CORBA::AbstractBase_var retval;
      TAO::Any_Impl *any_impl = this->any_.impl ();
      
      if (any_impl == 0)
        {
          ACE_THROW_RETURN (DynamicAny::DynAny::InvalidValue (), 0);
        }
      
      // This should always be non-zero for dynamic anys.  
      TAO::Unknown_IDL_Type *unk =
        dynamic_cast<TAO::Unknown_IDL_Type *> (any_impl);
        
      // Make a copy to extract from so we retain ownership of contents.  
      TAO_InputCDR for_reading (unk->_tao_get_cdr ());
       
      // The CDR extraction operator for AbstractBase handles
      // all cases.  
      if (!(for_reading >> retval.inout ()))
        {
          ACE_THROW_RETURN (DynamicAny::DynAny::InvalidValue (), 0);
        }  
      
      return retval._retn ();
    }
}

// ****************************************************************

void
TAO_DynCommon::insert_boolean_seq (
    const CORBA::BooleanSeq &value
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  TAO::DynAnyBasicTypeUtils<CORBA::BooleanSeq>::insert_value (value,
                                                              this);
}
  
void
TAO_DynCommon::insert_octet_seq (
    const CORBA::OctetSeq &value
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  TAO::DynAnyBasicTypeUtils<CORBA::OctetSeq>::insert_value (value,
                                                            this);
}

void
TAO_DynCommon::insert_char_seq (
    const CORBA::CharSeq &value
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  TAO::DynAnyBasicTypeUtils<CORBA::CharSeq>::insert_value (value,
                                                           this);
}

void
TAO_DynCommon::insert_short_seq (
    const CORBA::ShortSeq &value
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  TAO::DynAnyBasicTypeUtils<CORBA::ShortSeq>::insert_value (value,
                                                            this);
}

void
TAO_DynCommon::insert_ushort_seq (
    const CORBA::UShortSeq &value
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  TAO::DynAnyBasicTypeUtils<CORBA::UShortSeq>::insert_value (value,
                                                             this);
}

void
TAO_DynCommon::insert_long_seq (
    const CORBA::LongSeq &value
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  TAO::DynAnyBasicTypeUtils<CORBA::LongSeq>::insert_value (value,
                                                           this);
}

void
TAO_DynCommon::insert_ulong_seq (
    const CORBA::ULongSeq &value
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  TAO::DynAnyBasicTypeUtils<CORBA::ULongSeq>::insert_value (value,
                                                            this);
}

void
TAO_DynCommon::insert_float_seq (
    const CORBA::FloatSeq &value
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  TAO::DynAnyBasicTypeUtils<CORBA::FloatSeq>::insert_value (value,
                                                            this);
}

void
TAO_DynCommon::insert_double_seq (
    const CORBA::DoubleSeq &value
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  TAO::DynAnyBasicTypeUtils<CORBA::DoubleSeq>::insert_value (value,
                                                             this);
}

void
TAO_DynCommon::insert_longlong_seq (
    const CORBA::LongLongSeq &value
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  TAO::DynAnyBasicTypeUtils<CORBA::LongLongSeq>::insert_value (value,
                                                               this);
}

void
TAO_DynCommon::insert_ulonglong_seq (
    const CORBA::ULongLongSeq &value
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  TAO::DynAnyBasicTypeUtils<CORBA::ULongLongSeq>::insert_value (value,
                                                                this);
}

void
TAO_DynCommon::insert_longdouble_seq (
    const CORBA::LongDoubleSeq &value
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  TAO::DynAnyBasicTypeUtils<CORBA::LongDoubleSeq>::insert_value (value,
                                                                 this);
}

void
TAO_DynCommon::insert_wchar_seq (
    const CORBA::WCharSeq &value
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  TAO::DynAnyBasicTypeUtils<CORBA::WCharSeq>::insert_value (value,
                                                            this);
}

// ****************************************************************

CORBA::BooleanSeq *
TAO_DynCommon::get_boolean_seq (
    ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  CORBA::BooleanSeq *owned =
    TAO::DynAnyBasicTypeUtils<CORBA::BooleanSeq>::get_value (this);
  return new CORBA::BooleanSeq (*owned);
}

CORBA::OctetSeq *
TAO_DynCommon::get_octet_seq (
    ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  CORBA::OctetSeq *owned =
    TAO::DynAnyBasicTypeUtils<CORBA::OctetSeq>::get_value (this);
  return new CORBA::OctetSeq (*owned);
}

CORBA::CharSeq *
TAO_DynCommon::get_char_seq (
    ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  CORBA::CharSeq *owned =
    TAO::DynAnyBasicTypeUtils<CORBA::CharSeq>::get_value (this);
  return new CORBA::CharSeq (*owned);
}

CORBA::ShortSeq *
TAO_DynCommon::get_short_seq (
    ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  CORBA::ShortSeq *owned =
    TAO::DynAnyBasicTypeUtils<CORBA::ShortSeq>::get_value (this);
  return new CORBA::ShortSeq (*owned);
}

CORBA::UShortSeq *
TAO_DynCommon::get_ushort_seq (
    ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  CORBA::UShortSeq *owned =
    TAO::DynAnyBasicTypeUtils<CORBA::UShortSeq>::get_value (this);
  return new CORBA::UShortSeq (*owned);
}

CORBA::LongSeq *
TAO_DynCommon::get_long_seq (
    ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  CORBA::LongSeq *owned =
    TAO::DynAnyBasicTypeUtils<CORBA::LongSeq>::get_value (this);
  return new CORBA::LongSeq (*owned);
}

CORBA::ULongSeq *
TAO_DynCommon::get_ulong_seq (
    ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  CORBA::ULongSeq *owned =
    TAO::DynAnyBasicTypeUtils<CORBA::ULongSeq>::get_value (this);
  return new CORBA::ULongSeq (*owned);
}

CORBA::FloatSeq *
TAO_DynCommon::get_float_seq (
    ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  CORBA::FloatSeq *owned =
    TAO::DynAnyBasicTypeUtils<CORBA::FloatSeq>::get_value (this);
  return new CORBA::FloatSeq (*owned);
}

CORBA::DoubleSeq *
TAO_DynCommon::get_double_seq (
    ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  CORBA::DoubleSeq *owned =
    TAO::DynAnyBasicTypeUtils<CORBA::DoubleSeq>::get_value (this);
  return new CORBA::DoubleSeq (*owned);
}

CORBA::LongLongSeq *
TAO_DynCommon::get_longlong_seq (
    ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  CORBA::LongLongSeq *owned =
    TAO::DynAnyBasicTypeUtils<CORBA::LongLongSeq>::get_value (this);
  return new CORBA::LongLongSeq (*owned);
}

CORBA::ULongLongSeq *
TAO_DynCommon::get_ulonglong_seq (
    ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  CORBA::ULongLongSeq *owned =
    TAO::DynAnyBasicTypeUtils<CORBA::ULongLongSeq>::get_value (this);
  return new CORBA::ULongLongSeq (*owned);
}

CORBA::LongDoubleSeq *
TAO_DynCommon::get_longdouble_seq (
    ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  CORBA::LongDoubleSeq *owned =
    TAO::DynAnyBasicTypeUtils<CORBA::LongDoubleSeq>::get_value (this);
  return new CORBA::LongDoubleSeq (*owned);
}

CORBA::WCharSeq *
TAO_DynCommon::get_wchar_seq (
    ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  CORBA::WCharSeq *owned =
    TAO::DynAnyBasicTypeUtils<CORBA::WCharSeq>::get_value (this);
  return new CORBA::WCharSeq (*owned);
}

// ****************************************************************

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
      TAO::DynAnyFlagUtils<TAO_DynArray_i>::set_flag_t (component,
                                                        destroying);
      break;
    case CORBA::tk_enum:
      TAO::DynAnyFlagUtils<TAO_DynEnum_i>::set_flag_t (component,
                                                       destroying);
      break;
    case CORBA::tk_sequence:
      if (TAO_DynCommon::is_basic_type_seq (tc))
        {
          TAO::DynAnyFlagUtils<TAO_DynAny_i>::set_flag_t (component,
                                                          destroying);
        }
      else
        {
          TAO::DynAnyFlagUtils<TAO_DynSequence_i>::set_flag_t (component,
                                                               destroying);
        }
      break;
    case CORBA::tk_struct:
      TAO::DynAnyFlagUtils<TAO_DynStruct_i>::set_flag_t (component,
                                                         destroying);
      break;
    case CORBA::tk_union:
      TAO::DynAnyFlagUtils<TAO_DynUnion_i>::set_flag_t (component,
                                                        destroying);
      break;
    case CORBA::tk_fixed:
    case CORBA::tk_value:
    case CORBA::tk_value_box:
      ACE_THROW (CORBA::NO_IMPLEMENT ());
    default:
      TAO::DynAnyFlagUtils<TAO_DynAny_i>::set_flag_t (component,
                                                      destroying);
      break;
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

  DynamicAny::DynAny_var cc =
    this->current_component (ACE_ENV_SINGLE_ARG_PARAMETER);
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
    case CORBA::tk_struct:
    case CORBA::tk_union:
      ACE_THROW_RETURN (DynamicAny::DynAny::TypeMismatch (),
                        DynamicAny::DynAny::_nil ());
    case CORBA::tk_sequence:
      if (TAO_DynCommon::is_basic_type_seq (tc))
        {
          return cc._retn ();
        }
      else
        {
          ACE_THROW_RETURN (DynamicAny::DynAny::TypeMismatch (),
                            DynamicAny::DynAny::_nil ());
        }
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
  CORBA::Boolean equivalent =
    this->type_->equivalent (tc
                             ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (!equivalent)
    {
      ACE_THROW (DynamicAny::DynAny::TypeMismatch ());
    }
}

bool
TAO_DynCommon::is_basic_type_seq (CORBA::TypeCode_ptr tc)
{
  return (tc == CORBA::_tc_BooleanSeq
          || tc == CORBA::_tc_OctetSeq
          || tc == CORBA::_tc_CharSeq
          || tc == CORBA::_tc_WCharSeq
          || tc == CORBA::_tc_ShortSeq
          || tc == CORBA::_tc_UShortSeq
          || tc == CORBA::_tc_LongSeq
          || tc == CORBA::_tc_ULongSeq
          || tc == CORBA::_tc_LongLongSeq
          || tc == CORBA::_tc_ULongLongSeq
          || tc == CORBA::_tc_FloatSeq
          || tc == CORBA::_tc_DoubleSeq
          || tc == CORBA::_tc_LongDoubleSeq);
}

const CORBA::Boolean
TAO_DynCommon::has_components (void) const
{
  return this->has_components_;
}

const CORBA::Boolean
TAO_DynCommon::destroyed (void) const
{
  return this->destroyed_;
}

CORBA::Any &
TAO_DynCommon::the_any (void)
{
  return this->any_;
}

void
TAO_DynCommon::container_is_destroying (CORBA::Boolean val)
{
  this->container_is_destroying_ = val;
}

void
TAO_DynCommon::ref_to_component (CORBA::Boolean val)
{
  this->ref_to_component_ = val;
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

TAO_END_VERSIONED_NAMESPACE_DECL
