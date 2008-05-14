// -*- C++ -*-

//=============================================================================
/**
 * @file MonitorControl_utils.h
 *
 * $Id$
 *
 * @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================

#ifndef MONITORCONTROL_MACROS_H
#define MONITORCONTROL_MACROS_H

#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (ACE_HAS_MONITOR_FRAMEWORK) && (ACE_HAS_MONITOR_FRAMEWORK == 1)

#include "ace/Service_Config.h"
#include "ace/Dynamic_Service.h"
#include "ace/Reactor.h"

#include "ace/MonitorControl/AutoUpdateStarter.h"

typedef ACE_VERSIONED_NAMESPACE_NAME::ACE::Monitor_Control::CPULoadMonitor
  CPU_LOAD_MONITOR;
typedef ACE_VERSIONED_NAMESPACE_NAME::ACE::Monitor_Control::MemoryUsageMonitor
  MEMORY_USAGE_MONITOR;
typedef ACE_VERSIONED_NAMESPACE_NAME::ACE::Monitor_Control::NumThreadsMonitor
  NUM_THREADS_MONITOR;
typedef ACE_VERSIONED_NAMESPACE_NAME::ACE::Monitor_Control::BytesSentMonitor
  BYTES_SENT_MONITOR;
typedef ACE_VERSIONED_NAMESPACE_NAME::ACE::Monitor_Control::PacketsSentMonitor
  PACKETS_SENT_MONITOR;
typedef ACE_VERSIONED_NAMESPACE_NAME::ACE::Monitor_Control::BytesReceivedMonitor
  BYTES_RECEIVED_MONITOR;
typedef ACE_VERSIONED_NAMESPACE_NAME::ACE::Monitor_Control::PacketsReceivedMonitor
  PACKETS_RECEIVED_MONITOR;

using namespace ACE_VERSIONED_NAMESPACE_NAME::ACE::Monitor_Control;

template<typename OS_MP_TYPE>
Monitor_Base*
create_os_monitor (const char* name = 0,
                   ACE_Time_Value period = ACE_Time_Value::zero)
{
  Monitor_Base* retval = 0;

  ACE_VERSIONED_NAMESPACE_NAME::MC_ADMINMANAGER *mgr =
    ACE_VERSIONED_NAMESPACE_NAME::ACE_Dynamic_Service<
      ACE_VERSIONED_NAMESPACE_NAME::MC_ADMINMANAGER>::instance (
        "MC_ADMINMANAGER");

  const char* working_name =
    (name == 0 ? OS_MP_TYPE::default_name () : name);

  retval = mgr->admin ().monitor_point (working_name);

  if (retval == 0)
    {
      ACE_NEW_RETURN (retval, OS_MP_TYPE (working_name), 0);
      (void) mgr->admin ().monitor_point (retval, period);
    }

  return retval;
}

#define START_PERIODIC_MONITORS \
  ACE_VERSIONED_NAMESPACE_NAME::STARTER starter; \
  starter.activate ()

#define STOP_PERIODIC_MONITORS \
  ACE_VERSIONED_NAMESPACE_NAME::ACE_Dynamic_Service< \
    ACE_VERSIONED_NAMESPACE_NAME::MC_ADMINMANAGER>::instance ( \
      "MC_ADMINMANAGER")->admin ().reactor ()->end_reactor_event_loop (); \
  ACE_VERSIONED_NAMESPACE_NAME::ACE_Thread_Manager::instance ()->wait ()

#define ADD_PERIODIC_QUERY(HANDLER,QUERY,INTERVAL_SEC) \
  ACE_VERSIONED_NAMESPACE_NAME::ACE_Dynamic_Service< \
    ACE_VERSIONED_NAMESPACE_NAME::MC_ADMINMANAGER>::instance ( \
      "MC_ADMINMANAGER")->admin ().auto_query (HANDLER, QUERY, INTERVAL_SEC)

#endif /* ACE_HAS_MONITOR_FRAMEWORK==1 */

#include /**/ "ace/post.h"

#endif // MONITORCONTROL_MACROS_H
