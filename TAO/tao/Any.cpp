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
#include "tao/ORB_Core.h"
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

// Will replace if the typecode arg is an alias for the existing one -
// otherwise raises an exception.

void
CORBA_Any::type (CORBA::TypeCode_ptr tc,
                 CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::Boolean equiv = this->type_->equivalent (tc,
                                                  ACE_TRY_ENV);
  ACE_CHECK;

  if (equiv)
    {
      CORBA::release (this->type_);
      this->type_ = CORBA::TypeCode::_duplicate (tc);
    }
  else
    {
      ACE_THROW (CORBA::BAD_TYPECODE ());
    }
}

// TAO doesn't give any guarantees if the value returned by value can
// be cast to the desired type. This is a deprecated routine and its
// use must be avoided. Use the >>= operators.

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
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      if (this->value_ != 0)
        {
          TAO_OutputCDR stream;

          // Encode the value.
          stream.encode (this->type_,
                         this->value_,
                         0,
                         ACE_TRY_ENV);
          ACE_TRY_CHECK;

          // Retrieve the start of the message block chain and save it.
          this->cdr_ = stream.begin ()->clone ();
        }
    }
  ACE_CATCHANY
    {
      // do nothing
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

// Constructor using a message block.
CORBA_Any::CORBA_Any (CORBA::TypeCode_ptr type,
                      CORBA::UShort,
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
    any_owns_data_ (0)
{
  if (src.type_ != 0)
    this->type_ =
      CORBA::TypeCode::_duplicate (src.type_);
  else
    this->type_ =
      CORBA::TypeCode::_duplicate (CORBA::_tc_null);

  // CDR stream always contains encoded object, if any holds anything
  // at all.
  this->cdr_ = ACE_Message_Block::duplicate (src.cdr_);

  // No need to copy src's value_.  We can always get that from cdr.
}

// assignment operator

CORBA_Any &
CORBA_Any::operator= (const CORBA_Any &src)
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      // check if it is a self assignment
      if (this == &src)
        {
          return *this;
        }

      // Decrement the refcount on the Message_Block we hold, it does not
      // matter if we own the data or not, because we always own the
      // message block (i.e. it is always cloned or duplicated.
      ACE_Message_Block::release ((ACE_Message_Block *) this->cdr_);
      this->cdr_ = 0;

      // If we own any previous data, deallocate it.
      this->free_value (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (this->type_ != 0)
        {
          CORBA::release (this->type_);
        }

      // Now copy the contents of the source to ourselves.
      if (src.type_ != 0)
        {
          this->type_ =
            CORBA::TypeCode::_duplicate (src.type_);
        }
      else
        {
          this->type_ =
            CORBA::TypeCode::_duplicate (CORBA::_tc_null);
        }

      this->any_owns_data_ = 0;

      this->cdr_ = ACE_Message_Block::duplicate (src.cdr_);
      // Simply duplicate the cdr string here.  We can save the decode
      // operation if there's no need to extract the object.

      return *this;
    }
  ACE_CATCHANY
    {
      return *this;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (*this);
}

// Destructor for an "Any" deep-frees memory if needed.

CORBA_Any::~CORBA_Any (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      // decrement the refcount on the Message_Block we hold, it does not
      // matter if we own the data or not, because we always own the
      // message block (i.e. it is always cloned or duplicated.

      ACE_Message_Block::release (this->cdr_);
      this->cdr_ = 0;

      this->free_value (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (this->type_)
        CORBA::release (this->type_);
    }
  ACE_CATCHANY
    {
      // do nothing
    }
  ACE_ENDTRY;
  ACE_CHECK;
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
  CORBA::TypeCode_ptr tmp =
    CORBA::TypeCode::_duplicate (tc);
  CORBA::release (this->type_);
  this->type_ = tmp;

  // @@ NW: I think an Any should alway owns the CDR stream, so I removed the
  //     check here.
  // if the Any owns the data, we encode the "value" into a CDR stream
  // and store it. We also destroy the "value" since we own it.
  TAO_OutputCDR stream;
  stream.encode (tc,
                 value,
                 0,
                 ACE_TRY_ENV);
  ACE_CHECK;

  this->value_ = ACE_const_cast (void *, value);
  this->any_owns_data_ = any_owns_data;
  this->cdr_ = 0;

  // Retrieve the start of the message block chain and duplicate it.
  this->cdr_ = ACE_Message_Block::duplicate (stream.begin ());
}

void
CORBA_Any::_tao_replace (CORBA::TypeCode_ptr tc,
                         const ACE_Message_Block *mb,
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

  this->any_owns_data_ = 0;

  this->cdr_ = ACE_Message_Block::duplicate (mb);
  // We can save the decode operation if there's no need to extract
  // the object.
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
  // We can save the decode operation if there's no need to extract
  // the object.
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
CORBA_Any::free_value (CORBA::Environment &ACE_TRY_ENV)
{
  if (this->any_owns_data_ && this->value_ != 0)
    {
      // This is not exception safe.
      DEEP_FREE (this->type_, this->value_, 0, ACE_TRY_ENV);
      ACE_CHECK;
      ::operator delete (this->value_);
    }
  this->value_ = 0;
}

// Insertion of typecode.

void
CORBA_Any::operator<<= (CORBA::TypeCode_ptr tc)
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      CORBA::TypeCode_ptr *_tao_tc;

      ACE_NEW (_tao_tc,
               CORBA::TypeCode_ptr);

      *_tao_tc = CORBA::TypeCode::_duplicate (tc);
      this->replace (CORBA::_tc_TypeCode,
                     _tao_tc,
                     1,
                     ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      // do nothing...
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

void
CORBA_Any::operator<<= (const CORBA_Exception &exception)
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      TAO_OutputCDR stream;
      stream.encode (exception._type (),
                     &exception, 0,
                     ACE_TRY_ENV);
      ACE_TRY_CHECK;

      this->_tao_replace (exception._type (),
                          stream.begin (),
                          ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      // do nothing...
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

// Insertion of CORBA object - copying.

void
CORBA::Any::operator<<= (const CORBA::Object_ptr obj)
{
  CORBA::Object_ptr objptr =
    CORBA::Object::_duplicate (obj);

  (*this) <<= &objptr;
}

// Insertion of CORBA object - non-copying.

void
CORBA::Any::operator<<= (CORBA::Object_ptr *objptr)
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      CORBA::Object_ptr *_tao_object_ptr;

      ACE_NEW (_tao_object_ptr,
               CORBA::Object_ptr);

      *_tao_object_ptr = *objptr;
      this->replace (CORBA::_tc_Object,
                     _tao_object_ptr,
                     1,
                     ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      // do nothing...
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

// Insertion of from_string.

void
CORBA_Any::operator<<= (from_string s)
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
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
          ACE_NEW (tc,
                   CORBA::TypeCode (CORBA::tk_string,
                                    sizeof _oc_string,
                                    (char *) &_oc_string,
                                    1,
                                    sizeof (CORBA::String_var)));
        }
      else
        {
          tc = CORBA::_tc_string; // unbounded.
        }

      char **tmp;
      // Non-copying.
      if (s.nocopy_)
        {
          ACE_NEW(tmp,
                  char* (s.val_));
        }
      // Copying.
      else
        {
          ACE_NEW(tmp,
                  char* (CORBA::string_dup (s.val_)));
        }

      this->replace (tc,
                     tmp,
                     1,
                     ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      // do nothing...
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

// Extraction: these are safe and hence we have to check that the
// typecode of the Any is equal to the one we are trying to extract
// into

CORBA::Boolean
CORBA_Any::operator>>= (CORBA::Short &s) const
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      CORBA::Boolean result =
        this->type_->equivalent (CORBA::_tc_short,
                                 ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (result)
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
        {
          return 0;
        }
    }
  ACE_CATCHANY
    {
      return 0;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);
}

CORBA::Boolean
CORBA_Any::operator>>= (CORBA::UShort &s) const
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      CORBA::Boolean result =
        this->type_->equivalent (CORBA::_tc_ushort,
                                 ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (result)
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
        {
          return 0;
        }
    }
  ACE_CATCHANY
    {
      return 0;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);
}

CORBA::Boolean
CORBA_Any::operator>>= (CORBA::Long &l) const
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      CORBA::Boolean result =
        this->type_->equivalent (CORBA::_tc_long,
                                 ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (result)
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
        {
          return 0;
        }
    }
  ACE_CATCHANY
    {
      return 0;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);
}

CORBA::Boolean
CORBA_Any::operator>>= (CORBA::ULong &l) const
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      CORBA::Boolean result =
        this->type_->equivalent (CORBA::_tc_ulong,
                                 ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (result)
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
        {
          return 0;
        }
    }
  ACE_CATCHANY
    {
      return 0;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);
}

CORBA::Boolean
CORBA_Any::operator>>= (CORBA::LongLong &l) const
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      CORBA::Boolean result =
        this->type_->equivalent (CORBA::_tc_longlong,
                                 ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (result)
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
        {
          return 0;
        }
    }
  ACE_CATCHANY
    {
      return 0;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);
}

CORBA::Boolean
CORBA_Any::operator>>= (CORBA::ULongLong &l) const
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      CORBA::Boolean result =
        this->type_->equivalent (CORBA::_tc_ulonglong,
                                 ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (result)
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
        {
          return 0;
        }
    }
  ACE_CATCHANY
    {
      return 0;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);
}

CORBA::Boolean
CORBA_Any::operator>>= (CORBA::Float &f) const
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      CORBA::Boolean result =
        this->type_->equivalent (CORBA::_tc_float,
                                 ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (result)
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
        {
          return 0;
        }
    }
  ACE_CATCHANY
    {
      return 0;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);
}

CORBA::Boolean
CORBA_Any::operator>>= (CORBA::Double &d) const
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      CORBA::Boolean result =
        this->type_->equivalent (CORBA::_tc_double,
                                 ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (result)
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
        {
          return 0;
        }
    }
  ACE_CATCHANY
    {
      return 0;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);
}

CORBA::Boolean
CORBA_Any::operator>>= (CORBA::Any &a) const
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      CORBA::Boolean result =
        this->type_->equivalent (CORBA::_tc_any,
                                 ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (result)
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
              CORBA::TypeCode::traverse_status retval =
                stream.decode (CORBA::_tc_any,
                               &a,
                               0,
                               ACE_TRY_ENV);
             return (retval == CORBA::TypeCode::TRAVERSE_CONTINUE) ? 1 : 0;
            }
        }
      else
        {
          return 0;
        }
    }
  ACE_CATCHANY
    {
      return 0;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);
}

CORBA::Boolean
CORBA_Any::operator>>= (char *&s) const
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      CORBA::Boolean result =
        this->type_->equivalent (CORBA::_tc_string,
                                 ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (result)
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
                  ACE_const_cast (CORBA_Any *,
                                  this)->any_owns_data_ = 1;

                  char **tmp;

                  ACE_NEW_RETURN (tmp,
                                  char *,
                                  0);

                  *tmp = s;

                  ACE_const_cast (CORBA_Any *,
                                  this)->value_ = tmp;

                  return 1;
                }
              return 0;
            }
        }
      else
        {
          return 0;
        }
    }
  ACE_CATCHANY
    {
      return 0;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);
}

CORBA::Boolean
CORBA_Any::operator>>= (CORBA::TypeCode_ptr &tc) const
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      CORBA::Boolean result =
        this->type_->equivalent (CORBA::_tc_TypeCode,
                                 ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (result)
        {
          if (this->any_owns_data_ && this->value_)
            {
              tc = *(CORBA::TypeCode_ptr *) this->value_;
              return 1;
            }
          else
            {
              TAO_InputCDR stream ((ACE_Message_Block *) this->cdr_);
              CORBA::TypeCode::traverse_status retval =
                stream.decode (CORBA::_tc_TypeCode,
                               &tc,
                               0,
                               ACE_TRY_ENV);
              ACE_TRY_CHECK;
              return (retval == CORBA::TypeCode::TRAVERSE_CONTINUE) ? 1 : 0;
            }
        }
      else
        {
          return 0;
        }
    }
  ACE_CATCHANY
    {
      return 0;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);
}

// = extraction into the special types

CORBA::Boolean
CORBA_Any::operator>>= (to_boolean b) const
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      CORBA::Boolean result =
        this->type_->equivalent (CORBA::_tc_boolean,
                                 ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (result)
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
        {
          return 0;
        }
    }
  ACE_CATCHANY
    {
      return 0;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);
}

CORBA::Boolean
CORBA_Any::operator>>= (to_octet o) const
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      CORBA::Boolean result =
        this->type_->equivalent (CORBA::_tc_octet,
                                 ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (result)
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
        {
          return 0;
        }
    }
  ACE_CATCHANY
    {
      return 0;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);
}

CORBA::Boolean
CORBA_Any::operator>>= (to_char c) const
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      CORBA::Boolean result =
        this->type_->equivalent (CORBA::_tc_char,
                                 ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (result)
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
        {
          return 0;
        }
    }
  ACE_CATCHANY
    {
      return 0;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);
}

CORBA::Boolean
CORBA_Any::operator>>= (to_wchar wc) const
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      CORBA::Boolean result =
        this->type_->equivalent (CORBA::_tc_wchar,
                                 ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (result)
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
        {
          return 0;
        }
    }
  ACE_CATCHANY
    {
      return 0;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);
}

CORBA::Boolean
CORBA_Any::operator>>= (to_string s) const
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      CORBA::Boolean result =
        this->type_->equivalent (CORBA::_tc_string,
                                 ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (result)
        {
          CORBA::ULong bound = this->type_->length (ACE_TRY_ENV);
          ACE_TRY_CHECK;
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
                      ACE_const_cast (CORBA_Any *,
                                      this)->any_owns_data_ = 1;
                      return 1;
                    }
                  else
                    {
                      return 0;
                    }
                }
            }
          else
            {
              return 0;
            }
        }
      else
        {
          return 0;
        }
    }
  ACE_CATCHANY
    {
      return 0;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);
}

CORBA::Boolean
CORBA_Any::operator>>= (to_object obj) const
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      CORBA::Boolean result =
        this->type_->equivalent (CORBA::_tc_Object,
                                 ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (result)
        {
          if (this->any_owns_data_ && this->value_)
            {
              // CORBA 2.3 has changed the behavior of this operator. Caller
              // is now responsible for release.
              obj.ref_ =
                CORBA::Object::_duplicate (*(CORBA::Object_ptr *) this->value_);

              return 1;
            }
          else
            {
              // @@ This uses ORB_Core instance because we need one to
              // demarshal objects (to create the right profiles for that
              // object), but the Any does not belong to any ORB.
              TAO_InputCDR stream (this->cdr_,
                                  ACE_CDR_BYTE_ORDER,
                                  TAO_ORB_Core_instance ());

              CORBA::TypeCode::traverse_status status =
                stream.decode (CORBA::_tc_Object,
                               &obj.ref_,
                               0,
                               ACE_TRY_ENV);
              ACE_TRY_CHECK;

              // Because of the CORBA 2.3 change mentioned above, there is no
              // need to assign to this->value_.
              return (status == CORBA::TypeCode::TRAVERSE_CONTINUE) ? 1 : 0;;
            }
        }
      else
        {
          return 0;
        }
    }
  ACE_CATCHANY
    {
      return 0;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);
}

// this is a copying version for unbounded strings Not inline, to
// avoid use in Any.i before definition in ORB.i.

void
CORBA_Any::operator<<= (const char* s)
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      char **tmp;

      ACE_NEW (tmp,
               char * (CORBA::string_dup (s)));

      this->replace (CORBA::_tc_string,
                     tmp,
                     1,
                     ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      // do nothing
    }
  ACE_ENDTRY;
  ACE_CHECK;
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
  CORBA_Any_ptr tmp;

  ACE_NEW_RETURN (tmp,
                  CORBA::Any (*r.ptr_),
                  *this);

  if (this->ptr_ != 0)
    delete this->ptr_;

  this->ptr_ = tmp;

  return *this;
}

// = Debugging method.

// Prints the type and the value of the any value. Dumping is
// supported only for standard data types.

void
CORBA_Any::dump (const CORBA::Any &any_value)
{
  // Get the typecode kind.
  CORBA::TypeCode_var type = any_value.type ();
  CORBA::TCKind kind = type->kind ();

  switch (kind)
    {
      case CORBA::tk_null:
        ACE_DEBUG ((LM_DEBUG,
                    "Null\n"));
        break;
      case CORBA::tk_void:
        ACE_DEBUG ((LM_DEBUG,
                    "Void\n"));
        break;
      case CORBA::tk_short:
        {
          CORBA::Short s;
          any_value >>= s;
          ACE_DEBUG ((LM_DEBUG,
                      "Short %d\n",
                     s));
          break;
        }
      case CORBA::tk_long:
        {
          CORBA::Long l;
          any_value >>= l;
          ACE_DEBUG ((LM_DEBUG,
                      "Long %d\n",
                      l));
          break;
        }
      case CORBA::tk_ushort:
        {
          CORBA::UShort s;
          any_value >>= s;
          ACE_DEBUG ((LM_DEBUG,
                      "UShort %d\n",
                      s));
          break;
        }
      case CORBA::tk_ulong:
        {
          CORBA::ULong l;
          any_value >>= l;
          ACE_DEBUG ((LM_DEBUG,
                      "ULong %d\n",
                      l));
          break;
        }
      case CORBA::tk_float:
        {
          CORBA::Float f;
          any_value >>= f;
          ACE_DEBUG ((LM_DEBUG,
                      "Float %f\n",
                      f));
          break;
        }
      case CORBA::tk_double:
        {
          CORBA::Double d;
          any_value >>= d;
          ACE_DEBUG ((LM_DEBUG,
                      "Double %f\n",
                      d));
          break;
        }
      case CORBA::tk_boolean:
        {
          CORBA::Boolean b;
          to_boolean tb (b);
          any_value >>= tb;
          if (b == 1)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "Boolean TRUE\n"));
            }
          else
            {
              ACE_DEBUG ((LM_DEBUG,
                          "Boolean FALSE\n"));
            }
          break;
        }
      case CORBA::tk_char:
        {
          CORBA::Char ch;
          to_char tch (ch);
          any_value >>= tch;
          ACE_DEBUG ((LM_DEBUG,
                      "Char %c\n",
                      ch));
          break;
        }
      case CORBA::tk_string:
        {
          CORBA::String str;
          any_value >>= str;
          ACE_DEBUG ((LM_DEBUG,
                      "String %s\n",
                      str));
          break;
        }
      default:
        ACE_DEBUG ((LM_DEBUG,
                    "TCKind %d\n",
                    kind));
        break;
    }
}
