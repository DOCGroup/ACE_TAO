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

ACE_RCSID(PortableServer,
          Active_Policy_Strategies,
          "$Id$")

namespace TAO
{
  namespace Portable_Server
  {
    Active_Policy_Strategies::Active_Policy_Strategies() :
      thread_strategy_(0)
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
    }

    Thread_Strategy*
    Active_Policy_Strategies::thread_strategy (void) const
    {
      return thread_strategy_;
    }
  }
}
