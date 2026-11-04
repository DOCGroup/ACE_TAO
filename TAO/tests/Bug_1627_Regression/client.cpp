#include "TestC.h"
#include "ace/Get_Opt.h"
  ACE_TCHAR const *ior = ACE_TEXT("corbaloc:iiop:localhost:12345/Name\\2dwith\\2dhyphens");

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

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var const tmp =
        orb->string_to_object (ior);

      Test_var const server =
        Test::_narrow (tmp.in ());

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Test::Server reference <%s>\n",
                             ior),
                            1);
        }
      server->test_method ();
      orb->destroy ();
    }
  catch (CORBA::Exception const &)
    {
      return 1;
    }

  return 0;
}
