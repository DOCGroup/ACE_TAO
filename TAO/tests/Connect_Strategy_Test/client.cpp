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
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:x:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        options.ior = get_opts.opt_arg ();
        break;
      case 'x':
        options.do_shutdown = ACE_OS::atoi (get_opts.opt_arg ()) != 0;
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage:  %s -k <ior> -x <do_shutdown>\n"),
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
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      Client_Options options;
      if (parse_args (argc, argv, options) != 0)
        return 1;

      CORBA::Object_var const tmp =
        orb->string_to_object (options.ior.c_str ());

      Test::Hello_var const hello =
        Test::Hello::_narrow (tmp.in ());

      if (CORBA::is_nil (hello.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             ACE_TEXT ("Nil Test::Hello reference <%s>\n"),
                             options.ior.c_str ()),
                            1);
        }

      CORBA::String_var const the_string =
        hello->get_string ();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) - string returned <%C>\n",
                  the_string.in ()));

      if (options.do_shutdown)
        {
          ACE_DEBUG ((LM_DEBUG, "(%P|%t) - Going to shutdown server\n"));
          hello->shutdown ();
        }

      orb->destroy ();
    }
  catch (CORBA::Exception const& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
