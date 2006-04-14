// $Id$

#include "Consumer_Handler.h"
#include "ace/Argv_Type_Converter.h"

// This function runs the Callback Quoter Consumer application.

int
ACE_TMAIN (int argc, ACE_TCHAR **argv)
{
  ACE_Argv_Type_Converter convert (argc, argv);
  Consumer_Handler consumer;

  ACE_DEBUG ((LM_DEBUG,
              "\n\t***Consumer***\n\n"));

  if (consumer.init (convert.get_argc(), convert.get_ASCII_argv()) == -1)
    return -1;
  else
    return consumer.run ();
}
