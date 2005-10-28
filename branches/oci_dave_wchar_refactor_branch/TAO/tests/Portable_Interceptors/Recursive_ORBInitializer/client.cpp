// $Id$

#include "Client_ORBInitializer.h"
#include "tao/ORBInitializer_Registry.h"
#include "ace/Argv_Type_Converter.h"

ACE_RCSID(Recursive_ORBInitializer,
          client,
          "$Id$")

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_Argv_Type_Converter convert (argc, argv);

  Client_ORBInitializer* initializer1 = 0;
  Client_ORBInitializer* initializer2 = 0;

  ACE_NEW_RETURN (initializer2,
                  Client_ORBInitializer (0),
                  -1);  // No exceptions yet!

  ACE_NEW_RETURN (initializer1,
                  Client_ORBInitializer (initializer2),
                  -1);  // No exceptions yet!

  PortableInterceptor::ORBInitializer_var initializer_var1 =
    initializer1;

  PortableInterceptor::ORBInitializer_var initializer_var2 =
    initializer2;

  ACE_TRY_NEW_ENV
    {
      PortableInterceptor::register_orb_initializer (initializer_var1.in ()
                                                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::ORB_var orb =
        CORBA::ORB_init (convert.get_argc(), convert.get_ASCII_argv(), "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::ORB_var orb2 =
        CORBA::ORB_init (convert.get_argc(), convert.get_ASCII_argv(), "SecondORB" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Caught exception in client:");
      return 1;
    }
  ACE_ENDTRY;

  // The first ORBInitializer should be called twice, the second only once
  ACE_ASSERT (initializer1->pre_init_called == 2);
  ACE_ASSERT (initializer2->pre_init_called == 1);
  ACE_ASSERT (initializer1->post_init_called == 2);
  ACE_ASSERT (initializer2->post_init_called == 1);

  return 0;
}
