// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
// 
// = FILENAME
//    Message_Queue_Test.cpp
//
// = DESCRIPTION
//      This is a simple test of the ACE_Message_Queue that
//      illustrates how to use the forward and reverse iterators. 
//
// = AUTHOR
//    Irfan Pyarali
// 
// ============================================================================

#include "ace/Message_Queue.h"
#include "ace/Synch.h"
#include "test_config.h"

typedef ACE_Message_Queue <ACE_NULL_SYNCH> QUEUE;
typedef ACE_Message_Queue_Iterator <ACE_NULL_SYNCH> ITERATOR;
typedef ACE_Message_Queue_Reverse_Iterator <ACE_NULL_SYNCH> REVERSE_ITERATOR;

int 
main (int, char *[])
{
  ACE_START_TEST ("Message_Queue_Test");

  const int ITERATIONS = 5;
  char *buffer[ITERATIONS];
  QUEUE queue;
  int i;

  for (i = 1; i <= ITERATIONS; i++)
    {
      ACE_NEW_RETURN (buffer[i-1], char[BUFSIZ], -1);
      ACE_OS::sprintf (buffer[i-1], "%d", i);

      ACE_Message_Block *entry;

      ACE_NEW_RETURN (entry, ACE_Message_Block (buffer[i-1], sizeof buffer[i-1]), -1);

      if (queue.enqueue_prio (entry) == -1)
	ACE_ERROR_RETURN ((LM_ERROR, "QUEUE::enqueue_prio\n"), -1);
    }
  
  ACE_DEBUG ((LM_DEBUG, "\nForward Iterations\n"));
  {
    ITERATOR iterator (queue);  

    for (ACE_Message_Block *entry = 0;
	 iterator.next (entry) != 0;
	 iterator.advance ())
      ACE_DEBUG ((LM_DEBUG, "%s\n", entry->base ()));
  }

  ACE_DEBUG ((LM_DEBUG, "\nReverse Iterations\n"));
  {
    REVERSE_ITERATOR iterator (queue);

    for (ACE_Message_Block *entry = 0;
	 iterator.next (entry) != 0;
	 iterator.advance ())
      ACE_DEBUG ((LM_DEBUG, "%s\n", entry->base ()));
  }

  ACE_DEBUG ((LM_DEBUG, "\nForward Iterations\n"));
  {
    QUEUE::ITERATOR iterator (queue);

    for (ACE_Message_Block *entry = 0;
	 iterator.next (entry) != 0;
	 iterator.advance ())
      ACE_DEBUG ((LM_DEBUG, "%s\n", entry->base ()));
  }
  
  ACE_DEBUG ((LM_DEBUG, "\nReverse Iterations\n"));
  {
    QUEUE::REVERSE_ITERATOR iterator (queue);

    for (ACE_Message_Block *entry = 0;
	 iterator.next (entry) != 0;
	 iterator.advance ())
      ACE_DEBUG ((LM_DEBUG, "%s\n", entry->base ()));
  }

  for (i = 0; i < ITERATIONS; i++)
    {
      delete [] buffer[i];
    }

  ACE_END_TEST;
  return 0;
}

#if defined (ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
template class ACE_Message_Queue_Iterator<ACE_NULL_SYNCH>;
template class ACE_Message_Queue_Reverse_Iterator<ACE_NULL_SYNCH>;
#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */
