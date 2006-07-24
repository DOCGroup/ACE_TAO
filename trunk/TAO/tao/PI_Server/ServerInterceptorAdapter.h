// -*- C++ -*-

//=============================================================================
/**
 *  @file    ServerInterceptorAdapter.h
 *
 *  $Id$
 *
 *   This file contains a helper class to simplify the support of
 *   interceptors in TAO_IDL generated skeletons.
 *
 *  @author  Nanbor Wang <nanbor@cs.wustl.edu>
 *  @author  Ossama Othman <ossama@uci.edu>
 *  @author  Kirthika Parameswaran  <kirthika@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_SERVER_INTERCEPTOR_ADAPTER_H
#define TAO_SERVER_INTERCEPTOR_ADAPTER_H

#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


#if TAO_HAS_INTERCEPTORS == 1

#include "tao/PI_Server/ServerRequestInterceptorC.h"

#include "tao/PI/Interceptor_List_T.h"
#include "tao/ServerRequestInterceptor_Adapter.h"
#include "tao/Basic_Types.h"
#include "tao/PI_Server/ServerRequestDetails.h"


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  typedef Interceptor_List< ::PortableInterceptor::ServerRequestInterceptor,
                            ServerRequestDetails>
    ServerRequestInterceptor_List;
}

namespace CORBA
{
  class PolicyList;
}

class TAO_ServerRequest;

namespace TAO
{
  class ServerRequestInfo;

  /**
   * @class ServerRequestInterceptor_Adapter_Impl
   *
   * @brief ServerRequestInterceptor_Adapter_Impl
   *
   * A convenient helper class to invoke registered server request
   * interceptor(s).
   */
  class ServerRequestInterceptor_Adapter_Impl
    : public ServerRequestInterceptor_Adapter
  {
  public:

    /// Constructor.
    ServerRequestInterceptor_Adapter_Impl (void);

    /**
     * @name PortableInterceptor Server Side Interception Points
     *
     * Each of these methods corresponds to a server side interception
     * point.
     */
    //@{
#if TAO_HAS_EXTENDED_FT_INTERCEPTORS == 1
    /// This method implements the "starting" server side interception
    /// point. It will be used as the first interception point and it is
    /// proprietary to TAO.
    /// @@ Will go away once Bug 1369 is fixed
    void tao_ft_interception_point (
        TAO_ServerRequest &server_request,
        TAO::Argument * const args[],
        size_t nargs,
        void * servant_upcall,
        CORBA::TypeCode_ptr const * exceptions,
        CORBA::ULong nexceptions,
        CORBA::OctetSeq_out oc
        ACE_ENV_ARG_DECL);
#endif /*TAO_HAS_EXTENDED_FT_INTERCEPTORS*/

    /// This method implements the "intermediate" server side
    /// interception point if the above #ifdef is set to 1 and a
    /// starting intercetion point if it is not set to 1.
    ///
    /// @note This method should have been the "starting" interception
    ///       point according to the interceptor spec. This will be
    ///       fixed once Bug 1369 is completely done.
    void receive_request_service_contexts (
        TAO_ServerRequest &server_request,
        TAO::Argument * const args[],
        size_t nargs,
        void * servant_upcall,
        CORBA::TypeCode_ptr const * exceptions,
        CORBA::ULong nexceptions
        ACE_ENV_ARG_DECL);

    /// This method an "intermediate" server side interception point.
    void receive_request (
        TAO_ServerRequest &server_request,
        TAO::Argument * const args[],
        size_t nargs,
        void * servant_upcall,
        CORBA::TypeCode_ptr const * exceptions,
        CORBA::ULong nexceptions
        ACE_ENV_ARG_DECL);

    /// This method implements one of the "ending" server side
    /// interception points.
    void send_reply (
        TAO_ServerRequest &server_request,
        TAO::Argument * const args[],
        size_t nargs,
        void * servant_upcall,
        CORBA::TypeCode_ptr const * exceptions,
        CORBA::ULong nexceptions
        ACE_ENV_ARG_DECL);

    /// This method implements one of the "ending" server side
    /// interception points.
    void send_exception (
        TAO_ServerRequest &server_request,
        TAO::Argument * const args[],
        size_t nargs,
        void * servant_upcall,
        CORBA::TypeCode_ptr const * exceptions,
        CORBA::ULong nexceptions
        ACE_ENV_ARG_DECL);

    /// This method implements one of the "ending" server side
    /// interception points.
    void send_other (
        TAO_ServerRequest &server_request,
        TAO::Argument * const args[],
        size_t nargs,
        void * servant_upcall,
        CORBA::TypeCode_ptr const * exceptions,
        CORBA::ULong nexceptions
        ACE_ENV_ARG_DECL);
    //@}

    /// Register an interceptor.
    virtual void add_interceptor (
      PortableInterceptor::ServerRequestInterceptor_ptr interceptor
      ACE_ENV_ARG_DECL);

    virtual void add_interceptor (
      PortableInterceptor::ServerRequestInterceptor_ptr interceptor,
      const CORBA::PolicyList& policies
      ACE_ENV_ARG_DECL);

    virtual void destroy_interceptors (ACE_ENV_SINGLE_ARG_DECL);

    virtual TAO::PICurrent_Impl *allocate_pi_current (void);

    virtual void deallocate_pi_current (
        TAO::PICurrent_Impl *picurrent);

    virtual void execute_command (
        TAO_ServerRequest &server_request,
        TAO::Upcall_Command &command
        ACE_ENV_ARG_DECL);

  private:

    /// List of registered interceptors.
    ServerRequestInterceptor_List interceptor_list_;
  };

}  // End namespace TAO

TAO_END_VERSIONED_NAMESPACE_DECL

#endif  /* TAO_HAS_INTERCEPTORS */

#include /**/ "ace/post.h"

#endif /* TAO_SERVER_INTERCEPTOR_ADAPTER_H */
