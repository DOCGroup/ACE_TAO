// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    Context.h
//
// = DESCRIPTION
//    Header file for CORBA Context class.
//
// = AUTHOR
//     Jeff Parsons <parsons@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_CONTEXT_H
#define TAO_CONTEXT_H

#include "tao/corbafwd.h"

class TAO_Export CORBA_Context
{
  // = TITLE
  //   CORBA_Context
  //
  // = DESCRIPTION
  //   TAO's minimal implementation of the Context interface.
  //   Since Contexts are inherently un-typesafe, there use
  //   is deprecated and the feature may eventaully disappear
  //   from CORBA. It is implemented only to make the arg list
  //   of CORBA_Object::_create_request() compliant. The only
  //   (pointer) value that should be passed is 0.
  //
public:
  // Default ctor and dtor.
  CORBA_Context (void);
  ~CORBA_Context (void);

  // Pseudo object methods
  static CORBA_Context* _duplicate (CORBA_Context*);
  static CORBA_Context* _nil (void);

  // = Reference counting.
  CORBA::ULong _incr_refcnt (void);
  CORBA::ULong _decr_refcnt (void);

private:
  CORBA::ULong refcount_;
  // reference counting

  ACE_SYNCH_MUTEX refcount_lock_;
  // protect the reference count
};

typedef CORBA_Context* CORBA_Context_ptr;

class TAO_Export CORBA_Context_var
{
  // = TITLE
  //   The T_var class for Context
  //
  // = DESCRIPTION
  //   As any other pseudo object Context must have a T_var class,
  //   the interface an semantics are specified in the CORBA spec.
  //
  // = NOTE
  //   We use CORBA_Context_ptr as the _ptr type instead of
  //   CORBA::Context_ptr, this is an attempt to reduced the cyclic
  //   dependencies in TAO.
  //
public:
  CORBA_Context_var (void);
  CORBA_Context_var (CORBA_Context_ptr);
  CORBA_Context_var (const CORBA_Context_var &);
  ~CORBA_Context_var (void);

  CORBA_Context_var &operator= (CORBA_Context_ptr);
  CORBA_Context_var &operator= (const CORBA_Context_var &);
  CORBA_Context_ptr operator-> (void) const;

  operator const CORBA_Context_ptr &() const;
  operator CORBA_Context_ptr &();
  // in, inout, out, _retn
  CORBA_Context_ptr in (void) const;
  CORBA_Context_ptr &inout (void);
  CORBA_Context_ptr &out (void);
  CORBA_Context_ptr _retn (void);
  CORBA_Context_ptr ptr (void) const;

private:
  CORBA_Context_ptr ptr_;
};

class TAO_Export CORBA_Context_out
{
  // = TITLE
  //   The T_out class for Context
  //
  // = DESCRIPTION
  //   As any other pseudo object Context must have a T_out class,
  //   the interface an semantics are specified in the CORBA spec.
  //
  // = NOTE
  //   We use CORBA_Context_ptr as the _ptr type instead of
  //   CORBA::Context_ptr, this is an attempt to reduced the cyclic
  //   dependencies in TAO.
  //
public:
  CORBA_Context_out (CORBA_Context_ptr &);
  CORBA_Context_out (CORBA_Context_var &);
  CORBA_Context_out (CORBA_Context_out &);
  CORBA_Context_out &operator= (CORBA_Context_out &);
  CORBA_Context_out &operator= (const CORBA_Context_var &);
  CORBA_Context_out &operator= (CORBA_Context_ptr);
  operator CORBA_Context_ptr &();
  CORBA_Context_ptr &ptr (void);
  CORBA_Context_ptr operator-> (void);

private:
  CORBA_Context_ptr &ptr_;
};

#if defined (__ACE_INLINE__)
# include "tao/Context.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_CONTEXT_H */
