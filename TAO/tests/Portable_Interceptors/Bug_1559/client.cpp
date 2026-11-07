#include "ace/Get_Opt.h"
#include "testC.h"
#include "Client_ORBInitializer.h"
#include "tao/ORBInitializer_Registry.h"
#include "ace/SString.h"

struct Options
{
  ACE_TString ior { ACE_TEXT ("file://test.ior") };
};

int
parse_args (int argc, ACE_TCHAR *argv[], Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("ef:"));
  int c {};

  while ((c = get_opts ()) != -1)
    {
      switch (c)
        {
        case 'e':
          break;
        case 'f':
          options.ior = get_opts.opt_arg ();
          break;
        case '?':
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             "usage:  %s "
                             "-v "
                             "\n",
                             argv [0]),
                            -1);
        }
    }
  return 0;
}

void
run_test (Test_Interceptors::Visual_ptr server)
{
  server->normal (10);

  ACE_DEBUG ((LM_DEBUG, "\"normal\" operation done\n"));

  server->nothing ();

  ACE_DEBUG ((LM_DEBUG, "\"nothing\" operation done\n"));

  try
    {
      server->user ();
    }
  catch (Test_Interceptors::Silly const &)
    {
      ACE_DEBUG ((LM_DEBUG, "Caught Silly\n"));
    }

  try
    {
      server->system ();
    }
  catch (CORBA::INV_OBJREF const &)
    {
      ACE_DEBUG ((LM_DEBUG, "Caught CORBA::INV_OBJREF\n"));
    }
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      PortableInterceptor::ORBInitializer_ptr temp_initializer {};
      Options options;

      ACE_NEW_RETURN (temp_initializer,
                      Client_ORBInitializer,
                      -1);  // No exceptions yet!
      PortableInterceptor::ORBInitializer_var initializer =
        temp_initializer;

      PortableInterceptor::register_orb_initializer (initializer.in ());

      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv, options) != 0)
        return 1;

      CORBA::Object_var object =
        orb->string_to_object (options.ior.c_str ());

      Test_Interceptors::Visual_var server =
        Test_Interceptors::Visual::_narrow (object.in ());

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference <%s> is nil.\n",
                             options.ior.c_str ()),
                            1);
        }

      run_test (server.in ());

      server->shutdown ();

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Caught exception in client:");
      return 1;
    }

  return 0;
}
