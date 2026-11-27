#include "TestClient.h"

#include <iostream>

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb (CORBA::ORB_init (argc, argv));

      bool success {};
      {
        TestClient client (orb.in (), argc, argv);
        success = client.run ();
      }

      orb->destroy ();
      return success ? 0 : 1;
    }
  catch (CORBA::Exception const &ex)
    {
      std::cerr << "TestClient: " << ex << std::endl;
    }
  return 1;
}
