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
#include "ace/pre.h"
#define TAO_ANY_H

#include "tao/CDR.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Environment.h"
#include "tao/Object.h"

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
             CORBA::UShort dummy,
             int byte_order,
             const ACE_Message_Block* mb);
  // Constructor. Used by DynAny and others to optimize Any activities
  // by using CDR. The dummy arg is to keep calls like CORBA_Any foo
  // (CORBA::TypeCode_ptr bar (NULL), NULL) from being confused with
  // the constructor above.

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

  void operator<<= (CORBA::LongDouble);
  // insert a long double

  void operator<<= (const CORBA_Any&);
  // insert an Any, copying

  void operator<<= (CORBA_Any_ptr);
  // insert an Any, non-copying

  void operator<<= (const char*);
  // insert unbounded string

  void operator<<= (const CORBA::WChar*);
  // insert unbounded wide string

  void operator<<= (CORBA::TypeCode_ptr);
  // insert a TypeCode

  void operator<<= (const CORBA::Object_ptr);
  // insert an object reference, copying.

  void operator<<= (CORBA::Object_ptr *);
  // insert an object reference, non-copying. Any assumes the ownership
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

  CORBA::Boolean operator>>= (CORBA::LongDouble&) const;
  // extract a long double

  CORBA::Boolean operator>>= (CORBA_Any&) const; // non-spec
  CORBA::Boolean operator>>= (const CORBA_Any*&) const;
  // extract an Any

  CORBA::Boolean operator>>= (CORBA::TypeCode_ptr&) const;
  // extract a TypeCode

  CORBA::Boolean operator>>= (char*&) const; // deperecated
  CORBA::Boolean operator>>= (const char*&) const;
  // extract an unbounded string

  CORBA::Boolean operator>>= (CORBA::WChar*&) const; // deprecated
  CORBA::Boolean operator>>= (const CORBA::WChar*&) const;
  // extract an unbounded wide string

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
  typedef ACE_OutputCDR::from_wstring from_wstring;

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

  void operator<<= (from_wstring);
  // insert a bounded wide string

  void operator<<= (const CORBA_Exception &exception);
  // insert an exception into the Any

  // = Special types.

  // These extract octets, chars, booleans, bounded strings, and
  // object references. All these are defined in ACE_InputCDR.

  typedef ACE_InputCDR::to_boolean to_boolean;
  typedef ACE_InputCDR::to_char to_char;
  typedef ACE_InputCDR::to_wchar to_wchar;
  typedef ACE_InputCDR::to_octet to_octet;
  typedef ACE_InputCDR::to_string to_string;
  typedef ACE_InputCDR::to_wstring to_wstring;

  // This one's not in ACE.
  struct TAO_Export to_object
  {
    // This signature reflects the change set out in
    // issue 154 of the 1.3 RTF.
    to_object (CORBA_Object_out obj);
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

  CORBA::Boolean operator>>= (to_wstring) const;
  // extract a bounded wide string

  CORBA::Boolean operator>>= (to_object) const;
  // extract an object reference

  // the following are unsafe operations
  // ORBOS/90-01-11, pg 672: For C++ mapping using the CORBA_Environment
  // parameter, two forms of the replace method are provided.

  void replace (CORBA::TypeCode_ptr type,
                const void *value,
                CORBA::Boolean any_owns_data,
                CORBA_Environment &ACE_TRY_ENV =
                  TAO_default_environment ());
  // Replace the current typecode and data with the specified one -
  // unsafe.

  void replace (CORBA::TypeCode_ptr type,
                const void *value,
                CORBA_Environment &ACE_TRY_ENV =
                  TAO_default_environment ());
  // Replace the current typecode and data with the specified one -
  // unsafe. This uses a default value for the "any_owns_data" parameter

  CORBA::TypeCode_ptr type (void) const;
  // Return TypeCode of the element stored in the Any.

  void type (CORBA::TypeCode_ptr type,
             CORBA_Environment &ACE_TRY_ENV =
               TAO_default_environment ());
  // For use along with <<= of a value of aliased type when the alias must
  // be preserved.

  const void *value (void) const;
  // Returns 0 if the Any has not been assigned a value, following the
  // CORBA spec (ORBOS/98-01-11) it returns a non-zero value
  // otherwise. TAO does *not* guarantee that this value may be casted
  // to the contained type safely.

  // = TAO extensions
  CORBA::Boolean any_owns_data (void) const;
  // Reports whether the Any own the data or not. This is used by the
  // >>= operators generated by the IDL compiler. The >>= operator
  // checks if the Any owns the data. If it does, then it will simply
  // retrieve the data from internal cache.  Otherwise, the operator
  // will have to decode the cdr string.

  ACE_Message_Block* _tao_get_cdr (void) const;
  // Message block accessor.
  int _tao_byte_order (void) const;
  // Get the byte order inside the CDR stream.

  void _tao_replace (CORBA::TypeCode_ptr,
                     int byte_order,
                     const ACE_Message_Block *mb,
                     CORBA::Environment &ACE_TRY_ENV);
  // Replace via message block instead of <value_>.

  void _tao_replace (CORBA::TypeCode_ptr type,
                     int byte_order,
                     const ACE_Message_Block *mb,
                     CORBA::Boolean any_owns_data,
                     void* value,
                     CORBA::Environment &ACE_TRY_EN);
  // Replace all the contents of the any, used in the <<= operators.

  void _tao_replace (CORBA::TypeCode_ptr type,
                     CORBA::Boolean any_owns_data,
                     void* value,
                     CORBA::Environment &ACE_TRY_ENV);
  // Replace the value of the Any, used in the >>= operators.

  void _tao_encode (TAO_OutputCDR &cdr,
                    TAO_ORB_Core *orb_core,
                    CORBA::Environment &ACE_TRY_ENV);
  // Encode the contents of the Any into <cdr>

  void _tao_decode (TAO_InputCDR &cdr,
                    CORBA::Environment &ACE_TRY_ENV);
  // Decode the <cdr> using the typecode in the Any object.

#if !defined(__GNUC__) || __GNUC__ > 2 || __GNUC_MINOR__ >= 8
  typedef CORBA_Any_ptr _ptr_type;
  typedef CORBA_Any_var _var_type;
#endif /* __GNUC__ */
  // Useful for template programming.

protected:
  void free_value (CORBA::Environment &ACE_TRY_ENV);
  // Release the <value_>.

private:
  CORBA::TypeCode_ptr type_;
  // Typecode for the <Any>.

  void *value_;
  // Value for the <Any>.

  int byte_order_;
  ACE_Message_Block *cdr_;
  // encoded value.

  CORBA::Boolean any_owns_data_;
  // Flag that indicates the ORB is responsible for deleting the data.

  // 94-9-14 hides unsigned char insert/extract
  void operator<<= (unsigned char);
  CORBA::Boolean operator>>= (unsigned char&) const;

  friend class CORBA_NVList;
  friend class TAO_Marshal_Any;
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
  // This operation requires memory allocation.
  // If the allocation fails, *this is returned
  // unmodified.

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

  CORBA_Any *operator-> (void);

private:
  CORBA_Any *&ptr_;
  // Instance

  void operator= (const CORBA_Any_var &);
  // assignment from _var disallowed
};

// These operators are too complex to be inline....
TAO_Export CORBA::Boolean operator<< (TAO_OutputCDR& cdr,
                                      const CORBA::Any &x);
TAO_Export CORBA::Boolean operator>> (TAO_InputCDR& cdr,
                                      CORBA::Any &x);

#if defined (__ACE_INLINE__)
# include "tao/Any.i"
#else

// Copying versions of insertion
// operators which are defined as members of the Any class
// must also be defined for Any_var.
TAO_Export void operator<<= (CORBA_Any_var,
                             CORBA::Short);
TAO_Export void operator<<= (CORBA_Any_var,
                             CORBA::UShort);
TAO_Export void operator<<= (CORBA_Any_var,
                             CORBA::Long);
TAO_Export void operator<<= (CORBA_Any_var,
                             CORBA::ULong);
TAO_Export void operator<<= (CORBA_Any_var,
                             CORBA::LongLong);
TAO_Export void operator<<= (CORBA_Any_var,
                             CORBA::ULongLong);
TAO_Export void operator<<= (CORBA_Any_var,
                             CORBA::Float);
TAO_Export void operator<<= (CORBA_Any_var,
                             CORBA::Double);
TAO_Export void operator<<= (CORBA_Any_var,
                             const CORBA_Any&);
TAO_Export void operator<<= (CORBA_Any_var,
                             const char*);
TAO_Export void operator<<= (CORBA_Any_var,
                             CORBA::TypeCode_ptr);
TAO_Export void operator<<= (CORBA_Any_var,
                             const CORBA::Object_ptr);
TAO_Export void operator<<= (CORBA_Any_var,
                             CORBA::Any::from_boolean);
TAO_Export void operator<<= (CORBA_Any_var,
                             CORBA::Any::from_char);
TAO_Export void operator<<= (CORBA_Any_var,
                             CORBA::Any::from_wchar);
TAO_Export void operator<<= (CORBA_Any_var,
                             CORBA::Any::from_octet);
TAO_Export void operator<<= (CORBA_Any_var,
                             CORBA::Any::from_string);

// These are not required by the spec, but will make users
// of other ORBs that are used to them more comfortable.
TAO_Export CORBA::Boolean operator>>= (CORBA_Any_var,
                                       CORBA::Short&);
TAO_Export CORBA::Boolean operator>>= (CORBA_Any_var,
                                       CORBA::UShort&);
TAO_Export CORBA::Boolean operator>>= (CORBA_Any_var,
                                       CORBA::Long&);
TAO_Export CORBA::Boolean operator>>= (CORBA_Any_var,
                                       CORBA::ULong&);
TAO_Export CORBA::Boolean operator>>= (CORBA_Any_var,
                                       CORBA::LongLong&);
TAO_Export CORBA::Boolean operator>>= (CORBA_Any_var,
                                       CORBA::ULongLong&);
TAO_Export CORBA::Boolean operator>>= (CORBA_Any_var,
                                       CORBA::Float&);
TAO_Export CORBA::Boolean operator>>= (CORBA_Any_var,
                                       CORBA::Double&);
TAO_Export CORBA::Boolean operator>>= (CORBA_Any_var,
                                       CORBA_Any&);
TAO_Export CORBA::Boolean operator>>= (CORBA_Any_var,
                                       CORBA::TypeCode_ptr&);
TAO_Export CORBA::Boolean operator>>= (CORBA_Any_var,
                                       char*&);
TAO_Export CORBA::Boolean operator>>= (CORBA_Any_var,
                                       CORBA::Any::to_boolean);
TAO_Export CORBA::Boolean operator>>= (CORBA_Any_var,
                                       CORBA::Any::to_octet);
TAO_Export CORBA::Boolean operator>>= (CORBA_Any_var,
                                       CORBA::Any::to_char);
TAO_Export CORBA::Boolean operator>>= (CORBA_Any_var,
                                       CORBA::Any::to_wchar);
TAO_Export CORBA::Boolean operator>>= (CORBA_Any_var,
                                       CORBA::Any::to_string);
TAO_Export CORBA::Boolean operator>>= (CORBA_Any_var,
                                       CORBA::Any::to_object);

#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_ANY_H */
