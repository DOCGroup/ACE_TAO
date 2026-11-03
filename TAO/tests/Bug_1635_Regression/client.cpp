#include "tao/ORB.h"

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("SERVER (%P): Caught exception:");
      return 1;
    }

  return 0;
}
