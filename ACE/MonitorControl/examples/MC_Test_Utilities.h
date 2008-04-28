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

#include "ace/Monitor_Control_Types.h"

#include "MonitorControl/examples/MC_Test_Utilities_export.h"

using namespace ACE_VERSIONED_NAMESPACE_NAME::ACE::MonitorControl;

/**
 * @struct MC_Test_Utilities
 *
 * @brief Container for utility methods that display the output
 *        of the various OS monitors in a friendly format.
 *
 */
struct MC_TEST_UTILITIES_Export MC_Test_Utilities
{
  static void display_timestamp (const Monitor_Control_Types::Data &data);
  static void display_num_threads (const Monitor_Control_Types::Data &data);
  static void display_mq_size (const Monitor_Control_Types::Data &data);
  static void display_memory_usage (const Monitor_Control_Types::Data &data);
  static void display_cpu_load (const Monitor_Control_Types::Data &data);
  static void display_bytes_sent (const Monitor_Control_Types::Data &data);
  static void display_bytes_received (const Monitor_Control_Types::Data &data);
  static void display_packets_sent (const Monitor_Control_Types::Data &data);
  static void display_packets_received (const Monitor_Control_Types::Data &data);
};

#include /**/ "ace/post.h"

#endif /* MC_TEST_UTILITIES_H */
