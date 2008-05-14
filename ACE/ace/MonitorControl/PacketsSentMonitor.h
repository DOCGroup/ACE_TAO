// -*- C++ -*-

//=============================================================================
/**
 * @file PacketsSentMonitor.h
 *
 * $Id$
 *
 * @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================

#ifndef PACKETSSENTMONITOR_H
#define PACKETSSENTMONITOR_H

#include /**/ "ace/pre.h"

#include "ace/Monitor_Base.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (ACE_HAS_MONITOR_FRAMEWORK) && (ACE_HAS_MONITOR_FRAMEWORK == 1)

#if defined (ACE_HAS_WIN32_PDH)
#include "ace/MonitorControl/WindowsMultiInstanceMonitor.h"
#elif defined (linux)
#include "ace/MonitorControl/LinuxNetworkInterfaceMonitor.h"
#endif

#include "ace/MonitorControl/MonitorControl_export.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace Monitor_Control
  {
    /**
     * @class PacketsSentMonitor
     *
     * @brief Monitors total packets sent over all network interfaces.
     */
    class MONITORCONTROL_Export PacketsSentMonitor
      : public Monitor_Base
#if defined (ACE_HAS_WIN32_PDH)
        , public WindowsMultiInstanceMonitor
#elif defined (linux)
        , public LinuxNetworkInterfaceMonitor
#endif
    {
    public:
      PacketsSentMonitor (const char* name);

      /// Implementation of the pure virtual method.
      virtual void update (void);

      /// Stores the default name, used if none is supplied by the user.
      static const char* default_name (void);

    private:
      static const char* default_name_;
    };
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* ACE_HAS_MONITOR_FRAMEWORK==1 */

#include /**/ "ace/post.h"

#endif // PACKETSSENTMONITOR_H
