// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
// 
// = FILENAME
//    Handle_Set_Test.cpp
//
// = DESCRIPTION
//      This test illustrates the use of ACE_Handle_Set to maintain a
//      set of handles. No command line arguments are needed to run
//      the test. 
//
// = AUTHOR
//    Prashant Jain
// 
// ============================================================================


#include "ace/Handle_Set.h"
#include "test_config.h"

#define IS_ODD(X) (((X) & 1) != 0)

static void
run_test (int count)
{
  int duplicates = 0;
  int sets = 0;
  int clears = 0;

  ACE_Handle_Set handle_set;

  ACE_OS::srand (ACE_OS::time (0L));

  for (int i = 0; i < count; i++)
    {
      int j = int (ACE_OS::rand () % ACE_Handle_Set::MAXSIZE);

      if (IS_ODD (j))
	{
	  if (handle_set.is_set ((ACE_HANDLE) j))
	    duplicates++;

	  handle_set.set_bit ((ACE_HANDLE) j);
	  sets++;
	}
      else
	{
	  if (handle_set.is_set ((ACE_HANDLE) j))
	    duplicates--;

	  handle_set.clr_bit ((ACE_HANDLE) j);
	  clears++;
	}
    }

  ACE_ASSERT (count == sets + clears);
  ACE_ASSERT (handle_set.num_set () + duplicates == sets);

//  ACE_DEBUG ((LM_DEBUG, "count = %d, set_size = %d, duplicates = %d\n",
//	      count, handle_set.num_set (), (sets - clears) == duplicates));
}

int
main (int argc, char *argv[])
{
  ACE_START_TEST ("Handle_Set_Test.cpp");

  int count = argc > 1 ? ACE_OS::atoi (argv[1]) : ACE_Handle_Set::MAXSIZE;
  run_test (count);

  ACE_END_TEST;
  return 0;
}

