// $Id$

#include "Client_i.h"

ACE_RCSID(Time, client, "$Id$")

// This function runs the Bank client test.

int
main (int argc, char *argv[])
{
  Client_i client;

  ACE_DEBUG ((LM_DEBUG,
              "[CLIENT] Process/Thread Id : (%P/%t) Bank client\n"));

  if (client.init (argc, argv) == -1)
    return -1;
  else
    return client.run ();
}
