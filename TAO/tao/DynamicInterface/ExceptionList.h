// -*- C++ -*-
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
#include "ace/CORBA_macros.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


namespace CORBA
{
  class ExceptionList;
  typedef ExceptionList *ExceptionList_ptr;

  class TAO_DynamicInterface_Export ExceptionList
  {
    // = TITLE
    //    ExceptionList definition taken from CORBA v2.2 Feb 1998.
    //
    // = DESCRIPTION
    //    Maintains a list of TypeCodes for Exceptions.
  public:
    // = Intialization and termination methods.
    
    ExceptionList (void);
    // Constructor.
    
    ExceptionList (CORBA::ULong len,
                         CORBA::TypeCode_ptr *tc_list);
    // Constructor - initialize given a length and an array of
    // TypeCodes.
    
    ~ExceptionList (void);
    // Destructor.
    
    CORBA::ULong count (void);
    // Return the number of elements.

    ExceptionList_ptr _duplicate (void);
    // Increase the reference count.
    
    static ExceptionList_ptr _duplicate (ExceptionList *);
    // Increase the reference count in the spec defined manner.
    
    void _destroy (void);
    
    static ExceptionList_ptr _nil (void);
    
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
    ExceptionList (const ExceptionList &);
    ExceptionList &operator= (const ExceptionList &);
    
    ACE_Atomic_Op<TAO_SYNCH_MUTEX, CORBA::ULong> ref_count_;
    // Reference counter.
    
    ACE_Unbounded_Queue<CORBA::TypeCode_ptr> tc_list_;
    // Internal list of typecodes.
  };
  
  class TAO_DynamicInterface_Export ExceptionList_var
  {
    // = TITLE
    //    ExceptionList_var
    //
    // = DESCRIPTION
    //    Lifecycle management helper class for ExceptionList objects.
  public:
    ExceptionList_var (void);
    // Default constructor.
    
    ExceptionList_var (ExceptionList_ptr);
    
    ExceptionList_var (const ExceptionList_var &);
    // Copy constructor.
    
    ~ExceptionList_var (void);
    // Destructor.
    
    ExceptionList_var &operator= (ExceptionList_ptr);
    ExceptionList_var &operator= (const ExceptionList_var &);
    ExceptionList_ptr operator-> (void) const;
    
    operator const ExceptionList_ptr &() const;
    operator ExceptionList_ptr &();
    
    // in, inout, out, _retn.
    ExceptionList_ptr in (void) const;
    ExceptionList_ptr &inout (void);
    ExceptionList_ptr &out (void);
    ExceptionList_ptr _retn (void);
    ExceptionList_ptr ptr (void) const;
    
  private:
    ExceptionList_ptr ptr_;
  };
}  // End CORBA namespace

#if defined (__ACE_INLINE__)
# include "ExceptionList.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"

#endif /* TAO_CORBA_EXCEPTIONLIST_H */
