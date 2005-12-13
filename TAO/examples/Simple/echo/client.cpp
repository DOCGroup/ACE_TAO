//$Id$

# include "Echo_Client_i.h"
#include "ace/Argv_Type_Converter.h"

// The client program for the application.

int
ACE_TMAIN (int argc, ACE_TCHAR **argv)
{
  ACE_Argv_Type_Converter convert (argc, argv);
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

                                 
