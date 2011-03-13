// $Id$

#include "test_i.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "tao/RTCORBA/Priority_Mapping.h"
#include "tao/RTCORBA/Priority_Mapping_Manager.h"
#include "tao/Strategies/advanced_resource.h"
#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "ace/Sched_Params.h"
#include "ace/OS_NS_errno.h"

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

  void set (Test_i *server, int priority, int argc, ACE_TCHAR *argv[]);
  // Set the test attributes.

  // = The ACE_Task_Base methods....
  virtual int svc (void);

private:
  Test_i *server_;

  int priority_;
  // Corba priority at which this thread is running.  (This number is
  // used for creating the name of the orb.)

  int argc_;
  ACE_TCHAR *argv_[256];
};

// ****************************************************************

int nthreads = 0;
const int MAX_THREADS = 128;
Server servers[MAX_THREADS];
int priorities[MAX_THREADS];
const ACE_TCHAR *ior_output_file_base = ACE_TEXT("test.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:t:"));
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
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
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

  try
    {
      ACE_TCHAR *argv_[256];
      int argc_ = argc;
      for (int i = 0; i < argc; ++i)
        argv_[i] = argv[i];
      orb = CORBA::ORB_init (argc_, argv_);

      // Parse the arguments.
      if (parse_args (argc_, argv_) != 0)
        return 1;

      // Obtain Priority Mapping used by the ORB.
      CORBA::Object_var object =
        orb->resolve_initial_references ("PriorityMappingManager");

      RTCORBA::PriorityMappingManager_var mapping_manager =
        RTCORBA::PriorityMappingManager::_narrow (object.in ());

      if (CORBA::is_nil (mapping_manager.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Priority Mapping Manager is nil\n"),
                            1);
        }

      pm = mapping_manager->mapping ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "Caught exception in Orb per priority server:");
      return 1;
    }

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
             ACE_TCHAR *argv[])
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

  try
    {
      char orb_name[64];
      ACE_OS::sprintf (orb_name, "%d", this->priority_);
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc_, argv_, orb_name);

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      PortableServer::ObjectId_var oid =
        root_poa->activate_object (this->server_);

      CORBA::Object_var obj =
        root_poa->id_to_reference (oid.in ());

      CORBA::String_var ior =
        orb->object_to_string (obj.in ());

      ACE_DEBUG ((LM_DEBUG, "Activated as <%C>\n", ior.in ()));

      // Get the file name to store the ior.
      char file_name[100];
      ACE_OS::sprintf (file_name,
                       "%s_%d",
                       ACE_TEXT_ALWAYS_CHAR (ior_output_file_base),
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
      poa_manager->activate ();

      orb->run ();

    }
  catch (const CORBA::Exception& ex)
    {
      char message[100];
      ACE_OS::sprintf (message,
                       "ORB_per_Priority::server: Exception in thread with priority = %d",
                       this->priority_);
      ex._tao_print_exception (message);
    }

  return 0;
}
