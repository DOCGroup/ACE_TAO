// $Id$

#include "ace/Get_Opt.h"
#include "tao/IORTable/IORTable.h"
#include "test_i.h"
#include "Server_Worker.h"

const ACE_TCHAR *ior_output_file = 0;
ACE_TCHAR *another_output_file = 0;
ACE_CString ior_table_name = "";
char *another_table_name = 0;
int nthreads = 4;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("i:o:n:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'i':
        {
          ior_table_name = ACE_TEXT_ALWAYS_CHAR(get_opts.opt_arg ());
          int len = ACE_OS::strlen(ior_table_name.c_str ()) + 1;
          another_table_name = new char[len + 1];
          ACE_OS::strcpy(another_table_name, ior_table_name.c_str ());
          another_table_name[len-1] = '1';
          another_table_name[len] = '\0';
          break;
        }
      case 'o':
        {
          ior_output_file = get_opts.opt_arg ();
          int len = ACE_OS::strlen(ior_output_file) + 1;
          another_output_file = new ACE_TCHAR[len + 1];
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
  // Indicates successful parsing of the command line
  return 0;
}


int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      // Initialize the ORB
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                           1);

      // Get a Root POA reference
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      if (parse_args (argc, argv) != 0)
        return 1;

      Simple_Server_i server_impl (orb.in ());
      Another_One_i another_one_impl (orb.in());

      Simple_Server_var server =
        server_impl._this ();

      Another_One_var another_one =
        another_one_impl._this ();

      CORBA::String_var ior =
        orb->object_to_string (server.in ());

      CORBA::String_var another_ior =
        orb->object_to_string (another_one.in ());

      ACE_DEBUG ((LM_DEBUG, "Activated as <%C>\n", ior.in ()));

      ACE_DEBUG ((LM_DEBUG, "Activated another one as <%s>\n", another_ior.in ()));

      if (ior_table_name.length () != 0)
        {
          CORBA::Object_var table_object =
             orb->resolve_initial_references ("IORTable");

          IORTable::Table_var adapter =
             IORTable::Table::_narrow (table_object.in ());

          if (CORBA::is_nil (adapter.in ()))
            {
              ACE_ERROR ((LM_ERROR, "Nil IORTable\n"));
              return -1;
            }

          adapter->bind ( ior_table_name.c_str (), ior.in ());
          adapter->bind ( another_table_name, another_ior.in());
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

      poa_manager->activate ();

      Server_Worker worker (orb.in ());
      if (worker.activate (THR_NEW_LWP | THR_JOINABLE,
                           nthreads) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot activate client threads\n"),
                           1);

      worker.thr_mgr ()->wait ();

      ACE_DEBUG ((LM_DEBUG, "event loop finished\n"));
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
