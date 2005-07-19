// -*- C++ -*-

// ===================================================================
/**
 *  @file   ORBInitializer_Registry_Impl.h
 *
 *  $Id$
 *
 *  @author Ossama Othman <ossama@uci.edu>
 */
// ===================================================================

#ifndef TAO_PI_ORB_INITIALIZER_REGISTRY_IMPL_H
#define TAO_PI_ORB_INITIALIZER_REGISTRY_IMPL_H

#include /**/ "ace/pre.h"

#include "pi_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Array_Base.h"
#include "ace/Service_Config.h"
#include "tao/CORBA_methods.h"
#include "tao/Objref_VarOut_T.h"
#include "ace/Recursive_Thread_Mutex.h"
#include "tao/ORBInitializer_Registry_Adapter.h"

#include "PI.h"

namespace TAO
{
  /**
   * @class ORBInitializer_Registry
   *
   * @brief Global list that contains all portable interceptor ORB
   *        initializers.
   */
  class TAO_PI_Export ORBInitializer_Registry
    : public ORBInitializer_Registry_Adapter
  {
    public:
      ORBInitializer_Registry (void);

      /// Service config fini method, release all ORBInitializers at this
      /// moment
      virtual int fini (void);

      /// Register an ORBInitializer with the underlying ORBInitializer
      /// array.
      virtual void register_orb_initializer (
        PortableInterceptor::ORBInitializer_ptr init
        ACE_ENV_ARG_DECL);

      /// Begin initialization of all registered ORBInitializers before
      /// the ORB itself is initialized.
      virtual size_t pre_init (
        TAO_ORB_Core *orb_core,
        int argc,
        char *argv[],
        PortableInterceptor::SlotId &slotid
        ACE_ENV_ARG_DECL);

      /// Complete initialization of all registered ORBInitializers after
      /// the ORB has been initialized.
      virtual void post_init (
        size_t pre_init_count,
        TAO_ORB_Core *orb_core,
        int argc,
        char *argv[],
        PortableInterceptor::SlotId &slotid
        ACE_ENV_ARG_DECL);

  private:
    // Prevent copying
    ORBInitializer_Registry (const ORBInitializer_Registry &);
    void operator= (const ORBInitializer_Registry &);

  private:
    TAO_SYNCH_RECURSIVE_MUTEX lock_;

    /// Dynamic array containing registered ORBInitializers.
    ACE_Array_Base<PortableInterceptor::ORBInitializer_var> initializers_;
  };
}

ACE_STATIC_SVC_DECLARE (ORBInitializer_Registry)
ACE_FACTORY_DECLARE (TAO_PI, ORBInitializer_Registry)

#include /**/ "ace/post.h"

#endif /* TAO_PI_ORB_INITIALIZER_REGISTRY_IMPL_H */
