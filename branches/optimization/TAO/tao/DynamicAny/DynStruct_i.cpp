/* -*- C++ -*- */
// $Id$

// ====================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    DynStruct_i.cpp
//
// = AUTHOR
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ====================================================================

#include "DynStruct_i.h"

#include "DynAnyFactory.h"
#include "tao/Marshal.h"

ACE_RCSID(DynamicAny, DynStruct_i, "$Id$")

// Constructors and destructor

TAO_DynStruct_i::TAO_DynStruct_i (void)
{
}

TAO_DynStruct_i::~TAO_DynStruct_i (void)
{
}

void
TAO_DynStruct_i::check_typecode (CORBA::TypeCode_ptr tc,
                                 CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::TCKind kind =
    TAO_DynAnyFactory::unalias (tc, ACE_TRY_ENV);
  ACE_CHECK;

  if (kind == CORBA::tk_struct || kind == CORBA::tk_except)
    return;
  ACE_THROW (DynamicAny::DynAnyFactory::InconsistentTypeCode ());
}

void
TAO_DynStruct_i::init (const CORBA_Any& any,
                       CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::TypeCode_var tc = any.type ();
  this->check_typecode (tc.in (), ACE_TRY_ENV);
  ACE_CHECK;

  this->type_ = tc._retn ();
  this->current_index_ = 0;

  CORBA::ULong numfields =
    this->type_->member_count (ACE_TRY_ENV);
  ACE_CHECK;

  // Resize the array.
  this->da_members_.size (numfields);

  // Get the CDR stream of the argument.
  ACE_Message_Block *mb = any._tao_get_cdr ();

  TAO_InputCDR cdr (mb,
                    any._tao_byte_order ());

  for (CORBA::ULong i = 0; i < numfields; i++)
    {
      CORBA::TypeCode_var field_tc =
        this->type_->member_type (i,
                                  ACE_TRY_ENV);
      ACE_CHECK;

      // This Any constructor is a TAO extension.
      CORBA_Any field_any (field_tc.in (),
                           0,
                           cdr.byte_order (),
                           cdr.start ());

      // This recursive step will call the correct constructor
      // based on the type of field_any.
      this->da_members_[i] =
        TAO_DynAnyFactory::make_dyn_any (field_any,
                                                   ACE_TRY_ENV);
      ACE_CHECK;

      // Move to the next field in the CDR stream.
      (void) TAO_Marshal_Object::perform_skip (field_tc.in (),
                                               &cdr,
                                               ACE_TRY_ENV);
      ACE_CHECK;
    }
}

void
TAO_DynStruct_i::init (CORBA_TypeCode_ptr tc,
                       CORBA::Environment &ACE_TRY_ENV)
{
  this->check_typecode (tc, ACE_TRY_ENV);
  ACE_CHECK;

  this->type_ = CORBA::TypeCode::_duplicate (tc);
  this->current_index_ = 0;

  CORBA::ULong numfields = tc->member_count (ACE_TRY_ENV);
  ACE_CHECK;

  // Resize the array.
  this->da_members_.size (numfields);

  for (CORBA::ULong i = 0; i < numfields; i++)
    {
      // With a typecode arg, we just create the top level.
      this->da_members_[i] = 0;
    }
}

// ****************************************************************

TAO_DynStruct_i *
TAO_DynStruct_i::_narrow (CORBA::Object_ptr obj,
                          CORBA::Environment &)
{
  if (CORBA::is_nil (obj))
    {
      return 0;
    }

  return ACE_reinterpret_cast (
             TAO_DynStruct_i*,
             obj->_tao_QueryInterface (
                      ACE_reinterpret_cast (
                          ptr_arith_t,
                          &TAO_DynStruct_i::_narrow
                        )
                    )
           );
}

void*
TAO_DynStruct_i::_tao_QueryInterface (ptr_arith_t type)
{
  ptr_arith_t mytype =
    ACE_reinterpret_cast (ptr_arith_t,
                          &TAO_DynStruct_i::_narrow);
  if (type == mytype)
    {
      this->_add_ref ();
      return this;
    }

  return 
    this->ACE_NESTED_CLASS (DynamicAny, DynStruct::_tao_QueryInterface) (type);
}

// ****************************************************************

DynamicAny::FieldName
TAO_DynStruct_i::current_member_name (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  const char *name =
    this->type_->member_name (this->current_index_, ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);
  return CORBA::string_dup (name);
}

// Returns the unaliased TCKind.
CORBA::TCKind
TAO_DynStruct_i::current_member_kind (CORBA::Environment& ACE_TRY_ENV)
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  CORBA::TypeCode_var tc = this->type_.in ()->member_type (this->current_index_,
                                                           ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::tk_null);

  CORBA::TCKind retval = TAO_DynAnyFactory::unalias (tc.in (),
                                                ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::tk_null);

  return retval;
}

DynamicAny::NameValuePairSeq*
TAO_DynStruct_i::get_members (CORBA::Environment& ACE_TRY_ENV)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  CORBA::ULong length = this->da_members_.size ();

  DynamicAny::NameValuePairSeq* members;
  ACE_NEW_THROW_EX (members,
                    DynamicAny::NameValuePairSeq (length),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  // We must do this explicitly.
  members->length (length);

  // Assign name and value to each pearl on the string.
  for (CORBA::ULong i = 0; i < length; i++)
    {
      (*members)[i].id =
        CORBA::string_dup (this->type_.in ()->member_name (i));

      CORBA::Any_var temp = this->da_members_[i]->to_any (ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      (*members)[i].value = temp.in ();
    }

  return members;
}

void
TAO_DynStruct_i::set_members (const DynamicAny::NameValuePairSeq& value,
                              CORBA::Environment& ACE_TRY_ENV)
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  CORBA::ULong length = value.length ();

  // Check for length match.
  if (length != this->da_members_.size ())
    ACE_THROW (DynamicAny::DynAny::InvalidValue ());

  CORBA::TypeCode_var value_tc;

  for (CORBA::ULong i = 0; i < length; i++)
    {
      // Check for type and name match.
      CORBA_TypeCode_var tc =
        this->type_->member_type (i, ACE_TRY_ENV);
      ACE_CHECK;

      value_tc = value[i].value.type ();

      if (!value_tc->equal (tc.in ())
          || ACE_OS::strcmp (value[i].id,
                             this->type_.in ()->member_name (i)) != 0)
        ACE_THROW (DynamicAny::DynAny::InvalidValue ());

      if (!CORBA::is_nil (this->da_members_[i].in ()))
        {
          this->da_members_[i]->destroy (ACE_TRY_ENV);
          ACE_CHECK;
        }

      this->da_members_[i] =
        TAO_DynAnyFactory::make_dyn_any (value[i].value,
                                                   ACE_TRY_ENV);
      ACE_CHECK;
    }
}

DynamicAny::NameDynAnyPairSeq *
TAO_DynStruct_i::get_members_as_dyn_any (
      CORBA::Environment &
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  // @@ TODO
  return 0;
}

void
TAO_DynStruct_i::set_members_as_dyn_any (
      const DynamicAny::NameDynAnyPairSeq &,
      CORBA::Environment &
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
}

// ****************************************************************

CORBA::TypeCode_ptr
TAO_DynStruct_i::type (CORBA::Environment &)
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ))
{
  return CORBA::TypeCode::_duplicate (this->type_.in ());
}

void
TAO_DynStruct_i::assign (DynamicAny::DynAny_ptr dyn_any,
                         CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch
      ))
{
  TAO_DynStruct_i *rhs =
    TAO_DynStruct_i::_narrow (dyn_any, ACE_TRY_ENV);
  ACE_CHECK;

  if (rhs == 0)
    ACE_THROW (DynamicAny::DynAny::TypeMismatch ());

  this->type_ = rhs->type_;
  this->current_index_ = 0;
  this->da_members_ = rhs->da_members_;
}

void
TAO_DynStruct_i::from_any (const CORBA_Any& any,
                           CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  CORBA::TypeCode_var tc = any.type ();
  CORBA::Boolean equal = this->type_.in ()->equal (tc.in (),
                                                   ACE_TRY_ENV);
  ACE_CHECK;

  if (equal)
    {
      // Get the CDR stream of the argument.
      ACE_Message_Block* mb = any._tao_get_cdr ();
      TAO_InputCDR cdr (mb,
                        any._tao_byte_order ());

      // If we have an exception type, unmarshal the repository ID.
      CORBA::TCKind kind = TAO_DynAnyFactory::unalias (this->type_.in (),
                                                  ACE_TRY_ENV);
      ACE_CHECK;

      if (kind == CORBA::tk_except)
        {
          CORBA::String_var str;
          cdr >> str.out();
        }

      for (CORBA::ULong i = 0;
           i < this->da_members_.size ();
           i++)
        {
          CORBA::TypeCode_var field_tc =
            this->type_.in ()->member_type (i,
                                            ACE_TRY_ENV);
          ACE_CHECK;

          // This Any constructor is a TAO extension.
          CORBA_Any field_any (field_tc.in (),
                               0,
                               cdr.byte_order (),
                               cdr.start ());

          if (!CORBA::is_nil (this->da_members_[i].in ()))
            {
              this->da_members_[i]->destroy (ACE_TRY_ENV);
              ACE_CHECK;
            }

          this->da_members_[i] =
            TAO_DynAnyFactory::make_dyn_any (field_any,
                                                       ACE_TRY_ENV);
          ACE_CHECK;

          // Move to the next field in the CDR stream.
          (void) TAO_Marshal_Object::perform_skip (field_tc.in (),
                                                   &cdr,
                                                   ACE_TRY_ENV);
          ACE_CHECK;
        }
    }
  else
    {
      ACE_THROW (DynamicAny::DynAny::InvalidValue ());
    }
}

CORBA::Any_ptr
TAO_DynStruct_i::to_any (CORBA::Environment& ACE_TRY_ENV)
      ACE_THROW_SPEC ((
                       CORBA::SystemException
      ))
{
  TAO_OutputCDR out_cdr;

  // If we have an exception type, marshal the repository ID.
  CORBA::TCKind kind = TAO_DynAnyFactory::unalias (this->type_.in (),
                                              ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  if (kind == CORBA::tk_except)
    {
      out_cdr << this->type_->id ();
    }

  for (CORBA::ULong i = 0;
       i < this->da_members_.size ();
       i++)
    {
      // @@@ (JP) This is a workaround until we implement the default
      // intial values defined in CORBA 2.3.1.
      if (!this->da_members_[i].in ())
        {
          ACE_THROW_RETURN (CORBA::BAD_INV_ORDER (),
                            0);
        }

      CORBA_TypeCode_ptr field_tc =
        this->da_members_[i]->type (ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      // Recursive step
      CORBA_Any_var field_any =
        this->da_members_[i]->to_any (ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      ACE_Message_Block *field_mb = field_any->_tao_get_cdr ();

      TAO_InputCDR field_cdr (field_mb,
                              field_any->_tao_byte_order ());

      (void) TAO_Marshal_Object::perform_append (field_tc,
                                                 &field_cdr,
                                                 &out_cdr,
                                                 ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);
    }

  TAO_InputCDR in_cdr (out_cdr);

  CORBA_Any *retval;

  CORBA_TypeCode_ptr tc = this->type (ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  ACE_NEW_THROW_EX (retval,
                    CORBA_Any (tc,
                               0,
                               in_cdr.byte_order (),
                               in_cdr.start ()),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  return retval;
}

CORBA::Boolean
TAO_DynStruct_i::equal (
        DynamicAny::DynAny_ptr,
        CORBA::Environment &ACE_TRY_ENV
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

void
TAO_DynStruct_i::destroy (CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ))
{
  // Do a deep destroy
  for (CORBA::ULong i = 0; i < this->da_members_.size (); i++)
    {
      if (!CORBA::is_nil (this->da_members_[i].in ()))
        {
          this->da_members_[i]->destroy (ACE_TRY_ENV);
          ACE_CHECK;
        }
    }
}

DynamicAny::DynAny_ptr
TAO_DynStruct_i::copy (CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ))
{
  CORBA_Any_ptr a = this->to_any (ACE_TRY_ENV);
  ACE_CHECK_RETURN (DynamicAny::DynAny::_nil ());

  DynamicAny::DynAny_ptr retval =
    TAO_DynAnyFactory::make_dyn_any (*a, ACE_TRY_ENV);
  ACE_CHECK_RETURN (DynamicAny::DynAny::_nil ());

  return retval;
}

// ****************************************************************

void
TAO_DynStruct_i::insert_boolean (CORBA::Boolean value,
                                 CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  CORBA::TypeCode_var tc = this->type_.in ()->member_type (this->current_index_);
  CORBA::TCKind kind =
    TAO_DynAnyFactory::unalias (tc.in (), ACE_TRY_ENV);
  ACE_CHECK;

  if (kind == CORBA::tk_boolean)
    {
      DynamicAny::DynAny_ptr dp = this->current_component (ACE_TRY_ENV);
      ACE_CHECK;

      dp->insert_boolean (value,
                          ACE_TRY_ENV);
      ACE_CHECK;

      this->next (ACE_TRY_ENV);
      ACE_CHECK;
    }
  else
    {
      ACE_THROW (DynamicAny::DynAny::InvalidValue ());
    }
}

void
TAO_DynStruct_i::insert_octet (CORBA::Octet value,
                               CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  CORBA::TypeCode_var tc = this->type_.in ()->member_type (this->current_index_);
  CORBA::TCKind kind =
    TAO_DynAnyFactory::unalias (tc.in (), ACE_TRY_ENV);
  ACE_CHECK;

  if (kind == CORBA::tk_octet)
    {
      DynamicAny::DynAny_ptr dp = this->current_component (ACE_TRY_ENV);
      ACE_CHECK;

      dp->insert_octet (value,
                        ACE_TRY_ENV);
      ACE_CHECK;

      this->next (ACE_TRY_ENV);
      ACE_CHECK;
    }
  else
    {
      ACE_THROW (DynamicAny::DynAny::InvalidValue ());
    }
}

void
TAO_DynStruct_i::insert_char (CORBA::Char value,
                              CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{

  CORBA::TypeCode_var tc = this->type_.in ()->member_type (this->current_index_);
  CORBA::TCKind kind =
    TAO_DynAnyFactory::unalias (tc.in (), ACE_TRY_ENV);
  ACE_CHECK;

  if (kind == CORBA::tk_char)
    {
      DynamicAny::DynAny_ptr dp = this->current_component (ACE_TRY_ENV);
      ACE_CHECK;

      dp->insert_char (value,
                       ACE_TRY_ENV);
      ACE_CHECK;

      this->next (ACE_TRY_ENV);
      ACE_CHECK;
    }
  else
    {
      ACE_THROW (DynamicAny::DynAny::InvalidValue ());
    }
}

void
TAO_DynStruct_i::insert_short (CORBA::Short value,
                               CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  CORBA::TypeCode_var tc = this->type_.in ()->member_type (this->current_index_);
  CORBA::TCKind kind =
    TAO_DynAnyFactory::unalias (tc.in (), ACE_TRY_ENV);
  ACE_CHECK;

  if (kind == CORBA::tk_short)
    {
      DynamicAny::DynAny_ptr dp = this->current_component (ACE_TRY_ENV);
      ACE_CHECK;

      dp->insert_short (value,
                        ACE_TRY_ENV);
      ACE_CHECK;

      this->next (ACE_TRY_ENV);
      ACE_CHECK;
    }
  else
    {
      ACE_THROW (DynamicAny::DynAny::InvalidValue ());
    }
}

void
TAO_DynStruct_i::insert_ushort (CORBA::UShort value,
                                CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  CORBA::TypeCode_var tc = this->type_.in ()->member_type (this->current_index_);
  CORBA::TCKind kind =
    TAO_DynAnyFactory::unalias (tc.in (), ACE_TRY_ENV);
  ACE_CHECK;

  if (kind == CORBA::tk_ushort)
    {
      DynamicAny::DynAny_ptr dp = this->current_component (ACE_TRY_ENV);
      ACE_CHECK;

      dp->insert_ushort (value,
                         ACE_TRY_ENV);
      ACE_CHECK;

      this->next (ACE_TRY_ENV);
      ACE_CHECK;
    }
  else
    {
      ACE_THROW (DynamicAny::DynAny::InvalidValue ());
    }
}

void
TAO_DynStruct_i::insert_long (CORBA::Long value,
                              CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  CORBA::TypeCode_var tc = this->type_.in ()->member_type (this->current_index_);
  CORBA::TCKind kind =
    TAO_DynAnyFactory::unalias (tc.in (), ACE_TRY_ENV);
  ACE_CHECK;

  if (kind == CORBA::tk_long)
    {
      DynamicAny::DynAny_ptr dp = this->current_component (ACE_TRY_ENV);
      ACE_CHECK;

      dp->insert_long (value,
                       ACE_TRY_ENV);
      ACE_CHECK;

      this->next (ACE_TRY_ENV);
      ACE_CHECK;
    }
  else
    {
      ACE_THROW (DynamicAny::DynAny::InvalidValue ());
    }
}

void
TAO_DynStruct_i::insert_ulong (CORBA::ULong value,
                               CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  CORBA::TypeCode_var tc = this->type_.in ()->member_type (this->current_index_);
  CORBA::TCKind kind =
    TAO_DynAnyFactory::unalias (tc.in (), ACE_TRY_ENV);
  ACE_CHECK;

  if (kind == CORBA::tk_ulong)
    {
      DynamicAny::DynAny_ptr dp = this->current_component (ACE_TRY_ENV);
      ACE_CHECK;

      dp->insert_ulong (value,
                        ACE_TRY_ENV);
      ACE_CHECK;

      this->next (ACE_TRY_ENV);
      ACE_CHECK;
    }
  else
    {
      ACE_THROW (DynamicAny::DynAny::InvalidValue ());
    }
}

void
TAO_DynStruct_i::insert_float (CORBA::Float value,
                               CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  CORBA::TypeCode_var tc = this->type_.in ()->member_type (this->current_index_);
  CORBA::TCKind kind =
    TAO_DynAnyFactory::unalias (tc.in (), ACE_TRY_ENV);
  ACE_CHECK;

  if (kind == CORBA::tk_float)
    {
      DynamicAny::DynAny_ptr dp = this->current_component (ACE_TRY_ENV);
      ACE_CHECK;

      dp->insert_float (value,
                        ACE_TRY_ENV);
      ACE_CHECK;

      this->next (ACE_TRY_ENV);
      ACE_CHECK;
    }
  else
    {
      ACE_THROW (DynamicAny::DynAny::InvalidValue ());
    }
}

void
TAO_DynStruct_i::insert_double (CORBA::Double value,
                                CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  CORBA::TypeCode_var tc = this->type_.in ()->member_type (this->current_index_);
  CORBA::TCKind kind =
    TAO_DynAnyFactory::unalias (tc.in (), ACE_TRY_ENV);
  ACE_CHECK;

  if (kind == CORBA::tk_double)
    {
      DynamicAny::DynAny_ptr dp = this->current_component (ACE_TRY_ENV);
      ACE_CHECK;

      dp->insert_double (value,
                         ACE_TRY_ENV);
      ACE_CHECK;

      this->next (ACE_TRY_ENV);
      ACE_CHECK;
    }
  else
    {
      ACE_THROW (DynamicAny::DynAny::InvalidValue ());
    }
}

void
TAO_DynStruct_i::insert_string (const char * value,
                                CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  CORBA::TypeCode_var tc = this->type_.in ()->member_type (this->current_index_);
  CORBA::TCKind kind =
    TAO_DynAnyFactory::unalias (tc.in (), ACE_TRY_ENV);
  ACE_CHECK;

  if (kind == CORBA::tk_string)
    {
      DynamicAny::DynAny_ptr dp = this->current_component (ACE_TRY_ENV);
      ACE_CHECK;

      dp->insert_string (value,
                         ACE_TRY_ENV);
      ACE_CHECK;

      this->next (ACE_TRY_ENV);
      ACE_CHECK;
    }
  else
    {
      ACE_THROW (DynamicAny::DynAny::InvalidValue ());
    }
}

void
TAO_DynStruct_i::insert_reference (CORBA::Object_ptr value,
                                   CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  CORBA::TypeCode_var tc = this->type_.in ()->member_type (this->current_index_);
  CORBA::TCKind kind =
    TAO_DynAnyFactory::unalias (tc.in (), ACE_TRY_ENV);
  ACE_CHECK;

  if (kind == CORBA::tk_objref)
    {
      DynamicAny::DynAny_ptr dp = this->current_component (ACE_TRY_ENV);
      ACE_CHECK;

      dp->insert_reference (value,
                            ACE_TRY_ENV);
      ACE_CHECK;

      this->next (ACE_TRY_ENV);
      ACE_CHECK;
    }
  else
    {
      ACE_THROW (DynamicAny::DynAny::InvalidValue ());
    }
}

void
TAO_DynStruct_i::insert_typecode (CORBA::TypeCode_ptr value,
                                  CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  CORBA::TypeCode_var tc = this->type_.in ()->member_type (this->current_index_);
  CORBA::TCKind kind =
    TAO_DynAnyFactory::unalias (tc.in (), ACE_TRY_ENV);
  ACE_CHECK;

  if (kind == CORBA::tk_TypeCode)
    {
      DynamicAny::DynAny_ptr dp = this->current_component (ACE_TRY_ENV);
      ACE_CHECK;

      dp->insert_typecode (value,
                           ACE_TRY_ENV);
      ACE_CHECK;

      this->next (ACE_TRY_ENV);
      ACE_CHECK;
    }
  else
    {
      ACE_THROW (DynamicAny::DynAny::InvalidValue ());
    }
}

void
TAO_DynStruct_i::insert_longlong (CORBA::LongLong value,
                                  CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  CORBA::TypeCode_var tc = this->type_.in ()->member_type (this->current_index_);
  CORBA::TCKind kind =
    TAO_DynAnyFactory::unalias (tc.in (), ACE_TRY_ENV);
  ACE_CHECK;

  if (kind == CORBA::tk_longlong)
    {
      DynamicAny::DynAny_ptr dp = this->current_component (ACE_TRY_ENV);
      ACE_CHECK;

      dp->insert_longlong (value,
                           ACE_TRY_ENV);
      ACE_CHECK;

      this->next (ACE_TRY_ENV);
      ACE_CHECK;
    }
  else
    {
      ACE_THROW (DynamicAny::DynAny::InvalidValue ());
    }
}

void
TAO_DynStruct_i::insert_ulonglong (CORBA::ULongLong value,
                                   CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  CORBA::TypeCode_var tc = this->type_.in ()->member_type (this->current_index_);
  CORBA::TCKind kind =
    TAO_DynAnyFactory::unalias (tc.in (), ACE_TRY_ENV);
  ACE_CHECK;

  if (kind == CORBA::tk_ulonglong)
    {
      DynamicAny::DynAny_ptr dp = this->current_component (ACE_TRY_ENV);
      ACE_CHECK;

      dp->insert_ulonglong (value,
                            ACE_TRY_ENV);
      ACE_CHECK;

      this->next (ACE_TRY_ENV);
      ACE_CHECK;
    }
  else
    {
      ACE_THROW (DynamicAny::DynAny::InvalidValue ());
    }
}

void
TAO_DynStruct_i::insert_longdouble (
        CORBA::LongDouble,
        CORBA::Environment &ACE_TRY_ENV
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

void
TAO_DynStruct_i::insert_wchar (CORBA::WChar value,
                               CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  CORBA::TypeCode_var tc = this->type_.in ()->member_type (this->current_index_);
  CORBA::TCKind kind =
    TAO_DynAnyFactory::unalias (tc.in (), ACE_TRY_ENV);
  ACE_CHECK;

  if (kind == CORBA::tk_wchar)
    {
      DynamicAny::DynAny_ptr dp = this->current_component (ACE_TRY_ENV);
      ACE_CHECK;

      dp->insert_wchar (value,
                        ACE_TRY_ENV);
      ACE_CHECK;

      this->next (ACE_TRY_ENV);
      ACE_CHECK;
    }
  else
    {
      ACE_THROW (DynamicAny::DynAny::InvalidValue ());
    }
}

void
TAO_DynStruct_i::insert_wstring (
        const CORBA::WChar *,
        CORBA::Environment &ACE_TRY_ENV
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

void
TAO_DynStruct_i::insert_any (const CORBA::Any& value,
                             CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  CORBA::TypeCode_var tc = this->type_.in ()->member_type (this->current_index_);
  CORBA::TCKind kind =
    TAO_DynAnyFactory::unalias (tc.in (), ACE_TRY_ENV);
  ACE_CHECK;

  if (kind == CORBA::tk_any)
    {
      DynamicAny::DynAny_ptr dp = this->current_component (ACE_TRY_ENV);
      ACE_CHECK;

      dp->insert_any (value,
                      ACE_TRY_ENV);
      ACE_CHECK;

      this->next (ACE_TRY_ENV);
      ACE_CHECK;
    }
  else
    {
      ACE_THROW (DynamicAny::DynAny::InvalidValue ());
    }
}

void
TAO_DynStruct_i::insert_dyn_any (
        DynamicAny::DynAny_ptr,
        CORBA::Environment &ACE_TRY_ENV
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

#ifdef TAO_HAS_VALUETYPE
void
TAO_DynStruct_i::insert_val (
        CORBA::ValueBase_ptr,
        CORBA::Environment &ACE_TRY_ENV
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}
#endif /* TAO_HAS_VALUETYPE */

// ****************************************************************

// If the current component has not been intialized, these raise
// Invalid, which is not required by the spec, but which seems like a
// courteous thing to do.

CORBA::Boolean
TAO_DynStruct_i::get_boolean (CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  CORBA::Boolean val = 0;
  DynamicAny::DynAny_ptr dp =
    this->da_members_[this->current_index_].in ();

  if (dp)
    {
      CORBA_TypeCode_var tc = dp->type (ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      CORBA::TCKind kind = TAO_DynAnyFactory::unalias (tc.in (),
                                                  ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      if (kind == CORBA::tk_boolean)
        {
          val = dp->get_boolean (ACE_TRY_ENV);
          ACE_CHECK_RETURN (0);

         this->next (ACE_TRY_ENV);
          ACE_CHECK_RETURN (0);
        }
      else
        {
          ACE_THROW_RETURN (DynamicAny::DynAny::TypeMismatch (),
                            val);
        }
    }
  else
    {
      ACE_THROW_RETURN (DynamicAny::DynAny::InvalidValue (),
                        val);
    }

  return val;
}

CORBA::Octet
TAO_DynStruct_i::get_octet (CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  CORBA::Octet val = 0;
  DynamicAny::DynAny_ptr dp =
    this->da_members_[this->current_index_].in ();

  if (dp)
    {
      CORBA_TypeCode_var tc = dp->type (ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      CORBA::TCKind kind = TAO_DynAnyFactory::unalias (tc.in (),
                                                  ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      if (kind == CORBA::tk_octet)
        {
          val = dp->get_octet (ACE_TRY_ENV);
          ACE_CHECK_RETURN (0);

         this->next (ACE_TRY_ENV);
          ACE_CHECK_RETURN (0);
        }
      else
        {
          ACE_THROW_RETURN (DynamicAny::DynAny::TypeMismatch (),
                            val);
        }
    }
  else
    {
      ACE_THROW_RETURN (DynamicAny::DynAny::InvalidValue (),
                        val);
    }

  return val;
}

CORBA::Char
TAO_DynStruct_i::get_char (CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  CORBA::Char val = 0;
  DynamicAny::DynAny_ptr dp =
    this->da_members_[this->current_index_].in ();

  if (dp)
    {
      CORBA_TypeCode_var tc = dp->type (ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      CORBA::TCKind kind = TAO_DynAnyFactory::unalias (tc.in (),
                                                  ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      if (kind == CORBA::tk_char)
        {
          val = dp->get_char (ACE_TRY_ENV);
          ACE_CHECK_RETURN (0);

         this->next (ACE_TRY_ENV);
          ACE_CHECK_RETURN (0);
        }
      else
        {
          ACE_THROW_RETURN (DynamicAny::DynAny::TypeMismatch (),
                            val);
        }
    }
  else
    {
      ACE_THROW_RETURN (DynamicAny::DynAny::InvalidValue (),
                        val);
    }

  return val;
}

CORBA::Short
TAO_DynStruct_i::get_short (CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  CORBA::Short val = 0;
  DynamicAny::DynAny_ptr dp =
    this->da_members_[this->current_index_].in ();

  if (dp)
    {
      CORBA_TypeCode_var tc = dp->type (ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      CORBA::TCKind kind = TAO_DynAnyFactory::unalias (tc.in (),
                                                  ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      if (kind == CORBA::tk_short)
        {
          val = dp->get_short (ACE_TRY_ENV);
          ACE_CHECK_RETURN (0);

         this->next (ACE_TRY_ENV);
          ACE_CHECK_RETURN (0);
        }
      else
        {
          ACE_THROW_RETURN (DynamicAny::DynAny::TypeMismatch (),
                            val);
        }
    }
  else
    {
      ACE_THROW_RETURN (DynamicAny::DynAny::InvalidValue (),
                        val);
    }

  return val;
}

CORBA::UShort
TAO_DynStruct_i::get_ushort (CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  CORBA::UShort val = 0;
  DynamicAny::DynAny_ptr dp =
    this->da_members_[this->current_index_].in ();

  if (dp)
    {
      CORBA_TypeCode_var tc = dp->type (ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      CORBA::TCKind kind = TAO_DynAnyFactory::unalias (tc.in (),
                                                  ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      if (kind == CORBA::tk_ushort)
        {
          val = dp->get_ushort (ACE_TRY_ENV);
          ACE_CHECK_RETURN (0);

         this->next (ACE_TRY_ENV);
          ACE_CHECK_RETURN (0);
        }
      else
        {
          ACE_THROW_RETURN (DynamicAny::DynAny::TypeMismatch (),
                            val);
        }
    }
  else
    {
      ACE_THROW_RETURN (DynamicAny::DynAny::InvalidValue (),
                        val);
    }

  return val;
}

CORBA::Long
TAO_DynStruct_i::get_long (CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  CORBA::Long val = 0;
  DynamicAny::DynAny_ptr dp =
    this->da_members_[this->current_index_].in ();

  if (dp)
    {
      CORBA_TypeCode_var tc = dp->type (ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      CORBA::TCKind kind = TAO_DynAnyFactory::unalias (tc.in (),
                                                  ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      if (kind == CORBA::tk_long)
        {
          val = dp->get_long (ACE_TRY_ENV);
          ACE_CHECK_RETURN (0);

         this->next (ACE_TRY_ENV);
          ACE_CHECK_RETURN (0);
        }
      else
        {
          ACE_THROW_RETURN (DynamicAny::DynAny::TypeMismatch (),
                            val);
        }
    }
  else
    {
      ACE_THROW_RETURN (DynamicAny::DynAny::InvalidValue (),
                        val);
    }

  return val;
}

CORBA::ULong
TAO_DynStruct_i::get_ulong (CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  CORBA::ULong val = 0;
  DynamicAny::DynAny_ptr dp =
    this->da_members_[this->current_index_].in ();

  if (dp)
    {
      CORBA_TypeCode_var tc = dp->type (ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      CORBA::TCKind kind = TAO_DynAnyFactory::unalias (tc.in (),
                                                  ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      if (kind == CORBA::tk_ulong)
        {
          val = dp->get_ulong (ACE_TRY_ENV);
          ACE_CHECK_RETURN (0);

         this->next (ACE_TRY_ENV);
          ACE_CHECK_RETURN (0);
        }
      else
        {
          ACE_THROW_RETURN (DynamicAny::DynAny::TypeMismatch (),
                            val);
        }
    }
  else
    {
      ACE_THROW_RETURN (DynamicAny::DynAny::InvalidValue (),
                        val);
    }

  return val;
}

CORBA::Float
TAO_DynStruct_i::get_float (CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  CORBA::Float val = 0;
  DynamicAny::DynAny_ptr dp =
    this->da_members_[this->current_index_].in ();

  if (dp)
    {
      CORBA_TypeCode_var tc = dp->type (ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      CORBA::TCKind kind = TAO_DynAnyFactory::unalias (tc.in (),
                                                  ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      if (kind == CORBA::tk_float)
        {
          val = dp->get_float (ACE_TRY_ENV);
          ACE_CHECK_RETURN (0);

         this->next (ACE_TRY_ENV);
          ACE_CHECK_RETURN (0);
        }
      else
        {
          ACE_THROW_RETURN (DynamicAny::DynAny::TypeMismatch (),
                            val);
        }
    }
  else
    {
      ACE_THROW_RETURN (DynamicAny::DynAny::InvalidValue (),
                        val);
    }

  return val;
}

CORBA::Double
TAO_DynStruct_i::get_double (CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  CORBA::Double val = 0;
  DynamicAny::DynAny_ptr dp =
    this->da_members_[this->current_index_].in ();

  if (dp)
    {
      CORBA_TypeCode_var tc = dp->type (ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      CORBA::TCKind kind = TAO_DynAnyFactory::unalias (tc.in (),
                                                  ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      if (kind == CORBA::tk_double)
        {
          val = dp->get_double (ACE_TRY_ENV);
          ACE_CHECK_RETURN (0);

         this->next (ACE_TRY_ENV);
          ACE_CHECK_RETURN (0);
        }
      else
        {
          ACE_THROW_RETURN (DynamicAny::DynAny::TypeMismatch (),
                            val);
        }
    }
  else
    {
      ACE_THROW_RETURN (DynamicAny::DynAny::InvalidValue (),
                        val);
    }

  return val;
}

char *
TAO_DynStruct_i::get_string (CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  CORBA::Char *val = 0;
  DynamicAny::DynAny_ptr dp =
    this->da_members_[this->current_index_].in ();

  if (dp)
    {
      CORBA_TypeCode_var tc = dp->type (ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      CORBA::TCKind kind = TAO_DynAnyFactory::unalias (tc.in (),
                                                  ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      if (kind == CORBA::tk_string)
        {
          val = dp->get_string (ACE_TRY_ENV);
          ACE_CHECK_RETURN (0);

         this->next (ACE_TRY_ENV);
          ACE_CHECK_RETURN (0);
        }
      else
        {
          ACE_THROW_RETURN (DynamicAny::DynAny::TypeMismatch (),
                            val);
        }
    }
  else
    {
      ACE_THROW_RETURN (DynamicAny::DynAny::InvalidValue (),
                        val);
    }

  return val;
}

CORBA::Object_ptr
TAO_DynStruct_i::get_reference (CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  CORBA_Object_ptr val = 0;
  DynamicAny::DynAny_ptr dp =
    this->da_members_[this->current_index_].in ();

  if (dp)
    {
      CORBA_TypeCode_var tc = dp->type (ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      CORBA::TCKind kind = TAO_DynAnyFactory::unalias (tc.in (),
                                                  ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      if (kind == CORBA::tk_objref)
        {
          val = dp->get_reference (ACE_TRY_ENV);
          ACE_CHECK_RETURN (0);

         this->next (ACE_TRY_ENV);
          ACE_CHECK_RETURN (0);
        }
      else
        {
          ACE_THROW_RETURN (DynamicAny::DynAny::TypeMismatch (),
                            val);
        }
    }
  else
    {
      ACE_THROW_RETURN (DynamicAny::DynAny::InvalidValue (),
                        val);
    }

  return val;
}

CORBA::TypeCode_ptr
TAO_DynStruct_i::get_typecode (CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  CORBA_TypeCode_ptr val = 0;
  DynamicAny::DynAny_ptr dp =
    this->da_members_[this->current_index_].in ();

  if (dp)
    {
      CORBA_TypeCode_var tc = dp->type (ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      CORBA::TCKind kind = TAO_DynAnyFactory::unalias (tc.in (),
                                                  ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      if (kind == CORBA::tk_TypeCode)
        {
          val = dp->get_typecode (ACE_TRY_ENV);
          ACE_CHECK_RETURN (0);

         this->next (ACE_TRY_ENV);
          ACE_CHECK_RETURN (0);
        }
      else
        {
          ACE_THROW_RETURN (DynamicAny::DynAny::TypeMismatch (),
                            val);
        }
    }
  else
    {
      ACE_THROW_RETURN (DynamicAny::DynAny::InvalidValue (),
                        val);
    }

  return val;
}

CORBA::LongLong
TAO_DynStruct_i::get_longlong (CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
#if defined (ACE_LACKS_LONGLONG_T)
  CORBA::LongLong val = {0, 0};
#else  /* ! ACE_LACKS_LONGLONG_T */
  CORBA::LongLong val = 0;
#endif /* ! ACE_LACKS_LONGLONG_T */

  DynamicAny::DynAny_ptr dp = this->da_members_[this->current_index_].in ();

  if (dp)
    {
      CORBA_TypeCode_var tc = dp->type (ACE_TRY_ENV);
      ACE_CHECK_RETURN (val);

      CORBA::TCKind kind = TAO_DynAnyFactory::unalias (tc.in (),
                                                  ACE_TRY_ENV);
      ACE_CHECK_RETURN (val);

      if (kind == CORBA::tk_longlong)
        {
          val = dp->get_longlong (ACE_TRY_ENV);
          ACE_CHECK_RETURN (val);

         this->next (ACE_TRY_ENV);
          ACE_CHECK_RETURN (val);
        }
      else
        {
          ACE_THROW_RETURN (DynamicAny::DynAny::TypeMismatch (),
                            val);
        }
    }
  else
    {
      ACE_THROW_RETURN (DynamicAny::DynAny::InvalidValue (),
                        val);
    }

  return val;
}

CORBA::ULongLong
TAO_DynStruct_i::get_ulonglong (CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  CORBA::ULongLong val = 0;
  DynamicAny::DynAny_ptr dp =
    this->da_members_[this->current_index_].in ();

  if (dp)
    {
      CORBA_TypeCode_var tc = dp->type (ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      CORBA::TCKind kind = TAO_DynAnyFactory::unalias (tc.in (),
                                                  ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      if (kind == CORBA::tk_ulonglong)
        {
          val = dp->get_ulonglong (ACE_TRY_ENV);
          ACE_CHECK_RETURN (0);

         this->next (ACE_TRY_ENV);
          ACE_CHECK_RETURN (0);
        }
      else
        {
          ACE_THROW_RETURN (DynamicAny::DynAny::TypeMismatch (),
                            val);
        }
    }
  else
    {
      ACE_THROW_RETURN (DynamicAny::DynAny::InvalidValue (),
                        val);
    }

  return val;
}

CORBA::LongDouble
TAO_DynStruct_i::get_longdouble (
        CORBA::Environment &ACE_TRY_ENV
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  CORBA::LongDouble ret = ACE_CDR_LONG_DOUBLE_INITIALIZER;
  ACE_UNUSED_ARG (ret);
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), ret);
}

CORBA::WChar
TAO_DynStruct_i::get_wchar (CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  CORBA::WChar val = 0;
  DynamicAny::DynAny_ptr dp =
    this->da_members_[this->current_index_].in ();

  if (dp)
    {
      CORBA_TypeCode_var tc = dp->type (ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      CORBA::TCKind kind = TAO_DynAnyFactory::unalias (tc.in (),
                                                  ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      if (kind == CORBA::tk_wchar)
        {
          val = dp->get_wchar (ACE_TRY_ENV);
          ACE_CHECK_RETURN (0);

         this->next (ACE_TRY_ENV);
          ACE_CHECK_RETURN (0);
        }
      else
        {
          ACE_THROW_RETURN (DynamicAny::DynAny::TypeMismatch (),
                            val);
        }
    }
  else
    {
      ACE_THROW_RETURN (DynamicAny::DynAny::InvalidValue (),
                        val);
    }

  return val;
}

CORBA::WChar *
TAO_DynStruct_i::get_wstring (
        CORBA::Environment &ACE_TRY_ENV
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

CORBA::Any_ptr
TAO_DynStruct_i::get_any (CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  CORBA_Any_ptr val = 0;
  DynamicAny::DynAny_ptr dp =
    this->da_members_[this->current_index_].in ();

  if (dp)
    {
      CORBA_TypeCode_var tc = dp->type (ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      CORBA::TCKind kind = TAO_DynAnyFactory::unalias (tc.in (),
                                                  ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      if (kind == CORBA::tk_any)
        {
          val = dp->get_any (ACE_TRY_ENV);
          ACE_CHECK_RETURN (0);

         this->next (ACE_TRY_ENV);
          ACE_CHECK_RETURN (0);
        }
      else
        {
          ACE_THROW_RETURN (DynamicAny::DynAny::TypeMismatch (),
                            val);
        }
    }
  else
    {
      ACE_THROW_RETURN (DynamicAny::DynAny::InvalidValue (),
                        val);
    }

  return val;
}

DynamicAny::DynAny_ptr
TAO_DynStruct_i::get_dyn_any (
        CORBA::Environment &ACE_TRY_ENV
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), DynamicAny::DynAny::_nil ());
}

#ifdef TAO_HAS_VALUETYPE
CORBA::ValueBase_ptr
TAO_DynStruct_i::get_val (
        CORBA::Environment &ACE_TRY_ENV
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}
#endif /* TAO_HAS_VALUETYPE */

CORBA::Boolean
TAO_DynStruct_i::seek (CORBA::Long slot,
                       CORBA::Environment &)
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ))
{
  if (slot < 0
      || slot >= (CORBA::Long) this->da_members_.size ())
    return 0;

  this->current_index_ = slot;
  return 1;
}

void
TAO_DynStruct_i::rewind (CORBA::Environment &)
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ))
{
  this->current_index_ = 0;
}

CORBA::Boolean
TAO_DynStruct_i::next (CORBA::Environment &)
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ))
{
  if (this->current_index_ + 1 == (CORBA::Long) this->da_members_.size ())
    return 0;

  ++this->current_index_;
  return 1;
}

CORBA::ULong
TAO_DynStruct_i::component_count (
        CORBA::Environment &ACE_TRY_ENV
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

DynamicAny::DynAny_ptr
TAO_DynStruct_i::current_component (CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch
      ))
{
  if (!this->da_members_[this->current_index_].in ())
    {
      CORBA_TypeCode_var tc =
        this->type_.in ()->member_type (this->current_index_,
                                        ACE_TRY_ENV);
      ACE_CHECK_RETURN (DynamicAny::DynAny::_nil ());

      DynamicAny::DynAny_ptr dp =
        TAO_DynAnyFactory::make_dyn_any (tc.in (),
                                                   ACE_TRY_ENV);
      ACE_CHECK_RETURN (DynamicAny::DynAny::_nil ());

      this->da_members_[this->current_index_] = dp;
    }

  return DynamicAny::DynAny::_duplicate (
             this->da_members_[this->current_index_].in ()
           );
}
