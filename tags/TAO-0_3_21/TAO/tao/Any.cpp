// @ (#) $Id$

// Copyright 1994-1995 by Sun Microsystems Inc.
// All Rights Reserved
//
// ORB:         Implementation of CORBA_Any
//
// This includes three constructors, a destructor, and a "replace"
// method for the "Any" data type.  "Any" values pair a pointer to a
// data structure in the native binary representation (e.g. C struct)
// with a TypeCode that describes that data structure.
//
// The copy constructor and the destructor each use the TypeCode
// interpreter with specialized "visit" callback routines.  The
// "visit" routines are used respectively to make "deep copies" and
// perform "deep frees" of the aritrary values as described by the
// "Any" value's typecode.
//
// Note that these "visit" routines are called directly, and they
// choose whether or not to use the TypeCode interpreter to examine
// constituents.  In the simple cases, the "visit" routines can do
// their work without any further calls; only for constructed types is
// the interpreter's knowledge really required.
//
// THREADING NOTE: "Any" is a data structure which must be protected
// by external critical sections.  Like simpler numeric types, "Any"
// instances are accessed and modified atomically.  This
// implementation is reentrant, so that independent "Any" values may
// be manipulated concurrently when the underlying programming
// environment is itself reentrant.
//

#include "tao/Any.h"
#include "tao/Typecode.h"
#include "tao/Marshal.h"
#include "tao/Object.h"

#if !defined (__ACE_INLINE__)
# include "tao/Any.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(tao, Any, "$Id$")

CORBA::TypeCode_ptr
CORBA_Any::type (void) const
{
  return CORBA::TypeCode::_duplicate (this->type_);
}

// Will replace if the typecode arg is an
// alias for the existing one - otherwise raises an exception.
void
CORBA_Any::type (CORBA::TypeCode_ptr tc,
                 CORBA::Environment &env)
{
  if (this->type_->equivalent (tc, env))
    {
      CORBA::release (this->type_);
      this->type_ = CORBA::TypeCode::_duplicate (tc);
    }
  else
    {
      env.exception (new CORBA::BAD_TYPECODE ());
    }
}

// TAO doesn't give any guarantees if the value returned by value can be cast
// to the desired type. This is a deprecated routine and its use must be
// avoided. Use the >>= operators.
const void *
CORBA_Any::value (void) const
{
  if (this->any_owns_data_)
    return this->value_;
  else
    return this->cdr_;
}

// Default "Any" constructor -- initializes to nulls per the
// OMG C++ mapping.

// NOTE: null (zero) typecode pointers are also treated as the null
// typecode ...

CORBA_Any::CORBA_Any (void)
  : type_ (CORBA::TypeCode::_duplicate (CORBA::_tc_null)),
    value_ (0),
    cdr_ (0),
    any_owns_data_ (0)
{
}

// The more common "Any" constructor has its own copy of a typecode,
// and either holds or "consumes" an arbitrary data value satisfying
// the normal binary interface rules.

CORBA_Any::CORBA_Any (CORBA::TypeCode_ptr tc,
                      void *value,
                      CORBA::Boolean any_owns_data)
  : type_ (CORBA::TypeCode::_duplicate (tc)),
    value_ (value),
    cdr_ (0),
    any_owns_data_ (any_owns_data)
{
  if (this->value_ != 0)
    {
      CORBA::Environment env;
      TAO_OutputCDR stream;

      // encode the value
      stream.encode (this->type_, this->value_, 0, env);

      // retrieve the start of the message block chain and save it
      this->cdr_ = stream.begin ()->clone ();
    }
}

// Constructor using a message block.
CORBA_Any::CORBA_Any (CORBA::TypeCode_ptr type,
                      const ACE_Message_Block* mb)
  : type_ (CORBA::TypeCode::_duplicate (type)),
    value_ (0),
    cdr_ (ACE_Message_Block::duplicate (mb)),
    any_owns_data_ (0)
{
}

// Copy constructor for "Any".
CORBA_Any::CORBA_Any (const CORBA_Any &src)
  : value_ (0),
    cdr_ (0),
    any_owns_data_ (1)
{
  if (src.type_ != 0)
    this->type_ = CORBA::TypeCode::_duplicate (src.type_);
  else
    this->type_ = CORBA::TypeCode::_duplicate (CORBA::_tc_null);

  CORBA::Environment env;

  // CDR stream always contains encoded object, if any holds anything at all.
  this->cdr_ = ACE_Message_Block::duplicate (src.cdr_);

  // No need to copy src's value_.  We can always get that from cdr.
}

// assignment operator
CORBA_Any &
CORBA_Any::operator= (const CORBA_Any &src)
{
  CORBA::Environment env;

  // check if it is a self assignment
  if (this == &src)
    return *this;

  // decrement the refcount on the Message_Block we hold, it does not
  // matter if we own the data or not, because we always own the
  // message block (i.e. it is always cloned or duplicated.
  ACE_Message_Block::release ((ACE_Message_Block *) this->cdr_);
  this->cdr_ = 0;

  // if we own any previous data, deallocate it
  this->free_value (env);

  if (this->type_ != 0)
    CORBA::release (this->type_);

  // Now copy the contents of the source to ourselves.
  if (src.type_ != 0)
    this->type_ = CORBA::TypeCode::_duplicate (src.type_);
  else
    this->type_ = CORBA::TypeCode::_duplicate (CORBA::_tc_null);
  this->any_owns_data_ = 1;

  this->cdr_ = ACE_Message_Block::duplicate (src.cdr_);
  // Simply duplicate the cdr string here.  We can save the decode operation
  // if there's no need to extract the object.

  return *this;
}

// Destructor for an "Any" deep-frees memory if needed.
//

CORBA_Any::~CORBA_Any (void)
{
  // decrement the refcount on the Message_Block we hold, it does not
  // matter if we own the data or not, because we always own the
  // message block (i.e. it is always cloned or duplicated.
  ACE_Message_Block::release (this->cdr_);
  this->cdr_ = 0;

  CORBA::Environment env;
  this->free_value (env);

  if (this->type_)
    CORBA::release (this->type_);

}

// All-at-once replacement of the contents of an "Any."

void
CORBA_Any::replace (CORBA::TypeCode_ptr tc,
                    const void *value,
                    CORBA::Boolean any_owns_data,
                    CORBA::Environment &ACE_TRY_ENV)
{
  // Decrement the refcount on the Message_Block we hold, it does not
  // matter if we own the data or not, because we always own the
  // message block (i.e. it is always cloned or duplicated).
  ACE_Message_Block::release (this->cdr_);
  this->cdr_ = 0;

 this->free_value (ACE_TRY_ENV);
  ACE_CHECK;

  // Duplicate tc and then release this->type_, just in case tc and
  // type_ are the same thing.
  CORBA::TypeCode_ptr tmp = CORBA::TypeCode::_duplicate (tc);
  CORBA::release (this->type_);
  this->type_ = tmp;

  // NW: I think an Any should alway owns the CDR stream, so I removed the
  //     check here.
  // if the Any owns the data, we encode the "value" into a CDR stream and
  // store it. We also destroy the "value" since we own it.
  TAO_OutputCDR stream;
  stream.encode (tc, value, 0, ACE_TRY_ENV);
  ACE_CHECK;

  this->value_ = ACE_const_cast(void *, value);
  this->any_owns_data_ = any_owns_data;
  this->cdr_ = 0;

  // retrieve the start of the message block chain and duplicate it
  this->cdr_ = ACE_Message_Block::duplicate (stream.begin ());
}

void
CORBA_Any::_tao_replace (CORBA::TypeCode_ptr tc,
                         const ACE_Message_Block *mb,
                         CORBA::Boolean any_owns_data,
                         CORBA::Environment &env)
{
  // Decrement the refcount on the Message_Block we hold, it does not
  // matter if we own the data or not, because we always own the
  // message block (i.e. it is always cloned or duplicated).
  ACE_Message_Block::release (this->cdr_);
  this->cdr_ = 0;

  this->free_value (env);

  // Duplicate tc and then release this->type_, just in case tc and
  // type_ are the same thing.
  CORBA::TypeCode_ptr tmp = CORBA::TypeCode::_duplicate (tc);
  CORBA::release (this->type_);
  this->type_ = tmp;

  this->any_owns_data_ = any_owns_data;

  this->cdr_ = ACE_Message_Block::duplicate (mb);
  // We can save the decode operation
  // if there's no need to extract the object.
 }

void
CORBA_Any::_tao_replace (CORBA::TypeCode_ptr tc,
                         const ACE_Message_Block *mb,
                         CORBA::Boolean any_owns_data,
                         void* value,
                         CORBA::Environment &ACE_TRY_ENV)
{
  // Decrement the refcount on the Message_Block we hold, it does not
  // matter if we own the data or not, because we always own the
  // message block (i.e. it is always cloned or duplicated).
  ACE_Message_Block::release (this->cdr_);
  this->cdr_ = 0;

  this->free_value (ACE_TRY_ENV);
  ACE_CHECK;

  this->value_ = value;

  // Duplicate tc and then release this->type_, just in case tc and
  // type_ are the same thing.
  CORBA::TypeCode_ptr tmp = CORBA::TypeCode::_duplicate (tc);
  CORBA::release (this->type_);
  this->type_ = tmp;

  this->any_owns_data_ = any_owns_data;

  this->cdr_ = ACE_Message_Block::duplicate (mb);
  // We can save the decode operation
  // if there's no need to extract the object.
}

void
CORBA_Any::_tao_replace (CORBA::TypeCode_ptr tc,
                         CORBA::Boolean any_owns_data,
                         void* value,
                         CORBA::Environment &ACE_TRY_ENV)
{
  this->free_value (ACE_TRY_ENV);
  ACE_CHECK;

  this->value_ = value;

  // Duplicate tc and then release this->type_, just in case tc and
  // type_ are the same thing.
  CORBA::TypeCode_ptr tmp = CORBA::TypeCode::_duplicate (tc);
  CORBA::release (this->type_);
  this->type_ = tmp;

  this->any_owns_data_ = any_owns_data;
}

// Free internal data.
void
CORBA_Any::free_value (CORBA::Environment &env)
{
  if (this->any_owns_data_ && this->value_ != 0)
    {
      // This is not exception safe.
      DEEP_FREE (this->type_, this->value_, 0, env);
      ::operator delete (this->value_);
    }
  this->value_ = 0;
}

// insertion of typecode
void
CORBA_Any::operator<<= (CORBA::TypeCode_ptr tc)
{
  CORBA::Environment env;
  CORBA::TypeCode_ptr *_tao_tc;
  ACE_NEW (_tao_tc, CORBA::TypeCode_ptr);
  *_tao_tc = CORBA::TypeCode::_duplicate (tc);
  this->replace (CORBA::_tc_TypeCode,
                 _tao_tc,
                 1,
                 env);
}

// insertion of CORBA object
void
CORBA::Any::operator<<= (const CORBA::Object_ptr obj)
{
  CORBA::Object_ptr objptr = CORBA::Object::_duplicate (obj);
  (*this) <<= &objptr;
}

void
CORBA::Any::operator<<= (CORBA::Object_ptr *objptr)
{
  CORBA::Environment env;
  CORBA::Object_ptr *_tao_object_ptr;
  ACE_NEW (_tao_object_ptr, CORBA::Object_ptr);
  *_tao_object_ptr = *objptr;
  this->replace (CORBA::_tc_Object,
                 _tao_object_ptr,
                 1,
                 env);
}

// insertion of from_string
void
CORBA_Any::operator<<= (from_string s)
{
  // If the inserted string is bounded, we create a typecode.
  static CORBA::Long _oc_string [] =
  {
    // CDR typecode octets
    TAO_ENCAP_BYTE_ORDER,   // native endian + padding; "tricky"
    0                       // ... unbounded string to start with
  };

  CORBA::TypeCode_ptr tc = 0;
  if (s.bound_ > 0)
    {
      // Bounded string.
      _oc_string [1] = s.bound_;
      ACE_NEW (tc, CORBA::TypeCode (CORBA::tk_string,
                                    sizeof _oc_string,
                                    (char *) &_oc_string,
                                    1,
                                    sizeof (CORBA::String_var)));
    }
  else
    tc = CORBA::_tc_string; // unbounded.

  CORBA::Environment env;
  if (s.nocopy_)
    this->replace (tc, new char* (s.val_), 1, env);
  else
  // copying
    this->replace (tc, new char* (CORBA::string_dup (s.val_)),
                   1, env);
}

// Extraction: these are safe and hence we have to check that the
// typecode of the Any is equal to the one we are trying to extract
// into

CORBA::Boolean
CORBA_Any::operator>>= (CORBA::Short &s) const
{
  CORBA::Environment env;

  if (this->type_->equivalent (CORBA::_tc_short, env))
  {
    if (this->any_owns_data_ && this->value_)
    {
      s = *(CORBA::Short *) this->value_;
      return 1;
    }
    else
    {
      TAO_InputCDR stream (this->cdr_);
      return stream.read_short (s);
    }
  }
  else
  return 0;
}

CORBA::Boolean
CORBA_Any::operator>>= (CORBA::UShort &s) const
{
  CORBA::Environment env;

  if (this->type_->equivalent (CORBA::_tc_ushort, env))
  {
    if (this->any_owns_data_ && this->value_)
    {
      s = *(CORBA::UShort *) this->value_;
      return 1;
    }
    else
    {
      TAO_InputCDR stream (this->cdr_);
      return stream.read_ushort (s);
    }
  }
  else
  return 0;
}

CORBA::Boolean
CORBA_Any::operator>>= (CORBA::Long &l) const
{
  CORBA::Environment env;

  if (this->type_->equivalent (CORBA::_tc_long, env))
  {
    if (this->any_owns_data_ && this->value_)
    {
      l = *(CORBA::Long *) this->value_;
      return 1;
    }
    else
    {
      TAO_InputCDR stream (this->cdr_);
      return stream.read_long (l);
    }
  }
  else
  return 0;
}

CORBA::Boolean
CORBA_Any::operator>>= (CORBA::ULong &l) const
{
  CORBA::Environment env;

  if (this->type_->equivalent (CORBA::_tc_ulong, env))
  {
    if (this->any_owns_data_ && this->value_)
    {
      l = *(CORBA::ULong *) this->value_;
      return 1;
    }
    else
    {
      TAO_InputCDR stream (this->cdr_);
      return stream.read_ulong (l);
    }
  }
  else
  return 0;
}

CORBA::Boolean
CORBA_Any::operator>>= (CORBA::LongLong &l) const
{
  CORBA::Environment env;

  if (this->type_->equivalent (CORBA::_tc_longlong, env))
  {
    if (this->any_owns_data_ && this->value_)
    {
      l = *(CORBA::LongLong *) this->value_;
      return 1;
    }
    else
    {
      TAO_InputCDR stream (this->cdr_);
      return stream.read_longlong (l);
    }
  }
  else
  return 0;
}

CORBA::Boolean
CORBA_Any::operator>>= (CORBA::ULongLong &l) const
{
  CORBA::Environment env;

  if (this->type_->equivalent (CORBA::_tc_ulonglong, env))
  {
    if (this->any_owns_data_ && this->value_)
    {
      l = *(CORBA::ULongLong *) this->value_;
      return 1;
    }
    else
    {
      TAO_InputCDR stream (this->cdr_);
      return stream.read_ulonglong (l);
    }
  }
  else
  return 0;
}

CORBA::Boolean
CORBA_Any::operator>>= (CORBA::Float &f) const
{
  CORBA::Environment env;

  if (this->type_->equivalent (CORBA::_tc_float, env))
  {
    if (this->any_owns_data_ && this->value_)
    {
      f = *(CORBA::Float *) this->value_;
      return 1;
    }
    else
    {
      TAO_InputCDR stream (this->cdr_);
      return stream.read_float (f);
    }
  }
  else
  return 0;
}

CORBA::Boolean
CORBA_Any::operator>>= (CORBA::Double &d) const
{
  CORBA::Environment env;

  if (this->type_->equivalent (CORBA::_tc_double, env))
  {
    if (this->any_owns_data_ && this->value_)
    {
      d = *(CORBA::Double *) this->value_;
      return 1;
    }
    else
    {
      TAO_InputCDR stream (this->cdr_);
      return stream.read_double (d);
    }
  }
  else
  return 0;
}

CORBA::Boolean
CORBA_Any::operator>>= (CORBA::Any &a) const
{
  CORBA::Environment env;

  if (this->type_->equivalent (CORBA::_tc_any, env))
  {
    if (this->any_owns_data_ && this->value_)
    {
      a = *(CORBA::Any *) this->value_;
      return 1;
    }
    else
    {
      TAO_InputCDR stream (this->cdr_);
      // decode the CDR
      return (stream.decode (CORBA::_tc_any, &a, 0, env)
              == CORBA::TypeCode::TRAVERSE_CONTINUE) ? 1 : 0;
    }
  }
  else
  return 0;
}

CORBA::Boolean
CORBA_Any::operator>>= (char *&s) const
{
  CORBA::Environment env;

  if (this->type_->equal (CORBA::_tc_string, env))
  {
    if (this->any_owns_data_ && this->value_)
    {
      s = *(char **) this->value_;
      return 1;
    }
    else
    {
      TAO_InputCDR stream (this->cdr_);
      if (stream.read_string (s))
      {
        ACE_const_cast(CORBA_Any*, this)->any_owns_data_ = 1;
        char** tmp = new char*;
        *tmp = s;
        ACE_const_cast(CORBA_Any*, this)->value_ = tmp;
        return 1;
      }
      return 0;
    }
  }
  else
  return 0;
}

CORBA::Boolean
CORBA_Any::operator>>= (CORBA::TypeCode_ptr &tc) const
{
  CORBA::Environment env;

  if (this->type_->equivalent (CORBA::_tc_TypeCode, env))
  {
    if (this->any_owns_data_ && this->value_)
    {
      tc = *(CORBA::TypeCode_ptr *) this->value_;
      return 1;
    }
    else
    {
      TAO_InputCDR stream ((ACE_Message_Block *) this->cdr_);
      CORBA::Boolean flag =
      (stream.decode (CORBA::_tc_TypeCode,
                      &tc,
                      0,
                      env)
       == CORBA::TypeCode::TRAVERSE_CONTINUE) ? 1 : 0;
      return flag;
    }
  }
  else
  return 0;
}

// = extraction into the special types

CORBA::Boolean
CORBA_Any::operator>>= (to_boolean b) const
{
  CORBA::Environment env;

  if (this->type_->equivalent (CORBA::_tc_boolean, env))
  {
    if (this->any_owns_data_ && this->value_)
    {
      b.ref_ = *(CORBA::Boolean *) this->value_;
      return 1;
    }
    else
    {
      TAO_InputCDR stream ((ACE_Message_Block *) this->cdr_);
      return stream.read_boolean (b.ref_);
    }
  }
  else
  return 0;
}

CORBA::Boolean
CORBA_Any::operator>>= (to_octet o) const
{
  CORBA::Environment env;

  if (this->type_->equivalent (CORBA::_tc_octet, env))
  {
    if (this->any_owns_data_ && this->value_)
    {
      o.ref_ = *(CORBA::Octet *) this->value_;
      return 1;
    }
    else
    {
      TAO_InputCDR stream ((ACE_Message_Block *) this->cdr_);
      return stream.read_octet (o.ref_);
    }
  }
  else
  return 0;
}

CORBA::Boolean
CORBA_Any::operator>>= (to_char c) const
{
  CORBA::Environment env;

  if (this->type_->equivalent (CORBA::_tc_char, env))
  {
    if (this->any_owns_data_ && this->value_)
    {
      c.ref_ = *(CORBA::Char *) this->value_;
      return 1;
    }
    else
    {
      TAO_InputCDR stream ((ACE_Message_Block *) this->cdr_);
      return stream.read_char (c.ref_);
    }
  }
  else
  return 0;
}

CORBA::Boolean
CORBA_Any::operator>>= (to_wchar wc) const
{
  CORBA::Environment env;

  if (this->type_->equivalent (CORBA::_tc_wchar, env))
  {
    if (this->any_owns_data_ && this->value_)
    {
      wc.ref_ = *(CORBA::WChar *) this->value_;
      return 1;
    }
    else
    {
      TAO_InputCDR stream ((ACE_Message_Block *) this->cdr_);
      return stream.read_wchar (wc.ref_);
    }
  }
  else
  return 0;
}

CORBA::Boolean
CORBA_Any::operator>>= (to_string s) const
{
  CORBA::Environment env;

  if (this->type_->equivalent (CORBA::_tc_string, env))
  {
    CORBA::ULong bound = this->type_->length (env);
    if (s.bound_ == bound) // bounds are same
    {
      if (this->any_owns_data_ && this->value_)
      {
        s.val_ = *(char **) this->value_;
        return 1;
      }
      else
      {
        TAO_InputCDR stream ((ACE_Message_Block *) this->cdr_);
        if (stream.read_string (s.val_))
        {
          ACE_const_cast (CORBA_Any*, this)->any_owns_data_ = 1;
          return 1;
        }
      }
    }
  }

  // Otherwise.
  return 0;
}

CORBA::Boolean
CORBA_Any::operator>>= (to_object obj) const
{
  CORBA::Environment env;

  if (this->type_->equivalent (CORBA::_tc_Object, env))
  {
    if (this->any_owns_data_ && this->value_)
    {
      obj.ref_ = *(CORBA::Object_ptr *) this->value_;
      return 1;
    }
    else
    {
      TAO_InputCDR stream ((ACE_Message_Block *) this->cdr_);
      CORBA::Boolean flag = (stream.decode (CORBA::_tc_Object,
                                            &obj.ref_, 0, env)
                             == CORBA::TypeCode::TRAVERSE_CONTINUE) ? 1:0;
      if (flag)
      {
        CORBA::Object_ptr *tmp = new CORBA::Object_ptr;
        *tmp = obj.ref_;
        ACE_const_cast(CORBA_Any*,this)->value_ = tmp;
        return 1;
      }
      // we own this allocated value
      //          this->value_ = obj.ref_;
      return flag;
    }
  }
  else
  return 0;
}

// this is a copying version for unbounded strings
// Not inline, to avoid use in Any.i before definition in ORB.i.
void
CORBA_Any::operator<<= (const char* s)
{
  CORBA::Environment env;
  this->replace (CORBA::_tc_string, new char* (CORBA::string_dup (s)),
                 1, env);
}

// ----------------------------------------------------------------------
// Any_var type
// ----------------------------------------------------------------------

CORBA::Any_var &
CORBA_Any_var::operator= (CORBA::Any *p)
{
  if (this->ptr_ != p)
  {
    if (this->ptr_ != 0)
    delete (this->ptr_);

    this->ptr_ = p;
  }
  return *this;
}

CORBA::Any_var &
CORBA_Any_var::operator= (const CORBA::Any_var& r)
{
  if (this->ptr_ != 0)
  delete this->ptr_;

  this->ptr_ = new CORBA::Any (*r.ptr_);
  return *this;
}

// = Debugging method.

// Prints the type and the value of the any value. Dumping is
// supported only for standard data types.

void
CORBA_Any::dump (const CORBA::Any &any_value)
{
  CORBA::Environment env;

  // Get the type.
  CORBA::TypeCode_var type = any_value.type ();

  if (type->equal (CORBA::_tc_null, env))
    ACE_DEBUG ((LM_DEBUG,"Null\n"));

  else if (type->equal (CORBA::_tc_void, env))
    ACE_DEBUG ((LM_DEBUG,"Void\n"));

  else if (type->equal (CORBA::_tc_short, env))
    {
      CORBA::Short s;
      any_value >>= s;
      ACE_DEBUG ((LM_DEBUG,"Short %d\n",s));
    }
  else if (type->equal (CORBA::_tc_long, env))
    {
      CORBA::Long l;
      any_value >>= l;
      ACE_DEBUG ((LM_DEBUG,"Long %d\n", l));
    }
  else if (type->equal (CORBA::_tc_ushort, env))
    {
      CORBA::UShort s;
      any_value >>= s;
      ACE_DEBUG ((LM_DEBUG,"UShort %d\n", s));
    }
  else if (type->equal (CORBA::_tc_ulong, env))
    {
      CORBA::ULong l;
      any_value >>= l;
      ACE_DEBUG ((LM_DEBUG,"ULong %d\n", l));
    }
  else if (type->equal (CORBA::_tc_float, env))
    {
      CORBA::Float f;
      any_value >>= f;
      ACE_DEBUG ((LM_DEBUG,"Float %f\n", f));
    }
  else if (type->equal (CORBA::_tc_double, env))
    {
      CORBA::Double d;
      any_value >>= d;
      ACE_DEBUG ((LM_DEBUG,"Double %f\n", d));
    }
  else if (type->equal (CORBA::_tc_boolean, env))
    {
      CORBA::Boolean b;
      any_value >>= (to_boolean)b;
      if (b == 1)
        ACE_DEBUG ((LM_DEBUG, "Boolean TRUE\n"));
      else
        ACE_DEBUG ((LM_DEBUG, "Boolean FALSE\n"));
    }
  else if (type->equal (CORBA::_tc_char, env))
    {
      CORBA::Char ch;
      any_value >>= to_char (ch);
      ACE_DEBUG ((LM_DEBUG,"Char %c\n", ch));
    }
  else if (type->equal (CORBA::_tc_string, env))
    {
      CORBA::String str;
      any_value >>= str;
      ACE_DEBUG ((LM_DEBUG, "String %s\n", str));
    }
  else
    ACE_DEBUG ((LM_DEBUG, "TCKind %d\n", type->kind_));
}
