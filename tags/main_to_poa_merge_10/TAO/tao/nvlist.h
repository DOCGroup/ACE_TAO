// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    nvlist.h
//
// = AUTHOR
//     Copyright 1994-1995 by Sun Microsystems Inc.
//     and
//     Aniruddha Gokhale (additions, missing operations)
//
// ============================================================================

#if !defined (TAO_NVLIST_H)
#  define TAO_NVLIST_H

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
  TAO_CONST char *name (void) const;
  // optional name

  CORBA::Any_ptr value (void) const;
  // return the value

  CORBA::Flags flags (void) const;
  // return the parameter mode flag

  ~CORBA_NamedValue (void);
  // destructor - manages the name and value

  // = Methods required for COM IUnknown support.

  CORBA::ULong AddRef (void);
  CORBA::ULong Release (void);
  TAO_HRESULT QueryInterface (REFIID riid,
                              void **ppv);

private:
  u_int refcount_;
  // refcount used in release

  ACE_SYNCH_MUTEX lock_;
  // for synchronization

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

  CORBA::NamedValue_ptr add (CORBA::Flags,
                             CORBA::Environment &);
  // add an element and just initialize the flags

  CORBA::NamedValue_ptr add_item (const char *,
                                  CORBA::Flags,
                                  CORBA::Environment &);
  // add an element and initialize its name and flags

  CORBA::NamedValue_ptr add_value (const char *,
                                   const CORBA::Any &,
                                   CORBA::Flags,
                                   CORBA::Environment &);
  // initializes a value, name, and flags

  CORBA::NamedValue_ptr add_item_consume (char *,
                                          CORBA::Flags,
                                          CORBA::Environment &);
  // just like add_item. In addition, memory management of char * name is taken
  // over by the NVList

  CORBA::NamedValue_ptr add_value_consume (char *,
                                           CORBA::Any_ptr,
                                           CORBA::Flags,
                                           CORBA::Environment &);
  // just like add_value. In addition, the NVList controls the memory
  // management of the char *name and Any *value parameter

  CORBA::NamedValue_ptr item (CORBA::ULong n, CORBA::Environment &env);
  // retrieve the item at the nth location. Raises Bounds


  //  CORBA::Status
  void remove (CORBA::ULong n, CORBA::Environment &env);
  // remove element at index n. Raises Bounds

  // = Methods required for COM IUnknown support

  CORBA::ULong AddRef (void);
  CORBA::ULong Release (void);
  TAO_HRESULT QueryInterface (REFIID riid,
                              void **ppv);

private:
  CORBA_NVList (void);
  // constructor - cannot be instantiated directly other than through the
  // ORB::create_list method

  CORBA::Boolean add_element (CORBA::Flags, CORBA::Environment &);
  // helper to increase the list size. This is used by all the add_ methods of
  // the NVList class

  CORBA::NamedValue *values_;
  // list of parameters stored as NamedValues

  CORBA::ULong max_;
  // maximum length of list

  CORBA::ULong len_;
  // current length of list

  ACE_SYNCH_MUTEX lock_;
  // for synchronization

  CORBA::ULong refcount_;
  // maintains how many references exist to this object

  friend class CORBA_ORB;
  friend class CORBA_Request;
};

#endif /* TAO_NVLIST_H */
