// $Id$

#include "Collocation_Tester.h"
#include "tao/Strategies/advanced_resource.h"
#include "tao/CSD_Framework/CSD_ORBInitializer.h"
#include "tao/CSD_ThreadPool/CSD_TP_Strategy_Factory.h"
#include "tao/CSD_ThreadPool/CSD_ThreadPool.h"
#include "ace/Thread_Manager.h"

ACE_RCSID(Collocation, main, "$Id$")

int main (int argc, char *argv[])
{
  try
    {
      Collocation_Test coll_test;

      int orig_argc = argc;
      char **orig_argv = new char*[argc];
      for (int i = 0; i < argc; ++i)
        {
          orig_argv[i] = argv[i];
        }

      coll_test.init (argc, argv);
      coll_test.run ();
      coll_test.shutdown ();

      //reinitialize ORB to reproduce the problem
      coll_test.init (orig_argc, orig_argv);
      coll_test.run ();
      coll_test.shutdown ();

      delete[] orig_argv;
      //this will leak if we get an exception, but it's just a small test case
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Uncaught exception: ");
    }

  ACE_Thread_Manager::instance()->wait();
  return 0;
}
