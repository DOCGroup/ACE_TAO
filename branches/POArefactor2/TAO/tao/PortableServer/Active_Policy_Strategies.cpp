// @(#) $Id$

#include "Active_Policy_Strategies.h"
#include "POA_Cached_Policies.h"
#include "IdAssignmentPolicyC.h"
#include "IdUniquenessPolicyC.h"
#include "ImplicitActivationPolicyC.h"
#include "LifespanPolicyC.h"
#include "RequestProcessingPolicyC.h"
#include "ServantRetentionPolicyC.h"
#include "ThreadPolicyC.h"

#include "Thread_Strategy.h"
#include "Request_Processing_Strategy.h"
#include "Id_Assignment_Strategy.h"
#include "Lifespan_Strategy.h"
#include "Id_Uniqueness_Strategy.h"
#include "Activation_Strategy.h"

ACE_RCSID(PortableServer,
          Active_Policy_Strategies,
          "$Id$")

namespace TAO
{
  namespace Portable_Server
  {
    Active_Policy_Strategies::Active_Policy_Strategies() :
      thread_strategy_ (0),
      request_processing_strategy_ (0),
      id_assignment_strategy_ (0),
      lifespan_strategy_ (0),
      id_uniqueness_strategy_ (0),
      activation_strategy_ (0)
    {
    }

    void
    Active_Policy_Strategies::update (Cached_Policies &policies
                                      ACE_ENV_ARG_DECL)
    {
      // This has to changed into having a factory that checks this and loads the correct strategy
      if (policies.thread() == ::PortableServer::ORB_CTRL_MODEL)
      {
        ACE_NEW (thread_strategy_, ORBControl_Thread_Strategy);
      }
      else
      {
        ACE_NEW (thread_strategy_, Single_Thread_Strategy);
      }

      switch (policies.request_processing())
      {
        case ::PortableServer::USE_ACTIVE_OBJECT_MAP_ONLY :
        {
          ACE_NEW (request_processing_strategy_, AOM_Only_Request_Processing_Strategy);
          break;
        }
        case ::PortableServer::USE_DEFAULT_SERVANT :
        {
          ACE_NEW (request_processing_strategy_, Default_Servant_Request_Processing_Strategy);
          break;
        }
        case ::PortableServer::USE_SERVANT_MANAGER :
        {
          switch (policies.servant_retention())
          {
            case ::PortableServer::RETAIN :
            {
              ACE_NEW (request_processing_strategy_, Servant_Activator_Request_Processing_Strategy);
              break;
            }
            case ::PortableServer::NON_RETAIN :
            {
              ACE_NEW (request_processing_strategy_, Servant_Locator_Request_Processing_Strategy);
              break;
            }
          }
          break;
        }
      }

      switch (policies.id_assignment())
      {
        case ::PortableServer::USER_ID :
        {
          ACE_NEW (id_assignment_strategy_, User_Id_Assignment_Strategy);
          break;
        }
        case ::PortableServer::SYSTEM_ID :
        {
          ACE_NEW (id_assignment_strategy_, System_Id_Assignment_Strategy);
          break;
        }
      }

      switch (policies.id_uniqueness())
      {
        case ::PortableServer::UNIQUE_ID :
        {
          ACE_NEW (id_uniqueness_strategy_, Unique_Id_Uniqueness_Strategy);
          break;
        }
        case ::PortableServer::MULTIPLE_ID :
        {
          ACE_NEW (id_uniqueness_strategy_, Multiple_Id_Uniqueness_Strategy);
          break;
        }
      }

      switch (policies.lifespan())
      {
        case ::PortableServer::TRANSIENT :
        {
          ACE_NEW (lifespan_strategy_, Transient_Lifespan_Strategy);
          break;
        }
        case ::PortableServer::PERSISTENT :
        {
          ACE_NEW (lifespan_strategy_, Persistent_Lifespan_Strategy);
          break;
        }
      }

      switch (policies.implicit_activation())
      {
        case ::PortableServer::IMPLICIT_ACTIVATION :
        {
          ACE_NEW (activation_strategy_, Implicit_Activation_Strategy);
          break;
        }
        case ::PortableServer::NO_IMPLICIT_ACTIVATION :
        {
          ACE_NEW (activation_strategy_, Explicit_Activation_Strategy);
          break;
        }
      }
    }

    Thread_Strategy*
    Active_Policy_Strategies::thread_strategy (void) const
    {
      return thread_strategy_;
    }

    Request_Processing_Strategy*
    Active_Policy_Strategies::request_processing_strategy (void) const
    {
      return request_processing_strategy_;
    }

    Id_Assignment_Strategy *
    Active_Policy_Strategies::id_assignment_strategy (void) const
    {
      return id_assignment_strategy_;
    }

    Id_Uniqueness_Strategy *
    Active_Policy_Strategies::id_uniqueness_strategy (void) const
    {
      return id_uniqueness_strategy_;
    }

    Lifespan_Strategy*
    Active_Policy_Strategies::lifespan_strategy (void) const
    {
      return lifespan_strategy_;
    }

    Activation_Strategy*
    Active_Policy_Strategies::activation_strategy (void) const
    {
      return activation_strategy_;
    }
  }
}
