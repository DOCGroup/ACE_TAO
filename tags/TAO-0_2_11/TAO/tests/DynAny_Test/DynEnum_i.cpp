// $Id$

// ===================================================================
//
// = LIBRARY
//    TAO/tests/DynAny_Test
//
// = FILENAME
//    DynEnum_i.cpp
//
// = AUTHOR
//    Jeff Parsons <jp4@cs.wustl.edu>
//
// ===================================================================

#include "DynAny_i.h"
#include "DynEnum_i.h"

//////////////////////////////////////////////////////////////////////
// Constructors and destructor

DynEnum_i::DynEnum_i (const CORBA_Any& any)
  : type_ (CORBA::TypeCode::_duplicate (any.type ()))
{
  CORBA::Environment env;

  // The type will be correct if this constructor called from a 
  // factory function, but it could also be called by the 
  // user, so..... 
  if (DynAny_i::unalias (this->type_.in (), env) == CORBA::tk_enum)
    {
      // Get the CDR stream of the argument.
      ACE_Message_Block* mb =
        ACE_Message_Block::duplicate (any._tao_get_cdr ());

      TAO_InputCDR cdr (mb);

      cdr.read_ulong (this->value_);
    }
  else
    env.exception (new DynAny_i::InconsistentTypeCode);
}

DynEnum_i::DynEnum_i (CORBA_TypeCode_ptr tc)
  : type_ (CORBA::TypeCode::_duplicate (tc)),
    value_ (0)
{
  CORBA::Environment env;

  // Need to check if called by user.
  if (DynAny_i::unalias (tc, env) != CORBA::tk_enum)
    env.exception (new DynAny_i::InconsistentTypeCode);
}

DynEnum_i::~DynEnum_i (void)
{
}

//////////////////////////////////////////////////////////////////////
// Functions specific to DynEnum

char* 
DynEnum_i::value_as_string (CORBA::Environment& env)
{
  return CORBA::string_dup (this->type_.in ()->member_name (this->value_, env));
}

void 
DynEnum_i::value_as_string (const char* value_as_string,
                            CORBA::Environment& env)
{
  CORBA::ULong count = this->type_.in ()->member_count (env);

  for (CORBA::ULong i = 0; i < count; i++)
    if (!ACE_OS::strcmp (value_as_string, this->type_.in ()->member_name (i, env)))
      break;

  if (i < count)
    this->value_ = i;
  else
    // *** NOT part of CORBA spec ***
    env.exception (new DynAny::InvalidValue);
}

CORBA::ULong 
DynEnum_i::value_as_ulong (CORBA::Environment&)
{
  return this->value_;
}

void 
DynEnum_i::value_as_ulong (CORBA::ULong value_as_ulong,
                           CORBA::Environment& env)
{
  if (value_as_ulong < this->type_.in ()->member_count (env))
    this->value_ = value_as_ulong;
  else
    // *** NOT part of CORBA spec ***
    env.exception (new DynAny::InvalidValue);
}

//////////////////////////////////////////////////////////////////////
// Common functions

void
DynEnum_i::assign (DynAny_ptr dyn_any, CORBA::Environment &env)
{
  // *dyn_any->to_any raises Invalid if arg is bad.
  if (this->type_.in ()->equal (dyn_any->type (env), env))
    this->from_any (*dyn_any->to_any (env), env);
  else
    env.exception (new DynAny::Invalid);
}

DynAny_ptr
DynEnum_i::copy (CORBA::Environment &env)
{
  CORBA_Any_ptr a = this->to_any (env);
  return DynAny_i::create_dyn_any (*a, env);
}

void
DynEnum_i::destroy (CORBA::Environment &env)
{
  // Freeing the top level is all we have to do.
  CORBA::release (this->_this (env));
}

void
DynEnum_i::from_any (const CORBA_Any& any, CORBA::Environment &env)
{
  if (DynAny_i::unalias (any.type (), env) == CORBA::tk_enum)
    {
      // Get the CDR stream of the argument.
      ACE_Message_Block* mb =
        ACE_Message_Block::duplicate (any._tao_get_cdr ());

      TAO_InputCDR cdr (mb);

      cdr.read_ulong (this->value_);
    }
  else
    env.exception (new DynAny::Invalid);
}

CORBA::Any_ptr
DynEnum_i::to_any (CORBA::Environment& env)
{
  TAO_OutputCDR out_cdr;

  out_cdr.write_ulong (this->value_);

  return new CORBA_Any (this->type_.in (),
                        out_cdr.begin ());
}

CORBA::TypeCode_ptr
DynEnum_i::type (CORBA::Environment &)
{
  return this->type_.in ();
}

DynAny_ptr
DynEnum_i::current_component (CORBA::Environment &env)
{
  return this->_this (env);
}

CORBA::Boolean
DynEnum_i::next (CORBA::Environment &)
{
  return 0;
}

CORBA::Boolean
DynEnum_i::seek (CORBA::Long index, CORBA::Environment &)
{
  if (index == 0)
    return 1;
  else
    return 0;
}

// Nothing to do here.
void
DynEnum_i::rewind (CORBA::Environment &)
{
}


