// $Id$

#include "tao/corba.h"

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  if (TAO_debug_level != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR, "Error, wrong debug level, should be 0\n"), 1);
    }

  ACE_DEBUG ((LM_INFO, "TAO_debug_level before first ORB_init: %d\n",
    TAO_debug_level));

  try
    {
      CORBA::ORB_var orb1_ = CORBA::ORB_init(argc, argv, "ServerORB1");

      if (TAO_debug_level != 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR, "Error, wrong debug level, should be 0\n"), 1);
        }

      ACE_DEBUG ((LM_INFO, "TAO_debug_level after first ORB_init: %d\n",
                  TAO_debug_level));

      ACE_TCHAR *my_argv[3];
      my_argv[0] = argv[0];
      my_argv[1] = const_cast<ACE_TCHAR *> (ACE_TEXT ("-ORBDebugLevel"));
      my_argv[2] = const_cast<ACE_TCHAR *> (ACE_TEXT ("10"));
      int my_argc = 3;

      CORBA::ORB_var orb2_ = CORBA::ORB_init (my_argc, my_argv, "ServerORB2");

      ACE_DEBUG ((LM_INFO, "TAO_debug_level after second ORB_init: %d\n",
                  TAO_debug_level));

      if (TAO_debug_level != 10)
        {
          ACE_ERROR_RETURN ((LM_ERROR, "Error, wrong debug level, should be 10\n"), 1);
        }
    }
  catch (CORBA::Exception &ex)
    {
      ex._tao_print_exception ("Server main()");
      return 1;
    }
  return 0;
}
