#include "ace/Log_Msg.h"
// @(#)test_handle_set.cpp	1.1	10/18/96

#include "ace/Handle_Set.h"

#define IS_ODD(X) (((X) & 1) != 0)

void
test1 (int count)
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
	  if (handle_set.is_set (i))
	    duplicates++;

	  handle_set.set_bit (i);
	  sets++;
	}
      else
	{
	  if (handle_set.is_set (i))
	    duplicates--;

	  handle_set.clr_bit (i);
	  clears++;
	}
    }

  ACE_DEBUG ((LM_DEBUG, "count = %d, set_size = %d, duplicates = %d\n",
	      count, handle_set.num_set (), (sets - clears) == duplicates));
}

void
test2 (void)
{
  ACE_Handle_Set handle_set;
  ACE_HANDLE handle;

  handle_set.set_bit (0);
  handle_set.set_bit (1);
  handle_set.set_bit (32);
  handle_set.set_bit (63);
  handle_set.set_bit (64);
  handle_set.set_bit (65);
  handle_set.set_bit (122);
  handle_set.set_bit (129);
  handle_set.set_bit (245);
  handle_set.set_bit (255);

  for (ACE_Handle_Set_Iterator fi (handle_set); 
       (handle = fi ()) != -1; 
       ++fi)
    ACE_DEBUG ((LM_DEBUG, "handle = %d\n", handle));
}

int
main (int argc, char *argv[])
{
  int count = argc > 1 ? ACE_OS::atoi (argv[1]) : ACE_Handle_Set::MAXSIZE;
  test1 (count);
  test2 ();
}

