// -*- C++ -*-

//=============================================================================
/**
 *  @file    Context.h
 *
 *  $Id$
 *
 *  Header file for CORBA Context class.
 *
 *  @author  Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_CONTEXT_H
#define TAO_CONTEXT_H

#include /**/ "ace/pre.h"

#include "tao/DynamicInterface/dynamicinterface_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/DynamicInterface/DII_CORBA_methods.h"
#include "tao/AnyTypeCode/AnyTypeCode_methods.h"

#include "tao/Pseudo_VarOut_T.h"
#include "tao/orbconf.h"
#include "tao/Basic_Types.h"
#include "tao/ORB_Constants.h"
#include "tao/default_environment.h"

#include "ace/Unbounded_Queue.h"
#include "ace/Atomic_Op.h"
#include "ace/Synch_Traits.h"

#if defined (TAO_EXPORT_MACRO)
#undef TAO_EXPORT_MACRO
#endif
#define TAO_EXPORT_MACRO TAO_DynamicInterface_Export

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace CORBA
{
  typedef ULong Flags;

  class NVList;
  typedef NVList *NVList_ptr;

  class Context;
  typedef Context * Context_ptr;

  class ContextList;
  typedef ContextList * ContextList_ptr;

  typedef TAO_Pseudo_Var_T<Context> Context_var;
  typedef TAO_Pseudo_Out_T<Context> Context_out;

  typedef TAO_Pseudo_Var_T<ContextList> ContextList_var;
  typedef TAO_Pseudo_Out_T<ContextList> ContextList_out;

  /**
   * @class Context
   *
   * @brief CORBA::Context
   *
   * TAO's minimal implementation of the Context interface.  Since
   * Contexts are inherently un-typesafe, there use is deprecated and
   * the feature may eventaully disappear from CORBA.  It is
   * implemented only to make the arg list of
   * CORBA::Object::_create_request() compliant. The only (pointer)
   * value that should be passed is 0.
   */
  class TAO_DynamicInterface_Export Context
  {
  public:

    Context (void);

    ~Context (void);

    // = Pseudo-object methods
    static Context *_duplicate (Context*);
    static Context *_nil (void);

    // = Reference counting.
    CORBA::ULong _incr_refcount (void);
    CORBA::ULong _decr_refcount (void);

    // = All the spec-required functions below will just throw a
    //   CORBA::NO_IMPLEMENT exception and do nothing else.

    const char *context_name (void) const;

    CORBA::Context_ptr parent (void) const;

    void create_child (const char *child_ctx_name,
                       CORBA::Context_out child_ctx);

    void set_one_value (const char *propname, const CORBA::Any &propvalue);

    void set_values (CORBA::NVList_ptr values);

    void delete_values (const char *propname);

    void get_values (const char *start_scope,
                     CORBA::Flags op_flags,
                     const char *pattern,
                     CORBA::NVList_ptr &values);

    // Useful for template programming.
    typedef CORBA::Context_ptr _ptr_type;
    typedef CORBA::Context_var _var_type;
    typedef CORBA::Context_out _out_type;

  private:
    /// Reference counter.
    ACE_Atomic_Op<TAO_SYNCH_MUTEX, CORBA::ULong> refcount_;
  };

  /**
   * @class ContextList
   *
   * @brief ContextList definition taken from CORBA v2.3a Dec 1998
   *
   * Maintains a list of strings for Contexts.
   */
  class TAO_DynamicInterface_Export ContextList
  {
  public:
    /// Constructor.
    ContextList (void);

    /// Constructor - initialize given a length and an array of
    /// strings.
    ContextList (CORBA::ULong len, char **ctx_list);

    /// Destructor.
    ~ContextList (void);

    /// Return the number of elements.
    CORBA::ULong count (void);

    /// Increment the reference count.
    ContextList_ptr _duplicate (void);

    /// Increment the reference count.
    static ContextList_ptr _duplicate (ContextList *);

    /// Decrement the reference count and delete if it is 0.
    void _destroy (void);

    /// Return null pointer of this type.
    static ContextList_ptr _nil ();

    /// Add a string to the list.
    void add (char *ctx);

    /// Add and consume a string to the list.
    void add_consume (char *ctx);

    /// Return the typecode at slot i. Raises the "Bounds" exception.
    char *item (CORBA::ULong slot);

    /// Remove the typecode at slot i. Raises the "Bounds" exception.
    void remove (CORBA::ULong slot);

    /// Increment and decrement ref counts.
    void _incr_refcount (void);
    void  _decr_refcount (void);

    // Useful for template programming.
    typedef CORBA::ContextList_ptr _ptr_type;
    typedef CORBA::ContextList_var _var_type;
    typedef CORBA::ContextList_out _out_type;

  private:
    // Not allowed.
    ContextList (const ContextList &);
    ContextList &operator= (const ContextList &);

    /// Reference counter.
    ACE_Atomic_Op<TAO_SYNCH_MUTEX, CORBA::ULong> ref_count_;

    /// Internal list of typecodes.
    ACE_Unbounded_Queue<char *> ctx_list_;
  };
} // End CORBA namespace.

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/DynamicInterface/Context.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_CONTEXT_H */
