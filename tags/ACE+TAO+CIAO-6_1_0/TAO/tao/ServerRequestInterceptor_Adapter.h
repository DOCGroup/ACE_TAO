// -*- C++ -*-

//=============================================================================
/**
 *  @file    ServerRequestInterceptor_Adapter.h
 *
 *  $Id$
 *
 *   This file an adapter class to simplify the support of
 *   interceptors.
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_SERVER_REQUEST_INTERCEPTOR_ADAPTER_H
#define TAO_SERVER_REQUEST_INTERCEPTOR_ADAPTER_H

#include /**/ "ace/pre.h"

#include /**/ "tao/TAO_Export.h"
#include "tao/RequestInterceptor_Adapter.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/orbconf.h"
#include "tao/Basic_Types.h"

#if TAO_HAS_EXTENDED_FT_INTERCEPTORS == 1
# include "tao/OctetSeqC.h"
#endif /*TAO_HAS_EXTENDED_FT_INTERCEPTORS*/

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace PortableInterceptor
{
  class ServerRequestInterceptor;
  typedef ServerRequestInterceptor *ServerRequestInterceptor_ptr;

  typedef CORBA::Short ReplyStatus;
}

namespace CORBA
{
  class PolicyList;
}

class TAO_ServerRequest;

namespace TAO
{
  class ServerRequestInfo;
  class Argument;
  class PICurrent_Impl;
  class Upcall_Command;

  namespace Portable_Server
  {
    class Servant_Upcall;
  }

  /**
   * @class ServerRequestInterceptor_Adapter
   */
  class TAO_Export ServerRequestInterceptor_Adapter
    : public RequestInterceptor_Adapter
  {
  public:
    virtual ~ServerRequestInterceptor_Adapter (void);

    /**
     * @name PortableInterceptor Server Side Interception Points
     *
     * Each of these methods corresponds to a client side interception
     * point.
     */
    //@{
#if TAO_HAS_EXTENDED_FT_INTERCEPTORS == 1
    /// This method implements the "starting" server side interception
    /// point. It will be used as the first interception point and it is
    /// proprietary to TAO.
    virtual void tao_ft_interception_point (
        TAO_ServerRequest &server_request,
        TAO::Argument * const args[],
        size_t nargs,
        TAO::Portable_Server::Servant_Upcall *servant_upcall,
        CORBA::TypeCode_ptr const * exceptions,
        CORBA::ULong nexceptions,
        CORBA::OctetSeq_out oc) = 0;
#endif /*TAO_HAS_EXTENDED_FT_INTERCEPTORS*/

    /// This method implements the "intermediate" server side
    /// interception point if the above #ifdef is set to 1 and a
    /// starting intercetion point if it is not set to 1.
    ///
    /// @note This method should have been the "starting" interception
    ///       point according to the interceptor spec. This will be
    ///       fixed once Bug 1369 is completely done.
    virtual void receive_request_service_contexts (
        TAO_ServerRequest &server_request,
        TAO::Argument * const args[],
        size_t nargs,
        TAO::Portable_Server::Servant_Upcall *servant_upcall,
        CORBA::TypeCode_ptr const * exceptions,
        CORBA::ULong nexceptions) = 0;

    /// This method an "intermediate" server side interception point.
    virtual void receive_request (
        TAO_ServerRequest &server_request,
        TAO::Argument * const args[],
        size_t nargs,
        TAO::Portable_Server::Servant_Upcall *servant_upcall,
        CORBA::TypeCode_ptr const * exceptions,
        CORBA::ULong nexceptions) = 0;

    /// This method implements one of the "ending" server side
    /// interception points.
    virtual void send_reply (
        TAO_ServerRequest &server_request,
        TAO::Argument * const args[],
        size_t nargs,
        TAO::Portable_Server::Servant_Upcall *servant_upcall,
        CORBA::TypeCode_ptr const * exceptions,
        CORBA::ULong nexceptions) = 0;

    /// This method implements one of the "ending" server side
    /// interception points.
    virtual void send_exception (
        TAO_ServerRequest &server_request,
        TAO::Argument * const args[],
        size_t nargs,
        TAO::Portable_Server::Servant_Upcall *servant_upcall,
        CORBA::TypeCode_ptr const * exceptions,
        CORBA::ULong nexceptions) = 0;

    /// This method implements one of the "ending" server side
    /// interception points.
    virtual void send_other (
        TAO_ServerRequest &server_request,
        TAO::Argument * const args[],
        size_t nargs,
        TAO::Portable_Server::Servant_Upcall *servant_upcall,
        CORBA::TypeCode_ptr const * exceptions,
        CORBA::ULong nexceptions) = 0;
    //@}

    /// Register an interceptor.
    virtual void add_interceptor (
      PortableInterceptor::ServerRequestInterceptor_ptr interceptor) = 0;

    /// Register an interceptor with policies.
    virtual void add_interceptor (
      PortableInterceptor::ServerRequestInterceptor_ptr interceptor,
      const CORBA::PolicyList& policies) = 0;

    virtual TAO::PICurrent_Impl *allocate_pi_current (void) = 0;

    virtual void deallocate_pi_current (TAO::PICurrent_Impl *picurrent) = 0;

    virtual void execute_command (
        TAO_ServerRequest &server_request,
        TAO::Upcall_Command &command) = 0;
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_SERVER_REQUEST_INTERCEPTOR_ADAPTER_H */
