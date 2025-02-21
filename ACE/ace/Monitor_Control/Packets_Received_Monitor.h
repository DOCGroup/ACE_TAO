// -*- C++ -*-

//=============================================================================
/**
 * @file Packets_Received_Monitor.h
 *
 * @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================

#ifndef PACKETS_RECEIVED_MONITOR_H
#define PACKETS_RECEIVED_MONITOR_H

#include /**/ "ace/pre.h"

#include "ace/Monitor_Base.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (ACE_HAS_MONITOR_FRAMEWORK) && (ACE_HAS_MONITOR_FRAMEWORK == 1)

#if defined (ACE_HAS_PDH_H) && !defined (ACE_LACKS_PDH_H)
#include "ace/Monitor_Control/Windows_Multi_Instance_Monitor.h"
#elif defined (ACE_LINUX)
#include "ace/Monitor_Control/Linux_Network_Interface_Monitor.h"
#elif defined (__FreeBSD__) || defined (__Lynx__)
#include "ace/Monitor_Control/FreeBSD_Network_Interface_Monitor.h"
#elif defined (__NetBSD__) || defined (__OpenBSD__)
#include "ace/Monitor_Control/BSD_Network_Interface_Monitor.h"
#else
#include "ace/Monitor_Control/Null_Network_Interface_Monitor.h"
#endif

#include "ace/Monitor_Control/Monitor_Control_export.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace Monitor_Control
  {
    /**
     * @class Packets_Received_Monitor
     *
     * @brief Monitors total packets received over all network interfaces.
     */
    class MONITOR_CONTROL_Export Packets_Received_Monitor
      : public Monitor_Base
#if defined (ACE_HAS_WIN32_PDH)
      , public Windows_Multi_Instance_Monitor
#elif defined (ACE_LINUX)
      , public Linux_Network_Interface_Monitor
#elif defined (__NetBSD__) || defined (__OpenBSD__)
      , public BSD_Network_Interface_Monitor
#elif defined (__FreeBSD__) || defined (__Lynx__)
      , public FreeBSD_Network_Interface_Monitor
#else
      , public Null_Network_Interface_Monitor
#endif
    {
    public:
      Packets_Received_Monitor (const char* name);

      /// Implementation of the pure virtual method.
      virtual void update ();

      /// Stores the default name, used if none is supplied by the user.
      static const char* default_name ();

    private:
      /// Overridden reset, calls platform-specific reset.
      virtual void clear_i ();

    private:
      static const char* default_name_;
    };
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* ACE_HAS_MONITOR_FRAMEWORK==1 */

#include /**/ "ace/post.h"

#endif // PACKETS_RECEIVED_MONITOR_H
