// -*- C++ -*-

//=============================================================================
/**
 *  @file    Request_Processing_Strategy.cpp
 *
 *  $Id$
 *
 */
//=============================================================================

#include "tao/Server_Strategy_Factory.h"
#include "tao/ORB_Core.h"
#include "tao/TSS_Resources.h"

#include "tao/PortableServer/ServantActivatorC.h"
#include "tao/PortableServer/ServantLocatorC.h"

#include "tao/PortableServer/RequestProcessingStrategyServantManager.h"
#include "tao/PortableServer/ServantRetentionStrategy.h"
#include "tao/PortableServer/LifespanStrategy.h"
#include "tao/PortableServer/Non_Servant_Upcall.h"
#include "tao/PortableServer/POA.h"
#include "tao/PortableServer/POA_Current_Impl.h"
#include "tao/PortableServer/Servant_Upcall.h"

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
    Servant_Manager_Request_Processing_Strategy::get_servant (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableServer::POA::WrongPolicy))
    {
      ACE_THROW_RETURN (PortableServer::POA::WrongPolicy (),
                        0);
    }

    void
    Servant_Manager_Request_Processing_Strategy::set_servant (PortableServer::Servant servant
                 ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableServer::POA::WrongPolicy))
    {
      ACE_UNUSED_ARG (servant);

      ACE_THROW (PortableServer::POA::WrongPolicy ());
    }

  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

