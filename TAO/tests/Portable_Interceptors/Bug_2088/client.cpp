// $Id$

#include "ace/Get_Opt.h"
#include "Client_ORBInitializer.h"
#include "tao/ORBInitializer_Registry.h"

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  Client_ORBInitializer* initializer1 = 0;
  Client_ORBInitializer* initializer2 = 0;
  bool caught_exception = false;

  ACE_NEW_RETURN (initializer1,
                  Client_ORBInitializer,
                  -1);  // No exceptions yet!

  ACE_NEW_RETURN (initializer2,
                  Client_ORBInitializer,
                  -1);  // No exceptions yet!

  PortableInterceptor::ORBInitializer_var initializer_var1 = initializer1;
  PortableInterceptor::ORBInitializer_var initializer_var2 = initializer2;

  try
    {
      PortableInterceptor::register_orb_initializer (initializer_var1.in ());

      PortableInterceptor::register_orb_initializer (initializer_var2.in ());

      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);
    }
  catch (const CORBA::NO_MEMORY&)
    {
      // Initializer1 throws this exception and we should get it here
      caught_exception = true;
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught exception in client:");
      return 1;
    }

  // Only the pre init for initalizer 1 must be called, other initializers
  // shouldn't be caught
  if (initializer1->pre_init_called != true)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Pre init not called for 1\n"),
                        -1);
    }
  if (initializer2->pre_init_called != false)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Pre init called for 2\n"),
                        -1);
    }
  if (initializer1->post_init_called != false)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Post init not called for 1\n"),
                        -1);
    }
  if (initializer2->post_init_called != false)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Post init not called for 1\n"),
                        -1);
    }
  if (caught_exception != true)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Excep not caught\n"),
                        -1);
    }

  return 0;
}
