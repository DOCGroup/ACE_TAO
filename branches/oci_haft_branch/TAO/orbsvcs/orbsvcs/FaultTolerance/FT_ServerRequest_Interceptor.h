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

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */
#include "fault_tol_export.h"
#include "tao/LocalObject.h"
#include "tao/PortableInterceptorC.h"
#include "tao/ORB.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

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
  class TAO_FT_Export FT_ServerRequest_Interceptor
    : public virtual PortableInterceptor::ServerRequestInterceptor,
      public virtual TAO_Local_RefCounted_Object
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
    virtual char * name (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual void destroy (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual void receive_request_service_contexts (
      PortableInterceptor::ServerRequestInfo_ptr ri
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest));

    virtual void receive_request (
        PortableInterceptor::ServerRequestInfo_ptr ri
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableInterceptor::ForwardRequest));

    virtual void send_reply (
        PortableInterceptor::ServerRequestInfo_ptr ri
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual void send_exception (
        PortableInterceptor::ServerRequestInfo_ptr ri
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableInterceptor::ForwardRequest));

    virtual void send_other (
        PortableInterceptor::ServerRequestInfo_ptr ri
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableInterceptor::ForwardRequest));
    //@}
  private:

    void check_iogr_version (const IOP::ServiceContext &svc
                             ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    bool is_primary (void);

    void update_iogr (
        PortableInterceptor::ServerRequestInfo_ptr ri
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

  private:

    CORBA::ULong object_group_ref_version_;

    bool is_primary_;

    CORBA::Object_var iogr_;

    CORBA::ORB_var orb_;
  };

}
#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif  /* TAO_FT_SERVER_REQUEST_INTERCEPTOR_H */
