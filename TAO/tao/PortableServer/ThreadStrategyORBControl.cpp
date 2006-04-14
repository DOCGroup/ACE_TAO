// $Id$

#include "ThreadStrategyORBControl.h"
#include "ace/Log_Msg.h"

ACE_RCSID (PortableServer,
           ThreadStrategyORBControl,
           "$Id$")

namespace TAO
{
  namespace Portable_Server
  {
    int
    ThreadStrategyORBControl::enter ()
    {
      return 0;
    }

    int
    ThreadStrategyORBControl::exit ()
    {
      return 0;
    }

    ::PortableServer::ThreadPolicyValue
    ThreadStrategyORBControl::type() const
    {
      return ::PortableServer::ORB_CTRL_MODEL;
    }

    ACE_FACTORY_DEFINE (ACE_Local_Service, ThreadStrategyORBControl)

    ACE_STATIC_SVC_DEFINE (
        ThreadStrategyORBControl,
        ACE_TEXT ("ThreadStrategyORBControl"),
        ACE_SVC_OBJ_T,
        &ACE_SVC_NAME (ThreadStrategyORBControl),
        ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
        0
      )
  }
}

