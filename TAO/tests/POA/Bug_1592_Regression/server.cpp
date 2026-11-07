// -*- C++ -*-
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "test_i.h"
#include "ServantLocator.h"
#include "Server_ORBInitializer.h"
#include "tao/ORBInitializer_Registry.h"

#include <cstdio>
#include <cstring>
#include <memory>

int
parse_args (int argc, ACE_TCHAR *argv[], ACE_TCHAR const *&ior_file)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:"));

  int option {};

  while ((option = get_opts ()) != -1)
    switch (option)
      {
      case 'o':
        ior_file = get_opts.opt_arg ();
        break;

      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Usage:  %s "
                           "-o IOR\n",
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
      Server_ORBInitializer *temp_initializer {};
      ACE_NEW_RETURN (temp_initializer,
                      Server_ORBInitializer,
                      -1);  // No exceptions yet!
      PortableInterceptor::ORBInitializer_var orb_initializer =
        temp_initializer;

      PortableInterceptor::register_orb_initializer (orb_initializer.in ());

      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "Server ORB");

      ACE_TCHAR const *ior_file {};
      if (parse_args (argc, argv, ior_file) != 0)
        return -1;

      CORBA::Object_var poa_object =
        orb->resolve_initial_references ("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      // Create a child POA configured to use a ServantLocator.
      CORBA::PolicyList policies (2);

      policies.length (2);
      policies[0] =
        root_poa->create_request_processing_policy (
          PortableServer::USE_SERVANT_MANAGER);

      policies[1] =
        root_poa->create_servant_retention_policy (PortableServer::NON_RETAIN);

      PortableServer::POA_var child_poa =
        root_poa->create_POA ("child",
                              poa_manager.in (),
                              policies);

      ServantLocator *sl {};
      ACE_NEW_THROW_EX (sl,
                        ServantLocator (orb.in ()),
                        CORBA::NO_MEMORY ());

      PortableServer::ServantLocator_var servant_locator = sl;

      child_poa->set_servant_manager (servant_locator.in ());

      poa_manager->activate ();

      CORBA::Object_var obj =
        child_poa->create_reference ("IDL:test:1.0");

      CORBA::String_var ior = orb->object_to_string (obj.in ());

      // Write IOR to a file and close it before waiting for client requests.
      {
        std::unique_ptr<FILE, int (*) (FILE *)> output_file (
          ACE_OS::fopen (ior_file, "w"), ACE_OS::fclose);
        if (!output_file)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Cannot open output file <%s> for writing "
                             "IOR:\n%s\n",
                             ior_file,
                             ior.in ()),
                            1);
        int const result = std::fprintf (output_file.get (), "%s", ior.in ());
        if (result < 0 || static_cast<size_t> (result) != std::strlen (ior.in ()))
          ACE_ERROR_RETURN ((LM_ERROR, "Failed writing IOR to <%s>\n", ior_file), 1);
      }

      // Run the ORB event loop.
      orb->run ();

      root_poa->destroy (true, true);

      orb->destroy ();

      if (!sl->error_status_)
      {
        ACE_DEBUG ((LM_DEBUG,
                    "PortableInterceptor / ServantLocator test passed.\n"));
      }
      else
      {
        ACE_ERROR ((LM_ERROR,
                    "PortableInterceptor / ServantLocator test failed.\n"));
        return 1;
      }
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return -1;
    }

  return 0;
}
