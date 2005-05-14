// -*- C++ -*-
//=============================================================================
/**
 * @file ClientRequestInfo_i.h
 *
 * $Id$
 *
 * This is the implementation of the
 * PortableInterceptor::ClientRequestInfo interface.
 *
 * @author Ossama Othman <ossama@dre.vanderbilt.edu>
 * @author Kirthika Parameswaran <kirthika@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_CLIENT_REQUEST_INFO_I_H
#define TAO_CLIENT_REQUEST_INFO_I_H

#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if (TAO_HAS_INTERCEPTORS == 1)

#include "tao/Invocation_Utils.h"
#include "tao/PICurrent.h"
#include "tao/PICurrent_Copy_Callback.h"

class TAO_Service_Context;

namespace TAO
{
  class Invocation_Base;
}

namespace CORBA
{
  class AbstractBase;
  typedef AbstractBase *AbstractBase_ptr;
}

/**
 * @class TAO_ClientRequestInfo_i
 *
 * @brief Implementation of the PortableInterceptor::ClientRequestInfo
 *        interface.
 */
class TAO_Export TAO_ClientRequestInfo_i
{
public:

  /// Constructor from concrete interface.
  TAO_ClientRequestInfo_i (TAO::Invocation_Base *invocation);

  /// Constructor from abstract interface.
  TAO_ClientRequestInfo_i (TAO::Invocation_Base *invocation,
                           CORBA::AbstractBase_ptr abstract_target,
                           CORBA::Boolean response_expected = 1);

  /// Destructor.
  virtual ~TAO_ClientRequestInfo_i (void);

  /// Return an ID unique to the current request.  This request ID may
  /// or may not be the same as the GIOP request ID.
  CORBA::ULong request_id (
      ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Return the operation name for the current request.
  char * operation (
      ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Return the list of arguments passed to the current operation.
  virtual Dynamic::ParameterList * arguments (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Return the list of exceptions the current operation is capable
  /// of throwing.
  virtual Dynamic::ExceptionList * exceptions (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  Dynamic::ContextList * contexts (
      ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  Dynamic::RequestContext * operation_context (
      ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Return the result of the current request.  If there is no return
  /// value then an Any with tk_void TypeCode is returned.  This is
  /// method is not valid for oneway operations.
  virtual CORBA::Any * result (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Returns true for a two-way operation, and false otherwise.
  CORBA::Boolean response_expected (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException)) ;

  /// Return the sync_scope policy value for the current one-way
  /// operation.  If the operation is not a one-way, a
  /// CORBA::BAD_INV_ORDER exception is thrown.
  Messaging::SyncScope sync_scope (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Return the reply status for the current request.  Statuses can
  /// be PortableInterceptor::SUCCESSFUL, SYSTEM_EXCEPTION,
  /// USER_EXCEPTION, LOCATION_FORWARD, TRANSPORT_RETRY, UNKNOWN
  PortableInterceptor::ReplyStatus reply_status (
      ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// If the reply status is PortableInterceptor::LOCATION_FORWARD
  /// return the object reference to which the request was forwarded.
  CORBA::Object_ptr forward_reference (
      ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException)) ;

  CORBA::Any * get_slot (
      PortableInterceptor::SlotId id
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::InvalidSlot));

  /// Return the IOP::ServiceContext with the given IOP::ServiceId
  /// from the request service context list.
  IOP::ServiceContext * get_request_service_context (
      IOP::ServiceId id
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Return the IOP::ServiceContext with the given IOP::ServiceId
  /// from the reply service context list.
  IOP::ServiceContext * get_reply_service_context (
      IOP::ServiceId id
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Return the (initial, non-forwarded, or permanently forwarded)
  /// object reference of the target.
  CORBA::Object_ptr target (
      ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Return the object reference for the current target.  The target
  /// may change in the even of a location forward.
  CORBA::Object_ptr effective_target (
      ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  IOP::TaggedProfile * effective_profile (
      ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Return an Any containing the received exception, if any.
  /// Otherwise, throw a CORBA::BAD_INV_ORDER exception.
  /**
   * @note There is no trivial way to extract the exception from an Any.
   */
  CORBA::Any * received_exception (
      ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Return the repository ID for the received exception.
  char * received_exception_id (
      ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Return the first IOP::TaggedComponent that matches the given
  /// IOP::ComponentId in the object reference for the current
  /// target.
  IOP::TaggedComponent * get_effective_component (
      IOP::ComponentId id
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Return all IOP::TaggedComponent(s) that match the given
  /// IOP::ComponentId in the object reference for the current
  /// target.
  IOP::TaggedComponentSeq * get_effective_components (
      IOP::ComponentId id
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Return the policy of the given type in effect for the current
  /// request.
  CORBA::Policy_ptr get_request_policy (
      CORBA::PolicyType type
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Add the IOP::ServiceContext to the request (outgoing)
  /// IOP::ServiceContextList.
  void add_request_service_context (
      const IOP::ServiceContext & service_context,
      CORBA::Boolean replace
      ACE_ENV_ARG_DECL)
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
  // @@@@@ Need to go
  void response_expected (CORBA::Boolean flag);

  /// Set the status of the received reply.
  void reply_status (TAO::Invocation_Status s);

  // @@@@ NEEd to go
  void reply_status (int s);

  /// Extract the forward object reference from the
  /// PortableInterceptor::ForwardRequest exception, and set the reply
  /// status flag accordingly.
  void forward_reference (PortableInterceptor::ForwardRequest &exc);
  //@}

protected:

  /// Setup thread scope and request scope
  /// PortableInterceptor::Current objects.
  void setup_picurrent (void);

  /// Helper method to get the request and response service contexts.
  IOP::ServiceContext *get_service_context_i (
      TAO_Service_Context &service_context_list,
      IOP::ServiceId id
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  TAO_Stub *stub (void) const;

protected:

  /// Pointer to the invocation object.
  TAO::Invocation_Base *invocation_;

  /// Reference to the abstract interface target.
  // CORBA::AbstractBase_ptr abstract_target_;

  /// Pointer to the caught exception.
  CORBA::Exception *caught_exception_;

  /// Reply status for the current request.
  PortableInterceptor::ReplyStatus reply_status_;

  /// The "Request Scope Current" (RSC) object, as required by
  /// Portable Interceptors.
  TAO::PICurrent_Impl rs_pi_current_;

  /// Callback object to be executed when shallow copied slot table
  /// must be deep copied.
  /**
   * This callback object deep copies the slot table from the TSC to
   * the RSC.
   * @par
   * As an optimization, the TSC's slot table is initially only
   * shallowed copied to the RSC.  If the TSC's slot table will be
   * modified, e.g. via PICurrent::set_slot(), it's slot table must be
   * deep copied to the RSC before actually modifying that slot
   * table.  This is necessary since the RSC is read-only on the
   * client side, meaning that changes in the TSC that occur after
   * instantiation of the RSC must not be reflected in the RSC.
   */
  TAO::PICurrent_Copy_Callback copy_callback_;

};

# if defined (__ACE_INLINE__)
#  include "ClientRequestInfo_i.inl"
# endif  /* __ACE_INLINE__ */

#endif /* TAO_HAS_INTERCEPTORS == 1 */

#include /**/ "ace/post.h"

#endif /* TAO_CLIENT_REQUEST_INFO_I_H */
