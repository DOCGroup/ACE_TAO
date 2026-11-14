// -*- C++ -*-
#include "Subscribe.h"

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  Subscribe client {};

  try
    {
      client.init (argc, argv);

      client.run ();
    }
  catch (CORBA::UserException const &ue)
    {
      ue._tao_print_exception ("Subscribe user error: ");
      return 1;
    }
  catch (CORBA::SystemException const &se)
    {
      se._tao_print_exception ("Subscribe system error: ");
      return 1;
    }

  return 0;
}
