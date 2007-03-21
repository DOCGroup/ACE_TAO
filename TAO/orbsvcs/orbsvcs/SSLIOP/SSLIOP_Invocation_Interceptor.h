// -*- C++ -*-

// ===================================================================
/**
 *  @file   SSLIOP_Invocation_Interceptor.h
 *
 *  $Id$
 *
 *  @author Ossama Othman <ossama@uci.edu>
 */
// ===================================================================

#ifndef TAO_SSLIOP_INVOCATION_INTERCEPTOR_H
#define TAO_SSLIOP_INVOCATION_INTERCEPTOR_H

#include /**/ "ace/pre.h"

#include "orbsvcs/SSLIOP/SSLIOP_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/SSLIOPC.h"
#include "tao/PortableInterceptorC.h"
#include "tao/PI_Server/PI_Server.h"
#include "tao/LocalObject.h"

// This is to remove "inherits via dominance" warnings from MSVC.
// MSVC is being a little too paranoid.
#if defined (_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace SSLIOP
  {
    /**
     * @class Server_Invocation_Interceptor
     *
     * @brief Secure invocation server request interceptor.
     *
     * This server request interceptor rejects insecure request
     * invocations if the effective target object policy requires
     * secure invocations.
     */
    class Server_Invocation_Interceptor
      : public virtual PortableInterceptor::ServerRequestInterceptor,
        public virtual TAO_Local_RefCounted_Object
    {
    public:

      /// Constructor.
      Server_Invocation_Interceptor (::SSLIOP::Current_ptr current,
                                     ::Security::QOP qop);

      /**
       * @name PortableInterceptor::ServerRequestInterceptor Methods
       *
       * Methods required by the
       * PortableInterceptor::ServerRequestInterceptor interface.
       */
      //@{
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

    protected:

      /// Destructor
      /**
       * Protected destructor to force deallocation by the reference
       * counting mechanism.
       */
      ~Server_Invocation_Interceptor (void);

    private:

      /**
       * @name Copying and Assignment
       *
       * Protected to prevent copying through the copy constructor and the
       * assignment operator.
       */
      //@{
      Server_Invocation_Interceptor (const Server_Invocation_Interceptor &);
      void operator= (const Server_Invocation_Interceptor &);
      //@}

    private:

      /// Reference to the current SSLIOP execution context.
      ::SSLIOP::Current_var ssliop_current_;

      /// The default quality-of-protection settings in use.
      ::Security::QOP qop_;

    };

  }  // End SSLIOP namespace.
}  // End TAO namespace.

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"

#endif  /* TAO_SSLIOP_INVOCATION_INTERCEPTOR_H */
