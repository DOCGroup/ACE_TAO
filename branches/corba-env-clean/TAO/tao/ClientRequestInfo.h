// -*- C++ -*-

//=============================================================================
/**
 * @file ClientRequestInfo.h
 *
 * $Id$
 *
 * This is the implementation of the
 * PortableInterceptor::ClientRequestInfo interface.
 *
 * @author Kirthika Parameswaran <kirthika@cs.wustl.edu>
 * @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================

#ifndef TAO_CLIENT_REQUEST_INFO_H
#define TAO_CLIENT_REQUEST_INFO_H

#include "ace/pre.h"

#include "corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

#if (TAO_HAS_INTERCEPTORS == 1)

#include "PortableInterceptorC.h"
#include "LocalObject.h"
#include "StringSeqC.h"
#include "Service_Context.h"

class TAO_GIOP_Invocation;

/**
 * @class TAO_ClientRequestInfo
 *
 * @brief Implementation of the PortableInterceptor::ClientRequestInfo
 *        interface.
 */
class TAO_Export TAO_ClientRequestInfo
  : public virtual PortableInterceptor::ClientRequestInfo,
    public virtual CORBA::LocalObject
{
public:

  /// Constructor.
  TAO_ClientRequestInfo (TAO_GIOP_Invocation *invocation,
                         CORBA::Object_ptr target);

  /// Return an ID unique to the current request.  This request ID may
  /// or may not be the same as the GIOP request ID.
  virtual CORBA::ULong request_id (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Return the operation name for the current request.
  virtual char * operation (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Return the list of arguments passed to the current operation.
  virtual Dynamic::ParameterList * arguments (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Return the list of exceptions the current operation is capable
  /// of throwing.
  virtual Dynamic::ExceptionList * exceptions (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual Dynamic::ContextList * contexts (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual Dynamic::RequestContext * operation_context (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Return the result of the current request.  If there is no return
  /// value then an Any with tk_void TypeCode is returned.  This is
  /// method is not valid for oneway operations.
  virtual CORBA::Any * result (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Returns true for a two-way operation, and false otherwise.
  virtual CORBA::Boolean response_expected (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException)) ;

#if TAO_HAS_CORBA_MESSAGING == 1
  /// Return the sync_scope policy value for the current one-way
  /// operation.  If the operation is not a one-way, a
  /// CORBA::BAD_INV_ORDER exception is thrown.
  virtual CORBA::Short sync_scope (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));
#endif  /* TAO_HAS_CORBA_MESSAGING == 1 */

  /// Return the reply status for the current request.  Statuses can
  /// be PortableInterceptor::SUCCESSFUL, SYSTEM_EXCEPTION,
  /// USER_EXCEPTION, LOCATION_FORWARD, LOCATION_FORWARD_PERMANENT,
  /// TRANSPORT_RETRY.
  virtual PortableInterceptor::ReplyStatus reply_status (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// If the reply status is PortableInterceptor::LOCATION_FORWARD or
  /// PortableInterceptor::LOCATION_FORWARD_PERMANENT, return the
  /// object reference to which the request was forwarded.
  virtual CORBA::Object_ptr forward_reference (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException)) ;

  virtual CORBA::Any * get_slot (
      PortableInterceptor::SlotId id
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::InvalidSlot));

  /// Return the IOP::ServiceContext with the given IOP::ServiceId
  /// from the request service context list.
  virtual IOP::ServiceContext * get_request_service_context (
      IOP::ServiceId id
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Return the IOP::ServiceContext with the given IOP::ServiceId
  /// from the reply service context list.
  virtual IOP::ServiceContext * get_reply_service_context (
      IOP::ServiceId id
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Return the (initial, non-forwarded, or permanently forwarded)
  /// object reference of the target.
  virtual CORBA::Object_ptr target (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Return the object reference for the current target.  The target
  /// may change in the even of a location forward.
  virtual CORBA::Object_ptr effective_target (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual IOP::TaggedProfile * effective_profile (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Return an Any containing the received exception, if any.
  /// Otherwise, throw a CORBA::BAD_INV_ORDER exception.
  /**
   * @note There is no trivial way to extract the exception from an Any.
   */
  virtual CORBA::Any * received_exception (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /**
   * @note This is TAO specific and was done to combat the previous
   *       problem to some extent.  Avoid this method whenever
   *       possible.
   */
  virtual CORBA::Exception * _received_exception (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Return the repository ID for the received exception.
  virtual char * received_exception_id (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Return the first IOP::TaggedComponent that matches the given
  /// IOP::ComponentId in the object reference for the current
  /// target.
  virtual IOP::TaggedComponent * get_effective_component (
      IOP::ComponentId id
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Return all IOP::TaggedComponent(s) that match the given
  /// IOP::ComponentId in the object reference for the current
  /// target.
  virtual IOP::TaggedComponentSeq * get_effective_components (
      IOP::ComponentId id
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Return the policy of the given type in effect for the current
  /// request.
  virtual CORBA::Policy_ptr get_request_policy (
      CORBA::PolicyType type
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Add the IOP::ServiceContext to the request (outgoing)
  /// IOP::ServiceContextList.
  virtual void add_request_service_context (
      const IOP::ServiceContext & service_context,
      CORBA::Boolean replace
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)
     ACE_THROW_SPEC ((CORBA::SystemException));

  /**
   * @name Stub helper methods
   *
   * The following methods are used in the implementation of the
   * Stubs, they are not part of the ClientRequestInfo interface, but
   * an extension used internally by TAO.
   */
  //@{

  /// Change the exception status.
  void exception (CORBA::Exception *exception);

  /// Set the flag that states whether or not a response is expected.
  /// For example, no response is expected in a one-way operation.
  void response_expected (CORBA::Boolean flag);

  /// Set the status of the received reply.
  void reply_status (int invoke_status);

  /// Extract the forward object reference from the
  /// PortableInterceptor::ForwardRequest exception, and set the reply
  /// status flag accordingly.
  void forward_reference (PortableInterceptor::ForwardRequest &exc);
  //@}

protected:

  /// Helper method to get the request and response service contexts.
  IOP::ServiceContext *get_service_context_i (
      TAO_Service_Context &service_context_list,
      IOP::ServiceId id
      TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

protected:

  /// Pointer to the GIOP invocation object.
  TAO_GIOP_Invocation *invocation_;

  /// Reference to the target object.
  CORBA::Object_ptr target_;

  /// Pointer to the caught exception.
  CORBA::Exception *caught_exception_;

  /// True if a two-way operation, false otherwise.
  CORBA::Boolean response_expected_;

  /// Reply status for the current request.
  PortableInterceptor::ReplyStatus reply_status_;

};

# if defined (__ACE_INLINE__)
#  include "ClientRequestInfo.inl"
# endif  /* __ACE_INLINE__ */

#endif /* TAO_HAS_INTERCEPTORS == 1 */

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"

#endif /* TAO_CLIENT_REQUEST_INFO_H */
