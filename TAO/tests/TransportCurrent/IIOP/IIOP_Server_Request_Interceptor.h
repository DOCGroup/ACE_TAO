// -*- C++ -*-

//=============================================================================
/**
 * @file IIOP_Server_Request_Interceptor.h
 *
 * $Id$
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

namespace Test
{

  class IIOP_Server_Request_Interceptor
    : public virtual Test::Server_Request_Interceptor
    , public virtual TAO_Local_RefCounted_Object
  {
  public:

    /// Constructor.
    IIOP_Server_Request_Interceptor (const char* orbid, TEST test);

    /// Destructor.
    virtual ~IIOP_Server_Request_Interceptor ();

    /// queries the status of the test
    virtual bool self_test ();

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
    virtual char * name (void)
      ACE_THROW_SPEC ((CORBA::SystemException));

    /// incomming interception point
    virtual void receive_request_service_contexts (PortableInterceptor::ServerRequestInfo_ptr
                                                   ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableInterceptor::ForwardRequest));

    /// outgoing interception point
    virtual void send_reply (PortableInterceptor::ServerRequestInfo_ptr ri
                             ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    /// outgoing interception point
    virtual void send_exception (PortableInterceptor::ServerRequestInfo_ptr ri
                                 ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableInterceptor::ForwardRequest));

    /// outgoing interception point
    virtual void send_other (PortableInterceptor::ServerRequestInfo_ptr ri
                             ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableInterceptor::ForwardRequest));
    //@}

  protected:

    /// process incomming requests context
    void inbound_process_context
    (PortableInterceptor::ServerRequestInfo_ptr ri
     ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    /// process outgoing requests context
    void outbound_process_context
    (PortableInterceptor::ServerRequestInfo_ptr ri
     ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    /// saves the incomming request info
    void push_request_info (size_t requestID
                            ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    /// clears the outgoing request info
    void pop_request_info (size_t requestID);

    TAO::Transport::IIOP::Current_ptr resolve_iiop_transport_current (const char* orbid
                                                                      ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

  private:

    /// transport ID dtata structure
    typedef struct EP {
      CORBA::UShort port_;
      CORBA::String_var host_;

      EP ()
        : port_(0)
        , host_ ()
      {};

      EP (CORBA::UShort port, const char* host)
        : port_(port)
        , host_ (host)
      {};

      bool operator== (const struct EP &ep) const
      {
        return (this->port_ == ep.port_) &&
          (strcmp (this->host_.in (), ep.host_.in ()) == 0);
      };
      bool operator!= (const struct EP &ep) const
      {
        return !(this->operator== (ep));
      };

    } EndPoint;

  private:

    /// IIOPTraits test status
    bool iiop_test_successful_;

    /// Multi-threaded test status
    bool mult_thr_test_successful_;

    /// Endpoints hash table
    EndPoint endPoints_[1001];
  };

} // namespace Test

#endif  /* ISERVER_REQUEST_INTERCEPTOR_H */
