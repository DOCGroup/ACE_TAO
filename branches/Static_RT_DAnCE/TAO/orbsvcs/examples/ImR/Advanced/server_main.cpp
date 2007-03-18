//$Id$
#include "TestServer.h"

#include <ace/streams.h>

int main(int argc, char* argv[])
{
  try
  {
    CORBA::ORB_var orb;
    orb = CORBA::ORB_init(argc, argv);

    {
      TestServer server(orb.in(), argc, argv);
      server.run();
    }

    orb->destroy();
    return 0;
  }
  catch (CORBA::Exception& ex)
  {
    cerr << "CORBA error: " << ex << endl;
  }
  return 1;
}
