// -*- C++ -*-

//=============================================================================
/**
 *  @file     ServerRequestInfo.h
 *
 *  $Id$
 *
 *  This is the implementation of the
 *  PortableInterceptor::ServerRequestInfo interface.
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

//#include "PortableServerC.h"

#include "portableserver_export.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_ServerRequest;


class TAO_PortableServer_Export TAO_ServerRequestInfo
  : public virtual PortableInterceptor::ServerRequestInfo,
    public virtual CORBA::LocalObject
{
public:
  TAO_ServerRequestInfo (TAO_ServerRequest &server_request);

  virtual CORBA::ULong request_id (
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual char * operation (
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual Dynamic::ParameterList * arguments (
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual Dynamic::ExceptionList * exceptions (
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual Dynamic::ContextList * contexts (
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual Dynamic::RequestContext * operation_context (
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::Any * result (
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::Boolean response_expected (
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException)) ;

#if TAO_HAS_CORBA_MESSAGING == 1
  virtual Messaging::SyncScope sync_scope (
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));
#endif  /* TAO_HAS_CORBA_MESSAGING */

  virtual PortableInterceptor::ReplyStatus reply_status (
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::Object_ptr forward_reference (
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException)) ;

  virtual CORBA::Any * get_slot (
      PortableInterceptor::SlotId id,
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::InvalidSlot));

  virtual IOP::ServiceContext * get_request_service_context (
      IOP::ServiceId id,
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual IOP::ServiceContext * get_reply_service_context (
      IOP::ServiceId id,
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// This method causes problem since there is no trivial way to
  /// extract the exception from the Any.
  virtual CORBA::Any * sending_exception (
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Note: This is TAO specific and was done to combat the previous
  /// problem to some extent.
  virtual CORBA::Exception * _sending_exception (
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::OctetSeq * object_id (
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::OctetSeq * adapter_id (
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual char * target_most_derived_interface (
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::Policy_ptr get_server_policy (
      CORBA::PolicyType type,
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void set_slot (
      PortableInterceptor::SlotId id,
      const CORBA::Any & data,
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::InvalidSlot));

  virtual CORBA::Boolean target_is_a (
      const char * id,
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void add_reply_service_context (
      const IOP::ServiceContext & service_context,
      CORBA::Boolean replace,
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

public:

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

protected:
  /// Helper method to get the request and response service contexts.
  IOP::ServiceContext *get_service_context_i (
    TAO_Service_Context &service_context_list,
    IOP::ServiceId id,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException));

protected:

  /// Underlying request object that contains much of the information
  /// encapsulated by this ServerRequestInfo class.
  TAO_ServerRequest &server_request_;

  CORBA::Object_var forward_reference_;

  //  PortableServer::Current_var poa_current_;
  //  CORBA::OctetSeq_var adapter_id_;

  CORBA::Exception *caught_exception_;

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
