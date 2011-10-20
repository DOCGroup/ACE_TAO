/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    server.h
 *
 *  $Id$
 *
 *  @author Andy Gokhale
 *  @author Sumedh Mungee
 *  @author Sergio Flores-Gaitan and Nagarajan Surendran.
 */
//=============================================================================


#ifndef SERVER_H
#define SERVER_H

// ACE includes.

#if defined (TAO_PLATFORM_SVC_CONF_FILE_NOTSUP)
#define TAO_DEFAULT_SERVER_STRATEGY_FACTORY_ARGS \
{ "-ORBconcurrency", "thread-per-connection", \
  "-ORBdemuxstrategy", "dynamic", \
  "-ORBtablesize", "128" }
#endif /* TAO_PLATFORM_SVC_CONF_FILE_NOTSUP */

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Get_Opt.h"
#include "ace/Log_Msg.h"
#include "ace/ARGV.h"
#include "ace/Sched_Params.h"

// MT Cubit application includes.
#include "cubit_i.h"
#include "Task_Client.h"
#include "Util_Thread.h"
#include "Globals.h"
#include "Cubit_Task.h"

#if defined (VME_DRIVER)
#include <hostLib.h>
extern "C" STATUS vmeDrv (void);
extern "C" STATUS vmeDevCreate (char *);
#endif /* VME_DRIVER */

/**
 * @class Server
 *
 * @brief A multithreaded cubit server class.
 *
 * This class encapsulates the functionality of a multi-threaded
 * cubit server. To use this, call initialize and then
 * start_servants method.
 */
class Server
{
public:
  /// Default constructor.
  Server (void);

  /// Initialize the server state.
  int init (int argc, ACE_TCHAR **argv);

  /// Run method.
  int run (void);

private:
  /// Start the high and low priority servants.
  int start_servants (void);

  /// Preliminary argument processing code.
  void prelim_args_process (void);

  /// sets the priority to be used for the low priority servants.
  void init_low_priority (void);

  /// Writes the iors of the servants to a file
  int write_iors (void);

  /// Activates the high priority servant.
  int activate_high_servant (void);

  /// Activates the low priority servants.
  int activate_low_servants (void);

  /// Number of arguments for the servant.
  int argc_;

  /// Arguments for the ORB.
  ACE_TCHAR **argv_;

  /// Array to hold pointers to the Cubit objects.
  char * *cubits_;

  /// Pointer to the high priority task
  Cubit_Task *high_priority_task_;

  /// Array to hold pointers to the low priority tasks.
  Cubit_Task **low_priority_tasks_;

  /// Priority used for the high priority servant.
  ACE_Sched_Priority high_priority_;

  /// Priority used by the low priority servants.
  ACE_Sched_Priority low_priority_;

  /// Number of low priority servants
  u_int num_low_priority_;

  /// Number of priorities used.
  u_int num_priorities_;

  /**
   * Granularity of the assignment of the priorities.  Some OSs have
   * fewer levels of priorities than we have threads in our test, so
   * with this mechanism we assign priorities to groups of threads
   * when there are more threads than priorities.
   */
  u_int grain_;

  /// count of the number of priorities used within a grain.
  u_int counter_;

  /// argv passed to the high priority servant.
  ACE_ARGV *high_argv_;

  /// argv passed to the low priority servants.
  ACE_ARGV *low_argv_;

  /// Priority helper object.
  MT_Priority priority_;

  /// Thread manager for the servant threads.
  ACE_Thread_Manager servant_manager_;
};

#endif /* SERVER_H */
