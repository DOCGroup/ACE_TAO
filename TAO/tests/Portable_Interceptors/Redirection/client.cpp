// -*- C++ -*-
#include "ace/Get_Opt.h"
#include "ace/SString.h"
#include "testC.h"
#include "Client_ORBInitializer.h"
#include "tao/ORBInitializer_Registry.h"

struct Options
{
  ACE_TString primary_ior {};
  ACE_TString forward_ior {};
};

int
parse_args (int argc, ACE_TCHAR *argv[], Options &options)
{
  if (argc != 5)  // foo -p IOR_1 -s IOR_2
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Wrong number of arguments.\n")),
                      -1);

  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("p:s:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'p':
        options.primary_ior = get_opts.opt_arg ();
        break;
      case 's':
        options.forward_ior = get_opts.opt_arg ();
        break;
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Usage: %s -p IOR_1 -s IOR_2\n"),
                           argv[0]),
                          -1);
      }

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  Options options;

  try
    {
#if TAO_HAS_INTERCEPTORS == 1
      PortableInterceptor::ORBInitializer_ptr temp_initializer {};

      ACE_NEW_RETURN (temp_initializer,
                      Client_ORBInitializer,
                      -1);  // No exceptions yet!
      PortableInterceptor::ORBInitializer_var orb_initializer =
        temp_initializer;

      PortableInterceptor::register_orb_initializer (orb_initializer.in ());
#endif  /* TAO_HAS_INTERCEPTORS == 1 */

      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                            argv,
                                            "Client ORB");

      if (::parse_args (argc, argv, options) != 0)
        return -1;

      CORBA::Object_var object =
        orb->string_to_object (options.primary_ior.c_str ());

      RedirectionTest::test_var server =
        RedirectionTest::test::_narrow (object.in ());

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Object reference <%s> is nil.\n"),
                             options.primary_ior.c_str ()),
                            1);
        }

      CORBA::Short number {};
      for (int i { 1 }; i <= 4; ++i)
        {
          ACE_DEBUG ((LM_INFO,
                      ACE_TEXT ("CLIENT: Issuing request %d.\n"),
                      i));

          number = server->number ();

          ACE_DEBUG ((LM_INFO,
                      ACE_TEXT ("CLIENT: Request %d handled by object %d.\n"),
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
              ACE_TEXT ("PortableInterceptor::Redirection test passed.\n")));

  return 0;
}
