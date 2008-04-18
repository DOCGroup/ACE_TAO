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

#if defined (ACE_ENABLE_MONITORS)

#if defined (ACE_WIN32)
#include "MonitorControl/WindowsMultiInstanceMonitor.h"
#elif defined (linux)
#include "MonitorControl/LinuxNetworkInterfaceMonitor.h"
#endif

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    /**
     * @class PacketsSentMonitor
     *
     * @brief Monitors total packets sent over all network interfaces.
     */
    class MONITORCONTROL_Export PacketsSentMonitor
      : public Monitor_Base
#if defined (ACE_WIN32)
        , public WindowsMultiInstanceMonitor
#elif defined (linux)
        , public LinuxNetworkInterfaceMonitor
#endif
    {
    public:
      PacketsSentMonitor (void);

      /// Implementation of the pure virtual method.
      virtual void update (void);
    };
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* ACE_ENABLE_MONITORS */

#include /**/ "ace/post.h"

#endif // PACKETSSENTMONITOR_H
