// $Id$

#include "Cubit_Client.h"
#include "tao/Timeprobe.h"

ACE_RCSID(IDL_Cubit, client, "$Id$")

// This function runs the client test.

int
main (int argc, char **argv)
{
  Cubit_Client cubit_client;

  ACE_DEBUG ((LM_DEBUG,
              "\n\tIDL_Cubit: client\n\n"));

  if (cubit_client.init (argc, argv) == -1)
    return 1;

  int retval = cubit_client.run ();

  ACE_TIMEPROBE_PRINT;

#if defined (VXWORKS)
  int status;
  ACE_OS::thr_exit (&status);
#else  /* ! VXWORKS */
  // This causes a memPartFree on VxWorks.
  this->shutdown_server (this->shutdown_);
#endif /* ! VXWORKS */

  return retval;
}
