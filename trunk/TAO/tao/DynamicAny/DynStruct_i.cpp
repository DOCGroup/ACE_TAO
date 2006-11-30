// $Id$

#include "tao/DynamicAny/DynStruct_i.h"
#include "tao/DynamicAny/DynAnyFactory.h"
#include "tao/DynamicAny/DynAnyUtils_T.h"

#include "tao/AnyTypeCode/Marshal.h"
#include "tao/AnyTypeCode/Any_Unknown_IDL_Type.h"
#include "tao/AnyTypeCode/AnyTypeCode_methods.h"

#include "tao/CDR.h"

ACE_RCSID (DynamicAny,
           DynStruct_i,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_DynStruct_i::TAO_DynStruct_i (void)
{
}

TAO_DynStruct_i::~TAO_DynStruct_i (void)
{
}

void
TAO_DynStruct_i::check_typecode (CORBA::TypeCode_ptr tc
                                 ACE_ENV_ARG_DECL)
{
  CORBA::TCKind kind = TAO_DynAnyFactory::unalias (tc
                                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (kind == CORBA::tk_struct || kind == CORBA::tk_except)
    {
      return;
    }

  ACE_THROW (DynamicAny::DynAnyFactory::InconsistentTypeCode ());
}

void
TAO_DynStruct_i::init_common (void)
{
  this->ref_to_component_ = false;
  this->container_is_destroying_ = false;
  this->has_components_ = true;
  this->destroyed_ = false;
  this->component_count_ = static_cast <CORBA::ULong> (this->da_members_.size ());
  this->current_position_ = this->component_count_ ? 0 : -1;
}

void
TAO_DynStruct_i::init (const CORBA::Any& any
                       ACE_ENV_ARG_DECL)
{
  CORBA::TypeCode_ptr tc = any._tao_get_typecode ();
  this->check_typecode (tc
                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->type_ = CORBA::TypeCode::_duplicate (tc);

  this->set_from_any (any
                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}


// This code is common to from_any() and the init() overload that takes
// an Any argument.
void
TAO_DynStruct_i::set_from_any (const CORBA::Any & any
                               ACE_ENV_ARG_DECL)
{
  // member_type() does not work with aliased type codes.
  CORBA::TypeCode_var unaliased_tc =
    TAO_DynAnyFactory::strip_alias (any._tao_get_typecode ()
                                    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::ULong numfields =
    unaliased_tc->member_count (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // Resize the array.
  this->da_members_.size (numfields);

  this->init_common ();

  // Get the CDR stream of the Any, if there isn't one, make one.
  TAO::Any_Impl *impl = any.impl ();
  TAO_OutputCDR out;
  TAO_InputCDR in (static_cast<ACE_Message_Block *> (0));
  TAO::Unknown_IDL_Type *unk = 0;

  if (impl->encoded ())
    {
      unk = dynamic_cast<TAO::Unknown_IDL_Type *> (impl);

      in = unk->_tao_get_cdr ();
    }
  else
    {
      impl->marshal_value (out);
      TAO_InputCDR tmp_in (out);
      in = tmp_in;
    }

  // If we have an exception type, unmarshal the repository ID.
  CORBA::TCKind kind = TAO_DynAnyFactory::unalias (any._tao_get_typecode ()
                                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (kind == CORBA::tk_except)
    {
      CORBA::String_var str;
      in >> str.out ();
    }

  for (CORBA::ULong i = 0; i < numfields; i++)
    {
      CORBA::TypeCode_var field_tc =
        unaliased_tc->member_type (i
                                   ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      CORBA::Any field_any;
      TAO_InputCDR unk_in (in);
      ACE_NEW (unk,
               TAO::Unknown_IDL_Type (field_tc.in (),
                                      unk_in));
      field_any.replace (unk);

      // This recursive step will call the correct constructor
      // based on the type of field_any.
      this->da_members_[i] =
        TAO::MakeDynAnyUtils<const CORBA::Any&>::make_dyn_any_t (
          field_any._tao_get_typecode (),
          field_any);

      // Move to the next field in the CDR stream.
      (void) TAO_Marshal_Object::perform_skip (field_tc.in (),
                                               &in
                                               ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
}

void
TAO_DynStruct_i::init (CORBA::TypeCode_ptr tc
                       ACE_ENV_ARG_DECL)
{
  this->check_typecode (tc
                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->type_ = CORBA::TypeCode::_duplicate (tc);

  // member_type() does not work with aliased type codes.
  CORBA::TypeCode_var unaliased_tc =
  TAO_DynAnyFactory::strip_alias (this->type_.in ()
                                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->component_count_ =
    unaliased_tc->member_count (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // Resize the array.
  this->da_members_.size (this->component_count_);

  this->init_common ();

  CORBA::TypeCode_var mtype;

  for (CORBA::ULong i = 0; i < this->component_count_; ++i)
    {
      mtype = unaliased_tc->member_type (i
                                         ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      // Recursively initialize each member.
      this->da_members_[i] =
        TAO::MakeDynAnyUtils<CORBA::TypeCode_ptr>::make_dyn_any_t (
          mtype.in (),
          mtype.in ());
    }
}

// ****************************************************************

TAO_DynStruct_i *
TAO_DynStruct_i::_narrow (CORBA::Object_ptr _tao_objref
                          ACE_ENV_ARG_DECL_NOT_USED)
{
  if (CORBA::is_nil (_tao_objref))
    {
      return 0;
    }

  return dynamic_cast<TAO_DynStruct_i *> (_tao_objref);
}

// ****************************************************************

DynamicAny::FieldName
TAO_DynStruct_i::current_member_name (ACE_ENV_SINGLE_ARG_DECL)
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

  // Is this an empty exception?
  if (this->component_count_ == 0)
    {
      ACE_THROW_RETURN (DynamicAny::DynAny::TypeMismatch (),
                        0);
    }

  if (this->current_position_ == -1)
    {
      ACE_THROW_RETURN (DynamicAny::DynAny::InvalidValue (),
                        0);
    }

  // member_name() does not work with aliased type codes.
  CORBA::TypeCode_var unaliased_tc =
    TAO_DynAnyFactory::strip_alias (this->type_.in ()
                                    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);
  const char *name =
    unaliased_tc->member_name (this->current_position_
                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return CORBA::string_dup (name);
}

// Returns the unaliased TCKind.
CORBA::TCKind
TAO_DynStruct_i::current_member_kind (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                        CORBA::tk_null);
    }

  // Is this an empty exception?
  if (this->component_count_ == 0)
    {
      ACE_THROW_RETURN (DynamicAny::DynAny::TypeMismatch (),
                        CORBA::tk_null);
    }

  if (this->current_position_ == -1)
    {
      ACE_THROW_RETURN (DynamicAny::DynAny::InvalidValue (),
                        CORBA::tk_null);
    }

  // member_type() does not work with aliased type codes.
  CORBA::TypeCode_var unaliased_tc =
    TAO_DynAnyFactory::strip_alias (this->type_.in ()
                                    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::tk_null);
  CORBA::TypeCode_var tc =
  unaliased_tc->member_type (this->current_position_
                                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::tk_null);

  CORBA::TCKind retval = TAO_DynAnyFactory::unalias (tc.in ()
                                                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::tk_null);

  return retval;
}

DynamicAny::NameValuePairSeq *
TAO_DynStruct_i::get_members (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                        0);
    }

  DynamicAny::NameValuePairSeq* members;
  ACE_NEW_THROW_EX (members,
                    DynamicAny::NameValuePairSeq (this->component_count_),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  // We must do this explicitly.
  members->length (this->component_count_);

  DynamicAny::NameValuePairSeq_var safe_retval = members;
  CORBA::Any_var temp;
  CORBA::TypeCode_var unaliased_tc;

  CORBA::TCKind const tc_kind =
    this->type_->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  if (tc_kind == CORBA::tk_alias)
    {
      unaliased_tc =
        TAO_DynAnyFactory::strip_alias (this->type_.in ()
                                        ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);
    }
  else
    {
      unaliased_tc = CORBA::TypeCode::_duplicate (this->type_.in ());
    }

  // Assign name and value to each pearl on the string.
  for (CORBA::ULong i = 0; i < this->component_count_; ++i)
    {
      safe_retval[i].id =
        CORBA::string_dup (unaliased_tc->member_name (i
                                                      ACE_ENV_ARG_PARAMETER));
      ACE_CHECK_RETURN (0);

      temp = this->da_members_[i]->to_any (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      safe_retval[i].value = temp.in ();
    }

  return safe_retval._retn ();
}

void
TAO_DynStruct_i::set_members (const DynamicAny::NameValuePairSeq & values
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

  CORBA::ULong length = values.length ();

  // Check for length match.
  if (length != this->component_count_)
    {
      ACE_THROW (DynamicAny::DynAny::InvalidValue ());
    }

  CORBA::TypeCode_var value_tc;
  CORBA::TypeCode_var my_tc;
  CORBA::Boolean equivalent;

  // member_type() does not work with aliased type codes.
  CORBA::TypeCode_var unaliased_tc =
    TAO_DynAnyFactory::strip_alias (this->type_.in ()
                                    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  for (CORBA::ULong i = 0; i < length; ++i)
    {
      // Check for type and name match.
      my_tc = unaliased_tc->member_type (i
                                         ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      value_tc = values[i].value.type ();

      equivalent = my_tc->equivalent (value_tc.in ()
                                      ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      if (!equivalent)
        {
          ACE_THROW (DynamicAny::DynAny::TypeMismatch ());
        }

      this->da_members_[i]->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      this->da_members_[i] =
        TAO::MakeDynAnyUtils<const CORBA::Any&>::make_dyn_any_t (
          values[i].value._tao_get_typecode (),
          values[i].value);
    }

  this->current_position_ = length ? 0 : -1;
}

DynamicAny::NameDynAnyPairSeq *
TAO_DynStruct_i::get_members_as_dyn_any (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                        0);
    }

  DynamicAny::NameDynAnyPairSeq * members;
  ACE_NEW_THROW_EX (members,
                    DynamicAny::NameDynAnyPairSeq (this->component_count_),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  // We must do this explicitly.
  members->length (this->component_count_);

  DynamicAny::NameDynAnyPairSeq_var safe_retval = members;

  // member_name() does not work with aliased type codes.
  CORBA::TypeCode_var unaliased_tc =
    TAO_DynAnyFactory::strip_alias (this->type_.in ()
                                    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  // Assign name and value to each pearl on the string.
  for (CORBA::ULong i = 0; i < this->component_count_; ++i)
    {
      safe_retval[i].id =
        CORBA::string_dup (unaliased_tc->member_name (i
                                                      ACE_ENV_ARG_PARAMETER));
      ACE_CHECK_RETURN (0);

      // A deep copy is made only by copy() (CORBA 2.4.2 section 9.2.3.6).
      // Set the flag so the caller can't destroy.
      this->set_flag (this->da_members_[i].in (),
                      0
                      ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      safe_retval[i].value =
        DynamicAny::DynAny::_duplicate (this->da_members_[i].in ());
      ACE_CHECK_RETURN (0);
    }

  return safe_retval._retn ();
}

void
TAO_DynStruct_i::set_members_as_dyn_any (
    const DynamicAny::NameDynAnyPairSeq & values
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

  CORBA::ULong length = values.length ();

  // Check for length match.
  if (length != this->component_count_)
    {
      ACE_THROW (DynamicAny::DynAny::InvalidValue ());
    }

  CORBA::TypeCode_var value_tc;
  CORBA::TypeCode_var my_tc;
  CORBA::Boolean equivalent;

  // member_type() does not work with aliased type codes.
  CORBA::TypeCode_var unaliased_tc =
    TAO_DynAnyFactory::strip_alias (this->type_.in ()
                                    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  for (CORBA::ULong i = 0; i < length; ++i)
    {
      // Check for type and name match.
      my_tc = unaliased_tc->member_type (i
                                         ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      value_tc = values[i].value->type (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      equivalent = my_tc->equivalent (value_tc.in ()
                                      ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      if (!equivalent)
        {
          ACE_THROW (DynamicAny::DynAny::TypeMismatch ());
        }

      this->da_members_[i]->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      this->da_members_[i] =
        values[i].value->copy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
    }

  this->current_position_ = length ? 0 : -1;
}

// ****************************************************************

void
TAO_DynStruct_i::from_any (const CORBA::Any & any
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
    this->type_->equivalent (tc.in ()
                             ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (equivalent)
    {
      // Get the CDR stream of the Any, if there isn't one, make one.
      TAO::Any_Impl *impl = any.impl ();
      TAO_OutputCDR out;
      TAO_InputCDR in (static_cast<ACE_Message_Block *> (0));
      TAO::Unknown_IDL_Type *unk = 0;

      if (impl->encoded ())
        {
          unk = dynamic_cast<TAO::Unknown_IDL_Type *> (impl);

          in = unk->_tao_get_cdr ();
        }
      else
        {
          impl->marshal_value (out);
          TAO_InputCDR tmp_in (out);
          in = tmp_in;
        }

      // If we have an exception type, unmarshal the repository ID.
      CORBA::TCKind kind =
        TAO_DynAnyFactory::unalias (this->type_.in ()
                                    ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      if (kind == CORBA::tk_except)
        {
          CORBA::String_var str;
          in >> str.out ();
        }

      CORBA::TypeCode_var field_tc;
      CORBA::TypeCode_var unaliased =
        TAO::unaliased_typecode (this->type_.in ()
                                 ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      for (CORBA::ULong i = 0; i < this->component_count_; ++i)
        {
          field_tc = unaliased->member_type (i
                                             ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;

          CORBA::Any field_any;
          TAO_InputCDR unk_in (in);
          TAO::Unknown_IDL_Type *unk = 0;
          ACE_NEW (unk,
                   TAO::Unknown_IDL_Type (field_tc.in (),
                                          unk_in));
          field_any.replace (unk);

          this->da_members_[i]->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK;

          this->da_members_[i] =
            TAO::MakeDynAnyUtils<const CORBA::Any&>::make_dyn_any_t (
              field_any._tao_get_typecode (),
              field_any);

          // Move to the next field in the CDR stream.
          (void) TAO_Marshal_Object::perform_skip (field_tc.in (),
                                                   &in
                                                   ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
        }

      this->current_position_ = this->component_count_ ? 0 : -1;
    }
  else
    {
      ACE_THROW (DynamicAny::DynAny::TypeMismatch ());
    }
}

CORBA::Any_ptr
TAO_DynStruct_i::to_any (ACE_ENV_SINGLE_ARG_DECL)
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

  // If we have an exception type, marshal the repository ID.
  CORBA::TCKind kind = TAO_DynAnyFactory::unalias (this->type_.in ()
                                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  if (kind == CORBA::tk_except)
    {
      out_cdr << this->type_->id (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);
    }

  TAO::Any_Impl *field_impl = 0;
  TAO::Unknown_IDL_Type *field_unk = 0;
  TAO_InputCDR field_in_cdr (static_cast<ACE_Message_Block *> (0));

  for (CORBA::ULong i = 0; i < this->component_count_; ++i)
    {
      CORBA::TypeCode_var field_tc =
        this->da_members_[i]->type (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      // Recursive step.
      CORBA::Any_var field_any =
        this->da_members_[i]->to_any (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      TAO_OutputCDR field_out_cdr;
      field_impl = field_any->impl ();

      if (field_impl->encoded ())
        {
          field_unk =
            dynamic_cast<TAO::Unknown_IDL_Type *> (field_impl);

          field_in_cdr = field_unk->_tao_get_cdr ();
        }
      else
        {
          field_impl->marshal_value (field_out_cdr);
          TAO_InputCDR tmp (field_out_cdr);
          field_in_cdr = tmp;
        }

      (void) TAO_Marshal_Object::perform_append (field_tc.in (),
                                                 &field_in_cdr,
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
TAO_DynStruct_i::equal (DynamicAny::DynAny_ptr rhs
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
      rhs->seek (static_cast <CORBA::Long> (i)
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
TAO_DynStruct_i::destroy (ACE_ENV_SINGLE_ARG_DECL)
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
TAO_DynStruct_i::current_component (ACE_ENV_SINGLE_ARG_DECL)
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

  if (this->component_count_ == 0)
    {
      // Empty exception - no component.
      ACE_THROW_RETURN (DynamicAny::DynAny::TypeMismatch (),
                        DynamicAny::DynAny::_nil ());
    }

  if (this->current_position_ == -1)
    {
      return DynamicAny::DynAny::_nil ();
    }

  CORBA::ULong index = static_cast <CORBA::ULong> (this->current_position_);

  this->set_flag (this->da_members_[index].in (),
                  0
                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (DynamicAny::DynAny::_nil ());

  return DynamicAny::DynAny::_duplicate (
            this->da_members_[index].in ()
          );
}

TAO_END_VERSIONED_NAMESPACE_DECL
