#include "TestC.h"
#include "ace/Get_Opt.h"

struct Client_Options
{
  // Borrowed from argv for the duration of main.
  ACE_TCHAR const *ior { ACE_TEXT ("file://test.ior") };
  bool shutdown_server {};
};

static int
parse_args (int argc, ACE_TCHAR *argv[], Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:s"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        options.ior = get_opts.opt_arg ();
        break;
      case 's':
        options.shutdown_server = true;
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage:  %s -k <ior> -s\n"),
                           argv [0]),
                          -1);
      }
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  CORBA::Long result {};
  try
    {
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      Client_Options options;
      if (parse_args (argc, argv, options) != 0)
        return 1;

      CORBA::Object_var const tmp =
        orb->string_to_object (options.ior);

      Test_var const server =
        Test::_narrow (tmp.in ());

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Nil Test::Server reference <%s>\n"),
                             options.ior),
                            1);
        }

      if (options.shutdown_server)
        {
          server->shutdown ();
        }
      else
        {
          result = server->try_and_create_POA ();
        }

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      result = 1;
    }

  return result;
}
