// -*- C++ -*-
#include "ace/Get_Opt.h"
#include "testC.h"
#include "Client_ORBInitializer.h"
#include "tao/ORBInitializer_Registry.h"
#include "ace/SString.h"

struct Options
{
  ACE_TString primary_ior;
  ACE_TString first_forward_ior;
  ACE_TString second_forward_ior;
};

int
parse_args (int argc, ACE_TCHAR *argv[], Options &options)
{
  if (argc != 7)  // foo -p IOR_1 -f IOR_2 -s IOR_3
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Wrong number of arguments.\n"),
                      -1);

  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("p:f:s:"));
  int c {};

  while ((c = get_opts ()) != -1)
    {
      switch (c)
        {
        case 'p':
          options.primary_ior = get_opts.opt_arg ();
          break;
        case 'f':
          options.first_forward_ior = get_opts.opt_arg ();
          break;
        case 's':
          options.second_forward_ior = get_opts.opt_arg ();
          break;
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Usage:  %s "
                             "-p IOR_1 -f IOR_2 -s IOR_3\n",
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
      PortableInterceptor::ORBInitializer_ptr temp_initializer {};

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
        orb->string_to_object (options.primary_ior.c_str ());

      RedirectionTest::test_var const server =
        RedirectionTest::test::_narrow (object.in ());

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference <%s> is nil.\n",
                             options.primary_ior.c_str ()),
                            1);
        }

      for (int i = 1; i <= 9; ++i)
        {
          ACE_DEBUG ((LM_INFO,
                      "CLIENT: Issuing request %d.\n",
                      i));

          CORBA::Short const number = server->number ();

          ACE_DEBUG ((LM_INFO,
                      "CLIENT: Request %d handled by object %d.\n",
                      i,
                      number));
        }

      server->shutdown ();

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
