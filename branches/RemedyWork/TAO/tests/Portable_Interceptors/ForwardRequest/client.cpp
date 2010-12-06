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
  if (argc != 5)  // foo -k IOR_1 -k IOR_2
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Wrong number of arguments.\n"),
                      -1);

  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':

        if (ior1 == 0)
          ior1 = get_opts.opt_arg ();
        else if (ior2 == 0)
          ior2 = get_opts.opt_arg ();

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

      // Start out with the first IOR.  Interaction with the second
      // IOR occurs during the various interceptions executed during
      // this test.
      CORBA::Object_var object =
        orb->string_to_object (ior1);

      ForwardRequestTest::test_var server =
        ForwardRequestTest::test::_narrow (object.in ());

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference <%s> is nil.\n",
                             ior1),
                            1);
        }

      // Invoke the operation four times.  By design, the last three
      // invocations in this test will cause
      // PortableInterceptor::ForwardRequest exceptions to be thrown,
      // thus causing the request to be forwarded to another object.

      CORBA::Short old_number = 0;  // Previous invocation result.
      CORBA::Short number = 0;      // New invocation result.
      for (int i = 1; i <= 5; ++i)
        {
          ACE_DEBUG ((LM_INFO,
                      "CLIENT: Issuing request %d.\n",
                      i));

          if (i > 1)
            old_number = number;

          number = server->number ();


          ACE_DEBUG ((LM_INFO,
                      "CLIENT: Request %d handled by object %d.\n",
                      i,
                      number));

          // Check if the new result is the same as the previous
          // result.
          //
          // This test is designed so that no two sequential
          // invocation results are the same.  If they are the same,
          // then the requests were invoked on the same object, one
          // after the other.  This means that forwarding did not
          // occur, which is of course a failure in the
          // PortableInterceptor::ForwardRequest support.
          if (i > 1 && old_number == number)
            {
              status = -1;

              ACE_ERROR ((LM_ERROR,
                          "TEST FAILED: Request was not "
                          "forwarded.\n"));
              break;
            }
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
                "PortableInterceptor::ForwardRequest test passed.\n"));

  return status;
}
