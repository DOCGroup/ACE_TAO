// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    NVList.h
//
// = AUTHOR
//     Copyright 1994-1995 by Sun Microsystems Inc.
//     and
//     Aniruddha Gokhale (additions, missing operations)
//
// ============================================================================

#ifndef TAO_NVLIST_H
#define TAO_NVLIST_H

#include "tao/orbconf.h"

#if !defined (TAO_HAS_MINIMUM_CORBA)

#include "ace/Containers_T.h"
#include "tao/Any.h"

class TAO_Export CORBA_NamedValue
{
  // = TITLE
  // NamedValue ... these occur only in "NVList" (named value list) data
  // structures.  The binary form of the data structure is frozen and
  // visible to programs using it (e.g. from C).  The C++ class supports
  // some programming discipline, e.g. to avoid memory leaks.
  //
  // They just represent parameters to calls.  The name is optional, and
  // the value is packaged as an Any.  The flags indicate parameter
  // mode, and some ownership rules for "top level" memory.
public:
  const char *name (void) const;
  // optional name

  CORBA::Any_ptr value (void) const;
  // return the value

  CORBA::Flags flags (void) const;
  // return the parameter mode flag

  ~CORBA_NamedValue (void);
  // destructor - manages the name and value

  // The pseudo object static methods..
  static CORBA_NamedValue* _duplicate (CORBA_NamedValue*);
  static CORBA_NamedValue* _nil (void);

  // = Reference counting.
  CORBA::ULong _incr_refcnt (void);
  CORBA::ULong _decr_refcnt (void);

  // Useful for template programming.
#if !defined(__GNUC__) || __GNUC__ > 2 || __GNUC_MINOR__ >= 8
  typedef CORBA_NamedValue_ptr _ptr_type;
  typedef CORBA_NamedValue_var _var_type;
#endif /* __GNUC__ */

private:
  CORBA::ULong refcount_;
  // maintains how many references exist to this object

  ACE_SYNCH_MUTEX refcount_lock_;
  // Protects the reference count.

  CORBA::Any any_;
  // holds the value

  CORBA::Flags flags_;
  // parameter mode flags

  char *name_;
  // optional IDL name of the parameter

  CORBA_NamedValue (void);
  // private constructor. Cannot be directly instantiated other than by its
  // friends.

  friend class CORBA_ORB;
  friend class CORBA_NVList;
  friend class CORBA_Request;
};

class TAO_Export CORBA_NamedValue_var
{
  // = TITLE
  //   The T_var class for NamedValue
  //
  // = DESCRIPTION
  //   As any other pseudo object NamedValue must have a T_var class,
  //   the interface an semantics are specified in the CORBA spec.
  //
  // = NOTE
  //   We use CORBA_NamedValue_ptr as the _ptr type instead of
  //   CORBA::NamedValue_ptr, this is an attempt to reduced the cyclic
  //   dependencies in TAO.
  //
public:
  CORBA_NamedValue_var (void); // default constructor
  CORBA_NamedValue_var (CORBA_NamedValue_ptr);
  CORBA_NamedValue_var (const CORBA_NamedValue_var &); // copy constructor
  ~CORBA_NamedValue_var (void); // destructor

  CORBA_NamedValue_var &operator= (CORBA_NamedValue_ptr);
  CORBA_NamedValue_var &operator= (const CORBA_NamedValue_var &);
  CORBA_NamedValue_ptr operator-> (void) const;

  operator const CORBA_NamedValue_ptr &() const;
  operator CORBA_NamedValue_ptr &();
  // in, inout, out, _retn
  CORBA_NamedValue_ptr in (void) const;
  CORBA_NamedValue_ptr &inout (void);
  CORBA_NamedValue_ptr &out (void);
  CORBA_NamedValue_ptr _retn (void);
  CORBA_NamedValue_ptr ptr (void) const;

private:
  CORBA_NamedValue_ptr ptr_;
};

class TAO_Export CORBA_NamedValue_out
{
  // = TITLE
  //   The T_out class for NamedValue
  //
  // = DESCRIPTION
  //   As any other pseudo object NamedValue must have a T_out class,
  //   the interface an semantics are specified in the CORBA spec.
  //
  // = NOTE
  //   We use CORBA_NamedValue_ptr as the _ptr type instead of
  //   CORBA::NamedValue_ptr, this is an attempt to reduced the cyclic
  //   dependencies in TAO.
  //
public:
  CORBA_NamedValue_out (CORBA_NamedValue_ptr &);
  CORBA_NamedValue_out (CORBA_NamedValue_var &);
  CORBA_NamedValue_out (CORBA_NamedValue_out &);
  CORBA_NamedValue_out &operator= (CORBA_NamedValue_out &);
  CORBA_NamedValue_out &operator= (const CORBA_NamedValue_var &);
  CORBA_NamedValue_out &operator= (CORBA_NamedValue_ptr);
  operator CORBA_NamedValue_ptr &();
  CORBA_NamedValue_ptr &ptr (void);
  CORBA_NamedValue_ptr operator-> (void);

private:
  CORBA_NamedValue_ptr &ptr_;
};

// ****************************************************************

class TAO_Export CORBA_NVList
{
  // = TITLE
  //   NVList ... this is used in the (client side) DII (Dynamic
  //   Invocation Interface) to hold parameters, except for the return
  //   parameter. It's used in the same role in the (server side) DSI
  //   (Dynamic Skeleton Interface).
  //
  // = DESCRIPTION
  //   Each user (client, server) provides the typecode and memory for
  //   each parameter using an NVList, then talks to the ORB using a
  //   Request or ServerRequest pseudo-object.  The ORB copies data
  //   to/from the IPC messages (e.g. IIOP::Request, IIOP::Response)
  //   as appropriate.
public:
  ~CORBA_NVList (void);
  // destructor

  CORBA::ULong count (void) const;
  // return the current number of elements in the list

  CORBA_NamedValue_ptr add (CORBA::Flags,
                             CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ());
  // add an element and just initialize the flags

  CORBA_NamedValue_ptr add_item (const char *,
                                  CORBA::Flags,
                                  CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ());
  // add an element and initialize its name and flags

  CORBA_NamedValue_ptr add_value (const char *,
                                   const CORBA::Any &,
                                   CORBA::Flags,
                                   CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ());
  // initializes a value, name, and flags

  CORBA_NamedValue_ptr add_item_consume (char *,
                                          CORBA::Flags,
                                          CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ());
  // just like add_item. In addition, memory management of char * name is taken
  // over by the NVList

  CORBA_NamedValue_ptr add_value_consume (char *,
                                           CORBA::Any_ptr,
                                           CORBA::Flags,
                                           CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ());
  // just like add_value. In addition, the NVList controls the memory
  // management of the char *name and Any *value parameter

  CORBA_NamedValue_ptr item (CORBA::ULong n, CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ());
  // retrieve the item at the nth location. Raises Bounds

  //  CORBA::Status
  void remove (CORBA::ULong n, CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ());
  // remove element at index n. Raises Bounds

  // The pseudo object static methods..
  static CORBA_NVList* _duplicate (CORBA_NVList*);
  static CORBA_NVList* _nil (void);

  // = Reference counting.
  CORBA::ULong _incr_refcnt (void);
  CORBA::ULong _decr_refcnt (void);

  // Useful for template programming.
#if !defined(__GNUC__) || __GNUC__ > 2 || __GNUC_MINOR__ >= 8
  typedef CORBA_NVList_ptr _ptr_type;
  typedef CORBA_NVList_var _var_type;
#endif /* __GNUC__ */

private:
  CORBA_NVList (void);
  // constructor - cannot be instantiated directly other than through the
  // ORB::create_list method

  CORBA_NamedValue_ptr add_element (CORBA::Flags, CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ());
  // helper to increase the list size. This is used by all the add_ methods of
  // the NVList class

  ACE_Unbounded_Queue<CORBA_NamedValue_ptr> values_;
  // internal list of parameters stored as NamedValues

  CORBA::ULong max_;
  // maximum length of list

  CORBA::ULong refcount_;
  // maintains how many references exist to this object

  ACE_SYNCH_MUTEX refcount_lock_;
  // Protects the reference count.

  friend class CORBA_ORB;
  friend class CORBA_Request;
};

class TAO_Export CORBA_NVList_var
{
  // = TITLE
  //   The T_var class for NVList
  //
  // = DESCRIPTION
  //   As any other pseudo object NVList must have a T_var class,
  //   the interface an semantics are specified in the CORBA spec.
  //
  // = NOTE
  //   We use CORBA_NVList_ptr as the _ptr type instead of
  //   CORBA::NVList_ptr, this is an attempt to reduced the cyclic
  //   dependencies in TAO.
  //
public:
  CORBA_NVList_var (void);
  CORBA_NVList_var (CORBA_NVList_ptr);
  CORBA_NVList_var (const CORBA_NVList_var &);
  ~CORBA_NVList_var (void);

  CORBA_NVList_var &operator= (CORBA_NVList_ptr);
  CORBA_NVList_var &operator= (const CORBA_NVList_var &);
  CORBA_NVList_ptr operator-> (void) const;

  operator const CORBA_NVList_ptr &() const;
  operator CORBA_NVList_ptr &();
  // in, inout, out, _retn
  CORBA_NVList_ptr in (void) const;
  CORBA_NVList_ptr &inout (void);
  CORBA_NVList_ptr &out (void);
  CORBA_NVList_ptr _retn (void);
  CORBA_NVList_ptr ptr (void) const;

private:
  CORBA_NVList_ptr ptr_;
};

class TAO_Export CORBA_NVList_out
{
  // = TITLE
  //   The T_out class for NVList
  //
  // = DESCRIPTION
  //   As any other pseudo object NVList must have a T_out class,
  //   the interface an semantics are specified in the CORBA spec.
  //
  // = NOTE
  //   We use CORBA_NVList_ptr as the _ptr type instead of
  //   CORBA::NVList_ptr, this is an attempt to reduced the cyclic
  //   dependencies in TAO.
  //
public:
  CORBA_NVList_out (CORBA_NVList_ptr &);
  CORBA_NVList_out (CORBA_NVList_var &);
  CORBA_NVList_out (CORBA_NVList_out &);
  CORBA_NVList_out &operator= (CORBA_NVList_out &);
  CORBA_NVList_out &operator= (const CORBA_NVList_var &);
  CORBA_NVList_out &operator= (CORBA_NVList_ptr);
  operator CORBA_NVList_ptr &();
  CORBA_NVList_ptr &ptr (void);
  CORBA_NVList_ptr operator-> (void);

private:
  CORBA_NVList_ptr &ptr_;
};

// ****************************************************************

#if defined (__ACE_INLINE__)
# include "tao/NVList.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_HAS_MINIMUM_CORBA */

#endif /* TAO_NVLIST_H */
