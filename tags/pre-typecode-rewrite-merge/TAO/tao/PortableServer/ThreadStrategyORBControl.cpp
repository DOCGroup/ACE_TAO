// $Id$

#include "ThreadStrategyORBControl.h"

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

    ACE_FACTORY_DEFINE (TAO_PortableServer, ThreadStrategyORBControl)

    ACE_STATIC_SVC_DEFINE (
        ThreadStrategyORBControl,
        ACE_TEXT ("ThreadStrategyORBControl"),
        ACE_SVC_OBJ_T,
        &ACE_SVC_NAME (ThreadStrategyORBControl),
        ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
        0
      )

    #if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
    template class ACE_Dynamic_Service<ThreadStrategyORBControl>;
    #elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
    #pragma instantiate ACE_Dynamic_Service<ThreadStrategyORBControl>
    #endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
  }
}

