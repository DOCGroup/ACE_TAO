// $Id$

#include "Session_Factory.h"

#include "RT_Class.h"
#include "ORB_Holder.h"
#include "Servant_var.h"
#include "RIR_Narrow.h"
#include "RTServer_Setup.h"
#include "ORB_Task.h"
#include "ORB_Task_Activator.h"

#include "tao/PortableServer/PortableServer.h"
#include "tao/RTPortableServer/RTPortableServer.h"
#include "tao/Strategies/advanced_resource.h"
#include "tao/Messaging/Messaging.h"
#include "ace/Get_Opt.h"
#include "ace/Auto_Ptr.h"

ACE_RCSID (TAO_PERF_RTEC_RTCORBA_Baseline,
           server,
           "$Id$")

const ACE_TCHAR *ior_output_file = ACE_TEXT ("test.ior");
int use_rt_corba = 0;
int nthreads = 0;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:n:r"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;

      case 'n':
        nthreads = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'r':
        use_rt_corba = 1;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <iorfile> "
                           "-r (use RT-CORBA) "
                           "-n nthreads "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  /// Move the test to the real-time class if it is possible.
  RT_Class rt_class;

  try
    {
      ORB_Holder orb (argc, argv, "");

      if (parse_args (argc, argv) != 0)
        return 1;

      RTServer_Setup rtserver_setup (use_rt_corba,
                                     orb,
                                     rt_class,
                                     nthreads);

      PortableServer::POA_var root_poa =
        RIR_Narrow<PortableServer::POA>::resolve (orb,
                                                  "RootPOA");

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      poa_manager->activate ();

      PortableServer::POA_var the_poa (rtserver_setup.poa ());

      ORB_Task orb_task (orb);
      ORB_Task_Activator orb_task_activator (rt_class.priority_high (),
                                             rt_class.thr_sched_class (),
                                             nthreads,
                                             &orb_task);

      ACE_DEBUG ((LM_DEBUG, "Finished ORB and POA configuration\n"));

      Servant_var<Session_Factory> session_factory (
          new Session_Factory (orb,
                               the_poa.in ())
          );

      CORBA::Object_var object =
        session_factory->_this ();

      CORBA::String_var ior =
        orb->object_to_string (object.in ());

      // Output the ior to the <ior_output_file>
      FILE *output_file = ACE_OS::fopen (ior_output_file, "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file for writing IOR: %s",
                           ior_output_file),
                              1);
      ACE_OS::fprintf (output_file, "%s", ior.in ());
      ACE_OS::fclose (output_file);

      orb->run ();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - event loop finished\n"));
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
