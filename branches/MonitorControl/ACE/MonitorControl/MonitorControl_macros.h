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
#include "ace/Singleton.h"
#include "ace/Null_Mutex.h"

#include "MonitorControl/LowLevelMonitors_T.h"

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

#endif /* defined (ENABLE_MONITORS) */

#define ADD_MONITOR(TYPE) \
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
    mgr->admin ().add_monitor_point (mp); \
  if (!good_add) \
    { \
      ACE_ERROR ((LM_ERROR, \
                  "Monitor %s already registered.\n", \
                  mp->name ())); \
    } \
}

#include /**/ "ace/post.h"

#endif // MONITORCONTROL_MACROS_H
