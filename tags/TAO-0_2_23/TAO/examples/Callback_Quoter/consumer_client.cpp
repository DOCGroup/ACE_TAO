// $Id$

#include "Client_i.h"

// This function runs the Callback Quoter Consumer client.

int
main (int argc, char **argv)
{
  Client_i client;

  ACE_DEBUG ((LM_DEBUG,
              "\n\tConsumer client\n\n"));

  if (client.init (argc, argv) == -1)
    return -1;
  else
    return client.run ();
}
