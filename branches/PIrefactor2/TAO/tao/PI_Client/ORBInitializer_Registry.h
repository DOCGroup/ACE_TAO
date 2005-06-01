// -*- C++ -*-

// ===================================================================
/**
 *  @file   ORBInitializer_Registry.h
 *
 *  $Id$
 *
 *  @author Ossama Othman <ossama@uci.edu>
 */
// ===================================================================

#ifndef TAO_ORB_INITIALIZER_REGISTRY_H
#define TAO_ORB_INITIALIZER_REGISTRY_H

#include /**/ "ace/pre.h"

#include "pi_client_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Array_Base.h"
#include "ace/Service_Config.h"
#include "tao/CORBA_methods.h"
#include "tao/Objref_VarOut_T.h"
#include "ace/Thread_Mutex.h"
#include "tao/ORBInitializer_Registry_Adapter.h"

#include "ORBInitializerC.h"

namespace TAO
{
  /**
   * @class ORBInitializer_Registry
   *
   * @brief Global list that contains all portable interceptor ORB
   *        initializers.
   *
   * @note This class should be instantiated via its instance()
   *       method.  Normally this would be enforced by making the
   *       constructor protected but that forces a friend declaration
   *       containing a template type (TAO_Singleton) with a static
   *       member to be introduced.  In turn, this potentially
   *       introduces problems in MS Windows DLL environments due to
   *       the occurance of multiple singleton instances.  There
   *       should only be one!
   */
  class TAO_PI_CLIENT_Export ORBInitializer_Registry
    : public ORBInitializer_Registry_Adapter
  {
    public:
      ORBInitializer_Registry (void);

      /// Register an ORBInitializer with the underlying ORBInitializer
      /// array.
      virtual void register_orb_initializer (
        PortableInterceptor::ORBInitializer_ptr init
        ACE_ENV_ARG_DECL);

      /// Begin initialization of all registered ORBInitializers before
      /// the ORB itself is initialized.
      virtual void pre_init (
        PortableInterceptor::ORBInitInfo_ptr info
        ACE_ENV_ARG_DECL);

      /// Complete initialization of all registered ORBInitializers after
      /// the ORB has been initialized.
      virtual void post_init (
        PortableInterceptor::ORBInitInfo_ptr info
        ACE_ENV_ARG_DECL);

  private:
    // Prevent copying
    ORBInitializer_Registry (const ORBInitializer_Registry &);
    void operator= (const ORBInitializer_Registry &);

  private:
    TAO_SYNCH_MUTEX lock_;

    /// Dynamic array containing registered ORBInitializers.
    ACE_Array_Base<PortableInterceptor::ORBInitializer_var> initializers_;
  };

  ACE_STATIC_SVC_DECLARE (ORBInitializer_Registry)
  ACE_FACTORY_DECLARE (TAO_PI_CLIENT, ORBInitializer_Registry)

}

#include /**/ "ace/post.h"

#endif /* TAO_ORB_INITIALIZER_REGISTRY_H */
