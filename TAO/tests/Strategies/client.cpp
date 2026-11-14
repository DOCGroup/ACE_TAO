#include "ace/Get_Opt.h"
#include "simple_testC.h"

struct Client_Options
{
  ACE_TCHAR const *ior {ACE_TEXT ("file://test.ior")};
  bool server_shutdown {};
};

static int
parse_args (int argc, ACE_TCHAR *argv[], Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:x"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        options.ior = get_opts.opt_arg ();
        break;
      case 'x':
        options.server_shutdown = true;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage:  %s "
                                     "-k <ior> "
                                     "\n"),
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
      Client_Options options {};
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv, options) != 0)
        return 1;

      CORBA::Object_var const object =
        orb->string_to_object (options.ior);

      Simple_Server_var const server =
        Simple_Server::_narrow (object.in ());

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Object reference <%s> is nil.\n"),
                             options.ior),
                            1);
        }

      // Invoke a request on the server
      if (!server->print_status ())
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("The server has been contacted !!\n")));
        }

      if (options.server_shutdown)
        {
          server->shutdown ();
        }

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return 1;
    }

  return 0;
}
