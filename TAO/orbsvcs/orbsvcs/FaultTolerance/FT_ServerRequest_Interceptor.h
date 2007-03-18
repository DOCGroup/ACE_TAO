// -*- C++ -*-

//=============================================================================
/**
 * @file FT_ServerRequest_Interceptor.h
 *
 * $Id$
 *
 * ServerRequest_Interceptor for the FTORB
 *
 * @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================

#ifndef TAO_FT_SERVER_REQUEST_INTERCEPTOR_H
#define TAO_FT_SERVER_REQUEST_INTERCEPTOR_H

#include "orbsvcs/FaultTolerance/FT_ServerORB_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PI_Server/PI_Server.h"

#include "tao/LocalObject.h"
#include "tao/PortableInterceptorC.h"
#include "tao/ORB.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace IOP
{
  struct ServiceContext;
}
namespace TAO
{
  /**
   * @class Server_Request_Interceptor
   *
   * @brief Simple concrete server request interceptor for FTORB
   *
   *
   */
  class FT_ServerRequest_Interceptor
    : public virtual PortableInterceptor::ServerRequestInterceptor
    , public virtual TAO_Local_RefCounted_Object
  {
  public:

    /// Constructor.
    FT_ServerRequest_Interceptor (void);

    /// Destructor.
    ~FT_ServerRequest_Interceptor (void);

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

    virtual void receive_request_service_contexts (
      PortableInterceptor::ServerRequestInfo_ptr ri);

    virtual void receive_request (
        PortableInterceptor::ServerRequestInfo_ptr ri);

    virtual void send_reply (
        PortableInterceptor::ServerRequestInfo_ptr ri);

    virtual void send_exception (
        PortableInterceptor::ServerRequestInfo_ptr ri);

    virtual void send_other (
        PortableInterceptor::ServerRequestInfo_ptr ri);
    //@}
  private:

    void check_iogr_version (const IOP::ServiceContext &svc);

    void update_iogr (
        PortableInterceptor::ServerRequestInfo_ptr ri);

  private:

    CORBA::ULong object_group_ref_version_;

    CORBA::Boolean is_primary_;

    CORBA::Object_var iogr_;

    CORBA::ORB_var orb_;
  };

}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif  /* TAO_FT_SERVER_REQUEST_INTERCEPTOR_H */
