// -*- C++ -*-

#include "ace/Get_Opt.h"

#include "testC.h"
#include "Client_ORBInitializer.h"

ACE_RCSID(ForwardRequest,
          client,
          "$Id$")

const char *ior1 = 0;
const char *ior2 = 0;

int
parse_args (int argc, char *argv[])
{
  if (argc != 5)  // foo -k IOR_1 -k IOR_2
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Wrong number of arguments.\n"),
                      -1);

  ACE_Get_Opt get_opts (argc, argv, "k:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':

        if (ior1 == 0)
          ior1 = get_opts.optarg;
        else if (ior2 == 0)
          ior2 = get_opts.optarg;

        break;
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Usage:  %s "
                           "-k IOR_1 -k IOR_2\n",
                           argv[0]),
                          -1);
      }

  return 0;
}

int
main (int argc, char *argv[])
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      PortableInterceptor::ORBInitializer_ptr temp_initializer =
        PortableInterceptor::ORBInitializer::_nil ();

      ACE_NEW_RETURN (temp_initializer,
                      Client_ORBInitializer,
                      -1);  // No exceptions yet!
      PortableInterceptor::ORBInitializer_var orb_initializer =
        temp_initializer;

      PortableInterceptor::register_orb_initializer (orb_initializer.in (),
                                                     ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                            argv,
                                            "Client ORB",
                                            ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (::parse_args (argc, argv) != 0)
        return -1;

      // Start out with the first IOR.  Interaction with the second
      // IOR occurs during the various interceptions executed during
      // this test.
      CORBA::Object_var object =
        orb->string_to_object (ior1, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      ForwardRequestTest::test_var server =
        ForwardRequestTest::test::_narrow (object.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference <%s> is nil\n",
                             ior1),
                            1);
        }

      // Invoke the operation four times.  By design, the last three
      // invocations in this test will cause
      // PortableInterceptor::ForwardRequest exceptions to be thrown,
      // thus causing the request to be forwarded to another object.

      for (int i = 1; i <= 4; ++i)
        {
          ACE_DEBUG ((LM_INFO,
                      "CLIENT: Issuing request %d.\n",
                      i));
          
          CORBA::Short number = server->number (ACE_TRY_ENV);
          ACE_TRY_CHECK;

          ACE_DEBUG ((LM_INFO,
                      "CLIENT: Request %d handled by object %d.\n",
                      i,
                      number));
        }

      server->shutdown (ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Caught exception:");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}
