/* -*- C++ -*- */
// $Id$
// ===================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    DynSequence_i.cpp
//
// = AUTHOR
//    Jeff Parsons <jp4@cs.wustl.edu>
//
// ===================================================================

#include "tao/DynAny_i.h"
#include "tao/DynSequence_i.h"
#include "tao/InconsistentTypeCodeC.h"

//////////////////////////////////////////////////////////////////////
// Constructors and destructor

TAO_DynSequence_i::TAO_DynSequence_i (const CORBA_Any& any)
  : type_ (any.type ()),
    index_ (0),
    da_members_ (0)
{
  CORBA::Environment env;

  // The type will be correct if this constructor called from a
  // factory function, but it could also be called by the
  // user, so.....
  if (TAO_DynAny_i::unalias (this->type_.in (),
                             env)
       == CORBA::tk_sequence)
    {
      // Get the CDR stream of the argument.
      ACE_Message_Block* mb = any._tao_get_cdr ();

      TAO_InputCDR cdr (mb);

      CORBA::ULong length;

      // If the any is a sequence, first 4 bytes of cdr hold the length.
      cdr.read_ulong (length);

      // Resize the array.
      this->da_members_.size (length);

      // Get the type of the sequence elments.
      CORBA::TypeCode_ptr field_tc = this->get_element_type (env);

      for (CORBA::ULong i = 0; i < length; i++)
        {
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

// Can't set the length from just the typecode, so we'll
// do it upon initialization.
TAO_DynSequence_i::TAO_DynSequence_i (CORBA_TypeCode_ptr tc)
  : type_ (CORBA::TypeCode::_duplicate (tc)),
    index_ (0),
    da_members_ (0)
{
  CORBA::Environment env;

  // Need to check if called by user.
  if (TAO_DynAny_i::unalias (tc,
                             env)
       != CORBA::tk_sequence)
    env.exception (new CORBA_ORB_InconsistentTypeCode);
}

TAO_DynSequence_i::~TAO_DynSequence_i (void)
{
}

///////////////////////////////////////////////////////////////////////
// Functions specific to DynSequence

CORBA::ULong
TAO_DynSequence_i::length (CORBA::Environment &)
{
  return this->da_members_.size ();
}

// There is no way to resize an ACE_Array except by assignment to
// another ACE_Array of a different size, so we have to copy over
// and copy back, even if the result is only to decrease the size.
void
TAO_DynSequence_i::length (CORBA::ULong length,
                           CORBA::Environment &)
{
  this->da_members_.size (length);
}

AnySeq_ptr
TAO_DynSequence_i::get_elements (CORBA::Environment& TAO_IN_ENV)
{
  CORBA::ULong length = this->da_members_.size ();

  if (length == 0)
    return 0;

  AnySeq_ptr elements;
  ACE_NEW_THROW_RETURN (elements,
                        AnySeq (length),
                        CORBA::NO_MEMORY (),
                        0);

  // We must do this explicitly.
  elements->length (length);

  // Initialize each Any.
  for (CORBA::ULong i = 0; i < length; i++)
    {
      CORBA::Any_ptr temp = this->da_members_[i]->to_any (TAO_IN_ENV);
      (*elements)[i] = *temp;
      delete temp;
    }

  return elements;
}

void
TAO_DynSequence_i::set_elements (const AnySeq& value,
                                 CORBA::Environment& env)
{
  CORBA::ULong length = value.length ();
  CORBA::ULong size = this->da_members_.size ();

  if (size == 0)
    // Resize the array, it hasn't yet been initialized.
    this->da_members_.size (length);
  else if (size != length)
    {
      env.exception (new CORBA_DynAny::InvalidSeq);
      return;
    }

  CORBA::TypeCode_ptr element_type = this->get_element_type (env);

  for (CORBA::ULong i = 0; i < length; i++)
    {
      // Check each arg element for type match.
      if (value[i].type ()->equal (element_type,
                                   env))
        {
          if (!CORBA::is_nil (this->da_members_[i].in ()))
            this->da_members_[i]->destroy (env);

          this->da_members_[i] =
            TAO_DynAny_i::create_dyn_any (value[i],
                                          env);
        }
      else
        {
          env.exception (new CORBA_DynAny::InvalidSeq);
          break;
        }
    }
}

//////////////////////////////////////////////////////////////////////
// Common functions

void
TAO_DynSequence_i::assign (CORBA_DynAny_ptr dyn_any,
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
TAO_DynSequence_i::copy (CORBA::Environment &env)
{
  CORBA_Any_ptr a = this->to_any (env);
  return TAO_DynAny_i::create_dyn_any (*a,
                                       env);
}

void
TAO_DynSequence_i::destroy (CORBA::Environment &env)
{
  // Do a deep destroy
  for (CORBA::ULong i = 0; i < this->da_members_.size (); i++)
    if (!CORBA::is_nil (this->da_members_[i].in ()))
      this->da_members_[i]->destroy (env);

  // Free the top level
  delete this;
}

void
TAO_DynSequence_i::from_any (const CORBA_Any& any,
                             CORBA::Environment &env)
{
  if (this->type_.in ()->equal (any.type (),
                                env))
    {
      // Get the CDR stream of the argument.
      ACE_Message_Block* mb = any._tao_get_cdr ();
      TAO_InputCDR cdr (mb);

      CORBA::ULong arg_length;

      // If the any is a sequence, first 4 bytes of cdr hold the length.
      cdr.read_ulong (arg_length);

      CORBA::ULong length = this->da_members_.size ();

      if (length == 0)
        // Resize the array, it hasn't yet been initialized.
        this->da_members_.size (arg_length);
      else if (length != arg_length)
        {
          env.exception (new CORBA_DynAny::Invalid);
          return;
        }

      CORBA::TypeCode_ptr field_tc = this->get_element_type (env);

      for (CORBA::ULong i = 0; i < arg_length; i++)
        {
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
TAO_DynSequence_i::to_any (CORBA::Environment& TAO_IN_ENV)
{
  TAO_OutputCDR out_cdr;

  out_cdr.write_ulong (this->da_members_.size ());

  CORBA_TypeCode_ptr field_tc = this->get_element_type (TAO_IN_ENV);

  for (CORBA::ULong i = 0; i < this->da_members_.size (); i++)
    {
      // Each component must have been initialized.
      if (!this->da_members_[i].in ())
        {
          TAO_IN_ENV.exception (new CORBA_DynAny::Invalid);
          return 0;
        }

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
TAO_DynSequence_i::type (CORBA::Environment &)
{
  return this->type_.in ();
}

// If the DynAny has been initialized but this component has not, the
// first call to current_component will create the pointer and return it.
CORBA_DynAny_ptr
TAO_DynSequence_i::current_component (CORBA::Environment &env)
{
  if (this->da_members_.size () == 0)
    return 0;

  if (!this->da_members_[this->index_].in ())
    this->da_members_[this->index_] =
      TAO_DynAny_i::create_dyn_any (this->get_element_type (env),
                                    env);

  return this->da_members_[this->index_].in ();
}

CORBA::Boolean
TAO_DynSequence_i::next (CORBA::Environment &)
{
  CORBA::Long size = (CORBA::Long) this->da_members_.size ();

  if (size == 0 || this->index_ + 1 == size)
    return 0;

  ++this->index_;
  return 1;
}

CORBA::Boolean
TAO_DynSequence_i::seek (CORBA::Long index,
                         CORBA::Environment &)
{
  if (index < 0 || index >= (CORBA::Long) this->da_members_.size ())
    return 0;

  this->index_ = index;
  return 1;
}

void
TAO_DynSequence_i::rewind (CORBA::Environment &)
{
  this->index_ = 0;
}

///////////////////////////////////////////////////////////////////////
// The insert-primitive and get-primitive functions are required
// by the spec of all types of DynAny, although if the top level
// members aren't primitive types, these functions aren't too helpful.
// Also, while not mentioned in the spec, the example code seems to
// indicate that next() is called in the body of each of these, and
// it has been so implemented here.

///////////////////////////////////////////////////////////////////////
// Insert functions

void
TAO_DynSequence_i::insert_boolean (CORBA::Boolean value,
                                   CORBA::Environment &env)
{
  if (this->get_element_type (env)->kind (env) == CORBA::tk_boolean)
    {
      this->current_component (env)->insert_boolean (value,
                                                     env);
      this->next (env);
    }
  else
    env.exception (new CORBA_DynAny::InvalidValue);
}

void
TAO_DynSequence_i::insert_octet (CORBA::Octet value,
                                 CORBA::Environment &env)
{
  if (this->get_element_type (env)->kind (env) == CORBA::tk_octet)
    {
      this->current_component (env)->insert_octet (value,
                                                   env);
      this->next (env);
    }
  else
    env.exception (new CORBA_DynAny::InvalidValue);
}

void
TAO_DynSequence_i::insert_char (CORBA::Char value,
                                CORBA::Environment &env)
{
  if (this->get_element_type (env)->kind (env) == CORBA::tk_char)
    {
      this->current_component (env)->insert_char (value,
                                                  env);
      this->next (env);
    }
  else
    env.exception (new CORBA_DynAny::InvalidValue);
}

void
TAO_DynSequence_i::insert_short (CORBA::Short value,
                                 CORBA::Environment &env)
{
  if (this->get_element_type (env)->kind (env) == CORBA::tk_short)
    {
      this->current_component (env)->insert_short (value,
                                                   env);
      this->next (env);
    }
  else
    env.exception (new CORBA_DynAny::InvalidValue);
}

void
TAO_DynSequence_i::insert_ushort (CORBA::UShort value,
                                  CORBA::Environment &env)
{
  if (this->get_element_type (env)->kind (env) == CORBA::tk_ushort)
    {
      this->current_component (env)->insert_ushort (value,
                                                    env);
      this->next (env);
    }
  else
    env.exception (new CORBA_DynAny::InvalidValue);
}

void
TAO_DynSequence_i::insert_long (CORBA::Long value,
                                CORBA::Environment &env)
{
  if (this->get_element_type (env)->kind (env) == CORBA::tk_long)
    {
      this->current_component (env)->insert_long (value,
                                                  env);
      this->next (env);
    }
  else
    env.exception (new CORBA_DynAny::InvalidValue);
}

void
TAO_DynSequence_i::insert_ulong (CORBA::ULong value,
                                 CORBA::Environment &env)
{
  if (this->get_element_type (env)->kind (env) == CORBA::tk_long)
    {
      this->current_component (env)->insert_ulong (value,
                                                   env);
      this->next (env);
    }
  else
    env.exception (new CORBA_DynAny::InvalidValue);
}

void
TAO_DynSequence_i::insert_float (CORBA::Float value,
                                 CORBA::Environment &env)
{
  if (this->get_element_type (env)->kind (env) == CORBA::tk_float)
    {
      this->current_component (env)->insert_float (value,
                                                   env);
      this->next (env);
    }
  else
    env.exception (new CORBA_DynAny::InvalidValue);
}

void
TAO_DynSequence_i::insert_double (CORBA::Double value,
                                  CORBA::Environment &env)
{
  if (this->get_element_type (env)->kind (env) == CORBA::tk_double)
    {
      this->current_component (env)->insert_double (value,
                                                    env);
      this->next (env);
    }
  else
    env.exception (new CORBA_DynAny::InvalidValue);
}

void
TAO_DynSequence_i::insert_string (const char * value,
                                  CORBA::Environment &env)
{
  if (this->get_element_type (env)->kind (env) == CORBA::tk_string)
    {
      this->current_component (env)->insert_string (value,
                                                    env);
      this->next (env);
    }
  else
    env.exception (new CORBA_DynAny::InvalidValue);
}

void
TAO_DynSequence_i::insert_reference (CORBA::Object_ptr value,
                                     CORBA::Environment &env)
{
  if (this->get_element_type (env)->kind (env) == CORBA::tk_objref)
    {
      this->current_component (env)->insert_reference (value,
                                                       env);
      this->next (env);
    }
  else
    env.exception (new CORBA_DynAny::InvalidValue);
}

void
TAO_DynSequence_i::insert_typecode (CORBA::TypeCode_ptr value,
                                    CORBA::Environment &env)
{
  if (this->get_element_type (env)->kind (env) == CORBA::tk_TypeCode)
    {
      this->current_component (env)->insert_typecode (value,
                                                      env);
      this->next (env);
    }
  else
    env.exception (new CORBA_DynAny::InvalidValue);
}

void
TAO_DynSequence_i::insert_longlong (CORBA::LongLong value,
                                    CORBA::Environment &env)
{
  if (this->get_element_type (env)->kind (env) == CORBA::tk_longlong)
    {
      this->current_component (env)->insert_longlong (value,
                                                      env);
      this->next (env);
    }
  else
    env.exception (new CORBA_DynAny::InvalidValue);
}

void
TAO_DynSequence_i::insert_ulonglong (CORBA::ULongLong value,
                                     CORBA::Environment &env)
{
  if (this->get_element_type (env)->kind (env) == CORBA::tk_ulonglong)
    {
      this->current_component (env)->insert_ulonglong (value,
                                                       env);
      this->next (env);
    }
  else
    env.exception (new CORBA_DynAny::InvalidValue);
}

void
TAO_DynSequence_i::insert_wchar (CORBA::WChar value,
                                 CORBA::Environment &env)
{
  if (this->get_element_type (env)->kind (env) == CORBA::tk_wchar)
    {
      this->current_component (env)->insert_wchar (value,
                                                   env);
      this->next (env);
    }
  else
    env.exception (new CORBA_DynAny::InvalidValue);
}

void
TAO_DynSequence_i::insert_any (const CORBA::Any& value,
                               CORBA::Environment &env)
{
  if (this->get_element_type (env)->kind (env) == CORBA::tk_any)
    {
      this->current_component (env)->insert_any (value,
                                                 env);
      this->next (env);
    }
  else
    env.exception (new CORBA_DynAny::InvalidValue);
}

///////////////////////////////////////////////////////////////////////
// Get functions

// If the current component has not been intialized, these
// raise Invalid, which is not required by the spec, but which
// seems like a courteous thing to do.

CORBA::Boolean
TAO_DynSequence_i::get_boolean (CORBA::Environment &env)
{
  CORBA::Boolean val = 0;
  CORBA_DynAny_ptr dp = this->da_members_[this->index_].in ();

  if (dp)
    {
      if (this->get_element_type (env)->kind (env) == CORBA::tk_boolean)
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
TAO_DynSequence_i::get_octet (CORBA::Environment &env)
{
  CORBA::Octet val = 0;
  CORBA_DynAny_ptr dp = this->da_members_[this->index_].in ();

  if (dp)
    {
      if (this->get_element_type (env)->kind (env) == CORBA::tk_octet)
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
TAO_DynSequence_i::get_char (CORBA::Environment &env)
{
  CORBA::Char val = 0;
  CORBA_DynAny_ptr dp = this->da_members_[this->index_].in ();

  if (dp)
    {
      if (this->get_element_type (env)->kind (env) == CORBA::tk_char)
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
TAO_DynSequence_i::get_short (CORBA::Environment &env)
{
  CORBA::Short val = 0;
  CORBA_DynAny_ptr dp = this->da_members_[this->index_].in ();

  if (dp)
    {
      if (this->get_element_type (env)->kind (env) == CORBA::tk_short)
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
TAO_DynSequence_i::get_ushort (CORBA::Environment &env)
{
  CORBA::UShort val = 0;
  CORBA_DynAny_ptr dp = this->da_members_[this->index_].in ();

  if (dp)
    {
      if (this->get_element_type (env)->kind (env) == CORBA::tk_ushort)
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
TAO_DynSequence_i::get_long (CORBA::Environment &env)
{
  CORBA::Long val = 0;
  CORBA_DynAny_ptr dp = this->da_members_[this->index_].in ();

  if (dp)
    {
      if (this->get_element_type (env)->kind (env) == CORBA::tk_long)
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
TAO_DynSequence_i::get_ulong (CORBA::Environment &env)
{
  CORBA::ULong val = 0;
  CORBA_DynAny_ptr dp = this->da_members_[this->index_].in ();

  if (dp)
    {
      if (this->get_element_type (env)->kind (env) == CORBA::tk_ulong)
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
TAO_DynSequence_i::get_float (CORBA::Environment &env)
{
  CORBA::Float val = 0;
  CORBA_DynAny_ptr dp = this->da_members_[this->index_].in ();

  if (dp)
    {
      if (this->get_element_type (env)->kind (env) == CORBA::tk_float)
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
TAO_DynSequence_i::get_double (CORBA::Environment &env)
{
  CORBA::Double val = 0;
  CORBA_DynAny_ptr dp = this->da_members_[this->index_].in ();

  if (dp)
    {
      if (this->get_element_type (env)->kind (env) == CORBA::tk_double)
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
TAO_DynSequence_i::get_string (CORBA::Environment &env)
{
  CORBA::Char *val = 0;
  CORBA_DynAny_ptr dp = this->da_members_[this->index_].in ();

  if (dp)
    {
      if (this->get_element_type (env)->kind (env) == CORBA::tk_string)
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
TAO_DynSequence_i::get_reference (CORBA::Environment &env)
{
  CORBA_Object_ptr val = 0;
  CORBA_DynAny_ptr dp = this->da_members_[this->index_].in ();

  if (dp)
    {
      if (dp->type (env)->kind (env) == CORBA::tk_objref)
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
TAO_DynSequence_i::get_typecode (CORBA::Environment &env)
{
  CORBA_TypeCode_ptr val = 0;
  CORBA_DynAny_ptr dp = this->da_members_[this->index_].in ();

  if (dp)
    {
      if (this->get_element_type (env)->kind (env) == CORBA::tk_TypeCode)
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
TAO_DynSequence_i::get_longlong (CORBA::Environment &env)
{
#if defined (ACE_LACKS_LONGLONG_T)
  CORBA::LongLong val = {0, 0};
#else  /* ! ACE_LACKS_LONGLONG_T */
  CORBA::LongLong val = 0;
#endif /* ! ACE_LACKS_LONGLONG_T */
  CORBA_DynAny_ptr dp = this->da_members_[this->index_].in ();

  if (dp)
    {
      if (this->get_element_type (env)->kind (env) == CORBA::tk_longlong)
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
TAO_DynSequence_i::get_ulonglong (CORBA::Environment &env)
{
  CORBA::ULongLong val = 0;
  CORBA_DynAny_ptr dp = this->da_members_[this->index_].in ();

  if (dp)
    {
      if (this->get_element_type (env)->kind (env) == CORBA::tk_ulonglong)
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
TAO_DynSequence_i::get_wchar (CORBA::Environment &env)
{
  CORBA::WChar val = 0;
  CORBA_DynAny_ptr dp = this->da_members_[this->index_].in ();

  if (dp)
    {
      if (this->get_element_type (env)->kind (env) == CORBA::tk_wchar)
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
TAO_DynSequence_i::get_any (CORBA::Environment &env)
{
  CORBA_Any_ptr val = 0;
  CORBA_DynAny_ptr dp = this->da_members_[this->index_].in ();

  if (dp)
    {
      if (this->get_element_type (env)->kind (env) == CORBA::tk_any)
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

//////////////////////////////////////////////////////////////////////
// Private utility function.
CORBA::TypeCode_ptr
TAO_DynSequence_i::get_element_type (CORBA::Environment& env)
{
  CORBA::TypeCode_ptr element_type = this->type_.in ();

  // Strip away aliases (if any) on top of the outer type
  while (element_type->kind (env) != CORBA::tk_sequence)
    element_type = element_type->content_type (env);

  // Return the content type.
  return element_type->content_type (env);
}

