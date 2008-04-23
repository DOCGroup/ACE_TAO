// -*- C++ -*-

#include "ace/Get_Opt.h"
#include "ace/OS_NS_unistd.h"

#include "test_i.h"
#include "ServerORBInitializer.h"

#include "tao/ORBInitializer_Registry.h"
#include "tao/PI/PI.h"
#include "ace/OS_NS_stdio.h"

ACE_RCSID (PICurrent,
           server,
           "$Id$")

const char *ior_output_file = "test.ior";

extern PortableInterceptor::SlotId slot_id;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "o:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Usage: %s "
                           "-o <iorfile>"
                           "\n",
                           argv[0]),
                          -1);
      }

  // Indicates sucessful parsing of the command line
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      PortableInterceptor::ORBInitializer_ptr temp_initializer =
        PortableInterceptor::ORBInitializer::_nil ();

      ACE_NEW_RETURN (temp_initializer,
                      ServerORBInitializer,
                      -1);  // No CORBA exceptions yet!
      PortableInterceptor::ORBInitializer_var orb_initializer =
        temp_initializer;

      PortableInterceptor::register_orb_initializer (orb_initializer.in ());

      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                            argv,
                                            "test_orb");

      CORBA::Object_var obj =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in ());

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Unable to obtain RootPOA reference.\n"),
                          -1);

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      poa_manager->activate ();

      if (parse_args (argc, argv) != 0)
        return -1;

      obj = orb->resolve_initial_references ("PICurrent");

      PortableInterceptor::Current_var pi_current =
        PortableInterceptor::Current::_narrow (obj.in ());

      if (CORBA::is_nil (pi_current.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Unable to obtain PICurrent reference.\n"),
                          -1);

      test_i server_impl (pi_current.in (),
                          ::slot_id,
                          orb.in ());

      obj = server_impl._this ();

      PICurrentTest::test_var server =
        PICurrentTest::test::_narrow (obj.in ());

      if (CORBA::is_nil (server.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Unable to obtain reference to "
                           "PICurrentTest::test object.\n"),
                          -1);

      CORBA::String_var ior =
        orb->object_to_string (server.in ());

      ACE_DEBUG ((LM_INFO, "PICurrentTest::test: <%s>\n", ior.in ()));

      // If the ior_output_file exists, output the IOR to it.
      if (ior_output_file != 0)
        {
          FILE *output_file = ACE_OS::fopen (ior_output_file, "w");
          if (output_file == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Cannot open output file for writing "
                               "IOR: %s",
                               ior_output_file),
                              1);
          ACE_OS::fprintf (output_file, "%s", ior.in ());
          ACE_OS::fclose (output_file);
        }

      orb->run ();

      ACE_OS::sleep(1);
      orb->destroy ();

      ACE_DEBUG ((LM_INFO, "Event loop finished.\n"));
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("PICurrent test (server-side):");

      return -1;
    }

  return 0;
}
