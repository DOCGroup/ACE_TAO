/* -*- C++ -*- */
// $Id$
// ====================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    DynUnion_i.cpp
//
// = AUTHOR
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ====================================================================

#include "tao/DynAny_i.h"

#if !defined (TAO_HAS_MINIMUM_CORBA)

#include "tao/DynUnion_i.h"
#include "tao/InconsistentTypeCodeC.h"

// Constructors and destructor

TAO_DynUnion_i::TAO_DynUnion_i (const CORBA_Any& any)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      int tk = TAO_DynAny_i::unalias (any.type (),
                                      ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (tk == CORBA::tk_union)
        {
          // Initialize the typecode holder
          this->type_ = any.type ();

          this->set_from_any (any, 
                              ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
      else
        {
          ACE_THROW (CORBA_ORB_InconsistentTypeCode ());
        }
    }
  ACE_CATCHANY
    {
      // do nothing
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

TAO_DynUnion_i::TAO_DynUnion_i (CORBA_TypeCode_ptr tc)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      int tk = TAO_DynAny_i::unalias (tc,
                                      ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (tk == CORBA::tk_union)
        {
          // Initialize the typecode holder
          this->type_ = CORBA::TypeCode::_duplicate (tc);

          CORBA::TypeCode_ptr disc_tc = tc->discriminator_type (ACE_TRY_ENV);
          ACE_TRY_CHECK;

          // Get a typecode into the discriminator holder.
          this->discriminator_ = TAO_DynAny_i::create_dyn_any (disc_tc,
                                                               ACE_TRY_ENV);
          ACE_TRY_CHECK;

          // To be filled in by from_any() or assign().
          this->member_ = 0;
        }
      else
        {
          ACE_THROW (CORBA_ORB_InconsistentTypeCode ());
        }
    }
  ACE_CATCHANY
    {
      // do nothing
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

TAO_DynUnion_i::~TAO_DynUnion_i (void)
{
}

// Functions specific to DynUnion

// If an invalid value has been passed in to the discriminator, we
// don't save it but instead set the member index to the default
// value, if one exists.

CORBA::Boolean
TAO_DynUnion_i::set_as_default (CORBA::Environment& ACE_TRY_ENV)
{
  if ((CORBA::Long) this->current_index_ == this->type_->default_index (ACE_TRY_ENV))
    return 1;
  else
    return 0;
}

void
TAO_DynUnion_i::set_as_default (CORBA::Boolean set_as_default,
                                CORBA::Environment&)
{
  // Set_as_default should be treated as read-only and
  // probably will be in a future spec. For now, since
  // the IDL compiler generated the virtual function,
  // we override it as a no-op.
}

CORBA_DynAny_ptr
TAO_DynUnion_i::discriminator (CORBA::Environment &)
{
  return this->discriminator_.in ();
}

CORBA::TCKind
TAO_DynUnion_i::discriminator_kind (CORBA::Environment& ACE_TRY_ENV)
{
  CORBA_TypeCode_ptr tc = this->type_->discriminator_type (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::tk_null);

  CORBA::TCKind retval = TAO_DynAny_i::unalias (tc,
                                                ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::tk_null);

  return retval;
}

CORBA_DynAny_ptr
TAO_DynUnion_i::member (CORBA::Environment &)
{
  return this->member_.in ();
}

char*
TAO_DynUnion_i::member_name (CORBA::Environment& ACE_TRY_ENV)
{
  const char *retval = this->type_->member_name (this->current_index_,
                                                 ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  return CORBA::string_dup (retval);
}

// If a valid member name is supplied, this function creates a new
// member which is NOT initialized.

void
TAO_DynUnion_i::member_name (const char* member_name,
                             CORBA::Environment& ACE_TRY_ENV)
{
  CORBA::ULong count =
    this->type_->member_count (ACE_TRY_ENV);

  for (CORBA::ULong i = 0;
       i < count;
       i++)
    {
      const char *temp = this->type_->member_name (i,
                                                   ACE_TRY_ENV);
      ACE_CHECK;

      if (!ACE_OS::strcmp (member_name, 
                           temp))
        {
          // No sense doing anything if we're just "resetting" to the
          // current member.
          if (i == this->current_index_)
            {
              return;
            }
          else
            {
              CORBA_TypeCode_ptr member_type = 
                this->type_->member_type (i,
                                          ACE_TRY_ENV);
              ACE_CHECK;

              this->member_ =
                TAO_DynAny_i::create_dyn_any (member_type,
                                              ACE_TRY_ENV);
              ACE_CHECK;

              CORBA_Any_ptr disc_any = 
                this->type_->member_label (i,
                                           ACE_TRY_ENV);
              ACE_CHECK;

              this->discriminator_ =
                TAO_DynAny_i::create_dyn_any (*disc_any,
                                              ACE_TRY_ENV);
              ACE_CHECK;

              this->current_index_ = i;
              return;
            }
        }
    }
}

CORBA::TCKind
TAO_DynUnion_i::member_kind (CORBA::Environment& ACE_TRY_ENV)
{
  CORBA_TypeCode_ptr tc = this->type_->member_type (this->current_index_,
                                                    ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::tk_null);

  CORBA::TCKind retval = TAO_DynAny_i::unalias (tc,
                                                ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::tk_null);

  return retval;
}

// Common functions

void
TAO_DynUnion_i::assign (CORBA_DynAny_ptr dyn_any,
                        CORBA::Environment &ACE_TRY_ENV)
{
  // *dyn_any->to_any raises Invalid if arg is bad.
  CORBA_TypeCode_ptr tc = dyn_any->type (ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::Boolean equal = this->type_->equal (tc,
                                             ACE_TRY_ENV);
  ACE_CHECK;

  if (equal)
    {
      CORBA_Any_ptr any_ptr = dyn_any->to_any (ACE_TRY_ENV);
      ACE_CHECK;

      this->from_any (*any_ptr,
                      ACE_TRY_ENV);
      ACE_CHECK;
    }
  else
    {
      ACE_THROW (CORBA_DynAny::Invalid ());
    }
}

CORBA_DynAny_ptr
TAO_DynUnion_i::copy (CORBA::Environment &ACE_TRY_ENV)
{
  CORBA_Any_ptr a = this->to_any (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA_DynAny::_nil ());

  CORBA_DynAny_ptr retval = 
    TAO_DynAny_i::create_dyn_any (*a,
                                  ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA_DynAny::_nil ());

  return retval;
}

void
TAO_DynUnion_i::destroy (CORBA::Environment &ACE_TRY_ENV)
{
  // Free the two DynAnys.
  this->member_->destroy (ACE_TRY_ENV);
  ACE_CHECK;

  this->discriminator_->destroy (ACE_TRY_ENV);
  ACE_CHECK;

  // Free the top level
  delete this;
}

void
TAO_DynUnion_i::from_any (const CORBA_Any& any,
                          CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::Boolean equal = this->type_.in ()->equal (any.type (),
                                                   ACE_TRY_ENV);
  ACE_CHECK;

  if (equal)
    {
      this->set_from_any (any,
                          ACE_TRY_ENV);
      ACE_CHECK;
    }
  else
    {
      ACE_THROW (CORBA_DynAny::Invalid ());
    }
}

CORBA::Any_ptr
TAO_DynUnion_i::to_any (CORBA::Environment& ACE_TRY_ENV)
{
  // Both Dynanys must have been initialied.
  if (this->member_.in () == 0
      || this->discriminator_.in () == 0)
    {
      ACE_THROW_RETURN (CORBA_DynAny::Invalid (), 
                        0);
    }

  TAO_OutputCDR out_cdr;

  // Add the discriminator to the CDR stream.

  CORBA_TypeCode_ptr disc_tc =
    this->discriminator_->type (ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  CORBA_Any_var disc_any =
    this->discriminator_->to_any (ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  ACE_Message_Block *disc_mb =
    disc_any->_tao_get_cdr ();

  TAO_InputCDR disc_cdr (disc_mb);

  out_cdr.append (disc_tc,
                  &disc_cdr,
                  ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  // Add the member to the CDR stream.

  CORBA_TypeCode_ptr member_tc =
    this->member_->type (ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  CORBA_Any_var member_any =
    this->member_->to_any (ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  ACE_Message_Block *member_mb =
    member_any->_tao_get_cdr ();

  TAO_InputCDR member_cdr (member_mb);

  out_cdr.append (member_tc,
                  &member_cdr,
                  ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  // Make the Any.
  TAO_InputCDR in_cdr (out_cdr);

  CORBA_Any *retval;
  CORBA_TypeCode_ptr tc = this->type (ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  ACE_NEW_THROW_EX (retval,
                    CORBA_Any (tc,
                               0,
                               in_cdr.start ()),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  return retval;
}

CORBA::TypeCode_ptr
TAO_DynUnion_i::type (CORBA::Environment &)
{
  return this->type_.in ();
}

CORBA_DynAny_ptr
TAO_DynUnion_i::current_component (CORBA::Environment &)
{
  return this->member_.in ();
}

CORBA::Boolean
TAO_DynUnion_i::next (CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::ULong count = this->type_->member_count (ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  if (this->current_index_ + 1 == count)
    {
      return 0;
    }

  ++this->current_index_;

  if (!CORBA::is_nil (this->member_.in ()))
    {
      this->member_->destroy (ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);
    }

  CORBA_TypeCode_ptr tc = this->type_->member_type (this->current_index_,
                                                    ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  this->member_ =
    TAO_DynAny_i::create_dyn_any (tc,
                                  ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  return 1;
}

CORBA::Boolean
TAO_DynUnion_i::seek (CORBA::Long slot,
                      CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::ULong count = this->type_->member_count (ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  if (slot < 0 || slot >= (CORBA::Long) count)
    {
      return 0;
    }

  this->current_index_ = slot;

  if (!CORBA::is_nil (this->member_.in ()))
    {
      this->member_->destroy (ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);
    }

  CORBA_TypeCode_ptr tc = this->type_->member_type (this->current_index_,
                                                    ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  this->member_ =
    TAO_DynAny_i::create_dyn_any (tc,
                                  ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  return 1;
}

void
TAO_DynUnion_i::rewind (CORBA::Environment &ACE_TRY_ENV)
{
  if (this->current_index_ == 0)
    return;

  this->current_index_ = 0;

  if (!CORBA::is_nil (this->member_.in ()))
    {
      this->member_->destroy (ACE_TRY_ENV);
      ACE_CHECK;
    }

  CORBA_TypeCode_ptr tc = this->type_->member_type (this->current_index_,
                                                    ACE_TRY_ENV);
  ACE_CHECK;

  this->member_ =
    TAO_DynAny_i::create_dyn_any (tc,
                                  ACE_TRY_ENV);
  ACE_CHECK;
}

// Insert and get functions
//
// Note that next() is not called in the body of these functions as it is
// with DynStruct, DynSequence and DynArray. It doesn't seem to make sense
// for unions.

// Insert functions

void
TAO_DynUnion_i::insert_boolean (CORBA::Boolean value,
                                CORBA::Environment &ACE_TRY_ENV)
{
  CORBA_TypeCode_ptr type = this->member_->type (ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::TCKind kind = TAO_DynAny_i::unalias (type,
                                              ACE_TRY_ENV);
  ACE_CHECK;

  if (kind == CORBA::tk_boolean)
    {
      this->member_->insert_boolean (value,
                                     ACE_TRY_ENV);
      ACE_CHECK;
    }
  else
    {
      CORBA::ULong count =
        this->type_->member_count (ACE_TRY_ENV);
      ACE_CHECK;

      for (CORBA::ULong i = 0;
           i < count;
           i++)
        {
          CORBA::TypeCode_ptr tc =
            this->type_->member_type (i,
                                      ACE_TRY_ENV);
          ACE_CHECK;

          CORBA::TCKind try_kind = TAO_DynAny_i::unalias (tc,
                                                          ACE_TRY_ENV);
          ACE_CHECK;

          if (try_kind == CORBA::tk_boolean)
            {
              this->member_ =
                TAO_DynAny_i::create_dyn_any (tc,
                                              ACE_TRY_ENV);
              ACE_CHECK;

              this->member_->insert_boolean (value,
                                             ACE_TRY_ENV);
              ACE_CHECK;

              CORBA_Any_ptr any_ptr = this->type_->member_label (i,
                                                                 ACE_TRY_ENV);
              ACE_CHECK;

              this->discriminator_ =
                TAO_DynAny_i::create_dyn_any (*any_ptr,
                                              ACE_TRY_ENV);
              ACE_CHECK;

              this->current_index_ = i;
              return;
            }
        }

      ACE_THROW (CORBA_DynAny::InvalidValue ());
    }
}

void
TAO_DynUnion_i::insert_octet (CORBA::Octet value,
                              CORBA::Environment &ACE_TRY_ENV)
{
  CORBA_TypeCode_ptr type = this->member_->type (ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::TCKind kind = TAO_DynAny_i::unalias (type,
                                              ACE_TRY_ENV);
  ACE_CHECK;

  if (kind == CORBA::tk_octet)
    {
      this->member_->insert_octet (value,
                                   ACE_TRY_ENV);
      ACE_CHECK;
    }
  else
    {
      CORBA::ULong count =
        this->type_->member_count (ACE_TRY_ENV);
      ACE_CHECK;

      for (CORBA::ULong i = 0;
           i < count;
           i++)
        {
          CORBA::TypeCode_ptr tc =
            this->type_->member_type (i,
                                      ACE_TRY_ENV);
          ACE_CHECK;

          CORBA::TCKind try_kind = TAO_DynAny_i::unalias (tc,
                                                          ACE_TRY_ENV);
          ACE_CHECK;

          if (try_kind == CORBA::tk_octet)
            {
              this->member_ =
                TAO_DynAny_i::create_dyn_any (tc,
                                              ACE_TRY_ENV);
              ACE_CHECK;

              this->member_->insert_octet (value,
                                           ACE_TRY_ENV);
              ACE_CHECK;

              CORBA_Any_ptr any_ptr = this->type_->member_label (i,
                                                                 ACE_TRY_ENV);
              ACE_CHECK;

              this->discriminator_ =
                TAO_DynAny_i::create_dyn_any (*any_ptr,
                                              ACE_TRY_ENV);
              ACE_CHECK;

              this->current_index_ = i;
              return;
            }
        }

      ACE_THROW (CORBA_DynAny::InvalidValue ());
    }
}

void
TAO_DynUnion_i::insert_char (CORBA::Char value,
                             CORBA::Environment &ACE_TRY_ENV)
{
  CORBA_TypeCode_ptr type = this->member_->type (ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::TCKind kind = TAO_DynAny_i::unalias (type,
                                              ACE_TRY_ENV);
  ACE_CHECK;

  if (kind == CORBA::tk_char)
    {
      this->member_->insert_char (value,
                                  ACE_TRY_ENV);
      ACE_CHECK;
    }
  else
    {
      CORBA::ULong count =
        this->type_->member_count (ACE_TRY_ENV);
      ACE_CHECK;

      for (CORBA::ULong i = 0;
           i < count;
           i++)
        {
          CORBA::TypeCode_ptr tc =
            this->type_->member_type (i,
                                      ACE_TRY_ENV);
          ACE_CHECK;

          CORBA::TCKind try_kind = TAO_DynAny_i::unalias (tc,
                                                          ACE_TRY_ENV);
          ACE_CHECK;

          if (try_kind == CORBA::tk_char)
            {
              this->member_ =
                TAO_DynAny_i::create_dyn_any (tc,
                                              ACE_TRY_ENV);
              ACE_CHECK;

              this->member_->insert_char (value,
                                          ACE_TRY_ENV);
              ACE_CHECK;

              CORBA_Any_ptr any_ptr = this->type_->member_label (i,
                                                                 ACE_TRY_ENV);
              ACE_CHECK;

              this->discriminator_ =
                TAO_DynAny_i::create_dyn_any (*any_ptr,
                                              ACE_TRY_ENV);
              ACE_CHECK;

              this->current_index_ = i;
              return;
            }
        }

      ACE_THROW (CORBA_DynAny::InvalidValue ());
    }
}

void
TAO_DynUnion_i::insert_short (CORBA::Short value,
                              CORBA::Environment &ACE_TRY_ENV)
{
  CORBA_TypeCode_ptr type = this->member_->type (ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::TCKind kind = TAO_DynAny_i::unalias (type,
                                              ACE_TRY_ENV);
  ACE_CHECK;

  if (kind == CORBA::tk_short)
    {
      this->member_->insert_short (value,
                                   ACE_TRY_ENV);
      ACE_CHECK;
    }
  else
    {
      CORBA::ULong count =
        this->type_->member_count (ACE_TRY_ENV);
      ACE_CHECK;

      for (CORBA::ULong i = 0;
           i < count;
           i++)
        {
          CORBA::TypeCode_ptr tc =
            this->type_->member_type (i,
                                      ACE_TRY_ENV);
          ACE_CHECK;

          CORBA::TCKind try_kind = TAO_DynAny_i::unalias (tc,
                                                          ACE_TRY_ENV);
          ACE_CHECK;

          if (try_kind == CORBA::tk_short)
            {
              this->member_ =
                TAO_DynAny_i::create_dyn_any (tc,
                                              ACE_TRY_ENV);
              ACE_CHECK;

              this->member_->insert_short (value,
                                           ACE_TRY_ENV);
              ACE_CHECK;

              CORBA_Any_ptr any_ptr = this->type_->member_label (i,
                                                                 ACE_TRY_ENV);
              ACE_CHECK;

              this->discriminator_ =
                TAO_DynAny_i::create_dyn_any (*any_ptr,
                                              ACE_TRY_ENV);
              ACE_CHECK;

              this->current_index_ = i;
              return;
            }
        }

      ACE_THROW (CORBA_DynAny::InvalidValue ());
    }
}

void
TAO_DynUnion_i::insert_long (CORBA::Long value,
                             CORBA::Environment &ACE_TRY_ENV)
{
  CORBA_TypeCode_ptr type = this->member_->type (ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::TCKind kind = TAO_DynAny_i::unalias (type,
                                              ACE_TRY_ENV);
  ACE_CHECK;

  if (kind == CORBA::tk_long)
    {
      this->member_->insert_long (value,
                                  ACE_TRY_ENV);
      ACE_CHECK;
    }
  else
    {
      CORBA::ULong count =
        this->type_->member_count (ACE_TRY_ENV);
      ACE_CHECK;

      for (CORBA::ULong i = 0;
           i < count;
           i++)
        {
          CORBA::TypeCode_ptr tc =
            this->type_->member_type (i,
                                      ACE_TRY_ENV);
          ACE_CHECK;

          CORBA::TCKind try_kind = TAO_DynAny_i::unalias (tc,
                                                          ACE_TRY_ENV);
          ACE_CHECK;

          if (try_kind == CORBA::tk_long)
            {
              this->member_ =
                TAO_DynAny_i::create_dyn_any (tc,
                                              ACE_TRY_ENV);
              ACE_CHECK;

              this->member_->insert_long (value,
                                             ACE_TRY_ENV);
              ACE_CHECK;

              CORBA_Any_ptr any_ptr = this->type_->member_label (i,
                                                                 ACE_TRY_ENV);
              ACE_CHECK;

              this->discriminator_ =
                TAO_DynAny_i::create_dyn_any (*any_ptr,
                                              ACE_TRY_ENV);
              ACE_CHECK;

              this->current_index_ = i;
              return;
            }
        }

      ACE_THROW (CORBA_DynAny::InvalidValue ());
    }
}

void
TAO_DynUnion_i::insert_ushort (CORBA::UShort value,
                               CORBA::Environment &ACE_TRY_ENV)
{
  CORBA_TypeCode_ptr type = this->member_->type (ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::TCKind kind = TAO_DynAny_i::unalias (type,
                                              ACE_TRY_ENV);
  ACE_CHECK;

  if (kind == CORBA::tk_ushort)
    {
      this->member_->insert_ushort (value,
                                    ACE_TRY_ENV);
      ACE_CHECK;
    }
  else
    {
      CORBA::ULong count =
        this->type_->member_count (ACE_TRY_ENV);
      ACE_CHECK;

      for (CORBA::ULong i = 0;
           i < count;
           i++)
        {
          CORBA::TypeCode_ptr tc =
            this->type_->member_type (i,
                                      ACE_TRY_ENV);
          ACE_CHECK;

          CORBA::TCKind try_kind = TAO_DynAny_i::unalias (tc,
                                                          ACE_TRY_ENV);
          ACE_CHECK;

          if (try_kind == CORBA::tk_ushort)
            {
              this->member_ =
                TAO_DynAny_i::create_dyn_any (tc,
                                              ACE_TRY_ENV);
              ACE_CHECK;

              this->member_->insert_ushort (value,
                                            ACE_TRY_ENV);
              ACE_CHECK;

              CORBA_Any_ptr any_ptr = this->type_->member_label (i,
                                                                 ACE_TRY_ENV);
              ACE_CHECK;

              this->discriminator_ =
                TAO_DynAny_i::create_dyn_any (*any_ptr,
                                              ACE_TRY_ENV);
              ACE_CHECK;

              this->current_index_ = i;
              return;
            }
        }

      ACE_THROW (CORBA_DynAny::InvalidValue ());
    }
}

void
TAO_DynUnion_i::insert_ulong (CORBA::ULong value,
                              CORBA::Environment &ACE_TRY_ENV)
{
  CORBA_TypeCode_ptr type = this->member_->type (ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::TCKind kind = TAO_DynAny_i::unalias (type,
                                              ACE_TRY_ENV);
  ACE_CHECK;

  if (kind == CORBA::tk_ulong)
    {
      this->member_->insert_ulong (value,
                                     ACE_TRY_ENV);
      ACE_CHECK;
    }
  else
    {
      CORBA::ULong count =
        this->type_->member_count (ACE_TRY_ENV);
      ACE_CHECK;

      for (CORBA::ULong i = 0;
           i < count;
           i++)
        {
          CORBA::TypeCode_ptr tc =
            this->type_->member_type (i,
                                      ACE_TRY_ENV);
          ACE_CHECK;

          CORBA::TCKind try_kind = TAO_DynAny_i::unalias (tc,
                                                          ACE_TRY_ENV);
          ACE_CHECK;

          if (try_kind == CORBA::tk_ulong)
            {
              this->member_ =
                TAO_DynAny_i::create_dyn_any (tc,
                                              ACE_TRY_ENV);
              ACE_CHECK;

              this->member_->insert_ulong (value,
                                             ACE_TRY_ENV);
              ACE_CHECK;

              CORBA_Any_ptr any_ptr = this->type_->member_label (i,
                                                                 ACE_TRY_ENV);
              ACE_CHECK;

              this->discriminator_ =
                TAO_DynAny_i::create_dyn_any (*any_ptr,
                                              ACE_TRY_ENV);
              ACE_CHECK;

              this->current_index_ = i;
              return;
            }
        }

      ACE_THROW (CORBA_DynAny::InvalidValue ());
    }
}

void
TAO_DynUnion_i::insert_float (CORBA::Float value,
                              CORBA::Environment &ACE_TRY_ENV)
{
  CORBA_TypeCode_ptr type = this->member_->type (ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::TCKind kind = TAO_DynAny_i::unalias (type,
                                              ACE_TRY_ENV);
  ACE_CHECK;

  if (kind == CORBA::tk_float)
    {
      this->member_->insert_float (value,
                                   ACE_TRY_ENV);
      ACE_CHECK;
    }
  else
    {
      CORBA::ULong count =
        this->type_->member_count (ACE_TRY_ENV);
      ACE_CHECK;

      for (CORBA::ULong i = 0;
           i < count;
           i++)
        {
          CORBA::TypeCode_ptr tc =
            this->type_->member_type (i,
                                      ACE_TRY_ENV);
          ACE_CHECK;

          CORBA::TCKind try_kind = TAO_DynAny_i::unalias (tc,
                                                          ACE_TRY_ENV);
          ACE_CHECK;

          if (try_kind == CORBA::tk_float)
            {
              this->member_ =
                TAO_DynAny_i::create_dyn_any (tc,
                                              ACE_TRY_ENV);
              ACE_CHECK;

              this->member_->insert_float (value,
                                           ACE_TRY_ENV);
              ACE_CHECK;

              CORBA_Any_ptr any_ptr = this->type_->member_label (i,
                                                                 ACE_TRY_ENV);
              ACE_CHECK;

              this->discriminator_ =
                TAO_DynAny_i::create_dyn_any (*any_ptr,
                                              ACE_TRY_ENV);
              ACE_CHECK;

              this->current_index_ = i;
              return;
            }
        }

      ACE_THROW (CORBA_DynAny::InvalidValue ());
    }
}

void
TAO_DynUnion_i::insert_double (CORBA::Double value,
                               CORBA::Environment &ACE_TRY_ENV)
{
  CORBA_TypeCode_ptr type = this->member_->type (ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::TCKind kind = TAO_DynAny_i::unalias (type,
                                              ACE_TRY_ENV);
  ACE_CHECK;

  if (kind == CORBA::tk_double)
    {
      this->member_->insert_double (value,
                                    ACE_TRY_ENV);
      ACE_CHECK;
    }
  else
    {
      CORBA::ULong count =
        this->type_->member_count (ACE_TRY_ENV);
      ACE_CHECK;

      for (CORBA::ULong i = 0;
           i < count;
           i++)
        {
          CORBA::TypeCode_ptr tc =
            this->type_->member_type (i,
                                      ACE_TRY_ENV);
          ACE_CHECK;

          CORBA::TCKind try_kind = TAO_DynAny_i::unalias (tc,
                                                          ACE_TRY_ENV);
          ACE_CHECK;

          if (try_kind == CORBA::tk_double)
            {
              this->member_ =
                TAO_DynAny_i::create_dyn_any (tc,
                                              ACE_TRY_ENV);
              ACE_CHECK;

              this->member_->insert_double (value,
                                             ACE_TRY_ENV);
              ACE_CHECK;

              CORBA_Any_ptr any_ptr = this->type_->member_label (i,
                                                                 ACE_TRY_ENV);
              ACE_CHECK;

              this->discriminator_ =
                TAO_DynAny_i::create_dyn_any (*any_ptr,
                                              ACE_TRY_ENV);
              ACE_CHECK;

              this->current_index_ = i;
              return;
            }
        }

      ACE_THROW (CORBA_DynAny::InvalidValue ());
    }
}

void
TAO_DynUnion_i::insert_string (const char* value,
                               CORBA::Environment &ACE_TRY_ENV)
{
  CORBA_TypeCode_ptr type = this->member_->type (ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::TCKind kind = TAO_DynAny_i::unalias (type,
                                              ACE_TRY_ENV);
  ACE_CHECK;

  if (kind == CORBA::tk_string)
    {
      this->member_->insert_string (value,
                                    ACE_TRY_ENV);
      ACE_CHECK;
    }
  else
    {
      CORBA::ULong count =
        this->type_->member_count (ACE_TRY_ENV);
      ACE_CHECK;

      for (CORBA::ULong i = 0;
           i < count;
           i++)
        {
          CORBA::TypeCode_ptr tc =
            this->type_->member_type (i,
                                      ACE_TRY_ENV);
          ACE_CHECK;

          CORBA::TCKind try_kind = TAO_DynAny_i::unalias (tc,
                                                          ACE_TRY_ENV);
          ACE_CHECK;

          if (try_kind == CORBA::tk_string)
            {
              this->member_ =
                TAO_DynAny_i::create_dyn_any (tc,
                                              ACE_TRY_ENV);
              ACE_CHECK;

              this->member_->insert_string (value,
                                            ACE_TRY_ENV);
              ACE_CHECK;

              CORBA_Any_ptr any_ptr = this->type_->member_label (i,
                                                                 ACE_TRY_ENV);
              ACE_CHECK;

              this->discriminator_ =
                TAO_DynAny_i::create_dyn_any (*any_ptr,
                                              ACE_TRY_ENV);
              ACE_CHECK;

              this->current_index_ = i;
              return;
            }
        }

      ACE_THROW (CORBA_DynAny::InvalidValue ());
    }
}

void
TAO_DynUnion_i::insert_reference (CORBA::Object_ptr value,
                                  CORBA::Environment &ACE_TRY_ENV)
{
  CORBA_TypeCode_ptr type = this->member_->type (ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::TCKind kind = TAO_DynAny_i::unalias (type,
                                              ACE_TRY_ENV);
  ACE_CHECK;

  if (kind == CORBA::tk_objref)
    {
      this->member_->insert_reference (value,
                                       ACE_TRY_ENV);
      ACE_CHECK;
    }
  else
    {
      CORBA::ULong count =
        this->type_->member_count (ACE_TRY_ENV);
      ACE_CHECK;

      for (CORBA::ULong i = 0;
           i < count;
           i++)
        {
          CORBA::TypeCode_ptr tc =
            this->type_->member_type (i,
                                      ACE_TRY_ENV);
          ACE_CHECK;

          CORBA::TCKind try_kind = TAO_DynAny_i::unalias (tc,
                                                          ACE_TRY_ENV);
          ACE_CHECK;

          if (try_kind == CORBA::tk_objref)
            {
              this->member_ =
                TAO_DynAny_i::create_dyn_any (tc,
                                              ACE_TRY_ENV);
              ACE_CHECK;

              this->member_->insert_reference (value,
                                               ACE_TRY_ENV);
              ACE_CHECK;

              CORBA_Any_ptr any_ptr = this->type_->member_label (i,
                                                                 ACE_TRY_ENV);
              ACE_CHECK;

              this->discriminator_ =
                TAO_DynAny_i::create_dyn_any (*any_ptr,
                                              ACE_TRY_ENV);
              ACE_CHECK;

              this->current_index_ = i;
              return;
            }
        }

      ACE_THROW (CORBA_DynAny::InvalidValue ());
    }
}

void
TAO_DynUnion_i::insert_typecode (CORBA::TypeCode_ptr value,
                                 CORBA::Environment &ACE_TRY_ENV)
{
  CORBA_TypeCode_ptr type = this->member_->type (ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::TCKind kind = TAO_DynAny_i::unalias (type,
                                              ACE_TRY_ENV);
  ACE_CHECK;

  if (kind == CORBA::tk_TypeCode)
    {
      this->member_->insert_typecode (value,
                                      ACE_TRY_ENV);
      ACE_CHECK;
    }
  else
    {
      CORBA::ULong count =
        this->type_->member_count (ACE_TRY_ENV);
      ACE_CHECK;

      for (CORBA::ULong i = 0;
           i < count;
           i++)
        {
          CORBA::TypeCode_ptr tc =
            this->type_->member_type (i,
                                      ACE_TRY_ENV);
          ACE_CHECK;

          CORBA::TCKind try_kind = TAO_DynAny_i::unalias (tc,
                                                          ACE_TRY_ENV);
          ACE_CHECK;

          if (try_kind == CORBA::tk_TypeCode)
            {
              this->member_ =
                TAO_DynAny_i::create_dyn_any (tc,
                                              ACE_TRY_ENV);
              ACE_CHECK;

              this->member_->insert_typecode (value,
                                              ACE_TRY_ENV);
              ACE_CHECK;

              CORBA_Any_ptr any_ptr = this->type_->member_label (i,
                                                                 ACE_TRY_ENV);
              ACE_CHECK;

              this->discriminator_ =
                TAO_DynAny_i::create_dyn_any (*any_ptr,
                                              ACE_TRY_ENV);
              ACE_CHECK;

              this->current_index_ = i;
              return;
            }
        }

      ACE_THROW (CORBA_DynAny::InvalidValue ());
    }
}

void
TAO_DynUnion_i::insert_longlong (CORBA::LongLong value,
                                 CORBA::Environment &ACE_TRY_ENV)
{
  CORBA_TypeCode_ptr type = this->member_->type (ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::TCKind kind = TAO_DynAny_i::unalias (type,
                                              ACE_TRY_ENV);
  ACE_CHECK;

  if (kind == CORBA::tk_longlong)
    {
      this->member_->insert_longlong (value,
                                      ACE_TRY_ENV);
      ACE_CHECK;
    }
  else
    {
      CORBA::ULong count =
        this->type_->member_count (ACE_TRY_ENV);
      ACE_CHECK;

      for (CORBA::ULong i = 0;
           i < count;
           i++)
        {
          CORBA::TypeCode_ptr tc =
            this->type_->member_type (i,
                                      ACE_TRY_ENV);
          ACE_CHECK;

          CORBA::TCKind try_kind = TAO_DynAny_i::unalias (tc,
                                                          ACE_TRY_ENV);
          ACE_CHECK;

          if (try_kind == CORBA::tk_longlong)
            {
              this->member_ =
                TAO_DynAny_i::create_dyn_any (tc,
                                              ACE_TRY_ENV);
              ACE_CHECK;

              this->member_->insert_longlong (value,
                                              ACE_TRY_ENV);
              ACE_CHECK;

              CORBA_Any_ptr any_ptr = this->type_->member_label (i,
                                                                 ACE_TRY_ENV);
              ACE_CHECK;

              this->discriminator_ =
                TAO_DynAny_i::create_dyn_any (*any_ptr,
                                              ACE_TRY_ENV);
              ACE_CHECK;

              this->current_index_ = i;
              return;
            }
        }

      ACE_THROW (CORBA_DynAny::InvalidValue ());
    }
}

void
TAO_DynUnion_i::insert_ulonglong (CORBA::ULongLong value,
                                  CORBA::Environment &ACE_TRY_ENV)
{
  CORBA_TypeCode_ptr type = this->member_->type (ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::TCKind kind = TAO_DynAny_i::unalias (type,
                                              ACE_TRY_ENV);
  ACE_CHECK;

  if (kind == CORBA::tk_ulonglong)
    {
      this->member_->insert_ulonglong (value,
                                       ACE_TRY_ENV);
      ACE_CHECK;
    }
  else
    {
      CORBA::ULong count =
        this->type_->member_count (ACE_TRY_ENV);
      ACE_CHECK;

      for (CORBA::ULong i = 0;
           i < count;
           i++)
        {
          CORBA::TypeCode_ptr tc =
            this->type_->member_type (i,
                                      ACE_TRY_ENV);
          ACE_CHECK;

          CORBA::TCKind try_kind = TAO_DynAny_i::unalias (tc,
                                                          ACE_TRY_ENV);
          ACE_CHECK;

          if (try_kind == CORBA::tk_ulonglong)
            {
              this->member_ =
                TAO_DynAny_i::create_dyn_any (tc,
                                              ACE_TRY_ENV);
              ACE_CHECK;

              this->member_->insert_ulonglong (value,
                                               ACE_TRY_ENV);
              ACE_CHECK;

              CORBA_Any_ptr any_ptr = this->type_->member_label (i,
                                                                 ACE_TRY_ENV);
              ACE_CHECK;

              this->discriminator_ =
                TAO_DynAny_i::create_dyn_any (*any_ptr,
                                              ACE_TRY_ENV);
              ACE_CHECK;

              this->current_index_ = i;
              return;
            }
        }

      ACE_THROW (CORBA_DynAny::InvalidValue ());
    }
}

void
TAO_DynUnion_i::insert_wchar (CORBA::WChar value,
                              CORBA::Environment &ACE_TRY_ENV)
{
  CORBA_TypeCode_ptr type = this->member_->type (ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::TCKind kind = TAO_DynAny_i::unalias (type,
                                              ACE_TRY_ENV);
  ACE_CHECK;

  if (kind == CORBA::tk_wchar)
    {
      this->member_->insert_wchar (value,
                                   ACE_TRY_ENV);
      ACE_CHECK;
    }
  else
    {
      CORBA::ULong count =
        this->type_->member_count (ACE_TRY_ENV);
      ACE_CHECK;

      for (CORBA::ULong i = 0;
           i < count;
           i++)
        {
          CORBA::TypeCode_ptr tc =
            this->type_->member_type (i,
                                      ACE_TRY_ENV);
          ACE_CHECK;

          CORBA::TCKind try_kind = TAO_DynAny_i::unalias (tc,
                                                          ACE_TRY_ENV);
          ACE_CHECK;

          if (try_kind == CORBA::tk_wchar)
            {
              this->member_ =
                TAO_DynAny_i::create_dyn_any (tc,
                                              ACE_TRY_ENV);
              ACE_CHECK;

              this->member_->insert_wchar (value,
                                           ACE_TRY_ENV);
              ACE_CHECK;

              CORBA_Any_ptr any_ptr = this->type_->member_label (i,
                                                                 ACE_TRY_ENV);
              ACE_CHECK;

              this->discriminator_ =
                TAO_DynAny_i::create_dyn_any (*any_ptr,
                                              ACE_TRY_ENV);
              ACE_CHECK;

              this->current_index_ = i;
              return;
            }
        }

      ACE_THROW (CORBA_DynAny::InvalidValue ());
    }
}

void
TAO_DynUnion_i::insert_any (const CORBA::Any& value,
                            CORBA::Environment &ACE_TRY_ENV)
{
  CORBA_TypeCode_ptr type = this->member_->type (ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::TCKind kind = TAO_DynAny_i::unalias (type,
                                              ACE_TRY_ENV);
  ACE_CHECK;

  if (kind == CORBA::tk_any)
    {
      this->member_->insert_any (value,
                                 ACE_TRY_ENV);
      ACE_CHECK;
    }
  else
    {
      CORBA::ULong count =
        this->type_->member_count (ACE_TRY_ENV);
      ACE_CHECK;

      for (CORBA::ULong i = 0;
           i < count;
           i++)
        {
          CORBA::TypeCode_ptr tc =
            this->type_->member_type (i,
                                      ACE_TRY_ENV);
          ACE_CHECK;

          CORBA::TCKind try_kind = TAO_DynAny_i::unalias (tc,
                                                          ACE_TRY_ENV);
          ACE_CHECK;

          if (try_kind == CORBA::tk_any)
            {
              this->member_ =
                TAO_DynAny_i::create_dyn_any (tc,
                                              ACE_TRY_ENV);
              ACE_CHECK;

              this->member_->insert_any (value,
                                         ACE_TRY_ENV);
              ACE_CHECK;

              CORBA_Any_ptr any_ptr = this->type_->member_label (i,
                                                                 ACE_TRY_ENV);
              ACE_CHECK;

              this->discriminator_ =
                TAO_DynAny_i::create_dyn_any (*any_ptr,
                                              ACE_TRY_ENV);
              ACE_CHECK;

              this->current_index_ = i;
              return;
            }
        }

      ACE_THROW (CORBA_DynAny::InvalidValue ());
    }
}

// Get functions

CORBA::Boolean
TAO_DynUnion_i::get_boolean (CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::Boolean retval = 0;

  CORBA_TypeCode_ptr tc = this->member_->type (ACE_TRY_ENV);
  ACE_CHECK_RETURN (retval);

  CORBA::TCKind kind = TAO_DynAny_i::unalias (tc,
                                              ACE_TRY_ENV);
  ACE_CHECK_RETURN (retval);

  if (this->member_.in () && (kind == CORBA::tk_boolean))
    {
      retval = this->member_->get_boolean (ACE_TRY_ENV);
      ACE_CHECK_RETURN (retval);

      return retval;
    }
  else
    {
      ACE_THROW_RETURN (CORBA_DynAny::Invalid (), 
                        0);
    }
}

CORBA::Octet
TAO_DynUnion_i::get_octet (CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::Octet retval = 0;

  CORBA_TypeCode_ptr tc = this->member_->type (ACE_TRY_ENV);
  ACE_CHECK_RETURN (retval);

  CORBA::TCKind kind = TAO_DynAny_i::unalias (tc,
                                              ACE_TRY_ENV);
  ACE_CHECK_RETURN (retval);

  if (this->member_.in () && (kind == CORBA::tk_octet))
    {
      retval = this->member_->get_octet (ACE_TRY_ENV);
      ACE_CHECK_RETURN (retval);

      return retval;
    }
  else
    {
      ACE_THROW_RETURN (CORBA_DynAny::Invalid (), 
                        0);
    }
}

CORBA::Char
TAO_DynUnion_i::get_char (CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::Char retval = 0;

  CORBA_TypeCode_ptr tc = this->member_->type (ACE_TRY_ENV);
  ACE_CHECK_RETURN (retval);

  CORBA::TCKind kind = TAO_DynAny_i::unalias (tc,
                                              ACE_TRY_ENV);
  ACE_CHECK_RETURN (retval);

  if (this->member_.in () && (kind == CORBA::tk_char))
    {
      retval = this->member_->get_char (ACE_TRY_ENV);
      ACE_CHECK_RETURN (retval);

      return retval;
    }
  else
    {
      ACE_THROW_RETURN (CORBA_DynAny::Invalid (), 
                        0);
    }
}

CORBA::Short
TAO_DynUnion_i::get_short (CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::Short retval = 0;

  CORBA_TypeCode_ptr tc = this->member_->type (ACE_TRY_ENV);
  ACE_CHECK_RETURN (retval);

  CORBA::TCKind kind = TAO_DynAny_i::unalias (tc,
                                              ACE_TRY_ENV);
  ACE_CHECK_RETURN (retval);

  if (this->member_.in () && (kind == CORBA::tk_short))
    {
      retval = this->member_->get_short (ACE_TRY_ENV);
      ACE_CHECK_RETURN (retval);

      return retval;
    }
  else
    {
      ACE_THROW_RETURN (CORBA_DynAny::Invalid (), 
                        0);
    }
}

CORBA::Long
TAO_DynUnion_i::get_long (CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::Long retval = 0;

  CORBA_TypeCode_ptr tc = this->member_->type (ACE_TRY_ENV);
  ACE_CHECK_RETURN (retval);

  CORBA::TCKind kind = TAO_DynAny_i::unalias (tc,
                                              ACE_TRY_ENV);
  ACE_CHECK_RETURN (retval);

  if (this->member_.in () && (kind == CORBA::tk_long))
    {
      retval = this->member_->get_long (ACE_TRY_ENV);
      ACE_CHECK_RETURN (retval);

      return retval;
    }
  else
    {
      ACE_THROW_RETURN (CORBA_DynAny::Invalid (), 
                        0);
    }
}

CORBA::UShort
TAO_DynUnion_i::get_ushort(CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::UShort retval = 0;

  CORBA_TypeCode_ptr tc = this->member_->type (ACE_TRY_ENV);
  ACE_CHECK_RETURN (retval);

  CORBA::TCKind kind = TAO_DynAny_i::unalias (tc,
                                              ACE_TRY_ENV);
  ACE_CHECK_RETURN (retval);

  if (this->member_.in () && (kind == CORBA::tk_ushort))
    {
      retval = this->member_->get_ushort (ACE_TRY_ENV);
      ACE_CHECK_RETURN (retval);

      return retval;
    }
  else
    {
      ACE_THROW_RETURN (CORBA_DynAny::Invalid (), 
                        0);
    }
}

CORBA::ULong
TAO_DynUnion_i::get_ulong (CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::ULong retval = 0;

  CORBA_TypeCode_ptr tc = this->member_->type (ACE_TRY_ENV);
  ACE_CHECK_RETURN (retval);

  CORBA::TCKind kind = TAO_DynAny_i::unalias (tc,
                                              ACE_TRY_ENV);
  ACE_CHECK_RETURN (retval);

  if (this->member_.in () && (kind == CORBA::tk_ulong))
    {
      retval = this->member_->get_ulong (ACE_TRY_ENV);
      ACE_CHECK_RETURN (retval);

      return retval;
    }
  else
    {
      ACE_THROW_RETURN (CORBA_DynAny::Invalid (), 
                        0);
    }
}

CORBA::Float
TAO_DynUnion_i::get_float (CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::Float retval = 0;

  CORBA_TypeCode_ptr tc = this->member_->type (ACE_TRY_ENV);
  ACE_CHECK_RETURN (retval);

  CORBA::TCKind kind = TAO_DynAny_i::unalias (tc,
                                              ACE_TRY_ENV);
  ACE_CHECK_RETURN (retval);

  if (this->member_.in () && (kind == CORBA::tk_float))
    {
      retval = this->member_->get_float (ACE_TRY_ENV);
      ACE_CHECK_RETURN (retval);

      return retval;
    }
  else
    {
      ACE_THROW_RETURN (CORBA_DynAny::Invalid (), 
                        0);
    }
}

CORBA::Double
TAO_DynUnion_i::get_double (CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::Double retval = 0;

  CORBA_TypeCode_ptr tc = this->member_->type (ACE_TRY_ENV);
  ACE_CHECK_RETURN (retval);

  CORBA::TCKind kind = TAO_DynAny_i::unalias (tc,
                                              ACE_TRY_ENV);
  ACE_CHECK_RETURN (retval);

  if (this->member_.in () && (kind == CORBA::tk_double))
    {
      retval = this->member_->get_double (ACE_TRY_ENV);
      ACE_CHECK_RETURN (retval);

      return retval;
    }
  else
    {
      ACE_THROW_RETURN (CORBA_DynAny::Invalid (), 
                        0);
    }
}

CORBA::String
TAO_DynUnion_i::get_string (CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::String retval = 0;

  CORBA_TypeCode_ptr tc = this->member_->type (ACE_TRY_ENV);
  ACE_CHECK_RETURN (retval);

  CORBA::TCKind kind = TAO_DynAny_i::unalias (tc,
                                              ACE_TRY_ENV);
  ACE_CHECK_RETURN (retval);

  if (this->member_.in () && (kind == CORBA::tk_string))
    {
      retval = this->member_->get_string (ACE_TRY_ENV);
      ACE_CHECK_RETURN (retval);

      return retval;
    }
  else
    {
      ACE_THROW_RETURN (CORBA_DynAny::Invalid (), 
                        0);
    }
}

CORBA::TypeCode_ptr
TAO_DynUnion_i::get_typecode (CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::TypeCode_ptr retval = 0;

  CORBA_TypeCode_ptr tc = this->member_->type (ACE_TRY_ENV);
  ACE_CHECK_RETURN (retval);

  CORBA::TCKind kind = TAO_DynAny_i::unalias (tc,
                                              ACE_TRY_ENV);
  ACE_CHECK_RETURN (retval);

  if (this->member_.in () && (kind == CORBA::tk_TypeCode))
    {
      retval = this->member_->get_typecode (ACE_TRY_ENV);
      ACE_CHECK_RETURN (retval);

      return retval;
    }
  else
    {
      ACE_THROW_RETURN (CORBA_DynAny::Invalid (), 
                        0);
    }
}

CORBA::Object_ptr
TAO_DynUnion_i::get_reference (CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::Object_ptr retval = 0;

  CORBA_TypeCode_ptr tc = this->member_->type (ACE_TRY_ENV);
  ACE_CHECK_RETURN (retval);

  CORBA::TCKind kind = TAO_DynAny_i::unalias (tc,
                                              ACE_TRY_ENV);
  ACE_CHECK_RETURN (retval);

  if (this->member_.in () && (kind == CORBA::tk_objref))
    {
      retval = this->member_->get_reference (ACE_TRY_ENV);
      ACE_CHECK_RETURN (retval);

      return retval;
    }
  else
    {
      ACE_THROW_RETURN (CORBA_DynAny::Invalid (), 
                        0);
    }
}

CORBA::LongLong
TAO_DynUnion_i::get_longlong (CORBA::Environment &ACE_TRY_ENV)
{
#if defined (ACE_LACKS_LONGLONG_T)
  CORBA::LongLong retval = {0, 0};
#else  /* ! ACE_LACKS_LONGLONG_T */
  CORBA::LongLong retval = 0;
#endif /* ! ACE_LACKS_LONGLONG_T */

  CORBA_TypeCode_ptr tc = this->member_->type (ACE_TRY_ENV);
  ACE_CHECK_RETURN (retval);

  CORBA::TCKind kind = TAO_DynAny_i::unalias (tc,
                                              ACE_TRY_ENV);
  ACE_CHECK_RETURN (retval);

  if (this->member_.in () && (kind == CORBA::tk_longlong))
    {
      retval = this->member_->get_longlong (ACE_TRY_ENV);
      ACE_CHECK_RETURN (retval);

      return retval;
    }
  else
    {
      ACE_THROW_RETURN (CORBA_DynAny::Invalid (), 
                        retval);
    }
}

CORBA::ULongLong
TAO_DynUnion_i::get_ulonglong (CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::ULongLong retval = 0;

  CORBA_TypeCode_ptr tc = this->member_->type (ACE_TRY_ENV);
  ACE_CHECK_RETURN (retval);

  CORBA::TCKind kind = TAO_DynAny_i::unalias (tc,
                                              ACE_TRY_ENV);
  ACE_CHECK_RETURN (retval);

  if (this->member_.in () && (kind == CORBA::tk_ulonglong))
    {
      retval = this->member_->get_ulonglong (ACE_TRY_ENV);
      ACE_CHECK_RETURN (retval);

      return retval;
    }
  else
    {
      ACE_THROW_RETURN (CORBA_DynAny::Invalid (), 
                        0);
    }
}

CORBA::WChar
TAO_DynUnion_i::get_wchar (CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::WChar retval = 0;

  CORBA_TypeCode_ptr tc = this->member_->type (ACE_TRY_ENV);
  ACE_CHECK_RETURN (retval);

  CORBA::TCKind kind = TAO_DynAny_i::unalias (tc,
                                              ACE_TRY_ENV);
  ACE_CHECK_RETURN (retval);

  if (this->member_.in () && (kind == CORBA::tk_wchar))
    {
      retval = this->member_->get_wchar (ACE_TRY_ENV);
      ACE_CHECK_RETURN (retval);

      return retval;
    }
  else
    {
      ACE_THROW_RETURN (CORBA_DynAny::Invalid (), 
                        0);
    }
}

CORBA::Any_ptr
TAO_DynUnion_i::get_any (CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::Any_ptr retval = 0;

  CORBA_TypeCode_ptr tc = this->member_->type (ACE_TRY_ENV);
  ACE_CHECK_RETURN (retval);

  CORBA::TCKind kind = TAO_DynAny_i::unalias (tc,
                                              ACE_TRY_ENV);
  ACE_CHECK_RETURN (retval);

  if (this->member_.in () && (kind == CORBA::tk_any))
    {
      retval = this->member_->get_any (ACE_TRY_ENV);
      ACE_CHECK_RETURN (retval);

      return retval;
    }
  else
    {
      ACE_THROW_RETURN (CORBA_DynAny::Invalid (), 
                        0);
    }
}

// Private functions

// Some specialized instantiations of the template functor defined in
// DynUnion_i_T.cpp.

CORBA::Boolean
TAO_DynUnion_i::Boolean_extractor::check_match (const CORBA_Any &inside_any,
                                                const CORBA_Any &outside_any)
{
  CORBA::Any::to_boolean member_struct (this->member_index_);
  CORBA::Any::to_boolean arg_struct (this->arg_index_);

  inside_any >>= member_struct;
  outside_any >>= arg_struct;

  return member_struct.ref_ == arg_struct.ref_;
}

CORBA::Boolean
TAO_DynUnion_i::Char_extractor::check_match (const CORBA_Any &inside_any,
                                             const CORBA_Any &outside_any)
{
  CORBA::Any::to_char member_struct (this->member_index_);
  CORBA::Any::to_char arg_struct (this->arg_index_);

  inside_any >>= member_struct;
  outside_any >>= arg_struct;

  return member_struct.ref_ == arg_struct.ref_;
}

CORBA::Boolean
TAO_DynUnion_i::WChar_extractor::check_match (const CORBA_Any &inside_any,
                                              const CORBA_Any &outside_any)
{
  CORBA::Any::to_wchar member_struct (this->member_index_);
  CORBA::Any::to_wchar arg_struct (this->arg_index_);

  inside_any >>= member_struct;
  outside_any >>= arg_struct;

  return member_struct.ref_ == arg_struct.ref_;
}

CORBA::Boolean
TAO_DynUnion_i::Enum_extractor::check_match (const CORBA_Any &inside_any,
                                             const CORBA_Any &outside_any)
{
  // Get the CDR stream of one argument...
  ACE_Message_Block *mb = inside_any._tao_get_cdr ();

  TAO_InputCDR inside_cdr (mb);

  inside_cdr.read_ulong (this->member_index_);

  // And of the other...
  mb = outside_any._tao_get_cdr ();

  TAO_InputCDR outside_cdr (mb);

  outside_cdr.read_ulong (this->arg_index_);

  return this->member_index_ == this->arg_index_;
}

// Functor factory.
DU_Extractor_base*
TAO_DynUnion_i::get_extractor (CORBA::TCKind kind,
                               CORBA::Environment& ACE_TRY_ENV)
{
  DU_Extractor_base* retval;

  switch (kind)
    {
    case CORBA::tk_short:
      ACE_NEW_THROW_EX (retval,
                        DU_Extractor<CORBA::Short>,
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);
      return retval;
    case CORBA::tk_long:
      ACE_NEW_THROW_EX (retval,
                        DU_Extractor<CORBA::Long>,
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);
      return retval;
    case CORBA::tk_ushort:
      ACE_NEW_THROW_EX (retval,
                        DU_Extractor<CORBA::UShort>,
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);
      return retval;
    case CORBA::tk_ulong:
      ACE_NEW_THROW_EX (retval,
                        DU_Extractor<CORBA::ULong>,
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);
      return retval;
    case CORBA::tk_boolean:
      ACE_NEW_THROW_EX (retval,
                        Boolean_extractor,
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);
      return retval;
    case CORBA::tk_char:
      ACE_NEW_THROW_EX (retval,
                        Char_extractor,
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);
      return retval;

      // For platforms without native 64-bit ints . . .
#if !defined (ACE_LACKS_LONGLONG_T)
    case CORBA::tk_longlong:
      ACE_NEW_THROW_EX (retval,
                        DU_Extractor<CORBA::LongLong>,
                        CORBA::NO_MEMORY ());

      ACE_CHECK_RETURN (0);
      return retval;
#endif /* ACE_LACKS_LONGLONG_T */

    case CORBA::tk_ulonglong:
      ACE_NEW_THROW_EX (retval,
                        DU_Extractor<CORBA::ULongLong>,
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);
      return retval;
    case CORBA::tk_wchar:
      ACE_NEW_THROW_EX (retval,
                        WChar_extractor,
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);
      return retval;
    case CORBA::tk_enum:
      ACE_NEW_THROW_EX (retval,
                        Enum_extractor,
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);
      return retval;
    default:
      return 0;
    }
}

// This code is common to from_any and the constructor taking an Any
// argument.
void
TAO_DynUnion_i::set_from_any (const CORBA_Any& any,
                              CORBA::Environment &ACE_TRY_ENV)
{
  // Get the CDR stream of the argument.
  ACE_Message_Block* mb = any._tao_get_cdr ();

  TAO_InputCDR cdr (mb);

  CORBA_TypeCode_ptr disc_tc = 
    any.type ()->discriminator_type (ACE_TRY_ENV);
  ACE_CHECK;

  CORBA_Any disc_any (disc_tc,
                      0,
                      cdr.start ());

  if (!CORBA::is_nil (this->discriminator_.in ()))
    {
      this->discriminator_->destroy (ACE_TRY_ENV);
      ACE_CHECK;
    }

  // Set the discriminator holder.
  this->discriminator_ = TAO_DynAny_i::create_dyn_any (disc_any,
                                                       ACE_TRY_ENV);
  ACE_CHECK;

  cdr.skip (disc_tc);

  CORBA::ULong count = any.type ()->member_count (ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::ULong i;
  CORBA::Long default_index = -1;

  CORBA::TCKind discriminator_kind = disc_tc->kind (ACE_TRY_ENV);
  ACE_CHECK;

  DU_Extractor_base* functor =
    this->get_extractor (discriminator_kind,
                         ACE_TRY_ENV);
  ACE_CHECK;

  // Get the index.
  for (i = 0; i < count; i++)
    {
      CORBA_Any label_any = *any.type ()->member_label (i,
                                                        ACE_TRY_ENV);
      ACE_CHECK;

      CORBA::TCKind label_kind = label_any.type ()->kind (ACE_TRY_ENV);
      ACE_CHECK;

      // If there is a default index, note the value but don't call
      // check_match.
      if (label_kind == CORBA::tk_octet)
        {
          default_index = i;
          continue;
        }
      // Check if values match.
      else if (functor->check_match (disc_any,
                                     label_any))
        {
          this->current_index_ = i;
          break;
        }
    }

  delete functor;

  if (i == count && default_index == -1)
    {
      ACE_THROW (CORBA_DynAny::Invalid ());
    }
  else
    {
      if (i == count && default_index != -1)
        {
          this->current_index_ = default_index;
        }

      CORBA::TypeCode_ptr member_tc =
        any.type ()->member_type (this->current_index_,
                                  ACE_TRY_ENV);
      ACE_CHECK;

      CORBA_Any member_any (member_tc,
                            0,
                            cdr.start ());

      if (!CORBA::is_nil (this->member_.in ()))
        {
          this->member_->destroy (ACE_TRY_ENV);
          ACE_CHECK;
        }

      this->member_ =
        TAO_DynAny_i::create_dyn_any (member_any,
                                      ACE_TRY_ENV);
      ACE_CHECK;
    }
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class DU_Extractor<CORBA::Short>;
template class DU_Extractor<CORBA::Long>;
template class DU_Extractor<CORBA::UShort>;
template class DU_Extractor<CORBA::ULong>;

// For platforms without native 64-bit ints . . .
#if !defined (ACE_LACKS_LONGLONG_T)
template class DU_Extractor<CORBA::LongLong>;
#endif /* ACE_LACKS_LONGLONG_T */

template class DU_Extractor<CORBA::ULongLong>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate DU_Extractor<CORBA::Short>
#pragma instantiate DU_Extractor<CORBA::Long>
#pragma instantiate DU_Extractor<CORBA::UShort>
#pragma instantiate DU_Extractor<CORBA::ULong>

// For platforms without native 64-bit ints . . .
#if !defined (ACE_LACKS_LONGLONG_T)
#pragma instantiate DU_Extractor<CORBA::LongLong>
#endif /* ACE_LACKS_LONGLONG_T */

#pragma instantiate DU_Extractor<CORBA::ULongLong>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#endif /* TAO_HAS_MINIMUM_CORBA */
