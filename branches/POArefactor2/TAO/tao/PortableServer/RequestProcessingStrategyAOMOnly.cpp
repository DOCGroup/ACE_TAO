// -*- C++ -*-

//=============================================================================
/**
 *  @file    RequestProcessingStrategy.cpp
 *
 *  $Id$
 *
 */
//=============================================================================

#include "tao/PortableServer/RequestProcessingStrategyAOMOnly.h"
#include "tao/PortableServer/ServantManagerC.h"

ACE_RCSID (PortableServer,
           Request_Processing,
           "$Id$")

#if (TAO_HAS_MINIMUM_POA == 0)

namespace TAO
{
  namespace Portable_Server
  {
    AOM_Only_Request_Processing_Strategy::AOM_Only_Request_Processing_Strategy()
    {
    }

    AOM_Only_Request_Processing_Strategy::~AOM_Only_Request_Processing_Strategy()
    {
    }

    void
    AOM_Only_Request_Processing_Strategy::strategy_init(TAO_POA *poa)
    {
      RequestProcessingStrategy::strategy_init (poa);
    }

    PortableServer::ServantManager_ptr
    AOM_Only_Request_Processing_Strategy::get_servant_manager (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableServer::POA::WrongPolicy))
    {
      ACE_THROW_RETURN (PortableServer::POA::WrongPolicy (),
                        PortableServer::ServantManager::_nil ());
    }

    void
    AOM_Only_Request_Processing_Strategy::set_servant_manager (PortableServer::ServantManager_ptr imgr
                         ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableServer::POA::WrongPolicy))
    {
      ACE_UNUSED_ARG (imgr);

      ACE_THROW (PortableServer::POA::WrongPolicy ());
    }

    PortableServer::Servant
    AOM_Only_Request_Processing_Strategy::get_servant (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableServer::POA::WrongPolicy))
    {
      ACE_THROW_RETURN (PortableServer::POA::WrongPolicy (),
                        0);
    }

    void
    AOM_Only_Request_Processing_Strategy::set_servant (PortableServer::Servant servant
                 ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableServer::POA::WrongPolicy))
    {
      ACE_UNUSED_ARG (servant);

      ACE_THROW (PortableServer::POA::WrongPolicy ());
    }

    TAO_SERVANT_LOCATION
    AOM_Only_Request_Processing_Strategy::locate_servant (
      const PortableServer::ObjectId &system_id,
      PortableServer::Servant &servant
      ACE_ENV_ARG_DECL_NOT_USED)
    {
      ACE_UNUSED_ARG (system_id);
      ACE_UNUSED_ARG (servant);

      return TAO_SERVANT_NOT_FOUND;
    }

    PortableServer::Servant
    AOM_Only_Request_Processing_Strategy::locate_servant (
      const char *operation,
      const PortableServer::ObjectId &system_id,
      TAO::Portable_Server::Servant_Upcall &servant_upcall,
      TAO::Portable_Server::POA_Current_Impl &poa_current_impl,
      int &wait_occurred_restart_call
      ACE_ENV_ARG_DECL)
    {
      ACE_UNUSED_ARG (operation);
      ACE_UNUSED_ARG (system_id);
      ACE_UNUSED_ARG (servant_upcall);
      ACE_UNUSED_ARG (poa_current_impl);
      ACE_UNUSED_ARG (wait_occurred_restart_call);

      // If the USE_ACTIVE_OBJECT_MAP_ONLY policy is in effect, the POA raises
      // the OBJECT_NOT_EXIST system exception.
      ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                        0);
    }

  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

