#include "TestC.h"
#include "ace/Get_Opt.h"

#include <string>
std::basic_string<ACE_TCHAR> ior = ACE_TEXT ("file://test.ior");

int
parse_args (int argc, ACE_TCHAR *argv[]);

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  ACE_DEBUG ((LM_DEBUG,
             "Starting shutdown\n"));
  try
    {
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var const tmp =
        orb->string_to_object (ior.c_str ());

      Test::Echo_Caller_var const server =
        Test::Echo_Caller::_narrow (tmp.in ());

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Test::Echo_Caller reference <%s>\n",
                             ior.c_str ()),
                            1);
        }

      server->shutdown ();

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}
