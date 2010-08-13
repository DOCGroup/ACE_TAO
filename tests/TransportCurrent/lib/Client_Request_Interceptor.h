// -*- C++ -*-

//=============================================================================
/**
 * @file Client_Request_Interceptor.h
 *
 * $Id$
 *
 * Implementation header for the client request interceptor for the
 * Transport::Current test.
 *
 * @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================

#ifndef CLIENT_REQUEST_INTERCEPTOR_H
#define CLIENT_REQUEST_INTERCEPTOR_H

#include /**/ "ace/config.h"
#include /**/ "Current_Test_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PI/PI.h"
#include "tao/PortableInterceptorC.h"
#include "tao/LocalObject.h"
#include "tao/ORB.h"
#include "tao/CORBA_String.h"

namespace Test
{

  /// A metod doing the actual testing, so we could change it for the
  /// different traits implementations
  typedef int (*TEST) (CORBA::ORB_ptr);


  /**
   * @class Client_Request_Interceptor
   *
   * @brief Client request interceptor that exercises
   * Transport::Current support.
   *
   * This client request interceptor forwards a request to a secondary
   * object by throwing the PortableInterceptor::ForwardRequest
   * exception from within the send_request() interception point.  It
   * only forwards the initial request.  See the README file for
   * details.
   */
  class Current_Test_Export Client_Request_Interceptor
    : public virtual PortableInterceptor::ClientRequestInterceptor,
      public virtual ::CORBA::LocalObject
  {
 public:

    /// Constructor.
    Client_Request_Interceptor (const char *orb_id,
                                TEST test);
    /**
     * @name Methods Required by the Client Request Interceptor
     * Interface
     *
     * These are methods that must be implemented since they are pure
     * virtual in the abstract base class.  They are the canonical
     * methods required for all client request interceptors.
     */
    //@{
    /// Return the name of this ClientRequestinterceptor.
    virtual char * name (void);

    virtual void destroy (void);

    virtual void send_request (PortableInterceptor::ClientRequestInfo_ptr ri);

    virtual void send_poll (PortableInterceptor::ClientRequestInfo_ptr ri);

    virtual void receive_reply (PortableInterceptor::ClientRequestInfo_ptr ri);

    virtual void receive_exception (PortableInterceptor::ClientRequestInfo_ptr ri);

    virtual void receive_other (PortableInterceptor::ClientRequestInfo_ptr ri);
    //@}

    // Number of interceptions
    CORBA::Long interceptions (void);

  private:
    // Implementation method, every inteception point will have to
    // call it, supplying a name.
    void test_transport_current (const ACE_TCHAR* amethod);

  private:
    ///The ID of the ORB this interceptor is registered with.
    CORBA::String_var orb_id_;

    /// Pseudo-reference to the ORB this interceptor is registered
    /// with.
    CORBA::ORB_var orb_;

    /// The number of requests intercepted by this interceptor.
    CORBA::ULong request_count_;

    /// Stringified reference to the object the initial request will be
    /// forwarded to by this interceptor.
    CORBA::String_var forward_str_;

    /// Synchronize internal state...
    TAO_SYNCH_MUTEX lock_;

    /// Unique request ID
    unsigned long requestID_;

    /// Test function
    TEST test_;
  };

} // namespace Test

#endif  /* CLIENT_REQUEST_INTERCEPTOR_H */
