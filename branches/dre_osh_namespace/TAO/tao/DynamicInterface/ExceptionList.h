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
#include /**/ "ace/pre.h"

#include "dynamicinterface_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/corbafwd.h"
#include "tao/Pseudo_VarOut_T.h"

#include "ace/Unbounded_Queue.h"
#include "ace/Atomic_Op.h"
#include "ace/CORBA_macros.h"

#if defined (TAO_EXPORT_MACRO)
#undef TAO_EXPORT_MACRO
#endif
#define TAO_EXPORT_MACRO TAO_DynamicInterface_Export

namespace CORBA
{
  TAO_NAMESPACE_INLINE_FUNCTION void release (ExceptionList_ptr);
  TAO_NAMESPACE_INLINE_FUNCTION Boolean is_nil (ExceptionList_ptr);

  typedef TAO_Pseudo_Var_T<ExceptionList> ExceptionList_var;
  typedef TAO_Pseudo_Out_T<ExceptionList, ExceptionList_var> ExceptionList_out;

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
    
    void _incr_refcnt (void);
    void _decr_refcnt (void);
    // Increment and decrement ref counts.
    
    typedef CORBA::ExceptionList_ptr _ptr_type;
    typedef CORBA::ExceptionList_var _var_type;
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
}  // End CORBA namespace

#if defined (__ACE_INLINE__)
# include "ExceptionList.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_CORBA_EXCEPTIONLIST_H */
