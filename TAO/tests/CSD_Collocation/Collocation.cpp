#include "Collocation_Tester.h"
#include "tao/Strategies/advanced_resource.h"
#include "tao/CSD_Framework/CSD_ORBInitializer.h"
#include "tao/CSD_ThreadPool/CSD_TP_Strategy_Factory.h"
#include "tao/CSD_ThreadPool/CSD_ThreadPool.h"
#include "ace/Thread_Manager.h"

#include <vector>

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      Collocation_Test coll_test;

      int orig_argc = argc;
      std::vector<ACE_TCHAR *> orig_argv;
      orig_argv.reserve (argc);
      for (int i = 0; i < argc; ++i)
        orig_argv.push_back (argv[i]);

      coll_test.init (argc, argv);
      coll_test.run ();
      coll_test.shutdown ();

      //reinitialize ORB to reproduce the problem
      coll_test.init (orig_argc, orig_argv.data ());
      coll_test.run ();
      coll_test.shutdown ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Uncaught exception: ");
    }

  ACE_Thread_Manager::instance()->wait();
  return 0;
}
