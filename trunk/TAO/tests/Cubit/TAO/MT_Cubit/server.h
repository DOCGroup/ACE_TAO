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

// ACE includes.

#if defined (TAO_PLATFORM_SVC_CONF_FILE_NOTSUP)
#define TAO_DEFAULT_SERVER_STRATEGY_FACTORY_ARGS { "-ORBconcurrency", "thread-per-connection", \
  "-ORBdemuxstrategy", "dynamic", \
  "-ORBtablesize", "128" }
#endif

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

#if defined (VME_DRIVER)
#include <hostLib.h>
extern "C" STATUS vmeDrv (void);
extern "C" STATUS vmeDevCreate (char *);
#endif /* VME_DRIVER */

#if !defined (ACE_HAS_THREADS)
class NOOP_ACE_Barrier
{
public:
  NOOP_ACE_Barrier (int ) {}
  void wait (void) {}
};
#define ACE_Barrier NOOP_ACE_Barrier
#endif /* ACE_HAS_THREADS */

class Globals
{
public:
  Globals (void);
  int parse_args (int argc, char **argv);
  char hostname[BUFSIZ];
  char *ior_file;
  int base_port;
  u_int num_of_objs;
  u_int use_name_service;
  u_int thread_per_rate;
  u_int use_multiple_priority;
  u_int run_utilization_test;
  int ready_;
  // ready flag used by the high priority thread to wake up the low
  // priority threads after it's parsed the arguments.

  ACE_SYNCH_MUTEX ready_mtx_;
  // mutex for the condition variable.

  ACE_Condition<ACE_SYNCH_MUTEX> ready_cnd_;
  // condition variable for the low priority threads to wait 
  //until the high priority thread is done with the arguments parsing.
  
  ACE_Barrier *barrier_;
  // Barrier for the multiple clients to synchronize after binding to
  // the servants.

};

typedef ACE_Singleton<Globals,ACE_Null_Mutex> GLOBALS;

class Cubit_Task : public ACE_Task<ACE_MT_SYNCH>
{
  // = TITLE
  //    Encapsulates an ORB for the Cubit application.
public:
  Cubit_Task (const char *args,
              const char* orbname,
              u_int num_of_objs,
	      Task_State *ts,
	      ACE_Thread_Manager *thr_mgr,
	      u_int task_id);
  // Constructor.

  virtual int svc (void);
  // Active Object entry point.

  CORBA::String get_servant_ior (u_int index);

protected:
  Cubit_Task (void);
  // No-op constructor.

private:
  int initialize_orb (void);
  // Initialize the ORB, and POA.

  int create_servants (void);
  // Create the servants

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
  // Array to hold the servants

  CORBA::String *servants_iors_;
  // ior strings of the servants

  //CosNaming::NamingContext_var naming_context_;
  // Object reference to the naming service

  u_int task_id_;
  // id used for naming service object name.

  CosNaming::NamingContext_var mt_cubit_context_;
  // context where all MT Cubit objects will be created.

  Task_State *ts_;
  // state for the utilization thread to synchronize with the servants.

  TAO_ORB_Manager orb_manager_;
  // The TAO ORB Manager

  TAO_Naming_Client my_name_client_;
  // An instance of the name client used for resolving the factory
  // objects.
};


class Server
{
public:
  // default constructor
  int initialize (int argc, char **argv);
  int start_servants (ACE_Thread_Manager *serv_thr_mgr,Task_State *ts);
  Util_Thread * start_utilization (ACE_Thread_Manager *util_thr_mgr, Task_State *ts);
private:
  int argc_;
  char **argv_;
};
