// -*- C++ -*-

//=============================================================================
/**
 * @file TSS_ORBInitializer.h
 *
 * $Id$
 *
 * @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef TAO_TSS_ORB_INITIALIZER_H
#define TAO_TSS_ORB_INITIALIZER_H

#include "ace/config-all.h"

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
   * @class TSS_ORBInitializer
   *
   * @brief TAO CSIv2 server side ORBInitializer.
   *
   * CSIv2 target security service (TSS) ORBInitializer that registers
   * a TSS ServerRequestInterceptor with all ORBs.
   */
  class TSS_ORBInitializer
    : public virtual PortableInterceptor::ORBInitializer,
      public virtual TAO_Local_RefCounted_Object
  {
  public:

    /**
     * @name Methods Required by the ORBInitializer Interface
     *
     * These are methods that must be implemented since they are pure
     * virtual in the abstract base class.  They are the canonical
     * methods required for all ORB initializers.
     */
    //@{
    /// The pre-initialization hook.
    virtual void pre_init (PortableInterceptor::ORBInitInfo_ptr info
			   ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    /// The post-initialization hook.
    virtual void post_init (PortableInterceptor::ORBInitInfo_ptr info
			    ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));
    //@}
  };
}  // End TAO namespace

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif  /* TAO_TSS_ORB_INITIALIZER_H */
