// $Id$

#include "client_impl.h"

ACE_RCSID(Time, client, "$Id$")

// This function runs the time and date test.

int
main (int argc, char **argv)
{
  Client_Impl client;

  ACE_DEBUG ((LM_DEBUG,
              "\n\ttime and date test\n\n"));

  if (client.init (argc, argv) == -1)
    return 1;
  else
    return client.run ();
}

