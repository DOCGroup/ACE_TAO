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

#if !defined (TAO_ANY_H)
#define TAO_ANY_H

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
             CORBA::Boolean any_owns_data = CORBA::B_FALSE);
  // Constructor. The any_owns_data flag determines if the Any owns the value

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

  void operator<<= (CORBA::Float);
  // insert a float

  void operator<<= (CORBA::Double);
  // insert a double

  void operator<<= (const CORBA_Any&);
  // insert an Any

  void operator<<= (const char*);
  // insert unbounded strings

  // =type safe extraction

  CORBA::Boolean operator>>= (CORBA::Short&) const;
  // extract a short

  CORBA::Boolean operator>>= (CORBA::UShort&) const;
  // extract an unsigned short

  CORBA::Boolean operator>>= (CORBA::Long&) const;
  // extract a long

  CORBA::Boolean operator>>= (CORBA::ULong&) const;
  // extract an unsigned long

  CORBA::Boolean operator>>= (CORBA::Float&) const;
  // extract a float

  CORBA::Boolean operator>>= (CORBA::Double&) const;
  // extract a double

  CORBA::Boolean operator>>= (CORBA_Any&) const;
  // extract an Any

  CORBA::Boolean operator>>= (char*&) const;
  // extract an unbounded string

  // = Special types.

  // These are needed for insertion and extraction of booleans,
  // octets, chars, and bounded strings.

  struct from_boolean
  {
    from_boolean (CORBA::Boolean b);
    CORBA::Boolean val_;
  };

  struct from_octet
  {
    from_octet (CORBA::Octet o);
    CORBA::Octet val_;
  };

  struct from_char
  {
    from_char (CORBA::Char c);
    CORBA::Char val_;
  };

  struct from_string
  {
    from_string (char* s,
                 CORBA::ULong b,
                 CORBA::Boolean nocopy = CORBA::B_FALSE);
    char *val_;
    CORBA::ULong bound_;
    CORBA::Boolean nocopy_;
  };

  void operator<<= (from_boolean);
  // insert a boolean

  void operator<<= (from_char);
  // insert a char

  void operator<<= (from_octet);
  // insert an octet

  void operator<<= (from_string);
  // insert a bounded string

  // = Special types.

  // These extract octets, chars, booleans, bounded strings, and
  // object references

  struct to_boolean
  {
    to_boolean (CORBA::Boolean &b);
    CORBA::Boolean &ref_;
  };

  struct to_char
  {
    to_char (CORBA::Char &c);
    CORBA::Char &ref_;
  };

  struct to_octet
  {
    to_octet (CORBA::Octet &o);
    CORBA::Octet &ref_;
  };

  struct to_string
  {
    to_string (char *&s, CORBA::ULong b);
    char *&val_;
    CORBA::ULong bound_;
  };

  struct to_object
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

  CORBA::Boolean operator>>= (to_string) const;
  // extract a bounded string

  CORBA::Boolean operator>>= (to_object) const;
  // extract an object reference

  // = ALLOCATION

  void *operator new (size_t, const void *p);
  // Placement new.

  void *operator new (size_t s);
  // Default new.

  void operator delete (void *p);
  // Default delete

  // the following are unsafe operations
  // ORBOS/90-01-11, pg 672: For C++ mapping using the CORBA::Environment
  // parameter, two forms of the replace method are provided.

  void replace (CORBA::TypeCode_ptr type,
                const void *value,
                CORBA::Boolean any_owns_data,
                CORBA::Environment &env);
  // Replace the current typecode and data with the specified one -
  // unsafe.

  void replace (CORBA::TypeCode_ptr type,
                const void *value,
                CORBA::Environment &env);
  // Replace the current typecode and data with the specified one -
  // unsafe. This uses a default value for the "any_owns_data" parameter

  CORBA::TypeCode_ptr type (void) const;
  // Return TypeCode of the element stored in the Any.

  const void *value (void) const;
  // Returns 0 if the Any has not been assigned a value, following the
  // CORBA spec (ORBOS/98-01-11) it returns a non-zero value
  // otherwise. TAO does *not* guarantee that this value may be casted
  // to the contained type safely.

  // = Memory management methods.
  CORBA::ULong AddRef (void);
  CORBA::ULong Release (void);

private:
  CORBA::TypeCode_ptr type_;
  // Typecode for the <Any>.

  void *value_;
  // Value for the <Any>.

  CORBA::Boolean any_owns_data_;
  // Flag that indicates the ORB is responsible for deleting the data.

  CORBA::ULong refcount_;
  // Reference count the <Any> to reduce copying costs.

  // 94-9-14 hides unsigned char insert/extract
  void operator<<= (unsigned char);
  CORBA::Boolean operator>>= (unsigned char&) const;
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
  //   The _out class for CORBA_Any. This is used to help in managing the out
  //   parameters.
public:
  // = operations.

  CORBA_Any_out (CORBA_Any *&p);
  // construction from a reference to a CORBA_Any

  CORBA_Any_out (CORBA_Any_var &p);
  // construction from a var

  CORBA_Any_out (CORBA_Any_out &s);
  // copy constructor

  CORBA_Any_out &operator= (CORBA_Any_out &s);
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
