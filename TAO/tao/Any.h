// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    Any.h
//
// = AUTHOR
//     Copyright 1994-1995 by Sun Microsystems, Inc.
//     and Aniruddha Gokhale.
//
// ============================================================================

#ifndef TAO_ANY_H
#define TAO_ANY_H

#include "tao/CDR.h"
#include "tao/Environment.h"

class TAO_Export CORBA_Any
{
  // = TITLE
  //   Class "Any" can wrap values of any type, with the assistance
  //   of a TypeCode to describe that type.
  //
  // = DESCRIPTION
  //   This includes three constructors, a destructor, and a "replace"
  //   method for the "Any" data type.  "Any" values pair a pointer to
  //   a data structure in the native binary representation (e.g. C
  //   struct) with a TypeCode that describes that data structure.
  //
  //   The copy constructor and the destructor each use the TypeCode
  //   interpreter with specialized "visit" callback routines.  The
  //   "visit" routines are used respectively to make "deep copies"
  //   and perform "deep frees" of the aritrary values as described by
  //   the "Any" value's typecode.
  //
  //   Note that these "visit" routines are called directly, and they
  //   choose whether or not to use the TypeCode interpreter to
  //   examine constituents.  In the simple cases, the "visit"
  //   routines can do their work without any further calls; only for
  //   constructed types is the interpreter's knowledge really
  //   required.
  //
  //   THREADING NOTE: "Any" is a data structure which must be
  //   protected by external critical sections.  Like simpler numeric
  //   types, "Any" instances are accessed and modified atomically.
  //   This implementation is reentrant, so that independent "Any"
  //   values may be manipulated concurrently when the underlying
  //   programming environment is itself reentrant.
public:

  // = Minor codes for exceptional returns
  enum
  {
    UNINITIALIZED_type = 0xf000,
    VALUE_WITHOUT_TYPE,
    UNSUPPORTED_OPERATION
  };

  // = Initialization and termination operations.

  CORBA_Any (void);
  // Default constructor.

  CORBA_Any (CORBA::TypeCode_ptr type,
             void *value = 0,
             CORBA::Boolean any_owns_data = 0);
  // Constructor. The any_owns_data flag determines if the Any owns the value

  // = TAO extension
  CORBA_Any (CORBA::TypeCode_ptr type,
             const ACE_Message_Block* mb);
  // Constructor. Used by DynAny to compose/decompose
  // complex types using a CDR.

  CORBA_Any (const CORBA_Any &a);
  // Copy constructor.

  ~CORBA_Any (void);
  // Destructor.

  CORBA_Any &operator= (const CORBA_Any &);
  // assignment operator

  // = NOTE: 94-9-14 has assignment operator plus many insertion, as
  // specified below.

  // =type safe insertion

  void operator<<= (CORBA::Short);
  // insert a short

  void operator<<= (CORBA::UShort);
  // insert an unsigned short

  void operator<<= (CORBA::Long);
  // insert a long

  void operator<<= (CORBA::ULong);
  // insert an unsigned long

  void operator<<= (CORBA::LongLong);
  // insert a long long

  void operator<<= (CORBA::ULongLong);
  // insert an unsigned long long

  void operator<<= (CORBA::Float);
  // insert a float

  void operator<<= (CORBA::Double);
  // insert a double

  void operator<<= (const CORBA_Any&);
  // insert an Any

  void operator<<= (const char*);
  // insert unbounded strings

  void operator<<= (CORBA::TypeCode_ptr);
  // insert a TypeCode

  void operator<<= (const CORBA::Object_ptr);
  // insert an object reference, copying.

  void operator<<= (CORBA::Object_ptr *);
  // insert an object reference, non-copying.  Any assume the ownership
  // of the object.

  // =type safe extraction

  CORBA::Boolean operator>>= (CORBA::Short&) const;
  // extract a short

  CORBA::Boolean operator>>= (CORBA::UShort&) const;
  // extract an unsigned short

  CORBA::Boolean operator>>= (CORBA::Long&) const;
  // extract a long

  CORBA::Boolean operator>>= (CORBA::ULong&) const;
  // extract an unsigned long

  CORBA::Boolean operator>>= (CORBA::LongLong&) const;
  // extract a long long

  CORBA::Boolean operator>>= (CORBA::ULongLong&) const;
  // extract an unsigned long long

  CORBA::Boolean operator>>= (CORBA::Float&) const;
  // extract a float

  CORBA::Boolean operator>>= (CORBA::Double&) const;
  // extract a double

  CORBA::Boolean operator>>= (CORBA_Any&) const;
  // extract an Any

  CORBA::Boolean operator>>= (CORBA::TypeCode_ptr&) const;
  // extract a TypeCode

  CORBA::Boolean operator>>= (char*&) const;
  // extract an unbounded string

  // = Special types.

  // These are needed for insertion and extraction of booleans,
  // octets, chars, and bounded strings. CORBA spec requires
  // that they be here, we just typedef to the already-defined
  // ACE_OutputCDR types.

  typedef ACE_OutputCDR::from_boolean from_boolean;
  typedef ACE_OutputCDR::from_octet from_octet;
  typedef ACE_OutputCDR::from_char from_char;
  typedef ACE_OutputCDR::from_wchar from_wchar;
  typedef ACE_OutputCDR::from_string from_string;

  void operator<<= (from_boolean);
  // insert a boolean

  void operator<<= (from_char);
  // insert a char

  void operator<<= (from_wchar);
  // insert a wchar

  void operator<<= (from_octet);
  // insert an octet

  void operator<<= (from_string);
  // insert a bounded string

  // = Special types.

  // These extract octets, chars, booleans, bounded strings, and
  // object references. All these are defined in ACE_InputCDR.

  typedef ACE_InputCDR::to_boolean to_boolean;
  typedef ACE_InputCDR::to_char to_char;
  typedef ACE_InputCDR::to_wchar to_wchar;
  typedef ACE_InputCDR::to_octet to_octet;
  typedef ACE_InputCDR::to_string to_string;

  // This one's not in ACE.
  struct TAO_Export to_object
  {
    to_object (CORBA::Object_ptr &obj);
    CORBA::Object_ptr &ref_;
  };

  // extraction of the special types

  CORBA::Boolean operator>>= (to_boolean) const;
  // extract a boolean

  CORBA::Boolean operator>>= (to_octet) const;
  // extract an octet

  CORBA::Boolean operator>>= (to_char) const;
  // extract a char

  CORBA::Boolean operator>>= (to_wchar) const;
  // extract a wchar

  CORBA::Boolean operator>>= (to_string) const;
  // extract a bounded string

  CORBA::Boolean operator>>= (to_object) const;
  // extract an object reference

  // the following are unsafe operations
  // ORBOS/90-01-11, pg 672: For C++ mapping using the CORBA_Environment
  // parameter, two forms of the replace method are provided.

  void replace (CORBA::TypeCode_ptr type,
                const void *value,
                CORBA::Boolean any_owns_data,
                CORBA_Environment &TAO_IN_ENV =
                  CORBA::default_environment ());
  // Replace the current typecode and data with the specified one -
  // unsafe.

  void replace (CORBA::TypeCode_ptr type,
                const void *value,
                CORBA_Environment &TAO_IN_ENV =
                  CORBA::default_environment ());
  // Replace the current typecode and data with the specified one -
  // unsafe. This uses a default value for the "any_owns_data" parameter

  CORBA::TypeCode_ptr type (void) const;
  // Return TypeCode of the element stored in the Any.

  void type (CORBA::TypeCode_ptr type,
             CORBA_Environment &TAO_IN_ENV =
               CORBA::default_environment ());
  // For use along with <<= of a value of aliased type when the alias must
  // be preserved.

  const void *value (void) const;
  // Returns 0 if the Any has not been assigned a value, following the
  // CORBA spec (ORBOS/98-01-11) it returns a non-zero value
  // otherwise. TAO does *not* guarantee that this value may be casted
  // to the contained type safely.

  // = Debugging method.

  static void dump (const CORBA::Any any_value);
  // Prints the type and the value of the any value. Dumping is
  // supported only for standard data types.

  // = TAO extensions
  CORBA::Boolean any_owns_data (void) const;
  // does the Any own the data or not. This is used by the >>=
  // operators generated by the IDL compiler. The >>= operator checks
  // if the Any owns the data. If it does, then it will simply
  // retrieve the data from internal cache.  Otherwise, the operator
  // will have to decode the cdr string.

  ACE_Message_Block* _tao_get_cdr (void) const;
  // Message block accessor.

  void _tao_replace (CORBA::TypeCode_ptr,
                     const ACE_Message_Block *mb,
                     CORBA::Boolean any_owns_data,
                     CORBA::Environment &TAO_IN_ENV =
                       CORBA::default_environment ());
  // Replace via message block instead of <value_>.

protected:
  void free_value (CORBA::Environment &TAO_IN_ENV);
  // Release the <value_>.

private:
  CORBA::TypeCode_ptr type_;
  // Typecode for the <Any>.

  void *value_;
  // Value for the <Any>.

  ACE_Message_Block *cdr_;
  // encoded value.

  CORBA::Boolean any_owns_data_;
  // Flag that indicates the ORB is responsible for deleting the data.

  // 94-9-14 hides unsigned char insert/extract
  void operator<<= (unsigned char);
  CORBA::Boolean operator>>= (unsigned char&) const;

  friend class STUB_Object;
  friend class TAO_Marshal_Any;
  friend class CORBA_NVList;
};

class TAO_Export CORBA_Any_var
{
  // = TITLE
  //   Provide for automatic storage deallocation on going out of
  //   scope.
public:
  CORBA_Any_var (void);
  // default constructor

  CORBA_Any_var (CORBA_Any *a);
  // construct from an Any pointer

  CORBA_Any_var (const CORBA_Any_var &a);
  // copy constructor

  ~CORBA_Any_var (void);
  // destructor

  CORBA_Any_var &operator= (CORBA_Any *a);
  // assignment from a pointer to Any

  CORBA_Any_var &operator= (const CORBA_Any_var &a);
  // assignment from an Any_var

  CORBA_Any *operator-> (void);
  // arrow operator (smart pointer)

  operator const CORBA_Any *() const;
  // cast

  operator CORBA_Any *&();
  // cast

  const CORBA_Any &in (void) const;
  // for in Any parameter

  CORBA_Any &inout (void);
  // for inout Any parameter

  CORBA_Any *&out (void);
  // for out Any parameter

  CORBA_Any *_retn (void);
  // for Any return types

private:
  CORBA_Any *ptr_;
  // Holds the Any.
};

class TAO_Export CORBA_Any_out
{
  // = TITLE
  //   CORBA_Any_out
  //
  // = DESCRIPTION
  //   The _out class for CORBA_Any. This is used to help in
  //   managing the out parameters.
public:
  // = operations.

  CORBA_Any_out (CORBA_Any *&p);
  // construction from a reference to a CORBA_Any

  CORBA_Any_out (CORBA_Any_var &p);
  // construction from a var

  CORBA_Any_out (const CORBA_Any_out &s);
  // copy constructor

  CORBA_Any_out &operator= (const CORBA_Any_out &s);
  // assignment from a CORBA_Any_out

  CORBA_Any_out &operator= (CORBA_Any *p);
  // assignment from a CORBA_Any

  CORBA_Any_out &operator= (const CORBA_Any *p);
  // assignment from a const CORBA_Any

  operator CORBA_Any *&();
  // cast

  CORBA_Any *& ptr (void);
  // return underlying instance

private:
  CORBA_Any *&ptr_;
  // Instance

  void operator= (const CORBA_Any_var &);
  // assignment from _var disallowed
};

#if defined (__ACE_INLINE__)
# include "tao/Any.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_ANY_H */
