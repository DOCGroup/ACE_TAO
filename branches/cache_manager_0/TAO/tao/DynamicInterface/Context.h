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
#include "ace/pre.h"

#include "dynamicinterface_export.h"
#include "tao/corbafwd.h"
#include "ace/Unbounded_Queue.h"
#include "ace/Atomic_Op.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_DynamicInterface_Export CORBA_Context
{
  // = TITLE
  //    CORBA_Context
  //
  // = DESCRIPTION
  //    TAO's minimal implementation of the Context interface.  Since
  //    Contexts are inherently un-typesafe, there use is deprecated
  //    and the feature may eventaully disappear from CORBA. It is
  //    implemented only to make the arg list of
  //    CORBA_Object::_create_request() compliant. The only (pointer)
  //    value that should be passed is 0.
  //
public:
  // = Initialization and termination methods.

  CORBA_Context (void);
  // Constructor.

  ~CORBA_Context (void);
  // Destructor.

  // = Pseudo-object methods
  static CORBA_Context *_duplicate (CORBA_Context*);
  static CORBA_Context *_nil (void);

  // = Reference counting.
  CORBA::ULong _incr_refcnt (void);
  CORBA::ULong _decr_refcnt (void);

  // = All the spec-required functions below will just throw a
  //   CORBA::NO_IMPLEMENT exception and do nothing else.

  const char *context_name (TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS) const;

  CORBA_Context_ptr parent (TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS) const;

  void create_child (const char *child_ctx_name,
                     CORBA_Context_out child_ctx
                     TAO_ENV_ARG_DECL_WITH_DEFAULTS);

  void set_one_value (const char *propname,
                      const CORBA_Any &propvalue
                      TAO_ENV_ARG_DECL_WITH_DEFAULTS);

  void set_values (CORBA::NVList_ptr values
                   TAO_ENV_ARG_DECL_WITH_DEFAULTS);

  void delete_values (const char *propname
                      TAO_ENV_ARG_DECL_WITH_DEFAULTS);

  void get_values (const char *start_scope,
                   CORBA::Flags op_flags,
                   const char *pattern,
                   CORBA::NVList_ptr &values
                   TAO_ENV_ARG_DECL_WITH_DEFAULTS);

#if !defined(__GNUC__) || __GNUC__ > 2 || __GNUC_MINOR__ >= 8
  typedef CORBA_Context_ptr _ptr_type;
  typedef CORBA_Context_var _var_type;
#endif /* __GNUC__ */
  // Useful for template programming.

private:
  CORBA::ULong refcount_;
  // Reference counting.

  TAO_SYNCH_MUTEX refcount_lock_;
  // Protect the reference count.
};

typedef CORBA_Context* CORBA_Context_ptr;

class TAO_DynamicInterface_Export CORBA_Context_var
{
  // = TITLE
  //     The T_var class for Context.
  //
  // = DESCRIPTION
  //    As any other pseudo object Context must have a T_var class,
  //    the interface an semantics are specified in the CORBA spec.
  //
  //    We use <CORBA_Context_ptr> as the _ptr type instead of
  //    <CORBA::Context_ptr> in an attempt to reduced the cyclic
  //    dependencies in TAO.
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

class TAO_DynamicInterface_Export CORBA_Context_out
{
  // = TITLE
  //    The T_out class for Context
  //
  // = DESCRIPTION
  //    As any other pseudo object Context must have a T_out class,
  //    the interface an semantics are specified in the CORBA spec.
  //
  //    We use <CORBA_Context_ptr> as the _ptr type instead of
  //    <CORBA::Context_ptr> in an attempt to reduced the cyclic
  //    dependencies in TAO.
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

class TAO_DynamicInterface_Export CORBA_ContextList
{
  // = TITLE
  //    ContextList definition taken from CORBA v2.3a Dec 1998
  //
  // = DESCRIPTION
  //    Maintains a list of strings for Contexts.
public:
  CORBA_ContextList (void);
  // Constructor.

  CORBA_ContextList (CORBA::ULong len,
                     char **ctx_list);
  // Constructor - initialize given a length and an array of
  // strings.

  ~CORBA_ContextList (void);
  // Destructor.

  CORBA::ULong count (void);
  // Return the number of elements.

  CORBA_ContextList_ptr _duplicate (void);
  // Increment the reference count.

  static CORBA_ContextList_ptr _duplicate (CORBA_ContextList *);
  // Increment the reference count.

  void _destroy (void);
  // Decrement the reference count and delete if it is 0.

  static CORBA_ContextList_ptr _nil ();
  // Return null pointer of this type.

  void add (char *ctx);
  // Add a string to the list.

  void add_consume (char *ctx);
  // Add and consume a string to the list.

  char *item (CORBA::ULong slot
              TAO_ENV_ARG_DECL_WITH_DEFAULTS);
  // Return the typecode at slot i. Raises the "Bounds" exception.

  void remove (CORBA::ULong slot
               TAO_ENV_ARG_DECL_WITH_DEFAULTS);
  // Remove the typecode at slot i. Raises the "Bounds" exception.

  void _incr_refcnt (void);
  void  _decr_refcnt (void);
  // Increment and decrement ref counts.

#if !defined(__GNUC__) || __GNUC__ > 2 || __GNUC_MINOR__ >= 8
  typedef CORBA::ContextList_ptr _ptr_type;
  typedef CORBA::ContextList_var _var_type;
#endif /* __GNUC__ */
  // Useful for template programming.

private:
  // Not allowed.
  CORBA_ContextList (const CORBA_ContextList &);
  CORBA_ContextList &operator= (const CORBA_ContextList &);

  ACE_Atomic_Op<TAO_SYNCH_MUTEX, CORBA::ULong> ref_count_;
  // Reference counter.

  ACE_Unbounded_Queue<char *> ctx_list_;
  // Internal list of typecodes.
};

class TAO_DynamicInterface_Export CORBA_ContextList_var
{
public:
  CORBA_ContextList_var (void); // default constructor
  CORBA_ContextList_var (CORBA_ContextList_ptr);
  CORBA_ContextList_var (const CORBA_ContextList_var &); // copy constructor
  ~CORBA_ContextList_var (void); // destructor

  CORBA_ContextList_var &operator= (CORBA_ContextList_ptr);
  CORBA_ContextList_var &operator= (const CORBA_ContextList_var &);
  CORBA_ContextList_ptr operator-> (void) const;

  operator const CORBA_ContextList_ptr &() const;
  operator CORBA_ContextList_ptr &();
  // in, inout, out, _retn
  CORBA_ContextList_ptr in (void) const;
  CORBA_ContextList_ptr &inout (void);
  CORBA_ContextList_ptr &out (void);
  CORBA_ContextList_ptr _retn (void);
  CORBA_ContextList_ptr ptr (void) const;

private:
  CORBA_ContextList_ptr ptr_;
};

class TAO_DynamicInterface_Export CORBA_ContextList_out
{
  // = TITLE
  //    The T_out class for ContextList
  //
  // = DESCRIPTION
  //    As any other pseudo object ContextList must have a T_out class,
  //    the interface an semantics are specified in the CORBA spec.
  //
  // = NOTE
  //    We use CORBA_ContextList_ptr as the _ptr type instead of
  //    CORBA::ContextList_ptr, this is an attempt to reduced the cyclic
  //    dependencies in TAO.
  //
public:
  CORBA_ContextList_out (CORBA_ContextList_ptr &);
  CORBA_ContextList_out (CORBA_ContextList_var &);
  CORBA_ContextList_out (CORBA_ContextList_out &);
  CORBA_ContextList_out &operator= (CORBA_ContextList_out &);
  CORBA_ContextList_out &operator= (const CORBA_ContextList_var &);
  CORBA_ContextList_out &operator= (CORBA_ContextList_ptr);
  operator CORBA_ContextList_ptr &();
  CORBA_ContextList_ptr &ptr (void);
  CORBA_ContextList_ptr operator-> (void);

private:
  CORBA_ContextList_ptr &ptr_;
};

#if defined (__ACE_INLINE__)
# include "Context.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_CONTEXT_H */
