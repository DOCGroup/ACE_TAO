#include "ace/Get_Opt.h"
#include "ace/SString.h"
#include "testC.h"

struct Client_Options
{
  ACE_TString ior {ACE_TEXT ("file://test.ior")};
  int niterations {5};
  bool do_shutdown {};
};

int
parse_args (int argc, ACE_TCHAR *argv[], Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("xk:i:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'x':
        options.do_shutdown = true;
        break;

      case 'k':
        options.ior = get_opts.opt_arg ();
        break;

      case 'i':
        options.niterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage: %s -k <ior> -i <niterations> -x\n"),
                           argv[0]),
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
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv, options) != 0)
        return 1;

      CORBA::Object_var const object = orb->string_to_object (options.ior.c_str ());

      Simple_Server_var const server =
        Simple_Server::_narrow (object.in ());

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Object reference <%s> is nil.\n"),
                             options.ior.c_str ()),
                            1);
        }

      for (int i {}; i != options.niterations; ++i)
        {
          CORBA::Long const r =
            server->test_method (i);

          if (r != i)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "(%P|%t) unexpected result = %d for %d\n",
                          r, i));
            }
        }

      if (options.do_shutdown)
        {
          server->shutdown ();
        }
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
