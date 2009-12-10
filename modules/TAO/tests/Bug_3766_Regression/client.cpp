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

      VariableLengthInfo_var variable_info;
      test->do_something_VariableLength(variable_info);

      FixedLengthInfo_var fixed_info;
      test->do_something_FixedLength(fixed_info);

      test->shutdown();

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
