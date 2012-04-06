// -*- C++ -*-
// $Id$

#include "tao/DynamicAny/DynUnion_i.h"
#include "tao/DynamicAny/DynEnum_i.h"
#include "tao/DynamicAny/DynAnyFactory.h"
#include "tao/DynamicAny/DynAnyUtils_T.h"

#include "tao/AnyTypeCode/Marshal.h"
#include "tao/AnyTypeCode/Any_Unknown_IDL_Type.h"
#include "tao/AnyTypeCode/AnyTypeCode_methods.h"

#include "tao/CDR.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_DynUnion_i::TAO_DynUnion_i (CORBA::Boolean allow_truncation)
  : TAO_DynCommon (allow_truncation)
  , member_slot_ (0u)
{
}

TAO_DynUnion_i::~TAO_DynUnion_i (void)
{
}

void
TAO_DynUnion_i::init_common (void)
{
  this->ref_to_component_ = false;
  this->container_is_destroying_ = false;
  this->has_components_ = true;
  this->destroyed_ = false;
  this->component_count_ = 2;
  this->current_position_ = 0;
  this->member_slot_ = 0;
}

void
TAO_DynUnion_i::init (const CORBA::Any& any)
{
  CORBA::TypeCode_var tc = any.type ();

  CORBA::TCKind kind = TAO_DynAnyFactory::unalias (tc.in ());

  if (kind != CORBA::tk_union)
    {
      throw DynamicAny::DynAnyFactory::InconsistentTypeCode ();
    }

  // Initialize the typecode holder.
  this->type_ = tc;

  this->init_common ();

  // Set the from_factory arg to TRUE, so any problems will throw
  // InconsistentTypeCode.
  this->set_from_any (any);
}

void
TAO_DynUnion_i::init (CORBA::TypeCode_ptr tc)
{
  CORBA::TCKind kind = TAO_DynAnyFactory::unalias (tc);

  if (kind != CORBA::tk_union)
    {
      throw DynamicAny::DynAnyFactory::InconsistentTypeCode ();
    }

  // Initialize the typecode holder and current index.
  this->type_ = CORBA::TypeCode::_duplicate (tc);

  this->init_common ();

  // member_type()/member_label() do not work with aliased type codes.
  CORBA::TypeCode_var unaliased_tc =
  TAO_DynAnyFactory::strip_alias (this->type_.in ());

  CORBA::Any_var first_label =
    unaliased_tc->member_label (this->current_position_);

  // Initialize the discriminator to the label value of the first member.
  CORBA::TypeCode_var disc_tc = unaliased_tc->discriminator_type ();
  CORBA::TCKind disc_kind = TAO_DynAnyFactory::unalias (disc_tc.in ());
  CORBA::TCKind label_kind = TAO_DynAnyFactory::unalias (first_label->_tao_get_typecode ());
  if (disc_kind == CORBA::tk_enum &&
      label_kind == CORBA::tk_ulong)
    {
      // incase the discriminator is an enum type we have to walk
      // a slightly more complex path because enum labels are
      // stored as ulong in the union tc
      this->discriminator_ =
        TAO::MakeDynAnyUtils::make_dyn_any_t<CORBA::TypeCode_ptr> (
          disc_tc.in (),
          disc_tc.in (),
          this->allow_truncation_ );
      CORBA::ULong label_val;
      first_label >>= label_val;
      TAO_DynEnum_i::_narrow (this->discriminator_.in ())
                                    ->set_as_ulong (label_val);
    }
  else
    {
      this->discriminator_ =
        TAO::MakeDynAnyUtils::make_dyn_any_t<const CORBA::Any&> (
          first_label.in ()._tao_get_typecode (),
          first_label.in (),
          this->allow_truncation_ );
    }

  CORBA::TypeCode_var first_type =
    unaliased_tc->member_type (this->current_position_
                              );

  // Recursively initialize the member to its default value.
  this->member_ =
    TAO::MakeDynAnyUtils::make_dyn_any_t<CORBA::TypeCode_ptr> (
      first_type.in (),
      first_type.in (),
      this->allow_truncation_ );
}

// ****************************************************************

TAO_DynUnion_i *
TAO_DynUnion_i::_narrow (CORBA::Object_ptr _tao_objref)
{
  if (CORBA::is_nil (_tao_objref))
    {
      return 0;
    }

  return dynamic_cast<TAO_DynUnion_i *> (_tao_objref);
}

// This code is common to from_any() and the init() overload that takes
// an Any argument.
void
TAO_DynUnion_i::set_from_any (const CORBA::Any & any)
{
  // discriminator_type () does not work with aliased type codes,
  // only on unions, so strip the alias out of the type code
  //
  CORBA::TypeCode_var tc =
   TAO_DynAnyFactory::strip_alias (any._tao_get_typecode ());

  CORBA::TypeCode_var disc_tc =
    tc->discriminator_type ();

  CORBA::Any disc_any;
  TAO::Unknown_IDL_Type *unk = 0;

  // Get a CDR stream - if the Any doesn't have one, make one.
  TAO::Any_Impl *impl = any.impl ();
  TAO_OutputCDR out;
  TAO_InputCDR in (static_cast<ACE_Message_Block *> (0));

  if (impl->encoded ())
    {
      TAO::Unknown_IDL_Type * const tmp =
        dynamic_cast<TAO::Unknown_IDL_Type *> (impl);

      if (!tmp)
        throw CORBA::INTERNAL ();

      in = tmp->_tao_get_cdr ();
    }
  else
    {
      impl->marshal_value (out);
      TAO_InputCDR tmp_in (out);
      in = tmp_in;
    }

  TAO_InputCDR unk_in (in);
  ACE_NEW (unk,
           TAO::Unknown_IDL_Type (disc_tc.in (),
                                  unk_in));

  disc_any.replace (unk);

  // Need this here because we might have been called from init().
  if (!CORBA::is_nil (this->discriminator_.in ()))
    {
      this->discriminator_->destroy ();
    }

  // Set the discriminator.
  this->discriminator_ =
    TAO::MakeDynAnyUtils::make_dyn_any_t<const CORBA::Any&> (
      disc_any._tao_get_typecode (),
      disc_any,
      this->allow_truncation_ );

  // Move to the next field in the CDR stream.
  (void) TAO_Marshal_Object::perform_skip (disc_tc.in (), &in);

  CORBA::TypeCode_var unaliased =
    TAO_DynAnyFactory::strip_alias (tc.in ());

  CORBA::ULong const count = unaliased->member_count ();

  CORBA::Boolean match = false;
  CORBA::ULong i;

  // Get the index.
  for (i = 0; i < count; ++i)
    {
      CORBA::Any_var label_any = tc->member_label (i);

      match = this->label_match (label_any.in (), disc_any);

      if (match)
        {
          break;
        }
    }

  // Need this here because we might have been called from init().
  if (!CORBA::is_nil (this->member_.in ()))
    {
      this->member_->destroy ();
    }

  if (match)
    {
      CORBA::TypeCode_var member_tc = tc->member_type (i);

      CORBA::Any member_any;
      TAO::Unknown_IDL_Type *unk = 0;
      ACE_NEW (unk,
               TAO::Unknown_IDL_Type (member_tc.in (),
                                      in));
      member_any.replace (unk);

      this->member_ =
        TAO::MakeDynAnyUtils::make_dyn_any_t<const CORBA::Any&> (
          member_any._tao_get_typecode (),
          member_any,
          this->allow_truncation_ );

      this->member_slot_ = i;
    }
  else
    {
      // If no match, either the Any contains the default member or the
      // type code was bad.

      // default_index() does not work with aliased type codes.
      CORBA::TypeCode_var unaliased_tc =
        TAO_DynAnyFactory::strip_alias (this->type_.in ());

      CORBA::Long default_index =
        unaliased_tc->default_index ();

      if (default_index == -1)
        {
           set_to_no_active_member ();
        }
      else
        {
          CORBA::ULong index = static_cast<CORBA::ULong> (default_index);

          CORBA::TypeCode_var default_tc =
            tc->member_type (index
                            );

          CORBA::Any default_any;
          TAO::Unknown_IDL_Type *unk = 0;
          ACE_NEW (unk,
                   TAO::Unknown_IDL_Type (default_tc.in (),
                                          in));
          default_any.replace (unk);

          this->member_ =
            TAO::MakeDynAnyUtils::make_dyn_any_t<const CORBA::Any&> (
              default_any._tao_get_typecode (),
              default_any,
              this->allow_truncation_ );

          this->member_slot_ = index;
        }
    }
}

// ****************************************************************

DynamicAny::DynAny_ptr
TAO_DynUnion_i::get_discriminator (void)
{
  if (this->destroyed_)
    {
      throw ::CORBA::OBJECT_NOT_EXIST ();
    }

  // A deep copy is made only by copy() (CORBA 2.4.2 section 9.2.3.6).
  // Set the flag so the caller can't destroy.
  this->set_flag (this->discriminator_.in (), 0);

  return DynamicAny::DynAny::_duplicate (this->discriminator_.in ());
}

void
TAO_DynUnion_i::set_discriminator (DynamicAny::DynAny_ptr value)
{
  if (this->destroyed_)
    {
      throw ::CORBA::OBJECT_NOT_EXIST ();
    }

  CORBA::TypeCode_var tc = value->type ();

  CORBA::TypeCode_var disc_tc =
    this->discriminator_->type ();

  CORBA::Boolean equivalent = disc_tc->equivalent (tc.in ());

  if (!equivalent)
    {
      throw DynamicAny::DynAny::TypeMismatch ();
    }

  CORBA::Any_var value_any = value->to_any ();

  CORBA::ULong length =
    this->type_->member_count ();

  CORBA::Any_var label_any;
  CORBA::ULong i;

  // member_label() does not work with aliased type codes.
  CORBA::TypeCode_var unaliased_tc =
    TAO_DynAnyFactory::strip_alias (this->type_.in ());

  CORBA::Boolean match = 0;

  for (i = 0; i < length; ++i)
    {
      label_any = unaliased_tc->member_label (i);

      match = this->label_match (label_any.in (), value_any.in ());

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
      CORBA::TCKind disc_kind = TAO_DynAnyFactory::unalias (disc_tc.in ());
      CORBA::TCKind label_kind = TAO_DynAnyFactory::unalias (label_any->_tao_get_typecode ());
      if (disc_kind == CORBA::tk_enum &&
          label_kind == CORBA::tk_ulong)
        {
          // incase the discriminator is an enum type we have to walk
          // a slightly more complex path because enum labels are
          // stored as ulong in the union tc
          CORBA::ULong label_val;
          label_any >>= label_val;
          TAO_DynEnum_i::_narrow (this->discriminator_.in ())
                                        ->set_as_ulong (label_val);
        }
      else
        {
          this->discriminator_->from_any (label_any.in ());
        }

      // member_type() does not work with aliased type codes.
      CORBA::TypeCode_var member_tc = unaliased_tc->member_type (i);

      this->member_->destroy ();

      // Initialize member to default value.
      this->member_ =
        TAO::MakeDynAnyUtils::make_dyn_any_t<CORBA::TypeCode_ptr> (
          member_tc.in (),
          member_tc.in (),
          this->allow_truncation_ );

      // Named active member (CORBA 2.3.1).
      this->current_position_ = 1;
      this->component_count_ = 2;

      this->member_slot_ = i;

      // we're through, disc, value has already been set
      // no need for the copy operation below.
      return;
    }
  else
    {
      // default_index() does not work with aliased type codes.
      CORBA::TypeCode_var unaliased_tc =
        TAO_DynAnyFactory::strip_alias (this->type_.in ());

      // If no match, either the default member or no member is active.
      CORBA::Long default_index =
        unaliased_tc->default_index ();

      if (default_index == -1)
        {
          // This can't throw InvalidValue - if there's no default case
          // and all possible labels are used, we'd have had a match above.
          this->set_to_no_active_member ();
        }
      else
        {
          this->set_to_default_member ();

          this->member_slot_ = static_cast<CORBA::ULong> (default_index);
        }
    }

  // If we get here, we update the discriminator no matter what.
  // Any value will correspond to one of the three cases above.
  this->discriminator_->destroy ();

  this->discriminator_ = value->copy ();
}

void
TAO_DynUnion_i::set_to_default_member (void)
{
  if (this->destroyed_)
    {
      throw ::CORBA::OBJECT_NOT_EXIST ();
    }

  // default_index() does not work with aliased type codes.
  CORBA::TypeCode_var unaliased_tc =
    TAO_DynAnyFactory::strip_alias (this->type_.in ());

  CORBA::Long default_index =
    unaliased_tc->default_index ();

  if (default_index == -1)
    {
      // No explicit default case.
      throw DynamicAny::DynAny::TypeMismatch ();
    }
  else
    {
      CORBA::ULong index = static_cast<CORBA::ULong> (default_index);

      CORBA::TypeCode_var default_tc =
        unaliased_tc->member_type (index);

      this->member_->destroy ();

      this->member_ =
        TAO::MakeDynAnyUtils::make_dyn_any_t<CORBA::TypeCode_ptr> (
          default_tc.in (),
          default_tc.in (),
          this->allow_truncation_ );

      // Default member active (CORBA 2.3.1).
      this->current_position_ = 0;
      this->component_count_ = 2;

      this->member_slot_ = index;
    }
}

void
TAO_DynUnion_i::set_to_no_active_member (void)
{
  if (this->destroyed_)
    {
      throw ::CORBA::OBJECT_NOT_EXIST ();
    }

  // default_index() does not work with aliased type codes.
  CORBA::TypeCode_var unaliased_tc =
    TAO_DynAnyFactory::strip_alias (this->type_.in ());

  CORBA::Long default_index =
    unaliased_tc->default_index ();

  // Throw an exception is there is an explicit default case or if all
  // possible case label values are used.
  if (default_index != -1)
    {
      throw DynamicAny::DynAny::TypeMismatch ();
    }
  else
    {
      CORBA::TypeCode_var disc_tc =
        this->discriminator_->type ();

      CORBA::TCKind kind =
        TAO_DynAnyFactory::unalias (disc_tc.in ());

      if (kind == CORBA::tk_enum)
        {
          CORBA::ULong const member_count =
            this->type_->member_count ();

          CORBA::ULong const label_count =
            disc_tc->member_count ();

          if (member_count == label_count)
            {
              throw DynamicAny::DynAny::TypeMismatch ();
            }
        }
    }

  // No active member (CORBA 2.3.1).
  this->current_position_ = 0;
  this->component_count_ = 1;
}

CORBA::Boolean
TAO_DynUnion_i::has_no_active_member (void)
{
  if (this->destroyed_)
    {
      throw ::CORBA::OBJECT_NOT_EXIST ();
    }

  // No active member (CORBA 2.3.1).
  return (this->current_position_ == 0 && this->component_count_ == 1);
}

CORBA::TCKind
TAO_DynUnion_i::discriminator_kind (void)
{
  if (this->destroyed_)
    {
      throw ::CORBA::OBJECT_NOT_EXIST ();
    }

  CORBA::TypeCode_var tc =
    this->discriminator_->type ();

  return TAO_DynAnyFactory::unalias (tc.in ());
}

DynamicAny::DynAny_ptr
TAO_DynUnion_i::member (void)
{
  if (this->destroyed_)
    {
      throw ::CORBA::OBJECT_NOT_EXIST ();
    }

  CORBA::Boolean has_no_active_member =
    this->has_no_active_member ();

  if (has_no_active_member)
    {
      throw DynamicAny::DynAny::InvalidValue ();
    }

  // A deep copy is made only by copy() (CORBA 2.4.2 section 9.2.3.6).
  // Set the flag so the caller can't destroy.
  this->set_flag (this->member_.in (),
                  0
                 );

  return DynamicAny::DynAny::_duplicate (this->member_.in ());
}

char *
TAO_DynUnion_i::member_name (void)
{
  if (this->destroyed_)
    {
      throw ::CORBA::OBJECT_NOT_EXIST ();
    }

  CORBA::Boolean has_no_active_member =
    this->has_no_active_member ();

  if (has_no_active_member)
    {
      throw DynamicAny::DynAny::InvalidValue ();
    }

  const char *retval = this->type_->member_name (this->member_slot_);

  return CORBA::string_dup (retval);
}

CORBA::TCKind
TAO_DynUnion_i::member_kind (void)
{
  if (this->destroyed_)
    {
      throw ::CORBA::OBJECT_NOT_EXIST ();
    }

  CORBA::Boolean has_no_active_member =
    this->has_no_active_member ();

  if (has_no_active_member)
    {
      throw DynamicAny::DynAny::InvalidValue ();
    }

  CORBA::TypeCode_var tc = this->member_->type ();

  return TAO_DynAnyFactory::unalias (tc.in ());
}

// ****************************************************************

void
TAO_DynUnion_i::from_any (const CORBA::Any& any)
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
      // CORBA 2.3.1 section 9.2.3.3.
      this->current_position_ = 0;

      // May be changed in set_from_any().
      this->component_count_ = 2;

      // Set the from_factory arg to FALSE, so any problems will throw
      // TypeMismatch.
      this->set_from_any (any
                         );
    }
  else
    {
      throw DynamicAny::DynAny::TypeMismatch ();
    }
}

CORBA::Any_ptr
TAO_DynUnion_i::to_any (void)
{
  if (this->destroyed_)
    {
      throw ::CORBA::OBJECT_NOT_EXIST ();
    }

  TAO_OutputCDR out_cdr;

  // Add the discriminator to the CDR stream.

  CORBA::TypeCode_var disc_tc =
    this->discriminator_->type ();

  CORBA::Any_var disc_any =
    this->discriminator_->to_any ();

  TAO::Any_Impl *disc_any_impl = disc_any->impl ();
  TAO_OutputCDR disc_out_cdr;
  TAO_InputCDR disc_in_cdr (static_cast<ACE_Message_Block *> (0));

  if (disc_any_impl->encoded ())
    {
      TAO::Unknown_IDL_Type * const disc_unk =
        dynamic_cast<TAO::Unknown_IDL_Type *> (disc_any_impl);

      if (!disc_unk)
        throw CORBA::INTERNAL ();

      disc_in_cdr = disc_unk->_tao_get_cdr ();
    }
  else
    {
      disc_any_impl->marshal_value (disc_out_cdr);
      TAO_InputCDR disc_tmp_in_cdr (disc_out_cdr);
      disc_in_cdr = disc_tmp_in_cdr;
    }

  (void) TAO_Marshal_Object::perform_append (disc_tc.in (),
                                             &disc_in_cdr,
                                             &out_cdr);

  // Add the member to the CDR stream unless it has no active member.
  if (this->has_no_active_member () == 0)
    {
      CORBA::TypeCode_var member_tc =
        this->member_->type ();

      CORBA::Any_var member_any =
        this->member_->to_any ();

      TAO::Any_Impl *member_any_impl = member_any->impl ();
      TAO_OutputCDR member_out_cdr;
      TAO_InputCDR member_in_cdr (static_cast<ACE_Message_Block *> (0));

      if (member_any_impl->encoded ())
        {
          TAO::Unknown_IDL_Type * const member_unk =
            dynamic_cast<TAO::Unknown_IDL_Type *> (member_any_impl);

          if (!member_unk)
            throw CORBA::INTERNAL ();

          member_in_cdr = member_unk->_tao_get_cdr ();
        }
      else
        {
          member_any_impl->marshal_value (member_out_cdr);
          TAO_InputCDR member_tmp_in_cdr (member_out_cdr);
          member_in_cdr = member_tmp_in_cdr;
        }

      (void) TAO_Marshal_Object::perform_append (member_tc.in (),
                                                &member_in_cdr,
                                                &out_cdr);
    }

  // Make the Any.
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
TAO_DynUnion_i::equal (DynamicAny::DynAny_ptr rhs)
{
  if (this->destroyed_)
    {
      throw ::CORBA::OBJECT_NOT_EXIST ();
    }

  TAO_DynUnion_i *impl = TAO_DynUnion_i::_narrow (rhs);

  if (impl == 0)
    {
      return 0;
    }

  CORBA::Boolean equivalent =
    this->type_->equivalent (impl->type_.in ());

  if (!equivalent)
    {
      return 0;
    }

  CORBA::Boolean member_equal =
    this->member_->equal (impl->member_.in ());

  CORBA::Boolean disc_equal =
    this->discriminator_->equal (impl->discriminator_.in ());

  impl->_remove_ref ();

  return (member_equal && disc_equal);
}

void
TAO_DynUnion_i::destroy (void)
{
  if (this->destroyed_)
    {
      throw ::CORBA::OBJECT_NOT_EXIST ();
    }

  if (!this->ref_to_component_ || this->container_is_destroying_)
    {
      // Free the two components.

      if (! CORBA::is_nil (this->member_.in ()))
        {
          this->set_flag (this->member_.in (), 1);

          this->member_->destroy ();
        }

      this->set_flag (this->discriminator_.in (), 1);

      this->discriminator_->destroy ();

      this->destroyed_ = 1;
    }
}

DynamicAny::DynAny_ptr
TAO_DynUnion_i::current_component (void)
{
  if (this->destroyed_)
    {
      throw ::CORBA::OBJECT_NOT_EXIST ();
    }

  if (this->current_position_ == 1)
    {
      this->set_flag (this->member_.in (), 0);

      return DynamicAny::DynAny::_duplicate (this->member_.in ());
    }
  else
    {
      this->set_flag (this->discriminator_.in (), 0);

      return DynamicAny::DynAny::_duplicate (this->discriminator_.in ());
    }
}

// ****************************************************************

CORBA::Boolean
TAO_DynUnion_i::label_match (const CORBA::Any &my_any,
                             const CORBA::Any &other_any)
{
  // Use my_any so we can detect a default case label,
  // if we are iterating through the union type code's
  // member_label() calls.
  CORBA::TypeCode_var tc = my_any.type ();

  CORBA::TCKind kind = TAO_DynAnyFactory::unalias (tc.in ());

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

        // check whether the discriminator is possibly an enum type
        // since these get stored as ulong label values as well
        CORBA::TypeCode_var other_tc = other_any.type ();
        CORBA::TCKind kind = TAO_DynAnyFactory::unalias (other_tc.in ());
        if (kind == CORBA::tk_enum)
          {
            TAO::Any_Impl *other_impl = other_any.impl ();

            if (other_impl->encoded ())
              {
                TAO::Unknown_IDL_Type *other_unk =
                  dynamic_cast<TAO::Unknown_IDL_Type *> (other_impl);

                // We don't want unk's rd_ptr to move, in case we are
                // shared by another Any, so we use this to copy the
                // state, not the buffer.
                TAO_InputCDR for_reading (other_unk->_tao_get_cdr ());
                for_reading.read_ulong (other_val);
              }
            else
              {
                TAO_OutputCDR other_out;
                other_impl->marshal_value (other_out);
                TAO_InputCDR other_in (other_out);
                other_in.read_ulong (other_val);
              }
          }
        else
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

        TAO::Any_Impl *my_impl = my_any.impl ();

        if (my_impl->encoded ())
          {
            TAO::Unknown_IDL_Type * const my_unk =
              dynamic_cast<TAO::Unknown_IDL_Type *> (my_impl);

            if (!my_unk)
              throw CORBA::INTERNAL ();

            // We don't want unk's rd_ptr to move, in case we are shared by
            // another Any, so we use this to copy the state, not the buffer.
            TAO_InputCDR for_reading (my_unk->_tao_get_cdr ());
            for_reading.read_ulong (my_val);
          }
        else
          {
            TAO_OutputCDR my_out;
            my_impl->marshal_value (my_out);
            TAO_InputCDR my_in (my_out);
            my_in.read_ulong (my_val);
          }

        TAO::Any_Impl *other_impl = other_any.impl ();

        if (other_impl->encoded ())
          {
            TAO::Unknown_IDL_Type * const other_unk =
              dynamic_cast<TAO::Unknown_IDL_Type *> (other_impl);

            if (!other_unk)
              throw CORBA::INTERNAL ();

            // We don't want unk's rd_ptr to move, in case we are shared by
            // another Any, so we use this to copy the state, not the buffer.
            TAO_InputCDR for_reading (other_unk->_tao_get_cdr ());
            for_reading.read_ulong (other_val);
          }
        else
          {
            TAO_OutputCDR other_out;
            other_impl->marshal_value (other_out);
            TAO_InputCDR other_in (other_out);
            other_in.read_ulong (other_val);
          }

        return my_val == other_val;
      }
    // Cannot happen - we've covered all the legal discriminator types.
    default:
      return 0;
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
