// This may look like C, but it's really -*- C++ -*-
//
// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    managed_types.h
//
// = DESCRIPTION
//    C++ mapping of sequences of strings/objrefs or struct/union members with
//    strings/objrefs require a special managed type. These types are define
//    din this header file.

// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#if !defined(TAO_MANAGED_TYPES_H)
#define TAO_MANAGED_TYPES_H

class ACE_Svc_Export TAO_String_ManagedType
{
  // = TITLE
  //   TAO_String_ManagedType
  // = DESCRIPTION
  //   String var class. Provides automatic deallocation of storage
  //   for the string once it goes out of scope.

public:
  TAO_String_ManagedType (void);
  // default constructor

  TAO_String_ManagedType (char *p, CORBA::Boolean release=1);
  // constructor, owns p

  TAO_String_ManagedType (const char *p, CORBA::Boolean release=1);
  // constructor. Makes a copy of p

  TAO_String_ManagedType (const TAO_String_ManagedType &s, CORBA::Boolean release=1);
  // copy constructor

  ~TAO_String_ManagedType (void);
  // destructor

  TAO_String_ManagedType &operator= (char *p);
  // assignment operator

  TAO_String_ManagedType &operator= (const char *p);
  // assignment to a const char*. Makes a copy.

  TAO_String_ManagedType &operator= (const TAO_String_ManagedType &s);
  // assignment operator

  operator char *();
  // access and modify

  operator const char *() const;
  // only read privileges

  char &operator[] (CORBA::ULong index);
  // allows access and modification using an index

  char operator[] (CORBA::ULong index) const;
  // allows only accessing thru an index

  // = in, out, out, and _retn operations.
  // ORBOS/97-05-15, Appendix C.2

  const char *in (void) const;
  // for in parameter

  char *&inout (void);
  // for inout parameter

  char *&out (void);
  // for out parameter

  char *_retn (void);
  // for string of return type

 private:
  char *ptr_;
  // instance.

  CORBA::Boolean release_;
  // do we own it
};

#if 0
// TODO: @@@ XXXASG
template <T_ptr>
class ACE_Svc_Export TAO_ObjRef_ManagedType
{
  // = TITLE
  //   String var class. Provides automatic deallocation of storage
  //   for the string once it goes out of scope.

public:
  TAO_ObjRef_ManagedType (void);
  // default constructor

  TAO_ObjRef_ManagedType (T_ptr p, CORBA::Boolean release=1);
  // constructor, owns p

  TAO_ObjRef_ManagedType (const T_ptr p, CORBA::Boolean release=1);
  // constructor. Makes a copy of p

  TAO_ObjRef_ManagedType (const TAO_ObjRef_ManagedType &s, CORBA::Boolean release=1);
  // copy constructor

  ~TAO_ObjRef_ManagedType (void);
  // destructor

  TAO_ObjRef_ManagedType &operator= (T_ptr p);
  // assignment operator

  TAO_ObjRef_ManagedType &operator= (const T_ptr p);
  // assignment to a const char*. Makes a copy.

  TAO_ObjRef_ManagedType &operator= (const TAO_ObjRef_ManagedType &s);
  // assignment operator

  operator T_ptr ();
  // access and modify

  operator const T_ptr () const;
  // only read privileges

  char &operator[] (ULong index);
  // allows access and modification using an index

  char operator[] (ULong index) const;
  // allows only accessing thru an index

  // = in, out, out, and _retn operations.
  // ORBOS/97-05-15, Appendix C.2

  const T_ptr in (void) const;
  // for in parameter

  T_ptr &inout (void);
  // for inout parameter

  T_ptr &out (void);
  // for out parameter

  T_ptr _retn (void);
  // for string of return type

 private:
  CORBA::Boolean release_;
  // do we own it?

  T_ptr ptr_;
  // instance.
};
#endif /* if 0 */

#endif /* if !defined */
