//$Id$

# include "Time_Client_i.h"

// The client program for the application.

int
main (int argc, char **argv)
{
  Time_Client_i client;
  

  ACE_DEBUG ((LM_DEBUG,
              "\n\ttime and date client\n\n"));

  if (client.run ("Time",argc, argv) == -1)
    return -1; 
  else
    return 0;
   
}

                                 
