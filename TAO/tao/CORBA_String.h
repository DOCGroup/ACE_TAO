// -*- C++ -*-

//=============================================================================
/**
 *  @file    CORBA_String.h
 *
 *  $Id$
 *
 *  Header file for the CORBA string types.
 *
 *  @author DOC Group at Wash U, UCI, and Vanderbilt U.
 */
//=============================================================================

#ifndef TAO_CORBA_STRING_H
#define TAO_CORBA_STRING_H

#include "ace/pre.h"

#include "tao/TAO_Export.h"
#include "tao/Basic_Types.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// For the (W)String_var and (W)String_out iostream operators.
#include "ace/iosfwd.h"

class TAO_String_Manager;
class TAO_WString_Manager;

namespace CORBA
{
  /**
   * @name CORBA String Memory Management
   *
   * CORBA string memory management functions.
   */
  //@{
  TAO_Export char * string_alloc (ULong len);
  TAO_Export char * string_dup (const char *);
  TAO_Export void string_free (char *);
  //@}

  /**
   * @name CORBA Wide String Memory Management
   *
   * CORBA wide string memory management functions.
   */
  //@{
  TAO_Export WChar * wstring_alloc (ULong len);
  TAO_Export WChar * wstring_dup (const WChar * const);
  TAO_Export void wstring_free (WChar * const);
  //@}

  /**
   * @class String_var
   *
   * @brief CORBA::String var class.
   *
   * Provides automatic deallocation of storage for the string once it
   * goes out of scope.
   */
  class TAO_Export String_var
  {
  public:
    /// Default constructor.
    String_var (void);

    /// Constructor, owns p.
    String_var (char *p);

    /// constructor. Makes a copy of p.
    String_var (const char *p);

    /// copy constructor.
    String_var (const String_var &s);

    /// destructor.
    ~String_var (void);

    /// assignment operator.
    String_var &operator= (char *p);

    /// assignment to a const char*.  Makes a copy.
    String_var &operator= (const char *p);

    /// assignment operator.
    String_var &operator= (const String_var &s);

    /// Spec-defined read/write version.
    operator char *&();

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

    /// TAO extension.
    char *ptr (void);

  private:
    /// instance.
    char *ptr_;
  };

  /**
   * @class String_out
   *
   * @brief String_out
   *
   * To support the memory management for "out" parameter passing
   * mode.  ORBOS/97-05-15, Appendix C.2 defines a CORBA::String_out
   * class
   */
  class TAO_Export String_out
  {
  public:

    /// Construction from a reference to a string.
    String_out (char *&p);

    /// Construction from a var.
    String_out (CORBA::String_var &p);

    /// Construction from a TAO_String_Manager.
    String_out (TAO_String_Manager &p);

    /// Copy constructor.
    String_out (const String_out &s);

    /// Assignment from a string_out.
    String_out &operator= (const String_out &s);

    /// Assignment from a string.
    String_out &operator= (char *p);

    /// Assignment from a constant char*.
    String_out& operator= (const char* p);

    /// Cast.
    operator char *&();

    /// Return underlying instance.
    char *&ptr (void);

  private:
    /// Instance.
    char *&ptr_;

    // assignment from _var disallowed
    void operator= (const CORBA::String_var &);
  };

  // ****************************************************************

  /**
   * @class WString_var
   *
   * @brief CORBA::WString var class.
   *
   * Provides automatic deallocation of storage for wide strings.
   */
  class TAO_Export WString_var
  {
  public:
    /// default constructor.
    WString_var (void);

    /// constructor, owns p.
    WString_var (CORBA::WChar *p);

    /// constructor. Makes a copy of p.
    WString_var (const CORBA::WChar *p);

    /// copy constructor.
    WString_var (const WString_var &s);

    /// destructor.
    ~WString_var (void);

    /// assignment operator.
    WString_var &operator= (CORBA::WChar *p);

    /// assignment to a const char*.  Makes a copy.
    WString_var &operator= (const CORBA::WChar *p);

    /// assignment operator.
    WString_var &operator= (const WString_var &s);

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

    /// TAO extension.
    CORBA::WChar *ptr (void);

  private:
    /// instance.
    CORBA::WChar *ptr_;
  };

  /**
   * @class WString_out
   *
   * @brief WString_out
   *
   * To support the memory management for "out" parameter passing
   * mode.  ORBOS/97-05-15, Appendix C.2 defines a CORBA::WString_out
   * class.
   */
  class TAO_Export WString_out
  {
  public:

    /// Construction from a reference to a string.
    WString_out (CORBA::WChar *&p);

    /// Construction from a var.
    WString_out (CORBA::WString_var &p);

    /// Construction from a TAO_WString_Manager.
    WString_out (TAO_WString_Manager &p);

    /// Copy constructor.
    WString_out (const WString_out &s);

    /// Assignment from a string_out.
    WString_out &operator= (const WString_out &s);

    /// Assignment from a string.
    WString_out &operator= (CORBA::WChar *p);

    /// Cast.
    operator CORBA::WChar *&();

    /// Return underlying instance.
    CORBA::WChar *&ptr (void);

  private:
    /// Instance.
    CORBA::WChar *&ptr_;

    // assignment from _var disallowed
    void operator= (const CORBA::WString_var &);
  };
}  // End CORBA namespace.

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
