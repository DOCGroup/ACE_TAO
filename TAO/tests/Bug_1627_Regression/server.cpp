//
// $Id$
//
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "Test_i.h"

#include "tao/IORTable/IORTable.h"

const char *object_key = 0;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "o:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        object_key = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "SERVER (%P): usage:  %s "
                           "-k <object key>"
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
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);

      CORBA::Object_var poa_object =
        orb->resolve_initial_references ("RootPOA" ACE_ENV_ARG_PARAMETER);

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "SERVER (%P): Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in () ACE_ENV_ARG_PARAMETER);

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      if (parse_args (argc, argv) != 0)
        return 1;

      Test_i server_impl;



      root_poa->activate_object (   &server_impl
                                    ACE_ENV_ARG_PARAMETER);

      CORBA::Object_var server = server_impl._this ();

      CORBA::String_var ior =
        orb->object_to_string (server.in () ACE_ENV_ARG_PARAMETER);

      CORBA::Object_var table_object =
        orb->resolve_initial_references ("IORTable" ACE_ENV_ARG_PARAMETER);

      IORTable::Table_var adapter =
        IORTable::Table::_narrow (table_object.in () ACE_ENV_ARG_PARAMETER);

      adapter->bind("Name-with-hyphens", ior.in());


      FILE *output_file= ACE_OS::fopen ("server.ior", "w");
      if (output_file == 0)
         ACE_ERROR_RETURN ((LM_ERROR,
                               "SERVER (%P): Cannot open output file "
                               "for writing IOR: %s",
                               "server.ior"),
                              1);
      ACE_OS::fprintf (output_file, "%s", ior.in ());
      ACE_OS::fclose (output_file);

      ACE_DEBUG ((LM_DEBUG,
                      "SERVER (%P): Activated as file://%s\n",
                      "server.ior"));

      poa_manager->activate();

      orb->run ();

      root_poa->destroy (1, 1 ACE_ENV_ARG_PARAMETER);
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "SERVER (%P): Caught exception:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
