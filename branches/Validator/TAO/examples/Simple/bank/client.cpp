// $Id$

# include "Bank_Client_i.h"

// The client program for the application.

int
main (int argc, char **argv)
{
  Bank_Client_i client;
  

  ACE_DEBUG ((LM_DEBUG,
              "\nBank client\n\n"));

  if (client.run ("Bank",argc, argv) == -1)
    return -1; 
  else
    return 0;
   
}

                                 
