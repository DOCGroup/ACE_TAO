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
 * @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================

#ifndef TAO_CLIENT_REQUEST_INFO_H
#define TAO_CLIENT_REQUEST_INFO_H

#include "ace/pre.h"

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if (TAO_HAS_INTERCEPTORS == 1)

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

#include "tao/PortableInterceptorC.h"
#include "tao/LocalObject.h"

class TAO_ClientRequestInfo_i;

/**
 * @class TAO_ClientRequestInfo
 *
 * @brief Implementation of the PortableInterceptor::ClientRequestInfo
 *        interface.
 *
 * This class forwards all method calls to the underlying
 * ClientRequestInfo implementation.
 * @par
 * An instance of this class is placed in TSS, where as the underlying
 * implementation is instantiated on the stack during each CORBA
 * request.  During each request invocation, a pointer to the stack
 * instantiated implementation is placed in the instance of this
 * class.
 * @par
 * This may seem unnecessary.  However, it is necessary to avoid
 * instantiating an object that inherits from CORBA::Object in the
 * critical path.  Such an instantiation would cause a lock to be
 * initialized (not acquired) in the critical path, which can degrade
 * performance significantly.
 */
class TAO_ClientRequestInfo
  : public virtual PortableInterceptor::ClientRequestInfo,
    public virtual TAO_Local_RefCounted_Object
{
public:

  TAO_ClientRequestInfo (void);

  /// Return an ID unique to the current request.  This request ID may
  /// or may not be the same as the GIOP request ID.
  virtual CORBA::ULong request_id (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Return the operation name for the current request.
  virtual char * operation (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Return the list of arguments passed to the current operation.
  virtual Dynamic::ParameterList * arguments (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Return the list of exceptions the current operation is capable
  /// of throwing.
  virtual Dynamic::ExceptionList * exceptions (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual Dynamic::ContextList * contexts (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual Dynamic::RequestContext * operation_context (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Return the result of the current request.  If there is no return
  /// value then an Any with tk_void TypeCode is returned.  This is
  /// method is not valid for oneway operations.
  virtual CORBA::Any * result (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Returns true for a two-way operation, and false otherwise.
  virtual CORBA::Boolean response_expected (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException)) ;

  /// Return the sync_scope policy value for the current one-way
  /// operation.  If the operation is not a one-way, a
  /// CORBA::BAD_INV_ORDER exception is thrown.
  virtual Messaging::SyncScope sync_scope (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Return the reply status for the current request.  Statuses can
  /// be PortableInterceptor::SUCCESSFUL, SYSTEM_EXCEPTION,
  /// USER_EXCEPTION, LOCATION_FORWARD, LOCATION_FORWARD_PERMANENT,
  /// TRANSPORT_RETRY.
  virtual PortableInterceptor::ReplyStatus reply_status (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// If the reply status is PortableInterceptor::LOCATION_FORWARD or
  /// PortableInterceptor::LOCATION_FORWARD_PERMANENT, return the
  /// object reference to which the request was forwarded.
  virtual CORBA::Object_ptr forward_reference (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException)) ;

  virtual CORBA::Any * get_slot (
      PortableInterceptor::SlotId id
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::InvalidSlot));

  /// Return the IOP::ServiceContext with the given IOP::ServiceId
  /// from the request service context list.
  virtual IOP::ServiceContext * get_request_service_context (
      IOP::ServiceId id
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Return the IOP::ServiceContext with the given IOP::ServiceId
  /// from the reply service context list.
  virtual IOP::ServiceContext * get_reply_service_context (
      IOP::ServiceId id
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Return the (initial, non-forwarded, or permanently forwarded)
  /// object reference of the target.
  virtual CORBA::Object_ptr target (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Return the object reference for the current target.  The target
  /// may change in the even of a location forward.
  virtual CORBA::Object_ptr effective_target (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual IOP::TaggedProfile * effective_profile (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Return an Any containing the received exception, if any.
  /// Otherwise, throw a CORBA::BAD_INV_ORDER exception.
  /**
   * @note There is no trivial way to extract the exception from an Any.
   */
  virtual CORBA::Any * received_exception (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Return the repository ID for the received exception.
  virtual char * received_exception_id (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Return the first IOP::TaggedComponent that matches the given
  /// IOP::ComponentId in the object reference for the current
  /// target.
  virtual IOP::TaggedComponent * get_effective_component (
      IOP::ComponentId id
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Return all IOP::TaggedComponent(s) that match the given
  /// IOP::ComponentId in the object reference for the current
  /// target.
  virtual IOP::TaggedComponentSeq * get_effective_components (
      IOP::ComponentId id
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Return the policy of the given type in effect for the current
  /// request.
  virtual CORBA::Policy_ptr get_request_policy (
      CORBA::PolicyType type
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Add the IOP::ServiceContext to the request (outgoing)
  /// IOP::ServiceContextList.
  virtual void add_request_service_context (
      const IOP::ServiceContext & service_context,
      CORBA::Boolean replace
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
     ACE_THROW_SPEC ((CORBA::SystemException));

  /// Set the ClientRequestInfo implementation which this class
  /// forwards all method call to.
  void info (TAO_ClientRequestInfo_i *info);

  /// Get the ClientRequestInfo implementation which this class
  /// forwards all method call to.
  TAO_ClientRequestInfo_i * info (void) const;

private:

  /// Check if this ClientRequestInfo object is called within the
  /// context of a request.
  void check_validity (ACE_ENV_SINGLE_ARG_DECL);

private:

  /// Pointer to the object that actually implements the
  /// ClientRequestInfo functionality.
  TAO_ClientRequestInfo_i * info_;

};


// -------------------------------------------------------------------


/**
 * @class TAO_ClientRequestInfo_Guard
 *
 * @brief Guard for exception safe TAO_ClientRequestInfo_i pointer
 *        swapping.
 *
 * This class is used to ensure the swapping of
 * TAO_ClientRequestInfo_i pointers in a TAO_ClientRequestInfo object
 * is performed in an exception-safe manner when interception points
 * are being invoked.
 */
class TAO_ClientRequestInfo_Guard
{
public:

  /// Constructor.
  TAO_ClientRequestInfo_Guard (TAO_ClientRequestInfo *info,
                               TAO_ClientRequestInfo_i *ri);

  /// Destructor.
  ~TAO_ClientRequestInfo_Guard (void);

private:

  /// Pointer to the TAO_ClientRequestInfo object upon which pointer
  /// swaps will occur.
  TAO_ClientRequestInfo * info_;

  /// Pointer to the TAO_ClientRequestInfo_i object that was
  /// previously stored in the TAO_ClientRequestInfo object.
  TAO_ClientRequestInfo_i * previous_info_;

};


# if defined (__ACE_INLINE__)
#  include "ClientRequestInfo.inl"
# endif  /* __ACE_INLINE__ */

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_HAS_INTERCEPTORS == 1 */

#include "ace/post.h"

#endif /* TAO_CLIENT_REQUEST_INFO_H */
