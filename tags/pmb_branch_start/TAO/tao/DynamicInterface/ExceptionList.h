// This may look like C, but it's really -*- C++ -*-
//
// $Id$

// ============================================================================
//
// = LIBRARY
//    DynamicInterface
//
// = FILENAME
//    ExceptionList.h
//
// = DESCRIPTION
//    This file defines the ExceptionList datatype used in
//    dynamic invocations.
//
// = AUTHOR
//     Copyright 1994-1995 by Sun Microsystems Inc.
//
// ============================================================================

#ifndef TAO_CORBA_EXCEPTIONLIST_H
#define TAO_CORBA_EXCEPTIONLIST_H
#include "ace/pre.h"

#include "dynamicinterface_export.h"
#include "tao/corbafwd.h"
#include "ace/Unbounded_Queue.h"
#include "ace/Atomic_Op.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class CORBA_ExceptionList;
typedef CORBA_ExceptionList *CORBA_ExceptionList_ptr;

class TAO_DynamicInterface_Export CORBA_ExceptionList
{
  // = TITLE
  //    ExceptionList definition taken from CORBA v2.2 Feb 1998.
  //
  // = DESCRIPTION
  //    Maintains a list of TypeCodes for Exceptions.
public:
  // = Intialization and termination methods.

  CORBA_ExceptionList (void);
  // Constructor.

  CORBA_ExceptionList (CORBA::ULong len,
                       CORBA::TypeCode_ptr *tc_list);
  // Constructor - initialize given a length and an array of
  // TypeCodes.

  ~CORBA_ExceptionList (void);
  // Destructor.

  CORBA::ULong count (void);
  // Return the number of elements.

  CORBA_ExceptionList_ptr _duplicate (void);
  // Increase the reference count.

  static CORBA_ExceptionList_ptr _duplicate (CORBA_ExceptionList *);
  // Increase the reference count in the spec defined manner.

  void _destroy (void);

  static CORBA_ExceptionList_ptr _nil (void);

  void add (CORBA::TypeCode_ptr tc);
  // Add a TypeCode to the list.

  void add_consume (CORBA::TypeCode_ptr tc);
  // Add and consume a TypeCode to the list.

  CORBA::TypeCode_ptr item (CORBA::ULong slot
                            ACE_ENV_ARG_DECL_WITH_DEFAULTS);
  // Return the typecode at slot i. Raises the "Bounds" exception.

  void remove (CORBA::ULong slot
               ACE_ENV_ARG_DECL_WITH_DEFAULTS);
  // Remove the typecode at slot i. Raises the "Bounds" exception.

  void  _incr_refcnt (void);
  void  _decr_refcnt (void);
  // Increment and decrement ref counts.

#if !defined(__GNUC__) || __GNUC__ > 2 || __GNUC_MINOR__ >= 8
  typedef CORBA::ExceptionList_ptr _ptr_type;
  typedef CORBA::ExceptionList_var _var_type;
#endif /* __GNUC__ */
  // Useful for template programming.

private:
  // = Not allowed.
  CORBA_ExceptionList (const CORBA_ExceptionList &);
  CORBA_ExceptionList &operator= (const CORBA_ExceptionList &);

  ACE_Atomic_Op<TAO_SYNCH_MUTEX, CORBA::ULong> ref_count_;
  // Reference counter.

  ACE_Unbounded_Queue<CORBA::TypeCode_ptr> tc_list_;
  // Internal list of typecodes.
};

class TAO_DynamicInterface_Export CORBA_ExceptionList_var
{
  // = TITLE
  //    CORBA_ExceptionList_var
  //
  // = DESCRIPTION
  //    Lifecycle management helper class for ExceptionList objects.
public:
  CORBA_ExceptionList_var (void);
  // Default constructor.

  CORBA_ExceptionList_var (CORBA_ExceptionList_ptr);

  CORBA_ExceptionList_var (const CORBA_ExceptionList_var &);
  // Copy constructor.

  ~CORBA_ExceptionList_var (void);
  // Destructor.

  CORBA_ExceptionList_var &operator= (CORBA_ExceptionList_ptr);
  CORBA_ExceptionList_var &operator= (const CORBA_ExceptionList_var &);
  CORBA_ExceptionList_ptr operator-> (void) const;

  operator const CORBA_ExceptionList_ptr &() const;
  operator CORBA_ExceptionList_ptr &();

  // in, inout, out, _retn.
  CORBA_ExceptionList_ptr in (void) const;
  CORBA_ExceptionList_ptr &inout (void);
  CORBA_ExceptionList_ptr &out (void);
  CORBA_ExceptionList_ptr _retn (void);
  CORBA_ExceptionList_ptr ptr (void) const;

private:
  CORBA_ExceptionList_ptr ptr_;
};

#if defined (__ACE_INLINE__)
# include "ExceptionList.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_CORBA_EXCEPTIONLIST_H */
