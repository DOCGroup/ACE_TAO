// $Id$

#include "Logging_Test_i.h"

// This function runs the Logging service test.
int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  Logger_Client logger_client;

  ACE_DEBUG ((LM_DEBUG,
              "\n \t IDL_Logger: client \n\n"));

  if (logger_client.init (argc, argv) == -1)
    return 1;

  return logger_client.run ();

}
