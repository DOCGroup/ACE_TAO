// -*- C++ -*-


//=============================================================================
/**
 *  @file    CORBA_String.h
 *
 *  $Id$
 *
 *   Header file for the CORBA string types.
 *
 *
 */
//=============================================================================



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


/**
 * @class CORBA_String_var
 *
 * @brief CORBA_String var class. Provides automatic deallocation of storage
 * for the string once it goes out of scope.
 */
class TAO_Export CORBA_String_var
{
public:
  /// default constructor.
  CORBA_String_var (void);

  /// constructor, owns p.
  CORBA_String_var (char *p);

  /// constructor. Makes a copy of p.
  CORBA_String_var (const char *p);

  /// copy constructor.
  CORBA_String_var (const CORBA_String_var &s);

  /// destructor.
  ~CORBA_String_var (void);

  /// assignment operator.
  CORBA_String_var &operator= (char *p);

  /// assignment to a const char*.  Makes a copy.
  CORBA_String_var &operator= (const char *p);

  /// assignment operator.
  CORBA_String_var &operator= (const CORBA_String_var &s);

  /// access and modify.
  operator char *();

  /// only read privileges.
  operator const char *() const;

  /// allows access and modification using an slot.
  char &operator[] (CORBA::ULong slot);

  /// allows only accessing thru an slot.
  char operator[] (CORBA::ULong slot) const;

    // = in, out, out, and _retn operations.
    // ORBOS/97-05-15, Appendix C.2

  /// for in parameter.
  const char *in (void) const;

  /// for inout parameter.
  char *&inout (void);

  /// for out parameter.
  char *&out (void);

  /// for string of return type.
  char *_retn (void);

private:
  /// instance.
  char *ptr_;
};

/**
 * @class CORBA_String_out
 *
 * @brief CORBA_String_out
 *
 * To support the memory management for "out" parameter passing
 * mode.  ORBOS/97-05-15, Appendix C.2 defines a CORBA_String_out class
 */
class TAO_Export CORBA_String_out
{
public:
  // = operations.

  /// Construction from a reference to a string.
  CORBA_String_out (char *&p);

  /// Construction from a var.
  CORBA_String_out (CORBA_String_var &p);

  /// Copy constructor.
  CORBA_String_out (const CORBA_String_out &s);

  /// Assignment from a string_out.
  CORBA_String_out &operator= (const CORBA_String_out &s);

  /// Assignment from a string.
  CORBA_String_out &operator= (char *p);

  /// Assignment from a constant char*.
  CORBA_String_out& operator= (const char* p);

  /// Cast.
  operator char *&();

  /// Return underlying instance.
  char *&ptr (void);

private:
  /// Instance.
  char *&ptr_;

  // assignment from _var disallowed
  void operator= (const CORBA_String_var &);
};

// ****************************************************************

/**
 * @class CORBA_WString_var
 *
 * @brief CORBA_WString var class. Provides automatic deallocation of
 * storage for wide strings
 */
class TAO_Export CORBA_WString_var
{
public:
  /// default constructor.
  CORBA_WString_var (void);

  /// constructor, owns p.
  CORBA_WString_var (CORBA::WChar *p);

  /// constructor. Makes a copy of p.
  CORBA_WString_var (const CORBA::WChar *p);

  /// copy constructor.
  CORBA_WString_var (const CORBA_WString_var &s);

  /// destructor.
  ~CORBA_WString_var (void);

  /// assignment operator.
  CORBA_WString_var &operator= (CORBA::WChar *p);

  /// assignment to a const char*.  Makes a copy.
  CORBA_WString_var &operator= (const CORBA::WChar *p);

  /// assignment operator.
  CORBA_WString_var &operator= (const CORBA_WString_var &s);

  /// access and modify.
  operator CORBA::WChar *();

  /// only read privileges.
  operator const CORBA::WChar *() const;

  /// allows access and modification using an slot.
  CORBA::WChar &operator[] (CORBA::ULong slot);

  /// allows only accessing thru an slot.
  CORBA::WChar operator[] (CORBA::ULong slot) const;

    // = in, out, out, and _retn operations.
    // ORBOS/97-05-15, Appendix C.2

  /// for in parameter.
  const CORBA::WChar *in (void) const;

  /// for inout parameter.
  CORBA::WChar *&inout (void);

  /// for out parameter.
  CORBA::WChar *&out (void);

  /// for string of return type.
  CORBA::WChar *_retn (void);

private:
  /// instance.
  CORBA::WChar *ptr_;
};

/**
 * @class CORBA_WString_out
 *
 * @brief CORBA_WString_out
 *
 * To support the memory management for "out" parameter passing
 * mode.  ORBOS/97-05-15, Appendix C.2 defines a CORBA_WString_out
 * class
 */
class TAO_Export CORBA_WString_out
{
public:
  // = operations.

  /// Construction from a reference to a string.
  CORBA_WString_out (CORBA::WChar *&p);

  /// Construction from a var.
  CORBA_WString_out (CORBA_WString_var &p);

  /// Copy constructor.
  CORBA_WString_out (const CORBA_WString_out &s);

  /// Assignment from a string_out.
  CORBA_WString_out &operator= (const CORBA_WString_out &s);

  /// Assignment from a string.
  CORBA_WString_out &operator= (CORBA::WChar *p);

  /// Cast.
  operator CORBA::WChar *&();

  /// Return underlying instance.
  CORBA::WChar *&ptr (void);

private:
  /// Instance.
  CORBA::WChar *&ptr_;

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
