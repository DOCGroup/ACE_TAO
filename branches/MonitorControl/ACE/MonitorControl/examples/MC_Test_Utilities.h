// -*- C++ -*-

//=============================================================================
/**
 * @file MC_Test_Utilities.h
 *
 * $Id$
 *
 * @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================

#ifndef MC_TEST_UTILITIES_H
#define MC_TEST_UTILITIES_H

#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/MonitorControl_Types.h"

#include "MonitorControl/examples/MC_Test_Utilities_export.h"

using namespace ACE_VERSIONED_NAMESPACE_NAME::ACE::MonitorControl;

struct MC_TEST_UTILITIES_Export MC_Test_Utilities
{
  static void display_timestamp (const MonitorControl_Types::Data &data);
  static void display_num_threads (const MonitorControl_Types::Data &data);
  static void display_mq_size (const MonitorControl_Types::Data &data);
  static void display_memory_usage (const MonitorControl_Types::Data &data);
  static void display_cpu_load (const MonitorControl_Types::Data &data);
  static void display_bytes_sent (const MonitorControl_Types::Data &data);
  static void display_bytes_received (const MonitorControl_Types::Data &data);
  static void display_packets_sent (const MonitorControl_Types::Data &data);
  static void display_packets_received (const MonitorControl_Types::Data &data);
};

#include /**/ "ace/post.h"

#endif /* MC_TEST_UTILITIES_H */
