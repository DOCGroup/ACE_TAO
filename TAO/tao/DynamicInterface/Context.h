// -*- C++ -*-
// $Id$

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
  TAO_NAMESPACE_INLINE_FUNCTION void release (ContextList_ptr);
  TAO_NAMESPACE_INLINE_FUNCTION Boolean is_nil (ContextList_ptr);

  typedef TAO_Pseudo_Var_T<Context> Context_var;
  typedef TAO_Pseudo_Out_T<Context, Context_var> Context_out;

  typedef TAO_Pseudo_Var_T<ContextList> ContextList_var;
  typedef TAO_Pseudo_Out_T<ContextList, ContextList_var> ContextList_out;

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
    CORBA::ULong _incr_refcnt (void);
    CORBA::ULong _decr_refcnt (void);

    // = All the spec-required functions below will just throw a
    //   CORBA::NO_IMPLEMENT exception and do nothing else.

    const char *context_name (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS) const;

    CORBA::Context_ptr parent (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS) const;

    void create_child (const char *child_ctx_name,
                       CORBA::Context_out child_ctx
                       ACE_ENV_ARG_DECL_WITH_DEFAULTS);

    void set_one_value (const char *propname,
                        const CORBA::Any &propvalue
                        ACE_ENV_ARG_DECL_WITH_DEFAULTS);

    void set_values (CORBA::NVList_ptr values
                     ACE_ENV_ARG_DECL_WITH_DEFAULTS);

    void delete_values (const char *propname
                        ACE_ENV_ARG_DECL_WITH_DEFAULTS);

    void get_values (const char *start_scope,
                     CORBA::Flags op_flags,
                     const char *pattern,
                     CORBA::NVList_ptr &values
                     ACE_ENV_ARG_DECL_WITH_DEFAULTS);

    // Useful for template programming.
    typedef CORBA::Context_ptr _ptr_type;
    typedef CORBA::Context_var _var_type;

  private:
    /// Reference counting.
    CORBA::ULong refcount_;

    /// Protect the reference count.
    TAO_SYNCH_MUTEX refcount_lock_;
  };

  class TAO_DynamicInterface_Export ContextList
  {
    // = TITLE
    //    ContextList definition taken from CORBA v2.3a Dec 1998
    //
    // = DESCRIPTION
    //    Maintains a list of strings for Contexts.
  public:
    ContextList (void);
    // Constructor.

    ContextList (CORBA::ULong len,
                       char **ctx_list);
    // Constructor - initialize given a length and an array of
    // strings.

    ~ContextList (void);
    // Destructor.

    CORBA::ULong count (void);
    // Return the number of elements.

    ContextList_ptr _duplicate (void);
    // Increment the reference count.

    static ContextList_ptr _duplicate (ContextList *);
    // Increment the reference count.

    void _destroy (void);
    // Decrement the reference count and delete if it is 0.

    static ContextList_ptr _nil ();
    // Return null pointer of this type.

    void add (char *ctx);
    // Add a string to the list.

    void add_consume (char *ctx);
    // Add and consume a string to the list.

    char *item (CORBA::ULong slot
                ACE_ENV_ARG_DECL_WITH_DEFAULTS);
    // Return the typecode at slot i. Raises the "Bounds" exception.

    void remove (CORBA::ULong slot
                 ACE_ENV_ARG_DECL_WITH_DEFAULTS);
    // Remove the typecode at slot i. Raises the "Bounds" exception.

    void _incr_refcnt (void);
    void  _decr_refcnt (void);
    // Increment and decrement ref counts.

    // Useful for template programming.
    typedef CORBA::ContextList_ptr _ptr_type;
    typedef CORBA::ContextList_var _var_type;

  private:
  // Not allowed.
    ContextList (const ContextList &);
    ContextList &operator= (const ContextList &);

    ACE_Atomic_Op<TAO_SYNCH_MUTEX, CORBA::ULong> ref_count_;
    // Reference counter.

    ACE_Unbounded_Queue<char *> ctx_list_;
    // Internal list of typecodes.
  };
} // End CORBA namespace.

#if defined (__ACE_INLINE__)
# include "Context.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_CONTEXT_H */
