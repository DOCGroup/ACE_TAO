// $Id$

#include "ace/Get_Opt.h"
#include "tao/IORTable/IORTable.h"
#include "test_i.h"
#include "Server_Worker.h"

const char *ior_output_file = 0;
char *another_output_file = 0;
const char *ior_table_name = 0;
char *another_table_name = 0;
int nthreads = 4;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "i:o:n:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'i':
        {
          ior_table_name = get_opts.opt_arg ();
          int len = ACE_OS::strlen(ior_table_name) + 1;
          another_table_name = new char[len + 1];
          ACE_OS::strcpy(another_table_name, ior_table_name);
          another_table_name[len-1] = '1';
          another_table_name[len] = '\0';
          break;
        }
      case 'o':
        {
          ior_output_file = get_opts.opt_arg ();
          int len = ACE_OS::strlen(ior_output_file) + 1;
          another_output_file = new char[len + 1];
          ACE_OS::strcpy(another_output_file, ior_output_file);
          another_output_file[len-1] = '1';
          another_output_file[len] = '\0';
          break;
        }
      case 'n':
        nthreads = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "[-i <iortable name>]"
                           "-o <iorfile>"
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
      // Initialize the ORB
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                           1);

      // Get a Root POA reference
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;

      Simple_Server_i server_impl (orb.in ());
      Another_One_i another_one_impl (orb.in());

      Simple_Server_var server =
        server_impl._this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Another_One_var another_one =
        another_one_impl._this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::String_var ior =
        orb->object_to_string (server.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::String_var another_ior =
        orb->object_to_string (another_one.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "Activated as <%s>\n", ior.in ()));

      ACE_DEBUG ((LM_DEBUG, "Activated another one as <%s>\n", another_ior.in ()));

      if (ior_table_name != 0)
        {
          CORBA::Object_var table_object =
             orb->resolve_initial_references ("IORTable" ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (-1);

          IORTable::Table_var adapter =
             IORTable::Table::_narrow (table_object.in () ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (-1);

          if (CORBA::is_nil (adapter.in ()))
            {
              ACE_ERROR ((LM_ERROR, "Nil IORTable\n"));
              return -1;
            }

          adapter->bind ( ior_table_name, ior.in () ACE_ENV_ARG_PARAMETER);
          adapter->bind ( another_table_name, another_ior.in() ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (-1);
        }


      // If the ior_output_file exists, output the ior to it
      if (ior_output_file != 0)
        {
          FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
          if (output_file == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Cannot open output file for writing IOR: %s",
                               ior_output_file),
                              1);
          ACE_DEBUG ((LM_DEBUG,"Wrote ior to %s\n",
                      ior_output_file));
          ACE_OS::fprintf (output_file, "%s", ior.in ());
          ACE_OS::fclose (output_file);
          output_file = ACE_OS::fopen (another_output_file, "w");
          if (output_file == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Cannot open output file for writing IOR: %s",
                               another_output_file),
                              1);
          ACE_DEBUG ((LM_DEBUG,"Wrote another ior to %s\n",
                      another_output_file));
          ACE_OS::fprintf (output_file, "%s", another_ior.in ());
          ACE_OS::fclose (output_file);
        }

      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Server_Worker worker (orb.in ());
      if (worker.activate (THR_NEW_LWP | THR_JOINABLE,
                           nthreads) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot activate client threads\n"),
                           1);

      worker.thr_mgr ()->wait ();

      ACE_DEBUG ((LM_DEBUG, "event loop finished\n"));
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception caught:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
