/* -*- C++ -*- */
// $Id$

#include "DynSequence_i.h"
#include "DynAnyFactory.h"
#include "tao/Marshal.h"

ACE_RCSID (DynamicAny,
           DynSequence_i,
           "$Id$")


TAO_DynSequence_i::TAO_DynSequence_i (void)
{
}

TAO_DynSequence_i::~TAO_DynSequence_i (void)
{
}

void
TAO_DynSequence_i::init_common (void)
{
  this->ref_to_component_ = 0;
  this->container_is_destroying_ = 0;
  this->has_components_ = 1;
  this->destroyed_ = 0;
  this->current_position_ = -1;
  this->component_count_ = this->da_members_.size ();
}

void
TAO_DynSequence_i::init (const CORBA::Any& any
                         ACE_ENV_ARG_DECL)
{
  CORBA::TypeCode_var tc = any.type ();

  CORBA::TCKind kind =
    TAO_DynAnyFactory::unalias (tc.in ()
                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (kind != CORBA::tk_sequence)
    {
      ACE_THROW (DynamicAny::DynAnyFactory::InconsistentTypeCode ());
    }

  this->type_ = tc;

  // Get the CDR stream of the argument.
  ACE_Message_Block *mb = any._tao_get_cdr ();

  if (mb == 0)
    {
      ACE_NEW (mb,
               ACE_Message_Block);
      TAO_OutputCDR out;
      any.impl ()->marshal_value (out);
      ACE_CDR::consolidate (mb, out.begin ());
    }

  TAO_InputCDR cdr (mb,
                    any._tao_byte_order ());

  CORBA::ULong length;

  // If the any is a sequence, first 4 bytes of cdr hold the
  // length.
  cdr.read_ulong (length);

  // Resize the array.
  this->da_members_.size (length);

  this->init_common ();

  // Get the type of the sequence elments.
  CORBA::TypeCode_var field_tc =
    this->get_element_type (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  for (CORBA::ULong i = 0; i < length; ++i)
    {
      CORBA::Any field_any;
      TAO::Unknown_IDL_Type *unk = 0;
      ACE_NEW (unk,
               TAO::Unknown_IDL_Type (field_tc.in (),
                                      cdr.start (),
                                      cdr.byte_order ()));
      field_any.replace (unk);

      // This recursive step will call the correct constructor
      // based on the type of field_any.
      this->da_members_[i] =
        TAO_DynAnyFactory::make_dyn_any (field_any
                                         ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      // Move to the next field in the CDR stream.
      (void) TAO_Marshal_Object::perform_skip (field_tc.in (),
                                               &cdr
                                               ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
}

void
TAO_DynSequence_i::init (CORBA::TypeCode_ptr tc
                         ACE_ENV_ARG_DECL)
{
  CORBA::TCKind kind = TAO_DynAnyFactory::unalias (tc
                                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (kind != CORBA::tk_sequence)
    {
      ACE_THROW (DynamicAny::DynAnyFactory::InconsistentTypeCode ());
    }

  // Empty sequence.
  this->da_members_.size (0);

  this->init_common ();

  this->type_ = CORBA::TypeCode::_duplicate (tc);
}

// ****************************************************************

TAO_DynSequence_i *
TAO_DynSequence_i::_narrow (CORBA::Object_ptr obj
                            ACE_ENV_ARG_DECL_NOT_USED)
{
  if (CORBA::is_nil (obj))
    {
      return 0;
    }

  return ACE_reinterpret_cast (
             TAO_DynSequence_i*,
             obj->_tao_QueryInterface (
                      ACE_reinterpret_cast (
                          ptr_arith_t,
                          &TAO_DynSequence_i::_narrow
                        )
                    )
           );
}

void*
TAO_DynSequence_i::_tao_QueryInterface (ptr_arith_t type)
{
  ptr_arith_t mytype =
    ACE_reinterpret_cast (ptr_arith_t,
                          &TAO_DynSequence_i::_narrow);
  if (type == mytype)
    {
      this->_add_ref ();
      return this;
    }

  return
    this->ACE_NESTED_CLASS (DynamicAny, DynSequence::_tao_QueryInterface) (type);
}

// ****************************************************************

CORBA::TypeCode_ptr
TAO_DynSequence_i::get_element_type (ACE_ENV_SINGLE_ARG_DECL)
{
  CORBA::TypeCode_var element_type =
    CORBA::TypeCode::_duplicate (this->type_.in ());

  // Strip away aliases (if any) on top of the outer type.
  CORBA::TCKind kind = element_type->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  while (kind != CORBA::tk_sequence)
    {
      element_type = element_type->content_type (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

      kind = element_type->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());
    }

  // Return the content type.
  CORBA::TypeCode_ptr retval = element_type->content_type (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  return retval;
}

// = Functions specific to DynSequence.

CORBA::ULong
TAO_DynSequence_i::get_length (ACE_ENV_SINGLE_ARG_DECL)
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
TAO_DynSequence_i::set_length (CORBA::ULong length
                               ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::InvalidValue
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW (CORBA::OBJECT_NOT_EXIST ());
    }

  // CORBA::TypeCode::length() does not accept aliased type codes.
  CORBA::TypeCode_var stripped_tc =
    TAO_DynAnyFactory::strip_alias (this->type_.in ()
                                    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::ULong bound = stripped_tc->length (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (bound > 0 && length > bound)
    {
      ACE_THROW (DynamicAny::DynAny::InvalidValue ());
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
          this->current_position_ = ACE_static_cast (CORBA::Long,
                                                     this->component_count_);
        }
    }
  else if (length < this->component_count_)
    {
      // If the current position will no longer exist..
      if (this->current_position_ >= ACE_static_cast (CORBA::Long,
                                                      length))
        {
          this->current_position_ = -1;
        }
    }

  if (length > this->component_count_)
    {
      // Grow array first, then initialize new members.
      this->da_members_.size (length);

      CORBA::TypeCode_var elemtype =
        stripped_tc->content_type (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      for (CORBA::ULong i = this->component_count_; i < length; ++i)
        {
          this->da_members_[i] =
            TAO_DynAnyFactory::make_dyn_any (elemtype.in ()
                                             ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
        }
    }
  else if (length < this->component_count_)
    {
      // Destroy any dangling members first, then shrink array.
      for (CORBA::ULong j = length; j < this->component_count_; ++j)
        {
          this->da_members_[j]->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK;
        }

      this->da_members_.size (length);
    }

  // Now we can update component_count_.
  this->component_count_ = length;
}

DynamicAny::AnySeq *
TAO_DynSequence_i::get_elements (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                        0);
    }

  CORBA::ULong length = this->da_members_.size ();

  DynamicAny::AnySeq *elements;
  ACE_NEW_THROW_EX (elements,
                    DynamicAny::AnySeq (length),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  elements->length (length);
  DynamicAny::AnySeq_var safe_retval = elements;

  // Initialize each Any.
  for (CORBA::ULong i = 0; i < length; ++i)
    {
      CORBA::Any_var tmp =
        this->da_members_[i]->to_any (ACE_ENV_SINGLE_ARG_PARAMETER);

      ACE_CHECK_RETURN (0);

      safe_retval[i] = tmp.in ();
    }

  return safe_retval._retn ();
}

void
TAO_DynSequence_i::set_elements (const DynamicAny::AnySeq & value
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

  CORBA::TypeCode_var stripped_tc =
    TAO_DynAnyFactory::strip_alias (this->type_.in ()
                                    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::ULong length = value.length ();
  CORBA::ULong bound = stripped_tc->length (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (bound > 0 && length > bound)
    {
      ACE_THROW (DynamicAny::DynAny::InvalidValue ());
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

  CORBA::TypeCode_var element_type =
    this->get_element_type (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::TypeCode_var value_tc;

  for (CORBA::ULong i = 0; i < length; ++i)
    {
      // Check each arg element for type match.
      value_tc = value[i].type ();
      CORBA::Boolean equivalent =
        value_tc->equivalent (element_type.in ()
                              ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      if (equivalent)
        {
          // Destroy any existing members.
          if (i < this->component_count_)
            {
              this->da_members_[i]->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
              ACE_CHECK;
            }

          this->da_members_[i] =
            TAO_DynAnyFactory::make_dyn_any (value[i]
                                          ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
        }
      else
        {
          ACE_THROW (DynamicAny::DynAny::TypeMismatch ());
        }
    }

  // Destroy any dangling members.
  for (CORBA::ULong j = length; j < this->component_count_; ++j)
    {
      this->da_members_[j]->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
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
TAO_DynSequence_i::get_elements_as_dyn_any (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                        0);
    }

  DynamicAny::DynAnySeq *retval = 0;
  ACE_NEW_THROW_EX (retval,
                    DynamicAny::DynAnySeq (this->component_count_),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  retval->length (this->component_count_);
  DynamicAny::DynAnySeq_var safe_retval (retval);

  for (CORBA::ULong i = 0; i < this->component_count_; ++i)
    {
      // A deep copy is made only by copy() (CORBA 2.4.2 section 9.2.3.6).
      // Set the flag so the caller can't destroy.
      this->set_flag (this->da_members_[i].in (),
                      0
                      ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      safe_retval[i] =
        DynamicAny::DynAny::_duplicate (this->da_members_[i].in ());
      ACE_CHECK_RETURN (0);
    }

  return safe_retval._retn ();
}

void
TAO_DynSequence_i::set_elements_as_dyn_any (
    const DynamicAny::DynAnySeq & values
    ACE_ENV_ARG_DECL
  )
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

  CORBA::TypeCode_var stripped_tc =
    TAO_DynAnyFactory::strip_alias (this->type_.in ()
                                    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::ULong length = values.length ();
  CORBA::ULong bound = stripped_tc->length (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (bound > 0 && length > bound)
    {
      ACE_THROW (DynamicAny::DynAny::InvalidValue ());
    }

  // If the array grows, we must do it now.
  if (length > this->component_count_)
    {
      this->da_members_.size (length);
    }

  CORBA::TypeCode_var element_type = this->get_element_type (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::TypeCode_var val_type;
  CORBA::Boolean equivalent;

  for (CORBA::ULong i = 0; i < length; ++i)
    {
      val_type = values[i]->type (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      equivalent = val_type->equivalent (element_type.in ()
                                         ACE_ENV_ARG_PARAMETER);

      ACE_CHECK;

      if (equivalent)
        {
          // Destroy any existing members.
          if (i < this->component_count_)
            {
              this->da_members_[i]->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
              ACE_CHECK;
            }

          this->da_members_[i] = values[i]->copy (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK;
        }
      else
        {
          ACE_THROW (DynamicAny::DynAny::TypeMismatch ());
        }
    }

  // Destroy any dangling members.
  for (CORBA::ULong j = length; j < this->component_count_; ++j)
    {
      this->da_members_[j]->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
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
TAO_DynSequence_i::from_any (const CORBA::Any & any
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

  CORBA::TypeCode_var tc = any.type ();
  CORBA::Boolean equivalent = this->type_.in ()->equivalent (tc.in ()
                                                             ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (equivalent)
    {
      // Get the CDR stream of the argument.
      ACE_Message_Block *mb = any._tao_get_cdr ();

      if (mb == 0)
        {
          ACE_NEW (mb,
                   ACE_Message_Block);
          TAO_OutputCDR out;
          any.impl ()->marshal_value (out);
          ACE_CDR::consolidate (mb, out.begin ());
        }

      TAO_InputCDR cdr (mb,
                        any._tao_byte_order ());

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
        this->get_element_type (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      for (CORBA::ULong i = 0; i < arg_length; ++i)
        {
          CORBA::Any field_any;
          TAO::Unknown_IDL_Type *unk = 0;
          ACE_NEW (unk,
                   TAO::Unknown_IDL_Type (field_tc.in (),
                                          cdr.start (),
                                          cdr.byte_order ()));
          field_any.replace (unk);

          if (i < this->component_count_)
            {
              this->da_members_[i]->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
              ACE_CHECK;
            }

          this->da_members_[i] =
            TAO_DynAnyFactory::make_dyn_any (field_any
                                          ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;

          // Move to the next field in the CDR stream.
          (void) TAO_Marshal_Object::perform_skip (field_tc.in (),
                                                   &cdr
                                                   ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
        }

      // Destroy any dangling members.
      for (CORBA::ULong j = arg_length; j < this->component_count_; ++j)
        {
          this->da_members_[j]->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK;
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
      ACE_THROW (DynamicAny::DynAny::TypeMismatch ());
    }
}

CORBA::Any_ptr
TAO_DynSequence_i::to_any (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                        0);
    }

  TAO_OutputCDR out_cdr;

  out_cdr.write_ulong (this->component_count_);

  CORBA::TypeCode_var field_tc =
    this->get_element_type (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  for (CORBA::ULong i = 0; i < this->component_count_; ++i)
    {
      // Recursive step
      CORBA::Any_var field_any =
        this->da_members_[i]->to_any (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      ACE_Message_Block *field_mb = field_any->_tao_get_cdr ();

      TAO_InputCDR field_cdr (field_mb,
                              field_any->_tao_byte_order ());

      (void) TAO_Marshal_Object::perform_append (field_tc.in (),
                                                 &field_cdr,
                                                 &out_cdr
                                                 ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);
    }

  TAO_InputCDR in_cdr (out_cdr);

  CORBA::Any_ptr retval = 0;
  ACE_NEW_THROW_EX (retval,
                    CORBA::Any,
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  TAO::Unknown_IDL_Type *unk = 0;
  ACE_NEW_THROW_EX (unk,
                    TAO::Unknown_IDL_Type (this->type_.in (),
                                           in_cdr.start (),
                                           in_cdr.byte_order ()),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  retval->replace (unk);
  return retval;
}

CORBA::Boolean
TAO_DynSequence_i::equal (DynamicAny::DynAny_ptr rhs
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

  CORBA::TypeCode_var tc = rhs->type (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  CORBA::Boolean equivalent = tc->equivalent (this->type_.in ()
                                              ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  if (!equivalent)
    {
      return 0;
    }

  DynamicAny::DynAny_var tmp;
  CORBA::Boolean member_equal;

  for (CORBA::ULong i = 0; i < this->component_count_; ++i)
    {
      rhs->seek (ACE_static_cast (CORBA::Long,
                                  i)
                 ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      tmp = rhs->current_component (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      // Recursive step.
      member_equal = tmp->equal (this->da_members_[i].in ()
                                 ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      if (!member_equal)
        {
          return 0;
        }
    }

  return 1;
}

void
TAO_DynSequence_i::destroy (ACE_ENV_SINGLE_ARG_DECL)
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
      // Do a deep destroy.
      for (CORBA::ULong i = 0; i < this->component_count_; ++i)
        {
          this->set_flag (da_members_[i].in (),
                          1
                          ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;

          this->da_members_[i]->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK;
        }

      this->destroyed_ = 1;
    }
}

DynamicAny::DynAny_ptr
TAO_DynSequence_i::current_component (ACE_ENV_SINGLE_ARG_DECL)
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

  if (this->current_position_ == -1)
    {
      return DynamicAny::DynAny::_nil ();
    }

  CORBA::ULong index = ACE_static_cast (CORBA::ULong,
                                        this->current_position_);

  this->set_flag (this->da_members_[index].in (),
                  0
                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (DynamicAny::DynAny::_nil ());

  return DynamicAny::DynAny::_duplicate (
            this->da_members_[index].in ()
          );
}
