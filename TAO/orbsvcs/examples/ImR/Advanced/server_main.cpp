//$Id$
#include "TestServer.h"

#include <ace/streams.h>
#include "ace/Argv_Type_Converter.h"

int ACE_TMAIN(int argc, ACE_TCHAR* argv[])
{
  ACE_Argv_Type_Converter convert (argc, argv);

  try
  {
    CORBA::ORB_var orb;
    orb = CORBA::ORB_init(convert.get_argc(), convert.get_ASCII_argv());

    {
      TestServer server(orb.in(), convert.get_argc(), convert.get_ASCII_argv());
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
