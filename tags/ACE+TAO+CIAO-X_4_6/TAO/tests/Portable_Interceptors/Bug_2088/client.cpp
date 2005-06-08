// $Id$

#include "ace/Get_Opt.h"
#include "Client_ORBInitializer.h"
#include "tao/ORBInitializer_Registry.h"

ACE_RCSID(Interceptors, client, "$Id$")

int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      Client_ORBInitializer* initializer1 = 0;
      Client_ORBInitializer* initializer2 = 0;

      ACE_NEW_RETURN (initializer1,
                      Client_ORBInitializer,
                      -1);  // No exceptions yet!

      ACE_NEW_RETURN (initializer2,
                      Client_ORBInitializer,
                      -1);  // No exceptions yet!

      PortableInterceptor::ORBInitializer_var initializer_var1 =
        initializer1;

      PortableInterceptor::ORBInitializer_var initializer_var2 =
        initializer2;

      PortableInterceptor::register_orb_initializer (initializer_var1.in ()
                                                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableInterceptor::register_orb_initializer (initializer_var2.in ()
                                                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // For both initializers pre/post must be called.
      ACE_ASSERT (initializer1->pre_init_called == true);
      ACE_ASSERT (initializer2->pre_init_called == true);
      ACE_ASSERT (initializer1->post_init_called == true);
      ACE_ASSERT (initializer2->post_init_called == true);
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Caught exception in client:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
