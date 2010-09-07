// -*- C++ -*-
// $Id$

#include "tao/orbconf.h"

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)

#include "tao/PortableServer/RequestProcessingStrategyServantManager.h"
#include "tao/PortableServer/ServantManagerC.h"
#include "tao/PortableServer/Root_POA.h"
#include "tao/ORB_Constants.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    RequestProcessingStrategyServantManager::RequestProcessingStrategyServantManager (void)
    {
    }

    PortableServer::Servant
    RequestProcessingStrategyServantManager::get_servant (void)
    {
      throw PortableServer::POA::WrongPolicy ();
    }

    void
    RequestProcessingStrategyServantManager::set_servant (
      PortableServer::Servant)
    {
      throw PortableServer::POA::WrongPolicy ();
    }

    void
    RequestProcessingStrategyServantManager::validate_servant_manager (
      PortableServer::ServantManager_ptr servant_manager)
    {
      // When no servant manager is set, give an exception with minor code 4,
      // see 11.3.8.6 of the corba spec
      if (CORBA::is_nil (servant_manager))
        {
          throw ::CORBA::OBJ_ADAPTER (CORBA::OMGVMCID | 4, CORBA::COMPLETED_NO);
        }
    }

    PortableServer::Servant
    RequestProcessingStrategyServantManager::system_id_to_servant (
      const PortableServer::ObjectId &system_id)
    {
      return this->poa_->find_servant (system_id);
    }

    PortableServer::ObjectId *
    RequestProcessingStrategyServantManager::servant_to_id (
      PortableServer::Servant servant)
    {
      return this->poa_->servant_to_user_id (servant);
    }

    PortableServer::Servant
    RequestProcessingStrategyServantManager::id_to_servant (
      const PortableServer::ObjectId &id)
    {
      return this->poa_->user_id_to_servant_i (id);
    }

    ::PortableServer::RequestProcessingPolicyValue
    RequestProcessingStrategyServantManager::type() const
    {
      return ::PortableServer::USE_SERVANT_MANAGER;
    }
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_MINIMUM_POA == 0 */

