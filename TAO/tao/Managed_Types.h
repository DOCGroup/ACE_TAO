/* -*- C++ -*- */

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

#include "ace/pre.h"

#include "tao/Basic_Types.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/TAO_Export.h"

/****************************************************************/

/**
 * @class TAO_String_Manager
 *
 * @brief Manager for strings.
 *
 * This class implements the generic string manager and is used in the C++
 * mapping of "struct" members that are of type "string". The difference
 * between this class and the CORBA::String_var class is that the default
 * conctructor initializes the underlying string to an empty string in this
 * class whereas it is a NUL string for the _var class.
 */
class TAO_Export TAO_String_Manager
{
public:

  /// default CTOR will initialize the underlying ptr_ to empty string.
  TAO_String_Manager (void);

  /// copy constructor
  TAO_String_Manager (const TAO_String_Manager &);

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

/****************************************************************/

/**
 * @class TAO_SeqElem_String_Manager
 *
 * @brief TAO_SeqElem_String_Manager
 *
 * Manager for strings that are the element type of sequences.
 * Similar to the mapping for sequences of objects (and other
 * pseudo objects) the mapping for sequences of strings requires
 * an auxiliar class or <Manager> to handle the allocation and
 * deallocation of the string.  The main difference with respect
 * to String_var classes is that automatic release is not
 * controlled on a per-item basis, but for the sequence as a
 * whole.  The difference wrt Object_Manager is that strings are
 * duplicated using CORBA::string_dup () as opposed to
 * T::_duplicate(), and released using CORBA::string_free()
 * instead of CORBA::release()
 * This class implements the generic string manager and is used to
 * instantiate the proper sequence types.
 * This class will have the same semantics as the string manager classes
 * defined earlier with respect to the various assignment
 * operators. However, the freeing of old storage will be dependent on the
 * "release" value of the parent sequence class.
 */
class TAO_Export TAO_SeqElem_String_Manager
{
public:
  friend class TAO_Unbounded_String_Sequence;

  // @@ Giving friendship to a template is not implemented on several
  // compilers:
  // friend template<CORBA::ULong MAX>
  //   class TAO_Bounded_String_Sequence<TAO_SeqElem_String_Manager,MAX>;

  /// copy constructor
  TAO_SeqElem_String_Manager (const TAO_SeqElem_String_Manager &);

  /// constructor from address of an element
  TAO_SeqElem_String_Manager (char **buffer, CORBA::Boolean release);

  /// destructor
  ~TAO_SeqElem_String_Manager (void);

  /// assignment from another managed type
  TAO_SeqElem_String_Manager &operator= (const TAO_SeqElem_String_Manager&);

  /// assignment from var type will make a copy
  TAO_SeqElem_String_Manager &operator= (const CORBA::String_var&);

  /// assignment from a constant char* will make a copy
  TAO_SeqElem_String_Manager &operator= (const char *);

  /// assignment from char* will not make a copy. The SeqElem_String_Manager will now
  /// own the string.
  TAO_SeqElem_String_Manager &operator= (char *);

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
  /// Address of string element from the parent's buffer.
  char **ptr_;

  /// control memory managment semantics.
  CORBA::Boolean release_;

  // following are now allowed since these managed class will be used only by
  // the [] operator of the sequence class. The [] operator will not use the
  // following ctors to instantiate the managed instance

  /// default ctor
  TAO_SeqElem_String_Manager (void);

};

// ****************************************************************

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

/****************************************************************/

/**
 * @class TAO_SeqElem_WString_Manager
 *
 * @brief TAO_SeqElem_WString_Manager
 *
 * Manager for strings that are the element type of sequences.
 * Similar to the mapping for sequences of objects (and other
 * pseudo objects) the mapping for sequences of strings requires
 * an auxiliar class or <Manager> to handle the allocation and
 * deallocation of the string.  The main difference with respect
 * to WString_var classes is that automatic release is not
 * controlled on a per-item basis, but for the sequence as a
 * whole.  The difference wrt Object_Manager is that strings are
 * duplicated using CORBA::WString_dup () as opposed to
 * T::_duplicate(), and released using CORBA::WString_free()
 * instead of CORBA::release()
 * This class implements the generic string manager and is used to
 * instantiate the proper sequence types.
 * This class will have the same semantics as the string manager
 * classes defined earlier with respect to the various assignment
 * operators. However, the freeing of old storage will be
 * dependent on the "release" value of the parent sequence class.
 */
class TAO_Export TAO_SeqElem_WString_Manager
{
public:
  friend class TAO_Unbounded_WString_Sequence;

  // @@ Giving friendship to a template is not implemented on several
  // compilers:
  // friend template<CORBA::ULong MAX>
  //   class TAO_Bounded_WString_Sequence<TAO_SeqElem_WString_Manager,MAX>;

  /// constructor from address of an element
  TAO_SeqElem_WString_Manager (CORBA::WChar **buffer,
                               CORBA::Boolean release);

  /// copy constructor
  TAO_SeqElem_WString_Manager (const TAO_SeqElem_WString_Manager &);

  /// destructor
  ~TAO_SeqElem_WString_Manager (void);

  /// assignment from another managed type
  TAO_SeqElem_WString_Manager &operator= (const TAO_SeqElem_WString_Manager&);

  /// assignment from var type will make a copy
  TAO_SeqElem_WString_Manager &operator= (const CORBA::WString_var&);

  /// assignment from a constant char* will make a copy
  TAO_SeqElem_WString_Manager &operator= (const CORBA::WChar *);

  /// assignment from char* will not make a copy. The
  /// SeqElem_WString_Manager will now own the string.
  TAO_SeqElem_WString_Manager &operator= (CORBA::WChar *);

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
  /// Address of string element from the parent's buffer.
  CORBA::WChar **ptr_;

  /// control memory managment semantics.
  CORBA::Boolean release_;

  // following are now allowed since these managed class will be used only by
  // the [] operator of the sequence class. The [] operator will not use the
  // following ctors to instantiate the managed instance

  /// default ctor
  TAO_SeqElem_WString_Manager (void);

};

#if defined (__ACE_INLINE__)
#include "tao/Managed_Types.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_MANAGED_TYPES_H */
