/* -*- C++ -*- */
// $Id$
// ===================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    DynEnum_i.cpp
//
// = AUTHOR
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ===================================================================

#include "tao/DynAny_i.h"

#if !defined (TAO_HAS_MINIMUM_CORBA)

#include "tao/DynEnum_i.h"
#include "tao/InconsistentTypeCodeC.h"

// Constructors and destructor

TAO_DynEnum_i::TAO_DynEnum_i (const CORBA_Any &any)
  : type_ (any.type ()),
    value_ (0)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // The type will be correct if this constructor called from a
      // factory function, but it could also be called by the user,
      // so.....
      int tk =
        TAO_DynAny_i::unalias (this->type_.in (),
                               ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (tk == CORBA::tk_enum)
        {
          // Get the CDR stream of the argument.
          ACE_Message_Block* mb = any._tao_get_cdr ();

          TAO_InputCDR cdr (mb);

          cdr.read_ulong (this->value_);
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

TAO_DynEnum_i::TAO_DynEnum_i (CORBA_TypeCode_ptr tc)
  : type_ (CORBA::TypeCode::_duplicate (tc)),
    value_ (0)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // Need to check if called by user.
      int tk = TAO_DynAny_i::unalias (tc,
                                      ACE_TRY_ENV);
      ACE_TRY_CHECK;
      if (tk != CORBA::tk_enum)
        ACE_THROW (CORBA_ORB_InconsistentTypeCode ());
    }
  ACE_CATCHANY
    {
      // do nothing
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

TAO_DynEnum_i::~TAO_DynEnum_i (void)
{
}

// Functions specific to DynEnum

char *
TAO_DynEnum_i::value_as_string (CORBA::Environment& ACE_TRY_ENV)
{
  const char *retval = this->type_.in ()->member_name (this->value_,
                                                       ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  return CORBA::string_dup (retval);
}

void
TAO_DynEnum_i::value_as_string (const char *value_as_string,
                                CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::ULong count = this->type_.in ()->member_count (ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::ULong i;

  for (i = 0; i < count; i++)
    {
      const char *temp = this->type_.in ()->member_name (i,
                                                         ACE_TRY_ENV);
      ACE_CHECK;

      if (!ACE_OS::strcmp (value_as_string,
                           temp))
        {
          break;
        }
    }

  if (i < count)
    {
      this->value_ = i;
    }
  else
    {
      // *** NOT part of CORBA spec ***
      ACE_THROW (CORBA_DynAny::InvalidValue ());
    }
}

CORBA::ULong
TAO_DynEnum_i::value_as_ulong (CORBA::Environment &)
{
  return this->value_;
}

void
TAO_DynEnum_i::value_as_ulong (CORBA::ULong value_as_ulong,
                               CORBA::Environment& ACE_TRY_ENV)
{
  CORBA::ULong max = this->type_.in ()->member_count (ACE_TRY_ENV);
  ACE_CHECK;

  if (value_as_ulong < max)
    {
      this->value_ = value_as_ulong;
    }
  else
    {
      // *** NOT part of CORBA spec ***
      ACE_THROW (CORBA_DynAny::InvalidValue ());
    }
}

// Common functions

void
TAO_DynEnum_i::assign (CORBA_DynAny_ptr dyn_any,
                       CORBA::Environment &ACE_TRY_ENV)
{
  // *dyn_any->to_any raises Invalid if arg is bad.
  CORBA_TypeCode_ptr tc = dyn_any->type (ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::Boolean equal = this->type_.in ()->equal (tc,
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
TAO_DynEnum_i::copy (CORBA::Environment &ACE_TRY_ENV)
{
  CORBA_Any_ptr a = this->to_any (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA_DynAny::_nil ());

  CORBA_DynAny_ptr dp = TAO_DynAny_i::create_dyn_any (*a,
                                                      ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA_DynAny::_nil ());

  return dp;
}

void
TAO_DynEnum_i::destroy (CORBA::Environment &)
{
  // Freeing the top level is all we have to do.
  delete this;
}

void
TAO_DynEnum_i::from_any (const CORBA_Any& any,
                         CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::TCKind kind = TAO_DynAny_i::unalias (any.type (),
                                              ACE_TRY_ENV);
  ACE_CHECK;

  if (kind == CORBA::tk_enum)
    {
      // Get the CDR stream of the argument.
      ACE_Message_Block* mb = any._tao_get_cdr ();

      TAO_InputCDR cdr (mb);

      cdr.read_ulong (this->value_);
    }
  else
    {
      ACE_THROW (CORBA_DynAny::Invalid ());
    }
}

CORBA::Any_ptr
TAO_DynEnum_i::to_any (CORBA::Environment& ACE_TRY_ENV)
{
  TAO_OutputCDR out_cdr;

  out_cdr.write_ulong (this->value_);

  CORBA_Any *retval;
  ACE_NEW_THROW_EX (retval,
                    CORBA_Any (this->type_.in (),
                               0,
                               out_cdr.begin ()),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);
  return retval;
}

CORBA::TypeCode_ptr
TAO_DynEnum_i::type (CORBA::Environment &)
{
  return this->type_.in ();
}

CORBA_DynAny_ptr
TAO_DynEnum_i::current_component (CORBA::Environment &ACE_TRY_ENV)
{
  CORBA_DynAny_ptr retval = this->_this (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA_DynAny::_nil ());

  return retval;
}

CORBA::Boolean
TAO_DynEnum_i::next (CORBA::Environment &)
{
  return 0;
}

CORBA::Boolean
TAO_DynEnum_i::seek (CORBA::Long slot,
                     CORBA::Environment &)
{
  if (slot == 0)
    return 1;
  else
    return 0;
}

// Nothing to do here.
void
TAO_DynEnum_i::rewind (CORBA::Environment &)
{
}

// Insert and get functions
//
// These have no meaning for the enum type, but someone could
// try to call one, so they are implemented here to throw an
// enlightening exception.

void
TAO_DynEnum_i::insert_boolean (CORBA::Boolean,
                               CORBA::Environment &ACE_TRY_ENV)
{
  ACE_THROW (CORBA::BAD_OPERATION ());
}

void
TAO_DynEnum_i::insert_octet (CORBA::Octet,
                             CORBA::Environment &ACE_TRY_ENV)
{
  ACE_THROW (CORBA::BAD_OPERATION ());
}

void
TAO_DynEnum_i::insert_char (CORBA::Char,
                            CORBA::Environment &ACE_TRY_ENV)
{
  ACE_THROW (CORBA::BAD_OPERATION ());
}

void
TAO_DynEnum_i::insert_short (CORBA::Short,
                             CORBA::Environment &ACE_TRY_ENV)
{
  ACE_THROW (CORBA::BAD_OPERATION ());
}

void
TAO_DynEnum_i::insert_ushort (CORBA::UShort,
                              CORBA::Environment &ACE_TRY_ENV)
{
  ACE_THROW (CORBA::BAD_OPERATION ());
}

void
TAO_DynEnum_i::insert_long (CORBA::Long,
                            CORBA::Environment &ACE_TRY_ENV)
{
  ACE_THROW (CORBA::BAD_OPERATION ());
}

void
TAO_DynEnum_i::insert_ulong (CORBA::ULong,
                             CORBA::Environment &ACE_TRY_ENV)
{
  ACE_THROW (CORBA::BAD_OPERATION ());
}

void
TAO_DynEnum_i::insert_float (CORBA::Float,
                             CORBA::Environment &ACE_TRY_ENV)
{
  ACE_THROW (CORBA::BAD_OPERATION ());
}

void
TAO_DynEnum_i::insert_double (CORBA::Double,
                              CORBA::Environment &ACE_TRY_ENV)
{
  ACE_THROW (CORBA::BAD_OPERATION ());
}

void
TAO_DynEnum_i::insert_string (const char *,
                              CORBA::Environment &ACE_TRY_ENV)
{
  ACE_THROW (CORBA::BAD_OPERATION ());
}

void
TAO_DynEnum_i::insert_reference (CORBA::Object_ptr,
                                 CORBA::Environment &ACE_TRY_ENV)
{
  ACE_THROW (CORBA::BAD_OPERATION ());
}

void
TAO_DynEnum_i::insert_typecode (CORBA::TypeCode_ptr,
                                CORBA::Environment &ACE_TRY_ENV)
{
  ACE_THROW (CORBA::BAD_OPERATION ());
}

void
TAO_DynEnum_i::insert_longlong (CORBA::LongLong,
                               CORBA::Environment &ACE_TRY_ENV)
{
  ACE_THROW (CORBA::BAD_OPERATION ());
}

void
TAO_DynEnum_i::insert_ulonglong (CORBA::ULongLong,
                                 CORBA::Environment &ACE_TRY_ENV)
{
  ACE_THROW (CORBA::BAD_OPERATION ());
}

void
TAO_DynEnum_i::insert_wchar (CORBA::WChar,
                             CORBA::Environment &ACE_TRY_ENV)
{
  ACE_THROW (CORBA::BAD_OPERATION ());
}

void
TAO_DynEnum_i::insert_any (const CORBA::Any&,
                           CORBA::Environment &ACE_TRY_ENV)
{
  ACE_THROW (CORBA::BAD_OPERATION ());
}

CORBA::Boolean
TAO_DynEnum_i::get_boolean (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_THROW_RETURN (CORBA::BAD_OPERATION (), 0);
}

CORBA::Octet
TAO_DynEnum_i::get_octet (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_THROW_RETURN (CORBA::BAD_OPERATION (), 0);
}

CORBA::Char
TAO_DynEnum_i::get_char (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_THROW_RETURN (CORBA::BAD_OPERATION (), 0);
}

CORBA::Short
TAO_DynEnum_i::get_short (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_THROW_RETURN (CORBA::BAD_OPERATION (), 0);
}

CORBA::UShort
TAO_DynEnum_i::get_ushort (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_THROW_RETURN (CORBA::BAD_OPERATION (), 0);
}

CORBA::Long
TAO_DynEnum_i::get_long (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_THROW_RETURN (CORBA::BAD_OPERATION (), 0);
}

CORBA::ULong
TAO_DynEnum_i::get_ulong (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_THROW_RETURN (CORBA::BAD_OPERATION (), 0);
}

CORBA::Float
TAO_DynEnum_i::get_float (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_THROW_RETURN (CORBA::BAD_OPERATION (), 0);
}

CORBA::Double
TAO_DynEnum_i::get_double (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_THROW_RETURN (CORBA::BAD_OPERATION (), 0);
}

char *
TAO_DynEnum_i::get_string (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_THROW_RETURN (CORBA::BAD_OPERATION (), 0);
}

CORBA::Object_ptr
TAO_DynEnum_i::get_reference (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_THROW_RETURN (CORBA::BAD_OPERATION (), 0);
}

CORBA::TypeCode_ptr
TAO_DynEnum_i::get_typecode (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_THROW_RETURN (CORBA::BAD_OPERATION (), 0);
}

CORBA::LongLong
TAO_DynEnum_i::get_longlong (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_THROW_RETURN (CORBA::BAD_OPERATION (), tmp);
}

CORBA::ULongLong
TAO_DynEnum_i::get_ulonglong (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_THROW_RETURN (CORBA::BAD_OPERATION (), 0);
}

CORBA::WChar
TAO_DynEnum_i::get_wchar (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_THROW_RETURN (CORBA::BAD_OPERATION (), 0);
}

CORBA::Any_ptr
TAO_DynEnum_i::get_any (CORBA::Environment& ACE_TRY_ENV)
{
  ACE_THROW_RETURN (CORBA::BAD_OPERATION (), 0);
}

#endif /* TAO_HAS_MINIMUM_CORBA */
