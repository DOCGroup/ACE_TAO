// $Id$

#include "Logging_Test_i.h"
#include "ace/Argv_Type_Converter.h"

ACE_RCSID(Logger, Logging_Test, "$Id$")

// This function runs the Logging service test.

int
ACE_TMAIN (int argc, ACE_TCHAR **argv)
{
  ACE_Argv_Type_Converter convert (argc, argv);
  Logger_Client logger_client;

  ACE_DEBUG ((LM_DEBUG,
              "\n \t IDL_Logger: client \n\n"));

  if (logger_client.init (convert.get_argc(), convert.get_ASCII_argv()) == -1)
    return 1;

  return logger_client.run ();

}
