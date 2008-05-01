// -*- C++ -*-

//=============================================================================
/**
 * @file MonitorControl.h
 *
 * $Id$
 *
 * @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================

#ifndef MONITORCONTROL_H
#define MONITORCONTROL_H

#include /**/ "ace/pre.h"

#include "ace/Monitor_Point_Registry.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (ACE_HAS_MONITOR_FRAMEWORK) && (ACE_HAS_MONITOR_FRAMEWORK == 1)

#include "ace/Monitor_Admin_Manager.h"

#include "ace/MonitorControl/CPULoadMonitor.h"
#include "ace/MonitorControl/MemoryUsageMonitor.h"
#include "ace/MonitorControl/NumThreadsMonitor.h"
#include "ace/MonitorControl/BytesSentMonitor.h"
#include "ace/MonitorControl/PacketsSentMonitor.h"
#include "ace/MonitorControl/BytesReceivedMonitor.h"
#include "ace/MonitorControl/PacketsReceivedMonitor.h"
#include "ace/MonitorControl/MonitorGroup.h"
#include "ace/MonitorControl/MonitorQuery.h"
#include "ace/MonitorControl/MonitorControl_utils.h"

static int
ACE_Requires_MC_Initializer = MC_ADMINMANAGER::Initializer ();

#endif /* ACE_HAS_MONITOR_FRAMEWORK==1 */

#include /**/ "ace/post.h"

#endif // MONITORCONTROL_H
