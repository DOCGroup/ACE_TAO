// -*- C++ -*-
// $Id$

#include "ace/Get_Opt.h"
#include "testC.h"
#include "Client_ORBInitializer.h"
#include "tao/ORBInitializer_Registry.h"

const ACE_TCHAR *ior1 = 0;
const ACE_TCHAR *ior2 = 0;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  if (argc != 5)  // foo -p IOR_1 -s IOR_2
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Wrong number of arguments.\n"),
                      -1);

  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("p:s:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'p':
        ior1 = get_opts.opt_arg ();
        break;
      case 's':
        ior2 = get_opts.opt_arg ();
        break;
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Usage:  %s "
                           "-p IOR_1 -s IOR_2\n",
                           argv[0]),
                          -1);
      }

  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  int status = 0;

  try
    {
#if TAO_HAS_INTERCEPTORS == 1
      PortableInterceptor::ORBInitializer_ptr temp_initializer =
        PortableInterceptor::ORBInitializer::_nil ();

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

      if (::parse_args (argc, argv) != 0)
        return -1;

      CORBA::Object_var object =
        orb->string_to_object (ior1);

      RedirectionTest::test_var server =
        RedirectionTest::test::_narrow (object.in ());

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference <%s> is nil.\n",
                             ior1),
                            1);
        }

      CORBA::Short number = 0;
      for (int i = 1; i <= 4; ++i)
        {
          ACE_DEBUG ((LM_INFO,
                      "CLIENT: Issuing request %d.\n",
                      i));

          number = server->number ();

          ACE_DEBUG ((LM_INFO,
                      "CLIENT: Request %d handled by object %d.\n",
                      i,
                      number));
        }

      server->shutdown ();

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return -1;
    }

  if (status != -1)
    ACE_DEBUG ((LM_INFO,
                "PortableInterceptor::Redirection test passed.\n"));

  return status;
}
