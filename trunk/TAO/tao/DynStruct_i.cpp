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

#include "tao/DynAny_i.h"

#if !defined (TAO_HAS_MINIMUM_CORBA)

#include "tao/DynStruct_i.h"
#include "tao/InconsistentTypeCodeC.h"

// Constructors and destructor

TAO_DynStruct_i::TAO_DynStruct_i (const CORBA_Any& any)
  : type_ (any.type ()),
    current_index_ (0),
    da_members_ (0)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // The type will be correct if this constructor called from a
      // factory function, but it could also be called by the user,
      // so.....
      if (TAO_DynAny_i::unalias (this->type_.in (),
                                 ACE_TRY_ENV) == CORBA::tk_struct)
        {
          CORBA::ULong numfields =
            this->type_.in ()->member_count (ACE_TRY_ENV);
          ACE_TRY_CHECK;

          // Resize the array.
          this->da_members_.size (numfields);

          // Get the CDR stream of the argument.
          ACE_Message_Block *mb = any._tao_get_cdr ();

          TAO_InputCDR cdr (mb);

          for (CORBA::ULong i = 0; i < numfields; i++)
            {
              CORBA::TypeCode_ptr field_tc =
                this->type_.in ()->member_type (i,
                                                ACE_TRY_ENV);
              ACE_TRY_CHECK;

              // This Any constructor is a TAO extension.
              CORBA_Any field_any (field_tc,
                                  0,
                                  cdr.start ());

              // This recursive step will call the correct constructor
              // based on the type of field_any.
              this->da_members_[i] =
                TAO_DynAny_i::create_dyn_any (field_any,
                                              ACE_TRY_ENV);
              ACE_TRY_CHECK;

              // Move to the next field in the CDR stream.
              cdr.skip (field_tc);
            }
        }
      else
        ACE_THROW (CORBA_ORB_InconsistentTypeCode ());
    }
  ACE_CATCHANY
    {
      // do nothing
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

TAO_DynStruct_i::TAO_DynStruct_i (CORBA_TypeCode_ptr tc)
  : type_ (CORBA::TypeCode::_duplicate (tc)),
    current_index_ (0),
    da_members_ (0)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // Need to check if called by user.
      if (TAO_DynAny_i::unalias (tc, ACE_TRY_ENV) == CORBA::tk_struct)
        {
          CORBA::ULong numfields = tc->member_count (ACE_TRY_ENV);
          ACE_TRY_CHECK;

          // Resize the array.
          this->da_members_.size (numfields);

          for (CORBA::ULong i = 0; i < numfields; i++)

            // With a typecode arg, we just create the top level.
            this->da_members_[i] = 0;
        }
      else
        ACE_THROW (CORBA_ORB_InconsistentTypeCode ());
    }
  ACE_CATCHANY
    {
      // do nothing
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

TAO_DynStruct_i::~TAO_DynStruct_i (void)
{
}

// Functions specific to DynStruct

CORBA::FieldName
TAO_DynStruct_i::current_member_name (CORBA::Environment &)
{
  return CORBA::string_dup (this->type_.in ()->member_name (this->current_index_));
}

// Returns the unaliased TCKind.
CORBA::TCKind
TAO_DynStruct_i::current_member_kind (CORBA::Environment& ACE_TRY_ENV)
{
  return TAO_DynAny_i::unalias (this->type_.in ()->member_type (this->current_index_,
                                                                ACE_TRY_ENV),
                                ACE_TRY_ENV);
}

CORBA::NameValuePairSeq*
TAO_DynStruct_i::get_members (CORBA::Environment& ACE_TRY_ENV)
{
  CORBA::ULong length = this->da_members_.size ();

  CORBA::NameValuePairSeq* members;
  ACE_NEW_THROW_EX (members,
                    CORBA::NameValuePairSeq (length),
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
      (*members)[i].value = temp.in ();
    }

  return members;
}

void
TAO_DynStruct_i::set_members (const CORBA::NameValuePairSeq& value,
                              CORBA::Environment& ACE_TRY_ENV)
{
  CORBA::ULong length = value.length ();

  // Check for length match.
  if (length == this->da_members_.size ())
    {
      for (CORBA::ULong i = 0; i < length; i++)
        {
          // Check for type and name match.
          if (value[i].value.type ()->equal (this->type_.in ()->member_type (i,
                                                                             ACE_TRY_ENV))
              && !ACE_OS::strcmp (value[i].id,
                                  this->type_.in ()->member_name (i)))
            {
              if (!CORBA::is_nil (this->da_members_[i].in ()))
                this->da_members_[i]->destroy (ACE_TRY_ENV);

              this->da_members_[i] =
                TAO_DynAny_i::create_dyn_any (value[i].value,
                                              ACE_TRY_ENV);
            }
          else
            {
              ACE_THROW (CORBA_DynAny::InvalidSeq ());
              break;
            }
        }
    }
  else
    ACE_THROW (CORBA_DynAny::InvalidSeq ());
}

//////////////////////////////////////////////////////////////////////
// Common functions

void
TAO_DynStruct_i::assign (CORBA_DynAny_ptr dyn_any,
                         CORBA::Environment &ACE_TRY_ENV)
{
  // *dyn_any->to_any raises Invalid if arg is bad.
  if (this->type_.in ()->equal (dyn_any->type (ACE_TRY_ENV),
                                ACE_TRY_ENV))
    this->from_any (*dyn_any->to_any (ACE_TRY_ENV),
                    ACE_TRY_ENV);
  else
    ACE_THROW (CORBA_DynAny::Invalid ());
}

CORBA_DynAny_ptr
TAO_DynStruct_i::copy (CORBA::Environment &ACE_TRY_ENV)
{
  CORBA_Any_ptr a = this->to_any (ACE_TRY_ENV);
  return TAO_DynAny_i::create_dyn_any (*a,
                                       ACE_TRY_ENV);
}

void
TAO_DynStruct_i::destroy (CORBA::Environment &ACE_TRY_ENV)
{
  // Do a deep destroy
  for (CORBA::ULong i = 0; i < this->da_members_.size (); i++)
    if (!CORBA::is_nil (this->da_members_[i].in ()))
      this->da_members_[i]->destroy (ACE_TRY_ENV);

  // Free the top level
  delete this;
}

void
TAO_DynStruct_i::from_any (const CORBA_Any& any,
                           CORBA::Environment &ACE_TRY_ENV)
{
  if (this->type_.in ()->equal (any.type (),
                                ACE_TRY_ENV))
    {
      // Get the CDR stream of the argument.
      ACE_Message_Block* mb = any._tao_get_cdr ();
      TAO_InputCDR cdr (mb);

      for (CORBA::ULong i = 0;
           i < this->da_members_.size ();
           i++)
        {
          CORBA::TypeCode_ptr field_tc =
            this->type_.in ()->member_type (i,
                                            ACE_TRY_ENV);

          // This Any constructor is a TAO extension.
          CORBA_Any field_any (field_tc,
                               0,
                               cdr.start ());

          if (!CORBA::is_nil (this->da_members_[i].in ()))
            this->da_members_[i]->destroy (ACE_TRY_ENV);

          this->da_members_[i] =
            TAO_DynAny_i::create_dyn_any (field_any,
                                          ACE_TRY_ENV);

          // Move to the next field in the CDR stream.
          cdr.skip (field_tc);
        }
    }
  else
    ACE_THROW (CORBA_DynAny::Invalid ());
}

CORBA::Any_ptr
TAO_DynStruct_i::to_any (CORBA::Environment& ACE_TRY_ENV)
{
  TAO_OutputCDR out_cdr;

  for (CORBA::ULong i = 0;
       i < this->da_members_.size ();
       i++)
    {
      // Each component must have been initialied.
      if (!this->da_members_[i].in ())
        {
          ACE_THROW_RETURN (CORBA_DynAny::Invalid (), 0);
        }

      CORBA_TypeCode_ptr field_tc =
        this->da_members_[i]->type (ACE_TRY_ENV);

      // Recursive step
      CORBA_Any_var field_any =
        this->da_members_[i]->to_any (ACE_TRY_ENV);

      ACE_Message_Block *field_mb = field_any->_tao_get_cdr ();

      TAO_InputCDR field_cdr (field_mb);

      out_cdr.append (field_tc,
                      &field_cdr,
                      ACE_TRY_ENV);
    }

  TAO_InputCDR in_cdr (out_cdr);

  CORBA_Any *retval;
  ACE_NEW_THROW_EX (retval,
                    CORBA_Any (this->type (ACE_TRY_ENV),
                               0,
                               in_cdr.start ()),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);
  return retval;
}

CORBA::TypeCode_ptr
TAO_DynStruct_i::type (CORBA::Environment &)
{
  return this->type_.in ();
}

// If this component hasn't been initialized yet, the first call to
// current_component will create the pointer and return it.

CORBA_DynAny_ptr
TAO_DynStruct_i::current_component (CORBA::Environment &ACE_TRY_ENV)
{
  if (!this->da_members_[this->current_index_].in ())
    this->da_members_[this->current_index_] =
      TAO_DynAny_i::create_dyn_any
        (this->type_.in ()->member_type (this->current_index_,
                                         ACE_TRY_ENV),
         ACE_TRY_ENV);

  return this->da_members_[this->current_index_].in ();
}

CORBA::Boolean
TAO_DynStruct_i::next (CORBA::Environment &)
{
  if (this->current_index_ + 1 == (CORBA::Long) this->da_members_.size ())
    return 0;

  ++this->current_index_;
  return 1;
}

CORBA::Boolean
TAO_DynStruct_i::seek (CORBA::Long slot,
                       CORBA::Environment &)
{
  if (slot < 0
      || slot >= (CORBA::Long) this->da_members_.size ())
    return 0;

  this->current_index_ = slot;
  return 1;
}

void
TAO_DynStruct_i::rewind (CORBA::Environment &)
{
  this->current_index_ = 0;
}

// The insert-primitive and get-primitive functions are required
// by the spec of all types of DynAny, although if the top level
// members aren't primitive types, these functions aren't too helpful.
// Also, while not mentioned in the spec, the example code seems to
// indicate that next() is called in the body of each of these, and
// it has been so implemented here.

// Insert functions

void
TAO_DynStruct_i::insert_boolean (CORBA::Boolean value,
                                 CORBA::Environment &ACE_TRY_ENV)
{
  if (TAO_DynAny_i::unalias (this->type_.in ()->member_type (this->current_index_),
                             ACE_TRY_ENV)
      == CORBA::tk_boolean)
    {
      this->current_component (ACE_TRY_ENV)->insert_boolean (value,
                                                     ACE_TRY_ENV);
      this->next (ACE_TRY_ENV);
    }
  else
    ACE_THROW (CORBA_DynAny::InvalidValue ());
}

void
TAO_DynStruct_i::insert_octet (CORBA::Octet value,
                               CORBA::Environment &ACE_TRY_ENV)
{
  if (TAO_DynAny_i::unalias (this->type_.in ()->member_type (this->current_index_),
                             ACE_TRY_ENV)
      == CORBA::tk_octet)
    {
      this->current_component (ACE_TRY_ENV)->insert_octet (value,
                                                   ACE_TRY_ENV);
      this->next (ACE_TRY_ENV);
    }
  else
    ACE_THROW (CORBA_DynAny::InvalidValue ());
}

void
TAO_DynStruct_i::insert_char (CORBA::Char value,
                              CORBA::Environment &ACE_TRY_ENV)
{
  if (TAO_DynAny_i::unalias (this->type_.in ()->member_type (this->current_index_),
                             ACE_TRY_ENV)
      == CORBA::tk_char)
    {
      this->current_component (ACE_TRY_ENV)->insert_char (value,
                                                  ACE_TRY_ENV);
      this->next (ACE_TRY_ENV);
    }
  else
    ACE_THROW (CORBA_DynAny::InvalidValue ());
}

void
TAO_DynStruct_i::insert_short (CORBA::Short value,
                               CORBA::Environment &ACE_TRY_ENV)
{
  if (TAO_DynAny_i::unalias (this->type_.in ()->member_type (this->current_index_),
                             ACE_TRY_ENV)
      == CORBA::tk_short)
    {
      this->current_component (ACE_TRY_ENV)->insert_short (value,
                                                   ACE_TRY_ENV);
      this->next (ACE_TRY_ENV);
    }
  else
    ACE_THROW (CORBA_DynAny::InvalidValue ());
}

void
TAO_DynStruct_i::insert_ushort (CORBA::UShort value,
                                CORBA::Environment &ACE_TRY_ENV)
{
  if (TAO_DynAny_i::unalias (this->type_.in ()->member_type (this->current_index_),
                             ACE_TRY_ENV)
      == CORBA::tk_ushort)
    {
      this->current_component (ACE_TRY_ENV)->insert_ushort (value,
                                                    ACE_TRY_ENV);
      this->next (ACE_TRY_ENV);
    }
  else
    ACE_THROW (CORBA_DynAny::InvalidValue ());
}

void
TAO_DynStruct_i::insert_long (CORBA::Long value,
                              CORBA::Environment &ACE_TRY_ENV)
{
  if (TAO_DynAny_i::unalias (this->type_.in ()->member_type (this->current_index_),
                             ACE_TRY_ENV)
      == CORBA::tk_long)
    {
      this->current_component (ACE_TRY_ENV)->insert_long (value,
                                                  ACE_TRY_ENV);
      this->next (ACE_TRY_ENV);
    }
  else
    ACE_THROW (CORBA_DynAny::InvalidValue ());
}

void
TAO_DynStruct_i::insert_ulong (CORBA::ULong value,
                               CORBA::Environment &ACE_TRY_ENV)
{
  if (TAO_DynAny_i::unalias (this->type_.in ()->member_type (this->current_index_),
                             ACE_TRY_ENV)
      == CORBA::tk_long)
    {
      this->current_component (ACE_TRY_ENV)->insert_ulong (value,
                                                   ACE_TRY_ENV);
      this->next (ACE_TRY_ENV);
    }
  else
    ACE_THROW (CORBA_DynAny::InvalidValue ());
}

void
TAO_DynStruct_i::insert_float (CORBA::Float value,
                               CORBA::Environment &ACE_TRY_ENV)
{
  if (TAO_DynAny_i::unalias (this->type_.in ()->member_type (this->current_index_),
                             ACE_TRY_ENV)
      == CORBA::tk_float)
    {
      this->current_component (ACE_TRY_ENV)->insert_float (value,
                                                   ACE_TRY_ENV);
      this->next (ACE_TRY_ENV);
    }
  else
    ACE_THROW (CORBA_DynAny::InvalidValue ());
}

void
TAO_DynStruct_i::insert_double (CORBA::Double value,
                                CORBA::Environment &ACE_TRY_ENV)
{
  if (TAO_DynAny_i::unalias (this->type_.in ()->member_type (this->current_index_),
                             ACE_TRY_ENV)
      == CORBA::tk_double)
    {
      this->current_component (ACE_TRY_ENV)->insert_double (value,
                                                    ACE_TRY_ENV);
      this->next (ACE_TRY_ENV);
    }
  else
    ACE_THROW (CORBA_DynAny::InvalidValue ());
}

void
TAO_DynStruct_i::insert_string (const char * value,
                                CORBA::Environment &ACE_TRY_ENV)
{
  if (TAO_DynAny_i::unalias (this->type_.in ()->member_type (this->current_index_),
                             ACE_TRY_ENV)
      == CORBA::tk_string)
    {
      this->current_component (ACE_TRY_ENV)->insert_string (value,
                                                    ACE_TRY_ENV);
      this->next (ACE_TRY_ENV);
    }
  else
    ACE_THROW (CORBA_DynAny::InvalidValue ());
}

void
TAO_DynStruct_i::insert_reference (CORBA::Object_ptr value,
                                   CORBA::Environment &ACE_TRY_ENV)
{
  if (TAO_DynAny_i::unalias (this->type_.in ()->member_type (this->current_index_),
                             ACE_TRY_ENV)
      == CORBA::tk_objref)
    {
      this->current_component (ACE_TRY_ENV)->insert_reference (value,
                                                       ACE_TRY_ENV);
      this->next (ACE_TRY_ENV);
    }
  else
    ACE_THROW (CORBA_DynAny::InvalidValue ());
}

void
TAO_DynStruct_i::insert_typecode (CORBA::TypeCode_ptr value,
                                  CORBA::Environment &ACE_TRY_ENV)
{
  if (TAO_DynAny_i::unalias (this->type_.in ()->member_type (this->current_index_),
                             ACE_TRY_ENV)
      == CORBA::tk_TypeCode)
    {
      this->current_component (ACE_TRY_ENV)->insert_typecode (value,
                                                      ACE_TRY_ENV);
      this->next (ACE_TRY_ENV);
    }
  else
    ACE_THROW (CORBA_DynAny::InvalidValue ());
}

void
TAO_DynStruct_i::insert_longlong (CORBA::LongLong value,
                                  CORBA::Environment &ACE_TRY_ENV)
{
  if (TAO_DynAny_i::unalias (this->type_.in ()->member_type (this->current_index_),
                             ACE_TRY_ENV)
      == CORBA::tk_longlong)
    {
      this->current_component (ACE_TRY_ENV)->insert_longlong (value,
                                                      ACE_TRY_ENV);
      this->next (ACE_TRY_ENV);
    }
  else
    ACE_THROW (CORBA_DynAny::InvalidValue ());
}

void
TAO_DynStruct_i::insert_ulonglong (CORBA::ULongLong value,
                                   CORBA::Environment &ACE_TRY_ENV)
{
  if (TAO_DynAny_i::unalias (this->type_.in ()->member_type (this->current_index_),
                             ACE_TRY_ENV)
      == CORBA::tk_ulonglong)
    {
      this->current_component (ACE_TRY_ENV)->insert_ulonglong (value,
                                                       ACE_TRY_ENV);
      this->next (ACE_TRY_ENV);
    }
  else
    ACE_THROW (CORBA_DynAny::InvalidValue ());
}

void
TAO_DynStruct_i::insert_wchar (CORBA::WChar value,
                               CORBA::Environment &ACE_TRY_ENV)
{
  if (TAO_DynAny_i::unalias (this->type_.in ()->member_type (this->current_index_),
                             ACE_TRY_ENV)
      == CORBA::tk_wchar)
    {
      this->current_component (ACE_TRY_ENV)->insert_wchar (value,
                                                   ACE_TRY_ENV);
      this->next (ACE_TRY_ENV);
    }
  else
    ACE_THROW (CORBA_DynAny::InvalidValue ());
}

void
TAO_DynStruct_i::insert_any (const CORBA::Any& value,
                             CORBA::Environment &ACE_TRY_ENV)
{
  if (TAO_DynAny_i::unalias (this->type_.in ()->member_type (this->current_index_),
                             ACE_TRY_ENV)
      == CORBA::tk_any)
    {
      this->current_component (ACE_TRY_ENV)->insert_any (value,
                                                 ACE_TRY_ENV);
      this->next (ACE_TRY_ENV);
    }
  else
    ACE_THROW (CORBA_DynAny::InvalidValue ());
}

// Get functions

// If the current component has not been intialized, these raise
// Invalid, which is not required by the spec, but which seems like a
// courteous thing to do.

CORBA::Boolean
TAO_DynStruct_i::get_boolean (CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::Boolean val = 0;
  CORBA_DynAny_ptr dp =
    this->da_members_[this->current_index_].in ();

  if (dp)
    {
      if (TAO_DynAny_i::unalias (dp->type (ACE_TRY_ENV),
                                 ACE_TRY_ENV)
          == CORBA::tk_boolean)
        {
          val = dp->get_boolean (ACE_TRY_ENV);
          this->next (ACE_TRY_ENV);
        }
      else
        ACE_THROW_RETURN (CORBA_DynAny::TypeMismatch (), val);
    }
  else
    ACE_THROW_RETURN (CORBA_DynAny::Invalid (), val);

  return val;
}

CORBA::Octet
TAO_DynStruct_i::get_octet (CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::Octet val = 0;
  CORBA_DynAny_ptr dp =
    this->da_members_[this->current_index_].in ();

  if (dp)
    {
      if (TAO_DynAny_i::unalias (dp->type (ACE_TRY_ENV),
                                 ACE_TRY_ENV)
          == CORBA::tk_octet)
        {
          val = dp->get_octet (ACE_TRY_ENV);
          this->next (ACE_TRY_ENV);
        }
      else
        ACE_THROW_RETURN (CORBA_DynAny::TypeMismatch (), val);
    }
  else
    ACE_THROW_RETURN (CORBA_DynAny::Invalid (), val);

  return val;
}

CORBA::Char
TAO_DynStruct_i::get_char (CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::Char val = 0;
  CORBA_DynAny_ptr dp =
    this->da_members_[this->current_index_].in ();

  if (dp)
    {
      if (TAO_DynAny_i::unalias (dp->type (ACE_TRY_ENV),
                                 ACE_TRY_ENV)
          == CORBA::tk_char)
        {
          val = dp->get_char (ACE_TRY_ENV);
          this->next (ACE_TRY_ENV);
        }
      else
        ACE_THROW_RETURN (CORBA_DynAny::TypeMismatch (), val);
    }
  else
    ACE_THROW_RETURN (CORBA_DynAny::Invalid (), val);

  return val;
}

CORBA::Short
TAO_DynStruct_i::get_short (CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::Short val = 0;
  CORBA_DynAny_ptr dp =
    this->da_members_[this->current_index_].in ();

  if (dp)
    {
      if (TAO_DynAny_i::unalias (dp->type (ACE_TRY_ENV),
                                 ACE_TRY_ENV)
          == CORBA::tk_short)
        {
          val = dp->get_short (ACE_TRY_ENV);
          this->next (ACE_TRY_ENV);
        }
      else
        ACE_THROW_RETURN (CORBA_DynAny::TypeMismatch (), val);
    }
  else
    ACE_THROW_RETURN (CORBA_DynAny::Invalid (), val);

  return val;
}

CORBA::UShort
TAO_DynStruct_i::get_ushort (CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::UShort val = 0;
  CORBA_DynAny_ptr dp =
    this->da_members_[this->current_index_].in ();

  if (dp)
    {
      if (TAO_DynAny_i::unalias (dp->type (ACE_TRY_ENV),
                                 ACE_TRY_ENV)
          == CORBA::tk_ushort)
        {
          val = dp->get_ushort (ACE_TRY_ENV);
          this->next (ACE_TRY_ENV);
        }
      else
        ACE_THROW_RETURN (CORBA_DynAny::TypeMismatch (), val);
    }
  else
    ACE_THROW_RETURN (CORBA_DynAny::Invalid (), val);

  return val;
}

CORBA::Long
TAO_DynStruct_i::get_long (CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::Long val = 0;
  CORBA_DynAny_ptr dp =
    this->da_members_[this->current_index_].in ();

  if (dp)
    {
      if (TAO_DynAny_i::unalias (dp->type (ACE_TRY_ENV),
                                 ACE_TRY_ENV)
          == CORBA::tk_long)
        {
          val = dp->get_long (ACE_TRY_ENV);
          this->next (ACE_TRY_ENV);
        }
      else
        ACE_THROW_RETURN (CORBA_DynAny::TypeMismatch (), val);
    }
  else
    ACE_THROW_RETURN (CORBA_DynAny::Invalid (), val);

  return val;
}

CORBA::ULong
TAO_DynStruct_i::get_ulong (CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::ULong val = 0;
  CORBA_DynAny_ptr dp =
    this->da_members_[this->current_index_].in ();

  if (dp)
    {
      if (TAO_DynAny_i::unalias (dp->type (ACE_TRY_ENV),
                                 ACE_TRY_ENV)
          == CORBA::tk_ulong)
        {
          val = dp->get_ulong (ACE_TRY_ENV);
          this->next (ACE_TRY_ENV);
        }
      else
        ACE_THROW_RETURN (CORBA_DynAny::TypeMismatch (), val);
    }
  else
    ACE_THROW_RETURN (CORBA_DynAny::Invalid (), val);

  return val;
}

CORBA::Float
TAO_DynStruct_i::get_float (CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::Float val = 0;
  CORBA_DynAny_ptr dp =
    this->da_members_[this->current_index_].in ();

  if (dp)
    {
      if (TAO_DynAny_i::unalias (dp->type (ACE_TRY_ENV),
                                 ACE_TRY_ENV)
          == CORBA::tk_float)
        {
          val = dp->get_float (ACE_TRY_ENV);
          this->next (ACE_TRY_ENV);
        }
      else
        ACE_THROW_RETURN (CORBA_DynAny::TypeMismatch (), val);
    }
  else
    ACE_THROW_RETURN (CORBA_DynAny::Invalid (), val);

  return val;
}

CORBA::Double
TAO_DynStruct_i::get_double (CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::Double val = 0;
  CORBA_DynAny_ptr dp =
    this->da_members_[this->current_index_].in ();

  if (dp)
    {
      if (TAO_DynAny_i::unalias (dp->type (ACE_TRY_ENV),
                                 ACE_TRY_ENV)
          == CORBA::tk_double)
        {
          val = dp->get_double (ACE_TRY_ENV);
          this->next (ACE_TRY_ENV);
        }
      else
        ACE_THROW_RETURN (CORBA_DynAny::TypeMismatch (), val);
    }
  else
    ACE_THROW_RETURN (CORBA_DynAny::Invalid (), val);

  return val;
}

char *
TAO_DynStruct_i::get_string (CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::Char *val = 0;
  CORBA_DynAny_ptr dp =
    this->da_members_[this->current_index_].in ();

  if (dp)
    {
      if (TAO_DynAny_i::unalias (dp->type (ACE_TRY_ENV),
                                 ACE_TRY_ENV)
          == CORBA::tk_string)
        {
          val = dp->get_string (ACE_TRY_ENV);
          this->next (ACE_TRY_ENV);
        }
      else
        ACE_THROW_RETURN (CORBA_DynAny::TypeMismatch (), val);
    }
  else
    ACE_THROW_RETURN (CORBA_DynAny::Invalid (), val);

  return val;
}

CORBA::Object_ptr
TAO_DynStruct_i::get_reference (CORBA::Environment &ACE_TRY_ENV)
{
  CORBA_Object_ptr val = 0;
  CORBA_DynAny_ptr dp =
    this->da_members_[this->current_index_].in ();

  if (dp)
    {
      if (TAO_DynAny_i::unalias (dp->type (ACE_TRY_ENV),
                                 ACE_TRY_ENV)
          == CORBA::tk_objref)
        {
          val = dp->get_reference (ACE_TRY_ENV);
          this->next (ACE_TRY_ENV);
        }
      else
        ACE_THROW_RETURN (CORBA_DynAny::TypeMismatch (), val);
    }
  else
    ACE_THROW_RETURN (CORBA_DynAny::Invalid (), val);

  return val;
}

CORBA::TypeCode_ptr
TAO_DynStruct_i::get_typecode (CORBA::Environment &ACE_TRY_ENV)
{
  CORBA_TypeCode_ptr val = 0;
  CORBA_DynAny_ptr dp =
    this->da_members_[this->current_index_].in ();

  if (dp)
    {
      if (TAO_DynAny_i::unalias (dp->type (ACE_TRY_ENV),
                                 ACE_TRY_ENV)
          == CORBA::tk_TypeCode)
        {
          val = dp->get_typecode (ACE_TRY_ENV);
          this->next (ACE_TRY_ENV);
        }
      else
        ACE_THROW_RETURN (CORBA_DynAny::TypeMismatch (), val);
    }
  else
    ACE_THROW_RETURN (CORBA_DynAny::Invalid (), val);

  return val;
}

CORBA::LongLong
TAO_DynStruct_i::get_longlong (CORBA::Environment &ACE_TRY_ENV)
{
#if defined (ACE_LACKS_LONGLONG_T)
  CORBA::LongLong val = {0, 0};
#else  /* ! ACE_LACKS_LONGLONG_T */
  CORBA::LongLong val = 0;
#endif /* ! ACE_LACKS_LONGLONG_T */
  CORBA_DynAny_ptr dp = this->da_members_[this->current_index_].in ();

  if (dp)
    {
      if (TAO_DynAny_i::unalias (dp->type (ACE_TRY_ENV),
                                 ACE_TRY_ENV)
        == CORBA::tk_longlong)
        {
          val = dp->get_longlong (ACE_TRY_ENV);
          this->next (ACE_TRY_ENV);
        }
      else
        ACE_THROW_RETURN (CORBA_DynAny::TypeMismatch (), val);
    }
  else
    ACE_THROW_RETURN (CORBA_DynAny::Invalid (), val);

  return val;
}

CORBA::ULongLong
TAO_DynStruct_i::get_ulonglong (CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::ULongLong val = 0;
  CORBA_DynAny_ptr dp =
    this->da_members_[this->current_index_].in ();

  if (dp)
    {
      if (TAO_DynAny_i::unalias (dp->type (ACE_TRY_ENV),
                                 ACE_TRY_ENV)
          == CORBA::tk_ulonglong)
        {
          val = dp->get_ulonglong (ACE_TRY_ENV);
          this->next (ACE_TRY_ENV);
        }
      else
        ACE_THROW_RETURN (CORBA_DynAny::TypeMismatch (), val);
    }
  else
    ACE_THROW_RETURN (CORBA_DynAny::Invalid (), val);

  return val;
}

CORBA::WChar
TAO_DynStruct_i::get_wchar (CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::WChar val = 0;
  CORBA_DynAny_ptr dp =
    this->da_members_[this->current_index_].in ();

  if (dp)
    {
      if (TAO_DynAny_i::unalias (dp->type (ACE_TRY_ENV),
                                 ACE_TRY_ENV)
          == CORBA::tk_wchar)
        {
          val = dp->get_wchar (ACE_TRY_ENV);
          this->next (ACE_TRY_ENV);
        }
      else
        ACE_THROW_RETURN (CORBA_DynAny::TypeMismatch (), val);
    }
  else
    ACE_THROW_RETURN (CORBA_DynAny::Invalid (), val);

  return val;
}

CORBA::Any_ptr
TAO_DynStruct_i::get_any (CORBA::Environment &ACE_TRY_ENV)
{
  CORBA_Any_ptr val = 0;
  CORBA_DynAny_ptr dp =
    this->da_members_[this->current_index_].in ();

  if (dp)
    {
      if (TAO_DynAny_i::unalias (dp->type (ACE_TRY_ENV),
                                 ACE_TRY_ENV)
          == CORBA::tk_any)
        {
          val = dp->get_any (ACE_TRY_ENV);
          this->next (ACE_TRY_ENV);
        }
      else
        ACE_THROW_RETURN (CORBA_DynAny::TypeMismatch (), val);
    }
  else
    ACE_THROW_RETURN (CORBA_DynAny::Invalid (), val);

  return val;
}

#endif /* TAO_HAS_MINIMUM_CORBA */
