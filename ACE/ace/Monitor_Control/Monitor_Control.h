// -*- C++ -*-

//=============================================================================
/**
 * @file Monitor_Control.h
 *
 * @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================

#ifndef MONITOR_CONTROL_H
#define MONITOR_CONTROL_H

#include /**/ "ace/pre.h"

#include "ace/Monitor_Point_Registry.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (ACE_HAS_MONITOR_FRAMEWORK) && (ACE_HAS_MONITOR_FRAMEWORK == 1)

#include "ace/Monitor_Admin_Manager.h"

#include "CPU_Load_Monitor.h"
#include "Memory_Usage_Monitor.h"
#include "Num_Threads_Monitor.h"
#include "Bytes_Sent_Monitor.h"
#include "Packets_Sent_Monitor.h"
#include "Bytes_Received_Monitor.h"
#include "Packets_Received_Monitor.h"
#include "Monitor_Group.h"
#include "Monitor_Query.h"
#include "Monitor_Control_utils.h"

static int
ACE_Requires_MC_Initializer = MC_ADMINMANAGER::Initializer ();

#endif /* ACE_HAS_MONITOR_FRAMEWORK==1 */

#include /**/ "ace/post.h"

#endif // MONITOR_CONTROL_H
