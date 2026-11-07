// -*- C++ -*-
#include "ace/Get_Opt.h"
#include "test_i.h"
#include "Server_ORBInitializer.h"
#include "Server_Request_Interceptor.h"
#include "tao/ORBInitializer_Registry.h"
#include "ace/OS_NS_stdio.h"
#include "CrashPoint.h"
#include "ace/SString.h"
#include <memory>

struct Options
{
  ACE_TString ior_file;
  int crash_point {};
  int number {};
  int server_number {};
};

int
parse_args (int argc, ACE_TCHAR *argv[], Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("o:c:n:s:"));
  int c {};

  while ((c = get_opts ()) != -1)
    {
      switch (c)
        {
        case 'o':
          options.ior_file = get_opts.opt_arg ();
          break;
        case 'c':
          options.crash_point = ACE_OS::atoi (get_opts.opt_arg ());
          break;
        case 'n':
          options.number = ACE_OS::atoi (get_opts.opt_arg ());
          break;
        case 's':
          options.server_number = ACE_OS::atoi (get_opts.opt_arg ());
          break;
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Usage:  %s "
                             "-o <IOR> "
                             "-c <CrashPoint> "
                             "-n <server number> "
                             "-s <server number>\n",
                             argv[0]),
                            -1);
        }
    }

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
#if TAO_HAS_INTERCEPTORS == 1
      Server_ORBInitializer *temp_initializer {};
      ACE_NEW_RETURN (temp_initializer,
                      Server_ORBInitializer,
                      -1);
      PortableInterceptor::ORBInitializer_var const orb_initializer (temp_initializer);

      PortableInterceptor::register_orb_initializer (orb_initializer.in ());
#endif /* TAO_HAS_INTERCEPTORS == 1 */

      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv, "Server ORB");

      CORBA::Object_var const poa_object =
        orb->resolve_initial_references ("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var const poa_manager =
        root_poa->the_POAManager ();

      Options options {};
      if (::parse_args (argc, argv, options) != 0)
        return -1;

      crash_point = static_cast<short> (options.crash_point);
      test_i *test_impl {};
      ACE_NEW_RETURN (test_impl, test_i (options.number, orb.in ()), 1);
      // The servant var takes ownership; this pointer remains typed for POA activation.
      PortableServer::ServantBase_var const owner_transfer (test_impl);

      PortableServer::ObjectId_var const id =
        root_poa->activate_object (test_impl);

      CORBA::Object_var const object = root_poa->id_to_reference (id.in ());

      RedirectionTest::test_var const test =
        RedirectionTest::test::_narrow (object.in ());

      CORBA::String_var const ior =
        orb->object_to_string (test.in ());

      {
        std::unique_ptr<FILE, int (*) (FILE *)> const output_file (
          ACE_OS::fopen (options.ior_file.c_str (), "w"), ACE_OS::fclose);
        if (!output_file)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Cannot open output file <%s> for writing IOR: %C\n",
                             options.ior_file.c_str (),
                             ior.in ()),
                            1);
        ACE_OS::fprintf (output_file.get (), "%s", ior.in ());
      }

      if (options.server_number == 2)
        ACE_OS::exit (1);

      poa_manager->activate ();

      // Run the ORB event loop.
      orb->run ();

      root_poa->destroy (true, true);

      orb->destroy ();

      ACE_DEBUG ((LM_DEBUG, "Event loop finished.\n"));
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return -1;
    }

  return 0;
}
