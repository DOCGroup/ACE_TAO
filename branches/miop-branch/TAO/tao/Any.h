// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file    Any.h
 *
 *  $Id$
 *
 *  @author  Copyright 1994-1995 by Sun Microsystems
 *  @author Inc.  and Aniruddha Gokhale.
 */
//=============================================================================


#ifndef TAO_ANY_H
#define TAO_ANY_H
#include "ace/pre.h"

#include "tao/CDR.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Environment.h"
#include "tao/Object.h"
#include "tao/Typecode.h"

/**
 * @class CORBA_Any
 *
 * @brief Class "Any" can wrap values of any type, with the assistance
 * of a TypeCode to describe that type.
 *
 * This includes three constructors, a destructor, and a "replace"
 * method for the "Any" data type.  "Any" values pair a pointer to
 * a data structure in the native binary representation (e.g. C
 * struct) with a TypeCode that describes that data structure.
 * The copy constructor and the destructor each use the TypeCode
 * interpreter with specialized "visit" callback routines.  The
 * "visit" routines are used respectively to make "deep copies"
 * and perform "deep frees" of the aritrary values as described by
 * the "Any" value's typecode.
 * Note that these "visit" routines are called directly, and they
 * choose whether or not to use the TypeCode interpreter to
 * examine constituents.  In the simple cases, the "visit"
 * routines can do their work without any further calls; only for
 * constructed types is the interpreter's knowledge really
 * required.
 * THREADING NOTE: "Any" is a data structure which must be
 * protected by external critical sections.  Like simpler numeric
 * types, "Any" instances are accessed and modified atomically.
 * This implementation is reentrant, so that independent "Any"
 * values may be manipulated concurrently when the underlying
 * programming environment is itself reentrant.
 */
class TAO_Export CORBA_Any
{
public:

  // = Minor codes for exceptional returns
  enum
  {
    UNINITIALIZED_type = 0xf000,
    VALUE_WITHOUT_TYPE,
    UNSUPPORTED_OPERATION
  };

  // = Initialization and termination operations.

  /// Default constructor.
  CORBA_Any (void);

  /// Constructor.
  CORBA_Any (CORBA::TypeCode_ptr type);

  // = TAO extension
  /**
   * Constructor. Used by DynAny and others to optimize Any activities
   * by using CDR. The dummy arg is to keep calls like CORBA_Any foo
   * (CORBA::TypeCode_ptr bar (NULL), NULL) from being confused with
   * the constructor above.
   */
  CORBA_Any (CORBA::TypeCode_ptr type,
             CORBA::UShort dummy,
             int byte_order,
             const ACE_Message_Block* mb);

  /// Copy constructor.
  CORBA_Any (const CORBA_Any &a);

  /// Destructor.
  ~CORBA_Any (void);

  /// assignment operator
  CORBA_Any &operator= (const CORBA_Any &);

  // = NOTE: 94-9-14 has assignment operator plus many insertion, as
  // specified below.

  // =type safe insertion

  /// Insert a short.
  void operator<<= (CORBA::Short);

  /// Insert an unsigned short.
  void operator<<= (CORBA::UShort);

  /// Insert a long.
  void operator<<= (CORBA::Long);

  /// Insert an unsigned long.
  void operator<<= (CORBA::ULong);

  /// Insert a long long.
  void operator<<= (CORBA::LongLong);

  /// Insert an unsigned long long.
  void operator<<= (CORBA::ULongLong);

  /// Insert a float.
  void operator<<= (CORBA::Float);

  /// Insert a double.
  void operator<<= (CORBA::Double);

  /// Insert a long double.
  void operator<<= (CORBA::LongDouble);

  /// Insert an Any, copying.
  void operator<<= (const CORBA_Any&);

  /// Insert an Any, non-copying.
  void operator<<= (CORBA_Any_ptr);

  /// Insert unbounded string
  void operator<<= (const char*);

  /// Insert unbounded wide string.
  void operator<<= (const CORBA::WChar*);

  /// Insert a TypeCode.
  void operator<<= (CORBA::TypeCode_ptr);

  /// Insert an object reference, copying.
  void operator<<= (const CORBA::Object_ptr);

  /// Insert an object reference, non-copying. Any assumes the ownership
  /// of the object.
  void operator<<= (CORBA::Object_ptr *);

  // =Type safe extraction.

  /// Extract a short.
  CORBA::Boolean operator>>= (CORBA::Short&) const;

  /// Extract an unsigned short.
  CORBA::Boolean operator>>= (CORBA::UShort&) const;

  /// Extract a long.
  CORBA::Boolean operator>>= (CORBA::Long&) const;

  /// Extract an unsigned long.
  CORBA::Boolean operator>>= (CORBA::ULong&) const;

  /// Extract a long long.
  CORBA::Boolean operator>>= (CORBA::LongLong&) const;

  /// Extract an unsigned long long.
  CORBA::Boolean operator>>= (CORBA::ULongLong&) const;

  /// Extract a float.
  CORBA::Boolean operator>>= (CORBA::Float&) const;

  /// Extract a double.
  CORBA::Boolean operator>>= (CORBA::Double&) const;

  /// Extract a long double.
  CORBA::Boolean operator>>= (CORBA::LongDouble&) const;

  /// Extract an Any.
  CORBA::Boolean operator>>= (CORBA_Any&) const; // non-spec
  CORBA::Boolean operator>>= (const CORBA_Any*&) const;

  /// Extract a TypeCode.
  CORBA::Boolean operator>>= (CORBA::TypeCode_ptr&) const;

  /// Extract an unbounded string
  CORBA::Boolean operator>>= (const char*&) const;

  /// Extract an unbounded wide string.
  CORBA::Boolean operator>>= (const CORBA::WChar*&) const;

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

  /// Insert a boolean.
  void operator<<= (from_boolean);

  /// Insert a char.
  void operator<<= (from_char);

  /// Insert a wchar.
  void operator<<= (from_wchar);

  /// Insert an octet.
  void operator<<= (from_octet);

  /// Insert a bounded string.
  void operator<<= (from_string);

  /// Insert a bounded wide string.
  void operator<<= (from_wstring);

  /// Insert an exception into the Any (copying)
  void operator<<= (const CORBA_Exception &exception);

  /// Insert an exception into the Any (non-copying).
  void operator<<= (CORBA_Exception *exception);

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

  /// extract a boolean
  CORBA::Boolean operator>>= (to_boolean) const;

  /// extract an octet
  CORBA::Boolean operator>>= (to_octet) const;

  /// extract a char
  CORBA::Boolean operator>>= (to_char) const;

  /// extract a wchar
  CORBA::Boolean operator>>= (to_wchar) const;

  /// extract a bounded string
  CORBA::Boolean operator>>= (to_string) const;

  /// extract a bounded wide string
  CORBA::Boolean operator>>= (to_wstring) const;

  /// extract an object reference
  CORBA::Boolean operator>>= (to_object) const;

  // the following are unsafe operations
  // ORBOS/90-01-11, pg 672: For C++ mapping using the CORBA_Environment
  // parameter, two forms of the replace method are provided.

  /// Return TypeCode of the element stored in the Any.
  CORBA::TypeCode_ptr type (void) const;

  /// For use along with <<= of a value of aliased type when the alias must
  /// be preserved.
  void type (CORBA::TypeCode_ptr type
             TAO_ENV_ARG_DECL_WITH_DEFAULTS);

  /**
   * Returns 0 if the Any has not been assigned a value, following the
   * CORBA spec (ORBOS/98-01-11) it returns a non-zero value
   * otherwise. TAO does *not* guarantee that this value may be casted
   * to the contained type safely.
   */
  const void *value (void) const;

  // = TAO extensions
  /**
   * Reports whether the Any own the data or not. This is used by the
   * >>= operators generated by the IDL compiler. The >>= operator
   * checks if the Any owns the data. If it does, then it will simply
   * retrieve the data from internal cache.  Otherwise, the operator
   * will have to decode the cdr string.
   */
  CORBA::Boolean any_owns_data (void) const;

  /// Message block accessor.
  /// Get the byte order inside the CDR stream.
  ACE_Message_Block* _tao_get_cdr (void) const;
  int _tao_byte_order (void) const;

  /// Generated data types define a 'destructor' function that
  /// correctly destroys an object stored in the Any.
  typedef void (*_tao_destructor)(void*);

  /// Replace via message block instead of <value_>.
  void _tao_replace (CORBA::TypeCode_ptr,
                     int byte_order,
                     const ACE_Message_Block *mb);

  /// Replace all the contents of the any, used in the <<= operators.
  void _tao_replace (CORBA::TypeCode_ptr type,
                     int byte_order,
                     const ACE_Message_Block *mb,
                     CORBA::Boolean any_owns_data,
                     void* value,
                     CORBA::Any::_tao_destructor destructor);

  /// Replace the value of the Any, used in the >>= operators.
  void _tao_replace (CORBA::TypeCode_ptr type,
                     CORBA::Boolean any_owns_data,
                     void* value,
                     CORBA::Any::_tao_destructor destructor);

  /// Encode the contents of the Any into <cdr>
  void _tao_encode (TAO_OutputCDR &cdr,
                    TAO_ORB_Core *orb_core
                    TAO_ENV_ARG_DECL);

  /// Decode the <cdr> using the typecode in the Any object.
  void _tao_decode (TAO_InputCDR &cdr
                    TAO_ENV_ARG_DECL);

#if !defined(__GNUC__) || __GNUC__ > 2 || __GNUC_MINOR__ >= 8
  typedef CORBA_Any_ptr _ptr_type;
  typedef CORBA_Any_var _var_type;
#endif /* __GNUC__ */
  // Useful for template programming.

  /// Used to release Anys contained into anys.
  static void _tao_any_destructor (void*);
  static void _tao_any_string_destructor (void*);
  static void _tao_any_wstring_destructor (void*);
  static void _tao_any_tc_destructor (void*);

protected:
  /// Release the <value_>.
  void free_value (void);

private:
  /// Typecode for the <Any>.
  CORBA::TypeCode_var type_;

  /// encoded value.
  int byte_order_;
  ACE_Message_Block *cdr_;

  /// Flag that indicates the ORB is responsible for deleting the data.
  CORBA::Boolean any_owns_data_;

  /// Value for the <Any>.
  void *value_;

  /// If not zero this is the function used to destroy objects.
  CORBA::Any::_tao_destructor destructor_;

  // 94-9-14 hides unsigned char insert/extract
  void operator<<= (unsigned char);
  CORBA::Boolean operator>>= (unsigned char&) const;

  friend class CORBA_NVList;
  friend class TAO_Marshal_Any;
};

/**
 * @class CORBA_Any_var
 *
 * @brief Provide for automatic storage deallocation on going out of
 * scope.
 */
class TAO_Export CORBA_Any_var
{
public:
  /// default constructor
  CORBA_Any_var (void);

  /// construct from an Any pointer
  CORBA_Any_var (CORBA_Any *a);

  /// copy constructor
  CORBA_Any_var (const CORBA_Any_var &a);

  /// destructor
  ~CORBA_Any_var (void);

  /// assignment from a pointer to Any
  CORBA_Any_var &operator= (CORBA_Any *a);

  /**
   * assignment from an Any_var
   * This operation requires memory allocation.
   * If the allocation fails, *this is returned
   * unmodified.
   */
  CORBA_Any_var &operator= (const CORBA_Any_var &a);

  /// arrow operator (smart pointer)
  CORBA_Any *operator-> (void);

  /// cast
  operator const CORBA_Any *() const;

  /// cast
  operator CORBA_Any *&();

  /// for in Any parameter
  const CORBA_Any &in (void) const;

  /// for inout Any parameter
  CORBA_Any &inout (void);

  /// for out Any parameter
  CORBA_Any *&out (void);

  /// for Any return types
  CORBA_Any *_retn (void);

private:
  /// Holds the Any.
  CORBA_Any *ptr_;
};

/**
 * @class CORBA_Any_out
 *
 * @brief CORBA_Any_out
 *
 * The _out class for CORBA_Any. This is used to help in
 * managing the out parameters.
 */
class TAO_Export CORBA_Any_out
{
public:
  // = operations.

  /// construction from a reference to a CORBA_Any
  CORBA_Any_out (CORBA_Any *&p);

  /// construction from a var
  CORBA_Any_out (CORBA_Any_var &p);

  /// copy constructor
  CORBA_Any_out (const CORBA_Any_out &s);

  /// assignment from a CORBA_Any_out
  CORBA_Any_out &operator= (const CORBA_Any_out &s);

  /// assignment from a CORBA_Any
  CORBA_Any_out &operator= (CORBA_Any *p);

  /// assignment from a const CORBA_Any
  CORBA_Any_out &operator= (const CORBA_Any *p);

  /// cast
  operator CORBA_Any *&();

  /// return underlying instance
  CORBA_Any *& ptr (void);

  CORBA_Any *operator-> (void);

private:
  /// Instance
  CORBA_Any *&ptr_;

  /// assignment from _var disallowed
  void operator= (const CORBA_Any_var &);
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
TAO_Export void operator<<= (CORBA_Any_var &,
                             CORBA::Short);
TAO_Export void operator<<= (CORBA_Any_var &,
                             CORBA::UShort);
TAO_Export void operator<<= (CORBA_Any_var &,
                             CORBA::Long);
TAO_Export void operator<<= (CORBA_Any_var &,
                             CORBA::ULong);
TAO_Export void operator<<= (CORBA_Any_var &,
                             CORBA::LongLong);
TAO_Export void operator<<= (CORBA_Any_var &,
                             CORBA::ULongLong);
TAO_Export void operator<<= (CORBA_Any_var &,
                             CORBA::Float);
TAO_Export void operator<<= (CORBA_Any_var &,
                             CORBA::Double);
TAO_Export void operator<<= (CORBA_Any_var &,
                             const CORBA_Any&);
TAO_Export void operator<<= (CORBA_Any_var &,
                             const char*);
TAO_Export void operator<<= (CORBA_Any_var &,
                             CORBA::TypeCode_ptr);
TAO_Export void operator<<= (CORBA_Any_var &,
                             const CORBA::Object_ptr);
TAO_Export void operator<<= (CORBA_Any_var &,
                             CORBA::Any::from_boolean);
TAO_Export void operator<<= (CORBA_Any_var &,
                             CORBA::Any::from_char);
TAO_Export void operator<<= (CORBA_Any_var &,
                             CORBA::Any::from_wchar);
TAO_Export void operator<<= (CORBA_Any_var &,
                             CORBA::Any::from_octet);
TAO_Export void operator<<= (CORBA_Any_var &,
                             CORBA::Any::from_string);
TAO_Export void operator<<= (CORBA_Any_var &,
                             CORBA::Any::from_wstring);

// These are not required by the spec, but will make users
// of other ORBs that are used to them more comfortable.
TAO_Export CORBA::Boolean operator>>= (CORBA_Any_var &,
                                       CORBA::Short&);
TAO_Export CORBA::Boolean operator>>= (CORBA_Any_var &,
                                       CORBA::UShort&);
TAO_Export CORBA::Boolean operator>>= (CORBA_Any_var &,
                                       CORBA::Long&);
TAO_Export CORBA::Boolean operator>>= (CORBA_Any_var &,
                                       CORBA::ULong&);
TAO_Export CORBA::Boolean operator>>= (CORBA_Any_var &,
                                       CORBA::LongLong&);
TAO_Export CORBA::Boolean operator>>= (CORBA_Any_var &,
                                       CORBA::ULongLong&);
TAO_Export CORBA::Boolean operator>>= (CORBA_Any_var &,
                                       CORBA::Float&);
TAO_Export CORBA::Boolean operator>>= (CORBA_Any_var &,
                                       CORBA::Double&);
TAO_Export CORBA::Boolean operator>>= (CORBA_Any_var &,
                                       CORBA_Any&);
TAO_Export CORBA::Boolean operator>>= (CORBA_Any_var &,
                                       CORBA::TypeCode_ptr&);
TAO_Export CORBA::Boolean operator>>= (CORBA_Any_var &,
                                       const char*&);
TAO_Export CORBA::Boolean operator>>= (CORBA_Any_var &,
                                       const CORBA::WChar*&);
TAO_Export CORBA::Boolean operator>>= (CORBA_Any_var &,
                                       CORBA::Any::to_boolean);
TAO_Export CORBA::Boolean operator>>= (CORBA_Any_var &,
                                       CORBA::Any::to_octet);
TAO_Export CORBA::Boolean operator>>= (CORBA_Any_var &,
                                       CORBA::Any::to_char);
TAO_Export CORBA::Boolean operator>>= (CORBA_Any_var &,
                                       CORBA::Any::to_wchar);
TAO_Export CORBA::Boolean operator>>= (CORBA_Any_var &,
                                       CORBA::Any::to_string);
TAO_Export CORBA::Boolean operator>>= (CORBA_Any_var &,
                                       CORBA::Any::to_wstring);
TAO_Export CORBA::Boolean operator>>= (CORBA_Any_var &,
                                       CORBA::Any::to_object);

#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_ANY_H */
