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

#include "tao/IOPC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/CORBA_methods.h"
#include "tao/Policy_ForwardC.h"
#include "tao/Pseudo_VarOut_T.h"
#include "tao/Object_Argument_T.h"
#include "tao/Arg_Traits_T.h"
#include "tao/Any_Insert_Policy_T.h"
#include "tao/Configurable_Refcount.h"

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
  typedef TAO_Pseudo_Out_T<Object> Object_out;

  template<>
  TAO_Export Boolean
  is_nil<> (Object_ptr);

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
    static CORBA::Object_ptr _narrow (CORBA::Object_ptr obj);

    // These calls correspond to over-the-wire operations, or at least
    // do so in many common cases.  The normal implementation assumes a
    // particular simple, efficient, protocol-neutral interface for
    // making such calls, but may be overridden when it appears
    // appropriate.

    /// Determine if we are of the type specified by the "logical_type_id"
    virtual CORBA::Boolean _is_a (const char *logical_type_id);

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
    virtual CORBA::ULong _hash (CORBA::ULong maximum);

    /**
     * Try to determine if this object is the same as other_obj.  This
     * method relies on the representation of the object reference's
     * private state.  Since that changes easily (when different ORB
     * protocols are in use) there is no default implementation.
     */
    virtual CORBA::Boolean _is_equivalent (CORBA::Object_ptr other_obj);

#if (TAO_HAS_MINIMUM_CORBA == 0)

    virtual CORBA::Boolean _non_existent (void);

#if ! defined (CORBA_E_COMPACT) && ! defined (CORBA_E_MICRO)
    /// Get info about the object from the Interface Repository.
    virtual InterfaceDef_ptr _get_interface (void);

    /// Get info about the object from the Interface Repository.
    virtual CORBA::Object_ptr _get_component (void);
#endif

    /// Get the repository id.
    virtual char * _repository_id (void);

#if ! defined (CORBA_E_COMPACT) && ! defined (CORBA_E_MICRO)
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
                                  CORBA::Flags req_flags);

    virtual void _create_request (CORBA::Context_ptr ctx,
                                  const char *operation,
                                  CORBA::NVList_ptr arg_list,
                                  CORBA::NamedValue_ptr result,
                                  CORBA::ExceptionList_ptr exclist,
                                  CORBA::ContextList_ptr ctxtlist,
                                  CORBA::Request_ptr &request,
                                  CORBA::Flags req_flags);

    // The default implementation of this method uses the same simple,
    // multi-protocol remote invocation interface as is assumed by the
    // calls above ... that's how it can have a default
    // implementation.

    /// DII operation to create a request.
    virtual CORBA::Request_ptr _request (const char *operation);
#endif

#endif /* TAO_HAS_MINIMUM_CORBA */

#if (TAO_HAS_CORBA_MESSAGING == 1)

    CORBA::Policy_ptr _get_policy (CORBA::PolicyType type);

    CORBA::Policy_ptr _get_cached_policy (TAO_Cached_Policy_Type type);

    CORBA::Object_ptr _set_policy_overrides (
      const CORBA::PolicyList & policies,
      CORBA::SetOverrideType set_add);

    CORBA::PolicyList * _get_policy_overrides (
      const CORBA::PolicyTypeSeq & types);

    CORBA::Boolean _validate_connection (
      CORBA::PolicyList_out inconsistent_policies);

#endif /* TAO_HAS_CORBA_MESSAGING == 1 */

    virtual CORBA::ORB_ptr _get_orb (void);

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

    /// Get the refcount
    virtual CORBA::ULong _refcount_value (void) const;
    //@}

    // Useful for template programming.
    typedef Object_ptr _ptr_type;
    typedef Object_var _var_type;
    typedef Object_out _out_type;

    //@} End of CORBA specific methods

  public:
    /// Marshalling operator used by the stub code. A long story why
    /// the stub code uses this, let us keep it short here.
    static CORBA::Boolean marshal (const Object_ptr x,
                                   TAO_OutputCDR &cdr);

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
    virtual TAO::ObjectKey *_key (void);

    /// Constructor
    Object (TAO_Stub *p,
            CORBA::Boolean collocated = false,
            TAO_Abstract_ServantBase *servant = 0,
            TAO_ORB_Core *orb_core = 0);

    Object (IOP::IOR *ior, TAO_ORB_Core *orb_core);

    /// Get the underlying stub object.
    virtual TAO_Stub *_stubobj (void) const;
    virtual TAO_Stub *_stubobj (void);

    /// Set the proxy broker.
    virtual void _proxy_broker (TAO::Object_Proxy_Broker *proxy_broker);

  public:

    /// Allows us to forbid marshaling of local interfaces.
    virtual CORBA::Boolean marshal (TAO_OutputCDR &cdr);

#if defined (GEN_OSTREAM_OPS)

    /// Used by optionally generated ostream operators for interface
    /// to output the actual repo id for debugging.
    static std::ostream& _tao_stream (std::ostream &strm,
                                      const Object_ptr _tao_objref);
    virtual std::ostream& _tao_stream_v (std::ostream &strm) const;

#endif /* GEN_OSTREAM_OPS */

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

    /// Can this object be stringified?
    virtual bool can_convert_to_ior (void) const;

    /// A hook to allow users to provide custom object stringification.
    /// @note This method is intended to be used by classes that
    /// implement Smart Proxies and no others.
    virtual char* convert_to_ior (bool use_omg_ior_format,
                                  const char* ior_prefix) const;

    /// Wrapper for _remove_ref(), naming convention for
    /// templatizing.
    void _decr_refcount (void);

  protected:
    /// Initializing a local object.
    Object (int dummy = 0);

    /// Convenience accessor for the object proxy broker of the
    /// underlying stub.
    TAO::Object_Proxy_Broker *proxy_broker () const;

    /// Number of outstanding references to this object.
    TAO_Configurable_Refcount refcount_;

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
    TAO_ORB_Core * orb_core_;

    /**
     * Pointer to the protocol-specific "object" containing important
     * profiling information regarding this proxy.
     * The protocol proxy is (potentially) shared among several
     * Objects
     */
    TAO_Stub * protocol_proxy_;

    /// Protect reference count manipulation from race conditions.
    /**
     * This lock is only instantiated for unconstrained objects.  The
     * reason for this is that locality-constrained objects that do
     * not require reference counting (the default) may be
     * instantiated in the critical path.
     */
    ACE_Lock * object_init_lock_;
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
                                 TAO::Any_Insert_Policy_CORBA_Object>
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

  template<>
  struct TAO_Export In_Object_Argument_Cloner_T<CORBA::InterfaceDef_ptr>
  {
    static void duplicate(CORBA::InterfaceDef_ptr objref);
    static void release(CORBA::InterfaceDef_ptr objref);
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

#if defined (GEN_OSTREAM_OPS)

TAO_Export std::ostream&
operator<< (std::ostream&, CORBA::Object_ptr);

#endif /* GEN_OSTREAM_OPS */

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/Object.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_CORBA_OBJECT_H */
