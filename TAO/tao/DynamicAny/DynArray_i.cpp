// $Id$

#include "tao/DynamicAny/DynArray_i.h"
#include "tao/DynamicAny/DynAnyFactory.h"
#include "tao/AnyTypeCode/Marshal.h"
#include "tao/AnyTypeCode/Any_Unknown_IDL_Type.h"
#include "tao/AnyTypeCode/TypeCode.h"
#include "tao/CDR.h"

ACE_RCSID (DynamicAny,
           DynArray_i,
           "$Id$")

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
  this->ref_to_component_ = 0;
  this->container_is_destroying_ = 0;
  this->has_components_ = 1;
  this->destroyed_ = 0;
  this->current_position_ = 0;
  this->component_count_ = static_cast<CORBA::ULong> (this->da_members_.size ());
}

void
TAO_DynArray_i::init (const CORBA::Any & any
                      ACE_ENV_ARG_DECL)
{
  CORBA::TypeCode_var tc = any.type ();

  CORBA::TCKind kind = TAO_DynAnyFactory::unalias (tc.in ()
                                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (kind != CORBA::tk_array)
    {
      ACE_THROW (DynamicAny::DynAnyFactory::InconsistentTypeCode ());
    }

  this->type_ = tc;

  CORBA::ULong numfields = this->get_tc_length (tc.in ()
                                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  // Resize the array.
  this->da_members_.size (numfields);

  this->init_common ();

  // Get the CDR stream of the Any, if there isn't one, make one.
  TAO::Any_Impl *impl = any.impl ();
  TAO_OutputCDR out;
  TAO_InputCDR cdr (static_cast<ACE_Message_Block *> (0));

  if (impl->encoded  ())
    {
      TAO::Unknown_IDL_Type *unk =
        dynamic_cast<TAO::Unknown_IDL_Type *> (impl);

      cdr = unk->_tao_get_cdr ();
    }
  else
    {
      impl->marshal_value (out);
      TAO_InputCDR tmp_in (out);
      cdr = tmp_in;
    }

  CORBA::TypeCode_var field_tc =
    this->get_element_type (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  for (CORBA::ULong i = 0; i < numfields; ++i)
    {
      CORBA::Any field_any;
      TAO::Unknown_IDL_Type *field_unk = 0;
      TAO_InputCDR unk_in (cdr);
      ACE_NEW (field_unk,
               TAO::Unknown_IDL_Type (field_tc.in (),
                                      unk_in));
      field_any.replace (field_unk);

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
TAO_DynArray_i::init (CORBA::TypeCode_ptr tc
                      ACE_ENV_ARG_DECL)
{
  CORBA::TCKind kind = TAO_DynAnyFactory::unalias (tc
                                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (kind != CORBA::tk_array)
    {
      ACE_THROW (DynamicAny::DynAnyFactory::InconsistentTypeCode ());
    }

  this->type_ = CORBA::TypeCode::_duplicate (tc);

  CORBA::ULong numfields = this->get_tc_length (tc
                                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Resize the array.
  this->da_members_.size (numfields);

  this->init_common ();

  CORBA::TypeCode_var elemtype =
    this->get_element_type (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  for (CORBA::ULong i = 0; i < numfields; ++i)
    {
      // Recursively initialize each element.
      this->da_members_[i] =
        TAO_DynAnyFactory::make_dyn_any (elemtype.in ()
                                         ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
}

CORBA::TypeCode_ptr
TAO_DynArray_i::get_element_type (ACE_ENV_SINGLE_ARG_DECL)
{
  CORBA::TypeCode_var element_type =
    CORBA::TypeCode::_duplicate (this->type_.in ());

  // Strip away aliases (if any) on top of the outer type
  CORBA::TCKind kind = element_type->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  while (kind != CORBA::tk_array)
    {
      element_type =
        element_type->content_type (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

      kind = element_type->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());
    }

  // Return the content type.
  CORBA::TypeCode_ptr retval =
    element_type->content_type (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  return retval;
}

// Get the length from the (possibly aliased) typecode.
CORBA::ULong
TAO_DynArray_i::get_tc_length (CORBA::TypeCode_ptr tc
                               ACE_ENV_ARG_DECL)
{
  CORBA::TypeCode_var tctmp = CORBA::TypeCode::_duplicate (tc);
  CORBA::TCKind kind = tctmp->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  while (kind == CORBA::tk_alias)
    {
      tctmp = tctmp->content_type (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);
      kind = tctmp->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);
    }

  CORBA::ULong retval = tctmp->length (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return retval;
}

// ****************************************************************

TAO_DynArray_i *
TAO_DynArray_i::_narrow (CORBA::Object_ptr _tao_objref
                         ACE_ENV_ARG_DECL_NOT_USED)
{
  if (CORBA::is_nil (_tao_objref))
    {
      return 0;
    }

  return dynamic_cast<TAO_DynArray_i *> (_tao_objref);
}

// ****************************************************************

DynamicAny::AnySeq *
TAO_DynArray_i::get_elements (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                        0);
    }

  CORBA::ULong length = static_cast<CORBA::ULong> (this->da_members_.size ());

  DynamicAny::AnySeq *elements = 0;
  ACE_NEW_THROW_EX (elements,
                    DynamicAny::AnySeq (length),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  elements->length (length);
  DynamicAny::AnySeq_var safe_retval (elements);

  CORBA::Any_var tmp;

  // Initialize each Any.
  for (CORBA::ULong i = 0; i < length; i++)
    {
      tmp = this->da_members_[i]->to_any (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      safe_retval[i] = tmp.in ();
    }

  return safe_retval._retn ();
}

void
TAO_DynArray_i::set_elements (const DynamicAny::AnySeq & value
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

  CORBA::ULong length = value.length ();

  if (length != this->da_members_.size ())
    {
      ACE_THROW (DynamicAny::DynAny::InvalidValue ());
    }

  CORBA::TypeCode_var value_tc;
  CORBA::TypeCode_var element_type =
    this->get_element_type (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  for (CORBA::ULong i = 0; i < length; i++)
    {
      // Check each arg element for type match.
      value_tc = value[i].type ();
      CORBA::Boolean equivalent =
        value_tc->equivalent (element_type.in ()
                              ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      if (equivalent)
        {
          this->da_members_[i]->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK;

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
}

DynamicAny::DynAnySeq *
TAO_DynArray_i::get_elements_as_dyn_any (ACE_ENV_SINGLE_ARG_DECL)
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
TAO_DynArray_i::set_elements_as_dyn_any (
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

  CORBA::ULong length = static_cast<CORBA::ULong> (this->da_members_.size ());

  if (values.length () != length)
    {
      ACE_THROW (DynamicAny::DynAny::InvalidValue ());
    }

  CORBA::TypeCode_var element_type =
    this->get_element_type (ACE_ENV_SINGLE_ARG_PARAMETER);
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
          this->da_members_[i] =
            values[i]->copy (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK;
        }
      else
        {
          ACE_THROW (DynamicAny::DynAny::TypeMismatch ());
        }
    }
}

// ****************************************************************

void
TAO_DynArray_i::from_any (const CORBA::Any& any
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
  CORBA::Boolean equivalent =
    this->type_.in ()->equivalent (tc.in ()
                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (equivalent)
    {
      // Get the CDR stream of the Any,if there isn't one, make one.
      TAO::Any_Impl *impl = any.impl ();
      TAO_OutputCDR out;
      TAO_InputCDR cdr (static_cast<ACE_Message_Block *> (0));

      if (impl->encoded ())
        {
          TAO::Unknown_IDL_Type *unk =
            dynamic_cast<TAO::Unknown_IDL_Type *> (impl);

          cdr = unk->_tao_get_cdr ();
        }
      else
        {
          impl->marshal_value (out);
          TAO_InputCDR tmp_in (out);
          cdr = tmp_in;
        }

      CORBA::ULong length = static_cast<CORBA::ULong> (this->da_members_.size ());
      CORBA::ULong arg_length = this->get_tc_length (tc.in ()
                                                     ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      if (length != arg_length)
        {
          ACE_THROW (DynamicAny::DynAny::TypeMismatch ());
        }

      CORBA::TypeCode_var field_tc =
        this->get_element_type (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      for (CORBA::ULong i = 0; i < arg_length; ++i)
        {
          CORBA::Any field_any;
          TAO_InputCDR unk_in (cdr);
          TAO::Unknown_IDL_Type *field_unk = 0;
          ACE_NEW (field_unk,
                   TAO::Unknown_IDL_Type (field_tc.in (),
                                          unk_in));
          field_any.replace (field_unk);

          this->da_members_[i]->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK;

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

      this->current_position_ = arg_length ? 0 : -1;
    }
  else
    {
      ACE_THROW (DynamicAny::DynAny::TypeMismatch ());
    }
}

CORBA::Any_ptr
TAO_DynArray_i::to_any (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                        0);
    }

  CORBA::TypeCode_var field_tc =
    this->get_element_type (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  TAO_OutputCDR out_cdr;
  CORBA::Any_var field_any;
  size_t length = this->da_members_.size ();

  for (size_t i = 0; i < length; ++i)
    {
      // Recursive step.
      field_any =
        this->da_members_[i]->to_any (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      TAO::Any_Impl *field_impl = field_any->impl ();
      TAO_OutputCDR field_out;
      TAO_InputCDR field_cdr (static_cast<ACE_Message_Block *> (0));

      if (field_impl->encoded ())
        {
          TAO::Unknown_IDL_Type *field_unk =
            dynamic_cast<TAO::Unknown_IDL_Type *> (field_impl);

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
                                           in_cdr),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  retval->replace (unk);
  return retval;
}

CORBA::Boolean
TAO_DynArray_i::equal (DynamicAny::DynAny_ptr rhs
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
      rhs->seek (static_cast<CORBA::Long> (i)
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
TAO_DynArray_i::destroy (ACE_ENV_SINGLE_ARG_DECL)
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
TAO_DynArray_i::current_component (ACE_ENV_SINGLE_ARG_DECL)
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

  CORBA::ULong index = static_cast<CORBA::ULong> (this->current_position_);

  this->set_flag (this->da_members_[index].in (),
                  0
                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (DynamicAny::DynAny::_nil ());

  return DynamicAny::DynAny::_duplicate (
            this->da_members_[index].in ()
          );
}

TAO_END_VERSIONED_NAMESPACE_DECL
