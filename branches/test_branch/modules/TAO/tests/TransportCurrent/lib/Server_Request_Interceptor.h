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

#include /**/ "ace/config.h"
#include /**/ "Current_Test_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Current_TestC.h"

#include "tao/TransportCurrent/TCC.h"
#include "tao/PortableInterceptorC.h"
#include "tao/LocalObject.h"
#include "tao/PI_Server/PI_Server.h"

namespace Test
{

  /// A metod doing the actual testing, so we could change it for the
  /// different traits implementations

  typedef int (*TEST) (CORBA::ORB_ptr);

  /**
   * @class Server_Request_Interceptor
   *
   * @brief Simple concrete server request interceptor.
   *
   */
  class Current_Test_Export Server_Request_Interceptor
    : public virtual PortableInterceptor::ServerRequestInterceptor,
      public virtual ::CORBA::LocalObject
  {
  public:

    /// Constructor.
    Server_Request_Interceptor (const char*orb_id,
                                TEST test);

    /// Destructor.
    virtual ~Server_Request_Interceptor (void);

    virtual void test_transport_current (const ACE_TCHAR* amethod);

    /// Coherency test
    virtual bool self_test (void);

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
    virtual char * name (void);

    virtual void destroy (void);

    virtual void receive_request_service_contexts (PortableInterceptor::ServerRequestInfo_ptr ri);

    virtual void receive_request (PortableInterceptor::ServerRequestInfo_ptr ri);

    virtual void send_reply (PortableInterceptor::ServerRequestInfo_ptr ri);

    virtual void send_exception (PortableInterceptor::ServerRequestInfo_ptr ri);

    virtual void send_other (PortableInterceptor::ServerRequestInfo_ptr ri);
    //@}

  protected:

    /// The number of requests intercepted by this interceptor.
    CORBA::ULong request_count_;

    CORBA::String_var orb_id_;

    TEST test_;

  };

}

#endif  /* SERVER_REQUEST_INTERCEPTOR_H */
