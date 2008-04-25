// $Id$

#include "ace/Reactor.h"
#include "ace/Dynamic_Service.h"
#include "ace/Monitor_Admin_Manager.h"

#include "MonitorControl/AutoUpdateStarter.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    int
    AutoUpdateStarter::svc (void)
    {
      MC_ADMINMANAGER* mgr =
        ACE_Dynamic_Service<MC_ADMINMANAGER>::instance ("MC_ADMINMANAGER");

      /// We want the thread in which the event loop is started to
      /// own the reactor, otherwise the handle_timeout() calls
      /// aren't triggerd.
      mgr->admin ().reactor ()->owner (ACE_Thread::self ());

      return mgr->admin ().reactor ()->run_reactor_event_loop ();
    }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

