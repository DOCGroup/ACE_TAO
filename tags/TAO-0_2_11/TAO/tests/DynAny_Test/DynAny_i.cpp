// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/DynAny_Test
//
// = FILENAME
//    DynAny_i.cpp
//
// = AUTHOR
//    Jeff Parsons <jp4@cs.wustl.edu>
//
// ============================================================================

#include "tao/corba.h"
#include "DynAny_i.h"
#include "DynStruct_i.h"
#include "DynSequence_i.h"
#include "DynEnum_i.h"
#include "DynArray_i.h"
#include "DynUnion_i.h"

// Constructor from typecode
DynAny_i::DynAny_i (CORBA_TypeCode_ptr tc)
  : value_ (CORBA::TypeCode::_duplicate (tc))
{
  CORBA::Environment env;
  // Check to see if it's a simple type.
  switch (unalias (tc, env))
    {
      case CORBA::tk_null:
      case CORBA::tk_void:
      case CORBA::tk_short:
      case CORBA::tk_long: 
      case CORBA::tk_ushort:
      case CORBA::tk_ulong:
      case CORBA::tk_float:
      case CORBA::tk_double: 
      case CORBA::tk_longlong:
      case CORBA::tk_ulonglong:
      case CORBA::tk_boolean:
      case CORBA::tk_char:
      case CORBA::tk_wchar:
      case CORBA::tk_octet:
      case CORBA::tk_any:
      case CORBA::tk_TypeCode:
      case CORBA::tk_objref:
      case CORBA::tk_string:
        break;
      default:
        env.exception (new DynAny_i::InconsistentTypeCode);
        break;
    }
}

// Constructor from Any
DynAny_i::DynAny_i (const CORBA_Any& any)
  : value_ (any)
{
  // Check to see if it's a simple type.
  CORBA::Environment env;
  switch (unalias (any.type (), env))
    {
      case CORBA::tk_null:
      case CORBA::tk_void:
      case CORBA::tk_short:
      case CORBA::tk_long: 
      case CORBA::tk_ushort:
      case CORBA::tk_ulong:
      case CORBA::tk_float:
      case CORBA::tk_double: 
      case CORBA::tk_longlong:
      case CORBA::tk_ulonglong:
      case CORBA::tk_boolean:
      case CORBA::tk_char:
      case CORBA::tk_wchar:
      case CORBA::tk_octet:
      case CORBA::tk_any:
      case CORBA::tk_TypeCode:
      case CORBA::tk_objref:
      case CORBA::tk_string:
        break;
      default:
        env.exception (new DynAny::Invalid);
        break;
    }
}

// Destructor
DynAny_i::~DynAny_i (void)
{
}

CORBA::TypeCode_ptr
DynAny_i::type (CORBA::Environment &)
{
  return CORBA_TypeCode::_duplicate (this->value_.type ());
}

void
DynAny_i::assign (DynAny_ptr dyn_any, CORBA::Environment &env)
{
  if (this->value_.type ()->equal (dyn_any->type (env), env))
    this->value_ = *dyn_any->to_any (env);
  else
    env.exception (new DynAny::Invalid);
}

DynAny_ptr
DynAny_i::copy (CORBA::Environment &env)
{
  DynAny_i* tmp = new DynAny_i (this->value_);
  return tmp->_this (env);
}

void
DynAny_i::destroy (CORBA::Environment &env)
{
  CORBA::release (this->_this (env));
}

// We'll allow one (possibly different) objref to replace another
// for now, but maybe we don't want to do this.
void
DynAny_i::from_any (const CORBA_Any& any, CORBA::Environment &env)
{
  if (!this->value_.type ()->equal (any.type (), env) 
      || any.value () == 0 /* 
      || this->type_.in ()->kind (env) == CORBA::tk_objref 
      || tc->kind (env) == CORBA::tk_objref */)
    {
      env.exception (new DynAny::Invalid);
    }

  this->value_ = any;
}

CORBA::Any_ptr
DynAny_i::to_any (CORBA::Environment &env)
{
  if (this->value_.value ())
    return new CORBA::Any (this->value_);
  else
    {
      env.exception (new DynAny::Invalid);
      return 0;
    }
}

DynAny_ptr
DynAny_i::current_component (CORBA::Environment &env)
{
  return this->_this (env);
}

CORBA::Boolean
DynAny_i::next (CORBA::Environment &)
{
  return 0;
}

CORBA::Boolean
DynAny_i::seek (CORBA::Long index,
                CORBA::Environment &)
{
  if (index == 0)
    return 1;
  else
    return 0;
}

// Nothing to rewind in this class.
void
DynAny_i::rewind (CORBA::Environment &)
{
}

//****************INSERT FUNCTIONS****************************

void
DynAny_i::insert_boolean (CORBA::Boolean value, CORBA::Environment &env)
{
  if (unalias (this->value_.type (), env) == CORBA::tk_boolean)
    this->value_ <<= CORBA::Any::from_boolean (value);
  else
    env.exception (new DynAny::InvalidValue);
}

void
DynAny_i::insert_octet (CORBA::Octet value, CORBA::Environment &env)
{
  if (this->value_.type ()->kind (env) == CORBA::tk_octet)
    this->value_ <<= CORBA::Any::from_octet (value);
  else
    env.exception (new DynAny::InvalidValue);
}

void
DynAny_i::insert_char (CORBA::Char value, CORBA::Environment &env)
{
  if (this->value_.type ()->kind (env) == CORBA::tk_char)
    this->value_ <<= CORBA::Any::from_char (value);
  else
    env.exception (new DynAny::InvalidValue);
}

void
DynAny_i::insert_short (CORBA::Short value, CORBA::Environment &env)
{
  if (this->value_.type ()->kind (env) == CORBA::tk_short)
    this->value_ <<= value;
  else
    env.exception (new DynAny::InvalidValue);
}

void
DynAny_i::insert_ushort (CORBA::UShort value, CORBA::Environment &env)
{
  if (this->value_.type ()->kind (env) == CORBA::tk_ushort)
    this->value_ <<= value;
  else
    env.exception (new DynAny::InvalidValue);
}

void
DynAny_i::insert_long (CORBA::Long value, CORBA::Environment &env)
{
  if (this->value_.type ()->kind (env) == CORBA::tk_long)
    this->value_ <<= value;
  else
    env.exception (new DynAny::InvalidValue);
}

void
DynAny_i::insert_ulong (CORBA::ULong value, CORBA::Environment &env)
{
  if (this->value_.type ()->kind (env) == CORBA::tk_ulong)
    this->value_ <<= value;
  else
    env.exception (new DynAny::InvalidValue);
}

void
DynAny_i::insert_float (CORBA::Float value, CORBA::Environment &env)
{
  if (this->value_.type ()->kind (env) == CORBA::tk_float)
    this->value_ <<= value;
  else
    env.exception (new DynAny::InvalidValue);
}

void
DynAny_i::insert_double (CORBA::Double value, CORBA::Environment &env)
{
  if (this->value_.type ()->kind (env) == CORBA::tk_double)
    this->value_ <<= value;
  else
    env.exception (new DynAny::InvalidValue);
}

void
DynAny_i::insert_string (char * value, CORBA::Environment &env)
{
  if (this->value_.type ()->kind (env) == CORBA::tk_string)
    this->value_ <<= CORBA::Any::from_string (value, 0);
  else
    env.exception (new DynAny::InvalidValue);
}

void
DynAny_i::insert_reference (CORBA::Object_ptr value, CORBA::Environment &env)
{
  if (this->value_.type ()->kind (env) == CORBA::tk_objref)
    this->value_ <<= value;
  else
    env.exception (new DynAny::InvalidValue);
}

void
DynAny_i::insert_typecode (CORBA::TypeCode_ptr value, CORBA::Environment &env)
{
  if (this->value_.type ()->kind (env) == CORBA::tk_TypeCode)
    this->value_ <<= value;
  else
    env.exception (new DynAny::InvalidValue);
}

void
DynAny_i::insert_longlong (CORBA::LongLong value, CORBA::Environment &env)
{
  if (this->value_.type ()->kind (env) == CORBA::tk_longlong)
    this->value_ <<= value;
  else
    env.exception (new DynAny::InvalidValue);
}

void
DynAny_i::insert_ulonglong (CORBA::ULongLong value, CORBA::Environment &env)
{
  if (this->value_.type ()->kind (env) == CORBA::tk_ulonglong)
    this->value_ <<= value;
  else
    env.exception (new DynAny::InvalidValue);
}

void
DynAny_i::insert_wchar (CORBA::WChar value, CORBA::Environment &env)
{
  if (this->value_.type ()->kind (env) == CORBA::tk_wchar)
    this->value_ <<= CORBA::Any::from_wchar (value);
  else
    env.exception (new DynAny::InvalidValue);
}

void
DynAny_i::insert_any (CORBA::Any& value, CORBA::Environment &env)
{
  if (this->value_.type ()->kind (env) == CORBA::tk_any)
    this->value_ <<= value;
  else
    env.exception (new DynAny::InvalidValue);
}

//*************************GET FUNCTIONS**********************

CORBA::Boolean
DynAny_i::get_boolean (CORBA::Environment &env)
{
  CORBA::Boolean val;

  if (!(this->value_ >>= CORBA_Any::to_boolean (val)))
    env.exception (new DynAny::TypeMismatch);
  
  return val;
}

CORBA::Octet
DynAny_i::get_octet (CORBA::Environment &env)
{
  CORBA::Octet val;

  if (!(this->value_ >>= CORBA_Any::to_octet (val)))
    env.exception (new DynAny::TypeMismatch);

  return val;
}

CORBA::Char
DynAny_i::get_char (CORBA::Environment &env)
{
  CORBA::Char val;

  if (!(this->value_ >>= CORBA_Any::to_char (val)))
    env.exception (new DynAny::TypeMismatch);

  return val;
}

CORBA::Short
DynAny_i::get_short (CORBA::Environment &env)
{
  CORBA::Short val;

  if (!(this->value_ >>= val))
    env.exception (new DynAny::TypeMismatch);

  return val;
}

CORBA::UShort
DynAny_i::get_ushort (CORBA::Environment &env)
{
  CORBA::UShort val;

  if (!(this->value_ >>= val))
    env.exception (new DynAny::TypeMismatch);

  return val;
}

CORBA::Long
DynAny_i::get_long (CORBA::Environment &env)
{
  CORBA::Long val;

  if (!(this->value_ >>= val))
    env.exception (new DynAny::TypeMismatch);

  return val;
}

CORBA::ULong
DynAny_i::get_ulong (CORBA::Environment &env)
{
  CORBA::ULong val;

  if (!(this->value_ >>= val))
    env.exception (new DynAny::TypeMismatch);

  return val;
}

CORBA::Float
DynAny_i::get_float (CORBA::Environment &env)
{
  CORBA::Float val;

  if (!(this->value_ >>= val))
    env.exception (new DynAny::TypeMismatch);

  return val;
}

CORBA::Double
DynAny_i::get_double (CORBA::Environment &env)
{
  CORBA::Double val;

  if (!(this->value_ >>= val))
    env.exception (new DynAny::TypeMismatch);

  return val;
}

char *
DynAny_i::get_string (CORBA::Environment &env)
{
  CORBA::Char *val;
  CORBA::Any::to_string ts (val, 0);

  if (!(this->value_ >>= ts))
    env.exception (new DynAny::TypeMismatch);

  return ts.val_;
}

CORBA::Object_ptr
DynAny_i::get_reference (CORBA::Environment &env)
{
  CORBA_Object_ptr val;
  CORBA_Any_var a = this->to_any (env);

  if (!(this->value_ >>= CORBA_Any::to_object (val)))
    env.exception (new DynAny::TypeMismatch);

  return val;
}

CORBA::TypeCode_ptr
DynAny_i::get_typecode (CORBA::Environment &env)
{
  CORBA_TypeCode_ptr val;

  if (!(this->value_ >>= val))
    env.exception (new DynAny::TypeMismatch);

  return val;
}

CORBA::LongLong
DynAny_i::get_longlong (CORBA::Environment &env)
{
  CORBA::LongLong val;

  if (!(this->value_ >>= val))
    env.exception (new DynAny::TypeMismatch);

  return val;
}

CORBA::ULongLong
DynAny_i::get_ulonglong (CORBA::Environment &env)
{
  CORBA::ULongLong val;

  if (!(this->value_ >>= val))
    env.exception (new DynAny::TypeMismatch);

  return val;
}

CORBA::WChar
DynAny_i::get_wchar (CORBA::Environment &env)
{
  CORBA::WChar val;

  if (!(this->value_ >>= CORBA_Any::to_wchar (val)))
    env.exception (new DynAny::TypeMismatch);

  return val;
}

CORBA::Any_ptr
DynAny_i::get_any (CORBA::Environment &env)
{
  CORBA_Any_ptr val = new CORBA_Any;

  if (!(this->value_ >>= *val))
    {
      delete val;
      val = 0;
      env.exception (new DynAny::TypeMismatch);
    }

  return val;
}


//*************************** "ORB" ***********************************

///////////////////////////////////////////////////////////////////////
// InconsistentTypeCode exception

// default constructor
DynAny_i::InconsistentTypeCode::InconsistentTypeCode (void)
  : CORBA_UserException (DynAny_i::_tc_InconsistentTypeCode)
{
}

// destructor - all members are of self managing types
DynAny_i::InconsistentTypeCode::~InconsistentTypeCode (void)
{
}

// copy constructor
DynAny_i::InconsistentTypeCode::InconsistentTypeCode (const DynAny_i::InconsistentTypeCode &_tao_excp)
  : CORBA_UserException (_tao_excp._type ())
{
}

// assignment operator
DynAny_i::InconsistentTypeCode&
DynAny_i::InconsistentTypeCode::operator= (const DynAny_i::InconsistentTypeCode &_tao_excp)
{

  this->CORBA_UserException::operator= (_tao_excp);
  return *this;
}

// narrow
DynAny_i::InconsistentTypeCode_ptr 
DynAny_i::InconsistentTypeCode::_narrow (CORBA::Exception *exc)
{
  if (!ACE_OS::strcmp ("IDL:DynAny/InconsistentTypeCode:1.0", exc->_id ())) // same type
    return ACE_dynamic_cast (DynAny_i::InconsistentTypeCode_ptr, exc);
  else
    return 0;
}


void DynAny_i::InconsistentTypeCode::_raise ()
{
  TAO_RAISE(*this);
}

// TAO extension - the _alloc method
CORBA::Exception *DynAny_i::InconsistentTypeCode::_alloc (void)
{
  return new DynAny_i::InconsistentTypeCode;
}

static const CORBA::Long _oc_DynAny_InconsistentTypeCode[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  28, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x44796e41), ACE_NTOHL (0x6e792f54), ACE_NTOHL (0x7970654d), ACE_NTOHL (0x69736d61), ACE_NTOHL (0x7463683a), ACE_NTOHL (0x312e3000),  // repository ID = IDL:DynAny/InconsistentTypeCode:1.0
  13, ACE_NTOHL (0x54797065), ACE_NTOHL (0x4d69736d), ACE_NTOHL (0x61746368), ACE_NTOHL (0xfdfdfd),  // name = InconsistentTypeCode
  0, // member count
};
static CORBA::TypeCode _tc__tc_DynAny_InconsistentTypeCode (CORBA::tk_except, sizeof (_oc_DynAny_InconsistentTypeCode), (char *) &_oc_DynAny_InconsistentTypeCode, 0);
CORBA::TypeCode_ptr DynAny_i::_tc_InconsistentTypeCode = &_tc__tc_DynAny_InconsistentTypeCode;

///////////////////////////////////////////////////////////////////////////////////////////////
// The static "create" functions

DynAny_ptr
DynAny_i::create_dyn_any (const CORBA_Any& any,
                          CORBA::Environment& env)
{
  switch (DynAny_i::unalias (any.type (), env))
    {
      case CORBA::tk_null:
      case CORBA::tk_void:
      case CORBA::tk_short:
      case CORBA::tk_long: 
      case CORBA::tk_ushort:
      case CORBA::tk_ulong:
      case CORBA::tk_float:
      case CORBA::tk_double: 
      case CORBA::tk_longlong:
      case CORBA::tk_ulonglong:
      case CORBA::tk_boolean:
      case CORBA::tk_char:
      case CORBA::tk_wchar:
      case CORBA::tk_octet:
      case CORBA::tk_any:
      case CORBA::tk_TypeCode:
      case CORBA::tk_objref:
      case CORBA::tk_string:
        {
          DynAny_i* dp = new DynAny_i (any);
          return dp->_this (env);
        }
      case CORBA::tk_struct:
      case CORBA::tk_except:
        {
          DynStruct_i* dp = new DynStruct_i (any);
          return dp->_this (env);
        }
      case CORBA::tk_sequence:
        {
          DynSequence_i* dp = new DynSequence_i (any);
          return dp->_this (env);
        }
      case CORBA::tk_union:
        {
          DynUnion_i* dp = new DynUnion_i (any);
          return dp->_this (env);
        }
      case CORBA::tk_enum:
        {
          DynEnum_i* dp = new DynEnum_i (any);
          return dp->_this (env);
        }
      case CORBA::tk_array:
        {
          DynArray_i* dp = new DynArray_i (any);
          return dp->_this (env);
        }
      default:
        break;
    }

  return 0;
}

DynAny_ptr
DynAny_i::create_basic_dyn_any (CORBA_TypeCode_ptr tc,
                                CORBA::Environment& env)
{
  DynAny_i* dp = new DynAny_i (tc);
  return dp->_this (env);
}

DynStruct_ptr
DynAny_i::create_dyn_struct (CORBA_TypeCode_ptr tc,
                             CORBA::Environment& env)
{
  DynStruct_i* dp = new DynStruct_i (tc);
  return dp->_this (env);
}

DynSequence_ptr
DynAny_i::create_dyn_sequence (CORBA_TypeCode_ptr tc,
                               CORBA::Environment& env)
{
  DynSequence_i* dp = new DynSequence_i (tc);
  return dp->_this (env);
}

DynArray_ptr
DynAny_i::create_dyn_array (CORBA_TypeCode_ptr tc,
                            CORBA::Environment& env)
{
  DynArray_i* dp = new DynArray_i (tc);
  return dp->_this (env);
}

DynUnion_ptr
DynAny_i::create_dyn_union (CORBA_TypeCode_ptr tc,
                            CORBA::Environment& env)
{
  DynUnion_i* dp = new DynUnion_i (tc);
  return dp->_this (env);
}

DynEnum_ptr
DynAny_i::create_dyn_enum (CORBA_TypeCode_ptr tc,
                           CORBA::Environment& env)
{
  DynEnum_i* dp = new DynEnum_i (tc);
  return dp->_this (env);
}

DynAny_ptr
DynAny_i::create_dyn_any (CORBA_TypeCode_ptr tc,
                          CORBA::Environment& env)
{
  switch (DynAny_i::unalias (tc, env))
    {
      case CORBA::tk_null:
      case CORBA::tk_void:
      case CORBA::tk_short:
      case CORBA::tk_long: 
      case CORBA::tk_ushort:
      case CORBA::tk_ulong:
      case CORBA::tk_float:
      case CORBA::tk_double: 
      case CORBA::tk_longlong:
      case CORBA::tk_ulonglong:
      case CORBA::tk_boolean:
      case CORBA::tk_char:
      case CORBA::tk_wchar:
      case CORBA::tk_octet:
      case CORBA::tk_any:
      case CORBA::tk_TypeCode:
      case CORBA::tk_objref:
      case CORBA::tk_string:
        return create_basic_dyn_any (tc, env);
      case CORBA::tk_struct:
      case CORBA::tk_except:
        return create_dyn_struct (tc, env);
      case CORBA::tk_sequence:
        return create_dyn_sequence (tc, env);
      case CORBA::tk_union:
        return create_dyn_union (tc, env);
      case CORBA::tk_enum:
        return create_dyn_enum (tc, env);
      case CORBA::tk_array:
        return create_dyn_array (tc, env);
      default:
        break;
    }

  env.exception (new DynAny_i::InconsistentTypeCode);
  return 0;
}

CORBA::TCKind
DynAny_i::unalias (CORBA_TypeCode_ptr tc,
                   CORBA::Environment& env)
{
  CORBA::TCKind tck = tc->kind (env);

  while (tck == CORBA::tk_alias)
    tck = DynAny_i::unalias (tc->content_type (env), env);

  return tck;
}

