// -*- C++ -*-
//
// $Id$

//========================================================================
//
// = LIBRARY
//     TAO
//
// = FILENAME
//     ServerRequestInfo.h
//
// = DESCRIPTION
//     This is the implementation of the
//     PortableInterceptor::ServerRequestInfo interface.
//
// = AUTHOR
//     Kirthika Parameswaran <kirthika@cs.wustl.edu>
//     Ossama Othman <ossama@uci.edu>
//
//=========================================================================


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
#include "portableserver_export.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_PortableServer_Export TAO_ServerRequestInfo
  : public virtual PortableInterceptor::ServerRequestInfo,
    public virtual CORBA::LocalObject
{
 public:
  TAO_ServerRequestInfo (const char *operation,
                         IOP::ServiceContextList &service_context_list);

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

  // Probably the following methods should be delegated to the PICurrent
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

  virtual CORBA::Any * sending_exception (
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));
  // This method causes problem since there is no trivial way to
  // extract the exception from the Any.

  virtual CORBA::Exception * _sending_exception (
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Note: This is TAO specific and was done to combat the previous
  // problem to some extent.

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

 protected:
  void exception (CORBA::Exception *exception);
  // Change the exception status.

  void request_id (CORBA::ULong request_id);
  // Update the request id.

  CORBA::ULong request_id_;
  const char * operation_;
  Dynamic::ParameterList parameter_list_;
  Dynamic::ExceptionList  exception_list_;
  Dynamic::ContextList context_list_;
  Dynamic::RequestContext request_context_;
  CORBA::Object_var forward_reference_;

  // Needed to ensure no copy anywhere.
  IOP::ServiceContextList &service_context_list_;

  CORBA::Any result_val_;
  CORBA::OctetSeq_var object_id_;
  CORBA::OctetSeq_var adapter_id_;
  CORBA::Exception *caught_exception_;

  PortableInterceptor::ReplyStatus reply_status_;
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */


#endif /* TAO_HAS_INTERCEPTORS == 1 */

#include "ace/post.h"

#endif /* TAO_SERVER_REQUEST_INFO_H */
