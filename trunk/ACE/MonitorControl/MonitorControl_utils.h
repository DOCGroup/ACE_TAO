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

#include "ace/Service_Config.h"
#include "ace/Dynamic_Service.h"
#include "ace/Reactor.h"

#include "MonitorControl/AutoUpdateStarter.h"

typedef ACE_VERSIONED_NAMESPACE_NAME::ACE::MonitorControl::CPULoadMonitor
  CPU_LOAD_MONITOR;
typedef ACE_VERSIONED_NAMESPACE_NAME::ACE::MonitorControl::MemoryUsageMonitor
  MEMORY_USAGE_MONITOR;
typedef ACE_VERSIONED_NAMESPACE_NAME::ACE::MonitorControl::NumThreadsMonitor
  NUM_THREADS_MONITOR;
typedef ACE_VERSIONED_NAMESPACE_NAME::ACE::MonitorControl::BytesSentMonitor
  BYTES_SENT_MONITOR;
typedef ACE_VERSIONED_NAMESPACE_NAME::ACE::MonitorControl::PacketsSentMonitor
  PACKETS_SENT_MONITOR;
typedef ACE_VERSIONED_NAMESPACE_NAME::ACE::MonitorControl::BytesReceivedMonitor
  BYTES_RECEIVED_MONITOR;
typedef ACE_VERSIONED_NAMESPACE_NAME::ACE::MonitorControl::PacketsReceivedMonitor
  PACKETS_RECEIVED_MONITOR;

using namespace ACE_VERSIONED_NAMESPACE_NAME::ACE::MonitorControl;

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

#include /**/ "ace/post.h"

#endif // MONITORCONTROL_MACROS_H
