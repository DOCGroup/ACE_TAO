// $Id$

#include "test_i.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "tao/RTCORBA/Priority_Mapping.h"
#include "tao/RTCORBA/Priority_Mapping_Manager.h"
#include "tao/Strategies/advanced_resource.h"
#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "ace/Sched_Params.h"

ACE_RCSID(TPP, server, "$Id$")

class Server : public ACE_Task_Base
{
  // = TITLE
  //   Run the server thread, which will create its own orb.
  //
  // = DESCRIPTION
  //   Use the ACE_Task_Base class to run the server threads.
  //
public:
  Server (void);
  // ctor

  void set (Test_i *server, int priority, int argc, char *argv[]);
  // Set the test attributes.

  // = The ACE_Task_Base methods....
  virtual int svc (void);

private:
  Test_i *server_;

  int priority_;
  // Corba priority at which this thread is running.  (This number is
  // used for creating the name of the orb.)

  int argc_;
  char *argv_[256];
};

// ****************************************************************

int nthreads = 0;
const int MAX_THREADS = 128;
Server servers[MAX_THREADS];
int priorities[MAX_THREADS];
const char *ior_output_file_base = "test.ior";

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "o:t:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file_base = get_opts.opt_arg ();
        break;
      case 't':
        if (nthreads < MAX_THREADS)
          {
            priorities[nthreads] = ACE_OS::atoi (get_opts.opt_arg ());
            nthreads++;
          }
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <iorfile_base_name> "
                           "-t <priority>"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

int
main (int argc, char *argv[])
{
  int policy = ACE_SCHED_FIFO;
  int flags = THR_NEW_LWP|THR_JOINABLE|THR_SCHED_FIFO|THR_BOUND;
  int priority =
    ACE_Sched_Params::priority_max (policy);

  // Enable FIFO scheduling, e.g., RT scheduling class on Solaris.
  if (ACE_OS::sched_params (ACE_Sched_Params (policy,
                                              priority,
                                              ACE_SCOPE_PROCESS)) != 0)
    {
      if (ACE_OS::last_error () == EPERM)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "server (%P|%t): user is not superuser, "
                      "test runs in time-shared class\n"));
          policy = ACE_SCHED_OTHER;
          flags = THR_NEW_LWP|THR_JOINABLE|THR_SCHED_DEFAULT;
        }
      else
        ACE_ERROR ((LM_ERROR,
                    "server (%P|%t): sched_params failed\n"));
    }

  // Create a servant.
  Test_i server_impl;

  // Create an ORB to obtain Priority Mapping functionality.
  CORBA::ORB_var orb;
  RTCORBA::PriorityMapping *pm = 0;

  ACE_TRY_NEW_ENV
    {
      char *argv_[256];
      int argc_ = argc;
      for (int i = 0; i < argc; ++i)
        argv_[i] = argv[i];
      orb = CORBA::ORB_init (argc_, argv_, "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Parse the arguments.
      if (parse_args (argc_, argv_) != 0)
        return 1;

      // Obtain Priority Mapping used by the ORB.
      CORBA::Object_var object =
        orb->resolve_initial_references ("PriorityMappingManager"
                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      RTCORBA::PriorityMappingManager_var mapping_manager =
        RTCORBA::PriorityMappingManager::_narrow (object.in ()
                                              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (mapping_manager.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Priority Mapping Manager is nil\n"),
                            1);
        }

      pm = mapping_manager->mapping ();
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Caught exception in Orb per priority server:");
      return 1;
    }
  ACE_ENDTRY;

  for (int i = 0; i != nthreads; ++i)
    {
      CORBA::Short native_priority = 0;
      pm->to_native (priorities[i], native_priority);

      servers[i].set (&server_impl, priorities[i], argc, argv);

      if (servers[i].activate (flags,
                               1, 1,
                               native_priority) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot activate server threads\n"),
                          1);
    }

  ACE_Thread_Manager::instance ()->wait ();

  return 0;
}

// ****************************************************************

Server::Server (void)
{
}

void
Server::set (Test_i *server,
             int priority,
             int argc,
             char *argv[])
{
  server_ = server;
  priority_ = priority;
  argc_ = argc;
  for (int i = 0; i < argc; ++i)
    argv_[i] = argv[i];
}

int
Server::svc (void)
{
  ACE_hthread_t current;
  ACE_Thread::self (current);

  int native_priority;
  ACE_Thread::getprio (current, native_priority);

  ACE_DEBUG ((LM_DEBUG,
              "Server orb thread (%t): corba_priority = %d"
              " actual native priority = %d\n",
              priority_,
              native_priority));

  ACE_TRY_NEW_ENV
    {
      char orb_name[64];
      ACE_OS::sprintf (orb_name, "%d", this->priority_);
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc_, argv_, orb_name ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::ObjectId_var oid =
        root_poa->activate_object (this->server_ ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var obj =
        root_poa->id_to_reference (oid.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::String_var ior =
        orb->object_to_string (obj.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "Activated as <%s>\n", ior.in ()));

      // Get the file name to store the ior.
      char file_name[100];
      ACE_OS::sprintf (file_name,
                       "%s_%d",
                       ior_output_file_base,
                       this->priority_);

      // Output the ior to a file.
      FILE *output_file= ACE_OS::fopen (file_name, "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file for writing IOR: %s",
                           file_name),
                          1);
      ACE_OS::fprintf (output_file, "%s", ior.in ());
      ACE_OS::fclose (output_file);

      // Start orb event loop.
      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      orb->run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

    }
  ACE_CATCHANY
    {
      char message[100];
      ACE_OS::sprintf (message,
                       "ORB_per_Priority::server: Exception in thread with priority = %d",
                       this->priority_);
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, message);
    }
  ACE_ENDTRY;

  return 0;
}
