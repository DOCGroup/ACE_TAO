// ============================================================================
// @(#)Handle_Set_Test.cpp	1.1	10/18/96

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

#include "ace/Log_Msg.h"
#include "ace/Handle_Set.h"
#include "test_config.h"

#define IS_ODD(X) (((X) & 1) != 0)

void
run_test (int count)
{
  int duplicates = 0;
  int sets	 = 0;
  int clears	 = 0;

  ACE_Handle_Set handle_set;

  ACE_OS::srand (ACE_OS::time (0L));

  for (int i = 0; i < count; i++)
    {
      int i = int (ACE_OS::rand () % ACE_Handle_Set::MAXSIZE);

      if (IS_ODD (i))
	{
	  if (handle_set.is_set ((ACE_HANDLE) i))
	    duplicates++;

	  handle_set.set_bit ((ACE_HANDLE) i);
	  sets++;
	}
      else
	{
	  if (handle_set.is_set ((ACE_HANDLE) i))
	    duplicates--;

	  handle_set.clr_bit ((ACE_HANDLE) i);
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
  ACE_START_TEST;

  int count = argc > 1 ? ACE_OS::atoi (argv[1]) : ACE_Handle_Set::MAXSIZE;
  run_test (count);

  ACE_END_TEST;
  return 0;
}

