// -*- C++ -*-
// $Id$

#include "tao/AnyTypeCode/TypeCode.h"
#include "tao/AnyTypeCode/Any_Unknown_IDL_Type.h"
#include "tao/AnyTypeCode/AnyTypeCode_methods.h"
#include "tao/DynamicAny/DynEnum_i.h"
#include "tao/DynamicAny/DynAnyFactory.h"
#include "tao/CDR.h"

#include "ace/OS_NS_string.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_DynEnum_i::TAO_DynEnum_i (CORBA::Boolean allow_truncation)
  : TAO_DynCommon (allow_truncation)
  , value_ (0u)
{
}

TAO_DynEnum_i::~TAO_DynEnum_i (void)
{
}

void
TAO_DynEnum_i::init_common (void)
{
  this->ref_to_component_ = false;
  this->container_is_destroying_ = false;
  this->has_components_ = false;
  this->destroyed_ = false;
  this->current_position_ = -1;
  this->component_count_ = 0;
}

void
TAO_DynEnum_i::init (const CORBA::Any &any)
{
  CORBA::TypeCode_var tc = any.type ();
  CORBA::TCKind kind = TAO_DynAnyFactory::unalias (tc.in ());

  if (kind != CORBA::tk_enum)
    {
      throw DynamicAny::DynAnyFactory::InconsistentTypeCode ();
    }

  this->type_ = tc;

  TAO::Any_Impl *impl = any.impl ();

  if (impl->encoded ())
    {
      TAO::Unknown_IDL_Type * const unk =
        dynamic_cast<TAO::Unknown_IDL_Type *> (impl);

      if (!unk)
        throw CORBA::INTERNAL ();

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
TAO_DynEnum_i::init (CORBA::TypeCode_ptr tc)
{
  CORBA::TCKind kind = TAO_DynAnyFactory::unalias (tc);

  if (kind != CORBA::tk_enum)
    {
      throw DynamicAny::DynAnyFactory::InconsistentTypeCode ();
    }

  this->type_ = CORBA::TypeCode::_duplicate (tc);

  this->value_ = 0;

  this->init_common ();
}

// ****************************************************************

TAO_DynEnum_i *
TAO_DynEnum_i::_narrow (CORBA::Object_ptr _tao_objref)
{
  if (CORBA::is_nil (_tao_objref))
    {
      return 0;
    }

  return dynamic_cast<TAO_DynEnum_i *> (_tao_objref);
}

// ****************************************************************

char *
TAO_DynEnum_i::get_as_string (void)
{
  CORBA::TypeCode_var ct = TAO_DynAnyFactory::strip_alias (this->type_.in ());

  const char *retval = ct.in ()->member_name (this->value_);

  return CORBA::string_dup (retval);
}

void
TAO_DynEnum_i::set_as_string (const char *value_as_string)
{
  CORBA::TypeCode_var ct = TAO_DynAnyFactory::strip_alias (this->type_.in ());

  CORBA::ULong count = ct.in ()->member_count ();

  CORBA::ULong i;
  const char *temp = 0;

  for (i = 0; i < count; ++i)
    {
      temp = ct.in ()->member_name (i);

      if (!ACE_OS::strcmp (value_as_string, temp))
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
      throw DynamicAny::DynAny::InvalidValue ();
    }
}

CORBA::ULong
TAO_DynEnum_i::get_as_ulong (void)
{
  return this->value_;
}

void
TAO_DynEnum_i::set_as_ulong (CORBA::ULong value_as_ulong)
{
  CORBA::TypeCode_var ct = TAO_DynAnyFactory::strip_alias (this->type_.in ());
  CORBA::ULong const max = ct.in ()->member_count ();

  if (value_as_ulong < max)
    {
      this->value_ = value_as_ulong;
    }
  else
    {
      throw DynamicAny::DynAny::InvalidValue ();
    }
}

// ****************************************************************

void
TAO_DynEnum_i::from_any (const CORBA::Any& any)
{
  CORBA::TypeCode_var tc = any.type ();
  CORBA::TCKind kind = TAO_DynAnyFactory::unalias (tc.in ());

  if (kind == CORBA::tk_enum)
    {
      // Get the CDR stream of the Any, if there isn't one, make one.
      TAO::Any_Impl *impl = any.impl ();

      if (impl->encoded ())
        {
          TAO::Unknown_IDL_Type * const unk =
            dynamic_cast<TAO::Unknown_IDL_Type *> (impl);

          if (!unk)
            throw CORBA::INTERNAL ();

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
      throw DynamicAny::DynAny::TypeMismatch ();
    }
}

CORBA::Any_ptr
TAO_DynEnum_i::to_any (void)
{
  TAO_OutputCDR out_cdr;

  out_cdr.write_ulong (this->value_);

  CORBA::Any *retval;
  ACE_NEW_THROW_EX (retval,
                    CORBA::Any,
                    CORBA::NO_MEMORY ());

  TAO_InputCDR in_cdr (out_cdr);
  TAO::Unknown_IDL_Type *unk = 0;
  ACE_NEW_THROW_EX (unk,
                    TAO::Unknown_IDL_Type (this->type_.in (),
                                           in_cdr),
                    CORBA::NO_MEMORY ());

  retval->replace (unk);
  return retval;
}

CORBA::Boolean
TAO_DynEnum_i::equal (DynamicAny::DynAny_ptr rhs)
{
  CORBA::TypeCode_var tc = rhs->type ();

  CORBA::Boolean equivalent = tc->equivalent (this->type_.in ());

  if (!equivalent)
    {
      return false;
    }

  CORBA::Any_var any = rhs->to_any ();

  TAO::Any_Impl *impl = any->impl ();
  CORBA::ULong value;

  if (impl->encoded ())
    {
      TAO::Unknown_IDL_Type * const unk =
        dynamic_cast<TAO::Unknown_IDL_Type *> (impl);

      if (!unk)
        throw CORBA::INTERNAL ();

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
TAO_DynEnum_i::destroy (void)
{
  if (this->destroyed_)
    {
      throw ::CORBA::OBJECT_NOT_EXIST ();
    }

  if (!this->ref_to_component_ || this->container_is_destroying_)
    {
      this->destroyed_ = true;
    }
}

DynamicAny::DynAny_ptr
TAO_DynEnum_i::current_component (void)
{
  if (this->destroyed_)
    {
      throw ::CORBA::OBJECT_NOT_EXIST ();
    }

  throw DynamicAny::DynAny::TypeMismatch ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
