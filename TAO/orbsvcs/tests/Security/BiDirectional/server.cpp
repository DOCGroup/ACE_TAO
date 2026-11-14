#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_stdlib.h"
#include "test_i.h"
#include "tao/BiDir_GIOP/BiDirGIOP.h"
#include "tao/AnyTypeCode/Any.h"

#include <memory>

struct Server_Options
{
  ACE_TCHAR const *ior_output_file {};
  int iterations {10};
};

static int
parse_args (int argc, ACE_TCHAR *argv[], Server_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("o:i:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        options.ior_output_file = get_opts.opt_arg ();
        break;
      case 'i':
        options.iterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s -o <iorfile> -i <no_iterations>\n",
                           argv [0]),
                          -1);
      }

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      Server_Options options;
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);
      CORBA::Object_var const poa_object =
        orb->resolve_initial_references ("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT (" (%P|%t) Unable to initialize the POA.\n")),
                          1);

      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (poa_object.in ());
      PortableServer::POAManager_var const poa_manager =
        root_poa->the_POAManager ();

      CORBA::PolicyList policies (1);
      policies.length (1);

      CORBA::Any policy_value;
      policy_value <<= BiDirPolicy::BOTH;
      policies[0] =
        orb->create_policy (BiDirPolicy::BIDIRECTIONAL_POLICY_TYPE,
                            policy_value);

      PortableServer::POA_var const child_poa =
        root_poa->create_POA ("childPOA",
                              poa_manager.in (),
                              policies);

      for (CORBA::ULong i {}; i < policies.length (); ++i)
        policies[i]->destroy ();

      poa_manager->activate ();

      if (parse_args (argc, argv, options) != 0)
        return 1;

      Simple_Server_i server_impl (orb.in (), options.iterations);
      PortableServer::ObjectId_var const id =
        PortableServer::string_to_ObjectId ("simple_server");
      child_poa->activate_object_with_id (id.in (), &server_impl);

      CORBA::Object_var const object = child_poa->id_to_reference (id.in ());
      CORBA::String_var const ior = orb->object_to_string (object.in ());

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) Server activated as <%C>\n"),
                  ior.in ()));

      if (options.ior_output_file != nullptr)
        {
          std::unique_ptr<FILE, int (*) (FILE *)> output_file (
            ACE_OS::fopen (options.ior_output_file, ACE_TEXT ("w")),
            ACE_OS::fclose);
          if (!output_file)
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("Cannot open output file for writing IOR: %s\n"),
                               options.ior_output_file),
                              1);
          ACE_OS::fprintf (output_file.get (), "%s", ior.in ());
        }

      int result {};
      while (result == 0)
        {
          CORBA::Boolean const pending = orb->work_pending ();
          if (pending)
            orb->perform_work ();

          result = server_impl.call_client ();
        }

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("event loop finished\n")));
      root_poa->destroy (true, true);
      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception (ACE_TEXT ("Caught exception:"));
      return 1;
    }

  return 0;
}
