// $Id$

# include "Grid_Client_i.h"

// The client program for the application.

int
main (int argc, char *argv[])
{
  Grid_Client_i client;
  

  ACE_DEBUG ((LM_DEBUG,
              "\nGrid client\n\n"));

  if (client.run ("Grid", argc, argv) == -1)
    return -1; 
  else
    return 0;
   
}

                                 
