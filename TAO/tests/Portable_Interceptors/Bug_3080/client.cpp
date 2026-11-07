// -*- C++ -*-
#include "ace/Get_Opt.h"
#include "testC.h"
#include "Client_ORBInitializer.h"
#include "Client_Request_Interceptor.h"
#include "tao/ORBInitializer_Registry.h"
#include "ace/SString.h"

struct Options
{
  ACE_TString ior;
};

int
parse_args (int argc, ACE_TCHAR *argv[], Options &options)
{
  if (argc != 3)  // foo -p IOR_1 -s IOR_2
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Wrong number of arguments.\n"),
                      -1);

  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("f:"));
  int c {};

  while ((c = get_opts ()) != -1)
    {
      switch (c)
        {
        case 'f':
          options.ior = get_opts.opt_arg ();
          break;
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Usage:  %s "
                             "-f <IOR>\n",
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
#if TAO_HAS_INTERCEPTORS == 1
      Client_ORBInitializer *temp_initializer {};

      ACE_NEW_RETURN (temp_initializer,
                      Client_ORBInitializer,
                      -1);
      PortableInterceptor::ORBInitializer_var const orb_initializer (temp_initializer);

      PortableInterceptor::register_orb_initializer (orb_initializer.in ());
#endif  /* TAO_HAS_INTERCEPTORS == 1 */

      CORBA::ORB_var const orb = CORBA::ORB_init (argc,
                                                  argv,
                                                  "Client ORB");

      Options options {};
      if (::parse_args (argc, argv, options) != 0)
        return -1;

      CORBA::Object_var const object =
        orb->string_to_object (options.ior.c_str ());

      RedirectionTest::test_var const server =
        RedirectionTest::test::_narrow (object.in ());

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference <%s> is nil.\n",
                             options.ior.c_str ()),
                            1);
        }

      try
        {
          (void) server->number ();
        }
      catch (CORBA::TRANSIENT const &)
        {
          // ignore
        }

#if TAO_HAS_INTERCEPTORS == 1
      if (temp_initializer->client_interceptor_->request_count () != 1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Error, send request interceptor not called\n"),
                            1);
        }

      if (temp_initializer->client_interceptor_->receive_exception_count () != 1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Error, receive exception interceptor not called\n"),
                            1);
        }
#endif  /* TAO_HAS_INTERCEPTORS == 1 */

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return -1;
    }

  ACE_DEBUG ((LM_INFO,
              "PortableInterceptor::Redirection test passed.\n"));

  return 0;
}
