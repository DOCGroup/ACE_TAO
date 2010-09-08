// $Id$

#include "Hello.h"
#include "Server_Task.h"
#include "tao/Utils/Servant_Var.h"
#include "ace/Get_Opt.h"
#include "ace/High_Res_Timer.h"

const ACE_TCHAR *ior_output_file = ACE_TEXT("test.ior");

bool simulate_crashes = true;

int nthreads = 1;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("xn:o:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;

      case 'x':
        simulate_crashes = false;
        break;

      case 'n':
        nthreads = ACE_OS::atoi(get_opts.opt_arg());
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                              "usage:  %s"
                              " -o <iorfile>"
                              " -x (do not simulate crashes)"
                              " -n <nthreads>"
                              "\n",
                              argv [0]),
            -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  // Fetching the high res timer's global scale factor ensures that it
  // is calibrated (if necessary on this platform) at the beginning of
  // the test.  While the timer would otherwise be calibrated on first
  // use, this introduces delay in the middle of the test's execution.
  // This leads to failures due to timing assumptions (timeouts, etc.)
  // within the test itself.
  (void) ACE_High_Res_Timer::global_scale_factor();

  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil RootPOA\n"),
                          -1);

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      if (parse_args (argc, argv) != 0)
        return -1;

      TAO::Utils::Servant_Var<Hello> hello_impl(
          new Hello(orb.in(), simulate_crashes));

      PortableServer::ObjectId_var id =
        root_poa->activate_object (hello_impl.in ());

      CORBA::Object_var object = root_poa->id_to_reference (id.in ());

      Test::Hello_var hello =
        Test::Hello::_narrow (object.in ());

      CORBA::String_var ior =
        orb->object_to_string (hello.in ());

      // Output the IOR to the <ior_output_file>
      FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file for writing IOR: %s",
                           ior_output_file),
                              -1);
      ACE_OS::fprintf (output_file, "%s", ior.in ());
      ACE_OS::fclose (output_file);

      poa_manager->activate ();


      Server_Task st (orb.in (),
                      ACE_Thread_Manager::instance ());

      if (st.activate (THR_NEW_LWP | THR_JOINABLE, nthreads, 1) == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      "Error activating server task\n"));

          return -1;
        }

      ACE_Thread_Manager::instance()->wait();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - event loop finished\n"));

      root_poa->destroy (1, 1);

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return -1;
    }

  return 0;
}
