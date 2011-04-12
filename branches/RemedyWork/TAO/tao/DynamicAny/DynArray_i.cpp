// -*- C++ -*-
// $Id$

#include "tao/DynamicAny/DynArray_i.h"
#include "tao/DynamicAny/DynAnyFactory.h"
#include "tao/DynamicAny/DynAnyUtils_T.h"

#include "tao/AnyTypeCode/Marshal.h"
#include "tao/AnyTypeCode/Any_Unknown_IDL_Type.h"
#include "tao/AnyTypeCode/AnyTypeCode_methods.h"

#include "tao/CDR.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_DynArray_i::TAO_DynArray_i (void)
{
}

TAO_DynArray_i::~TAO_DynArray_i (void)
{
}

void
TAO_DynArray_i::init_common (void)
{
  this->ref_to_component_ = false;
  this->container_is_destroying_ = false;
  this->has_components_ = true;
  this->destroyed_ = false;
  this->current_position_ = 0;
  this->component_count_ = static_cast<CORBA::ULong> (this->da_members_.size ());
}

void
TAO_DynArray_i::init (const CORBA::Any & any)
{
  CORBA::TypeCode_var tc = any.type ();

  CORBA::TCKind kind = TAO_DynAnyFactory::unalias (tc.in ());

  if (kind != CORBA::tk_array)
    {
      throw DynamicAny::DynAnyFactory::InconsistentTypeCode ();
    }

  this->type_ = tc;

  CORBA::ULong numfields = this->get_tc_length (tc.in ());
  // Resize the array.
  this->da_members_.size (numfields);

  this->init_common ();

  // Get the CDR stream of the Any, if there isn't one, make one.
  TAO::Any_Impl *impl = any.impl ();
  TAO_OutputCDR out;
  TAO_InputCDR cdr (static_cast<ACE_Message_Block *> (0));

  if (impl->encoded  ())
    {
      TAO::Unknown_IDL_Type * const unk =
        dynamic_cast<TAO::Unknown_IDL_Type *> (impl);

      if (!unk)
        throw CORBA::INTERNAL ();

      cdr = unk->_tao_get_cdr ();
    }
  else
    {
      impl->marshal_value (out);
      TAO_InputCDR tmp_in (out);
      cdr = tmp_in;
    }

  CORBA::TypeCode_var field_tc =
    this->get_element_type ();

  for (CORBA::ULong i = 0; i < numfields; ++i)
    {
      CORBA::Any field_any;
      TAO::Unknown_IDL_Type *field_unk = 0;
      TAO_InputCDR unk_in (cdr);
      ACE_NEW (field_unk,
               TAO::Unknown_IDL_Type (field_tc.in (), unk_in));
      field_any.replace (field_unk);

      // This recursive step will call the correct constructor
      // based on the type of field_any.
      this->da_members_[i] =
        TAO::MakeDynAnyUtils::make_dyn_any_t<const CORBA::Any&> (
          field_any._tao_get_typecode (),
          field_any);

      // Move to the next field in the CDR stream.
      (void) TAO_Marshal_Object::perform_skip (field_tc.in (), &cdr);
    }
}

void
TAO_DynArray_i::init (CORBA::TypeCode_ptr tc)
{
  CORBA::TCKind kind = TAO_DynAnyFactory::unalias (tc);

  if (kind != CORBA::tk_array)
    {
      throw DynamicAny::DynAnyFactory::InconsistentTypeCode ();
    }

  this->type_ = CORBA::TypeCode::_duplicate (tc);

  CORBA::ULong numfields = this->get_tc_length (tc);

  // Resize the array.
  this->da_members_.size (numfields);

  this->init_common ();

  CORBA::TypeCode_var elemtype = this->get_element_type ();

  for (CORBA::ULong i = 0; i < numfields; ++i)
    {
      // Recursively initialize each element.
      this->da_members_[i] =
        TAO::MakeDynAnyUtils::make_dyn_any_t<CORBA::TypeCode_ptr> (
          elemtype.in (),
          elemtype.in ());
    }
}

CORBA::TypeCode_ptr
TAO_DynArray_i::get_element_type (void)
{
  CORBA::TypeCode_var element_type =
    CORBA::TypeCode::_duplicate (this->type_.in ());

  // Strip away aliases (if any) on top of the outer type
  CORBA::TCKind kind = element_type->kind ();

  while (kind != CORBA::tk_array)
    {
      element_type = element_type->content_type ();

      kind = element_type->kind ();
    }

  // Return the content type.
  return element_type->content_type ();
}

// Get the length from the (possibly aliased) typecode.
CORBA::ULong
TAO_DynArray_i::get_tc_length (CORBA::TypeCode_ptr tc)
{
  CORBA::TypeCode_var tctmp = CORBA::TypeCode::_duplicate (tc);
  CORBA::TCKind kind = tctmp->kind ();

  while (kind == CORBA::tk_alias)
    {
      tctmp = tctmp->content_type ();
      kind = tctmp->kind ();
    }

  return tctmp->length ();
}

// ****************************************************************

TAO_DynArray_i *
TAO_DynArray_i::_narrow (CORBA::Object_ptr _tao_objref)
{
  if (CORBA::is_nil (_tao_objref))
    {
      return 0;
    }

  return dynamic_cast<TAO_DynArray_i *> (_tao_objref);
}

// ****************************************************************

DynamicAny::AnySeq *
TAO_DynArray_i::get_elements (void)
{
  if (this->destroyed_)
    {
      throw ::CORBA::OBJECT_NOT_EXIST ();
    }

  CORBA::ULong length = static_cast<CORBA::ULong> (this->da_members_.size ());

  DynamicAny::AnySeq *elements = 0;
  ACE_NEW_THROW_EX (elements,
                    DynamicAny::AnySeq (length),
                    CORBA::NO_MEMORY ());

  elements->length (length);
  DynamicAny::AnySeq_var safe_retval (elements);

  CORBA::Any_var tmp;

  // Initialize each Any.
  for (CORBA::ULong i = 0; i < length; i++)
    {
      tmp = this->da_members_[i]->to_any ();

      safe_retval[i] = tmp.in ();
    }

  return safe_retval._retn ();
}

void
TAO_DynArray_i::set_elements (const DynamicAny::AnySeq & value)
{
  if (this->destroyed_)
    {
      throw ::CORBA::OBJECT_NOT_EXIST ();
    }

  CORBA::ULong const length = value.length ();

  if (length != this->da_members_.size ())
    {
      throw DynamicAny::DynAny::InvalidValue ();
    }

  CORBA::TypeCode_var value_tc;
  CORBA::TypeCode_var element_type = this->get_element_type ();

  for (CORBA::ULong i = 0; i < length; i++)
    {
      // Check each arg element for type match.
      value_tc = value[i].type ();
      CORBA::Boolean equivalent =
        value_tc->equivalent (element_type.in ());

      if (equivalent)
        {
          this->da_members_[i]->destroy ();

          this->da_members_[i] =
            TAO::MakeDynAnyUtils::make_dyn_any_t<const CORBA::Any&> (
              value[i]._tao_get_typecode (),
              value[i]);
        }
      else
        {
          throw DynamicAny::DynAny::TypeMismatch ();
        }
    }
}

DynamicAny::DynAnySeq *
TAO_DynArray_i::get_elements_as_dyn_any (void)
{
  if (this->destroyed_)
    {
      throw ::CORBA::OBJECT_NOT_EXIST ();
    }

  DynamicAny::DynAnySeq *retval = 0;
  ACE_NEW_THROW_EX (retval,
                    DynamicAny::DynAnySeq (this->component_count_),
                    CORBA::NO_MEMORY ());

  retval->length (this->component_count_);
  DynamicAny::DynAnySeq_var safe_retval (retval);

  for (CORBA::ULong i = 0; i < this->component_count_; ++i)
    {
      // A deep copy is made only by copy() (CORBA 2.4.2 section 9.2.3.6).
      // Set the flag so the caller can't destroy.
      this->set_flag (this->da_members_[i].in (), 0);

      safe_retval[i] =
        DynamicAny::DynAny::_duplicate (this->da_members_[i].in ());
    }

  return safe_retval._retn ();
}

void
TAO_DynArray_i::set_elements_as_dyn_any (
    const DynamicAny::DynAnySeq & values

  )
{
  if (this->destroyed_)
    {
      throw ::CORBA::OBJECT_NOT_EXIST ();
    }

  CORBA::ULong length = static_cast<CORBA::ULong> (this->da_members_.size ());

  if (values.length () != length)
    {
      throw DynamicAny::DynAny::InvalidValue ();
    }

  CORBA::TypeCode_var element_type = this->get_element_type ();

  CORBA::TypeCode_var val_type;
  CORBA::Boolean equivalent;

  for (CORBA::ULong i = 0; i < length; ++i)
    {
      val_type = values[i]->type ();

      equivalent = val_type->equivalent (element_type.in ());


      if (equivalent)
        {
          this->da_members_[i] = values[i]->copy ();
        }
      else
        {
          throw DynamicAny::DynAny::TypeMismatch ();
        }
    }
}

// ****************************************************************

void
TAO_DynArray_i::from_any (const CORBA::Any& any)
{
  if (this->destroyed_)
    {
      throw ::CORBA::OBJECT_NOT_EXIST ();
    }

  CORBA::TypeCode_var tc = any.type ();
  CORBA::Boolean equivalent = this->type_.in ()->equivalent (tc.in ());

  if (equivalent)
    {
      // Get the CDR stream of the Any,if there isn't one, make one.
      TAO::Any_Impl *impl = any.impl ();
      TAO_OutputCDR out;
      TAO_InputCDR cdr (static_cast<ACE_Message_Block *> (0));

      if (impl->encoded ())
        {
          TAO::Unknown_IDL_Type * const unk =
            dynamic_cast<TAO::Unknown_IDL_Type *> (impl);

          if (!unk)
            throw CORBA::INTERNAL ();

          cdr = unk->_tao_get_cdr ();
        }
      else
        {
          impl->marshal_value (out);
          TAO_InputCDR tmp_in (out);
          cdr = tmp_in;
        }

      CORBA::ULong length = static_cast<CORBA::ULong> (this->da_members_.size ());
      CORBA::ULong arg_length = this->get_tc_length (tc.in ());

      if (length != arg_length)
        {
          throw DynamicAny::DynAny::TypeMismatch ();
        }

      CORBA::TypeCode_var field_tc = this->get_element_type ();

      for (CORBA::ULong i = 0; i < arg_length; ++i)
        {
          CORBA::Any field_any;
          TAO_InputCDR unk_in (cdr);
          TAO::Unknown_IDL_Type *field_unk = 0;
          ACE_NEW (field_unk,
                   TAO::Unknown_IDL_Type (field_tc.in (), unk_in));
          field_any.replace (field_unk);

          this->da_members_[i]->destroy ();

          this->da_members_[i] =
            TAO::MakeDynAnyUtils::make_dyn_any_t<const CORBA::Any&> (
              field_any._tao_get_typecode (),
              field_any);

          // Move to the next field in the CDR stream.
          (void) TAO_Marshal_Object::perform_skip (field_tc.in (), &cdr);
        }

      this->current_position_ = arg_length ? 0 : -1;
    }
  else
    {
      throw DynamicAny::DynAny::TypeMismatch ();
    }
}

CORBA::Any_ptr
TAO_DynArray_i::to_any (void)
{
  if (this->destroyed_)
    {
      throw ::CORBA::OBJECT_NOT_EXIST ();
    }

  CORBA::TypeCode_var field_tc = this->get_element_type ();

  TAO_OutputCDR out_cdr;
  CORBA::Any_var field_any;
  size_t length = this->da_members_.size ();

  for (size_t i = 0; i < length; ++i)
    {
      // Recursive step.
      field_any = this->da_members_[i]->to_any ();

      TAO::Any_Impl *field_impl = field_any->impl ();
      TAO_OutputCDR field_out;
      TAO_InputCDR field_cdr (static_cast<ACE_Message_Block *> (0));

      if (field_impl->encoded ())
        {
          TAO::Unknown_IDL_Type * const field_unk =
            dynamic_cast<TAO::Unknown_IDL_Type *> (field_impl);

          if (!field_unk)
            throw CORBA::INTERNAL ();

          field_cdr = field_unk->_tao_get_cdr ();
        }
      else
        {
          field_impl->marshal_value (field_out);
          TAO_InputCDR tmp_in (field_out);
          field_cdr = tmp_in;
        }

      (void) TAO_Marshal_Object::perform_append (field_tc.in (),
                                                 &field_cdr,
                                                 &out_cdr);
    }

  TAO_InputCDR in_cdr (out_cdr);

  CORBA::Any_ptr retval = 0;
  ACE_NEW_THROW_EX (retval,
                    CORBA::Any,
                    CORBA::NO_MEMORY ());

  TAO::Unknown_IDL_Type *unk = 0;
  ACE_NEW_THROW_EX (unk,
                    TAO::Unknown_IDL_Type (this->type_.in (),
                                           in_cdr),
                    CORBA::NO_MEMORY ());

  retval->replace (unk);
  return retval;
}

CORBA::Boolean
TAO_DynArray_i::equal (DynamicAny::DynAny_ptr rhs)
{
  if (this->destroyed_)
    {
      throw ::CORBA::OBJECT_NOT_EXIST ();
    }

  CORBA::TypeCode_var tc = rhs->type ();

  CORBA::Boolean equivalent = tc->equivalent (this->type_.in ());

  if (!equivalent)
    {
      return false;
    }

  DynamicAny::DynAny_var tmp;
  CORBA::Boolean member_equal;

  for (CORBA::ULong i = 0; i < this->component_count_; ++i)
    {
      rhs->seek (static_cast<CORBA::Long> (i));

      tmp = rhs->current_component ();

      // Recursive step.
      member_equal = tmp->equal (this->da_members_[i].in ());

      if (!member_equal)
        {
          return false;
        }
    }

  return true;
}

void
TAO_DynArray_i::destroy (void)
{
  if (this->destroyed_)
    {
      throw ::CORBA::OBJECT_NOT_EXIST ();
    }

  if (!this->ref_to_component_ || this->container_is_destroying_)
    {
      // Do a deep destroy.
      for (CORBA::ULong i = 0; i < this->component_count_; ++i)
        {
          this->set_flag (da_members_[i].in (), 1);

          this->da_members_[i]->destroy ();
        }

      this->destroyed_ = 1;
    }
}

DynamicAny::DynAny_ptr
TAO_DynArray_i::current_component (void)
{
  if (this->destroyed_)
    {
      throw ::CORBA::OBJECT_NOT_EXIST ();
    }

  if (this->current_position_ == -1)
    {
      return DynamicAny::DynAny::_nil ();
    }

  CORBA::ULong index = static_cast<CORBA::ULong> (this->current_position_);

  this->set_flag (this->da_members_[index].in (), 0);

  return DynamicAny::DynAny::_duplicate (this->da_members_[index].in ());
}

TAO_END_VERSIONED_NAMESPACE_DECL
