// -*- C++ -*-

//=============================================================================
/**
 *  @file    Object.h
 *
 *  $Id$
 *
 *   A "Object" is an entity that can be the target of an invocation
 *   using an ORB.  All CORBA objects provide this functionality.
 *   See the CORBA 2.6 specification for details.
 *
 *  @author  Portions Copyright 1994-1995 by Sun Microsystems Inc.
 *  @author  Portions Copyright 1997-2002 by Washington University
 */
//=============================================================================


#ifndef TAO_CORBA_OBJECT_H
#define TAO_CORBA_OBJECT_H

#include "ace/pre.h"

#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Policy_ForwardC.h"

class TAO_Stub;
class TAO_Abstract_ServantBase;
class TAO_Object_Proxy_Broker;

/**
 * @class CORBA_Object
 *
 * @brief Implementation of a CORBA object reference.
 *
 * All CORBA objects, both unconstrained and locality-constrained,
 * inherit from this class.  The interface is defined in the CORBA
 * specification and the C++ mapping.
 */
class TAO_Export CORBA_Object
{
public:

  /// Destructor.
  virtual ~CORBA_Object (void);

  /// Address of this variable used in _unchecked_narrow().
  static int _tao_class_id;

  /// Increment the ref count.
  static CORBA::Object_ptr _duplicate (CORBA::Object_ptr obj);

  /// Return a NULL object.
  static CORBA::Object_ptr _nil (void);

  /// No-op it is just here to simplify some templates.
  ACE_INLINE_FOR_GNUC
  static CORBA::Object_ptr _narrow (CORBA_Object_ptr obj
                                    ACE_ENV_ARG_DECL_WITH_DEFAULTS);
  static CORBA::Object_ptr _unchecked_narrow (CORBA_Object_ptr obj
                                              ACE_ENV_ARG_DECL_WITH_DEFAULTS);

  /// Used in the implementation of CORBA::Any
  static void _tao_any_destructor (void*);

  /// Uninlined part of the now-inlined CORBA::is_nil().
  static CORBA::Boolean is_nil_i (CORBA::Object_ptr obj);

  // These calls correspond to over-the-wire operations, or at least
  // do so in many common cases.  The normal implementation assumes a
  // particular simple, efficient, protocol-neutral interface for
  // making such calls, but may be overridden when it appears
  // appropriate.

  /// determine if we are of the type specified by the "logical_type_id"
  virtual CORBA::Boolean _is_a (const char *logical_type_id
                                ACE_ENV_ARG_DECL_WITH_DEFAULTS);

  /// The repository ID for the most derived class, this is an
  /// implementation method and does no remote invocations!
  virtual const char* _interface_repository_id (void) const;

  /// Is this object collocated with the servant?
  virtual CORBA::Boolean _is_collocated (void) const;

  /// Is this a local object?
  virtual CORBA::Boolean _is_local (void) const;

  virtual TAO_Abstract_ServantBase*_servant (void) const;

#if (TAO_HAS_MINIMUM_CORBA == 0)

  virtual CORBA::Boolean _non_existent (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);

  /// This method is deprecated in the CORBA 2.2 spec, we just return 0
  /// every time.
  virtual CORBA::ImplementationDef_ptr _get_implementation (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);


  /// Get info about the object from the Interface Repository.
  virtual CORBA_InterfaceDef_ptr _get_interface (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);


  // DII operations to create a request.
  //
  // The mapping for create_request is split into two forms,
  // corresponding to the two usage styles described in CORBA section
  // 6.2.1.

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
  // calls above ... that's how it can have a default implementation.

  /// DII operation to create a request.
  virtual CORBA::Request_ptr _request (const char *operation
                                       ACE_ENV_ARG_DECL_WITH_DEFAULTS);

#endif /* TAO_HAS_MINIMUM_CORBA */

#if (TAO_HAS_CORBA_MESSAGING == 1)

  CORBA::Policy_ptr _get_policy (
      CORBA::PolicyType type
      ACE_ENV_ARG_DECL_WITH_DEFAULTS);

  CORBA::Policy_ptr _get_client_policy (
      CORBA::PolicyType type
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

  /// Return the object key as an out parameter.  Caller should release
  /// return value when finished with it.
  virtual TAO_ObjectKey *_key (ACE_ENV_SINGLE_ARG_DECL);

  /**
   * Return a reference to the object key of profile in-use.
   * If there's no in-use profile, then the program will
   * probably crash.  This method does not create a new copy.
   */
  virtual const TAO_ObjectKey &_object_key (void);

  /// Downcasting this object pointer to some other derived class.
  /// This QueryInterface stuff only work for local object.
  virtual void * _tao_QueryInterface (ptr_arith_t type);

#if !defined(__GNUC__) || __GNUC__ > 2 || __GNUC_MINOR__ >= 8
  typedef CORBA_Object_ptr _ptr_type;
  typedef CORBA_Object_var _var_type;
#endif /* __GNUC__ */
  // Useful for template programming.

  // = Reference count managment.
  /// Increment the reference count.
  virtual void _add_ref (void);

  /// Decrement the reference count.
  virtual void _remove_ref (void);

  // = TAO extensions

  CORBA_Object (TAO_Stub *p = 0,
                CORBA::Boolean collocated = 0,
                TAO_Abstract_ServantBase *servant = 0);


  /// get the underlying stub object
  virtual TAO_Stub *_stubobj (void) const;

  /// Sets the proxy broker.
  virtual void _proxy_broker (TAO_Object_Proxy_Broker *proxy_broker);

  /// Gets the proxy broker.
  virtual TAO_Object_Proxy_Broker *_proxy_broker (void);

protected:

  /// Initializing a local object.
  CORBA_Object (int dummy);

private:

  // = Unimplemented methods
  CORBA_Object (const CORBA_Object &);
  CORBA_Object &operator = (const CORBA_Object &);

protected:

  /// Flag to indicate collocation.  It is 0 except for collocated
  /// objects.
  CORBA::Boolean is_collocated_;

  /// Servant pointer.  It is 0 except for collocated objects.
  TAO_Abstract_ServantBase *servant_;

  /// Specify whether this is a local object or not.
  CORBA::Boolean is_local_;

  /**
   * Pointer to the Proxy Broker i.e. the instance
   * that takes care of getting the right proxy
   * for performing a given call.
   */
  TAO_Object_Proxy_Broker *proxy_broker_;

private:

  /**
   * Pointer to the protocol-specific "object" containing important
   * profiling information regarding this proxy.
   * The protocol proxy is (potentially) shared among several
   * CORBA_Objects
   */
  TAO_Stub *protocol_proxy_;

};

class TAO_Export CORBA_Object_var
{
public:
  CORBA_Object_var (void); // default constructor
  CORBA_Object_var (CORBA::Object_ptr);
  CORBA_Object_var (const CORBA_Object_var &); // copy constructor
  ~CORBA_Object_var (void); // destructor

  CORBA_Object_var &operator= (CORBA::Object_ptr);
  CORBA_Object_var &operator= (const CORBA_Object_var &);
  CORBA::Object_ptr operator-> (void) const;

  /// in, inout, out, _retn
  operator const CORBA::Object_ptr &() const;
  operator CORBA::Object_ptr &();
  CORBA::Object_ptr in (void) const;
  CORBA::Object_ptr &inout (void);
  CORBA::Object_ptr &out (void);
  CORBA::Object_ptr _retn (void);
  CORBA::Object_ptr ptr (void) const;

  static CORBA::Object_ptr tao_duplicate (CORBA::Object_ptr);
  static void tao_release (CORBA::Object_ptr);
  static CORBA::Object_ptr tao_nil (void);
  static CORBA::Object_ptr tao_narrow (CORBA::Object * ACE_ENV_ARG_DECL_NOT_USED);
  static CORBA::Object * tao_upcast (void *);

private:
  CORBA::Object_ptr ptr_;
};

class TAO_Export CORBA_Object_out
{
public:
  CORBA_Object_out (CORBA::Object_ptr &);
  CORBA_Object_out (CORBA_Object_var &);
  CORBA_Object_out (const CORBA_Object_out &);
  CORBA_Object_out &operator= (const CORBA_Object_out &);
  CORBA_Object_out &operator= (const CORBA_Object_var &);
  CORBA_Object_out &operator= (CORBA::Object_ptr);
  operator CORBA::Object_ptr &();
  CORBA::Object_ptr &ptr (void);
  CORBA::Object_ptr operator-> (void);

private:
  CORBA::Object_ptr &ptr_;
};


/// This function pointer is set only when the Portable server library
/// is present.
extern TAO_Export TAO_Object_Proxy_Broker * (*_TAO_collocation_Object_Proxy_Broker_Factory_function_pointer) (
    CORBA::Object_ptr obj
  );


TAO_Export CORBA::Boolean
operator<< (TAO_OutputCDR&, const CORBA_Object*);

TAO_Export CORBA::Boolean
operator>> (TAO_InputCDR&, CORBA_Object*&);


#if defined (__ACE_INLINE__)
# include "tao/Object.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_CORBA_OBJECT_H */
