// $Id$

#include "Collocation_Tester.h"
#include "tao/Strategies/advanced_resource.h"
#include "tao/CSD_Framework/CSD_ORBInitializer.h"
#include "tao/CSD_ThreadPool/CSD_TP_Strategy_Factory.h"
#include "tao/CSD_ThreadPool/CSD_ThreadPool.h"
#include "ace/Thread_Manager.h"

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      Collocation_Test coll_test;

      int orig_argc = argc;
      ACE_TCHAR **orig_argv = new ACE_TCHAR*[argc];
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
