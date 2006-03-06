// -*- C++ -*-

//=============================================================================
/**
 *  @file    Object.h
 *
 *  $Id$
 *
 *   A "Object" is an entity that can be the target of an invocation
 *   using an ORB.  All CORBA objects provide this functionality.
 *   See the CORBA 3.x specification for details.
 *
 *  @author  Portions Copyright 1994-1995 by Sun Microsystems Inc.
 *  @author  Portions Copyright 1997-2002 by Washington University
 */
//=============================================================================

#ifndef TAO_CORBA_OBJECT_H
#define TAO_CORBA_OBJECT_H

#include /**/ "ace/pre.h"

#include "tao/IOP_IORC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/CORBA_methods.h"
#include "tao/Policy_ForwardC.h"
#include "tao/Pseudo_VarOut_T.h"
#include "tao/Object_Argument_T.h"
#include "tao/Arg_Traits_T.h"
#include "tao/Any_Insert_Policy_T.h"

#if defined (HPUX) && defined (IOR)
   /* HP-UX 11.11 defines IOR in /usr/include/pa/inline.h
      and we don't want that definition.  See IOP_IORC.h. */
# undef IOR
#endif /* HPUX && IOR */

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_Lock;
ACE_END_VERSIONED_NAMESPACE_DECL

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Stub;
class TAO_Abstract_ServantBase;
class TAO_ORB_Core;

namespace TAO
{
  class ObjectKey;
  class Object_Proxy_Broker;
}

namespace CORBA
{
  class InterfaceDef;
  typedef InterfaceDef *InterfaceDef_ptr;

  class ImplementationDef;
  typedef ImplementationDef *ImplementationDef_ptr;

  class Context;
  typedef Context *Context_ptr;

  class Request;
  typedef Request *Request_ptr;

  class NVList;
  typedef NVList *NVList_ptr;

  class NamedValue;
  typedef NamedValue * NamedValue_ptr;

  typedef ULong Flags;

  class ExceptionList;
  typedef ExceptionList *ExceptionList_ptr;

  class ContextList;
  typedef ContextList *ContextList_ptr;

  class Object;
  typedef Object *Object_ptr;
  typedef TAO_Pseudo_Var_T<Object> Object_var;
  typedef TAO_Pseudo_Out_T<Object, Object_var> Object_out;

  /**
   * @class Object
   *
   * @brief Implementation of a CORBA object reference.
   *
   * All CORBA objects, both unconstrained and locality-constrained,
   * inherit from this class.  The interface is defined in the CORBA
   * specification and the C++ mapping.
   */
  class TAO_Export Object
  {
  public:

    /// Destructor.
    virtual ~Object (void);

    /**
     * @name Spec defined methods
     *
     * These methods are defined here since they are required by the
     * CORBA spec in a form specified by the C++ mapping.
     */
    //@{
    /// Increment the ref count.
    static CORBA::Object_ptr _duplicate (CORBA::Object_ptr obj);

    /// Return a NULL object.
    static CORBA::Object_ptr _nil (void);

    /// No-op it is just here to simplify some templates.
    static CORBA::Object_ptr _narrow (CORBA::Object_ptr obj
                                      ACE_ENV_ARG_DECL_WITH_DEFAULTS);

    // These calls correspond to over-the-wire operations, or at least
    // do so in many common cases.  The normal implementation assumes a
    // particular simple, efficient, protocol-neutral interface for
    // making such calls, but may be overridden when it appears
    // appropriate.

    /// Determine if we are of the type specified by the "logical_type_id"
    virtual CORBA::Boolean _is_a (const char *logical_type_id
                                  ACE_ENV_ARG_DECL_WITH_DEFAULTS);

    /// The repository ID for the most derived class, this is an
    /// implementation method and does no remote invocations!
    virtual const char* _interface_repository_id (void) const;


    /**
     * Return a (potentially non-unique) hash value for this object.
     * This method relies on the representation of the object
     * reference's private state.  Since that changes easily (when
     * different ORB protocols are in use) there is no default
     * implementation.
     */
    virtual CORBA::ULong _hash (CORBA::ULong maximum
                                ACE_ENV_ARG_DECL_WITH_DEFAULTS);

    /**
     * Try to determine if this object is the same as other_obj.  This
     * method relies on the representation of the object reference's
     * private state.  Since that changes easily (when different ORB
     * protocols are in use) there is no default implementation.
     */
    virtual CORBA::Boolean _is_equivalent (CORBA::Object_ptr other_obj
                                           ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC (());

#if (TAO_HAS_MINIMUM_CORBA == 0)

    virtual CORBA::Boolean _non_existent (
        ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
      );

    /// This method is deprecated in the CORBA 2.2 spec, we just return 0
    /// every time.
    virtual CORBA::ImplementationDef_ptr _get_implementation (
        ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
      );

    /// Get info about the object from the Interface Repository.
    virtual InterfaceDef_ptr _get_interface (
        ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
      );

    /// Get info about the object from the Interface Repository.
    virtual CORBA::Object_ptr _get_component (
        ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
      );

    /// Get the repository id.
    virtual char * _repository_id (
        ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
    );

    // DII operations to create a request.
    //
    // The mapping for create_request is split into two forms,
    // corresponding to the two usage styles described in CORBA
    // section 6.2.1.

    virtual void _create_request (CORBA::Context_ptr ctx,
                                  const char *operation,
                                  CORBA::NVList_ptr arg_list,
                                  CORBA::NamedValue_ptr result,
                                  CORBA::Request_ptr &request,
                                  CORBA::Flags req_flags
                                  ACE_ENV_ARG_DECL_WITH_DEFAULTS);

    virtual void _create_request (CORBA::Context_ptr ctx,
                                  const char *operation,
                                  CORBA::NVList_ptr arg_list,
                                  CORBA::NamedValue_ptr result,
                                  CORBA::ExceptionList_ptr exclist,
                                  CORBA::ContextList_ptr ctxtlist,
                                  CORBA::Request_ptr &request,
                                  CORBA::Flags req_flags
                                  ACE_ENV_ARG_DECL_WITH_DEFAULTS);

    // The default implementation of this method uses the same simple,
    // multi-protocol remote invocation interface as is assumed by the
    // calls above ... that's how it can have a default
    // implementation.

    /// DII operation to create a request.
    virtual CORBA::Request_ptr _request (const char *operation
                                         ACE_ENV_ARG_DECL_WITH_DEFAULTS);

#endif /* TAO_HAS_MINIMUM_CORBA */

#if (TAO_HAS_CORBA_MESSAGING == 1)

    CORBA::Policy_ptr _get_policy (CORBA::PolicyType type
                                   ACE_ENV_ARG_DECL_WITH_DEFAULTS);

    CORBA::Policy_ptr _get_cached_policy (TAO_Cached_Policy_Type type
                                          ACE_ENV_ARG_DECL_WITH_DEFAULTS);

    CORBA::Object_ptr _set_policy_overrides (
      const CORBA::PolicyList & policies,
      CORBA::SetOverrideType set_add
      ACE_ENV_ARG_DECL_WITH_DEFAULTS);

    CORBA::PolicyList * _get_policy_overrides (
      const CORBA::PolicyTypeSeq & types
      ACE_ENV_ARG_DECL_WITH_DEFAULTS);

    CORBA::Boolean _validate_connection (
      CORBA::PolicyList_out inconsistent_policies
      ACE_ENV_ARG_DECL_WITH_DEFAULTS);

#endif /* TAO_HAS_CORBA_MESSAGING == 1 */

    virtual CORBA::ORB_ptr _get_orb (
        ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
      );

    /**
     * @name Reference Count Managment
     *
     * These are the standard CORBA object reference count manipulations
     * methods.
     */
    //@{
    /// Increment the reference count.
    virtual void _add_ref (void);

    /// Decrement the reference count.
    virtual void _remove_ref (void);
    //@}

    // Useful for template programming.
    typedef Object_ptr _ptr_type;
    typedef Object_var _var_type;

    //@} End of CORBA specific methods


    /**
     * @name Methods that are TAO specific.
     *
     * These methods are defined here as helper functions to be used
     * by other parts of TAO. Theoretically they shold all start with
     * tao_. But we have deviated from that principle.
     */

    /// Marshalling operator used by the stub code. A long story why
    /// the stub code uses this, let us keep it short here.
    static CORBA::Boolean marshal (const Object_ptr obj,
                                   TAO_OutputCDR &strm);

    /// Accessor for the cached servant reference held on the stub
    /// if this object is collocated
    virtual TAO_Abstract_ServantBase *_servant (void) const;

    /// Is this object collocated with the servant?
    /// Note this does not return this->is_collocated_ but will instead
    /// query the underlying stub for its collocation status
    virtual CORBA::Boolean _is_collocated (void) const;

    /// Is this a local object?
    virtual CORBA::Boolean _is_local (void) const;

    /// Used in the implementation of CORBA::Any
    static void _tao_any_destructor (void*);

    /// Uninlined part of the now-inlined CORBA::is_nil().
    static CORBA::Boolean is_nil_i (CORBA::Object_ptr obj);

    /// Helper function for reading contents of an IOR
    static void tao_object_initialize (Object *);

    /// Return the object key as an out parameter.  Caller should release
    /// return value when finished with it.
    virtual TAO::ObjectKey *_key (ACE_ENV_SINGLE_ARG_DECL);

    /// Constructor
    Object (TAO_Stub *p,
            CORBA::Boolean collocated = 0,
            TAO_Abstract_ServantBase *servant = 0,
            TAO_ORB_Core *orb_core = 0);

    Object (IOP::IOR *ior,
            TAO_ORB_Core *orb_core = 0);

    /// Get the underlying stub object.
    virtual TAO_Stub *_stubobj (void) const;
    virtual TAO_Stub *_stubobj (void);

    /// Set the proxy broker.
    virtual void _proxy_broker (TAO::Object_Proxy_Broker *proxy_broker);

  public:

    /// Allows us to forbid marshaling of local interfaces.
    virtual CORBA::Boolean marshal (TAO_OutputCDR &cdr);

    /// Accessor to the flag..
    CORBA::Boolean is_evaluated (void) const;

    /// Mutator for setting the servant in collocated cases.
    /**
     * This is used by the Object_Adapter to set the servant for
     * collocated cases and only when the object is initialized. The
     * object initialization takes place when IOR's are lazily
     * evaluated.
     */
    void set_collocated_servant (TAO_Abstract_ServantBase *);

    /// Accessor for the ORB_Core..
    TAO_ORB_Core *orb_core (void) const;

    /// Accessors for the underlying IOP::IOR's.
    /**
     * The steal_ior () call basically relinquishes the ownership of
     * the IOR. This is useful for cases when one wants to initialize
     * a new CORBA Object
     */
    IOP::IOR *steal_ior (void);

    const IOP::IOR &ior (void) const;

    //@} End of TAO-specific methods..

  protected:

    /// Initializing a local object.
    Object (int dummy = 0);

    /// Convenience accessor for the object proxy broker of the
    /// underlying stub.
    TAO::Object_Proxy_Broker *proxy_broker () const;

  private:

    // = Unimplemented methods
    Object (const Object &);
    Object &operator = (const Object &);

  private:

    /// Specify whether this is a local object or not.
    CORBA::Boolean is_local_;

    /// Flag to indicate whether the IOP::IOR has been evaluated fully.
    CORBA::Boolean is_evaluated_;

    /// If the IOR hasnt been evaluated fully, then the contents of
    /// the IOR that we received  should be in here!
    IOP::IOR_var ior_;

    /// Cached pointer of our ORB_Core
    /**
     * Be aware that this pointer can be zero or not. In fact there are two
     * ways to get this pointer filled:
     * - If CORBA::Object is constructed through one of the constructors with a
     * stub and null ORB_Core, we use the orb_core from the stub to fill
     * this pointer
     * - If the other constructor, which uses IOP::IOR is used, a stub
     * needs to be created first (i.e., the IOR needs to be evaluated first),
     * in which case the ORB_Core would be null. The orb_core pointer then
     * needs to be accessed from the stub and passed back as part of
     * _get_orb().
     */
    TAO_ORB_Core *orb_core_;

    /**
     * Pointer to the protocol-specific "object" containing important
     * profiling information regarding this proxy.
     * The protocol proxy is (potentially) shared among several
     * Objects
     */
    TAO_Stub * protocol_proxy_;

    /// Number of outstanding references to this object.
    CORBA::ULong refcount_;

    /// Protect reference count manipulation from race conditions.
    /**
     * This lock is only instantiated for unconstrained objects.  The
     * reason for this is that locality-constrained objects that do
     * not require reference counting (the default) may be
     * instantiated in the critical path.
     */
    ACE_Lock * refcount_lock_;
  };
}   // End CORBA namespace.

namespace TAO
{
  template<>
  class TAO_Export Arg_Traits<CORBA::Object>
    : public Object_Arg_Traits_T<CORBA::Object_ptr,
                                 CORBA::Object_var,
                                 CORBA::Object_out,
                                 TAO::Objref_Traits<CORBA::Object>,
                                 TAO::Any_Insert_Policy_CORBA_Object <CORBA::Object_ptr> >
  {
  };

  template<>
  struct TAO_Export Objref_Traits<CORBA::Object>
  {
    static CORBA::Object_ptr duplicate (CORBA::Object_ptr);
    static void release (CORBA::Object_ptr);
    static CORBA::Object_ptr nil (void);
    static CORBA::Boolean marshal (const CORBA::Object_ptr p,
                                   TAO_OutputCDR & cdr);
  };
}

/// This function pointer is set only when the Portable server
/// library is present.
extern
  TAO_Export TAO::Object_Proxy_Broker *
  (*_TAO_Object_Proxy_Broker_Factory_function_pointer) (void);

TAO_Export CORBA::Boolean
operator<< (TAO_OutputCDR&, const CORBA::Object*);

TAO_Export CORBA::Boolean
operator>> (TAO_InputCDR&, CORBA::Object *&);

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/Object.i"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_CORBA_OBJECT_H */
