// $Id$

#include "Supplier_i.h"

// This function runs the Callback Quoter Supplier daemon.

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
 Supplier supplier;

  ACE_DEBUG ((LM_DEBUG,
              "\n\tMarket Status Supplier Daemon\n\n"));

  if (supplier.init (argc, argv) == -1)
    return -1;
  else
    return supplier.run ();
}
