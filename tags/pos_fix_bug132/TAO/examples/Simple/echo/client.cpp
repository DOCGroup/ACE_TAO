//$Id$

# include "Echo_Client_i.h"

// The client program for the application.

int
main (int argc, char **argv)
{
  Echo_Client_i client;
  

  ACE_DEBUG ((LM_DEBUG,
              "\nEcho client\n\n"));

  if (client.run ("Echo",
                  argc, 
                  argv) == -1)
    return -1; 
  else
    return 0;
   
}

                                 
