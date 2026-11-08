#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "tao/IORTable/IORTable.h"
#include "test_i.h"
#include "Server_Worker.h"

#include <memory>
#include <string>

struct Server_Options
{
  ACE_TString ior_output_file {};
  ACE_TString another_output_file {};
  std::string ior_table_name;
  std::string another_table_name;
  int nthreads {4};
};

static int
write_ior (ACE_TCHAR const *filename, char const *ior)
{
  std::unique_ptr<FILE, int (*) (FILE *)> output_file (
    ACE_OS::fopen (filename, ACE_TEXT ("w")), ACE_OS::fclose);
  if (!output_file)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Cannot open output file for writing IOR: %s"),
                       filename),
                      -1);
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Wrote IOR to %s\n"), filename));
  ACE_OS::fprintf (output_file.get (), "%s", ior);
  return 0;
}

int
parse_args (int argc, ACE_TCHAR *argv[], Server_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("i:o:n:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'i':
        {
          options.ior_table_name = ACE_TEXT_ALWAYS_CHAR (get_opts.opt_arg ());
          options.another_table_name = options.ior_table_name;
          if (!options.another_table_name.empty ())
            options.another_table_name.back () = '1';
          break;
        }
      case 'o':
        {
          options.ior_output_file = get_opts.opt_arg ();
          options.another_output_file = options.ior_output_file;
          if (!options.another_output_file.empty ())
            options.another_output_file[options.another_output_file.length () - 1] = '1';
          break;
        }
      case 'n':
        options.nthreads = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage: %s [-i <iortable name>] -o <iorfile> -n <nthreads>\n"),
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  Server_Options options;
  try
    {
      // Initialize the ORB
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var poa_object =
        orb->resolve_initial_references ("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                           1);

      // Get a Root POA reference
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      if (parse_args (argc, argv, options) != 0)
        return 1;

      Simple_Server_i server_impl (orb.in ());
      Another_One_i another_one_impl (orb.in ());

      Simple_Server_var server =
        server_impl._this ();

      Another_One_var another_one =
        another_one_impl._this ();

      CORBA::String_var ior =
        orb->object_to_string (server.in ());

      CORBA::String_var another_ior =
        orb->object_to_string (another_one.in ());

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Activated as <%C>\n"), ior.in ()));

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Activated another one as <%C>\n"),
                  another_ior.in ()));

      if (!options.ior_table_name.empty ())
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

          adapter->bind (options.ior_table_name.c_str (), ior.in ());
          adapter->bind (options.another_table_name.c_str (), another_ior.in ());
        }

      // If the ior_output_file exists, output the ior to it
      if (!options.ior_output_file.empty ())
        {
          if (write_ior (options.ior_output_file.c_str (), ior.in ()) != 0 ||
              write_ior (options.another_output_file.c_str (), another_ior.in ()) != 0)
            return 1;
        }

      poa_manager->activate ();

      Server_Worker worker (orb.in ());
      if (worker.activate (THR_NEW_LWP | THR_JOINABLE,
                           options.nthreads) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot activate client threads\n"),
                           1);

      worker.thr_mgr ()->wait ();

      ACE_DEBUG ((LM_DEBUG, "event loop finished\n"));

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
