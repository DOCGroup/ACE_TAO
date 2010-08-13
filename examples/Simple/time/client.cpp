//$Id$

# include "Time_Client_i.h"

// The client program for the application.

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  Time_Client_i client;

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("\n\tTime and date client\n\n")));

  if (client.run ("Time", argc, argv) == -1)
    return -1;
  else
    return 0;
}
