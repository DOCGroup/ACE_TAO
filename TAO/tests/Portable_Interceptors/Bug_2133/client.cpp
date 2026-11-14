#include "TestC.h"
#include "ace/Get_Opt.h"
#include "tao/PortableInterceptorC.h"
#include "ClientORBInitializer.h"
#include "tao/ORBInitializer_Registry.h"
#include "ace/SString.h"

struct Options
{
  ACE_TString ior { ACE_TEXT ("file://test.ior") };
};

int
parse_args (int argc, ACE_TCHAR *argv[], Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:"));
  int c {};

  while ((c = get_opts ()) != -1)
    {
      switch (c)
        {
        case 'k':
          options.ior = get_opts.opt_arg ();
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
    }
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  CORBA::ORB_var orb {};

  try
    {
      PortableInterceptor::ORBInitializer_ptr temp_orb_initializer {};
      PortableInterceptor::ORBInitializer_var orb_initializer {};
      Options options;

      // Register the ClientRequest_Interceptor ORBInitializer.
      ACE_NEW_RETURN (temp_orb_initializer,
                      ClientORBInitializer,
                      -1);

      orb_initializer = temp_orb_initializer;

      PortableInterceptor::register_orb_initializer (orb_initializer.in ());

      orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv, options) != 0)
        return 1;

      CORBA::Object_var tmp =
        orb->string_to_object (options.ior.c_str ());

      Test::Hello_var hello =
        Test::Hello::_narrow (tmp.in ());

      if (CORBA::is_nil (hello.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Test::Hello reference <%s>\n",
                             options.ior.c_str ()),
                            1);
        }

      ACE_DEBUG ((LM_DEBUG,
                  "Client about to make method call that is doomed to failure...\n"));

      CORBA::String_var const the_string =
        hello->get_string ();

      ACE_ERROR_RETURN ((LM_DEBUG,
                         "Error - the remote call succeeded which is bloody "
                         "miraculous given that no server is running !!\n"),
                        1);

      orb->destroy ();
    }
  catch (CORBA::Exception const &)
    {
      orb->destroy ();

      if (ClientRequest_Interceptor::success_flag_)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Success - the server was unreachable and PI "
                      "receive_exception was invoked.\n"));
          return 0;
        }
      else
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Error: regression failed - interceptor "
                             "receive_exception interception point was not invoked !!\n"),
                            1);
        }
    }

  return 1;
}
