// -*- C++ -*-

//=============================================================================
/**
 * @file TSS_RequestInterceptor.h
 *
 * $Id$
 *
 * Implementation header for the server request interceptor for the
 * Security Attribute Service (server side).
 *
 * @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef TAO_TSS_REQUEST_INTERCEPTOR_H
#define TAO_TSS_REQUEST_INTERCEPTOR_H

#include "ace/pre.h"
#include "orbsvcs/Security/security_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

namespace TAO
{
  /**
   * @class TSS_RequestInterceptor
   *
   * @brief Target Security Service (TSS) server request interceptor.
   *
   * This server request interceptor implements Security Attribute
   * Service functionality of the Target Security Service.
   */
  class TAO_Security_Export TSS_RequestInterceptor
    : public virtual PortableInterceptor::ServerRequestInterceptor,
      public virtual TAO_Local_RefCounted_Object
  {
  public:

    /**
     * Error codes used within a ContextError service context.
     */
    typedef enum
      {
        NONE,
        INVALID_EVIDENCE,      // Major = 1, Minor = 1, NO_PERMISSION
        INVALID_MECHANISM,     // Major = 2, Minor = 1, NO_PERMISSION
        CONFLICTING_EVIDENCE,  // Major = 3, Minor = 1, NO_PERMISSION
        NO_CONTEXT,            // Major = 4, Minor = 1, NO_PERMISSION
        POLICY_CHANGE
      } ErrorCode;

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

    virtual void receive_request_service_contexts (
        PortableInterceptor::ServerRequestInfo_ptr ri
        ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableInterceptor::ForwardRequest));

    virtual void receive_request (PortableInterceptor::ServerRequestInfo_ptr ri
                                  ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableInterceptor::ForwardRequest));

    virtual void send_reply (PortableInterceptor::ServerRequestInfo_ptr ri
                             ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual void send_exception (PortableInterceptor::ServerRequestInfo_ptr ri
                                 ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableInterceptor::ForwardRequest));

    virtual void send_other (PortableInterceptor::ServerRequestInfo_ptr ri
                             ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableInterceptor::ForwardRequest));
    //@}

  private:

    CORBA::Boolean accept_context ();
    CORBA::Boolean accept_transport_context ();
    void reference_context ();
    void discard_context ();

    CORBA::Boolean extract_sas_context ();
    void add_complete_establish_context ();
    void add_context_error ();

  };
}  // End TAO namespace

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"

#endif  /* TAO_TSS_REQUEST_INTERCEPTOR_H */
