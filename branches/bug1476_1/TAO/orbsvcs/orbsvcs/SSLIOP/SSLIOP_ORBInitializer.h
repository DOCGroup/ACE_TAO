// -*- C++ -*-

//=============================================================================
/**
 *  @file    SSLIOP_ORBInitializer.h
 *
 *  $Id$
 *
 *  @author  Ossama Othman <ossama@uci.edu>
 */
//=============================================================================


#ifndef TAO_SSLIOP_ORB_INITIALIZER_H
#define TAO_SSLIOP_ORB_INITIALIZER_H

#include /**/ "ace/pre.h"

#include "SSLIOP_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/orbsvcs/CSIIOPC.h"
#include "orbsvcs/orbsvcs/SecurityC.h"

#include "tao/PortableInterceptorC.h"
#include "tao/LocalObject.h"

// This is to remove "inherits via dominance" warnings from MSVC.
// MSVC is being a little too paranoid.
#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

namespace TAO
{
  namespace SSLIOP
  {

    /**
     * @name ORBInitializer
     *
     * @brief
     * ORB initializer that registers all SSLIOP-specific interceptors and
     * object references.
     */
    class TAO_SSLIOP_Export ORBInitializer :
      public virtual PortableInterceptor::ORBInitializer,
      public virtual TAO_Local_RefCounted_Object
    {
    public:

      /// Constructor.
      ORBInitializer (::Security::QOP qop,
                      CSIIOP::AssociationOptions csiv2_target_supports,
                      CSIIOP::AssociationOptions csiv2_target_requires);

      virtual void pre_init (PortableInterceptor::ORBInitInfo_ptr info
                         ACE_ENV_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException));

      virtual void post_init (PortableInterceptor::ORBInitInfo_ptr info
                              ACE_ENV_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException));

    private:

      // Obtain the TSS slot ID assigned to the "SSLIOPCurrent" object.
      size_t get_tss_slot_id (PortableInterceptor::ORBInitInfo_ptr info
                              ACE_ENV_ARG_DECL);

    private:

      /// The default quality-of-protection settings in use.
      ::Security::QOP qop_;

      /// Default support CSIv2 association options.
      CSIIOP::AssociationOptions csiv2_target_supports_;

      /// Default required CSIv2 association options.
      CSIIOP::AssociationOptions csiv2_target_requires_;
    };

  }  // End SSLIOP namespace.
}  // End TAO namespace.

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"

#endif /* TAO_SSLIOP_ORB_INITIALIZER_H */
