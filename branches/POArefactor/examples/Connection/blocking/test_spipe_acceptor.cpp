// $Id$

// ACE_SPIPE Server.

#include "SPIPE-acceptor.h"

ACE_RCSID(blocking, test_spipe_acceptor, "$Id$")

#if ((defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)) || (defined (ACE_HAS_AIO_CALLS)))

int
main (int argc, char *argv[])
{
  // Perform Service_Config initializations
  ACE_Service_Config daemon (argv[0]);

  IPC_Server peer_acceptor;

  if (peer_acceptor.init (argc, argv) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "init"), -1);

  return peer_acceptor.svc ();
}

#else

int
main (int, char *[])
{
  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("Asynchronous IO is unsupported.\n")));

  return 0;
}

#endif
