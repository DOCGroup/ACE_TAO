// -*- C++ -*-
// $Id$

#include "tao/DynamicAny/DynValueBox_i.h"
#include "tao/DynamicAny/DynAnyFactory.h"
#include "tao/DynamicAny/DynAnyUtils_T.h"

#include "tao/AnyTypeCode/Marshal.h"
#include "tao/AnyTypeCode/Any_Unknown_IDL_Type.h"
#include "tao/AnyTypeCode/AnyTypeCode_methods.h"

#include "tao/CDR.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_DynValueBox_i::TAO_DynValueBox_i (CORBA::Boolean allow_truncation)
  : TAO_DynCommon (allow_truncation)
  , TAO_DynAny_i(allow_truncation)
  , TAO_DynValueCommon_i (allow_truncation)
{
}

TAO_DynValueBox_i::~TAO_DynValueBox_i (void)
{
}

void
TAO_DynValueBox_i::set_to_value (void)
{
  if (CORBA::is_nil (this->boxed_.in ()))
    {
      set_to_null ();
    }
  else
    {
      this->is_null_      = false;
      this->component_count_ = 1u;
      this->current_position_ = 0;
    }
}

void
TAO_DynValueBox_i::check_typecode (CORBA::TypeCode_ptr tc)
{
  CORBA::TCKind kind = TAO_DynAnyFactory::unalias (tc);
  if (kind == CORBA::tk_value_box)
    {
      return;
    }

  throw DynamicAny::DynAnyFactory::InconsistentTypeCode ();
}

void
TAO_DynValueBox_i::init (CORBA::TypeCode_ptr tc)
{
  this->check_typecode (tc);
  this->type_ = CORBA::TypeCode::_duplicate (tc);

  // member_type() does not work with aliased type codes.
  CORBA::TypeCode_var unaliased_tc =
    TAO_DynAnyFactory::strip_alias (this->type_.in ());
  CORBA::TypeCode_var
    mtype (unaliased_tc->content_type ());
  this->boxed_ =
    TAO::MakeDynAnyUtils::make_dyn_any_t<CORBA::TypeCode_ptr>
    (mtype.in (), mtype.in (), this->allow_truncation_);

  this->init_common ();
  this->set_to_null ();

}

void
TAO_DynValueBox_i::init (const CORBA::Any & any)
{
  CORBA::TypeCode_ptr tc = any._tao_get_typecode ();
  this->check_typecode (tc);
  this->type_ = CORBA::TypeCode::_duplicate (tc);
  this->set_from_any (any);
}

TAO_DynValueBox_i *
TAO_DynValueBox_i::_narrow (CORBA::Object_ptr _tao_objref)
{
  return (CORBA::is_nil (_tao_objref)) ?
         0 :
         dynamic_cast<TAO_DynValueBox_i *> (_tao_objref);
}

CORBA::Any *
TAO_DynValueBox_i::get_boxed_value (void)
{
  if (this->destroyed_)
    {
      throw ::CORBA::OBJECT_NOT_EXIST ();
    }

  if (CORBA::is_nil (this->boxed_.in ()))
    {
      throw DynamicAny::DynAny::InvalidValue ();
    }

  return this->boxed_->to_any ();
}

void
TAO_DynValueBox_i::set_boxed_value (const CORBA::Any & boxed)
{
  if (this->destroyed_)
    {
      throw ::CORBA::OBJECT_NOT_EXIST ();
    }

  // content_type() does not work with aliased type codes.
  CORBA::TypeCode_var unaliased_tc =
    TAO_DynAnyFactory::strip_alias (this->type_.in ());
  CORBA::TypeCode_var my_tc = unaliased_tc->content_type ();
  CORBA::TypeCode_var value_tc = boxed._tao_get_typecode ();
  if (!my_tc->equivalent (value_tc.in ()))
    {
      throw DynamicAny::DynAny::TypeMismatch ();
    }

  this->boxed_ =
    TAO::MakeDynAnyUtils::make_dyn_any_t<const CORBA::Any&>
      (boxed._tao_get_typecode (), boxed, this->allow_truncation_);
  this->set_to_value ();
}

DynamicAny::DynAny_ptr
TAO_DynValueBox_i::get_boxed_value_as_dyn_any (void)
{
  if (this->destroyed_)
    {
      throw ::CORBA::OBJECT_NOT_EXIST ();
    }

  if (CORBA::is_nil (this->boxed_.in ()))
    {
      throw DynamicAny::DynAny::InvalidValue ();
    }

  // A deep copy is made only by copy() (CORBA 2.4.2 section 9.2.3.6).
  // Set the flag so the caller can't destroy.
  this->set_flag (this->boxed_.in (), 0);

  return DynamicAny::DynAny::_duplicate (this->boxed_.in ());
}

void
TAO_DynValueBox_i::set_boxed_value_as_dyn_any (DynamicAny::DynAny_ptr boxed )
{
  if (this->destroyed_)
    {
      throw ::CORBA::OBJECT_NOT_EXIST ();
    }

  // content_type() does not work with aliased type codes.
  CORBA::TypeCode_var unaliased_tc =
    TAO_DynAnyFactory::strip_alias (this->type_.in ());
  CORBA::TypeCode_var my_tc = unaliased_tc->content_type ();
  CORBA::TypeCode_var value_tc = boxed->type ();
  if (!my_tc->equivalent (value_tc.in ()))
    {
      throw DynamicAny::DynAny::TypeMismatch ();
    }
  this->boxed_ = boxed->copy ();
  this->set_to_value ();
}

void
TAO_DynValueBox_i::from_any (const CORBA::Any & any)
{
  if (this->destroyed_)
    {
      throw ::CORBA::OBJECT_NOT_EXIST ();
    }

  CORBA::TypeCode_var tc = any.type ();
  if (!this->type_->equivalent (tc.in ()))
    {
      throw DynamicAny::DynAny::TypeMismatch ();
    }

  this->set_from_any (any);
}

CORBA::Boolean
TAO_DynValueBox_i::equal (DynamicAny::DynAny_ptr rhs)
{
  if (this->destroyed_)
    {
      throw ::CORBA::OBJECT_NOT_EXIST ();
    }

  CORBA::TypeCode_var tc = rhs->type ();
  if (!tc->equivalent (this->type_.in ()))
    {
      return false;
    }

  DynamicAny::DynValueBox_ptr rhs_v=
    dynamic_cast<DynamicAny::DynValueBox_ptr> (rhs);
  if (this->is_null () != rhs_v->is_null ())
    {
      return false;
    }

  if (!this->is_null ())
    {
      DynamicAny::DynAny_var
        tmp (rhs->current_component ());
      return tmp->equal (this->boxed_.in ());
    }

  return true;
}

void
TAO_DynValueBox_i::destroy (void)
{
  if (this->destroyed_)
    {
      throw ::CORBA::OBJECT_NOT_EXIST ();
    }

  if (!this->ref_to_component_ || this->container_is_destroying_)
    {
      // Do a deep destroy.
      if (!CORBA::is_nil (this->boxed_.in ()))
        {
          this->set_flag (this->boxed_.in (), 1);
          this->boxed_.in ()->destroy ();
        }

      this->destroyed_ = 1;
    }
}

DynamicAny::DynAny_ptr
TAO_DynValueBox_i::current_component (void)
{
  if (this->destroyed_)
    {
      throw ::CORBA::OBJECT_NOT_EXIST ();
    }

  // Is this an NULL ValueBoxtype?
  if (this->is_null_)
    {
      return DynamicAny::DynAny::_nil ();
    }

  this->set_flag (this->boxed_.in (), 0);
  return DynamicAny::DynAny::_duplicate (this->boxed_.in ());
}

// This code is common to from_any() and the init() overload that takes
// an Any argument.
void
TAO_DynValueBox_i::set_from_any (const CORBA::Any & any)
{
  // Get the CDR stream of the Any, if there isn't one, make one.
  TAO_OutputCDR out;
  TAO_InputCDR in (static_cast<ACE_Message_Block *> (0));
  TAO::Any_Impl *impl = any.impl ();
  if (impl->encoded ())
    {
      TAO::Unknown_IDL_Type *unk =
        dynamic_cast<TAO::Unknown_IDL_Type *> (impl);
      if (!unk)
        throw CORBA::INTERNAL ();

      in = unk->_tao_get_cdr ();
    }
  else
    {
      impl->marshal_value (out);
      TAO_InputCDR tmp_in (out);
      in = tmp_in;
    }

  while (true)
    {
      TAO_InputCDR indrected_strm ((size_t) 0);
      CORBA::Boolean is_indirected= false;
      CORBA::Boolean is_null_object= false;

      // Read in the ValueBox header (to skip over it) and check
      // for the null type.
      if (!CORBA::ValueBase::_tao_validate_box_type (
            in,
            indrected_strm,
            this->type_.in ()->id (),
            is_null_object,
            is_indirected)              ||
          is_null_object)
        {
          this->set_to_null ();
          return;
        }

      if (!is_indirected)
        {
          break;
        }

      in = indrected_strm;
    }

  // content_type() does not work with aliased type codes.
  CORBA::TypeCode_var unaliased_tc =
    TAO_DynAnyFactory::strip_alias (this->type_.in ());
  CORBA::TypeCode_var boxed_tc (unaliased_tc->content_type ());
  TAO::Unknown_IDL_Type * unk = 0;
  ACE_NEW_THROW_EX (unk,
                    TAO::Unknown_IDL_Type (boxed_tc.in (), in),
                    CORBA::NO_MEMORY ());
  CORBA::Any boxed_any;
  boxed_any.replace (unk);
  this->boxed_ =
    TAO::MakeDynAnyUtils::make_dyn_any_t<const CORBA::Any&>
      (boxed_any._tao_get_typecode (), boxed_any, this->allow_truncation_);
  this->init_common ();
}

CORBA::Any_ptr
TAO_DynValueBox_i::to_any (void)
{
  if (this->destroyed_)
    {
      throw ::CORBA::OBJECT_NOT_EXIST ();
    }

  TAO_OutputCDR out_cdr;

  // Is this an NULL Valuetype?
  if (!CORBA::ValueBase::_tao_write_special_value (
       out_cdr, reinterpret_cast <CORBA::ValueBase *> (
         this->is_null_ ? 0 : this                 )   ))
    {
      // Build <value-tag>, which states if chunking is used
      // and if type information ((list of) repository id(s))
      // is provided.
      CORBA::Long valuetag =
        TAO_OBV_GIOP_Flags::Value_tag_base |
        TAO_OBV_GIOP_Flags::Type_info_single;

      ACE_CString type_id (this->type_->id ());

      // Write <value-tag> & Marshal type information.
      if (!out_cdr.write_long (valuetag) ||
          !CORBA::ValueBase::_tao_write_repository_id (out_cdr, type_id))
        {
          throw CORBA::INTERNAL ();
        }

      // Now write the boxed value itself

      TAO_InputCDR boxed_in_cdr (static_cast<ACE_Message_Block *> (0));
      CORBA::Any_var boxed_any (this->boxed_->to_any ());
      TAO::Any_Impl * boxed_impl = boxed_any->impl ();
      if (boxed_impl->encoded ())
        {
          TAO::Unknown_IDL_Type * boxed_unk=
            dynamic_cast<TAO::Unknown_IDL_Type *> (boxed_impl);
          if (!boxed_unk)
            {
              throw CORBA::INTERNAL ();
            }
          boxed_in_cdr = boxed_unk->_tao_get_cdr ();
        }
      else
        {
          TAO_OutputCDR boxed_out_cdr;
          boxed_impl->marshal_value (boxed_out_cdr);
          TAO_InputCDR tmp (boxed_out_cdr);
          boxed_in_cdr = tmp;
        }

      CORBA::TypeCode_var boxed_tc = this->boxed_->type ();
      (void) TAO_Marshal_Object::perform_append (boxed_tc.in (),
                                                 &boxed_in_cdr,
                                                 &out_cdr);
    }

  // Convert the out_cdr into a new any.
  TAO_InputCDR in_cdr (out_cdr);
  TAO::Unknown_IDL_Type * unk = 0;
  ACE_NEW_THROW_EX (unk,
                    TAO::Unknown_IDL_Type (this->type_.in (), in_cdr),
                    CORBA::NO_MEMORY ());
  CORBA::Any_ptr retval = 0;
  ACE_NEW_THROW_EX (retval, CORBA::Any, CORBA::NO_MEMORY ());
  retval->replace (unk);
  return retval;
}

TAO_END_VERSIONED_NAMESPACE_DECL
