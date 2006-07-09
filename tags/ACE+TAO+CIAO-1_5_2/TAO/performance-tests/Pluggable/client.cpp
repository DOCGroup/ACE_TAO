// $Id$

#include "PP_Test_Client.h"
#include "tao/Timeprobe.h"

ACE_RCSID(IDL_Cubit, client, "$Id$")

// This function runs the client test.

int
main (int argc, char **argv)
{
  PP_Test_Client pp_test_client;

  ACE_DEBUG ((LM_DEBUG,
              "\n\tPluggable_Test: client\n\n"));

  if (pp_test_client.init (argc, argv) == -1)
    return 1;

  int retval = pp_test_client.run ();

  ACE_TIMEPROBE_PRINT;

  return retval;
}
