// -*- C++ -*-

//=============================================================================
/**
 * @file Server_Request_Interceptor.h
 *
 * $Id$
 *
 * Implementation header for the server request interceptor for the
 * PortableInterceptor::ForwardRequest test.
 *
 * @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================

#ifndef SERVER_REQUEST_INTERCEPTOR_H
#define SERVER_REQUEST_INTERCEPTOR_H

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Current_TestC.h"

#include "tao/TransportCurrent/TCC.h"

#include "tao/PortableInterceptorC.h"
#include "tao/LocalObject.h"
#include "tao/PI_Server/PI_Server.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

#include "ORBInitializer_T.h"

namespace Test
{

  /// A metod doing the actual testing, so we could change it for the
  /// different traits implementations

  typedef int (*TEST) (CORBA::ORB_ptr ACE_ENV_ARG_DECL_WITH_DEFAULTS);

  /**
   * @class Server_Request_Interceptor
   *
   * @brief Simple concrete server request interceptor.
   *
   */
  class Server_Request_Interceptor
    : public virtual PortableInterceptor::ServerRequestInterceptor,
      public virtual TAO_Local_RefCounted_Object
  {
  public:

    /// Constructor.
    Server_Request_Interceptor (const char*orb_id, TEST test);

    /// Destructor.
    virtual ~Server_Request_Interceptor (void);

    virtual void test_transport_current (const char* amethod
                                 ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual bool self_test (void) const;

    /**
     * @name Methods Required by the Server Request Interceptor
     * Interface
     *
     * These are methods that must be implemented since they are pure
     * virtual in the abstract base class.  They are the canonical
     * methods required for all server request interceptors.
     */
    //@{
    /// Return the name of this ServerRequestinterceptor.
    virtual char * name (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual void destroy (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual void receive_request_service_contexts (PortableInterceptor::ServerRequestInfo_ptr ri
                                                   ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableInterceptor::ForwardRequest));

    virtual void receive_request (
                                  PortableInterceptor::ServerRequestInfo_ptr ri
                                  ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableInterceptor::ForwardRequest));

    virtual void send_reply (
                             PortableInterceptor::ServerRequestInfo_ptr ri
                             ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual void send_exception (
                                 PortableInterceptor::ServerRequestInfo_ptr ri
                                 ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableInterceptor::ForwardRequest));

    virtual void send_other (
                             PortableInterceptor::ServerRequestInfo_ptr ri
                             ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableInterceptor::ForwardRequest));
    //@}

  protected:

    /// The number of requests intercepted by this interceptor.
    CORBA::ULong request_count_;

    CORBA::String_var orb_id_;

    TEST test_;

  };

  // A specialization to handle server-side interceptor registration
  template <>
  void ORBInitializer<PortableInterceptor::ServerRequestInterceptor>::post_init
    (PortableInterceptor::ORBInitInfo* oii ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));
};

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif  /* SERVER_REQUEST_INTERCEPTOR_H */
