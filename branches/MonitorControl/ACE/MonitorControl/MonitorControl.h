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

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/MC_Generic_Registry.h"

#include "MonitorControl/MonitorControl_macros.h"
#include "MonitorControl/CPULoadMonitor.h"
#include "MonitorControl/MemoryUsageMonitor.h"
#include "MonitorControl/NumThreadsMonitor.h"
#include "MonitorControl/BytesSentMonitor.h"
#include "MonitorControl/PacketsSentMonitor.h"
#include "MonitorControl/BytesReceivedMonitor.h"
#include "MonitorControl/PacketsReceivedMonitor.h"
#include "MonitorControl/MonitorGroup_T.h"
#include "MonitorControl/MonitorQuery.h"

static int
ACE_Requires_MC_Initializer = MC_ADMINMANAGER::Initializer ();

#include /**/ "ace/post.h"

#endif // MONITORCONTROL_H
