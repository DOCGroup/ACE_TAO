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

//////////////////////////////////////////////////////////////////////
// Constructors and destructor

TAO_DynEnum_i::TAO_DynEnum_i (const CORBA_Any& any)
  : type_ (any.type ()),
    value_ (0)
{
  CORBA::Environment env;

  // The type will be correct if this constructor called from a
  // factory function, but it could also be called by the
  // user, so.....
  if (TAO_DynAny_i::unalias (this->type_.in (), env) == CORBA::tk_enum)
    {
      // Get the CDR stream of the argument.
      ACE_Message_Block* mb = any._tao_get_cdr ();

      TAO_InputCDR cdr (mb);

      cdr.read_ulong (this->value_);
    }
  else
    env.exception (new CORBA_ORB_InconsistentTypeCode);
}

TAO_DynEnum_i::TAO_DynEnum_i (CORBA_TypeCode_ptr tc)
  : type_ (CORBA::TypeCode::_duplicate (tc)),
    value_ (0)
{
  CORBA::Environment env;

  // Need to check if called by user.
  if (TAO_DynAny_i::unalias (tc,
                             env)
        != CORBA::tk_enum)
    env.exception (new CORBA_ORB_InconsistentTypeCode);
}

TAO_DynEnum_i::~TAO_DynEnum_i (void)
{
}

//////////////////////////////////////////////////////////////////////
// Functions specific to DynEnum

char*
TAO_DynEnum_i::value_as_string (CORBA::Environment& env)
{
  return CORBA::string_dup (this->type_.in ()->member_name (this->value_,
                                                            env));
}

void
TAO_DynEnum_i::value_as_string (const char* value_as_string,
                                CORBA::Environment& env)
{
  CORBA::ULong count = this->type_.in ()->member_count (env);
  CORBA::ULong i;

  for (i = 0; i < count; i++)
    if (!ACE_OS::strcmp (value_as_string,
                         this->type_.in ()->member_name (i,
                                                         env)))
      break;

  if (i < count)
    this->value_ = i;
  else
    // *** NOT part of CORBA spec ***
    env.exception (new CORBA_DynAny::InvalidValue);
}

CORBA::ULong
TAO_DynEnum_i::value_as_ulong (CORBA::Environment &)
{
  return this->value_;
}

void
TAO_DynEnum_i::value_as_ulong (CORBA::ULong value_as_ulong,
                               CORBA::Environment& env)
{
  if (value_as_ulong < this->type_.in ()->member_count (env))
    this->value_ = value_as_ulong;
  else
    // *** NOT part of CORBA spec ***
    env.exception (new CORBA_DynAny::InvalidValue);
}

//////////////////////////////////////////////////////////////////////
// Common functions

void
TAO_DynEnum_i::assign (CORBA_DynAny_ptr dyn_any,
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
TAO_DynEnum_i::copy (CORBA::Environment &env)
{
  CORBA_Any_ptr a = this->to_any (env);
  return TAO_DynAny_i::create_dyn_any (*a,
                                       env);
}

void
TAO_DynEnum_i::destroy (CORBA::Environment &)
{
  // Freeing the top level is all we have to do.
  delete this;
}

void
TAO_DynEnum_i::from_any (const CORBA_Any& any,
                         CORBA::Environment &env)
{
  if (TAO_DynAny_i::unalias (any.type (), env) == CORBA::tk_enum)
    {
      // Get the CDR stream of the argument.
      ACE_Message_Block* mb = any._tao_get_cdr ();

      TAO_InputCDR cdr (mb);

      cdr.read_ulong (this->value_);
    }
  else
    env.exception (new CORBA_DynAny::Invalid);
}

CORBA::Any_ptr
TAO_DynEnum_i::to_any (CORBA::Environment& ACE_TRY_ENV)
{
  TAO_OutputCDR out_cdr;

  out_cdr.write_ulong (this->value_);

  CORBA_Any* retval;
  ACE_NEW_THROW_EX (retval,
                    CORBA_Any (this->type_.in (),
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
TAO_DynEnum_i::current_component (CORBA::Environment &env)
{
  return this->_this (env);
}

CORBA::Boolean
TAO_DynEnum_i::next (CORBA::Environment &)
{
  return 0;
}

CORBA::Boolean
TAO_DynEnum_i::seek (CORBA::Long index,
                     CORBA::Environment &)
{
  if (index == 0)
    return 1;
  else
    return 0;
}

// Nothing to do here.
void
TAO_DynEnum_i::rewind (CORBA::Environment &)
{
}

/////////////////////////////////////////////////////////////////
// Insert and get functions
//
// These have no meaning for the enum type, but someone could
// try to call one, so they are implemented here to throw an
// enlightening exception.

void
TAO_DynEnum_i::insert_boolean (CORBA::Boolean,
                               CORBA::Environment &env)
{
  env.exception (new CORBA::BAD_OPERATION (CORBA::COMPLETED_NO));
}

void
TAO_DynEnum_i::insert_octet (CORBA::Octet,
                             CORBA::Environment &env)
{
  env.exception (new CORBA::BAD_OPERATION (CORBA::COMPLETED_NO));
}

void
TAO_DynEnum_i::insert_char (CORBA::Char,
                            CORBA::Environment &env)
{
  env.exception (new CORBA::BAD_OPERATION (CORBA::COMPLETED_NO));
}

void
TAO_DynEnum_i::insert_short (CORBA::Short,
                             CORBA::Environment &env)
{
  env.exception (new CORBA::BAD_OPERATION (CORBA::COMPLETED_NO));
}

void
TAO_DynEnum_i::insert_ushort (CORBA::UShort,
                              CORBA::Environment &env)
{
  env.exception (new CORBA::BAD_OPERATION (CORBA::COMPLETED_NO));
}

void
TAO_DynEnum_i::insert_long (CORBA::Long,
                            CORBA::Environment &env)
{
  env.exception (new CORBA::BAD_OPERATION (CORBA::COMPLETED_NO));
}

void
TAO_DynEnum_i::insert_ulong (CORBA::ULong,
                             CORBA::Environment &env)
{
  env.exception (new CORBA::BAD_OPERATION (CORBA::COMPLETED_NO));
}

void
TAO_DynEnum_i::insert_float (CORBA::Float,
                             CORBA::Environment &env)
{
  env.exception (new CORBA::BAD_OPERATION (CORBA::COMPLETED_NO));
}

void
TAO_DynEnum_i::insert_double (CORBA::Double,
                              CORBA::Environment &env)
{
  env.exception (new CORBA::BAD_OPERATION (CORBA::COMPLETED_NO));
}

void
TAO_DynEnum_i::insert_string (const char *,
                              CORBA::Environment &env)
{
  env.exception (new CORBA::BAD_OPERATION (CORBA::COMPLETED_NO));
}

void
TAO_DynEnum_i::insert_reference (CORBA::Object_ptr,
                                 CORBA::Environment &env)
{
  env.exception (new CORBA::BAD_OPERATION (CORBA::COMPLETED_NO));
}

void
TAO_DynEnum_i::insert_typecode (CORBA::TypeCode_ptr,
                                CORBA::Environment &env)
{
  env.exception (new CORBA::BAD_OPERATION (CORBA::COMPLETED_NO));
}

void
TAO_DynEnum_i::insert_longlong (CORBA::LongLong,
                               CORBA::Environment &env)
{
  env.exception (new CORBA::BAD_OPERATION (CORBA::COMPLETED_NO));
}

void
TAO_DynEnum_i::insert_ulonglong (CORBA::ULongLong,
                                 CORBA::Environment &env)
{
  env.exception (new CORBA::BAD_OPERATION (CORBA::COMPLETED_NO));
}

void
TAO_DynEnum_i::insert_wchar (CORBA::WChar,
                             CORBA::Environment &env)
{
  env.exception (new CORBA::BAD_OPERATION (CORBA::COMPLETED_NO));
}

void
TAO_DynEnum_i::insert_any (const CORBA::Any&,
                           CORBA::Environment &env)
{
  env.exception (new CORBA::BAD_OPERATION (CORBA::COMPLETED_NO));
}

CORBA::Boolean
TAO_DynEnum_i::get_boolean (CORBA::Environment &env)
{
  env.exception (new CORBA::BAD_OPERATION (CORBA::COMPLETED_NO));
  return 0;
}

CORBA::Octet
TAO_DynEnum_i::get_octet (CORBA::Environment &env)
{
  env.exception (new CORBA::BAD_OPERATION (CORBA::COMPLETED_NO));
  return 0;
}

CORBA::Char
TAO_DynEnum_i::get_char (CORBA::Environment &env)
{
  env.exception (new CORBA::BAD_OPERATION (CORBA::COMPLETED_NO));
  return 0;
}

CORBA::Short
TAO_DynEnum_i::get_short (CORBA::Environment &env)
{
  env.exception (new CORBA::BAD_OPERATION (CORBA::COMPLETED_NO));
  return 0;
}

CORBA::UShort
TAO_DynEnum_i::get_ushort (CORBA::Environment &env)
{
  env.exception (new CORBA::BAD_OPERATION (CORBA::COMPLETED_NO));
  return 0;
}

CORBA::Long
TAO_DynEnum_i::get_long (CORBA::Environment &env)
{
  env.exception (new CORBA::BAD_OPERATION (CORBA::COMPLETED_NO));
  return 0;
}

CORBA::ULong
TAO_DynEnum_i::get_ulong (CORBA::Environment &env)
{
  env.exception (new CORBA::BAD_OPERATION (CORBA::COMPLETED_NO));
  return 0;
}

CORBA::Float
TAO_DynEnum_i::get_float (CORBA::Environment &env)
{
  env.exception (new CORBA::BAD_OPERATION (CORBA::COMPLETED_NO));
  return 0;
}

CORBA::Double
TAO_DynEnum_i::get_double (CORBA::Environment &env)
{
  env.exception (new CORBA::BAD_OPERATION (CORBA::COMPLETED_NO));
  return 0;
}

char *
TAO_DynEnum_i::get_string (CORBA::Environment &env)
{
  env.exception (new CORBA::BAD_OPERATION (CORBA::COMPLETED_NO));
  return 0;
}

CORBA::Object_ptr
TAO_DynEnum_i::get_reference (CORBA::Environment &env)
{
  env.exception (new CORBA::BAD_OPERATION (CORBA::COMPLETED_NO));
  return 0;
}

CORBA::TypeCode_ptr
TAO_DynEnum_i::get_typecode (CORBA::Environment &env)
{
  env.exception (new CORBA::BAD_OPERATION (CORBA::COMPLETED_NO));
  return 0;
}

CORBA::LongLong
TAO_DynEnum_i::get_longlong (CORBA::Environment &env)
{
  env.exception (new CORBA::BAD_OPERATION (CORBA::COMPLETED_NO));

#if defined (ACE_LACKS_LONGLONG_T)
  CORBA::LongLong tmp = {0, 0};
  return tmp;
#else  /* ! ACE_LACKS_LONGLONG_T */
  return 0;
#endif /* ! ACE_LACKS_LONGLONG_T */
}

CORBA::ULongLong
TAO_DynEnum_i::get_ulonglong (CORBA::Environment &env)
{
  env.exception (new CORBA::BAD_OPERATION (CORBA::COMPLETED_NO));
  return 0;
}

CORBA::WChar
TAO_DynEnum_i::get_wchar (CORBA::Environment &env)
{
  env.exception (new CORBA::BAD_OPERATION (CORBA::COMPLETED_NO));
  return 0;
}

CORBA::Any_ptr
TAO_DynEnum_i::get_any (CORBA::Environment& env)
{
  env.exception (new CORBA::BAD_OPERATION (CORBA::COMPLETED_NO));
  return 0;
}

#endif /* TAO_HAS_MINIMUM_CORBA */
