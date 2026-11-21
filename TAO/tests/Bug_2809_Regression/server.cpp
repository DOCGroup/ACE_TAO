#include "tao/corba.h"

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  if (TAO_debug_level != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR, "Error, wrong debug level, should be 0\n"), 1);
    }

  ACE_DEBUG ((LM_INFO,
              "TAO_debug_level before first ORB_init: %d\n",
              TAO_debug_level));

  try
    {
      CORBA::ORB_var const orb1 = CORBA::ORB_init (argc, argv, "ServerORB1");

      if (TAO_debug_level != 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR, "Error, wrong debug level, should be 0\n"), 1);
        }

      ACE_DEBUG ((LM_INFO, "TAO_debug_level after first ORB_init: %d\n",
                  TAO_debug_level));

      ACE_TCHAR debug_option[] = ACE_TEXT ("-ORBDebugLevel");
      ACE_TCHAR debug_level[] = ACE_TEXT ("10");
      ACE_TCHAR *my_argv[] =
        {
          argv[0],
          debug_option,
          debug_level
        };
      int my_argc {3};

      CORBA::ORB_var const orb2 = CORBA::ORB_init (my_argc, my_argv, "ServerORB2");

      ACE_DEBUG ((LM_INFO, "TAO_debug_level after second ORB_init: %d\n",
                  TAO_debug_level));

      if (TAO_debug_level != 10)
        {
          ACE_ERROR_RETURN ((LM_ERROR, "Error, wrong debug level, should be 10\n"), 1);
        }

      orb2->destroy ();
      orb1->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Server main()");
      return 1;
    }
  return 0;
}
