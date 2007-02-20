// $Id$

#include "tao/corba.h"

int main(int argc, char* argv[])
{
  if (TAO_debug_level != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR, "Error, wrong debug level, should be 0\n"), 1);
    }

  ACE_DEBUG ((LM_INFO, "TAO_debug_level before first ORB_init: %d\n",
    TAO_debug_level));

  CORBA::ORB_var orb1_ = CORBA::ORB_init(argc, argv, "ServerORB1");

  if (TAO_debug_level != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR, "Error, wrong debug level, should be 0\n"), 1);
    }

  ACE_DEBUG ((LM_INFO, "TAO_debug_level after first ORB_init: %d\n",
    TAO_debug_level));

  const char* my_argv[3];
  my_argv[0] = argv[0];
  my_argv[1] = "-ORBDebugLevel";
  my_argv[2] = "10";
  int my_argc = 3;

  CORBA::ORB_var orb2_ = CORBA::ORB_init(my_argc,
                                         const_cast<char**> (my_argv),
                                         "ServerORB2");

  ACE_DEBUG ((LM_INFO, "TAO_debug_level after second ORB_init: %d\n",
    TAO_debug_level));

  if (TAO_debug_level != 10)
    {
      ACE_ERROR_RETURN ((LM_ERROR, "Error, wrong debug level, should be 10\n"), 1);
    }

  return 0;
}

