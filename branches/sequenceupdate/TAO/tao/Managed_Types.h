// -*- C++ -*-

//=============================================================================
/**
 *  @file    Managed_Types.h
 *
 *  $Id$
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#ifndef TAO_MANAGED_TYPES_H
#define TAO_MANAGED_TYPES_H

#include /**/ "ace/pre.h"

#include "tao/TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Basic_Types.h"
#include "tao/CORBA_String.h"

/****************************************************************/

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_String_Manager
 *
 * @brief Manager for strings.
 *
 * This class implements the generic string manager and is used in the C++
 * mapping of "struct" members that are of type "string". The difference
 * between this class and the CORBA::String_var class is that the default
 * constructor initializes the underlying string to an empty string in this
 * class whereas it is a NUL string for the _var class.
 */
class TAO_Export TAO_String_Manager
{
public:

  /// default CTOR will initialize the underlying ptr_ to empty string.
  TAO_String_Manager (void);

  /// copy constructor
  TAO_String_Manager (const TAO_String_Manager &);

  /// constructor from const char* makes a copy.
  TAO_String_Manager (const char *);

  /// destructor
  ~TAO_String_Manager (void);

  /// assignment from another managed type
  TAO_String_Manager &operator= (const TAO_String_Manager&);

  /// assignment from var type will make a copy
  TAO_String_Manager &operator= (const CORBA::String_var&);

  /// assignment from a constant char* will make a copy
  TAO_String_Manager &operator= (const char *);

  /// assignment from char* will not make a copy. The String_Manager will now
  /// own the string.
  TAO_String_Manager &operator= (char *);

  /// cast  (read-only)
  operator const char*() const;

  /// for in parameter.
  const char *in (void) const;

  /// for inout parameter.
  char *&inout (void);

  /// for out parameter.
  char *&out (void);

  /// for string of return type.
  char *_retn (void);

private:
  /// The underlying string
  char *ptr_;

};

/**
 * @class TAO_WString_Manager
 *
 * @brief Manager for wide strings.
 *
 * This class implements the generic wstring manager and is used
 * in the C++ mapping of "struct" members that are of type
 * "wstring".
 * The difference between this class and the CORBA::WString_var
 * class is that the default constructor initializes the
 * underlying wstring to an empty string in this class whereas it
 * is a NUL wstring for the _var class.
 */
class TAO_Export TAO_WString_Manager
{
public:

  /// default CTOR will initialize the underlying ptr_ to empty string.
  TAO_WString_Manager (void);

  /// copy constructor
  TAO_WString_Manager (const TAO_WString_Manager &);

  /// constructor from const whar* makes a copy.
  TAO_WString_Manager (const CORBA::WChar *);

  /// destructor
  ~TAO_WString_Manager (void);

  /// assignment from another managed type
  TAO_WString_Manager &operator= (const TAO_WString_Manager&);

  /// assignment from var type will make a copy
  TAO_WString_Manager &operator= (const CORBA::WString_var&);

  /// assignment from a constant wchar* will make a copy
  TAO_WString_Manager &operator= (const CORBA::WChar *);

  /// assignment from wchar* will not make a copy. The WString_Manager
  /// will now own the string.
  TAO_WString_Manager &operator= (CORBA::WChar *);

  /// cast  (read-only)
  operator const CORBA::WChar*() const;

  /// for in parameter.
  const CORBA::WChar *in (void) const;

  /// for inout parameter.
  CORBA::WChar *&inout (void);

  /// for out parameter.
  CORBA::WChar *&out (void);

  /// for string of return type.
  CORBA::WChar *_retn (void);

private:
  /// The underlying wide string
  CORBA::WChar *ptr_;

};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "tao/Managed_Types.i"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_MANAGED_TYPES_H */
