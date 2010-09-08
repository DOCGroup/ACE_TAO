// $Id$

#include "test_i.h"
#include "ace/Get_Opt.h"
#include "ace/Sched_Params.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_errno.h"

// IOR file name
static const ACE_TCHAR *ior_output_file = ACE_TEXT("test.ior");

static int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <ior file> "
                           "\n",
                           argv [0]),
                          -1);
      }

  // Indicates sucessful parsing of the command line
  return 0;
}

int
set_rt_mode (void)
{
  int policy = ACE_SCHED_FIFO;
  int priority =
    (ACE_Sched_Params::priority_min (policy)
     + ACE_Sched_Params::priority_max (policy)) / 2;

  // Enable FIFO scheduling, e.g., RT scheduling class on Solaris.
  int result =
    ACE_OS::sched_params (ACE_Sched_Params (policy,
                                            priority,
                                            ACE_SCOPE_PROCESS));
  if (result != 0)
    {
      if (ACE_OS::last_error () == EPERM)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "client (%P|%t): user is not superuser, "
                      "test runs in time-shared class\n"));
        }
      else
        ACE_ERROR ((LM_ERROR,
                    "client (%P|%t): sched_params failed %p\n",
                    "set_rt_mode"));
    }

  // Get our thread handle.
  ACE_hthread_t self;
  ACE_OS::thr_self (self);

  // Set our thread priority.
  if (ACE_OS::thr_setprio (self, priority) != 0)
    ACE_ERROR ((LM_ERROR,
                "server (%P|%t):thr_setprio failed %p\n",
                "set_rt_mode"));

  // Do a sanity check.
  if (ACE_OS::thr_getprio (self, priority) == 0)
    ACE_DEBUG ((LM_DEBUG,
                "client (%P|%t): thread priority = %d.\n",
                priority));

  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  int result = set_rt_mode ();
  if (result != 0)
    return result;

  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc,
                         argv);

      // Get the command line options.
      if (parse_args (argc, argv) != 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "parse_args failed\n"),
                            1);
        }

      CORBA::Object_var poa_object =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      Test_i server_impl (orb.in ());

      Test_var server =
        server_impl._this ();

      CORBA::String_var ior =
        orb->object_to_string (server.in ());

      ACE_DEBUG ((LM_DEBUG,
                  "Activated as <%C>\n",
                  ior.in ()));

      FILE *output_file= ACE_OS::fopen (ior_output_file, "w");

      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file for writing IOR: %s",
                           ior_output_file),
                          1);

      ACE_OS::fprintf (output_file,
                       "%s",
                       ior.in ());

      ACE_OS::fclose (output_file);

      poa_manager->activate ();

      orb->run ();

      ACE_DEBUG ((LM_DEBUG, "event loop finished\n"));

      root_poa->destroy (1,
                         1);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("server");
      return 1;
    }

  return 0;
}
