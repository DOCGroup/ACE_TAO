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

// TAO includes.
#include "tao/corba.h"

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

class Cubit_Task : public ACE_Task<ACE_MT_SYNCH>
{
  // = TITLE
  //    Encapsulates an ORB for the Cubit application.
 public:
  Cubit_Task (const char *args,
              const char* orbname,
              u_int num_of_objs,
	      ACE_Barrier *barrier,
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

  PortableServer::POA_var root_poa_;
  // Pointer to the Root POA

  PortableServer::POA_var poa_;
  // Pointer to the child POA used on the application.

  PortableServer::POAManager_var poa_manager_;
  // The POA Manager for both the root POA and the child POA.

  Cubit_i **servants_;
  // Array to hold the servants

  ACE_Barrier *barrier_;
  // Barrier for the multiple servants to synchronize after
  // binding to the orb.

  CORBA::String *servants_iors_;
  // ior strings of the servants

  CosNaming::NamingContext_var naming_context_;
  // Object reference to the naming service

  u_int task_id_;
  // id used for naming service object name.

  CosNaming::NamingContext_var mt_cubit_context_;
  // context where all MT Cubit objects will be created.

  Task_State *ts_;
  // state for the utilization thread to synchronize with the servants.
};

