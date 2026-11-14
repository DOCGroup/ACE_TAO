#include "ace/OS_NS_unistd.h"
#include "ace/Get_Opt.h"
#include "testC.h"
#include "Smart_Proxy_Impl.h"

struct Options
{
  ACE_TCHAR const *ior {ACE_TEXT ("file://test.ior")};
};

bool dtor_called {};

static int
parse_args (Options &options, int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("i:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'i':
        options.ior = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-i "
                           "\n",
                           argv [0]),
                          -1);
      }
  return 0;
}

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      Options options;
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      if (parse_args (options, argc, argv) != 0)
        return 1;

      CORBA::Object_var obj = orb->string_to_object (options.ior);

      // Force a scope to observe destruction of the smart proxy.
      {
        Test_Smart_Factory *test_factory {};
        ACE_NEW_RETURN (test_factory,
                        Test_Smart_Factory,
                        -1);

        ACE_UNUSED_ARG (test_factory);

        Test_var server = Test::_narrow (obj.in ());

        if (CORBA::is_nil (server.in ()))
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference <%s> is nil.\n",
                             options.ior),
                            1);

        server->hello (3);

#if (TAO_HAS_MINIMUM_CORBA == 0)
        // Test the _non_existent function.
        ACE_DEBUG ((LM_DEBUG, "Testing _non_existent()\n"));
        CORBA::Boolean const ne = server->_non_existent ();
        if (ne)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Not a Messenger object reference\n"),
                            1);
        else
          ACE_DEBUG ((LM_DEBUG, "Successfully called _non_existent()\n"));
#endif /* TAO_HAS_MINIMUM_CORBA */

        server->shutdown ();

        // Allow the preceding oneway shutdown request to reach the server.
        ACE_Time_Value const tv (0, 100000);
        ACE_OS::sleep (tv);
      }
      // Leaving the scope above should destroy the smart proxy.
      if (!dtor_called)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "The Smart proxy is not deleted\n"),
                          1);

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Client-side exception:");
      return 1;
    }

  return 0;
}
