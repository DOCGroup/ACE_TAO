// -*- C++ -*-
// $Id$

#include "tao/DynamicAny/DynSequence_i.h"
#include "tao/DynamicAny/DynAnyFactory.h"
#include "tao/DynamicAny/DynAnyUtils_T.h"

#include "tao/AnyTypeCode/Marshal.h"
#include "tao/AnyTypeCode/Any_Unknown_IDL_Type.h"
#include "tao/AnyTypeCode/AnyTypeCode_methods.h"

#include "tao/CDR.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_DynSequence_i::TAO_DynSequence_i (void)
{
}

TAO_DynSequence_i::~TAO_DynSequence_i (void)
{
}

void
TAO_DynSequence_i::init_common (void)
{
  this->ref_to_component_ = false;
  this->container_is_destroying_ = false;
  this->has_components_ = true;
  this->destroyed_ = false;
  this->current_position_ = -1;
  this->component_count_ = static_cast<CORBA::ULong> (this->da_members_.size ());
}

void
TAO_DynSequence_i::init (const CORBA::Any& any)
{
  CORBA::TypeCode_var tc = any.type ();

  CORBA::TCKind kind = TAO_DynAnyFactory::unalias (tc.in ());

  if (kind != CORBA::tk_sequence)
    {
      throw DynamicAny::DynAnyFactory::InconsistentTypeCode ();
    }

  this->type_ = tc;

  // Get the CDR stream of the Any, if there isn't one, make one.
  TAO::Any_Impl *impl = any.impl ();
  CORBA::ULong length;
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

  // If the any is a sequence, first 4 bytes of cdr hold the
  // length.
  cdr.read_ulong (length);

  // Resize the array.
  this->da_members_.size (length);

  this->init_common ();

  // Get the type of the sequence elments.
  CORBA::TypeCode_var field_tc =
    this->get_element_type ();

  for (CORBA::ULong i = 0; i < length; ++i)
    {
      CORBA::Any field_any;
      TAO_InputCDR unk_in (cdr);
      TAO::Unknown_IDL_Type *field_unk = 0;
      ACE_NEW (field_unk,
               TAO::Unknown_IDL_Type (field_tc.in (), unk_in));
      field_any.replace (field_unk);

      // This recursive step will call the correct constructor
      // based on the type of field_any.
      this->da_members_[i] =
        TAO::MakeDynAnyUtils::make_dyn_any_t<const CORBA::Any&> (
          field_any._tao_get_typecode (), field_any);

      // Move to the next field in the CDR stream.
      (void) TAO_Marshal_Object::perform_skip (field_tc.in (), &cdr);
    }
}

void
TAO_DynSequence_i::init (CORBA::TypeCode_ptr tc)
{
  CORBA::TCKind kind = TAO_DynAnyFactory::unalias (tc);

  if (kind != CORBA::tk_sequence)
    {
      throw DynamicAny::DynAnyFactory::InconsistentTypeCode ();
    }

  // Empty sequence.
  this->da_members_.size (0);

  this->init_common ();

  this->type_ = CORBA::TypeCode::_duplicate (tc);
}

// ****************************************************************

TAO_DynSequence_i *
TAO_DynSequence_i::_narrow (CORBA::Object_ptr _tao_objref)
{
  if (CORBA::is_nil (_tao_objref))
    {
      return 0;
    }

  return dynamic_cast<TAO_DynSequence_i *> (_tao_objref);
}

// ****************************************************************

CORBA::TypeCode_ptr
TAO_DynSequence_i::get_element_type (void)
{
  CORBA::TypeCode_var element_type =
    CORBA::TypeCode::_duplicate (this->type_.in ());

  // Strip away aliases (if any) on top of the outer type.
  CORBA::TCKind kind = element_type->kind ();

  while (kind != CORBA::tk_sequence)
    {
      element_type = element_type->content_type ();

      kind = element_type->kind ();
    }

  // Return the content type.
  CORBA::TypeCode_ptr retval = element_type->content_type ();

  return retval;
}

// = Functions specific to DynSequence.

CORBA::ULong
TAO_DynSequence_i::get_length (void)
{
  if (this->destroyed_)
    {
      throw ::CORBA::OBJECT_NOT_EXIST ();
    }

  return this->component_count_;
}

void
TAO_DynSequence_i::set_length (CORBA::ULong length)
{
  if (this->destroyed_)
    {
      throw ::CORBA::OBJECT_NOT_EXIST ();
    }

  // CORBA::TypeCode::length() does not accept aliased type codes.
  CORBA::TypeCode_var stripped_tc =
    TAO_DynAnyFactory::strip_alias (this->type_.in ());

  CORBA::ULong bound = stripped_tc->length ();

  if (bound > 0 && length > bound)
    {
      throw DynamicAny::DynAny::InvalidValue ();
    }

  // CORBA 2.3.1 has several explicit rules about resetting the
  // current position, depending on the current value of the
  // current position, the current size, and the new length.
  if (length == 0)
    {
      this->current_position_ = -1;
    }
  else if (length > this->component_count_)
    {
      if (this->current_position_ == -1)
        {
          // Set it to the first new slot.
          this->current_position_ = static_cast<CORBA::Long> (this->component_count_);
        }
    }
  else if (length < this->component_count_)
    {
      // If the current position will no longer exist..
      if (this->current_position_ >= static_cast<CORBA::Long> (length))
        {
          this->current_position_ = -1;
        }
    }

  if (length > this->component_count_)
    {
      // Grow array first, then initialize new members.
      this->da_members_.size (length);

      CORBA::TypeCode_var elemtype =
        stripped_tc->content_type ();

      for (CORBA::ULong i = this->component_count_; i < length; ++i)
        {
          this->da_members_[i] =
        TAO::MakeDynAnyUtils::make_dyn_any_t<CORBA::TypeCode_ptr> (
          elemtype.in (),
          elemtype.in ());
        }
    }
  else if (length < this->component_count_)
    {
      // Destroy any dangling members first, then shrink array.
      for (CORBA::ULong j = length; j < this->component_count_; ++j)
        {
          this->da_members_[j]->destroy ();
        }

      this->da_members_.size (length);
    }

  // Now we can update component_count_.
  this->component_count_ = length;
}

DynamicAny::AnySeq *
TAO_DynSequence_i::get_elements (void)
{
  if (this->destroyed_)
    {
      throw ::CORBA::OBJECT_NOT_EXIST ();
    }

  CORBA::ULong length = static_cast<CORBA::ULong> (this->da_members_.size ());

  DynamicAny::AnySeq *elements;
  ACE_NEW_THROW_EX (elements,
                    DynamicAny::AnySeq (length),
                    CORBA::NO_MEMORY ());

  elements->length (length);
  DynamicAny::AnySeq_var safe_retval = elements;

  // Initialize each Any.
  for (CORBA::ULong i = 0; i < length; ++i)
    {
      CORBA::Any_var tmp =
        this->da_members_[i]->to_any ();


      safe_retval[i] = tmp.in ();
    }

  return safe_retval._retn ();
}

void
TAO_DynSequence_i::set_elements (const DynamicAny::AnySeq & value)
{
  if (this->destroyed_)
    {
      throw ::CORBA::OBJECT_NOT_EXIST ();
    }

  CORBA::TypeCode_var stripped_tc =
    TAO_DynAnyFactory::strip_alias (this->type_.in ());

  CORBA::ULong length = value.length ();
  CORBA::ULong bound = stripped_tc->length ();

  if (bound > 0 && length > bound)
    {
      throw DynamicAny::DynAny::InvalidValue ();
    }

  // CORBA 2.4.2.
  if (length == 0)
    {
      this->current_position_ = -1;
    }
  else
    {
      this->current_position_ = 0;
    }

  // If the array grows, we must do it now.
  if (length > this->component_count_)
    {
      this->da_members_.size (length);
    }

  CORBA::TypeCode_var element_type = this->get_element_type ();

  CORBA::TypeCode_var value_tc;

  for (CORBA::ULong i = 0; i < length; ++i)
    {
      // Check each arg element for type match.
      value_tc = value[i].type ();
      CORBA::Boolean equivalent =
        value_tc->equivalent (element_type.in ());

      if (equivalent)
        {
          // Destroy any existing members.
          if (i < this->component_count_)
            {
              this->da_members_[i]->destroy ();
            }

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

  // Destroy any dangling members.
  for (CORBA::ULong j = length; j < this->component_count_; ++j)
    {
      this->da_members_[j]->destroy ();
    }

  // If the array shrinks, we must wait until now to do it.
  if (length < this->component_count_)
    {
      this->da_members_.size (length);
    }

  // Now we can update component_count_.
  this->component_count_ = length;
}

DynamicAny::DynAnySeq *
TAO_DynSequence_i::get_elements_as_dyn_any (void)
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
TAO_DynSequence_i::set_elements_as_dyn_any (
  const DynamicAny::DynAnySeq & values)
{
  if (this->destroyed_)
    {
      throw ::CORBA::OBJECT_NOT_EXIST ();
    }

  CORBA::TypeCode_var stripped_tc =
    TAO_DynAnyFactory::strip_alias (this->type_.in ());

  CORBA::ULong length = values.length ();
  CORBA::ULong bound = stripped_tc->length ();

  if (bound > 0 && length > bound)
    {
      throw DynamicAny::DynAny::InvalidValue ();
    }

  // If the array grows, we must do it now.
  if (length > this->component_count_)
    {
      this->da_members_.size (length);
    }

  CORBA::TypeCode_var element_type =
    this->get_element_type ();

  CORBA::TypeCode_var val_type;
  CORBA::Boolean equivalent;

  for (CORBA::ULong i = 0; i < length; ++i)
    {
      val_type = values[i]->type ();

      equivalent = val_type->equivalent (element_type.in ());

      if (equivalent)
        {
          // Destroy any existing members.
          if (i < this->component_count_)
            {
              this->da_members_[i]->destroy ();
            }

          this->da_members_[i] =
            values[i]->copy ();
        }
      else
        {
          throw DynamicAny::DynAny::TypeMismatch ();
        }
    }

  // Destroy any dangling members.
  for (CORBA::ULong j = length; j < this->component_count_; ++j)
    {
      this->da_members_[j]->destroy ();
    }

  // If the array shrinks, we must wait until now to do it.
  if (length < this->component_count_)
    {
      this->da_members_.size (length);
    }

  // Now we can update component_count_.
  this->component_count_ = length;
}

// ****************************************************************

void
TAO_DynSequence_i::from_any (const CORBA::Any & any)
{
  if (this->destroyed_)
    {
      throw ::CORBA::OBJECT_NOT_EXIST ();
    }

  CORBA::TypeCode_var tc = any.type ();
  CORBA::Boolean equivalent =
    this->type_.in ()->equivalent (tc.in ());

  if (equivalent)
    {
      // Get the CDR stream of the Any, if there isn't one, make one.
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


      CORBA::ULong arg_length;

      // If the any is a sequence, first 4 bytes of cdr hold the
      // length.
      cdr.read_ulong (arg_length);

      // If the array grows, we must do it now.
      if (arg_length > this->component_count_)
        {
          this->da_members_.size (arg_length);
        }

      CORBA::TypeCode_var field_tc =
        this->get_element_type ();

      for (CORBA::ULong i = 0; i < arg_length; ++i)
        {
          CORBA::Any field_any;
          TAO_InputCDR unk_in (cdr);
          TAO::Unknown_IDL_Type *field_unk = 0;
          ACE_NEW (field_unk,
                   TAO::Unknown_IDL_Type (field_tc.in (),
                                          unk_in));
          field_any.replace (field_unk);

          if (i < this->component_count_)
            {
              this->da_members_[i]->destroy ();
            }

          this->da_members_[i] =
            TAO::MakeDynAnyUtils::make_dyn_any_t<const CORBA::Any&> (
              field_any._tao_get_typecode (),
              field_any);

          // Move to the next field in the CDR stream.
          (void) TAO_Marshal_Object::perform_skip (field_tc.in (), &cdr);
        }

      // Destroy any dangling members.
      for (CORBA::ULong j = arg_length; j < this->component_count_; ++j)
        {
          this->da_members_[j]->destroy ();
        }

      // If the array shrinks, we must wait until now to do it.
      if (arg_length < this->component_count_)
        {
          this->da_members_.size (arg_length);
        }

      // Now we can update component_count_.
      this->component_count_ = arg_length;

      this->current_position_ = arg_length ? 0 : -1;
    }
  else
    {
      throw DynamicAny::DynAny::TypeMismatch ();
    }
}

CORBA::Any_ptr
TAO_DynSequence_i::to_any (void)
{
  if (this->destroyed_)
    {
      throw ::CORBA::OBJECT_NOT_EXIST ();
    }

  TAO_OutputCDR out_cdr;
  out_cdr.write_ulong (this->component_count_);

  CORBA::TypeCode_var field_tc =
    this->get_element_type ();

  for (CORBA::ULong i = 0; i < this->component_count_; ++i)
    {
      // Recursive step
      CORBA::Any_var field_any =
        this->da_members_[i]->to_any ();

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
TAO_DynSequence_i::equal (DynamicAny::DynAny_ptr rhs)
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

  if (rhs->component_count () != this->component_count_)
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
TAO_DynSequence_i::destroy (void)
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
TAO_DynSequence_i::current_component (void)
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
