// $Id$

#include "ace/Reactor.h"

#include "MonitorControl/AutoUpdateStarter.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    int 
    AutoUpdateStarter::svc (void)
    {
      /// We want the thread in which the event loop is started to
      /// own the reactor, otherwise the handle_timeout() calls 
      /// aren't triggerd.
      ACE_Reactor::instance ()->owner (ACE_Thread::self ());
      
      return ACE_Reactor::instance ()->run_reactor_event_loop ();
    }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

