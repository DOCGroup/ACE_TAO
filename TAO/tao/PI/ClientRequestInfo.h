// -*- C++ -*-

//=============================================================================
/**
 * @file ClientRequestInfo.h
 *
 * This is the implementation of the
 * PortableInterceptor::ClientRequestInfo interface.
 *
 * @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================

#ifndef TAO_CLIENT_REQUEST_INFO_H
#define TAO_CLIENT_REQUEST_INFO_H

#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if (TAO_HAS_INTERCEPTORS == 1)

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

#include "tao/AnyTypeCode/AnyTypeCode_methods.h"
#include "tao/PI/PI_includeC.h"
#include "tao/PI/PICurrent_Impl.h"
#include "tao/CORBA_methods.h"
#include "tao/ORB_Constants.h"
#include "tao/LocalObject.h"
#include "tao/Invocation_Utils.h"
#include "tao/TimeBaseC.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Service_Context;

namespace TAO
{
  class Invocation_Base;
}

namespace Dynamic
{
  class ParameterList;
  class ExceptionList;
  typedef CORBA::StringSeq RequestContext;
  typedef CORBA::StringSeq ContextList;
}

namespace Messaging
{
  typedef CORBA::Short SyncScope;
}

/**
 * @class TAO_ClientRequestInfo
 *
 * @brief Implementation of the PortableInterceptor::ClientRequestInfo
 *        interface.
 */
class TAO_PI_Export TAO_ClientRequestInfo
  : public virtual PortableInterceptor::ClientRequestInfo,
    public virtual ::CORBA::LocalObject
{
public:
  TAO_ClientRequestInfo (TAO::Invocation_Base *invocation);

  /// Return an ID unique to the current request.  This request ID may
  /// or may not be the same as the GIOP request ID.
  CORBA::ULong request_id () override;

  /// Return the operation name for the current request.
  char * operation () override;

  /// Return the list of arguments passed to the current operation.
  Dynamic::ParameterList * arguments () override;

  /// Return the list of exceptions the current operation is capable
  /// of throwing.
  Dynamic::ExceptionList * exceptions () override;

  Dynamic::ContextList * contexts () override;

  Dynamic::RequestContext * operation_context () override;

  /// Return the result of the current request.  If there is no return
  /// value then an Any with tk_void TypeCode is returned.  This is
  /// method is not valid for oneway operations.
  CORBA::Any * result () override;

  /// Returns true for a two-way operation, and false otherwise.
  CORBA::Boolean response_expected () override;

  /// Return the sync_scope policy value for the current one-way
  /// operation.  If the operation is not a one-way, a
  /// CORBA::BAD_INV_ORDER exception is thrown.
  Messaging::SyncScope sync_scope () override;

  /// Return the reply status for the current request.
  /**
   * Statuses can be PortableInterceptor::SUCCESSFUL,
   * SYSTEM_EXCEPTION, USER_EXCEPTION, LOCATION_FORWARD,
   * TRANSPORT_RETRY, UNKNOWN.
   */
  PortableInterceptor::ReplyStatus reply_status () override;

  /// If the reply status is PortableInterceptor::LOCATION_FORWARD or
  /// return the object reference to which the request was forwarded.
  CORBA::Object_ptr forward_reference () override;

  CORBA::Any * get_slot (PortableInterceptor::SlotId id) override;

  /// Return the IOP::ServiceContext with the given IOP::ServiceId
  /// from the request service context list.
  IOP::ServiceContext * get_request_service_context (
      IOP::ServiceId id) override;

  /// Return the IOP::ServiceContext with the given IOP::ServiceId
  /// from the reply service context list.
  IOP::ServiceContext * get_reply_service_context (IOP::ServiceId id) override;

  /// Return the (initial, non-forwarded, or permanently forwarded)
  /// object reference of the target.
  CORBA::Object_ptr target () override;

  /// Return the object reference for the current target.  The target
  /// may change in the even of a location forward.
  CORBA::Object_ptr effective_target () override;

  IOP::TaggedProfile * effective_profile () override;

  /// Return an Any containing the received exception, if any.
  /// Otherwise, throw a CORBA::BAD_INV_ORDER exception.
  /**
   * @note There is no trivial way to extract the exception from an Any.
   */
  CORBA::Any * received_exception () override;

  /// Return the repository ID for the received exception.
  char * received_exception_id () override;

  /// Return the first IOP::TaggedComponent that matches the given
  /// IOP::ComponentId in the object reference for the current
  /// target.
  IOP::TaggedComponent * get_effective_component (IOP::ComponentId id) override;

  /// Return all IOP::TaggedComponent(s) that match the given
  /// IOP::ComponentId in the object reference for the current
  /// target.
  IOP::TaggedComponentSeq * get_effective_components (IOP::ComponentId id) override;

  /// Return the policy of the given type in effect for the current
  /// request.
  CORBA::Policy_ptr get_request_policy (CORBA::PolicyType type) override;

  /// Add the IOP::ServiceContext to the request (outgoing)
  /// IOP::ServiceContextList.
  void add_request_service_context (
      const IOP::ServiceContext & service_context,
      CORBA::Boolean replace) override;

  /*
  * Proprietary accessor methods for the FT retention ID and
  * request expiration time.
  */

  /// Set the absolute FT expiration time for this request.
  void tao_ft_expiration_time (TimeBase::TimeT time);

  /// Get the absolute FT expiration time for this request
  TimeBase::TimeT tao_ft_expiration_time () const;

  /// Set the FT request retention ID for this request.
  void tao_ft_retention_id (CORBA::Long request_id);

  /// Get the FT request retention ID for this request.
  CORBA::Long tao_ft_retention_id () const;

  /*
  * End proprietary FT methods.
  */

private:
  bool parameter_list (Dynamic::ParameterList &param_list);

  bool exception_list (Dynamic::ExceptionList &exception_list);

  bool result (CORBA::Any *any);

  /// Check if this ClientRequestInfo object is called within the
  /// context of a request.
  void check_validity ();

  /// Setup thread scope and request scope
  /// PortableInterceptor::Current objects.
  void setup_picurrent ();

  /// Helper method to get the request and response service contexts.
  IOP::ServiceContext *get_service_context_i (
      TAO_Service_Context &service_context_list,
      IOP::ServiceId id);

private:
  /// Pointer to the invocation object.
  TAO::Invocation_Base *invocation_;

  /// The "Request Scope Current" (RSC) object, as required by
  /// Portable Interceptors.
  TAO::PICurrent_Impl rs_pi_current_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_HAS_INTERCEPTORS == 1 */

#include /**/ "ace/post.h"

#endif /* TAO_CLIENT_REQUEST_INFO_H */
