/* -*- C++ -*- */
// $Id$

#include "DynUnion_i.h"
#include "DynAnyFactory.h"
#include "tao/Marshal.h"

ACE_RCSID(DynamicAny, DynUnion_i, "$Id$")

// Constructors and destructor.

TAO_DynUnion_i::TAO_DynUnion_i (void)
{
}

TAO_DynUnion_i::~TAO_DynUnion_i (void)
{
}

void
TAO_DynUnion_i::init_common (void)
{
  this->ref_to_component_ = 0;
  this->container_is_destroying_ = 0;
  this->has_components_ = 1;
  this->destroyed_ = 0;
  this->component_count_ = 2;
  this->current_position_ = 0;
  this->member_slot_ = 0;
}

void
TAO_DynUnion_i::init (const CORBA_Any& any
                      ACE_ENV_ARG_DECL)
{
  CORBA::TypeCode_var tc = any.type ();

  CORBA::TCKind kind = TAO_DynAnyFactory::unalias (tc.in ()
                                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (kind != CORBA::tk_union)
    {
      ACE_THROW (DynamicAny::DynAnyFactory::InconsistentTypeCode ());
    }

  // Initialize the typecode holder.
  this->type_ = tc;

  this->init_common ();

  // Set the from_factory arg to TRUE, so any problems will throw
  // InconsistentTypeCode.
  this->set_from_any (any,
                      1
                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_DynUnion_i::init (CORBA_TypeCode_ptr tc
                      ACE_ENV_ARG_DECL)
{
  CORBA::TCKind kind = TAO_DynAnyFactory::unalias (tc
                                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (kind != CORBA::tk_union)
    {
      ACE_THROW (DynamicAny::DynAnyFactory::InconsistentTypeCode ());
    }

  // Initialize the typecode holder and current index.
  this->type_ = CORBA::TypeCode::_duplicate (tc);

  this->init_common ();

  CORBA::Any_var first_label = tc->member_label (this->current_position_
                                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Initialize the discriminator to the label value of the first member.
  this->discriminator_ =
    TAO_DynAnyFactory::make_dyn_any (first_label.in ()
                                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::TypeCode_var first_type = tc->member_type (this->current_position_
                                                    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Recursively initialize the member to its default value.
  this->member_ = TAO_DynAnyFactory::make_dyn_any (first_type.in ()
                                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

// ****************************************************************

TAO_DynUnion_i *
TAO_DynUnion_i::_narrow (CORBA::Object_ptr obj
                         ACE_ENV_ARG_DECL_NOT_USED)
{
  if (CORBA::is_nil (obj))
    {
      return 0;
    }

  return ACE_reinterpret_cast (
             TAO_DynUnion_i*,
             obj->_tao_QueryInterface (
                      ACE_reinterpret_cast (
                          ptr_arith_t,
                          &TAO_DynUnion_i::_narrow
                        )
                    )
           );
}

void*
TAO_DynUnion_i::_tao_QueryInterface (ptr_arith_t type)
{
  ptr_arith_t mytype =
    ACE_reinterpret_cast (ptr_arith_t,
                          &TAO_DynUnion_i::_narrow);
  if (type == mytype)
    {
      this->_add_ref ();
      return this;
    }

  return
    this->ACE_NESTED_CLASS (DynamicAny, DynUnion::_tao_QueryInterface) (type);
}

// This code is common to from_any() and the init() overload that takes
// an Any argument.
void
TAO_DynUnion_i::set_from_any (const CORBA_Any & any,
                              CORBA::Boolean from_factory
                              ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAnyFactory::InconsistentTypeCode
    ))
{
  // Get the CDR stream of the argument.
  ACE_Message_Block* mb = any._tao_get_cdr ();

  TAO_InputCDR cdr (mb,
                    any._tao_byte_order ());

  CORBA::TypeCode_var tc = any.type ();

  CORBA_TypeCode_var disc_tc = tc->discriminator_type (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CORBA_Any disc_any (disc_tc.in (),
                      0,
                      cdr.byte_order (),
                      cdr.start ());

  // Need this here because we might have been called from init().
  if (!CORBA::is_nil (this->discriminator_.in ()))
    {
      this->discriminator_->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
    }

  // Set the discriminator.
  this->discriminator_ = TAO_DynAnyFactory::make_dyn_any (disc_any
                                                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Move to the next field in the CDR stream.
  (void) TAO_Marshal_Object::perform_skip (disc_tc.in (),
                                           &cdr
                                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::ULong count = tc->member_count (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::Boolean match = 0;
  CORBA::ULong i;

  // Get the index.
  for (i = 0; i < count; ++i)
    {
      CORBA_Any_var label_any = tc->member_label (i
                                                  ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      match = this->label_match (label_any.in (),
                                 disc_any
                                 ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      if (match)
        {
          break;
        }
    }

  // Need this here because we might have been called from init().
  if (!CORBA::is_nil (this->member_.in ()))
    {
      this->member_->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
    }

  if (match)
    {
      CORBA::TypeCode_var member_tc = tc->member_type (i
                                                       ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      CORBA_Any member_any (member_tc.in (),
                            0,
                            cdr.byte_order (),
                            cdr.start ());

      this->member_ =
        TAO_DynAnyFactory::make_dyn_any (member_any
                                         ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      this->member_slot_ = i;
    }
  else
    {
      // If no match, either the Any contains the default member or the
      // type code was bad.

      // default_index() does not work with aliased type codes.
      CORBA::TypeCode_var unaliased_tc =
        TAO_DynAnyFactory::strip_alias (this->type_.in ()
                                        ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      CORBA::Long default_index = unaliased_tc->default_index (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      if (default_index == -1)
        {
          if (from_factory)
            {
              ACE_THROW (DynamicAny::DynAnyFactory::InconsistentTypeCode ());
            }
          else
            {
              ACE_THROW (DynamicAny::DynAny::TypeMismatch ());
            }
        }
      else
        {
          CORBA::ULong index = ACE_static_cast (CORBA::ULong,
                                                default_index);

          CORBA::TypeCode_var default_tc = tc->member_type (index
                                                            ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;

          CORBA_Any default_any (default_tc.in (),
                                 0,
                                 cdr.byte_order (),
                                 cdr.start ());

          this->member_ =
            TAO_DynAnyFactory::make_dyn_any (default_any
                                             ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;

          this->member_slot_ = index;
        }
    }
}

// ****************************************************************

DynamicAny::DynAny_ptr
TAO_DynUnion_i::get_discriminator (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                        DynamicAny::DynAny::_nil ());
    }

  // A deep copy is made only by copy() (CORBA 2.4.2 section 9.2.3.6).
  // Set the flag so the caller can't destroy.
  this->set_flag (this->discriminator_.in (),
                  0
                  ACE_ENV_ARG_PARAMETER);

  return DynamicAny::DynAny::_duplicate (this->discriminator_.in ());
}

void
TAO_DynUnion_i::set_discriminator (DynamicAny::DynAny_ptr value
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

  CORBA::TypeCode_var tc = value->type (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::TypeCode_var disc_tc = this->discriminator_->type (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::Boolean equivalent = disc_tc->equivalent (tc.in ()
                                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (!equivalent)
    {
      ACE_THROW (DynamicAny::DynAny::TypeMismatch ());
    }

  CORBA_Any_var value_any = value->to_any (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::ULong length = this->type_->member_count (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CORBA_Any_var label_any;
  CORBA::ULong i;

  // member_label() does not work with aliased type codes.
  CORBA::TypeCode_var unaliased_tc =
    TAO_DynAnyFactory::strip_alias (this->type_.in ()
                                    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::Boolean match = 0;

  for (i = 0; i < length; ++i)
    {
      label_any = unaliased_tc->member_label (i
                                              ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      match = this->label_match (label_any.in (),
                                 value_any.in ()
                                 ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      if (match)
        {
          break;
        }
    }

  if (match)
    {
      // If the incoming label value matches the one we already
      // have, we do nothing.
      if (i == this->member_slot_)
        {
          return;
        }

      // If we got a match, a named member will be active.
      this->discriminator_->from_any (label_any.in ()
                                      ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      // member_type() does not work with aliased type codes.
      CORBA::TypeCode_var member_tc =
        unaliased_tc->member_type (i
                                   ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      this->member_->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      // Initialize member to default value.
      this->member_ = TAO_DynAnyFactory::make_dyn_any (member_tc.in ()
                                                       ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      // Named active member (CORBA 2.3.1).
      this->current_position_ = 1;
      this->component_count_ = 2;

      this->member_slot_ = i;
    }
  else
    {
      // default_index() does not work with aliased type codes.
      CORBA::TypeCode_var unaliased_tc =
        TAO_DynAnyFactory::strip_alias (this->type_.in ()
                                        ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      // If no match, either the default member or no member is active.
      CORBA::Long default_index = unaliased_tc->default_index (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      if (default_index == -1)
        {
          // This can't throw InvalidValue - if there's no default case
          // and all possible labels are used, we'd have had a match above.
          this->set_to_no_active_member (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK;
        }
      else
        {
          this->set_to_default_member (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK;

          this->member_slot_ = ACE_static_cast (CORBA::ULong,
                                                default_index);
        }
    }

  // If we get here, we update the discriminator no matter what.
  // Any value will correspond to one of the three cases above.
  this->discriminator_->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->discriminator_ = value->copy (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_DynUnion_i::set_to_default_member (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW (CORBA::OBJECT_NOT_EXIST ());
    }

  // default_index() does not work with aliased type codes.
  CORBA::TypeCode_var unaliased_tc =
    TAO_DynAnyFactory::strip_alias (this->type_.in ()
                                    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::Long default_index = unaliased_tc->default_index (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (default_index == -1)
    {
      // No explicit default case.
      ACE_THROW (DynamicAny::DynAny::TypeMismatch ());
    }
  else
    {
      CORBA::ULong index = ACE_static_cast (CORBA::ULong,
                                            default_index);

      CORBA::TypeCode_var default_tc =
        unaliased_tc->member_type (index
                                   ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      this->member_->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      this->member_ = TAO_DynAnyFactory::make_dyn_any (default_tc.in ()
                                                       ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      // Default member active (CORBA 2.3.1).
      this->current_position_ = 0;
      this->component_count_ = 2;

      this->member_slot_ = index;
    }
}

void
TAO_DynUnion_i::set_to_no_active_member (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW (CORBA::OBJECT_NOT_EXIST ());
    }

  // default_index() does not work with aliased type codes.
  CORBA::TypeCode_var unaliased_tc =
    TAO_DynAnyFactory::strip_alias (this->type_.in ()
                                    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::Long default_index = unaliased_tc->default_index (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // Throw an exception is there is an explicit default case or if all
  // possible case label values are used.
  if (default_index != -1)
    {
      ACE_THROW (DynamicAny::DynAny::TypeMismatch ());
    }
  else
    {
      CORBA::TypeCode_var disc_tc = this->discriminator_->type (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      CORBA::TCKind kind = TAO_DynAnyFactory::unalias (disc_tc.in ()
                                                       ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      if (kind == CORBA::tk_enum)
        {
          CORBA::ULong member_count = this->type_->member_count (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK;

          CORBA::ULong label_count = disc_tc->member_count (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK;

          if (member_count == label_count)
            {
              ACE_THROW (DynamicAny::DynAny::TypeMismatch ());
            }
        }
    }

  // No active member (CORBA 2.3.1).
  this->current_position_ = 0;
  this->component_count_ = 1;
}

CORBA::Boolean
TAO_DynUnion_i::has_no_active_member (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                        0);
    }

  // No active member (CORBA 2.3.1).
  return (this->current_position_ == 0
          && this->component_count_ == 1);
}

CORBA::TCKind
TAO_DynUnion_i::discriminator_kind (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                        CORBA::tk_null);
    }

  CORBA_TypeCode_var tc = this->discriminator_->type (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::tk_null);

  CORBA::TCKind retval = TAO_DynAnyFactory::unalias (tc.in ()
                                                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::tk_null);

  return retval;
}

DynamicAny::DynAny_ptr
TAO_DynUnion_i::member (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::InvalidValue
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                        DynamicAny::DynAny::_nil ());
    }

  CORBA::Boolean has_no_active_member =
    this->has_no_active_member (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (DynamicAny::DynAny::_nil ());

  if (has_no_active_member)
    {
      ACE_THROW_RETURN (DynamicAny::DynAny::InvalidValue (),
                        DynamicAny::DynAny::_nil ());
    }

  // A deep copy is made only by copy() (CORBA 2.4.2 section 9.2.3.6).
  // Set the flag so the caller can't destroy.
  this->set_flag (this->member_.in (),
                  0
                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (DynamicAny::DynAny::_nil ());

  return DynamicAny::DynAny::_duplicate (this->member_.in ());
}

char *
TAO_DynUnion_i::member_name (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::InvalidValue
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                        0);
    }

  CORBA::Boolean has_no_active_member =
    this->has_no_active_member (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  if (has_no_active_member)
    {
      ACE_THROW_RETURN (DynamicAny::DynAny::InvalidValue (),
                        0);
    }

  const char *retval = this->type_->member_name (this->member_slot_
                                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return CORBA::string_dup (retval);
}

CORBA::TCKind
TAO_DynUnion_i::member_kind (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::InvalidValue
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                        CORBA::tk_null);
    }

  CORBA::Boolean has_no_active_member =
    this->has_no_active_member (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::tk_null);

  if (has_no_active_member)
    {
      ACE_THROW_RETURN (DynamicAny::DynAny::InvalidValue (),
                        CORBA::tk_null);
    }

  CORBA_TypeCode_var tc = this->member_->type (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::tk_null);

  CORBA::TCKind retval = TAO_DynAnyFactory::unalias (tc.in ()
                                                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::tk_null);

  return retval;
}

// ****************************************************************

void
TAO_DynUnion_i::from_any (const CORBA_Any& any
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
      // CORBA 2.3.1 section 9.2.3.3.
      this->current_position_ = 0;

      // May be changed in set_from_any().
      this->component_count_ = 2;

      // Set the from_factory arg to FALSE, so any problems will throw
      // TypeMismatch.
      this->set_from_any (any,
                          0
                          ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
  else
    {
      ACE_THROW (DynamicAny::DynAny::TypeMismatch ());
    }
}

CORBA::Any_ptr
TAO_DynUnion_i::to_any (ACE_ENV_SINGLE_ARG_DECL)
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

  // Add the discriminator to the CDR stream.

  CORBA_TypeCode_var disc_tc = this->discriminator_->type (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  CORBA_Any_var disc_any = this->discriminator_->to_any (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  ACE_Message_Block *disc_mb = disc_any->_tao_get_cdr ();

  TAO_InputCDR disc_cdr (disc_mb,
                         disc_any->_tao_byte_order ());

  (void) TAO_Marshal_Object::perform_append (disc_tc.in (),
                                             &disc_cdr,
                                             &out_cdr
                                             ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  // Add the member to the CDR stream.

  CORBA_TypeCode_var member_tc = this->member_->type (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  CORBA_Any_var member_any = this->member_->to_any (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  ACE_Message_Block *member_mb = member_any->_tao_get_cdr ();

  TAO_InputCDR member_cdr (member_mb,
                           member_any->_tao_byte_order ());

  (void) TAO_Marshal_Object::perform_append (member_tc.in (),
                                             &member_cdr,
                                             &out_cdr
                                             ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  // Make the Any.
  TAO_InputCDR in_cdr (out_cdr);

  CORBA_Any_ptr retval = 0;
  ACE_NEW_THROW_EX (retval,
                    CORBA_Any (this->type_.in (),
                               0,
                               in_cdr.byte_order (),
                               in_cdr.start ()),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  return retval;
}

CORBA::Boolean
TAO_DynUnion_i::equal (DynamicAny::DynAny_ptr rhs
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

  TAO_DynUnion_i *impl = TAO_DynUnion_i::_narrow (rhs
                                                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  if (impl == 0)
    {
      return 0;
    }

  CORBA::Boolean equivalent = this->type_->equivalent (impl->type_.in ()
                                                       ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  if (!equivalent)
    {
      return 0;
    }

  CORBA::Boolean member_equal = this->member_->equal (impl->member_.in ()
                                                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  CORBA::Boolean disc_equal =
    this->discriminator_->equal (impl->discriminator_.in ()
                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  impl->_remove_ref ();

  return (member_equal && disc_equal);
}

void
TAO_DynUnion_i::destroy (ACE_ENV_SINGLE_ARG_DECL)
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
      // Free the two components.

      this->set_flag (this->member_.in (),
                      1
                      ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      this->member_->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      this->set_flag (this->discriminator_.in (),
                      1
                      ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      this->discriminator_->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      this->destroyed_ = 1;
    }
}

DynamicAny::DynAny_ptr
TAO_DynUnion_i::current_component (ACE_ENV_SINGLE_ARG_DECL)
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

  if (this->current_position_ == 1)
    {
      this->set_flag (this->member_.in (),
                      0
                      ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (DynamicAny::DynAny::_nil ());

      return DynamicAny::DynAny::_duplicate (this->member_.in ());
    }
  else
    {
      this->set_flag (this->discriminator_.in (),
                      0
                      ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (DynamicAny::DynAny::_nil ());

      return DynamicAny::DynAny::_duplicate (this->discriminator_.in ());
    }
}

// ****************************************************************

CORBA::Boolean
TAO_DynUnion_i::label_match (const CORBA_Any &my_any,
                             const CORBA_Any &other_any
                             ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  // Use my_any so we can detect a default case label,
  // if we are iterating through the union type code's
  // member_label() calls.
  CORBA::TypeCode_var tc = my_any.type ();

  CORBA::TCKind kind = TAO_DynAnyFactory::unalias (tc.in ()
                                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  // No need to do any type checking - it was done before this
  // call was made.
  switch (kind)
  {
    case CORBA::tk_octet:
      // Default case label - just skip it.
      return 0;
    case CORBA::tk_short:
      {
        CORBA::Short my_val;
        CORBA::Short other_val;
        my_any >>= my_val;
        other_any >>= other_val;
        return my_val == other_val;
      }
    case CORBA::tk_long:
      {
        CORBA::Long my_val;
        CORBA::Long other_val;
        my_any >>= my_val;
        other_any >>= other_val;
        return my_val == other_val;
      }
    case CORBA::tk_ushort:
      {
        CORBA::UShort my_val;
        CORBA::UShort other_val;
        my_any >>= my_val;
        other_any >>= other_val;
        return my_val == other_val;
      }
    case CORBA::tk_ulong:
      {
        CORBA::ULong my_val;
        CORBA::ULong other_val;
        my_any >>= my_val;
        other_any >>= other_val;
        return my_val == other_val;
      }
    case CORBA::tk_boolean:
      {
        CORBA::Boolean my_val;
        CORBA::Boolean other_val;
        my_any >>= CORBA::Any::to_boolean (my_val);
        other_any >>= CORBA::Any::to_boolean (other_val);
        return my_val == other_val;
      }
    case CORBA::tk_char:
      {
        CORBA::Char my_val;
        CORBA::Char other_val;
        my_any >>= CORBA::Any::to_char (my_val);
        other_any >>= CORBA::Any::to_char (other_val);
        return my_val == other_val;
      }
// For platforms without native 64-bit ints.
#if !defined (ACE_LACKS_LONGLONG_T)
    case CORBA::tk_longlong:
      {
        CORBA::LongLong my_val;
        CORBA::LongLong other_val;
        my_any >>= my_val;
        other_any >>= other_val;
        return my_val == other_val;
      }
#endif /* ACE_LACKS_LONGLONG_T */
    case CORBA::tk_ulonglong:
      {
        CORBA::ULongLong my_val;
        CORBA::ULongLong other_val;
        my_any >>= my_val;
        other_any >>= other_val;
        return my_val == other_val;
      }
    case CORBA::tk_wchar:
      {
        CORBA::WChar my_val;
        CORBA::WChar other_val;
        my_any >>= CORBA::Any::to_wchar (my_val);
        other_any >>= CORBA::Any::to_wchar (other_val);
        return my_val == other_val;
      }
    case CORBA::tk_enum:
      {
        CORBA::ULong my_val;
        CORBA::ULong other_val;
        ACE_Message_Block *mb = my_any._tao_get_cdr ();
        TAO_InputCDR my_cdr (mb,
                             my_any._tao_byte_order ());
        my_cdr.read_ulong (my_val);
        mb = other_any._tao_get_cdr ();
        TAO_InputCDR other_cdr (mb,
                                other_any._tao_byte_order ());
        other_cdr.read_ulong (other_val);
        return my_val == other_val;
      }
    // Cannot happen - we've covered all the legal discriminator types.
    default:
      return 0;
  }
}

