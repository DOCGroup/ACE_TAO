//$Id$

# include "Client_i.h"

// The client program for the application.

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
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
