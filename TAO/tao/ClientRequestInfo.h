// -*- C++ -*-

//=============================================================================
/**
 * @file     ClientRequestInfo.h
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
#include "Invocation.h"

class TAO_GIOP_Invocation;

class TAO_Export TAO_ClientRequestInfo
  : public virtual PortableInterceptor::ClientRequestInfo,
    public virtual CORBA::LocalObject
{
public:
  TAO_ClientRequestInfo (TAO_GIOP_Invocation *invocation,
                         CORBA::Object_ptr target);

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
  virtual CORBA::Short sync_scope (
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));
#endif  /* TAO_HAS_CORBA_MESSAGING == 1 */

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

  virtual CORBA::Object_ptr target (
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::Object_ptr effective_target (
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual IOP::TaggedProfile * effective_profile (
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// This method causes problem since there is no trivial way to
  /// extract the exception from the Any.
  virtual CORBA::Any * received_exception (
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Note: This is TAO specific and was done to combat the previous
  /// problem to some extent.
  virtual CORBA::Exception * _received_exception (
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual char * received_exception_id (
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual IOP::TaggedComponent * get_effective_component (
      IOP::ComponentId id,
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual IOP::TaggedComponentSeq * get_effective_components (
      IOP::ComponentId id,
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::Policy_ptr get_request_policy (
      CORBA::PolicyType type,
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void add_request_service_context (
      const IOP::ServiceContext & service_context,
      CORBA::Boolean replace,
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
     ACE_THROW_SPEC ((CORBA::SystemException));

protected:

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

protected:

  TAO_GIOP_Invocation *invocation_;

  CORBA::Object_ptr target_;
  CORBA::Exception *caught_exception_;

  CORBA::Boolean response_expected_;
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
