// -*- C++ -*-
#include "RedGreen_Test.h"

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  ACE_High_Res_Timer::calibrate ();

  RedGreen_Test client;

  if (client.parse_args (argc, argv) != 0)
    return 1;

  try
    {
      client.init (argc, argv); // Init the Client

      client.run ();
    }
  catch (CORBA::Exception const& se)
    {
      se._tao_print_exception ("Error: ");
      return 1;
    }

  return 0;
}
