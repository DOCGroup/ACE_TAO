#include "TestServer.h"

#include <iostream>

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb (CORBA::ORB_init (argc, argv));

      TestServer server (orb.in (), argc, argv);
      server.run ();

      orb->destroy ();
      return 0;
    }
  catch (CORBA::Exception const &ex)
    {
      std::cerr << "CORBA error: " << ex << std::endl;
    }
  return 1;
}
