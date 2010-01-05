// -*- C++ -*-
// $Id$

#include "Filter.h"

int
ACE_TMAIN (int argc, ACE_TCHAR *argv [])
{
  FilterClient client;

  try
    {
      client.init_supplier (argc, argv); //Init the Client

      client.run_supplier ();
    }
  catch (const CORBA::UserException& ue)
    {
      ue._tao_print_exception ("TLS_Client user error: ");
      return 1;
    }
  catch (const CORBA::SystemException& se)
    {
      se._tao_print_exception ("Supplier system error: ");
      return 1;
    }

  return 0;
}
