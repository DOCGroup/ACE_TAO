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
//    Jeff Parsons <jp4@cs.wustl.edu>
//
// ====================================================================

#include "tao/DynAny_i.h"

#if !defined (TAO_HAS_MINIMUM_CORBA)

#include "tao/DynStruct_i.h"
#include "tao/InconsistentTypeCodeC.h"

//////////////////////////////////////////////////////////////////////
// Constructors and destructor

TAO_DynStruct_i::TAO_DynStruct_i (const CORBA_Any& any)
  : type_ (any.type ()),
    index_ (0),
    da_members_ (0)
{
  CORBA::Environment env;

  // The type will be correct if this constructor called from a
  // factory function, but it could also be called by the
  // user, so.....
  if (TAO_DynAny_i::unalias (this->type_.in (), env) == CORBA::tk_struct)
    {
      CORBA::ULong numfields = this->type_.in ()->member_count (env);

      // Resize the array.
      this->da_members_.size (numfields);

      // Get the CDR stream of the argument.
      ACE_Message_Block* mb = any._tao_get_cdr ();

      TAO_InputCDR cdr (mb);

      for (CORBA::ULong i = 0; i < numfields; i++)
        {
          CORBA::TypeCode_ptr field_tc =
            this->type_.in ()->member_type (i,
                                            env);

          // This Any constructor is a TAO extension.
          CORBA_Any field_any (field_tc,
                               cdr.start ());

          // This recursive step will call the correct constructor
          // based on the type of field_any.
          this->da_members_[i] =
            TAO_DynAny_i::create_dyn_any (field_any,
                                          env);

          // Move to the next field in the CDR stream.
          cdr.skip (field_tc);
        }
    }
  else
    env.exception (new CORBA_ORB_InconsistentTypeCode);
}

TAO_DynStruct_i::TAO_DynStruct_i (CORBA_TypeCode_ptr tc)
  : type_ (CORBA::TypeCode::_duplicate (tc)),
    index_ (0),
    da_members_ (0)
{
  CORBA::Environment env;

  // Need to check if called by user.
  if (TAO_DynAny_i::unalias (tc, env) == CORBA::tk_struct)
    {
      CORBA::ULong numfields = tc->member_count (env);

      // Resize the array.
      this->da_members_.size (numfields);

      for (CORBA::ULong i = 0; i < numfields; i++)

        // With a typecode arg, we just create the top level.
        this->da_members_[i] = 0;
    }
  else
    env.exception (new CORBA_ORB_InconsistentTypeCode);
}

TAO_DynStruct_i::~TAO_DynStruct_i (void)
{
}

//////////////////////////////////////////////////////////////////////////
// Functions specific to DynStruct

FieldName
TAO_DynStruct_i::current_member_name (CORBA::Environment &)
{
  return CORBA::string_dup (this->type_.in ()->member_name (this->index_));
}

// Returns the unaliased TCKind.
CORBA::TCKind
TAO_DynStruct_i::current_member_kind (CORBA::Environment& env)
{
  return TAO_DynAny_i::unalias (this->type_.in ()->member_type (this->index_,
                                                                env),
                                env);
}

NameValuePairSeq*
TAO_DynStruct_i::get_members (CORBA::Environment& TAO_IN_ENV)
{
  CORBA::ULong length = this->da_members_.size ();

  NameValuePairSeq* members;
  ACE_NEW_THROW_RETURN (members,
                        NameValuePairSeq (length),
                        CORBA::NO_MEMORY (),
                        0);

  // We must do this explicitly.
  members->length (length);

  // Assign name and value to each pearl on the string.
  for (CORBA::ULong i = 0; i < length; i++)
    {
      (*members)[i].id =
        CORBA::string_dup (this->type_.in ()->member_name (i));

      CORBA::Any_ptr temp = this->da_members_[i]->to_any (TAO_IN_ENV);
      (*members)[i].value = *temp;
      delete temp;
    }

  return members;
}

void
TAO_DynStruct_i::set_members (const NameValuePairSeq& value,
                              CORBA::Environment& env)
{
  CORBA::ULong length = value.length ();

  // Check for length match.
  if (length == this->da_members_.size ())
    {
      for (CORBA::ULong i = 0; i < length; i++)
        {
          // Check for type and name match.
          if (value[i].value.type ()->equal (this->type_.in ()->member_type (i,
                                                                             env))
              && !ACE_OS::strcmp (value[i].id,
                                  this->type_.in ()->member_name (i)))
            {
              if (!CORBA::is_nil (this->da_members_[i].in ()))
                this->da_members_[i]->destroy (env);

              this->da_members_[i] =
                TAO_DynAny_i::create_dyn_any (value[i].value,
                                              env);
            }
          else
            {
              env.exception (new CORBA_DynAny::InvalidSeq);
              break;
            }
        }
    }
  else
    env.exception (new CORBA_DynAny::InvalidSeq);
}

//////////////////////////////////////////////////////////////////////
// Common functions

void
TAO_DynStruct_i::assign (CORBA_DynAny_ptr dyn_any,
                         CORBA::Environment &env)
{
  // *dyn_any->to_any raises Invalid if arg is bad.
  if (this->type_.in ()->equal (dyn_any->type (env),
                                env))
    this->from_any (*dyn_any->to_any (env),
                    env);
  else
    env.exception (new CORBA_DynAny::Invalid);
}

CORBA_DynAny_ptr
TAO_DynStruct_i::copy (CORBA::Environment &env)
{
  CORBA_Any_ptr a = this->to_any (env);
  return TAO_DynAny_i::create_dyn_any (*a,
                                       env);
}

void
TAO_DynStruct_i::destroy (CORBA::Environment &env)
{
  // Do a deep destroy
  for (CORBA::ULong i = 0; i < this->da_members_.size (); i++)
    if (!CORBA::is_nil (this->da_members_[i].in ()))
      this->da_members_[i]->destroy (env);

  // Free the top level
  delete this;
}

void
TAO_DynStruct_i::from_any (const CORBA_Any& any,
                           CORBA::Environment &env)
{
  if (this->type_.in ()->equal (any.type (),
                                env))
    {
      // Get the CDR stream of the argument.
      ACE_Message_Block* mb = any._tao_get_cdr ();
      TAO_InputCDR cdr (mb);

      for (CORBA::ULong i = 0; i < this->da_members_.size (); i++)
        {
          CORBA::TypeCode_ptr field_tc =
            this->type_.in ()->member_type (i,
                                            env);

          // This Any constructor is a TAO extension.
          CORBA_Any field_any (field_tc,
                               cdr.start ());

          if (!CORBA::is_nil (this->da_members_[i].in ()))
            this->da_members_[i]->destroy (env);

          this->da_members_[i] =
            TAO_DynAny_i::create_dyn_any (field_any,
                                          env);

          // Move to the next field in the CDR stream.
          cdr.skip (field_tc);
        }
    }
  else
    env.exception (new CORBA_DynAny::Invalid);
}

CORBA::Any_ptr
TAO_DynStruct_i::to_any (CORBA::Environment& TAO_IN_ENV)
{
  TAO_OutputCDR out_cdr;

  for (CORBA::ULong i = 0; i < this->da_members_.size (); i++)
    {
      // Each component must have been initialied.
      if (!this->da_members_[i].in ())
        {
          TAO_IN_ENV.exception (new CORBA_DynAny::Invalid);
          return 0;
        }

      CORBA_TypeCode_ptr field_tc = this->da_members_[i]->type (TAO_IN_ENV);

      // Recursive step
      CORBA_Any_ptr field_any = this->da_members_[i]->to_any (TAO_IN_ENV);

      ACE_Message_Block* field_mb = field_any->_tao_get_cdr ();

      TAO_InputCDR field_cdr (field_mb);

      out_cdr.append (field_tc,
                      &field_cdr,
                      TAO_IN_ENV);

      delete field_any;
    }

  TAO_InputCDR in_cdr (out_cdr);

  CORBA_Any* retval;
  ACE_NEW_THROW_RETURN (retval,
                        CORBA_Any (this->type (TAO_IN_ENV),
                                   in_cdr.start ()),
                        CORBA::NO_MEMORY (),
                        0);
  return retval;
}

CORBA::TypeCode_ptr
TAO_DynStruct_i::type (CORBA::Environment &)
{
  return this->type_.in ();
}

// If this component hasn't been initialized yet, the first call
// to current_component will create the pointer and return it.
CORBA_DynAny_ptr
TAO_DynStruct_i::current_component (CORBA::Environment &env)
{
  if (!this->da_members_[this->index_].in ())
    this->da_members_[this->index_] =
      TAO_DynAny_i::create_dyn_any (
        this->type_.in ()->member_type (this->index_,
                                        env),
        env
      );

  return this->da_members_[this->index_].in ();
}

CORBA::Boolean
TAO_DynStruct_i::next (CORBA::Environment &)
{
  if (this->index_ + 1 == (CORBA::Long) this->da_members_.size ())
    return 0;

  ++this->index_;
  return 1;
}

CORBA::Boolean
TAO_DynStruct_i::seek (CORBA::Long index,
                       CORBA::Environment &)
{
  if (index < 0 || index >= (CORBA::Long) this->da_members_.size ())
    return 0;

  this->index_ = index;
  return 1;
}

void
TAO_DynStruct_i::rewind (CORBA::Environment &)
{
  this->index_ = 0;
}

//////////////////////////////////////////////////////////////////////////
// The insert-primitive and get-primitive functions are required
// by the spec of all types of DynAny, although if the top level
// members aren't primitive types, these functions aren't too helpful.
// Also, while not mentioned in the spec, the example code seems to
// indicate that next() is called in the body of each of these, and
// it has been so implemented here.

/////////////////////////////////////////////////////////////////////////////
// Insert functions

void
TAO_DynStruct_i::insert_boolean (CORBA::Boolean value,
                                 CORBA::Environment &env)
{
  if (TAO_DynAny_i::unalias (this->type_.in ()->member_type (this->index_),
                             env)
    == CORBA::tk_boolean)
    {
      this->current_component (env)->insert_boolean (value,
                                                     env);
      this->next (env);
    }
  else
    env.exception (new CORBA_DynAny::InvalidValue);
}

void
TAO_DynStruct_i::insert_octet (CORBA::Octet value,
                               CORBA::Environment &env)
{
  if (TAO_DynAny_i::unalias (this->type_.in ()->member_type (this->index_),
                             env)
    == CORBA::tk_octet)
    {
      this->current_component (env)->insert_octet (value,
                                                   env);
      this->next (env);
    }
  else
    env.exception (new CORBA_DynAny::InvalidValue);
}

void
TAO_DynStruct_i::insert_char (CORBA::Char value,
                              CORBA::Environment &env)
{
  if (TAO_DynAny_i::unalias (this->type_.in ()->member_type (this->index_),
                             env)
    == CORBA::tk_char)
    {
      this->current_component (env)->insert_char (value,
                                                  env);
      this->next (env);
    }
  else
    env.exception (new CORBA_DynAny::InvalidValue);
}

void
TAO_DynStruct_i::insert_short (CORBA::Short value,
                               CORBA::Environment &env)
{
  if (TAO_DynAny_i::unalias (this->type_.in ()->member_type (this->index_),
                             env)
    == CORBA::tk_short)
    {
      this->current_component (env)->insert_short (value,
                                                   env);
      this->next (env);
    }
  else
    env.exception (new CORBA_DynAny::InvalidValue);
}

void
TAO_DynStruct_i::insert_ushort (CORBA::UShort value,
                                CORBA::Environment &env)
{
  if (TAO_DynAny_i::unalias (this->type_.in ()->member_type (this->index_),
                             env)
    == CORBA::tk_ushort)
    {
      this->current_component (env)->insert_ushort (value,
                                                    env);
      this->next (env);
    }
  else
    env.exception (new CORBA_DynAny::InvalidValue);
}

void
TAO_DynStruct_i::insert_long (CORBA::Long value,
                              CORBA::Environment &env)
{
  if (TAO_DynAny_i::unalias (this->type_.in ()->member_type (this->index_),
                             env)
    == CORBA::tk_long)
    {
      this->current_component (env)->insert_long (value,
                                                  env);
      this->next (env);
    }
  else
    env.exception (new CORBA_DynAny::InvalidValue);
}

void
TAO_DynStruct_i::insert_ulong (CORBA::ULong value,
                               CORBA::Environment &env)
{
  if (TAO_DynAny_i::unalias (this->type_.in ()->member_type (this->index_),
                             env)
    == CORBA::tk_long)
    {
      this->current_component (env)->insert_ulong (value,
                                                   env);
      this->next (env);
    }
  else
    env.exception (new CORBA_DynAny::InvalidValue);
}

void
TAO_DynStruct_i::insert_float (CORBA::Float value,
                               CORBA::Environment &env)
{
  if (TAO_DynAny_i::unalias (this->type_.in ()->member_type (this->index_),
                             env)
    == CORBA::tk_float)
    {
      this->current_component (env)->insert_float (value,
                                                   env);
      this->next (env);
    }
  else
    env.exception (new CORBA_DynAny::InvalidValue);
}

void
TAO_DynStruct_i::insert_double (CORBA::Double value,
                                CORBA::Environment &env)
{
  if (TAO_DynAny_i::unalias (this->type_.in ()->member_type (this->index_),
                             env)
    == CORBA::tk_double)
    {
      this->current_component (env)->insert_double (value,
                                                    env);
      this->next (env);
    }
  else
    env.exception (new CORBA_DynAny::InvalidValue);
}

void
TAO_DynStruct_i::insert_string (const char * value,
                                CORBA::Environment &env)
{
  if (TAO_DynAny_i::unalias (this->type_.in ()->member_type (this->index_),
                             env)
    == CORBA::tk_string)
    {
      this->current_component (env)->insert_string (value,
                                                    env);
      this->next (env);
    }
  else
    env.exception (new CORBA_DynAny::InvalidValue);
}

void
TAO_DynStruct_i::insert_reference (CORBA::Object_ptr value,
                                   CORBA::Environment &env)
{
  if (TAO_DynAny_i::unalias (this->type_.in ()->member_type (this->index_),
                             env)
    == CORBA::tk_objref)
    {
      this->current_component (env)->insert_reference (value,
                                                       env);
      this->next (env);
    }
  else
    env.exception (new CORBA_DynAny::InvalidValue);
}

void
TAO_DynStruct_i::insert_typecode (CORBA::TypeCode_ptr value,
                                  CORBA::Environment &env)
{
  if (TAO_DynAny_i::unalias (this->type_.in ()->member_type (this->index_),
                             env)
    == CORBA::tk_TypeCode)
    {
      this->current_component (env)->insert_typecode (value,
                                                      env);
      this->next (env);
    }
  else
    env.exception (new CORBA_DynAny::InvalidValue);
}

void
TAO_DynStruct_i::insert_longlong (CORBA::LongLong value,
                                  CORBA::Environment &env)
{
  if (TAO_DynAny_i::unalias (this->type_.in ()->member_type (this->index_),
                             env)
    == CORBA::tk_longlong)
    {
      this->current_component (env)->insert_longlong (value,
                                                      env);
      this->next (env);
    }
  else
    env.exception (new CORBA_DynAny::InvalidValue);
}

void
TAO_DynStruct_i::insert_ulonglong (CORBA::ULongLong value,
                                   CORBA::Environment &env)
{
  if (TAO_DynAny_i::unalias (this->type_.in ()->member_type (this->index_),
                             env)
    == CORBA::tk_ulonglong)
    {
      this->current_component (env)->insert_ulonglong (value,
                                                       env);
      this->next (env);
    }
  else
    env.exception (new CORBA_DynAny::InvalidValue);
}

void
TAO_DynStruct_i::insert_wchar (CORBA::WChar value,
                               CORBA::Environment &env)
{
  if (TAO_DynAny_i::unalias (this->type_.in ()->member_type (this->index_),
                             env)
    == CORBA::tk_wchar)
    {
      this->current_component (env)->insert_wchar (value,
                                                   env);
      this->next (env);
    }
  else
    env.exception (new CORBA_DynAny::InvalidValue);
}

void
TAO_DynStruct_i::insert_any (const CORBA::Any& value,
                             CORBA::Environment &env)
{
  if (TAO_DynAny_i::unalias (this->type_.in ()->member_type (this->index_),
                             env)
    == CORBA::tk_any)
    {
      this->current_component (env)->insert_any (value,
                                                 env);
      this->next (env);
    }
  else
    env.exception (new CORBA_DynAny::InvalidValue);
}

///////////////////////////////////////////////////////////////////
// Get functions

// If the current component has not been intialized, these
// raise Invalid, which is not required by the spec, but which
// seems like a courteous thing to do.

CORBA::Boolean
TAO_DynStruct_i::get_boolean (CORBA::Environment &env)
{
  CORBA::Boolean val = 0;;
  CORBA_DynAny_ptr dp = this->da_members_[this->index_].in ();

  if (dp)
    {
      if (TAO_DynAny_i::unalias (dp->type (env),
                                 env)
        == CORBA::tk_boolean)
        {
          val = dp->get_boolean (env);
          this->next (env);
        }
      else
        env.exception (new CORBA_DynAny::TypeMismatch);
    }
  else
    env.exception (new CORBA_DynAny::Invalid);

  return val;
}

CORBA::Octet
TAO_DynStruct_i::get_octet (CORBA::Environment &env)
{
  CORBA::Octet val = 0;
  CORBA_DynAny_ptr dp = this->da_members_[this->index_].in ();

  if (dp)
    {
      if (TAO_DynAny_i::unalias (dp->type (env),
                                 env)
        == CORBA::tk_octet)
        {
          val = dp->get_octet (env);
          this->next (env);
        }
      else
        env.exception (new CORBA_DynAny::TypeMismatch);
    }
  else
    env.exception (new CORBA_DynAny::Invalid);

  return val;
}

CORBA::Char
TAO_DynStruct_i::get_char (CORBA::Environment &env)
{
  CORBA::Char val = 0;
  CORBA_DynAny_ptr dp = this->da_members_[this->index_].in ();

  if (dp)
    {
      if (TAO_DynAny_i::unalias (dp->type (env),
                                 env)
        == CORBA::tk_char)
        {
          val = dp->get_char (env);
          this->next (env);
        }
      else
        env.exception (new CORBA_DynAny::TypeMismatch);
    }
  else
    env.exception (new CORBA_DynAny::Invalid);

  return val;
}

CORBA::Short
TAO_DynStruct_i::get_short (CORBA::Environment &env)
{
  CORBA::Short val = 0;
  CORBA_DynAny_ptr dp = this->da_members_[this->index_].in ();

  if (dp)
    {
      if (TAO_DynAny_i::unalias (dp->type (env),
                                 env)
        == CORBA::tk_short)
        {
          val = dp->get_short (env);
          this->next (env);
        }
      else
        env.exception (new CORBA_DynAny::TypeMismatch);
    }
  else
    env.exception (new CORBA_DynAny::Invalid);

  return val;
}

CORBA::UShort
TAO_DynStruct_i::get_ushort (CORBA::Environment &env)
{
  CORBA::UShort val = 0;
  CORBA_DynAny_ptr dp = this->da_members_[this->index_].in ();

  if (dp)
    {
      if (TAO_DynAny_i::unalias (dp->type (env),
                                 env)
        == CORBA::tk_ushort)
        {
          val = dp->get_ushort (env);
          this->next (env);
        }
      else
        env.exception (new CORBA_DynAny::TypeMismatch);
    }
  else
    env.exception (new CORBA_DynAny::Invalid);

  return val;
}

CORBA::Long
TAO_DynStruct_i::get_long (CORBA::Environment &env)
{
  CORBA::Long val = 0;
  CORBA_DynAny_ptr dp = this->da_members_[this->index_].in ();

  if (dp)
    {
      if (TAO_DynAny_i::unalias (dp->type (env),
                                 env)
        == CORBA::tk_long)
        {
          val = dp->get_long (env);
          this->next (env);
        }
      else
        env.exception (new CORBA_DynAny::TypeMismatch);
    }
  else
    env.exception (new CORBA_DynAny::Invalid);

  return val;
}

CORBA::ULong
TAO_DynStruct_i::get_ulong (CORBA::Environment &env)
{
  CORBA::ULong val = 0;
  CORBA_DynAny_ptr dp = this->da_members_[this->index_].in ();

  if (dp)
    {
      if (TAO_DynAny_i::unalias (dp->type (env),
                                 env)
        == CORBA::tk_ulong)
        {
          val = dp->get_ulong (env);
          this->next (env);
        }
      else
        env.exception (new CORBA_DynAny::TypeMismatch);
    }
  else
    env.exception (new CORBA_DynAny::Invalid);

  return val;
}

CORBA::Float
TAO_DynStruct_i::get_float (CORBA::Environment &env)
{
  CORBA::Float val = 0;
  CORBA_DynAny_ptr dp = this->da_members_[this->index_].in ();

  if (dp)
    {
      if (TAO_DynAny_i::unalias (dp->type (env),
                                 env)
        == CORBA::tk_float)
        {
          val = dp->get_float (env);
          this->next (env);
        }
      else
        env.exception (new CORBA_DynAny::TypeMismatch);
    }
  else
    env.exception (new CORBA_DynAny::Invalid);

  return val;
}

CORBA::Double
TAO_DynStruct_i::get_double (CORBA::Environment &env)
{
  CORBA::Double val = 0;
  CORBA_DynAny_ptr dp = this->da_members_[this->index_].in ();

  if (dp)
    {
      if (TAO_DynAny_i::unalias (dp->type (env),
                                 env)
        == CORBA::tk_double)
        {
          val = dp->get_double (env);
          this->next (env);
        }
      else
        env.exception (new CORBA_DynAny::TypeMismatch);
    }
  else
    env.exception (new CORBA_DynAny::Invalid);

  return val;
}

char *
TAO_DynStruct_i::get_string (CORBA::Environment &env)
{
  CORBA::Char *val = 0;
  CORBA_DynAny_ptr dp = this->da_members_[this->index_].in ();

  if (dp)
    {
      if (TAO_DynAny_i::unalias (dp->type (env),
                                 env)
        == CORBA::tk_string)
        {
          val = dp->get_string (env);
          this->next (env);
        }
      else
        env.exception (new CORBA_DynAny::TypeMismatch);
    }
  else
    env.exception (new CORBA_DynAny::Invalid);

  return val;
}

CORBA::Object_ptr
TAO_DynStruct_i::get_reference (CORBA::Environment &env)
{
  CORBA_Object_ptr val = 0;
  CORBA_DynAny_ptr dp = this->da_members_[this->index_].in ();

  if (dp)
    {
      if (TAO_DynAny_i::unalias (dp->type (env),
                                 env)
        == CORBA::tk_objref)
        {
          val = dp->get_reference (env);
          this->next (env);
        }
      else
        env.exception (new CORBA_DynAny::TypeMismatch);
    }
  else
    env.exception (new CORBA_DynAny::Invalid);

  return val;
}

CORBA::TypeCode_ptr
TAO_DynStruct_i::get_typecode (CORBA::Environment &env)
{
  CORBA_TypeCode_ptr val = 0;
  CORBA_DynAny_ptr dp = this->da_members_[this->index_].in ();

  if (dp)
    {
      if (TAO_DynAny_i::unalias (dp->type (env),
                                 env)
        == CORBA::tk_TypeCode)
        {
          val = dp->get_typecode (env);
          this->next (env);
        }
      else
        env.exception (new CORBA_DynAny::TypeMismatch);
    }
  else
    env.exception (new CORBA_DynAny::Invalid);

  return val;
}

CORBA::LongLong
TAO_DynStruct_i::get_longlong (CORBA::Environment &env)
{
#if defined (ACE_LACKS_LONGLONG_T)
  CORBA::LongLong val = {0, 0};
#else  /* ! ACE_LACKS_LONGLONG_T */
  CORBA::LongLong val = 0;
#endif /* ! ACE_LACKS_LONGLONG_T */
  CORBA_DynAny_ptr dp = this->da_members_[this->index_].in ();

  if (dp)
    {
      if (TAO_DynAny_i::unalias (dp->type (env),
                                 env)
        == CORBA::tk_longlong)
        {
          val = dp->get_longlong (env);
          this->next (env);
        }
      else
        env.exception (new CORBA_DynAny::TypeMismatch);
    }
  else
    env.exception (new CORBA_DynAny::Invalid);

  return val;
}

CORBA::ULongLong
TAO_DynStruct_i::get_ulonglong (CORBA::Environment &env)
{
  CORBA::ULongLong val = 0;
  CORBA_DynAny_ptr dp = this->da_members_[this->index_].in ();

  if (dp)
    {
      if (TAO_DynAny_i::unalias (dp->type (env),
                                 env)
        == CORBA::tk_ulonglong)
        {
          val = dp->get_ulonglong (env);
          this->next (env);
        }
      else
        env.exception (new CORBA_DynAny::TypeMismatch);
    }
  else
    env.exception (new CORBA_DynAny::Invalid);

  return val;
}

CORBA::WChar
TAO_DynStruct_i::get_wchar (CORBA::Environment &env)
{
  CORBA::WChar val = 0;
  CORBA_DynAny_ptr dp = this->da_members_[this->index_].in ();

  if (dp)
    {
      if (TAO_DynAny_i::unalias (dp->type (env),
                                 env)
        == CORBA::tk_wchar)
        {
          val = dp->get_wchar (env);
          this->next (env);
        }
      else
        env.exception (new CORBA_DynAny::TypeMismatch);
    }
  else
    env.exception (new CORBA_DynAny::Invalid);

  return val;
}

CORBA::Any_ptr
TAO_DynStruct_i::get_any (CORBA::Environment &env)
{
  CORBA_Any_ptr val = 0;
  CORBA_DynAny_ptr dp = this->da_members_[this->index_].in ();

  if (dp)
    {
      if (TAO_DynAny_i::unalias (dp->type (env),
                                 env)
        == CORBA::tk_any)
        {
          val = dp->get_any (env);
          this->next (env);
        }
      else
        env.exception (new CORBA_DynAny::TypeMismatch);
    }
  else
    env.exception (new CORBA_DynAny::Invalid);

  return val;
}

#endif /* TAO_HAS_MINIMUM_CORBA */
