// -*- C++ -*-

//=============================================================================
/**
 *  @file    Request_Processing_Strategy.cpp
 *
 *  $Id$
 *
 */
//=============================================================================

#include "tao/PortableServer/RequestProcessingStrategyServantManager.h"
#include "tao/PortableServer/ServantManagerC.h"
#include "tao/ORB_Constants.h"

ACE_RCSID (PortableServer,
           Request_Processing,
           "$Id$")

#if (TAO_HAS_MINIMUM_POA == 0)

namespace TAO
{
  namespace Portable_Server
  {
    Servant_Manager_Request_Processing_Strategy::~Servant_Manager_Request_Processing_Strategy (void)
    {
    }

    PortableServer::Servant
    Servant_Manager_Request_Processing_Strategy::get_servant (
      ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::WrongPolicy))
    {
      ACE_THROW_RETURN (PortableServer::POA::WrongPolicy (),
                        0);
    }

    void
    Servant_Manager_Request_Processing_Strategy::set_servant (
      PortableServer::Servant servant
      ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::WrongPolicy))
    {
      ACE_UNUSED_ARG (servant);

      ACE_THROW (PortableServer::POA::WrongPolicy ());
    }

    void
    Servant_Manager_Request_Processing_Strategy::validate_servant_manager (
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
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

