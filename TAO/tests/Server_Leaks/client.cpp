#include "TestC.h"
#include "ace/Get_Opt.h"
#include <string>

struct Client_Options
{
  std::basic_string<ACE_TCHAR> ior {ACE_TEXT ("file://test.ior")};
  bool do_shutdown {};
};

int
parse_args (int argc, ACE_TCHAR *argv[], Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:x"));
  int option {};

  while ((option = get_opts ()) != -1)
    switch (option)
      {
      case 'k':
        options.ior = get_opts.opt_arg ();
        break;

      case 'x':
        options.do_shutdown = true;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage:  %s -k <ior> -x\n"),
                           argv [0]),
                          -1);
      }
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  Client_Options options;
  try
    {
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv, options) != 0)
        return 1;

      CORBA::Object_var const tmp = orb->string_to_object (options.ior.c_str ());

      Test::Ping_Service_var const ping_service =
        Test::Ping_Service::_narrow (tmp.in ());

      if (CORBA::is_nil (ping_service.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             ACE_TEXT ("Nil process reference <%s>\n"),
                             options.ior.c_str ()),
                            1);
        }

      for (int iteration {}; iteration != 10; ++iteration)
        {
          ping_service->ping ();
        }

      if (options.do_shutdown)
        {
          ping_service->shutdown ();
        }

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
