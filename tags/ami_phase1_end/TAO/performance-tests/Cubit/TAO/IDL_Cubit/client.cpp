// $Id$

#include "Cubit_Client.h"
#include "tao/Timeprobe.h"
#include "tests/test_config.h"

ACE_RCSID(IDL_Cubit, client, "$Id$")

// This function runs the client test.

int
main (int argc, char **argv)
{
//  ACE_START_TEST (ASYS_TEXT ("Cubit_Client"));

  Cubit_Client cubit_client;

  ACE_DEBUG ((LM_DEBUG,
              "\n\tIDL_Cubit: client\n\n"));

  if (cubit_client.init (argc, argv) == -1)
    return 1;

  int retval = cubit_client.run ();

  ACE_TIMEPROBE_PRINT;

//  ACE_END_TEST;

  return retval;
}
