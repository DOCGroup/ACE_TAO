// $Id$

#include "client_impl.h"

ACE_RCSID(ImplRepo, client, "$Id$")

// This function runs the test.

int
main (int argc, char **argv)
{
  Client_i client;

  ACE_DEBUG ((LM_DEBUG,
              "\n\tSimple_Client\n\n"));

  if (client.init (argc, argv) == -1)
    return 1;
  else
    return client.run ();
}

