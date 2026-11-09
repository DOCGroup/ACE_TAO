#include "ace/OS_NS_stdio.h"
#include "ace/Get_Opt.h"
#include "TestC.h"

struct Client_Options
{
  ACE_TCHAR const *ior_server {};
};

int
parse_args (int argc, ACE_TCHAR *argv[], Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        options.ior_server = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k file://<iorfile>"
                           "\n",
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
      Client_Options options;
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv, options) != 0)
        ACE_ERROR_RETURN ((LM_ERROR, "Wrong arguments\n"), -1);

      CORBA::Object_var const tmp = orb->string_to_object (options.ior_server);

      Test::Server_var const server =
        Test::Server::_unchecked_narrow (tmp.in ());

      if (CORBA::is_nil (server.in ()))
        ACE_ERROR_RETURN ((LM_ERROR, "Nil reference\n"), -1);

      server->shutdown ();

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception in client.cpp:\n");
      return -1;
    }

  return 0;
}
