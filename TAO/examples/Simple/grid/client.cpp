// -*- C++ -*-
// $Id$
#include "Client_i.h"

// This function runs the grid test.

int
main (int argc, char **argv)
{
  Client_i client;

  ACE_DEBUG ((LM_DEBUG,
              "\n\tGrid client\n\n"));

  if (client.init (argc, argv) == -1)
    return -1;
  else
    return client.run ();
}

