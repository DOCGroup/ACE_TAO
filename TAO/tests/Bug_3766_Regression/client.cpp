// $Id$

#include "TestC.h"
#include "ace/OS.h"

using namespace std;

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA");

      CORBA::Object_var object = orb->string_to_object("file://server.ior");

      Test_var test = Test::_narrow (object.in ());

      Info_var info;

      test->do_something(info);

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
