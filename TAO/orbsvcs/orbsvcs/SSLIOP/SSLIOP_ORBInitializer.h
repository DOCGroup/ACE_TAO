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

#include "orbsvcs/SSLIOP/SSLIOP_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CSIIOPC.h"
#include "orbsvcs/SecurityC.h"

#include "tao/PI/PI.h"
#include "tao/LocalObject.h"

// This is to remove "inherits via dominance" warnings from MSVC.
// MSVC is being a little too paranoid.
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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
    class ORBInitializer
      : public virtual PortableInterceptor::ORBInitializer,
        public virtual TAO_Local_RefCounted_Object
    {
    public:

      /// Constructor.
      ORBInitializer (::Security::QOP qop,
                      CSIIOP::AssociationOptions csiv2_target_supports,
                      CSIIOP::AssociationOptions csiv2_target_requires);

      virtual void pre_init (PortableInterceptor::ORBInitInfo_ptr info);

      virtual void post_init (PortableInterceptor::ORBInitInfo_ptr info);

    private:

      // Obtain the TSS slot ID assigned to the "SSLIOPCurrent" object.
      size_t get_tss_slot_id (PortableInterceptor::ORBInitInfo_ptr info);

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

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"

#endif /* TAO_SSLIOP_ORB_INITIALIZER_H */
