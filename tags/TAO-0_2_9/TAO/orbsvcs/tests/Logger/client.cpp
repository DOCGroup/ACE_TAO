// $Id$

#include "client_i.h"

ACE_RCSID(Logger, client, "$Id$")

// This function runs the Logging service test.

int
main (int argc, char **argv)
{
  Logger_Client logger_client;

  ACE_DEBUG ((LM_DEBUG,
              "\n \t IDL_Logger: client \n\n"));

  if (logger_client.init (argc, argv) == -1)
    return 1;

  return logger_client.run ();
}
