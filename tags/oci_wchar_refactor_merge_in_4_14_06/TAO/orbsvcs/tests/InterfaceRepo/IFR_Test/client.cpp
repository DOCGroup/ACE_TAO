// -*- C++ -*-
// $Id$

#include "Admin_Client.h"
#include "ace/Argv_Type_Converter.h"

ACE_RCSID (IFR_Test,
           client,
           "$Id$")

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_Argv_Type_Converter convert (argc, argv);

  Admin_Client admin_client;

  int retval = admin_client.init (convert.get_argc(), convert.get_ASCII_argv());

  if (retval == -1)
    {
      return 1;
    }

  return admin_client.run ();
}
