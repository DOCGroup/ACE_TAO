// -*- C++ -*-

//=============================================================================
/**
 *  @file    ImR_Client_Adapter_Impl.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================


#ifndef TAO_IMR_CLIENT_ADAPTER_IMPL_H
#define TAO_IMR_CLIENT_ADAPTER_IMPL_H

#include /**/ "ace/pre.h"

#include "imr_client_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/ImR_Client_Adapter.h"

#include "ace/Service_Config.h"

class ServerObject_i;

namespace TAO
{
  namespace ImR_Client
  {
    /**
     * @class IFR_Client_Adapter_Impl
     *
     * @brief IFR_Client_Adapter_Impl.
     *
     * Class that adapts various functions in the PortableServer library
     * which use the Implementation Repository. This is the derived class
     * that contains the actual implementations.
     */
    class TAO_IMR_Client_Export ImR_Client_Adapter_Impl
      : public ::TAO::Portable_Server::ImR_Client_Adapter
    {
    public:
      // Used to force the initialization of the PortableServer code.
      static int Initializer (void);

      /// ImplRepo helper method, notify the ImplRepo on startup
      virtual void imr_notify_startup (TAO_Root_POA* poa ACE_ENV_ARG_DECL);

      /// ImplRepo helper method, notify the ImplRepo on shutdown
      virtual void imr_notify_shutdown (TAO_Root_POA* poa ACE_ENV_ARG_DECL);

    private:
      /// Implementation Repository Server Object
      ServerObject_i *server_object_;
    };

    ACE_STATIC_SVC_DECLARE (ImR_Client_Adapter_Impl)
    ACE_FACTORY_DECLARE (TAO_IMR_Client, ImR_Client_Adapter_Impl)

    #if defined(ACE_HAS_BROKEN_STATIC_CONSTRUCTORS)

    typedef int (*TAO_Module_Initializer) (void);

    static TAO_Module_Initializer
    TAO_Requires_ImR_Client_Initializer =
      &TAO::ImR_Client::ImR_Client_Adapter_Impl::Initializer;

    #else

    static int
    TAO_Requires_ImR_Client_Initializer =
      TAO::ImR_Client::ImR_Client_Adapter_Impl::Initializer ();

    #endif /* ACE_HAS_BROKEN_STATIC_CONSTRUCTORS */

  }
}

#include /**/ "ace/post.h"

#endif /* TAO_IMR_CLIENT_ADAPTER_IMPL_H */
