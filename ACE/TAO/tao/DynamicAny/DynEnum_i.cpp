// $Id$

#include "tao/AnyTypeCode/Any_Unknown_IDL_Type.h"
#include "tao/AnyTypeCode/AnyTypeCode_methods.h"
#include "tao/DynamicAny/DynEnum_i.h"
#include "tao/DynamicAny/DynAnyFactory.h"
#include "tao/CDR.h"

#include "ace/OS_NS_string.h"


ACE_RCSID (DynamicAny,
           DynEnum_i,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_DynEnum_i::TAO_DynEnum_i (void)
{
}

TAO_DynEnum_i::~TAO_DynEnum_i (void)
{
}

void
TAO_DynEnum_i::init_common (void)
{
  this->ref_to_component_ = 0;
  this->container_is_destroying_ = 0;
  this->has_components_ = 0;
  this->destroyed_ = 0;
  this->current_position_ = -1;
  this->component_count_ = 0;
}

void
TAO_DynEnum_i::init (const CORBA::Any &any
                     ACE_ENV_ARG_DECL)
{
  CORBA::TypeCode_var tc = any.type ();

  CORBA::TCKind kind =
    TAO_DynAnyFactory::unalias (tc.in ()
                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (kind != CORBA::tk_enum)
    {
      ACE_THROW (DynamicAny::DynAnyFactory::InconsistentTypeCode ());
    }

  this->type_ = tc;

  TAO::Any_Impl *impl = any.impl ();

  if (impl->encoded ())
    {
      TAO::Unknown_IDL_Type *unk =
        dynamic_cast<TAO::Unknown_IDL_Type *> (impl);

      // We don't want unk's rd_ptr to move, in case we are shared by
      // another Any, so we use this to copy the state, not the buffer.
      TAO_InputCDR for_reading (unk->_tao_get_cdr ());
      for_reading.read_ulong (this->value_);
    }
  else
    {
      TAO_OutputCDR out;
      impl->marshal_value (out);
      TAO_InputCDR in (out);
      in.read_ulong (this->value_);
    }

  this->init_common ();
}

void
TAO_DynEnum_i::init (CORBA::TypeCode_ptr tc
                     ACE_ENV_ARG_DECL)
{
  CORBA::TCKind kind = TAO_DynAnyFactory::unalias (tc
                                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (kind != CORBA::tk_enum)
    {
      ACE_THROW (DynamicAny::DynAnyFactory::InconsistentTypeCode ());
    }

  this->type_ = CORBA::TypeCode::_duplicate (tc);

  this->value_ = 0;

  this->init_common ();
}

// ****************************************************************

TAO_DynEnum_i *
TAO_DynEnum_i::_narrow (CORBA::Object_ptr _tao_objref
                        ACE_ENV_ARG_DECL_NOT_USED)
{
  if (CORBA::is_nil (_tao_objref))
    {
      return 0;
    }

  return dynamic_cast<TAO_DynEnum_i *> (_tao_objref);
}

// ****************************************************************

char *
TAO_DynEnum_i::get_as_string (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  const char *retval =
    this->type_.in ()->member_name (this->value_
                                    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return CORBA::string_dup (retval);
}

void
TAO_DynEnum_i::set_as_string (const char *value_as_string
                              ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::InvalidValue
    ))
{
  CORBA::ULong count =
    this->type_.in ()->member_count (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::ULong i;
  const char *temp = 0;

  for (i = 0; i < count; ++i)
    {
      temp = this->type_.in ()->member_name (i
                                             ACE_ENV_ARG_PARAMETER);
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
      ACE_THROW (DynamicAny::DynAny::InvalidValue ());
    }
}

CORBA::ULong
TAO_DynEnum_i::get_as_ulong (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  return this->value_;
}

void
TAO_DynEnum_i::set_as_ulong (CORBA::ULong value_as_ulong
                             ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::InvalidValue
    ))
{
  CORBA::ULong max =
    this->type_.in ()->member_count (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (value_as_ulong < max)
    {
      this->value_ = value_as_ulong;
    }
  else
    {
      ACE_THROW (DynamicAny::DynAny::InvalidValue ());
    }
}

// ****************************************************************

void
TAO_DynEnum_i::from_any (const CORBA::Any& any
                         ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  CORBA::TypeCode_var tc = any.type ();
  CORBA::TCKind kind = TAO_DynAnyFactory::unalias (tc.in ()
                                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (kind == CORBA::tk_enum)
    {
      // Get the CDR stream of the Any, if there isn't one, make one.
      TAO::Any_Impl *impl = any.impl ();

      if (impl->encoded ())
        {
          TAO::Unknown_IDL_Type *unk =
            dynamic_cast<TAO::Unknown_IDL_Type *> (impl);

          // We don't want unk's rd_ptr to move, in case we are shared by
          // another Any, so we use this to copy the state, not the buffer.
          TAO_InputCDR for_reading (unk->_tao_get_cdr ());
          for_reading.read_ulong (this->value_);
        }
      else
        {
          TAO_OutputCDR out;
          impl->marshal_value (out);
          TAO_InputCDR in (out);
          in.read_ulong (this->value_);
        }
    }
  else
    {
      ACE_THROW (DynamicAny::DynAny::TypeMismatch ());
    }
}

CORBA::Any_ptr
TAO_DynEnum_i::to_any (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  TAO_OutputCDR out_cdr;

  out_cdr.write_ulong (this->value_);

  CORBA::Any *retval;
  ACE_NEW_THROW_EX (retval,
                    CORBA::Any,
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  TAO_InputCDR in_cdr (out_cdr);
  TAO::Unknown_IDL_Type *unk = 0;
  ACE_NEW_THROW_EX (unk,
                    TAO::Unknown_IDL_Type (this->type_.in (),
                                           in_cdr),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  retval->replace (unk);
  return retval;
}

CORBA::Boolean
TAO_DynEnum_i::equal (DynamicAny::DynAny_ptr rhs
                      ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  CORBA::TypeCode_var tc = rhs->type (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  CORBA::Boolean equivalent = tc->equivalent (this->type_.in ()
                                              ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  if (!equivalent)
    {
      return 0;
    }

  CORBA::Any_var any = rhs->to_any (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  TAO::Any_Impl *impl = any->impl ();
  CORBA::ULong value;

  if (impl->encoded ())
    {
      TAO::Unknown_IDL_Type *unk =
        dynamic_cast<TAO::Unknown_IDL_Type *> (impl);

      // We don't want unk's rd_ptr to move, in case we are shared by
      // another Any, so we use this to copy the state, not the buffer.
      TAO_InputCDR for_reading (unk->_tao_get_cdr ());
      for_reading.read_ulong (value);
    }
  else
    {
      TAO_OutputCDR out;
      impl->marshal_value (out);
      TAO_InputCDR in (out);
      in.read_ulong (value);
    }

  return value == this->value_;
}

void
TAO_DynEnum_i::destroy (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW (CORBA::OBJECT_NOT_EXIST ());
    }

  if (!this->ref_to_component_ || this->container_is_destroying_)
    {
      this->destroyed_ = 1;
    }
}

DynamicAny::DynAny_ptr
TAO_DynEnum_i::current_component (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                        DynamicAny::DynAny::_nil ());
    }

  ACE_THROW_RETURN (DynamicAny::DynAny::TypeMismatch (),
                    DynamicAny::DynAny::_nil ());
}

TAO_END_VERSIONED_NAMESPACE_DECL
