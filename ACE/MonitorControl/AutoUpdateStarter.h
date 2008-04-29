// -*- C++ -*-

//=============================================================================
/**
 * @file AutoUpdateStarter.h
 *
 * $Id$
 *
 * @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================

#ifndef AUTOUPDATESTARTER_H
#define AUTOUPDATESTARTER_H

#include /**/ "ace/pre.h"

#include "ace/Task.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (ACE_HAS_MONITOR_FRAMEWORK) && (ACE_HAS_MONITOR_FRAMEWORK == 1)

#include "MonitorControl/MonitorControl_export.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    /**
     * @class AutoUpdateStarter
     *
     * @brief Starts reactor event loop that handles registered timers.
     *
     * For all the monitors that are periodically updated, the reactor
     * event loop that calls their associated registered timers must
     * be started in a separate thread so the rest of the application
     * isn't blocked. A very simple class inheriting from ACE_Task_Base
     * and overriding the svc() method suffices.
     */
    class MONITORCONTROL_Export AutoUpdateStarter : public ACE_Task_Base
    {
      public:
        int svc (void);
    };
  }
}

typedef ACE::MonitorControl::AutoUpdateStarter STARTER;

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* ACE_HAS_MONITOR_FRAMEWORK==1 */

#include /**/ "ace/post.h"

#endif // AUTOUPDATESTARTER_H
