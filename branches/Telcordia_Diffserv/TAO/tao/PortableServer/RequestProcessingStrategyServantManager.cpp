// -*- C++ -*-

#include "tao/PortableServer/RequestProcessingStrategyServantManager.h"
#include "tao/PortableServer/ServantManagerC.h"
#include "tao/PortableServer/Root_POA.h"
#include "tao/ORB_Constants.h"

ACE_RCSID (PortableServer,
           Request_Processing,
           "$Id$")

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    RequestProcessingStrategyServantManager::RequestProcessingStrategyServantManager (void)
    {
    }

    PortableServer::Servant
    RequestProcessingStrategyServantManager::get_servant (
      ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::WrongPolicy))
    {
      ACE_THROW_RETURN (PortableServer::POA::WrongPolicy (),
                        0);
    }

    void
    RequestProcessingStrategyServantManager::set_servant (
      PortableServer::Servant servant
      ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::WrongPolicy))
    {
      ACE_UNUSED_ARG (servant);

      ACE_THROW (PortableServer::POA::WrongPolicy ());
    }

    void
    RequestProcessingStrategyServantManager::validate_servant_manager (
      PortableServer::ServantManager_ptr servant_manager
      ACE_ENV_ARG_DECL)
    {
      // When no servant manager is set, give an exception with minor code 4,
      // see 11.3.8.6 of the corba spec
      if (CORBA::is_nil (servant_manager))
        {
          ACE_THROW (CORBA::OBJ_ADAPTER (CORBA::OMGVMCID | 4,
                                         CORBA::COMPLETED_NO));
        }
    }

    PortableServer::Servant
    RequestProcessingStrategyServantManager::system_id_to_servant (
      const PortableServer::ObjectId &system_id
      ACE_ENV_ARG_DECL)
    {
      return this->poa_->find_servant (system_id
                                       ACE_ENV_ARG_PARAMETER);
    }

    PortableServer::ObjectId *
    RequestProcessingStrategyServantManager::servant_to_id (
      PortableServer::Servant servant
      ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::ServantNotActive,
                         PortableServer::POA::WrongPolicy))
    {
      return this->poa_->servant_to_user_id (servant ACE_ENV_ARG_PARAMETER);
    }

    PortableServer::Servant
    RequestProcessingStrategyServantManager::id_to_servant (
      const PortableServer::ObjectId &id
      ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::ObjectNotActive,
                         PortableServer::POA::WrongPolicy))
    {
      return this->poa_->user_id_to_servant_i (id ACE_ENV_ARG_PARAMETER);
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

