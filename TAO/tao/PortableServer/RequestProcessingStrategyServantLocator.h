// -*- C++ -*-

//=============================================================================
/**
 *  @file RequestProcessingStrategyServantLocator.h
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_REQUESTPROCESSINGSTRATEGYSERVANTLOCATOR_H
#define TAO_REQUESTPROCESSINGSTRATEGYSERVANTLOCATOR_H
#include /**/ "ace/pre.h"

#include "tao/PortableServer/RequestProcessingStrategyServantManager.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)

#include "tao/PortableServer/Servant_Location.h"
#include "tao/PortableServer/PortableServer.h"
#include "tao/PortableServer/ServantLocatorC.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    class RequestProcessingStrategyServantLocator
      : public RequestProcessingStrategyServantManager
    {
    public:
      RequestProcessingStrategyServantLocator () = default;

      void strategy_cleanup() override;

      PortableServer::ServantManager_ptr get_servant_manager () override;

      void set_servant_manager (PortableServer::ServantManager_ptr imgr) override;

      TAO_Servant_Location locate_servant (
        const PortableServer::ObjectId &system_id,
        PortableServer::Servant &servant) override;

      void post_invoke_servant_cleanup(
        const PortableServer::ObjectId &system_id,
        const TAO::Portable_Server::Servant_Upcall &servant_upcall) override;

      PortableServer::Servant locate_servant (
        const char *operation,
        const PortableServer::ObjectId &system_id,
        TAO::Portable_Server::Servant_Upcall &servant_upcall,
        TAO::Portable_Server::POA_Current_Impl &poa_current_impl,
        bool &wait_occurred_restart_call) override;

      void cleanup_servant (
        PortableServer::Servant servant,
        const PortableServer::ObjectId &user_id) override;

      void etherealize_objects (CORBA::Boolean etherealize_objects) override;

    private:
      PortableServer::ServantLocator_var servant_locator_;
    };
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_MINIMUM_POA == 0 */

#include /**/ "ace/post.h"
#endif /* TAO_REQUESTPROCESSINGSTRATEGYSERVANTLOCATOR_H */
