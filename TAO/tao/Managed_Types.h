/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    Managed_Types.h
//
// = AUTHOR
//
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef TAO_MANAGED_TYPES_H
#define TAO_MANAGED_TYPES_H

#include "tao/corbafwd.h"

/****************************************************************/

class TAO_Export TAO_String_Manager
{
  // = TITLE
  //   Manager for strings.
  //
  // = DESCRIPTION
  //
  //   This class implements the generic string manager and is used in the C++
  //   mapping of "struct" members that are of type "string". The difference
  //   between this class and the CORBA::String_var class is that the default
  //   conctructor initializes the underlying string to an empty string in this
  //   class whereas it is a NUL string for the _var class.
  //
public:

  TAO_String_Manager (void);
  // default CTOR will initialize the underlying ptr_ to empty string. 

  TAO_String_Manager (const TAO_String_Manager &);
  // copy constructor

  ~TAO_String_Manager (void);
  // destructor

  TAO_String_Manager &operator= (const TAO_String_Manager&);
  // assignment from another managed type

  TAO_String_Manager &operator= (const CORBA::String_var&);
  // assignment from var type will make a copy

  TAO_String_Manager &operator= (const char *);
  // assignment from a constant char* will make a copy

  TAO_String_Manager &operator= (char *);
  // assignment from char* will not make a copy. The String_Manager will now
  // own the string.

  operator const char*() const;
  // cast  (read-only)

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
  // The underlying string

};

/****************************************************************/

class TAO_Export TAO_SeqElem_String_Manager
{
  // = TITLE
  //   TAO_SeqElem_String_Manager
  //
  // = DESCRIPTION
  //   Manager for strings that are the element type of sequences.
  //
  //   Similar to the mapping for sequences of objects (and other
  //   pseudo objects) the mapping for sequences of strings requires
  //   an auxiliar class or <Manager> to handle the allocation and
  //   deallocation of the string.  The main difference with respect
  //   to String_var classes is that automatic release is not
  //   controlled on a per-item basis, but for the sequence as a
  //   whole.  The difference wrt Object_Manager is that strings are
  //   duplicated using CORBA::string_dup () as opposed to
  //   T::_duplicate(), and released using CORBA::string_free()
  //   instead of CORBA::release()
  //
  //   This class implements the generic string manager and is used to
  //   instantiate the proper sequence types.
  //
  //   This class will have the same semantics as the string manager classes
  //   defined earlier with respect to the various assignment
  //   operators. However, the freeing of old storage will be dependent on the
  //   "release" value of the parent sequence class.
  //
public:
  friend class TAO_Unbounded_String_Sequence;

  // @@ Giving friendship to a template is not implemented on several
  // compilers:
  // friend template<CORBA::ULong MAX>
  //   class TAO_Bounded_String_Sequence<TAO_SeqElem_String_Manager,MAX>;

  TAO_SeqElem_String_Manager (const TAO_SeqElem_String_Manager &);
  // copy constructor

  TAO_SeqElem_String_Manager (char **buffer, CORBA::Boolean release);
  // constructor from address of an element

  ~TAO_SeqElem_String_Manager (void);
  // destructor

  TAO_SeqElem_String_Manager &operator= (const TAO_SeqElem_String_Manager&);
  // assignment from another managed type

  TAO_SeqElem_String_Manager &operator= (const CORBA::String_var&);
  // assignment from var type will make a copy

  TAO_SeqElem_String_Manager &operator= (const char *);
  // assignment from a constant char* will make a copy

  TAO_SeqElem_String_Manager &operator= (char *);
  // assignment from char* will not make a copy. The SeqElem_String_Manager will now
  // own the string.

  operator const char*() const;
  // cast  (read-only)

  const char *in (void) const;
  // for in parameter.

  char *&inout (void);
  // for inout parameter.

  char *&out (void);
  // for out parameter.

  char *_retn (void);
  // for string of return type.

private:
  char **ptr_;
  // Address of string element from the parent's buffer.

  CORBA::Boolean release_;
  // control memory managment semantics.

  // following are now allowed since these managed class will be used only by
  // the [] operator of the sequence class. The [] operator will not use the
  // following ctors to instantiate the managed instance

  TAO_SeqElem_String_Manager (void);
  // default ctor

};

#if 0 /* WString_var not implemented in TAO yet - 01/03/1999 */
/****************************************************************/

class TAO_Export TAO_WString_Manager
{
  // = TITLE
  //   Manager for wide strings.
  //
  // = DESCRIPTION
  //
  //   This class implements the generic wstring manager and is used in the C++
  //   mapping of "struct" members that are of type "wstring". The difference
  //   between this class and the CORBA::WString_var class is that the default
  //   conctructor initializes the underlying wstring to an empty string in this
  //   class whereas it is a NUL wstring for the _var class.
  //
public:

  TAO_WString_Manager (void);
  // default CTOR will initialize the underlying ptr_ to empty string. 

  TAO_WString_Manager (const TAO_WString_Manager &);
  // copy constructor

  ~TAO_WString_Manager (void);
  // destructor

  TAO_WString_Manager &operator= (const TAO_WString_Manager&);
  // assignment from another managed type

  TAO_WString_Manager &operator= (const CORBA::WString_var&);
  // assignment from var type will make a copy

  TAO_WString_Manager &operator= (const CORBA::WChar *);
  // assignment from a constant wchar* will make a copy

  TAO_WString_Manager &operator= (CORBA::WChar *);
  // assignment from wchar* will not make a copy. The WString_Manager will now
  // own the string.

  operator const CORBA::WChar*() const;
  // cast  (read-only)

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
  // The underlying wide string

};

/****************************************************************/

class TAO_Export TAO_SeqElem_WString_Manager
{
  // = TITLE
  //   TAO_SeqElem_WString_Manager
  //
  // = DESCRIPTION
  //   Manager for strings that are the element type of sequences.
  //
  //   Similar to the mapping for sequences of objects (and other
  //   pseudo objects) the mapping for sequences of strings requires
  //   an auxiliar class or <Manager> to handle the allocation and
  //   deallocation of the string.  The main difference with respect
  //   to WString_var classes is that automatic release is not
  //   controlled on a per-item basis, but for the sequence as a
  //   whole.  The difference wrt Object_Manager is that strings are
  //   duplicated using CORBA::WString_dup () as opposed to
  //   T::_duplicate(), and released using CORBA::WString_free()
  //   instead of CORBA::release()
  //
  //   This class implements the generic string manager and is used to
  //   instantiate the proper sequence types.
  //
  //   This class will have the same semantics as the string manager classes
  //   defined earlier with respect to the various assignment
  //   operators. However, the freeing of old storage will be dependent on the
  //   "release" value of the parent sequence class.
  //
public:
  friend class TAO_Unbounded_WString_Sequence;

  // @@ Giving friendship to a template is not implemented on several
  // compilers:
  // friend template<CORBA::ULong MAX>
  //   class TAO_Bounded_WString_Sequence<TAO_SeqElem_WString_Manager,MAX>;

  TAO_SeqElem_WString_Manager (char **buffer, CORBA::Boolean release);
  // constructor from address of an element

  ~TAO_SeqElem_WString_Manager (void);
  // destructor

  TAO_SeqElem_WString_Manager &operator= (const TAO_SeqElem_WString_Manager&);
  // assignment from another managed type

  TAO_SeqElem_WString_Manager &operator= (const CORBA::WString_var&);
  // assignment from var type will make a copy

  TAO_SeqElem_WString_Manager &operator= (const char *);
  // assignment from a constant char* will make a copy

  TAO_SeqElem_WString_Manager &operator= (char *);
  // assignment from char* will not make a copy. The SeqElem_WString_Manager will now
  // own the string.

  operator const char*() const;
  // cast  (read-only)

  const char *in (void) const;
  // for in parameter.

  char *&inout (void);
  // for inout parameter.

  char *&out (void);
  // for out parameter.

  char *_retn (void);
  // for string of return type.

private:
  char **ptr_;
  // Address of string element from the parent's buffer.

  CORBA::Boolean release_;
  // control memory managment semantics.

  // following are now allowed since these managed class will be used only by
  // the [] operator of the sequence class. The [] operator will not use the
  // following ctors to instantiate the managed instance

  TAO_SeqElem_WString_Manager (void);
  // default ctor

  TAO_SeqElem_WString_Manager (const TAO_SeqElem_WString_Manager &);
  // copy constructor

};
#endif /* 0 */

#if defined (__ACE_INLINE__)
#include "tao/Managed_Types.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_MANAGED_TYPES_H */
