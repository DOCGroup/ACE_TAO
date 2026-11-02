// -*- C++ -*-

//=============================================================================
/**
 * @file IIOP_Server_Request_Interceptor.h
 *
 * Implementation header for the server request interceptor for the
 * IIOP_Threading test.
 *
 * @author Iliyan Jeliazkov <iliyan@ociweb.com>
 * @author Ciju John <johnc@ociweb.com>
 */
//=============================================================================

#ifndef ISERVER_REQUEST_INTERCEPTOR_H
#define ISERVER_REQUEST_INTERCEPTOR_H

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/TransportCurrent/IIOP_Transport_Current.h"

#include "Server_Request_Interceptor.h"

#include <array>
#include <cstring>

namespace Test
{
  class IIOP_Server_Request_Interceptor
    : public virtual Test::Server_Request_Interceptor
    , public virtual ::CORBA::LocalObject
  {
  public:
    /// Constructor.
    IIOP_Server_Request_Interceptor (char const *orbid, TEST test);

    /// Destructor.
    ~IIOP_Server_Request_Interceptor () override = default;

    IIOP_Server_Request_Interceptor (IIOP_Server_Request_Interceptor const &) = delete;
    IIOP_Server_Request_Interceptor (IIOP_Server_Request_Interceptor &&) = delete;
    IIOP_Server_Request_Interceptor &operator= (IIOP_Server_Request_Interceptor const &) = delete;
    IIOP_Server_Request_Interceptor &operator= (IIOP_Server_Request_Interceptor &&) = delete;

    /// queries the status of the test
    bool self_test () override;

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
    char * name () override;

    /// incoming interception point
    void receive_request_service_contexts (PortableInterceptor::ServerRequestInfo_ptr) override;

    /// outgoing interception point
    void send_reply (PortableInterceptor::ServerRequestInfo_ptr ri) override;

    /// outgoing interception point
    void send_exception (PortableInterceptor::ServerRequestInfo_ptr ri) override;

    /// outgoing interception point
    void send_other (PortableInterceptor::ServerRequestInfo_ptr ri) override;
    //@}

  protected:
    /// process incoming requests context
    void inbound_process_context
    (PortableInterceptor::ServerRequestInfo_ptr ri);

    /// process outgoing requests context
    void outbound_process_context
    (PortableInterceptor::ServerRequestInfo_ptr ri);

    /// saves the incoming request info
    void push_request_info (size_t requestID);

    /// clears the outgoing request info
    void pop_request_info (size_t requestID);

    TAO::Transport::IIOP::Current_ptr resolve_iiop_transport_current (char const *orbid);

  private:
    /// transport ID dtata structure
    struct EndPoint
    {
      CORBA::UShort port_ {};
      CORBA::String_var host_ {};

      EndPoint () = default;

      EndPoint (CORBA::UShort port, char const *host)
        : port_ (port)
        , host_ (host)
      {}

      bool operator== (EndPoint const &ep) const
      {
        return (this->port_ == ep.port_) &&
          (std::strcmp (this->host_.in (), ep.host_.in ()) == 0);
      }
      bool operator!= (EndPoint const &ep) const
      {
        return !(*this == ep);
      }
    };

  private:
    /// IIOPTraits test status
    bool iiop_test_successful_;

    /// Endpoints hash table
    std::array<EndPoint, 1001> endPoints_ {};
  };
} // namespace Test

#endif  /* ISERVER_REQUEST_INTERCEPTOR_H */
