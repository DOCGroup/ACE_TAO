#include "Hello.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "tao/PortableServer/PortableServer.h"
#include "ServerORBInitializer2.h"
#include "ServerRequest_Interceptor2.h"
#include "tao/PI/PI.h"
#include "tao/ORBInitializer_Registry.h"
#include <cstdio>
#include <memory>

struct Server_Options
{
  ACE_TString ior_output_file;
  CORBA::ULong id_number {};
};

using File_Ptr = std::unique_ptr<FILE, int (*) (FILE *)>;

int
parse_args (int argc, ACE_TCHAR *argv[], Server_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("o:n:"));
  int c {};
  ACE_TString ior_file (ACE_TEXT ("n"));
  while ((c = get_opts ()) != -1)
    {
      switch (c)
        {
        case 'o':
          options.ior_output_file = get_opts.opt_arg ();
          break;
        case 'n':
          options.id_number = static_cast<CORBA::ULong> (ACE_OS::atoi (get_opts.opt_arg ()));
          ior_file += get_opts.opt_arg ();
          ior_file += ACE_TEXT (".ior");
          options.ior_output_file = ior_file;
          break;
        case '?':
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("usage: %s [-o <iorfile>] [-n <server_id>]\n"),
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
      Server_ORBInitializer2 *temp_initializer {};
      ACE_NEW_RETURN (temp_initializer,
                      Server_ORBInitializer2,
                      -1);  // No exceptions yet!
      // The ORBInitializer_var adopts the ACE_NEW_RETURN allocation.
      PortableInterceptor::ORBInitializer_var orb_initializer =
        temp_initializer;

      PortableInterceptor::register_orb_initializer (orb_initializer.in ());

      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var const poa_object =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT (" (%P|%t) Panic: nil RootPOA\n")),
                          -1);

      PortableServer::POAManager_var const poa_manager =
        root_poa->the_POAManager ();

      CORBA::PolicyList policies (2);
      policies.length (2);

      policies[0] =
        root_poa->create_id_assignment_policy (PortableServer::USER_ID);

      policies[1] =
        root_poa->create_lifespan_policy (PortableServer::PERSISTENT);

      PortableServer::POA_var const my_poa =
        root_poa->create_POA ("my_poa",
                              poa_manager.in (),
                              policies);

      // Creation of the new POA is over, so destroy the Policy_ptr's.
      for (CORBA::ULong i = 0; i < policies.length (); ++i)
        {
          CORBA::Policy_ptr const policy = policies[i];
          policy->destroy ();
        }

      Server_Options options;
      if (parse_args (argc, argv, options) != 0)
        return -1;

      Hello *hello_impl {};
      ACE_NEW_RETURN (hello_impl,
                      Hello (orb.in (), Test::Hello::_nil (), options.id_number),
                      -1);
      // ServantBase_var adopts the initial reference returned by ACE_NEW_RETURN.
      PortableServer::ServantBase_var owner (hello_impl);

      PortableServer::ObjectId_var const server_id =
        PortableServer::string_to_ObjectId ("server_id");

      my_poa->activate_object_with_id (server_id.in (),
                                       hello_impl);

      CORBA::Object_var const hello =
        my_poa->id_to_reference (server_id.in ());

      CORBA::String_var const ior =
        orb->object_to_string (hello.in ());

      {
        File_Ptr output_file (ACE_OS::fopen (options.ior_output_file.c_str (), ACE_TEXT ("w")),
                              std::fclose);
        if (!output_file)
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Cannot open output file for writing IOR: %s\n"),
                             options.ior_output_file.c_str ()),
                            -1);
        if (std::fprintf (output_file.get (), "%s", ior.in ()) < 0)
          ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Cannot write IOR to output file\n")), -1);
      }

      poa_manager->activate ();

      orb->run ();

      root_poa->destroy (true, true);

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return -1;
    }

  return 0;
}
