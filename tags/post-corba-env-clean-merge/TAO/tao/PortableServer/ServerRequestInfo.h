// -*- C++ -*-

//=============================================================================
/**
 * @file ServerRequestInfo.h
 *
 * $Id$
 *
 * This is the implementation of the
 * PortableInterceptor::ServerRequestInfo interface.
 *
 * @author Kirthika Parameswaran <kirthika@cs.wustl.edu>
 * @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================

#ifndef TAO_SERVER_REQUEST_INFO_H
#define TAO_SERVER_REQUEST_INFO_H

#include "ace/pre.h"

#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if (TAO_HAS_INTERCEPTORS == 1)

#include "tao/PortableInterceptorC.h"
#include "tao/LocalObject.h"
#include "tao/StringSeqC.h"
#include "tao/OctetSeqC.h"
#include "tao/Service_Context.h"
#include "tao/TAO_Server_Request.h"

#include "Object_Adapter.h"

#include "portableserver_export.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */


/**
 * @class TAO_ServerRequestInfo
 *
 * @brief Implementation of the PortableInterceptor::ServerRequestInfo
 *        interface.
 */
class TAO_PortableServer_Export TAO_ServerRequestInfo
  : public virtual PortableInterceptor::ServerRequestInfo,
    public virtual CORBA::LocalObject
{
public:

  /// Constructor.
  TAO_ServerRequestInfo (TAO_ServerRequest &server_request,
                         TAO_Object_Adapter::Servant_Upcall *servant_upcall);

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

  /// Returns true for a two-way operation, and false otherwis.e
  virtual CORBA::Boolean response_expected (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException)) ;

#if TAO_HAS_CORBA_MESSAGING == 1
  /// Return the sync_scope policy value for the current one-way
  /// operation.  If the operation is not a one-way, a
  /// CORBA::BAD_INV_ORDER exception is thrown.
  virtual Messaging::SyncScope sync_scope (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));
#endif  /* TAO_HAS_CORBA_MESSAGING */

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

  /// Retrieve data from the "request scope" PICurrent object.
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

  /// Return an Any containing the exception being sent, if any.
  /// Otherwise, throw a CORBA::BAD_INV_ORDER exception.
  /**
   * @note There is no trivial way to extract the exception from an Any.
   */
  virtual CORBA::Any * sending_exception (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /**
   * @note This is TAO specific and was done to combat the previous
   *       problem to some extent.  Avoid this method whenever
   *       possible.
   */
  virtual CORBA::Exception * _sending_exception (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Return the ObjectId for the target object.
  virtual CORBA::OctetSeq * object_id (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Return the AdapterId for the POA handling the current request.
  virtual CORBA::OctetSeq * adapter_id (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Return the most derived interface of the target object.
  virtual char * target_most_derived_interface (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Return the policy of the given type in effect for the current
  /// request.
  virtual CORBA::Policy_ptr get_server_policy (
      CORBA::PolicyType type
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Insert data into the "request scope" PICurrent object.
  virtual void set_slot (
      PortableInterceptor::SlotId id,
      const CORBA::Any & data
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::InvalidSlot));

  /// Returns true if the target's type corresponds to the given
  /// RepositoryId.
  virtual CORBA::Boolean target_is_a (
      const char * id
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Add the IOP::ServiceContext to the reply (outgoing)
  /// IOP::ServiceContextList.
  virtual void add_reply_service_context (
      const IOP::ServiceContext & service_context,
      CORBA::Boolean replace
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

public:

  /**
   * @name Helper methods specific to TAO.
   */
  //@{

  /// Change the exception status.
  void exception (CORBA::Exception *exception);

  /// Set the status of the received reply.
  void reply_status (PortableInterceptor::ReplyStatus s);

  /// Extract the forward object reference from the
  /// PortableInterceptor::ForwardRequest exception, and set the reply
  /// status flag accordingly.
  void forward_reference (PortableInterceptor::ForwardRequest &exc);

  /// Set the forward reference associated with the current
  /// LOCATION_FORWARD reply.  This method is only invoked when a
  /// PortableServer::ForwardRequest exception is thrown by a servant
  /// manager.
  void forward_reference (CORBA::Object_ptr obj);

  //@}

  /// Return a reference to the underlying TAO_ServerRequest object.
  TAO_ServerRequest &server_request (void);

protected:

  /// Helper method to get the request and response service contexts.
  IOP::ServiceContext *get_service_context_i (
    TAO_Service_Context &service_context_list,
    IOP::ServiceId id
    TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException));

protected:

  /// Underlying request object that contains much of the information
  /// encapsulated by this TAO_ServerRequestInfo class.
  TAO_ServerRequest &server_request_;

  /// Pointer to the Servant_Upcall object that contains the object
  /// ID, among other things.
  TAO_Object_Adapter::Servant_Upcall *servant_upcall_;

  /// Pointer to the caught exception.
  CORBA::Exception *caught_exception_;

  /// Reply status for the current request.
  PortableInterceptor::ReplyStatus reply_status_;

};

# if defined (__ACE_INLINE__)
#  include "ServerRequestInfo.inl"
# endif  /* __ACE_INLINE__ */

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */


#endif /* TAO_HAS_INTERCEPTORS == 1 */

#include "ace/post.h"

#endif /* TAO_SERVER_REQUEST_INFO_H */
