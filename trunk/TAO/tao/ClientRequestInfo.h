// -*- C++ -*-
//
// $Id$

//========================================================================
//
// = LIBRARY
//     TAO
//
// = FILENAME
//     ClientRequestInfo.h
//
// = DESCRIPTION
//     This is the implementation of the
//     PortableInterceptor::ClientRequestInfo interface.
//
// = AUTHOR
//     Kirthika Parameswaran <kirthika@cs.wustl.edu>
//     Ossama Othman <ossama@uci.edu>
//
//=========================================================================


#ifndef TAO_CLIENT_REQUEST_INFO_H
#define TAO_CLIENT_REQUEST_INFO_H

#include "ace/pre.h"

#include "tao/corbafwd.h"

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

#include "tao/PortableInterceptorC.h"
#include "tao/LocalObject.h"
#include "tao/StringSeqC.h"


class TAO_Export TAO_ClientRequestInfo
  : public virtual PortableInterceptor::ClientRequestInfo,
    public virtual CORBA::LocalObject
{
public:
  TAO_ClientRequestInfo (const char * operation,
                         IOP::ServiceContextList &service_context_list,
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

# if (TAO_HAS_CORBA_MESSAGING == 1)
  virtual CORBA::Short sync_scope (
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

  virtual CORBA::Object_ptr target (
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::Object_ptr effective_target (
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual IOP::TaggedProfile * effective_profile (
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::Any * received_exception (
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));
  // This method causes problem since there is no trivial way to
  // extract the exception from the Any.

  virtual CORBA::Exception * _received_exception (
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Note: This is TAO specific and was done to combat the previous
  // problem to some extent.

  virtual char * received_exception_id (
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual IOP::TaggedComponent * get_effective_component (
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
  void exception (CORBA::Exception *exception);
  // Change the exception status.

  void request_id (CORBA::ULong request_id);
  // Update the request id.

protected:
  CORBA::ULong request_id_;
  const char * operation_;
  Dynamic::ParameterList parameter_list_;
  Dynamic::ExceptionList exception_list_;
  Dynamic::ContextList context_list_;
  Dynamic::RequestContext request_context_;
  CORBA::Object_var forward_reference_;

  // Needed to ensure no copy anywhere.
  IOP::ServiceContextList &service_context_list_;

  CORBA::Any result_val_;
  CORBA::Object_var target_;
  CORBA::Object_var effective_target_;
  CORBA::Any any_exception_;
  CORBA::Exception *caught_exception_;
};

#endif /* TAO_HAS_INTERCEPTORS == 1 */

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"

#endif /* TAO_CLIENT_REQUEST_INFO_H */
