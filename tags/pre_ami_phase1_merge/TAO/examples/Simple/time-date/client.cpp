// $Id$

# include "Time_Date_Client_i.h"

// The client program for the application.

int
main (int argc, char *argv[])
{
  Time_Date_Client_i client;
  

  ACE_DEBUG ((LM_DEBUG,
              "\nTime_Date client\n\n"));

  if (client.run ("Time_Date", argc, argv) == -1)
    return -1; 
  else
    return 0;
}
