// $Id$

# include "Persistent_Client.h"

// The "persistent client" program for the application.

int
main (int argc, char *argv[])
{
  Persistent_Client_i client;
  

  ACE_DEBUG ((LM_DEBUG,
              "\nGrid client\n\n"));

  if (client.run ("grid", argc, argv) == -1)
    return -1; 
  else
    return 0;
}

                                 
