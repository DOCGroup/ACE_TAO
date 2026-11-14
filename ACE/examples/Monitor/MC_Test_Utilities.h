// -*- C++ -*-

//=============================================================================
/**
 * @file MC_Test_Utilities.h
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

#include "MC_Test_Utilities_export.h"

#if defined (ACE_HAS_MONITOR_FRAMEWORK) && (ACE_HAS_MONITOR_FRAMEWORK == 1)

using ACE_VERSIONED_NAMESPACE_NAME::ACE::Monitor_Control::Monitor_Control_Types;

#endif /* ACE_HAS_MONITOR_FRAMEWORK==1 */

/**
 * @struct MC_Test_Utilities
 *
 * @brief Container for utility methods that display the output
 *        of the various OS monitors in a friendly format.
 */
struct MC_TEST_UTILITIES_Export MC_Test_Utilities
{
#if defined (ACE_HAS_MONITOR_FRAMEWORK) && (ACE_HAS_MONITOR_FRAMEWORK == 1)

  static void display_timestamp (Monitor_Control_Types::Data const &data);
  static void display_num_threads (Monitor_Control_Types::Data const &data);
  static void display_mq_size (Monitor_Control_Types::Data const &data);
  static void display_memory_usage (Monitor_Control_Types::Data const &data);
  static void display_cpu_load (Monitor_Control_Types::Data const &data);
  static void display_bytes_sent (Monitor_Control_Types::Data const &data);
  static void display_bytes_received (Monitor_Control_Types::Data const &data);
  static void display_packets_sent (Monitor_Control_Types::Data const &data);
  static void display_packets_received (Monitor_Control_Types::Data const &data);

#endif /* ACE_HAS_MONITOR_FRAMEWORK==1 */
};

#include /**/ "ace/post.h"

#endif /* MC_TEST_UTILITIES_H */
