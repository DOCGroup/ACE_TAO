//$Id$
#include "tao/RTScheduling/RTScheduler.h"
#include "testC.h"
#include "test.h"

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  CORBA::ORB_var orb;
  try
    {
      orb = CORBA::ORB_init (argc,
                             argv);

      test_impl foo_i;
      int something = 28;
      foo_i.bar ((CORBA::VoidData) &something);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught exception:");

      orb->shutdown ();
      orb->destroy ();
      return 0;
    }

  return 0;
}
