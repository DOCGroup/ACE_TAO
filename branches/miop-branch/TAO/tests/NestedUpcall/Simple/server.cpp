// $Id$

#include "server_i.h"
#include "ace/Get_Opt.h"
#include "ace/Task.h"

ACE_RCSID(Simple, server, "$Id$")

static int quiet = 0;
// The test is quiet...

static const char *ior_file = "test.ior";
// File of file to which the ior is written

static int number_of_threads = 1;
// Number of server threads.

class Server_Task : public ACE_Task_Base
{
public:
  Server_Task (CORBA::ORB_ptr orb);
  int svc (void);

private:
  CORBA::ORB_var orb_;
};

Server_Task::Server_Task (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

int
Server_Task::svc (void)
{
  TAO_ENV_DECLARE_NEW_ENV;
  ACE_TRY
    {
      this->orb_->run (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      return -1;
    }
  ACE_ENDTRY;
  return 0;
}

static int
write_ior_to_file (const char *ior)
{
  FILE *output_file = ACE_OS::fopen (ior_file, "w");

  if (output_file == 0)
    ACE_ERROR_RETURN ((LM_ERROR, "Cannot open output files for writing IOR: %s\n",
                       ior_file),
                      -1);

  int result = 0;

  result = ACE_OS::fprintf (output_file,
                            "%s",
                            ior);
  if (result < 0 ||
      ACE_static_cast (size_t, result) != ACE_OS::strlen (ior))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "ACE_OS::fprintf failed while writing %s to %s\n",
                       ior,
                       ior_file),
                      -1);

  ACE_OS::fclose (output_file);

  return 0;
}

static int
parse_args (int argc,
            char **argv)
{
  ACE_Get_Opt get_opts (argc, argv, "q:f:t:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'q':
        quiet = 1;
        break;

      case 't':
        number_of_threads = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'f':
        ior_file = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " [-t number of threads]"
                           " [-f ior file]"
                           " [-q (quite)]"
                           "\n",
                           argv[0]),
                          -1);
      }

  return 0;
}

int
main (int argc,
      char **argv)
{
  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                            argv,
                                            0
                                            TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      int result = parse_args (argc,
                               argv);
      if (result != 0)
        return result;

      CORBA::Object_var object = orb->resolve_initial_references ("RootPOA"
                                                                  TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (object.in ()
                                      TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      poa_manager->activate (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      server_i server_servant (quiet,
                               orb.in ());

      server_var server_object = server_servant._this (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::String_var ior = orb->object_to_string (server_object.in ()
                                                     TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      result = write_ior_to_file (ior.in ());
      if (result != 0)
        return result;

      Server_Task server_tasks (orb.in ());

      result = server_tasks.activate (THR_BOUND,
                                      number_of_threads);

      if (result != 0)
        return result;

      result = ACE_Thread_Manager::instance ()->wait ();

      if (result != 0)
        return result;

      root_poa->destroy (1,
                         1
                         TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "server::main");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}
