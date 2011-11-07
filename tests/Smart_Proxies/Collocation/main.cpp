// $Id$

#include "Coll_Tester.h"
#include "tao/Strategies/advanced_resource.h"

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  Collocation_Test coll_test;

  try
    {
      coll_test.init (argc, argv);

      coll_test.run ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Uncaught exception: ");
    }

  return 0;
}
