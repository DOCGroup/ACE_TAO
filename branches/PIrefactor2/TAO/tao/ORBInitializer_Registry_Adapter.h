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

#include "ace/Service_Object.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/CORBA_macros.h"

namespace PortableInterceptor
{
  class ORBInitializer;
  typedef ORBInitializer *ORBInitializer_ptr;

  class ORBInitInfo;
  typedef ORBInitInfo *ORBInitInfo_ptr;
}

namespace TAO
{
  /**
   * @class ORBInitializer_Registry_Adapter
   *
   * Class that offers an interface to the ORB to load and manipulate
   * ORBInitializer_Registry
   */
  class ORBInitializer_Registry_Adapter
    : public ACE_Service_Object
  {
    public:
      /// Register an ORBInitializer with the underlying ORBInitializer
      /// array.
      virtual void register_orb_initializer (
        PortableInterceptor::ORBInitializer_ptr init
        ACE_ENV_ARG_DECL) = 0;

      /// Begin initialization of all registered ORBInitializers before
      /// the ORB itself is initialized.
      virtual void pre_init (
        PortableInterceptor::ORBInitInfo_ptr info
        ACE_ENV_ARG_DECL) = 0;

      /// Complete initialization of all registered ORBInitializers after
      /// the ORB has been initialized.
      virtual void post_init (
        PortableInterceptor::ORBInitInfo_ptr info
        ACE_ENV_ARG_DECL) = 0;
  };
}

#include /**/ "ace/post.h"

#endif /* TAO_ORB_INITIALIZER_REGISTRY_ADAPTER_H */
