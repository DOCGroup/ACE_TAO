/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests
//
// = FILENAME
//    server.h
//
// = AUTHOR
//    Andy Gokhale, Sumedh Mungee,Sergio Flores-Gaitan and Nagarajan
//    Surendran.
//
// ============================================================================

#ifndef SERVER_H
#define SERVER_H

// ACE includes.

#if defined (TAO_PLATFORM_SVC_CONF_FILE_NOTSUP)
#define TAO_DEFAULT_SERVER_STRATEGY_FACTORY_ARGS \
{ "-ORBconcurrency", "thread-per-connection", \
  "-ORBdemuxstrategy", "dynamic", \
  "-ORBtablesize", "128" }
#endif /* TAO_PLATFORM_SVC_CONF_FILE_NOTSUP */

#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Get_Opt.h"
#include "ace/Log_Msg.h"
#include "ace/ARGV.h"
#include "ace/Sched_Params.h"

// TAO includes.
#include "tao/corba.h"
#include "tao/PortableServer/ORB_Manager.h"

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

class Server
{
  // = TITLE
  //     A multithreaded cubit server class.
  //
  // = DESCRIPTION
  //     This class encapsulates the functionality of a multi-threaded
  //     cubit server. To use this, call initialize and then
  //     start_servants method.
public:
  Server (void);
  // Default constructor.

  int init (int argc, char **argv);
  // Initialize the server state.

  int run (void);
  // Run method.

private:
  int start_servants (void);
  // Start the high and low priority servants.

  void prelim_args_process (void);
  // Preliminary argument processing code.

  void init_low_priority (void);
  // sets the priority to be used for the low priority servants.

  int write_iors (void);
  // Writes the iors of the servants to a file

  int activate_high_servant (void);
  // Activates the high priority servant.

  int activate_low_servants (void);
  // Activates the low priority servants.

  int argc_;
  // Number of arguments for the servant.

  char **argv_;
  // Arguments for the ORB.

  char * *cubits_;
  // Array to hold pointers to the Cubit objects.

  Cubit_Task *high_priority_task_;
  // Pointer to the high priority task

  Cubit_Task **low_priority_tasks_;
  // Array to hold pointers to the low priority tasks.

  ACE_Sched_Priority high_priority_;
  // Priority used for the high priority servant.

  ACE_Sched_Priority low_priority_;
  // Priority used by the low priority servants.

  u_int num_low_priority_;
  // Number of low priority servants

  u_int num_priorities_;
  // Number of priorities used.

  u_int grain_;
  // Granularity of the assignment of the priorities.  Some OSs have
  // fewer levels of priorities than we have threads in our test, so
  // with this mechanism we assign priorities to groups of threads
  // when there are more threads than priorities.

  u_int counter_;
  // count of the number of priorities used within a grain.

  ACE_ARGV *high_argv_;
  // argv passed to the high priority servant.

  ACE_ARGV *low_argv_;
  // argv passed to the low priority servants.

  MT_Priority priority_;
  // Priority helper object.

  ACE_Thread_Manager servant_manager_;
  // Thread manager for the servant threads.
};

#endif /* SERVER_H */
