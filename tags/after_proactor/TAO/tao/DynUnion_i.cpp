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
//    Jeff Parsons <jp4@cs.wustl.edu>
//
// ====================================================================

#include "tao/DynAny_i.h"
#include "tao/DynUnion_i.h"
#include "tao/InconsistentTypeCodeC.h"

//////////////////////////////////////////////////////////////////////
// Constructors and destructor

TAO_DynUnion_i::TAO_DynUnion_i (const CORBA_Any& any)
{
  CORBA::Environment env;

  if (TAO_DynAny_i::unalias (any.type (),
                             env)
      == CORBA::tk_union)
    {
      // Initialize the typecode holder
      this->type_ = any.type ();

      this->set_from_any (any, env);
    }
  else
    env.exception (new CORBA_ORB_InconsistentTypeCode);
}

TAO_DynUnion_i::TAO_DynUnion_i (CORBA_TypeCode_ptr tc)
{
  CORBA::Environment env;

  if (TAO_DynAny_i::unalias (tc,
                             env)
    == CORBA::tk_union)
    {
      // Initialize the typecode holder
      this->type_ = CORBA::TypeCode::_duplicate (tc);

      CORBA::TypeCode_ptr disc_tc = tc->discriminator_type (env);

      // Get a typecode into the discriminator holder.
      this->discriminator_ = TAO_DynAny_i::create_dyn_any (disc_tc,
                                                           env);

      // To be filled in by from_any() or assign().
      this->member_ = 0;
    }
  else
    env.exception (new CORBA_ORB_InconsistentTypeCode);
}

TAO_DynUnion_i::~TAO_DynUnion_i (void)
{
}

/////////////////////////////////////////////////////////////////////
// Functions specific to DynUnion

// If an invalid value has been passed in to the discriminator,
// we don't save it but instead set the member index to the
// default value, if one exists.
CORBA::Boolean
TAO_DynUnion_i::set_as_default (CORBA::Environment& env)
{
  if ((CORBA::Long) this->index_ == this->type_->default_index (env))
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
TAO_DynUnion_i::discriminator_kind (CORBA::Environment& env)
{
  return TAO_DynAny_i::unalias (this->type_->discriminator_type (env),
                                env);
}

CORBA_DynAny_ptr
TAO_DynUnion_i::member (CORBA::Environment &)
{
  return this->member_.in ();
}

char*
TAO_DynUnion_i::member_name (CORBA::Environment& env)
{
  return CORBA::string_dup (this->type_->member_name (this->index_,
                                                      env));
}

// If a valid member name is supplied, this function creates a new
// member which is NOT initialized.
void
TAO_DynUnion_i::member_name (const char* member_name,
                             CORBA::Environment& env)
{
  CORBA::ULong count = this->type_->member_count (env);

  for (CORBA::ULong i = 0; i < count; i++)
    {
      if (!ACE_OS::strcmp (member_name,
                           this->type_->member_name (i,
                                                     env)))
        {
          // No sense doing anything if we're just "resetting" to the
          // current member.
          if (i == this->index_)
            return;
          else
            {
              this->member_ =
                TAO_DynAny_i::create_dyn_any (this->type_->member_type (i,
                                                                        env),
                                              env);
              this->discriminator_ =
                TAO_DynAny_i::create_dyn_any (*this->type_->member_label (i,
                                                                          env),
                                              env);
              this->index_ = i;
              return;
            }
        }
    }
}


CORBA::TCKind
TAO_DynUnion_i::member_kind (CORBA::Environment& env)
{
  return TAO_DynAny_i::unalias (this->type_->member_type (this->index_,
                                                          env),
                                env);
}

//////////////////////////////////////////////////////////////////////
// Common functions

void
TAO_DynUnion_i::assign (CORBA_DynAny_ptr dyn_any,
                        CORBA::Environment &env)
{
  // *dyn_any->to_any raises Invalid if arg is bad.
  if (this->type_->equal (dyn_any->type (env),
                          env))
    this->from_any (*dyn_any->to_any (env),
                    env);
  else
    env.exception (new CORBA_DynAny::Invalid);
}

CORBA_DynAny_ptr
TAO_DynUnion_i::copy (CORBA::Environment &env)
{
  CORBA_Any_ptr a = this->to_any (env);
  return TAO_DynAny_i::create_dyn_any (*a,
                                       env);
}

void
TAO_DynUnion_i::destroy (CORBA::Environment &env)
{
  // Free the two DynAnys.
  this->member_->destroy (env);
  this->discriminator_->destroy (env);

  // Free the top level
  delete this;
}

void
TAO_DynUnion_i::from_any (const CORBA_Any& any,
                          CORBA::Environment &env)
{
  if (this->type_.in ()->equal (any.type (),
                                env))
    this->set_from_any (any,
                        env);
  else
    env.exception (new CORBA_DynAny::Invalid);
}

CORBA::Any_ptr
TAO_DynUnion_i::to_any (CORBA::Environment& TAO_IN_ENV)
{
  // Both Dynanys must have been initialied.
  if (this->member_.in () == 0 || this->discriminator_.in () == 0)
    {
      TAO_IN_ENV.exception (new CORBA_DynAny::Invalid);
      return 0;
    }

  TAO_OutputCDR out_cdr;

  // Add the discriminator to the CDR stream.

  CORBA_TypeCode_ptr disc_tc = this->discriminator_->type (TAO_IN_ENV);

  CORBA_Any_ptr disc_any = this->discriminator_->to_any (TAO_IN_ENV);

  ACE_Message_Block* disc_mb = disc_any->_tao_get_cdr ();

  TAO_InputCDR disc_cdr (disc_mb);

  out_cdr.append (disc_tc,
                  &disc_cdr,
                  TAO_IN_ENV);

  delete disc_any;

  // Add the member to the CDR stream.

  CORBA_TypeCode_ptr member_tc = this->member_->type (TAO_IN_ENV);

  CORBA_Any_ptr member_any = this->member_->to_any (TAO_IN_ENV);

  ACE_Message_Block* member_mb = member_any->_tao_get_cdr ();

  TAO_InputCDR member_cdr (member_mb);

  out_cdr.append (member_tc,
                  &member_cdr,
                  TAO_IN_ENV);

  delete member_any;

  // Make the Any.
  TAO_InputCDR in_cdr (out_cdr);

  CORBA_Any* retval;
  ACE_NEW_THROW_RETURN (retval,
                        CORBA_Any (this->type (TAO_IN_ENV),
                                   in_cdr.start ()),
                        CORBA::NO_MEMORY (CORBA::COMPLETED_NO),
                        0);
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
TAO_DynUnion_i::next (CORBA::Environment &env)
{
  if (this->index_ + 1 == this->type_->member_count (env))
    return 0;

  ++this->index_;

  if (!CORBA::is_nil (this->member_.in ()))
    this->member_->destroy (env);

  this->member_ =
    TAO_DynAny_i::create_dyn_any (this->type_->member_type (this->index_,
                                                            env),
                                  env);
  return 1;
}

CORBA::Boolean
TAO_DynUnion_i::seek (CORBA::Long index,
                      CORBA::Environment &env)
{
  if (index < 0 || index >= (CORBA::Long) this->type_->member_count (env))
    return 0;

  this->index_ = index;

  if (!CORBA::is_nil (this->member_.in ()))
    this->member_->destroy (env);

  this->member_ =
    TAO_DynAny_i::create_dyn_any (this->type_->member_type (this->index_,
                                                            env),
                                  env);
  return 1;
}

void
TAO_DynUnion_i::rewind (CORBA::Environment &env)
{
  if (this->index_ == 0)
    return;

  this->index_ = 0;

  if (!CORBA::is_nil (this->member_.in ()))
    this->member_->destroy (env);

  this->member_ =
    TAO_DynAny_i::create_dyn_any (this->type_->member_type (this->index_,
                                                            env),
                                  env);
}

/////////////////////////////////////////////////////////////////////////////
// Insert and get functions
//
// Note that next() is not called in the body of these functions as it is
// with DynStruct, DynSequence and DynArray. It doesn't seem to make sense
// for unions.

///////////////////////////////////////////////////////////////////////////////
// Insert functions

void
TAO_DynUnion_i::insert_boolean (CORBA::Boolean value,
                                CORBA::Environment &env)
{
  if (TAO_DynAny_i::unalias (this->member_->type (env),
                             env)
      == CORBA::tk_boolean)
    this->member_->insert_boolean (value,
                                   env);
  else
    {
      CORBA::ULong count = this->type_->member_count (env);

      for (CORBA::ULong i = 0; i < count; i++)
        {
          CORBA::TypeCode_ptr tc = this->type_->member_type (i,
                                                             env);

          if (TAO_DynAny_i::unalias (tc,
                                     env)
              == CORBA::tk_boolean)
            {
              this->member_ = TAO_DynAny_i::create_dyn_any (tc,
                                                            env);
              this->member_->insert_boolean (value,
                                             env);
              this->discriminator_ =
                TAO_DynAny_i::create_dyn_any (*this->type_->member_label (i,
                                                                          env),
                                              env);
              this->index_ = i;
              return;
            }
        }

      env.exception (new CORBA_DynAny::InvalidValue);
    }
}

void
TAO_DynUnion_i::insert_octet (CORBA::Octet value,
                              CORBA::Environment &env)
{
  if (TAO_DynAny_i::unalias (this->member_->type (env),
                             env)
      == CORBA::tk_octet)
    this->member_->insert_octet (value,
                                 env);
  else
    {
      CORBA::ULong count = this->type_->member_count (env);

      for (CORBA::ULong i = 0; i < count; i++)
        {
          CORBA::TypeCode_ptr tc = this->type_->member_type (i,
                                                             env);

          if (TAO_DynAny_i::unalias (tc,
                                     env)
              == CORBA::tk_octet)
            {
              this->member_ = TAO_DynAny_i::create_dyn_any (tc,
                                                            env);
              this->member_->insert_octet (value,
                                           env);
              this->discriminator_ =
                TAO_DynAny_i::create_dyn_any (*this->type_->member_label (i,
                                                                          env),
                                              env);
              this->index_ = i;
              return;
            }
        }

      env.exception (new CORBA_DynAny::InvalidValue);
    }
}

void
TAO_DynUnion_i::insert_char (CORBA::Char value,
                             CORBA::Environment &env)
{
  if (TAO_DynAny_i::unalias (this->member_->type (env),
                             env)
      == CORBA::tk_char)
    this->member_->insert_char (value,
                                env);
  else
    {
      CORBA::ULong count = this->type_->member_count (env);

      for (CORBA::ULong i = 0; i < count; i++)
        {
          CORBA::TypeCode_ptr tc = this->type_->member_type (i,
                                                             env);

          if (TAO_DynAny_i::unalias (tc,
                                     env)
              == CORBA::tk_char)
            {
              this->member_ = TAO_DynAny_i::create_dyn_any (tc,
                                                            env);
              this->member_->insert_char (value,
                                          env);
              this->discriminator_ =
                TAO_DynAny_i::create_dyn_any (*this->type_->member_label (i,
                                                                          env),
                                              env);
              this->index_ = i;
              return;
            }
        }

      env.exception (new CORBA_DynAny::InvalidValue);
    }
}

void
TAO_DynUnion_i::insert_short (CORBA::Short value,
                              CORBA::Environment &env)
{
  if (TAO_DynAny_i::unalias (this->member_->type (env),
                             env)
      == CORBA::tk_short)
    this->member_->insert_short (value, env);
  else
    {
      CORBA::ULong count = this->type_->member_count (env);

      for (CORBA::ULong i = 0; i < count; i++)
        {
          CORBA::TypeCode_ptr tc = this->type_->member_type (i,
                                                             env);

          if (TAO_DynAny_i::unalias (tc,
                                     env)
              == CORBA::tk_short)
            {
              this->member_ = TAO_DynAny_i::create_dyn_any (tc,
                                                            env);
              this->member_->insert_short (value,
                                           env);
              this->discriminator_ =
                TAO_DynAny_i::create_dyn_any (*this->type_->member_label (i,
                                                                          env),
                                              env);
              this->index_ = i;
              return;
            }
        }

      env.exception (new CORBA_DynAny::InvalidValue);
    }
}

void
TAO_DynUnion_i::insert_long (CORBA::Long value,
                             CORBA::Environment &env)
{
  if (TAO_DynAny_i::unalias (this->member_->type (env),
                             env)
      == CORBA::tk_long)
    this->member_->insert_long (value,
                                env);
  else
    {
      CORBA::ULong count = this->type_->member_count (env);

      for (CORBA::ULong i = 0; i < count; i++)
        {
          CORBA::TypeCode_ptr tc = this->type_->member_type (i,
                                                             env);

          if (TAO_DynAny_i::unalias (tc,
                                     env)
              == CORBA::tk_long)
            {
              this->member_ = TAO_DynAny_i::create_dyn_any (tc,
                                                            env);
              this->member_->insert_long (value,
                                          env);
              this->discriminator_ =
                TAO_DynAny_i::create_dyn_any (*this->type_->member_label (i,
                                                                          env),
                                              env);
              this->index_ = i;
              return;
            }
        }

      env.exception (new CORBA_DynAny::InvalidValue);
    }
}

void
TAO_DynUnion_i::insert_ushort (CORBA::UShort value,
                               CORBA::Environment &env)
{
  if (TAO_DynAny_i::unalias (this->member_->type (env),
                             env)
      == CORBA::tk_ushort)
    this->member_->insert_ushort (value,
                                  env);
  else
    {
      CORBA::ULong count = this->type_->member_count (env);

      for (CORBA::ULong i = 0; i < count; i++)
        {
          CORBA::TypeCode_ptr tc = this->type_->member_type (i,
                                                             env);

          if (TAO_DynAny_i::unalias (tc,
                                     env)
              == CORBA::tk_ushort)
            {
              this->member_ = TAO_DynAny_i::create_dyn_any (tc,
                                                            env);
              this->member_->insert_ushort (value,
                                            env);
              this->discriminator_ =
                TAO_DynAny_i::create_dyn_any (*this->type_->member_label (i,
                                                                          env),
                                              env);
              this->index_ = i;
              return;
            }
        }

      env.exception (new CORBA_DynAny::InvalidValue);
    }
}

void
TAO_DynUnion_i::insert_ulong (CORBA::ULong value,
                              CORBA::Environment &env)
{
  if (TAO_DynAny_i::unalias (this->member_->type (env),
                             env)
      == CORBA::tk_ulong)
    this->member_->insert_ulong (value,
                                 env);
  else
    {
      CORBA::ULong count = this->type_->member_count (env);

      for (CORBA::ULong i = 0; i < count; i++)
        {
          CORBA::TypeCode_ptr tc = this->type_->member_type (i,
                                                             env);

          if (TAO_DynAny_i::unalias (tc,
                                     env)
              == CORBA::tk_ulong)
            {
              this->member_ = TAO_DynAny_i::create_dyn_any (tc,
                                                            env);
              this->member_->insert_ulong (value,
                                           env);
              this->discriminator_ =
                TAO_DynAny_i::create_dyn_any (*this->type_->member_label (i,
                                                                          env),
                                              env);
              this->index_ = i;
              return;
            }
        }

      env.exception (new CORBA_DynAny::InvalidValue);
    }
}

void
TAO_DynUnion_i::insert_float (CORBA::Float value,
                              CORBA::Environment &env)
{
  if (TAO_DynAny_i::unalias (this->member_->type (env),
                             env)
      == CORBA::tk_float)
    this->member_->insert_float (value,
                                 env);
  else
    {
      CORBA::ULong count = this->type_->member_count (env);

      for (CORBA::ULong i = 0; i < count; i++)
        {
          CORBA::TypeCode_ptr tc = this->type_->member_type (i,
                                                             env);

          if (TAO_DynAny_i::unalias (tc,
                                     env)
              == CORBA::tk_float)
            {
              this->member_ = TAO_DynAny_i::create_dyn_any (tc,
                                                            env);
              this->member_->insert_float (value,
                                           env);
              this->discriminator_ =
                TAO_DynAny_i::create_dyn_any (*this->type_->member_label (i,
                                                                          env),
                                              env);
              this->index_ = i;
              return;
            }
        }

      env.exception (new CORBA_DynAny::InvalidValue);
    }
}

void
TAO_DynUnion_i::insert_double (CORBA::Double value,
                               CORBA::Environment &env)
{
  if (TAO_DynAny_i::unalias (this->member_->type (env),
                             env)
      == CORBA::tk_double)
    this->member_->insert_double (value,
                                  env);
  else
    {
      CORBA::ULong count = this->type_->member_count (env);

      for (CORBA::ULong i = 0; i < count; i++)
        {
          CORBA::TypeCode_ptr tc = this->type_->member_type (i,
                                                             env);

          if (TAO_DynAny_i::unalias (tc,
                                     env)
              == CORBA::tk_double)
            {
              this->member_ = TAO_DynAny_i::create_dyn_any (tc,
                                                            env);
              this->member_->insert_double (value,
                                            env);
              this->discriminator_ =
                TAO_DynAny_i::create_dyn_any (*this->type_->member_label (i,
                                                                          env),
                                              env);
              this->index_ = i;
              return;
            }
        }

      env.exception (new CORBA_DynAny::InvalidValue);
    }
}

void
TAO_DynUnion_i::insert_string (const char* value,
                               CORBA::Environment &env)
{
  if (TAO_DynAny_i::unalias (this->member_->type (env),
                             env)
      == CORBA::tk_string)
    this->member_->insert_string (value,
                                  env);
  else
    {
      CORBA::ULong count = this->type_->member_count (env);

      for (CORBA::ULong i = 0; i < count; i++)
        {
          CORBA::TypeCode_ptr tc = this->type_->member_type (i,
                                                             env);

          if (TAO_DynAny_i::unalias (tc,
                                     env)
              == CORBA::tk_string)
            {
              this->member_ = TAO_DynAny_i::create_dyn_any (tc,
                                                            env);
              this->member_->insert_string (value,
                                            env);
              this->discriminator_ =
                TAO_DynAny_i::create_dyn_any (*this->type_->member_label (i,
                                                                          env),
                                              env);
              this->index_ = i;
              return;
            }
        }

      env.exception (new CORBA_DynAny::InvalidValue);
    }
}

void
TAO_DynUnion_i::insert_reference (CORBA::Object_ptr value,
                                  CORBA::Environment &env)
{
  if (TAO_DynAny_i::unalias (this->member_->type (env),
                             env)
      == CORBA::tk_objref)
    this->member_->insert_reference (value,
                                     env);
  else
    {
      CORBA::ULong count = this->type_->member_count (env);

      for (CORBA::ULong i = 0; i < count; i++)
        {
          CORBA::TypeCode_ptr tc = this->type_->member_type (i,
                                                             env);

          if (TAO_DynAny_i::unalias (tc,
                                     env)
              == CORBA::tk_objref)
            {
              this->member_ = TAO_DynAny_i::create_dyn_any (tc,
                                                            env);
              this->member_->insert_reference (value,
                                               env);
              this->discriminator_ =
                TAO_DynAny_i::create_dyn_any (*this->type_->member_label (i,
                                                                          env),
                                              env);
              this->index_ = i;
              return;
            }
        }

      env.exception (new CORBA_DynAny::InvalidValue);
    }
}

void
TAO_DynUnion_i::insert_typecode (CORBA::TypeCode_ptr value,
                                 CORBA::Environment &env)
{
  if (TAO_DynAny_i::unalias (this->member_->type (env),
                             env)
      == CORBA::tk_TypeCode)
    this->member_->insert_typecode (value,
                                    env);
  else
    {
      CORBA::ULong count = this->type_->member_count (env);

      for (CORBA::ULong i = 0; i < count; i++)
        {
          CORBA::TypeCode_ptr tc = this->type_->member_type (i,
                                                             env);

          if (TAO_DynAny_i::unalias (tc,
                                     env)
              == CORBA::tk_TypeCode)
            {
              this->member_ = TAO_DynAny_i::create_dyn_any (tc,
                                                            env);
              this->member_->insert_typecode (value,
                                              env);
              this->discriminator_ =
                TAO_DynAny_i::create_dyn_any (*this->type_->member_label (i,
                                                                          env),
                                              env);
              this->index_ = i;
              return;
            }
        }

      env.exception (new CORBA_DynAny::InvalidValue);
    }
}

void
TAO_DynUnion_i::insert_longlong (CORBA::LongLong value,
                                 CORBA::Environment &env)
{
  if (TAO_DynAny_i::unalias (this->member_->type (env),
                             env)
      == CORBA::tk_longlong)
    this->member_->insert_longlong (value,
                                    env);
  else
    {
      CORBA::ULong count = this->type_->member_count (env);

      for (CORBA::ULong i = 0; i < count; i++)
        {
          CORBA::TypeCode_ptr tc = this->type_->member_type (i,
                                                             env);

          if (TAO_DynAny_i::unalias (tc,
                                     env)
              == CORBA::tk_longlong)
            {
              this->member_ = TAO_DynAny_i::create_dyn_any (tc,
                                                            env);
              this->member_->insert_longlong (value,
                                              env);
              this->discriminator_ =
                TAO_DynAny_i::create_dyn_any (*this->type_->member_label (i,
                                                                          env),
                                              env);
              this->index_ = i;
              return;
            }
        }

      env.exception (new CORBA_DynAny::InvalidValue);
    }
}

void
TAO_DynUnion_i::insert_ulonglong (CORBA::ULongLong value,
                                  CORBA::Environment &env)
{
  if (TAO_DynAny_i::unalias (this->member_->type (env),
                             env)
      == CORBA::tk_ulonglong)
    this->member_->insert_ulonglong (value,
                                     env);
  else
    {
      CORBA::ULong count = this->type_->member_count (env);

      for (CORBA::ULong i = 0; i < count; i++)
        {
          CORBA::TypeCode_ptr tc = this->type_->member_type (i,
                                                             env);

          if (TAO_DynAny_i::unalias (tc,
                                     env)
              == CORBA::tk_ulonglong)
            {
              this->member_ = TAO_DynAny_i::create_dyn_any (tc,
                                                            env);
              this->member_->insert_ulonglong (value,
                                               env);
              this->discriminator_ =
                TAO_DynAny_i::create_dyn_any (*this->type_->member_label (i,
                                                                          env),
                                              env);
              this->index_ = i;
              return;
            }
        }

      env.exception (new CORBA_DynAny::InvalidValue);
    }
}

void
TAO_DynUnion_i::insert_wchar (CORBA::WChar value,
                              CORBA::Environment &env)
{
  if (TAO_DynAny_i::unalias (this->member_->type (env),
                             env)
      == CORBA::tk_wchar)
    this->member_->insert_wchar (value,
                                 env);
  else
    {
      CORBA::ULong count = this->type_->member_count (env);

      for (CORBA::ULong i = 0; i < count; i++)
        {
          CORBA::TypeCode_ptr tc = this->type_->member_type (i,
                                                             env);

          if (TAO_DynAny_i::unalias (tc,
                                     env)
              == CORBA::tk_wchar)
            {
              this->member_ = TAO_DynAny_i::create_dyn_any (tc,
                                                            env);
              this->member_->insert_wchar (value,
                                           env);
              this->discriminator_ =
                TAO_DynAny_i::create_dyn_any (*this->type_->member_label (i,
                                                                          env),
                                              env);
              this->index_ = i;
              return;
            }
        }

      env.exception (new CORBA_DynAny::InvalidValue);
    }
}

void
TAO_DynUnion_i::insert_any (const CORBA::Any& value,
                            CORBA::Environment &env)
{
  if (TAO_DynAny_i::unalias (this->member_->type (env),
                             env)
      == CORBA::tk_any)
    this->member_->insert_any (value, env);
  else
    {
      CORBA::ULong count = this->type_->member_count (env);

      for (CORBA::ULong i = 0; i < count; i++)
        {
          CORBA::TypeCode_ptr tc = this->type_->member_type (i,
                                                             env);

          if (TAO_DynAny_i::unalias (tc,
                                     env)
              == CORBA::tk_any)
            {
              this->member_ = TAO_DynAny_i::create_dyn_any (tc,
                                                            env);
              this->member_->insert_any (value,
                                         env);
              this->discriminator_ =
                TAO_DynAny_i::create_dyn_any (*this->type_->member_label (i,
                                                                          env),
                                              env);
              this->index_ = i;
              return;
            }
        }

      env.exception (new CORBA_DynAny::InvalidValue);
    }
}

/////////////////////////////////////////////////////////////////////////////////
// Get functions

CORBA::Boolean
TAO_DynUnion_i::get_boolean (CORBA::Environment &env)
{
  if (this->member_.in () && TAO_DynAny_i::unalias (this->member_->type (env),
                                                    env)
      == CORBA::tk_boolean)
    return this->member_->get_boolean (env);
  else
    {
      env.exception (new CORBA_DynAny::Invalid);
      return 0;
    }
}

CORBA::Octet
TAO_DynUnion_i::get_octet (CORBA::Environment &env)
{
  if (this->member_.in () && TAO_DynAny_i::unalias (this->member_->type (env),
                                                    env)
      == CORBA::tk_octet)
    return this->member_->get_octet (env);
  else
    {
      env.exception (new CORBA_DynAny::Invalid);
      return 0;
    }
}

CORBA::Char
TAO_DynUnion_i::get_char (CORBA::Environment &env)
{
  if (this->member_.in () && TAO_DynAny_i::unalias (this->member_->type (env),
                                                    env)
      == CORBA::tk_char)
    return this->member_->get_char (env);
  else
    {
      env.exception (new CORBA_DynAny::Invalid);
      return 0;
    }
}

CORBA::Short
TAO_DynUnion_i::get_short (CORBA::Environment &env)
{
  if (this->member_.in () && TAO_DynAny_i::unalias (this->member_->type (env),
                                                    env)
      == CORBA::tk_short)
    return this->member_->get_short (env);
  else
    {
      env.exception (new CORBA_DynAny::Invalid);
      return 0;
    }
}

CORBA::Long
TAO_DynUnion_i::get_long (CORBA::Environment &env)
{
  if (this->member_.in () && TAO_DynAny_i::unalias (this->member_->type (env),
                                                    env)
      == CORBA::tk_long)
    return this->member_->get_long (env);
  else
    {
      env.exception (new CORBA_DynAny::Invalid);
      return 0;
    }
}

CORBA::UShort
TAO_DynUnion_i::get_ushort(CORBA::Environment &env)
{
  if (this->member_.in () && TAO_DynAny_i::unalias (this->member_->type (env),
                                                    env)
      == CORBA::tk_ushort)
    return this->member_->get_ushort (env);
  else
    {
      env.exception (new CORBA_DynAny::Invalid);
      return 0;
    }
}

CORBA::ULong
TAO_DynUnion_i::get_ulong (CORBA::Environment &env)
{
  if (this->member_.in () && TAO_DynAny_i::unalias (this->member_->type (env),
                                                    env)
      == CORBA::tk_ulong)
    return this->member_->get_ulong (env);
  else
    {
      env.exception (new CORBA_DynAny::Invalid);
      return 0;
    }
}

CORBA::Float
TAO_DynUnion_i::get_float (CORBA::Environment &env)
{
  if (this->member_.in () && TAO_DynAny_i::unalias (this->member_->type (env),
                                                    env)
      == CORBA::tk_float)
    return this->member_->get_float (env);
  else
    {
      env.exception (new CORBA_DynAny::Invalid);
      return 0;
    }
}

CORBA::Double
TAO_DynUnion_i::get_double (CORBA::Environment &env)
{
  if (this->member_.in () && TAO_DynAny_i::unalias (this->member_->type (env),
                                                    env)
      == CORBA::tk_double)
    return this->member_->get_double (env);
  else
    {
      env.exception (new CORBA_DynAny::Invalid);
      return 0;
    }
}

CORBA::String
TAO_DynUnion_i::get_string (CORBA::Environment &env)
{
  if (this->member_.in () && TAO_DynAny_i::unalias (this->member_->type (env),
                                                    env)
      == CORBA::tk_string)
    return this->member_->get_string (env);
  else
    {
      env.exception (new CORBA_DynAny::Invalid);
      return 0;
    }
}

CORBA::TypeCode_ptr
TAO_DynUnion_i::get_typecode (CORBA::Environment &env)
{
  if (this->member_.in () && TAO_DynAny_i::unalias (this->member_->type (env),
                                                    env)
      == CORBA::tk_TypeCode)
    return this->member_->get_typecode (env);
  else
    {
      env.exception (new CORBA_DynAny::Invalid);
      return 0;
    }
}

CORBA::Object_ptr
TAO_DynUnion_i::get_reference (CORBA::Environment &env)
{
  if (this->member_.in () && TAO_DynAny_i::unalias (this->member_->type (env),
                                                    env)
      == CORBA::tk_objref)
    return this->member_->get_reference (env);
  else
    {
      env.exception (new CORBA_DynAny::Invalid);
      return 0;
    }
}

CORBA::LongLong
TAO_DynUnion_i::get_longlong (CORBA::Environment &env)
{
  if (this->member_.in () && TAO_DynAny_i::unalias (this->member_->type (env),
                                                    env)
      == CORBA::tk_longlong)
    return this->member_->get_longlong (env);
  else
    {
      env.exception (new CORBA_DynAny::Invalid);

#if defined (ACE_LACKS_LONGLONG_T)
      CORBA::LongLong tmp = {0, 0};
      return tmp;
#else  /* ! ACE_LACKS_LONGLONG_T */
      return 0;
#endif /* ! ACE_LACKS_LONGLONG_T */
    }
}

CORBA::ULongLong
TAO_DynUnion_i::get_ulonglong (CORBA::Environment &env)
{
  if (this->member_.in () && TAO_DynAny_i::unalias (this->member_->type (env),
                                                    env)
      == CORBA::tk_ulonglong)
    return this->member_->get_ulonglong (env);
  else
    {
      env.exception (new CORBA_DynAny::Invalid);
      return 0;
    }
}

CORBA::WChar
TAO_DynUnion_i::get_wchar (CORBA::Environment &env)
{
  if (this->member_.in () && TAO_DynAny_i::unalias (this->member_->type (env),
                                                    env)
      == CORBA::tk_wchar)
    return this->member_->get_wchar (env);
  else
    {
      env.exception (new CORBA_DynAny::Invalid);
      return 0;
    }
}

CORBA::Any_ptr
TAO_DynUnion_i::get_any (CORBA::Environment &env)
{
  if (this->member_.in () && TAO_DynAny_i::unalias (this->member_->type (env),
                                                    env)
      == CORBA::tk_any)
    return this->member_->get_any (env);
  else
    {
      env.exception (new CORBA_DynAny::Invalid);
      return 0;
    }
}

/////////////////////////////////////////////////////////////////////////////
// Private functions

// Some specialized instantiations of the template functor defined in
// DynUnion_i_T.cpp.

CORBA::Boolean
DU_Extractor<CORBA::Boolean>::check_match (
    const CORBA_Any& inside_any,
    const CORBA_Any& outside_any
  )
{
  CORBA::Any::to_boolean member_struct (this->member_index_);
  CORBA::Any::to_boolean arg_struct (this->arg_index_);
  inside_any >>= member_struct;
  outside_any >>= arg_struct;
  return member_struct.ref_ == arg_struct.ref_;
}

CORBA::Boolean
DU_Extractor<CORBA::Char>::check_match (
    const CORBA_Any& inside_any,
    const CORBA_Any& outside_any
  )
{
  CORBA::Any::to_char member_struct (this->member_index_);
  CORBA::Any::to_char arg_struct (this->arg_index_);
  inside_any >>= member_struct;
  outside_any >>= arg_struct;
  return member_struct.ref_ == arg_struct.ref_;
}

CORBA::Boolean
TAO_DynUnion_i::WChar_extractor::check_match (const CORBA_Any& inside_any,
                                              const CORBA_Any& outside_any)
{
  CORBA::Any::to_wchar member_struct (this->member_index_);
  CORBA::Any::to_wchar arg_struct (this->arg_index_);
  inside_any >>= member_struct;
  outside_any >>= arg_struct;
  return member_struct.ref_ == arg_struct.ref_;
}

CORBA::Boolean
TAO_DynUnion_i::Enum_extractor::check_match (const CORBA_Any& inside_any,
                                             const CORBA_Any& outside_any)
{
  // Get the CDR stream of one argument...
  ACE_Message_Block* mb = inside_any._tao_get_cdr ();

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
                               CORBA::Environment& TAO_IN_ENV)
{
  DU_Extractor_base* retval;

  switch (kind)
    {
      case CORBA::tk_short:
        ACE_NEW_THROW_RETURN (retval,
                              DU_Extractor<CORBA::Short>,
                              CORBA::NO_MEMORY (CORBA::COMPLETED_NO),
                              0);
        return retval;
      case CORBA::tk_long:
        ACE_NEW_THROW_RETURN (retval,
                              DU_Extractor<CORBA::Long>,
                              CORBA::NO_MEMORY (CORBA::COMPLETED_NO),
                              0);
        return retval;
      case CORBA::tk_ushort:
        ACE_NEW_THROW_RETURN (retval,
                              DU_Extractor<CORBA::UShort>,
                              CORBA::NO_MEMORY (CORBA::COMPLETED_NO),
                              0);
        return retval;
      case CORBA::tk_ulong:
        ACE_NEW_THROW_RETURN (retval,
                              DU_Extractor<CORBA::ULong>,
                              CORBA::NO_MEMORY (CORBA::COMPLETED_NO),
                              0);
        return retval;
      case CORBA::tk_boolean:
        ACE_NEW_THROW_RETURN (retval,
                              DU_Extractor<CORBA::Boolean>,
                              CORBA::NO_MEMORY (CORBA::COMPLETED_NO),
                              0);
        return retval;
      case CORBA::tk_char:
        ACE_NEW_THROW_RETURN (retval,
                              DU_Extractor<CORBA::Char>,
                              CORBA::NO_MEMORY (CORBA::COMPLETED_NO),
                              0);
        return retval;

// For platforms without native 64-bit ints . . .
#if !defined (ACE_LACKS_LONGLONG_T)
      case CORBA::tk_longlong:
        ACE_NEW_THROW_RETURN (retval,
                              DU_Extractor<CORBA::LongLong>,
                              CORBA::NO_MEMORY (CORBA::COMPLETED_NO),
                              0);

        return retval;
#endif /* ACE_LACKS_LONGLONG_T */

      case CORBA::tk_ulonglong:
        ACE_NEW_THROW_RETURN (retval,
                              DU_Extractor<CORBA::ULongLong>,
                              CORBA::NO_MEMORY (CORBA::COMPLETED_NO),
                              0);
        return retval;
      case CORBA::tk_wchar:
        ACE_NEW_THROW_RETURN (retval,
                              WChar_extractor,
                              CORBA::NO_MEMORY (CORBA::COMPLETED_NO),
                              0);
        return retval;
      case CORBA::tk_enum:
        ACE_NEW_THROW_RETURN (retval,
                              Enum_extractor,
                              CORBA::NO_MEMORY (CORBA::COMPLETED_NO),
                              0);
        return retval;
      default:
        return 0;
    }
}

// This code is common to from_any and the constructor taking an Any
// argument.
void
TAO_DynUnion_i::set_from_any (const CORBA_Any& any,
                              CORBA::Environment &env)
{
  // Get the CDR stream of the argument.
  ACE_Message_Block* mb = any._tao_get_cdr ();

  TAO_InputCDR cdr (mb);

  CORBA_TypeCode_ptr disc_tc = any.type ()->discriminator_type (env);

  CORBA_Any disc_any (disc_tc,
                      cdr.start ());

  if (!CORBA::is_nil (this->discriminator_.in()))
    this->discriminator_->destroy (env);

  // Set the discriminator holder.
  this->discriminator_ = TAO_DynAny_i::create_dyn_any (disc_any,
                                                       env);

  cdr.skip (disc_tc);

  CORBA::ULong count = any.type ()->member_count (env);
  CORBA::ULong i;
  CORBA::Long default_index = -1;

  CORBA::TCKind discriminator_kind = disc_tc->kind (env);

  DU_Extractor_base* functor =
    this->get_extractor (discriminator_kind,
                         env);

  // Get the index.
  for (i = 0; i < count; i++)
    {
      CORBA_Any label_any = *any.type ()->member_label (i,
                                                        env);

      CORBA::TCKind label_kind = label_any.type ()->kind (env);

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
          this->index_ = i;
          break;
        }
    }

  delete functor;

  if (i == count && default_index == -1)
    env.exception (new CORBA_DynAny::Invalid);
  else
    {
      if (i == count && default_index != -1)
        this->index_ = default_index;

      CORBA_Any member_any (any.type ()->member_type (this->index_,
                                                      env),
                            cdr.start ());

      if (!CORBA::is_nil (this->member_.in ()))
        this->member_->destroy (env);

      this->member_ = TAO_DynAny_i::create_dyn_any (member_any,
                                                    env);
    }
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class DU_Extractor<CORBA::Short>;
template class DU_Extractor<CORBA::Long>;
template class DU_Extractor<CORBA::UShort>;
template class DU_Extractor<CORBA::ULong>;
template class DU_Extractor<CORBA::Boolean>;
template class DU_Extractor<CORBA::Char>;

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
#pragma instantiate DU_Extractor<CORBA::Boolean>
#pragma instantiate DU_Extractor<CORBA::Char>

// For platforms without native 64-bit ints . . .
#if !defined (ACE_LACKS_LONGLONG_T)
#pragma instantiate DU_Extractor<CORBA::LongLong>
#endif /* ACE_LACKS_LONGLONG_T */

#pragma instantiate DU_Extractor<CORBA::ULongLong>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
