// $Id$

#include "nestea_client_i.h"

ACE_RCSID(ImplRepo, nestea_client, "$Id$")

// This function runs the test.

int
main (int argc, char **argv)
{
  Nestea_Client_i client;

  ACE_DEBUG ((LM_DEBUG, "\n\tNestea Bookshelf Client\n\n"));

  if (client.init (argc, argv) == -1)
    return 1;
  else
    return client.run ();
}

