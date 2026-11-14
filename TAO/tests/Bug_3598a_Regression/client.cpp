#include "TestC.h"
#include "ace/Get_Opt.h"
#include "tao/PortableInterceptorC.h"
#include "ClientORBInitializer.h"
#include "tao/ORBInitializer_Registry.h"
#include "tao/Strategies/advanced_resource.h"
#include "tao/Strategies/OC_Endpoint_Selector_Loader.h"
#include <string>

int
parse_args (int argc, ACE_TCHAR *argv[], std::basic_string<ACE_TCHAR> &ior)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:"));
  int option {};

  while ((option = get_opts ()) != -1)
    switch (option)
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
  int retval {};
  std::basic_string<ACE_TCHAR> ior {ACE_TEXT ("file://test.ior")};
  try
    {
      PortableInterceptor::ORBInitializer_ptr temp_orb_initializer {};
      PortableInterceptor::ORBInitializer_var orb_initializer;

      // Register the ClientRequest_Interceptor ORBInitializer.
      ACE_NEW_RETURN (temp_orb_initializer,
                      ClientORBInitializer,
                      -1);

      // The _var takes ownership of the newly allocated initializer.
      orb_initializer = temp_orb_initializer;

      PortableInterceptor::register_orb_initializer (orb_initializer.in ());

      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv, ior) != 0)
        return 1;

      CORBA::Object_var const tmp = orb->string_to_object (ior.c_str ());

      Test::Hello_var const hello = Test::Hello::_narrow (tmp.in ());

      if (CORBA::is_nil (hello.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Test::Hello reference <%s>\n",
                             ior),
                            1);
        }

      ACE_DEBUG ((LM_DEBUG, "Client about to make method call that is doomed to failure...\n"));

      bool call_failed {};
      try
      {
        CORBA::String_var the_string =
          hello->get_string ();
      }
      catch (CORBA::Exception const &)
      {
        call_failed = true;
      }

      if (!call_failed)
        {
          ACE_ERROR ((LM_ERROR,
                     "Error - the remote call succeeded which is bloody miraculous given that no server is running !!\n"));
          ++retval;
        }

      if (ClientRequest_Interceptor::success_flag_)
        {
          ACE_DEBUG ((LM_DEBUG, "Success - the server was unreachable and PI receive_exception was invoked.\n"));
        }
      else
        {
          ACE_ERROR ((LM_DEBUG,
                     "Error: regression failed - interceptor receive_exception interception point was not invoked !!\n"));
          ++retval;
        }

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      ++retval;
    }

  return retval;
}
