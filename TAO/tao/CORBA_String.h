// -*- C++ -*-
//
// $Id$


// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    CORBA_String.h
//
// = DESCRIPTION
//     Header file for the CORBA string types.
//
// ============================================================================


#ifndef TAO_CORBA_STRING_H
#define TAO_CORBA_STRING_H

#include "ace/pre.h"

#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


// For the (W)String_var and (W)String_out iostream operators.
#if defined (ACE_HAS_MINIMUM_IOSTREAMH_INCLUSION)
#include "ace/streams.h"
#endif /* ACE_HAS_MINIMUM_IOSTREAMH_INCLUSION */


class TAO_Export CORBA_String_var
{
  // = TITLE
  //   CORBA_String var class. Provides automatic deallocation of storage
  //   for the string once it goes out of scope.
public:
  CORBA_String_var (void);
  // default constructor.

  CORBA_String_var (char *p);
  // constructor, owns p.

  CORBA_String_var (const char *p);
  // constructor. Makes a copy of p.

  CORBA_String_var (const CORBA_String_var &s);
  // copy constructor.

  ~CORBA_String_var (void);
  // destructor.

  CORBA_String_var &operator= (char *p);
  // assignment operator.

  CORBA_String_var &operator= (const char *p);
  // assignment to a const char*.  Makes a copy.

  CORBA_String_var &operator= (const CORBA_String_var &s);
  // assignment operator.

  operator char *();
  // access and modify.

  operator const char *() const;
  // only read privileges.

  char &operator[] (CORBA::ULong slot);
  // allows access and modification using an slot.

  char operator[] (CORBA::ULong slot) const;
  // allows only accessing thru an slot.

    // = in, out, out, and _retn operations.
    // ORBOS/97-05-15, Appendix C.2

  const char *in (void) const;
  // for in parameter.

  char *&inout (void);
  // for inout parameter.

  char *&out (void);
  // for out parameter.

  char *_retn (void);
  // for string of return type.

private:
  char *ptr_;
  // instance.
};

class TAO_Export CORBA_String_out
{
  // = TITLE
  //   CORBA_String_out
  //
  // = DESCRIPTION
  //   To support the memory management for "out" parameter passing
  //   mode.  ORBOS/97-05-15, Appendix C.2 defines a CORBA_String_out class
public:
  // = operations.

  CORBA_String_out (char *&p);
  // Construction from a reference to a string.

  CORBA_String_out (CORBA_String_var &p);
  // Construction from a var.

  CORBA_String_out (const CORBA_String_out &s);
  // Copy constructor.

  CORBA_String_out &operator= (const CORBA_String_out &s);
  // Assignment from a string_out.

  CORBA_String_out &operator= (char *p);
  // Assignment from a string.

  CORBA_String_out& operator= (const char* p);
  // Assignment from a constant char*.

  operator char *&();
  // Cast.

  char *&ptr (void);
  // Return underlying instance.

private:
  char *&ptr_;
  // Instance.

  // assignment from _var disallowed
  void operator= (const CORBA_String_var &);
};

// ****************************************************************

class TAO_Export CORBA_WString_var
{
  // = TITLE
  //   CORBA_WString var class. Provides automatic deallocation of
  //   storage for wide strings
  //
public:
  CORBA_WString_var (void);
  // default constructor.

  CORBA_WString_var (CORBA::WChar *p);
  // constructor, owns p.

  CORBA_WString_var (const CORBA::WChar *p);
  // constructor. Makes a copy of p.

  CORBA_WString_var (const CORBA_WString_var &s);
  // copy constructor.

  ~CORBA_WString_var (void);
  // destructor.

  CORBA_WString_var &operator= (CORBA::WChar *p);
  // assignment operator.

  CORBA_WString_var &operator= (const CORBA::WChar *p);
  // assignment to a const char*.  Makes a copy.

  CORBA_WString_var &operator= (const CORBA_WString_var &s);
  // assignment operator.

  operator CORBA::WChar *();
  // access and modify.

  operator const CORBA::WChar *() const;
  // only read privileges.

  CORBA::WChar &operator[] (CORBA::ULong slot);
  // allows access and modification using an slot.

  CORBA::WChar operator[] (CORBA::ULong slot) const;
  // allows only accessing thru an slot.

    // = in, out, out, and _retn operations.
    // ORBOS/97-05-15, Appendix C.2

  const CORBA::WChar *in (void) const;
  // for in parameter.

  CORBA::WChar *&inout (void);
  // for inout parameter.

  CORBA::WChar *&out (void);
  // for out parameter.

  CORBA::WChar *_retn (void);
  // for string of return type.

private:
  CORBA::WChar *ptr_;
  // instance.
};

class TAO_Export CORBA_WString_out
{
  // = TITLE
  //   CORBA_WString_out
  //
  // = DESCRIPTION
  //   To support the memory management for "out" parameter passing
  //   mode.  ORBOS/97-05-15, Appendix C.2 defines a CORBA_WString_out
  //   class
  //
public:
  // = operations.

  CORBA_WString_out (CORBA::WChar *&p);
  // Construction from a reference to a string.

  CORBA_WString_out (CORBA_WString_var &p);
  // Construction from a var.

  CORBA_WString_out (const CORBA_WString_out &s);
  // Copy constructor.

  CORBA_WString_out &operator= (const CORBA_WString_out &s);
  // Assignment from a string_out.

  CORBA_WString_out &operator= (CORBA::WChar *p);
  // Assignment from a string.

  operator CORBA::WChar *&();
  // Cast.

  CORBA::WChar *&ptr (void);
  // Return underlying instance.

private:
  CORBA::WChar *&ptr_;
  // Instance.

  // assignment from _var disallowed
  void operator= (const CORBA_WString_var &);
};

# if !defined (ACE_LACKS_IOSTREAM_TOTALLY)

TAO_Export ostream &
operator<< (ostream &, const CORBA::String_var &);
TAO_Export istream &
operator>> (istream &, CORBA::String_var &);
TAO_Export ostream &
operator<< (ostream &, CORBA::String_out &);
TAO_Export istream &
operator>> (istream &, CORBA::String_out &);
TAO_Export ostream &
operator<< (ostream &, const CORBA::WString_var &);
TAO_Export istream &
operator>> (istream &, CORBA::WString_var &);
TAO_Export ostream &
operator<< (ostream &, CORBA::WString_out &);
TAO_Export istream &
operator>> (istream &, CORBA::WString_out &);

# endif /* ACE_LACKS_IOSTREAM_TOTALLY */

#if defined (__ACE_INLINE__)
# include "tao/CORBA_String.inl"
#endif /* ! __ACE_INLINE__ */


#include "ace/post.h"

#endif  /* TAO_CORBA_STRING_H */
