// -*- C++ -*-

//=============================================================================
/**
 *  @file    ExceptionList.h
 *
 *  This file defines the ExceptionList datatype used in
 *  dynamic invocations.
 *
 *  @author  Copyright 1994-1995 by Sun Microsystems Inc.
 */
//=============================================================================

#ifndef TAO_CORBA_EXCEPTIONLIST_H
#define TAO_CORBA_EXCEPTIONLIST_H

#include /**/ "ace/pre.h"

#include "tao/DynamicInterface/dynamicinterface_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/DynamicInterface/DII_CORBA_methods.h"
#include "tao/AnyTypeCode/AnyTypeCode_methods.h"
#include "tao/orbconf.h"
#include "tao/Basic_Types.h"
#include "tao/default_environment.h"
#include "tao/Pseudo_VarOut_T.h"

#include "ace/Unbounded_Queue.h"
#include <atomic>

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace CORBA
{
  class ExceptionList;
  typedef ExceptionList * ExceptionList_ptr;

  typedef TAO_Pseudo_Var_T<ExceptionList> ExceptionList_var;
  typedef TAO_Pseudo_Out_T<ExceptionList> ExceptionList_out;

  /**
   * @class ExceptionList
   *
   * @brief ExceptionList definition taken from CORBA v2.2 Feb 1998.
   *
   * Maintains a list of TypeCodes for Exceptions.
   */
  class TAO_DynamicInterface_Export ExceptionList
  {
  public:
    /// Constructor.
    ExceptionList ();

    /// Constructor - initialize given a length and an array of
    /// TypeCodes.
    ExceptionList (CORBA::ULong len, CORBA::TypeCode_ptr *tc_list);

    /// Destructor.
    ~ExceptionList ();

    /// Return the number of elements.
    CORBA::ULong count ();

    /// Increase the reference count.
    ExceptionList_ptr _duplicate ();

    /// Increase the reference count in the spec defined manner.
    static ExceptionList_ptr _duplicate (ExceptionList *);

    void _destroy ();

    static ExceptionList_ptr _nil ();

    /// Add a TypeCode to the list.
    void add (CORBA::TypeCode_ptr tc);

    /// Add and consume a TypeCode to the list.
    void add_consume (CORBA::TypeCode_ptr tc);

    /// Return the typecode at slot i. Raises the "Bounds" exception.
    CORBA::TypeCode_ptr item (CORBA::ULong slot);

    /// Remove the typecode at slot i. Raises the "Bounds" exception.
    void remove (CORBA::ULong slot);

    /// Increment and decrement ref counts.
    void _incr_refcount ();
    void _decr_refcount ();

    /// Useful for template programming.
    typedef CORBA::ExceptionList_ptr _ptr_type;
    typedef CORBA::ExceptionList_var _var_type;
    typedef CORBA::ExceptionList_out _out_type;

  private:
    // = Not allowed.
    ExceptionList (const ExceptionList &);
    ExceptionList &operator= (const ExceptionList &);

    /// Reference counter.
    std::atomic<uint32_t> refcount_;

    /// Internal list of typecodes.
    ACE_Unbounded_Queue<CORBA::TypeCode_ptr> tc_list_;
  };
}  // End CORBA namespace

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/DynamicInterface/ExceptionList.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_CORBA_EXCEPTIONLIST_H */
