//$Id$
#include "TestClient.h"

#include <ace/streams.h>

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
  {
    CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

    {
      TestClient client (orb.in (), argc, argv);
      client.run();
    }

    orb->destroy();
    return 0;
  }
  catch (CORBA::Exception& ex)
  {
    cerr << "TestClient: " << ex << endl;
  }
  return 1;
}
