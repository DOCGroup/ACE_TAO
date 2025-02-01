// -*- C++ -*-

//=============================================================================
/**
 *  @file RequestProcessingStrategyAOMOnly.h
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_REQUESTPROCESSINGSTRATEGYAOMONLY_H
#define TAO_REQUESTPROCESSINGSTRATEGYAOMONLY_H
#include /**/ "ace/pre.h"

#include "tao/PortableServer/RequestProcessingStrategy.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/Servant_Location.h"
#include "tao/PortableServer/PortableServer.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    /**
     * @class RequestProcessingStrategyAOMOnly
     *
     * Request Processing Strategy which only uses the Active Object Map (AOM)
     */
    class RequestProcessingStrategyAOMOnly : public RequestProcessingStrategy
    {
    public:
      RequestProcessingStrategyAOMOnly () = default;

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)
      PortableServer::ServantManager_ptr get_servant_manager () override;

      void set_servant_manager (PortableServer::ServantManager_ptr imgr) override;

      void set_servant (PortableServer::Servant servant) override;

      PortableServer::Servant get_servant () override;
#endif /* TAO_HAS_MINIMUM_POA == 0 !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO) */

      TAO_Servant_Location
      locate_servant (const PortableServer::ObjectId &system_id,
                      PortableServer::Servant &servant) override;

      PortableServer::Servant
      locate_servant (const char *operation,
                      const PortableServer::ObjectId &system_id,
                      TAO::Portable_Server::Servant_Upcall &servant_upcall,
                      TAO::Portable_Server::POA_Current_Impl &poa_current_impl,
                      bool &wait_occurred_restart_call) override;

      PortableServer::Servant system_id_to_servant (
        const PortableServer::ObjectId &system_id) override;

      PortableServer::Servant id_to_servant (
        const PortableServer::ObjectId &id) override;

      void cleanup_servant (
        PortableServer::Servant servant,
        const PortableServer::ObjectId &user_id) override;

      void etherealize_objects (CORBA::Boolean etherealize_objects) override;

      PortableServer::ObjectId *servant_to_id (PortableServer::Servant servant) override;

      void post_invoke_servant_cleanup(
        const PortableServer::ObjectId &system_id,
        const TAO::Portable_Server::Servant_Upcall &servant_upcall) override;
    };
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_REQUESTPROCESSINGSTRATEGYAOMONLY_H */
