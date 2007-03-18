// -*- C++ -*-

//=============================================================================
/**
 *  @file     Security_ORBInitializer.h
 *
 *  $Id$
 *
 *  @author  Ossama Othman <ossama@uci.edu>
 */
//=============================================================================


#ifndef TAO_SECURITY_ORB_INITIALIZER_H
#define TAO_SECURITY_ORB_INITIALIZER_H

#include /**/ "ace/pre.h"

#include "orbsvcs/Security/security_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PI/PI.h"
#include "tao/LocalObject.h"

#include "orbsvcs/Security/Security_PolicyFactory.h"

// This is to remove "inherits via dominance" warnings from MSVC.
// MSVC is being a little too paranoid.
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/// Security ORB initializer.
namespace TAO
{
  namespace Security
  {

    /**
     * @class ORBInitializer
     *
     * @brief ORBInitializer that configures CORBA Security features
     *        into an ORB.
     *
     * This ORBInitializer configures CORBA Security features into an
     * ORB, such as CSIv2, security objects, security policy
     * factories, etc, into an ORB.
     */
    class TAO_Security_Export ORBInitializer
      : public virtual PortableInterceptor::ORBInitializer,
        public virtual TAO_Local_RefCounted_Object
    {
    public:

      /**
       * @name PortableInterceptor::ORBInitializer Methods.
       *
       * Methods required by the PortableInterceptor::ORBInitializer
       * interface.
       */
      //@{
      virtual void pre_init (PortableInterceptor::ORBInitInfo_ptr info);

      virtual void post_init (PortableInterceptor::ORBInitInfo_ptr info);
      //@}

    private:

      /// Register Security policy factories.
      void register_policy_factories (PortableInterceptor::ORBInitInfo_ptr info);

    private:

      /// PolicyFactory that is used to create all security related
      /// policies capable of being created via ORB::create_policy().
      PortableInterceptor::PolicyFactory_var policy_factory_;

    };

  } // End Security namespace
}  // End TAO namespace

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"

#endif /* TAO_SECURITY_ORB_INITIALIZER_H */
