#include "ace/Get_Opt.h"
#include "ace/SString.h"
#include "Client_Worker.h"

struct Client_Options
{
  ACE_TString ior {ACE_TEXT ("file://test.ior")};
  int niterations {5};
  int nthreads {5};
  bool do_shutdown {};
};

int
parse_args (int argc, ACE_TCHAR *argv[], Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:n:i:x"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        options.ior = get_opts.opt_arg ();
        break;

      case 'n':
        options.nthreads = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'i':
        options.niterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'x':
        options.do_shutdown = true;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage: %s -k <ior> -n <nthreads> -i <niterations> [-x]\n"),
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  Client_Options options;
  try
    {
      // Initialize the ORB
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv, options) != 0)
        return 1;

      if (options.ior.empty ())
        ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("The IOR must not be empty.\n")), 1);

      // Get Object Reference using IOR file
      CORBA::Object_var object =
        orb->string_to_object (options.ior.c_str ());

      // Cast to Appropriate Type
      Simple_Server_var server =
        Simple_Server::_narrow (object.in ());

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Object reference <%s> is nil\n"),
                             options.ior.c_str ()),
                              1);
        }

      Client_Worker client (server.in (), options.niterations);

      if (client.activate (THR_NEW_LWP | THR_JOINABLE,
                           options.nthreads) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Cannot Activate Client Threads\n"),
                          1);

      client.thr_mgr ()->wait ();

      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) threads finished\n"));

      // Shut down the server if -x option given in command line
      if (options.do_shutdown)
        {
          server->shutdown ();
        }

      // Destroying the ORB..
      orb->destroy ();
    }

  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }
  return 0;
}
