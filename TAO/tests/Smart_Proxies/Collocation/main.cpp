#include "Coll_Tester.h"
#include "tao/Strategies/advanced_resource.h"

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  Collocation_Test coll_test;

  int result = 0;
  try
    {
      result = coll_test.init (argc, argv);
      if (result == 0)
        result = coll_test.run ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Uncaught exception: ");
      result = 1;
    }

  return result;
}
