// $Id$

#include "Client_i.h"

ACE_RCSID(Echo, client, "$Id$")

// This function runs the echo test.

int
main (int argc, char **argv)
{
  Client_i client;

  ACE_DEBUG ((LM_DEBUG,
              "\n\techo client\n\n"));

  if (client.init (argc, argv) == -1)
    return -1;
  else
    return client.run ();
}
