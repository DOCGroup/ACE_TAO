// $Id$

#include "Client_ORBInitializer.h"
#include "tao/ORBInitializer_Registry.h"

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
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

  try
    {
      PortableInterceptor::register_orb_initializer (initializer_var1.in ());

      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      CORBA::ORB_var orb2 =
        CORBA::ORB_init (argc, argv, "SecondORB");
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught exception in client:");
      return 1;
    }

  // The first ORBInitializer should be called twice, the second only once
  ACE_ASSERT (initializer1->pre_init_called == 2);
  ACE_ASSERT (initializer2->pre_init_called == 1);
  ACE_ASSERT (initializer1->post_init_called == 2);
  ACE_ASSERT (initializer2->post_init_called == 1);

  return 0;
}
