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

#include "TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Object.h"
#include "ace/CORBA_macros.h"
#include "tao/Basic_Types.h"

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
        PortableInterceptor::ORBInitializer_ptr init
        ACE_ENV_ARG_DECL) = 0;

      /// Begin initialization of all registered ORBInitializers before
      /// the ORB itself is initialized.
      virtual void pre_init (
        TAO_ORB_Core *orb_core,
        int argc,
        char *argv[],
        PortableInterceptor::SlotId &slotid
        ACE_ENV_ARG_DECL) = 0;

      /// Complete initialization of all registered ORBInitializers after
      /// the ORB has been initialized. Returns the number of slots
      /// allocated
      virtual void post_init (
        TAO_ORB_Core *orb_core,
        int argc,
        char *argv[],
        PortableInterceptor::SlotId &slotid
        ACE_ENV_ARG_DECL) = 0;
  };
}

#include /**/ "ace/post.h"

#endif /* TAO_ORB_INITIALIZER_REGISTRY_ADAPTER_H */
