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

#include "tao/corba.h"

#if !defined (__ACE_INLINE__)
# include "tao/Any.i"
#endif /* ! __ACE_INLINE__ */

CORBA::TypeCode_ptr
CORBA_Any::type (void) const
{
  return CORBA::TypeCode::_duplicate (this->type_);
}

// TAO doesn't give any guarantees if the value returned by value can be cast
// to the desired type. This is a deprecated routine and its use must be
// avoided. Use the >>= operators.
const void *
CORBA_Any::value (void) const
{
  if (this->any_owns_data_)
    return this->cdr_;
  else
    return this->value_;
}

// Default "Any" constructor -- initializes to nulls per the
// OMG C++ mapping.

// NOTE: null (zero) typecode pointers are also treated as the null
// typecode ...

CORBA_Any::CORBA_Any (void)
  : type_ (CORBA::TypeCode::_duplicate (CORBA::_tc_null)),
    value_ (0),
    cdr_ (0),
    any_owns_data_ (CORBA::B_FALSE)
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
  // if the Any owns the data, we encode the "value" into a CDR stream and
  // store it. We also destroy the "value" since we own it.
  if (this->any_owns_data_ && this->value_ != 0)
    {
      CORBA::Environment env;
      TAO_OutputCDR stream;

      // encode the value
      stream.encode (this->type_, this->value_, 0, env);

      // retrieve the start of the message block chain and save it
      this->cdr_ = stream.begin ()->clone ();
    }
}

// Copy constructor for "Any".
CORBA_Any::CORBA_Any (const CORBA_Any &src)
  : value_ (0),
    cdr_ (0),
    any_owns_data_ (CORBA::B_TRUE)
{
  if (src.type_ != 0)
    this->type_ = CORBA::TypeCode::_duplicate (src.type_);
  else
    this->type_ = CORBA::TypeCode::_duplicate (CORBA::_tc_null);

  CORBA::Environment env;

  // does the source own its data? If not, then it is not in the message block
  // form and must be encoded. Else we must simply duplicate the message block
  if (src.any_owns_data_)
    {
      // the data was already encoded in "src". We simply duplicate it to avoid
      // copies.
      this->cdr_ = ACE_Message_Block::duplicate (src.cdr_);
    }
  else
    {
      // "src" did not own the data. So we must do the encoding ourselves
      TAO_OutputCDR stream;

      stream.encode (this->type_, src.value_, 0, env);
      // retrieve the start of the message block chain and duplicate it
      this->cdr_ = stream.begin ()->clone ();
    }
}

// assignment operator
CORBA_Any &
CORBA_Any::operator= (const CORBA_Any &src)
{
  CORBA::Environment env;

  // check if it is a self assignment
  if (this == &src)
    {
      return *this;
    }

  // decrement the refcount on the Message_Block we hold, it does not
  // matter if we own the data or not, because we always own the
  // message block (i.e. it is always cloned or duplicated.
  ACE_Message_Block::release ((ACE_Message_Block *) this->cdr_);

  // if we own any previous data, deallocate it
  if (this->any_owns_data_ && this->value_ != 0)
    {
      DEEP_FREE (this->type_, this->value_, 0, env);
      delete this->value_;
      this->value_ = 0;

      if (this->type_ != 0)
	CORBA::release (this->type_);
    }

  // Now copy the contents of the source to ourselves.
  if (src.type_ != 0)
    this->type_ = CORBA::TypeCode::_duplicate (src.type_);
  else
    this->type_ = CORBA::TypeCode::_duplicate (CORBA::_tc_null);
  this->value_ = 0;
  this->any_owns_data_ = CORBA::B_TRUE;

  // does the source own its data? If not, then it is not in the message block
  // form and must be encoded. Else we must simply duplicate the message block
  if (src.any_owns_data_)
    {
      this->cdr_ = ACE_Message_Block::duplicate (src.cdr_);
    }
  else
    {
      TAO_OutputCDR stream;

      stream.encode (this->type_, src.value_, 0, env);
      // retrieve the start of the message block chain and duplicate it
      this->cdr_ = stream.begin ()->clone ();
    }
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
  if (this->any_owns_data_)
    {
      // free up the storage for the value
      if (this->value_)
        {
          DEEP_FREE (this->type_, this->value_, 0, env);
	  delete this->value_;
          this->value_ = 0;
        }
    }

  if (this->type_)
    CORBA::release (this->type_);

}

// All-at-once replacement of the contents of an "Any."

void
CORBA_Any::replace (CORBA::TypeCode_ptr tc,
                    const void *value,
                    CORBA::Boolean any_owns_data,
                    CORBA::Environment &env)
{
  // decrement the refcount on the Message_Block we hold, it does not
  // matter if we own the data or not, because we always own the
  // message block (i.e. it is always cloned or duplicated.
  ACE_Message_Block::release (this->cdr_);

  if (this->any_owns_data_ && this->value_ != 0)
    {
      DEEP_FREE (this->type_, this->value_, 0, env);
      delete this->value_;
      this->value_ = 0;
    }

  // Duplicate tc and then release this->type_, just in case tc and
  // type_ are the same thing.
  CORBA::TypeCode_ptr tmp = CORBA::TypeCode::_duplicate (tc);
  CORBA::release (this->type_);
  this->type_ = tmp;

  this->value_ = ACE_const_cast(void *, value);
  this->any_owns_data_ = any_owns_data;
  this->cdr_ = 0;

  // if the Any owns the data, we encode the "value" into a CDR stream and
  // store it. We also destroy the "value" since we own it.
  if (this->any_owns_data_)
    {
      CORBA::Environment env;
      TAO_OutputCDR stream;

      stream.encode (tc, value, 0, env);
      // retrieve the start of the message block chain and duplicate it
      this->cdr_ = ACE_Message_Block::duplicate (stream.begin ());
    }
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
                                    CORBA::B_TRUE));
    }
  else
    tc = CORBA::_tc_string; // unbounded.

  CORBA::Environment env;
  if (s.nocopy_)
    this->replace (tc, new char* (s.val_), CORBA::B_TRUE, env);
  else
    // copying
    this->replace (tc, new char* (CORBA::string_dup (s.val_)),
                   CORBA::B_TRUE, env);
}

// Extraction: these are safe and hence we have to check that the
// typecode of the Any is equal to the one we are trying to extract
// into

CORBA::Boolean
CORBA_Any::operator>>= (CORBA::Short &s) const
{
  CORBA::Environment env;

  if (this->type_->equal (CORBA::_tc_short, env))
    {
      if (this->any_owns_data_)
        {
          TAO_InputCDR stream (this->cdr_);
          return stream.read_short (s);
        }
      else
        {
          s = *(CORBA::Short *) this->value_;
          return CORBA::B_TRUE;
        }
    }
  else
    return CORBA::B_FALSE;
}

CORBA::Boolean
CORBA_Any::operator>>= (CORBA::UShort &s) const
{
  CORBA::Environment env;

  if (this->type_->equal (CORBA::_tc_ushort, env))
    {
      if (this->any_owns_data_)
        {
          TAO_InputCDR stream (this->cdr_);
          return stream.read_ushort (s);
        }
      else
        {
          s = *(CORBA::UShort *) this->value_;
          return CORBA::B_TRUE;
        }
    }
  else
    return CORBA::B_FALSE;
}

CORBA::Boolean
CORBA_Any::operator>>= (CORBA::Long &l) const
{
  CORBA::Environment env;

  if (this->type_->equal (CORBA::_tc_long, env))
    {
      if (this->any_owns_data_)
        {
          TAO_InputCDR stream (this->cdr_);
          return stream.read_long (l);
        }
      else
        {
          l = *(CORBA::Long *) this->value_;
          return CORBA::B_TRUE;
        }
    }
  else
    return CORBA::B_FALSE;
}

CORBA::Boolean
CORBA_Any::operator>>= (CORBA::ULong &l) const
{
  CORBA::Environment env;

  if (this->type_->equal (CORBA::_tc_ulong, env))
    {
      if (this->any_owns_data_)
        {
          TAO_InputCDR stream (this->cdr_);
          return stream.read_ulong (l);
        }
      else
        {
          l = *(CORBA::ULong *) this->value_;
          return CORBA::B_TRUE;
        }
    }
  else
    return CORBA::B_FALSE;
}

CORBA::Boolean
CORBA_Any::operator>>= (CORBA::Float &f) const
{
  CORBA::Environment env;

  if (this->type_->equal (CORBA::_tc_float, env))
    {
      if (this->any_owns_data_)
        {
          TAO_InputCDR stream (this->cdr_);
          return stream.read_float (f);
        }
      else
        {
          f = *(CORBA::Float *) this->value_;
          return CORBA::B_TRUE;
        }
    }
  else
    return CORBA::B_FALSE;
}

CORBA::Boolean
CORBA_Any::operator>>= (CORBA::Double &d) const
{
  CORBA::Environment env;

  if (this->type_->equal (CORBA::_tc_double, env))
    {
      if (this->any_owns_data_)
        {
          TAO_InputCDR stream (this->cdr_);
          return stream.read_double (d);
        }
      else
        {
          d = *(CORBA::Double *) this->value_;
          return CORBA::B_TRUE;
        }
    }
  else
    return CORBA::B_FALSE;
}

CORBA::Boolean
CORBA_Any::operator>>= (CORBA::Any &a) const
{
  CORBA::Environment env;

  if (this->type_->equal (CORBA::_tc_any, env))
    {
      if (this->any_owns_data_)
        {
          TAO_InputCDR stream (this->cdr_);
          // decode the CDR
          return (stream.decode (CORBA::_tc_any, &a, 0, env)
                  == CORBA::TypeCode::TRAVERSE_CONTINUE) ? 1 : 0;
        }
      else
        {
          a = *(CORBA::Any *) this->value_;
          return CORBA::B_TRUE;
        }
    }
  else
    return CORBA::B_FALSE;
}

CORBA::Boolean
CORBA_Any::operator>>= (char *&s) const
{
  CORBA::Environment env;

  if (this->type_->equal (CORBA::_tc_string, env))
    {
      if (this->any_owns_data_)
        {
          TAO_InputCDR stream (this->cdr_);
          return stream.read_string (s);
        }
      else
        {
          s = *(char **) this->value_;
          return CORBA::B_TRUE;
        }
    }
  else
    return CORBA::B_FALSE;
}

// = extraction into the special types

CORBA::Boolean
CORBA_Any::operator>>= (to_boolean b) const
{
  CORBA::Environment env;

  if (this->type_->equal (CORBA::_tc_boolean, env))
    {
      if (this->any_owns_data_)
        {
          TAO_InputCDR stream ((ACE_Message_Block *) this->cdr_);
          return stream.read_boolean (b.ref_);
        }
      else
        {
          b.ref_ = *(CORBA::Boolean *) this->value_;
          return CORBA::B_TRUE;
        }
    }
  else
    return CORBA::B_FALSE;
}

CORBA::Boolean
CORBA_Any::operator>>= (to_octet o) const
{
  CORBA::Environment env;

  if (this->type_->equal (CORBA::_tc_octet, env))
    {
      if (this->any_owns_data_)
        {
          TAO_InputCDR stream ((ACE_Message_Block *) this->cdr_);
          return stream.read_octet (o.ref_);
        }
      else
        {
          o.ref_ = *(CORBA::Octet *) this->value_;
          return CORBA::B_TRUE;
        }
    }
  else
    return CORBA::B_FALSE;
}

CORBA::Boolean
CORBA_Any::operator>>= (to_char c) const
{
  CORBA::Environment env;

  if (this->type_->equal (CORBA::_tc_char, env))
    {
      if (this->any_owns_data_)
        {
          TAO_InputCDR stream ((ACE_Message_Block *) this->cdr_);
          return stream.read_char (c.ref_);
        }
      else
        {
          c.ref_ = *(CORBA::Char *) this->value_;
          return CORBA::B_TRUE;
        }
    }
  else
    return CORBA::B_FALSE;
}

CORBA::Boolean
CORBA_Any::operator>>= (to_string s) const
{
  CORBA::Environment env;

  // The typecode must be equal. Since we do not readily have access
  // to the typecode of the string into which we want to retrieve, we
  // emulate the behavior of "equal"
  if (this->type_->kind (env) == CORBA::tk_string)
    {
      CORBA::ULong bound = this->type_->length (env);
      if (s.bound_ == bound) // bounds are same
        {
          if (this->any_owns_data_)
            {
              TAO_InputCDR stream ((ACE_Message_Block *) this->cdr_);
              CORBA::Boolean flag = stream.read_string (s.val_);
              return flag;
            }
          else
            {
              s.val_ = *(char **) this->value_;
              return CORBA::B_TRUE;
            }
        }
    }

  // Otherwise.
  return CORBA::B_FALSE;
}

CORBA::Boolean
CORBA_Any::operator>>= (to_object obj) const
{
  CORBA::Environment env;

  if (this->type_->equal (CORBA::_tc_Object, env))
    {
      if (this->any_owns_data_)
        {
          TAO_InputCDR stream ((ACE_Message_Block *) this->cdr_);
          CORBA::Boolean flag = (stream.decode (CORBA::_tc_Object,
                                                &obj.ref_, 0, env)
                                 == CORBA::TypeCode::TRAVERSE_CONTINUE) ? 1:0;
          // we own this allocated value
          //          this->value_ = obj.ref_;
          return flag;
        }
      else
        {
          obj.ref_ = *(CORBA::Object_ptr *) this->value_;
          return CORBA::B_TRUE;
        }
    }
  else
    return CORBA::B_FALSE;
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
CORBA_Any::dump (const CORBA::Any any_value)
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
      if (b == CORBA::B_TRUE)
        ACE_DEBUG ((LM_DEBUG, "Boolean B_TRUE\n"));
      else
        ACE_DEBUG ((LM_DEBUG, "Boolean B_FALSE\n"));
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
