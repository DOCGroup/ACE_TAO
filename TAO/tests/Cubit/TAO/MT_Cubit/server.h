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
//    Andy Gokhale, Sumedh Mungee, and Sergio Flores-Gaitan
//
// ============================================================================

#if !defined (SERVER_H)
#define SERVER_H

// ACE includes.

#if defined (TAO_PLATFORM_SVC_CONF_FILE_NOTSUP)
#define TAO_DEFAULT_SERVER_STRATEGY_FACTORY_ARGS \
{ "-ORBconcurrency", "thread-per-connection", \
  "-ORBdemuxstrategy", "dynamic", \
  "-ORBtablesize", "128" }
#endif /* TAO_PLATFORM_SVC_CONF_FILE_NOTSUP */

#include "ace/OS.h"
#include "ace/Get_Opt.h"
#include "ace/Log_Msg.h"
#include "ace/ARGV.h"
#include "ace/Sched_Params.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/Naming/Naming_Utils.h"

// TAO includes.
#include "tao/corba.h"
#include "tao/TAO.h"

// MT Cubit application includes.
#include "cubit_i.h"
#include "Task_Client.h"
#include "Util_Thread.h"
#include "Globals.h"

#if defined (VME_DRIVER)
#include <hostLib.h>
extern "C" STATUS vmeDrv (void);
extern "C" STATUS vmeDevCreate (char *);
#endif /* VME_DRIVER */

// Make the globals a Singleton.
typedef ACE_Singleton<Globals,ACE_Null_Mutex> GLOBALS;

// @@ Naga, can you please split this class into a separate file
// called Cubit_Task.h?  It looks odd in a file called server.h!

class Cubit_Task : public ACE_Task<ACE_MT_SYNCH>
{
  // = TITLE
  //    Encapsulates an ORB for the Cubit application.
public:
  Cubit_Task (const char *args,
              const char* orbname,
              u_int num_of_objs,
	      ACE_Thread_Manager *thr_mgr,
	      u_int task_id);
  // Constructor.

  virtual int svc (void);
  // Active Object entry point.

  CORBA::String get_servant_ior (u_int index);
  // @@ Naga, can you please comment this?

protected:
  Cubit_Task (void);
  // No-op constructor.

private:
  int initialize_orb (void);
  // Initialize the ORB, and POA.

  int create_servants (void);
  // Create the servants.

  CORBA::String key_;
  // All cubit objects will have this as prefix to its key.

  char *orbname_;
  // Name of the ORB.

  char *orbargs_;
  // ORB arguments.

  u_int num_of_objs_;
  // Number of objects we're managing.

  CORBA::ORB_var orb_;
  // Pointer to the ORB

  Cubit_i **servants_;
  // Array to hold the servants.

  CORBA::String *servants_iors_;
  // IOR strings of the servants.

  //CosNaming::NamingContext_var naming_context_;
  // Object reference to the naming service.

  u_int task_id_;
  // ID used for naming service object name.

  CosNaming::NamingContext_var mt_cubit_context_;
  // Context where all MT Cubit objects will be created.

  TAO_ORB_Manager orb_manager_;
  // The TAO ORB Manager.

  TAO_Naming_Client my_name_client_;
  // An instance of the name client used for resolving the factory
  // objects.
};

// @@ Naga, why do we inherit this "public virtual?"  Isn't "virtual"
// good enough?  
class Server : public virtual MT_Priority
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

  int initialize (int argc, char **argv);
  // Initialize the server state.

  int start_servants (ACE_Thread_Manager *serv_thr_mgr);
  // Start the high and low priority servants.

private:
  void prelim_args_process (void);
  // Preliminary argument processing code.

  void init_low_priority (void);
  // sets the priority to be used for the low priority servants.

  void write_iors (void);
  // Writes the iors of the servants to a file

  int activate_high_servant (ACE_Thread_Manager *serv_thr_mgr);
  // Activates the high priority servant.

  int activate_low_servants (ACE_Thread_Manager *serv_thr_mgr);
  // Activates the low priority servants.

  int argc_;
  // Number of arguments for the servant.

  char **argv_;
  // Arguments for the ORB.

  CORBA::String *cubits_;
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
  // @@ Naga, can you please comment this?

  ACE_ARGV *high_argv_;
  // argv passed to the high priority servant.

  ACE_ARGV *low_argv_;
  // argv passed to the low priority servants.

  MT_Priority priority_;
  // Priority helper object.
  // @@ Naga, why do we both inherit from MT_Priority and also define
  // an instance of it?
};

#endif /* SERVER_H */







