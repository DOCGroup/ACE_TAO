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
// COM NOTE: Yes, this is a utility data type whose implementation is
// fully exposed.  Factories for these are not normally used in C++.

#include "tao/corba.h"

#if !defined (__ACE_INLINE__)
# include "tao/Any.i"
#endif /* ! __ACE_INLINE__ */

CORBA::TypeCode_ptr
CORBA_Any::type (void) const
{
  return this->type_;
}

const void *
CORBA_Any::value (void) const
{
  return this->value_;
}

// Default "Any" constructor -- initializes to nulls per the
// OMG C++ mapping.

// NOTE: null (zero) typecode pointers are also treated as the null
// typecode ...

CORBA_Any::CORBA_Any (void)
  : type_ (CORBA::_tc_null),
    value_ (0),
    any_owns_data_ (CORBA::B_FALSE),
    refcount_ (1)
{
}

// The more common "Any" constructor has its own copy of a typecode,
// and either holds or "consumes" an arbitrary data value satisfying
// the normal binary interface rules.

CORBA_Any::CORBA_Any (CORBA::TypeCode_ptr tc,
                      void *value,
                      CORBA::Boolean any_owns_data)
  : type_ (tc),
    value_ (value),
    any_owns_data_ (any_owns_data),
    refcount_ (1)
{
  tc->AddRef ();
}

// Copy constructor for "Any".
CORBA_Any::CORBA_Any (const CORBA_Any &src)
  : type_ (src.type_ != 0 ? src.type_ : CORBA::_tc_null),
    any_owns_data_ (CORBA::B_TRUE),
    refcount_ (1)
{
  CORBA::Environment env;
  size_t size;

  this->type_->AddRef ();

  size = this->type_->size (env);

  // allocate sufficient memory and deep copy the data
  // @@ - the following allocation to be addressed by our memory management
  // scheme
  this->value_ = (char *) ACE_OS::calloc (1, size);
  (void) DEEP_COPY (this->type_, src.value_, this->value_, env);
}

// assignment operator
CORBA_Any &
CORBA_Any::operator= (const CORBA_Any &src)
{
  CORBA::Environment env;
  size_t size;

  // check if it is a self assignment
  if (this == &src)
    {
      this->AddRef ();
      return *this;
    }

  // if we own any previous data, deallocate it
  if (this->any_owns_data_)
    {
      DEEP_FREE (this->type_, this->value_, 0, env);
      // @@: The following needs to be addressed properly. We need to make
      // sure if we use "delete" or "free"
      //      delete this->value_;
    }

  // Now copy the contents of the source to ourselves.
  this->type_ = (src.type_) != 0 ? src.type_ : CORBA::_tc_null;

  this->any_owns_data_ = CORBA::B_TRUE;
  this->refcount_ = 1;

  this->type_->AddRef ();

  size = this->type_->size (env);

  // allocate sufficient storage and deep copy the data
  // @@ - address the following
  this->value_ = (char *) ACE_OS::calloc (1, size);
  (void) DEEP_COPY (this->type_, src.value_, this->value_, env);
  return *this;
}

// Destructor for an "Any" deep-frees memory if needed.
//
// NOTE that the assertion below will fire on application programmer
// errors, such as using AddRef/Release out of sync with the true
// lifetime of an Any value allocated on the stack.  BUT it involves
// changing the refcounting policy so that it's initialized to zero,
// not one ... which policy affects the whole source base, and not
// just this data type.  Get to this later.

CORBA_Any::~CORBA_Any (void)
{
  CORBA::Environment env;

  // assert (this->refcount_ == 0);

  if (this->any_owns_data_)
    {
      // we own the data. So first do a deep free and then deallocate it.
      DEEP_FREE (this->type_, this->value_, 0, env);
      // @@ - address the following
      // delete this->value_;
    }

  if (this->type_)
    this->type_->Release ();
}

// All-at-once replacement of the contents of an "Any."

void
CORBA_Any::replace (CORBA::TypeCode_ptr tc,
                    const void *v,
                    CORBA::Boolean any_owns_data,
                    CORBA::Environment &env)
{
  if (this->any_owns_data_)
    {
      if (this->value_)
        {
          DEEP_FREE (this->type_, this->value_, 0, env);
          // @@ - to be addressed
          //          delete this->value_;
        }
    }

  if (env.exception ())
    return;

  if (this->type_ != 0)
    this->type_->Release ();

  this->type_ = tc;
  tc->AddRef ();
  this->value_ = (void *) v;
  this->any_owns_data_ = any_owns_data;
}

// insertion of from_string
void
CORBA_Any::operator<<= (from_string s)
{
  // If the inserted string is bounded, we create a typecode.
  static CORBA::Long _oc_string [] =
  {     // CDR typecode octets
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
  else // copying
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
      s = *(CORBA::Short *) this->value_;
      return CORBA::B_TRUE;
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
      s = *(CORBA::UShort *) this->value_;
      return CORBA::B_TRUE;
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
      l = *(CORBA::Long *) this->value_;
      return CORBA::B_TRUE;
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
      l = *(CORBA::ULong *) this->value_;
      return CORBA::B_TRUE;
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
      f = *(CORBA::Float *) this->value_;
      return CORBA::B_TRUE;
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
      d = *(CORBA::Double *) this->value_;
      return CORBA::B_TRUE;
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
      a = *(CORBA::Any *) this->value_;
      return CORBA::B_TRUE;
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
      s = *(char **) this->value_;
      return CORBA::B_TRUE;
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
      b.ref_ = *(CORBA::Boolean *) this->value_;
      return CORBA::B_TRUE;
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
      o.ref_ = *(CORBA::Octet *) this->value_;
      return CORBA::B_TRUE;
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
      c.ref_ = *(CORBA::Char *) this->value_;
      return CORBA::B_TRUE;
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
          s.val_ = *(char **) this->value_;
          return CORBA::B_TRUE;
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
      obj.ref_ = *(CORBA::Object_ptr *) this->value_;
      return CORBA::B_TRUE;
    }
  else
    return CORBA::B_FALSE;
}

CORBA::ULong
CORBA_Any::AddRef (void)
{
  return ++refcount_;
}

CORBA::ULong
CORBA_Any::Release (void)
{
  {
    ACE_ASSERT (this != 0);

    if (--refcount_ != 0)
      return refcount_;
  }

  delete this;
  return 0;
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
    delete (this->ptr_);

  this->ptr_ = new CORBA::Any (*r.ptr_);
  return *this;
}
