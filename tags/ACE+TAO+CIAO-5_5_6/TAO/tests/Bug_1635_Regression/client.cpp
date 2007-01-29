//
// $Id$
//
#include "tao/ORB.h"

int
main (int argc, char *argv[])
{

  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "");
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("SERVER (%P): Caught exception:");
      return 1;
    }

  return 0;
}
