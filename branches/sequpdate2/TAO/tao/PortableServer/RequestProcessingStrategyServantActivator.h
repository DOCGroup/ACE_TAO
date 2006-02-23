// -*- C++ -*-

//=============================================================================
/**
 *  @file RequestProcessingStrategyServantActivator.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_REQUESTPROCESSINGSTRATEGYSERVANTACTIVATOR_H
#define TAO_REQUESTPROCESSINGSTRATEGYSERVANTACTIVATOR_H
#include /**/ "ace/pre.h"

#include "RequestProcessingStrategyServantManager.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if (TAO_HAS_MINIMUM_POA == 0)

#include "Servant_Location.h"
#include "PortableServer.h"
#include "ServantActivatorC.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    class RequestProcessingStrategyServantActivator
      : public RequestProcessingStrategyServantManager
    {
    public:
      RequestProcessingStrategyServantActivator (void);

      virtual void strategy_cleanup(ACE_ENV_SINGLE_ARG_DECL);

      PortableServer::ServantManager_ptr get_servant_manager (ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::WrongPolicy));

      void set_servant_manager (PortableServer::ServantManager_ptr imgr
                                ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::WrongPolicy));

      virtual TAO_SERVANT_LOCATION locate_servant (
        const PortableServer::ObjectId &system_id,
        PortableServer::Servant &servant
        ACE_ENV_ARG_DECL);

      virtual PortableServer::Servant locate_servant (
        const char *operation,
        const PortableServer::ObjectId &system_id,
        TAO::Portable_Server::Servant_Upcall &servant_upcall,
        TAO::Portable_Server::POA_Current_Impl &poa_current_impl,
        int &wait_occurred_restart_call
        ACE_ENV_ARG_DECL);

      virtual void cleanup_servant (
        PortableServer::Servant servant,
        const PortableServer::ObjectId &user_id
        ACE_ENV_ARG_DECL);

      virtual void etherealize_objects (CORBA::Boolean etherealize_objects);

      virtual void post_invoke_servant_cleanup(
        const PortableServer::ObjectId &system_id,
        const TAO::Portable_Server::Servant_Upcall &servant_upcall);

    private:
      PortableServer::Servant incarnate_servant (
        const PortableServer::ObjectId& object_id
        ACE_ENV_ARG_DECL);

      void etherealize_servant (const PortableServer::ObjectId& object_id,
        PortableServer::Servant servant,
        CORBA::Boolean cleanup_in_progress
        ACE_ENV_ARG_DECL);

    private:
      PortableServer::ServantActivator_var servant_activator_;
      CORBA::Boolean etherealize_objects_;
    };
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_MINIMUM_POA == 0 */

#include /**/ "ace/post.h"
#endif /* TAO_REQUESTPROCESSINGSTRATEGYSERVANTACTIVATOR_H */
