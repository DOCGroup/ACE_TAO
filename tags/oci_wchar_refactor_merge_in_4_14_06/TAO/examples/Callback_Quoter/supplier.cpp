// $Id$

#include "Supplier_i.h"
#include "ace/Argv_Type_Converter.h"

// This function runs the Callback Quoter Supplier daemon.

int
ACE_TMAIN (int argc, ACE_TCHAR **argv)
{
  ACE_Argv_Type_Converter convert (argc, argv);
  Supplier supplier;

  ACE_DEBUG ((LM_DEBUG,
              "\n\tMarket Status Supplier Daemon\n\n"));

  if (supplier.init (convert.get_argc(), convert.get_ASCII_argv()) == -1)
    return -1;
  else
    return supplier.run ();
}
