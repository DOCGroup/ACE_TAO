#include "TestC.h"
#include "ace/Get_Opt.h"
#include "ace/SString.h"
#include "tao/PortableInterceptorC.h"
#include "ClientORBInitializer.h"
#include "tao/ORBInitializer_Registry.h"
#include "orbsvcs/FaultTolerance/FT_ClientService_Activate.h"

struct Client_Options
{
  ACE_TString ior { ACE_TEXT ("file://test.ior") };
};

int
parse_args (int argc, ACE_TCHAR *argv[], Client_Options &options)
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
                             ACE_TEXT ("usage: %s -k <ior>\n"),
                             argv[0]),
                            -1);
        }
    }
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      PortableInterceptor::ORBInitializer_ptr temp_orb_initializer {};
      PortableInterceptor::ORBInitializer_var orb_initializer;

      // Register the ClientRequest_Interceptor ORBInitializer.
      ACE_NEW_RETURN (temp_orb_initializer,
                      ClientORBInitializer,
                      -1);

      orb_initializer = temp_orb_initializer;

      PortableInterceptor::register_orb_initializer (orb_initializer.in ());

      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

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

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Client about to make method call that is doomed to failure...\n")));

      CORBA::String_var const the_string = hello->get_string ();
      ACE_UNUSED_ARG (the_string);

      ACE_ERROR_RETURN ((LM_DEBUG,
                         ACE_TEXT ("Error - the remote call succeeded which is bloody "
                                   "miraculous given that no server is running !!\n")),
                        1);
    }
  catch (CORBA::Exception const &)
    {
      if (ClientRequest_Interceptor::success_flag_)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("Success - the server was unreachable and PI receive_exception was invoked.\n")));
          return 0;
        }
      else
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             ACE_TEXT ("Error: regression failed - interceptor "
                                       "receive_exception interception point was not invoked !!\n")),
                            1);
        }
    }

  return 1;
}
