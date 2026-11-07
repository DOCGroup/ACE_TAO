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
      foo_i.bar (&something);

      orb->destroy ();
    }
  catch (CORBA::Exception const & ex)
    {
      ex._tao_print_exception ("Caught exception:");

      try
        {
          orb->shutdown ();
          orb->destroy ();
        }
      catch (CORBA::Exception const & ex)
        {
          ex._tao_print_exception ("Caught exception while shutting down:");
          return 1;
        }
      return 0;
    }

  return 0;
}
