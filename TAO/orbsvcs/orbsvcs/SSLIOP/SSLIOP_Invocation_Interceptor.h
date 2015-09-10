// -*- C++ -*-

// ===================================================================
/**
 *  @file   SSLIOP_Invocation_Interceptor.h
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
#include "orbsvcs/SecurityLevel2C.h"
#include "tao/PortableInterceptorC.h"
#include "tao/PI/ORBInitInfo.h"
#include "tao/PI_Server/PI_Server.h"
#include "tao/PortableServer/PS_CurrentC.h"
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
     * secure invocations. Two instances are going to be used,
     * one for collocated and one for remote invocations because
     * within the interception point we don't know whether we
     * are collocated or not
     */
    class Server_Invocation_Interceptor
      : public virtual PortableInterceptor::ServerRequestInterceptor,
        public virtual ::CORBA::LocalObject
    {
    public:

      /*!
        \brief Constructor.
        \param info reference to the ORBInitInfo object so that
                    the interceptor can get access to initial references, etc.
        \param default_qop the default Quality of Protection
        \param tss_slot the TSS slot used by the various security features.
        \param collocated Are we handling collocated calls in this instance or not
      */
      Server_Invocation_Interceptor (PortableInterceptor::ORBInitInfo_ptr info,
                                     ::Security::QOP default_qop,
                                     size_t tss_slot,
                                     bool collocated);

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

      /// Reference to the POA current
      PortableServer::Current_var poa_current_;

      /// The default quality-of-protection settings in use.
      ::Security::QOP qop_;

      /// Are we handling collocated calls
      bool collocated_;

      /// SecurityLevel2 security manager reference
      SecurityLevel2::SecurityManager_var sec2manager_;
      SecurityLevel2::Current_var sec2_current_;
    };

  }  // End SSLIOP namespace.
}  // End TAO namespace.

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"

#endif  /* TAO_SSLIOP_INVOCATION_INTERCEPTOR_H */
