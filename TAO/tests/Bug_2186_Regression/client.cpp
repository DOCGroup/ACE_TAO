// $Id$

#include "Hello.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"

ACE_RCSID(Hello, client, "$Id$")

const char *server_ior = "file://server.ior";
const char *ior_output_file = "client.ior";

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "k:o:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        server_ior = get_opts.opt_arg ();
        break;
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA" ACE_ENV_ARG_PARAMETER);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in () ACE_ENV_ARG_PARAMETER);

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil RootPOA\n"),
                          1);

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      Hello *hello_impl;
      ACE_NEW_RETURN (hello_impl,
                      Hello (orb.in ()),
                      1);
      PortableServer::ServantBase_var owner_transfer(hello_impl);

      Test::Hello_var hello =
        hello_impl->_this ();

      CORBA::String_var ior =
        orb->object_to_string (hello.in () ACE_ENV_ARG_PARAMETER);

      // Output the IOR to the <ior_output_file>
      FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file for writing IOR: %s",
                           ior_output_file),
                              1);
      ACE_OS::fprintf (output_file, "%s", ior.in ());
      ACE_OS::fclose (output_file);

      poa_manager->activate ();

      CORBA::Object_var tmp =
        orb->string_to_object(server_ior ACE_ENV_ARG_PARAMETER);

      Test::Hello_var server =
        Test::Hello::_narrow(tmp.in () ACE_ENV_ARG_PARAMETER);

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Test::Hello reference <%s>\n",
                             ior.in ()),
                            1);
        }

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) - Sending client obj ref to the server and requesting a callback.\n"));

      server->request_callback (hello.in () ACE_ENV_ARG_PARAMETER);

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) - Test (client) Succeeded !!!\n"));

      orb->destroy ();
    }
  ACE_CATCHANY
    {
      ACE_DEBUG ((LM_ERROR, "(%P|%t) - Test (client) Failed !!!\n"));
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception caught:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
