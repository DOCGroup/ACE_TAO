// -*- C++ -*-

//=============================================================================
/**
 * @file CSS_RequestInterceptor.h
 *
 * $Id$
 *
 * Implementation header for the client request interceptor for the
 * Security Attribute Service (client side).
 *
 * @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================


#ifndef TAO_CSS_REQUEST_INTERCEPTOR_H
#define TAO_CSS_REQUEST_INTERCEPTOR_H

#include "ace/pre.h"
#include "orbsvcs/Security/security_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableInterceptorC.h"
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
   * @class CSS_RequestInterceptor
   *
   * @brief Client Security Service (CSS) client request interceptor.
   *
   * This client request interceptor implements Security Attribute
   * Service functionality of the Client Security Service.
   */
  class TAO_Security_Export CSS_RequestInterceptor
  : public virtual PortableInterceptor::ClientRequestInterceptor,
    public virtual TAO_Local_RefCounted_Object
  {
  public:

    /// Constructor.
    CSS_Requestinterceptor (void);

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
    virtual char * name (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual void destroy (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual void send_request (PortableInterceptor::ClientRequestInfo_ptr ri
                               ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableInterceptor::ForwardRequest));

    virtual void send_poll (PortableInterceptor::ClientRequestInfo_ptr ri
                            ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual void receive_reply (PortableInterceptor::ClientRequestInfo_ptr ri
                                ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual void receive_exception (
        PortableInterceptor::ClientRequestInfo_ptr ri
        ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableInterceptor::ForwardRequest));

    virtual void receive_other (PortableInterceptor::ClientRequestInfo_ptr ri
                                ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableInterceptor::ForwardRequest));
    //@}

  };

}  // End TAO namespace


#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"

#endif  /* TAO_CSS_REQUEST_INTERCEPTOR_H */
