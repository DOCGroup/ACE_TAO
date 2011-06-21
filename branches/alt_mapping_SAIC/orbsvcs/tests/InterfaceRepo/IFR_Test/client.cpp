// -*- C++ -*-
// $Id$

#include "Admin_Client.h"

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  Admin_Client admin_client;

  int retval = admin_client.init (argc, argv);

  if (retval == -1)
    {
      return 1;
    }

  return admin_client.run ();
}
