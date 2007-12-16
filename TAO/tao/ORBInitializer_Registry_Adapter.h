// -*- C++ -*-

// ===================================================================
/**
 *  @file   ORBInitializer_Registry_Adapter.h
 *
 *  $Id$
 *
 *  @author Johnny Willemsen  <jwillemsen@remedy.nl>
 */
// ===================================================================

#ifndef TAO_ORB_INITIALIZER_REGISTRY_ADAPTER_H
#define TAO_ORB_INITIALIZER_REGISTRY_ADAPTER_H

#include /**/ "ace/pre.h"

#include /**/ "tao/TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Basic_Types.h"
#include "ace/Service_Object.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_ORB_Core;

namespace PortableInterceptor
{
  class ORBInitializer;
  typedef ORBInitializer *ORBInitializer_ptr;

  typedef CORBA::ULong SlotId;
}

namespace TAO
{
  /**
   * @class ORBInitializer_Registry_Adapter
   *
   * Class that offers an interface to the ORB to load and manipulate
   * ORBInitializer_Registry
   */
  class TAO_Export ORBInitializer_Registry_Adapter
    : public ACE_Service_Object
  {
    public:
      virtual ~ORBInitializer_Registry_Adapter (void);

      /// Register an ORBInitializer with the underlying ORBInitializer
      /// array.
      virtual void register_orb_initializer (
        PortableInterceptor::ORBInitializer_ptr init) = 0;

      /**
       * Begin initialization of all registered ORBInitializers before
       * the ORB itself is initialized.
       * @return Returns the number of initializers called by pre_init, exactly
       * the same number must be called by post_init. During pre_init new
       * initializers can be registered, these must not be called during
       * post_init
       */
      virtual size_t pre_init (
        TAO_ORB_Core *orb_core,
        int argc,
        char *argv[],
        PortableInterceptor::SlotId &slotid) = 0;

      /**
       * Complete initialization of all registered ORBInitializers after
       * the ORB has been initialized. Returns the number of slots
       * allocated.
       * @param pre_init_count The number of ORBInitializers we must
       * call post_init on. There can be more in the registry when during
       * pre_init new interceptors are regisered
       * @param orb_core The ORB Core
       * @param argc Number of arguments
       * @param argv Argv array
       * @param slotid The slot id
       */
      virtual void post_init (
        size_t pre_init_count,
        TAO_ORB_Core *orb_core,
        int argc,
        char *argv[],
        PortableInterceptor::SlotId slotid) = 0;
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_ORB_INITIALIZER_REGISTRY_ADAPTER_H */
