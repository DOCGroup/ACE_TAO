// Regression-test server for the idle-transport-timeout feature.
//
// Writes its IOR to a file so that run_test.pl (and the client) can
// locate it.  Accepts a configurable svc.conf via -ORBSvcConf so the
// test driver can set -ORBTransportIdleTimeout to a small value.

#include "Echo_i.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "ace/Log_Msg.h"

static const ACE_TCHAR *ior_output_file = ACE_TEXT ("test.ior");

static int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("o:"));
  int c;
  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Usage: server [-o <ior_file>]\n")),
                          -1);
      }
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var poa_object =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Nil RootPOA\n")), 1);

      PortableServer::POAManager_var mgr = root_poa->the_POAManager ();

      Echo_i *echo_impl = nullptr;
      ACE_NEW_RETURN (echo_impl, Echo_i (orb.in ()), 1);
      PortableServer::ServantBase_var owner (echo_impl);

      PortableServer::ObjectId_var oid =
        root_poa->activate_object (echo_impl);

      CORBA::Object_var obj = root_poa->id_to_reference (oid.in ());
      Test::Echo_var echo = Test::Echo::_narrow (obj.in ());

      CORBA::String_var ior = orb->object_to_string (echo.in ());

      // Write IOR to file so the test driver and client can find it
      FILE *f = ACE_OS::fopen (ior_output_file, ACE_TEXT ("w"));
      if (f == nullptr)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Cannot open output file '%s'\n"),
                           ior_output_file), 1);
      ACE_OS::fprintf (f, "%s", ior.in ());
      ACE_OS::fclose (f);

      mgr->activate ();

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) server: IOR written to '%s'\n"),
                  ior_output_file));

      orb->run ();

      root_poa->destroy (true, true);
      orb->destroy ();
    }
  catch (const CORBA::Exception &ex)
    {
      ex._tao_print_exception (ACE_TEXT ("server exception"));
      return 1;
    }
  return 0;
}
