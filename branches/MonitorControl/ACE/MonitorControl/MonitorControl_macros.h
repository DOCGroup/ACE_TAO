// -*- C++ -*-

//=============================================================================
/**
 * @file MonitorControl_macros.h
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

#include "MonitorControl/LowLevelMonitors_T.h"
#include "MonitorControl/AutoUpdateStarter.h"

#if defined (ENABLE_MONITORS)

typedef ACE_VERSIONED_NAMESPACE_NAME::ENABLED_CPU_LOAD_MONITOR
  CPU_LOAD_MONITOR;
typedef ACE_VERSIONED_NAMESPACE_NAME::ENABLED_MEMORY_USAGE_MONITOR
  MEMORY_USAGE_MONITOR;
typedef ACE_VERSIONED_NAMESPACE_NAME::ENABLED_NUM_THREADS_MONITOR
  NUM_THREADS_MONITOR;
typedef ACE_VERSIONED_NAMESPACE_NAME::ENABLED_BYTES_SENT_MONITOR
  BYTES_SENT_MONITOR;
typedef ACE_VERSIONED_NAMESPACE_NAME::ENABLED_PACKETS_SENT_MONITOR
  PACKETS_SENT_MONITOR;
typedef ACE_VERSIONED_NAMESPACE_NAME::ENABLED_BYTES_RECEIVED_MONITOR
  BYTES_RECEIVED_MONITOR;
typedef ACE_VERSIONED_NAMESPACE_NAME::ENABLED_PACKETS_RECEIVED_MONITOR
  PACKETS_RECEIVED_MONITOR;

#else /* !defined (ENABLE_MONITORS) */

typedef ACE_VERSIONED_NAMESPACE_NAME::DISABLED_CPU_LOAD_MONITOR
  CPU_LOAD_MONITOR;
typedef ACE_VERSIONED_NAMESPACE_NAME::DISABLED_MEMORY_USAGE_MONITOR
  MEMORY_USAGE_MONITOR;
typedef ACE_VERSIONED_NAMESPACE_NAME::DISABLED_NUM_THREADS_MONITOR
  NUM_THREADS_MONITOR;
typedef ACE_VERSIONED_NAMESPACE_NAME::DISABLED_BYTES_SENT_MONITOR
  BYTES_SENT_MONITOR;
typedef ACE_VERSIONED_NAMESPACE_NAME::DISABLED_PACKETS_SENT_MONITOR
  PACKETS_SENT_MONITOR;
typedef ACE_VERSIONED_NAMESPACE_NAME::DISABLED_BYTES_RECEIVED_MONITOR
  BYTES_RECEIVED_MONITOR;
typedef ACE_VERSIONED_NAMESPACE_NAME::DISABLED_PACKETS_RECEIVED_MONITOR
  PACKETS_RECEIVED_MONITOR;

#endif /* defined (ENABLE_MONITORS) */

#define ADD_MANUAL_MONITOR(TYPE) \
{ \
  ACE_VERSIONED_NAMESPACE_NAME::MC_ADMINMANAGER *mgr = \
    ACE_VERSIONED_NAMESPACE_NAME::ACE_Dynamic_Service< \
      ACE_VERSIONED_NAMESPACE_NAME::MC_ADMINMANAGER>::instance ( \
        "MC_ADMINMANAGER"); \
  TYPE* mp = \
    ACE_VERSIONED_NAMESPACE_NAME::ACE_Singleton< \
      TYPE, \
      ACE_VERSIONED_NAMESPACE_NAME::ACE_Null_Mutex>::instance (); \
  bool good_add = \
    mgr->admin ().monitor_point (mp, 0); \
  if (!good_add) \
    { \
      ACE_ERROR ((LM_ERROR, \
                  "Monitor %s already registered.\n", \
                  mp->name ())); \
    } \
}

#define ADD_PERIODIC_MONITOR(TYPE,INTERVAL_MSEC) \
{ \
  ACE_VERSIONED_NAMESPACE_NAME::MC_ADMINMANAGER *mgr = \
    ACE_VERSIONED_NAMESPACE_NAME::ACE_Dynamic_Service< \
      ACE_VERSIONED_NAMESPACE_NAME::MC_ADMINMANAGER>::instance ( \
        "MC_ADMINMANAGER"); \
  TYPE* mp = \
    ACE_VERSIONED_NAMESPACE_NAME::ACE_Singleton< \
      TYPE, \
      ACE_VERSIONED_NAMESPACE_NAME::ACE_Null_Mutex>::instance (); \
  bool good_add = \
    mgr->admin ().monitor_point (mp, INTERVAL_MSEC); \
  if (!good_add) \
    { \
      ACE_ERROR ((LM_ERROR, \
                  "Monitor %s already registered.\n", \
                  mp->name ())); \
    } \
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
