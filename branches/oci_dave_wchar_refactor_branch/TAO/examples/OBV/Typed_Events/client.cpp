//$Id$

# include "Client_i.h"
#include "ace/Argv_Type_Converter.h"

// The client program for the application.

int
ACE_TMAIN (int argc, ACE_TCHAR **argv)
{
  ACE_Argv_Type_Converter convert (argc, argv);
  Checkpoint_Client_i client;


  ACE_DEBUG ((LM_DEBUG,
              "\nEvent_Types client\n\n"));

  if (client.run ("Event_Types_Checkpoint",
                  argc,
                  argv) == -1)
    return -1;
  else
    return 0;

}
