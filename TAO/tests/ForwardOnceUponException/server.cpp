#include "test_i.h"
#include "tao/IORTable/IORTable.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "tao/Invocation_Utils.h"
#include <memory>

struct Server_Options
{
  ACE_TString ior_output_file {ACE_TEXT ("test.ior")};
  int expect_servant_calls {1};
  int raise_exception {TAO::FOE_NON};
  int num_requests {1};
};

int
parse_args (int argc, ACE_TCHAR *argv[], Server_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:e:r:i:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        options.ior_output_file = get_opts.opt_arg ();
        break;
      case 'e':
        options.raise_exception = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 'r':
        options.expect_servant_calls = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 'i':
        options.num_requests = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <iorfile> -e <raise_exception> -r <expect_servant_calls>"
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
      Server_Options options;
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var const poa_object =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (poa_object.in ());
      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POAManager_var const poa_manager =
        root_poa->the_POAManager ();

      if (parse_args (argc, argv, options) != 0)
        return 1;

      Simple_Server_i server_impl (orb.in (), options.raise_exception);

      PortableServer::ObjectId_var const id =
        root_poa->activate_object (&server_impl);

      CORBA::Object_var const object = root_poa->id_to_reference (id.in ());

      Simple_Server_var const server =
        Simple_Server::_narrow (object.in ());

      CORBA::String_var const ior =
        orb->object_to_string (server.in ());

      CORBA::Object_var const table_object =
        orb->resolve_initial_references ("IORTable");

      IORTable::Table_var const table =
        IORTable::Table::_narrow (table_object.in ());
      if (CORBA::is_nil (table.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the IORTable.\n"),
                          1);
      table->bind ("Simple_Server", ior.in ());

      //ACE_DEBUG ((LM_DEBUG, "Activated as <%C>\n", ior.in ()));

      {
        std::unique_ptr<FILE, int (*) (FILE *)> output_file (
          ACE_OS::fopen (options.ior_output_file.c_str (), ACE_TEXT ("w")),
          ACE_OS::fclose);
        if (!output_file)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Cannot open output file for writing IOR: %s",
                             ACE_TEXT_ALWAYS_CHAR (options.ior_output_file.c_str ())),
                            1);
        ACE_OS::fprintf (output_file.get (), "%s", ior.in ());
      }

      poa_manager->activate ();

      orb->run ();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t)server: event loop finished\n"));

      // If expect_servant_calls is 0, this is the request-forwarding loop case. The
      // servant should receive more calls than there were requests.
      if ((options.expect_servant_calls == 0 &&
           (server_impl.ncalls () > options.num_requests))
          || (options.expect_servant_calls > 0 &&
              (server_impl.ncalls () == options.expect_servant_calls)))
      {
        return 0;
      }
      else
        ACE_ERROR_RETURN ((LM_ERROR,
                           "server: Test failed - expected %d servant calls but got %d calls \n",
                           options.expect_servant_calls, server_impl.ncalls ()),
                          1);
     }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
