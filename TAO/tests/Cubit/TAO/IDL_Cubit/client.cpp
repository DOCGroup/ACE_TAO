// $Id$

#include "client_i.h"
#include "tao/Timeprobe.h"

// This function runs the test.

int
main (int argc, char **argv)
{
  Cubit_Client cubit_client;

  ACE_DEBUG ((LM_DEBUG,
              "\n \t IDL_Cubit: client \n\n"));

  if (cubit_client.init (argc, argv) == -1)
    return 1;

  int retval = cubit_client.run ();

  ACE_TIMEPROBE_PRINT;

  return retval;
}

