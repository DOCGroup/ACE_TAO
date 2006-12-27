// -*- C++ -*-

#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "ace/Task.h"

#include "tao/ORBInitializer_Registry.h"

#include "Current_Test_Impl.h"
#include "Server_Request_Interceptor.h"

ACE_RCSID (Transport_Current,
           server,
           "$Id$")

const char *ior_output_file = "server.ior";
int nthreads = 1;
int use_collocated_call = 1;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "t:o:n:c:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'c':
        use_collocated_call = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;
      case 't':
        nthreads = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 'n': // Does nothing. Added just for symetry with the client.
        break;
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Usage: %s "
                           "-o <iorfile>"
                           "-t threads "
                           "-n n (ignored, if provided) "
                           "-c make_collocated_calls "
                           "\n",
                           argv[0]),
                          -1);
      }

  // Indicates sucessful parsing of the command line
  return 0;
}



/// A helper class to encapsulate a task

class Worker : public ACE_Task_Base
{
public:
  Worker (CORBA::ORB_ptr);
  virtual int svc (void);

private:
  // The ORB
  CORBA::ORB_var orb_;
};


/// Ctor

Worker::Worker (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}


/// Test referencing the TC data *inside* the context of a client-side
/// interceptor

int
Worker::svc (void)
{
  ACE_TRY_NEW_ENV
    {
      this->orb_->run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Server: exception raised");
    }
  ACE_ENDTRY;
  return 0;
}



/// Main driver

int
server_main (int argc,
             ACE_TCHAR *argv[],
             Test::Server_Request_Interceptor *cri)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      PortableInterceptor::ORBInitializer_ptr temp_initializer =
        PortableInterceptor::ORBInitializer::_nil ();

      ACE_NEW_RETURN (temp_initializer,
                      Test::ORBInitializer <PortableInterceptor::ServerRequestInterceptor> (cri),
                      -1);  // No exceptions yet!

      PortableInterceptor::ORBInitializer_var orb_initializer =
        temp_initializer;

      PortableInterceptor::register_orb_initializer (orb_initializer.in ()
                                                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;


      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                            argv,
                                            ACE_TEXT ("test_orb")
                                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var obj =
        orb->resolve_initial_references ("RootPOA"
                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in ()
                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Server (%P|%t) Unable to obtain")
                           ACE_TEXT (" RootPOA reference.\n")),
                          -1);

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return -1;

      Current_Test_Impl server_impl (orb.in (),
                                     root_poa.in (),
                                     use_collocated_call);

      obj = server_impl._this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Test::Transport::CurrentTest_var server =
        Test::Transport::CurrentTest::_narrow (obj.in ()
                                               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (server.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Server (%P|%t) Unable to obtain ")
                           ACE_TEXT ("reference to CurrentTest object.\n")),
                          -1);

      CORBA::String_var ior =
        orb->object_to_string (server.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // If the ior_output_file exists, output the IOR to it.
      if (ior_output_file != 0)
        {
          FILE *output_file = ACE_OS::fopen (ior_output_file, "w");
          if (output_file == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Server (%P|%t) Cannot write "
                               "IOR: %s - %m",
                               ior_output_file),
                              -1);
          ACE_OS::fprintf (output_file, "%s", ior.in ());
          ACE_OS::fclose (output_file);
        }

      // Spawn a number of clients doing the same thing for a
      // predetermined number of times
      Worker worker (orb.in ());
      if (worker.activate (THR_NEW_LWP | THR_JOINABLE,
                           nthreads) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Server (%P|%t) Cannot activate %d server threads\n"),
                           nthreads),
                          -1);
      worker.thr_mgr ()->wait ();

      if (TAO_debug_level >= 1)
        ACE_DEBUG ((LM_INFO, ACE_TEXT ("Server (%P|%t) Event loop finished.\n")));

      if (!cri->self_test ())
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("Server (%P|%t) Interceptor self_test failed\n")));

      server->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           ACE_TEXT ("Server (%P|%t) ERROR: "));

      return -1;
    }
  ACE_ENDTRY;

  ACE_DEBUG ((LM_INFO, ACE_TEXT ("Server (%P|%t) Completed successfuly.\n")));
  return 0;
}
