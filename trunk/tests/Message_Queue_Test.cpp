#include "ace/Message_Queue.h"
#include "ace/Synch.h"
#include <iostream.h>

typedef ACE_Message_Queue <ACE_NULL_SYNCH> QUEUE;
typedef ACE_Message_Queue_Iterator <ACE_NULL_SYNCH> ITERATOR;
typedef ACE_Message_Queue_Reverse_Iterator <ACE_NULL_SYNCH> REVERSE_ITERATOR;

int 
main ()
{
  const int ITERATIONS = 5;
  QUEUE queue;

  for (int i = 1; i <= ITERATIONS; i++)
    {
      char *buffer = new char[BUFSIZ];
      sprintf (buffer, "%d", i);
      ACE_Message_Block *entry = new ACE_Message_Block (buffer, sizeof buffer);
      if (queue.enqueue (entry) == -1)
	ACE_ERROR_RETURN ((LM_ERROR, "QUEUE::enqueue\n"), -1);
    }
  
  ACE_DEBUG ((LM_DEBUG, "\nForward Iterations\n"));
  {
    ITERATOR iterator (queue);  
    for (ACE_Message_Block *entry = 0;
	 iterator.next (entry) != 0;
	 iterator.advance ())
      {
	ACE_DEBUG ((LM_DEBUG, "%s\n", entry->base ()));
      }  
  }

  ACE_DEBUG ((LM_DEBUG, "\nReverse Iterations\n"));
  {
    REVERSE_ITERATOR iterator (queue);
    for (ACE_Message_Block *entry = 0;
	 iterator.next (entry) != 0;
	 iterator.advance ())
      {
	ACE_DEBUG ((LM_DEBUG, "%s\n", entry->base ()));
      }  
  }

  ACE_DEBUG ((LM_DEBUG, "\nForward Iterations\n"));
  {
    QUEUE::ITERATOR iterator (queue);
    for (ACE_Message_Block *entry = 0;
	 iterator.next (entry) != 0;
	 iterator.advance ())
      {
	ACE_DEBUG ((LM_DEBUG, "%s\n", entry->base ()));
      }  
  }
  
  ACE_DEBUG ((LM_DEBUG, "\nReverse Iterations\n"));
  {
    QUEUE::REVERSE_ITERATOR iterator (queue);
    for (ACE_Message_Block *entry = 0;
	 iterator.next (entry) != 0;
	 iterator.advance ())
      {
	ACE_DEBUG ((LM_DEBUG, "%s\n", entry->base ()));
      }  
  }

  return 0;
}
