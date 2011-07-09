// $Id$

#include "ace/Containers.h"
#include "ace/Malloc_T.h"
#include "ace/Synch.h" // Needed for the lock.
#include "DataElement.h"

class StackExample
{
public:
  // Illustrate all the differnet
  // types of stacks provided by ACE.
  int run (void);

private:
  // Illustrate the use of an unbounded stack.
  int runUnboundedStack (ACE_Allocator* allocator);
};

// Listing 1 code/ch05
int StackExample::run (void)
{
  ACE_TRACE ("StackUser::run");

  ACE_Allocator *allocator = 0;
  size_t block_size = sizeof(ACE_Node<DataElement>);
  ACE_NEW_RETURN
    (allocator,
     ACE_Dynamic_Cached_Allocator<ACE_Null_Mutex>
       (100 + 1, block_size),
     -1);

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\n# of live objects %d\n"),
              DataElement::numOfActiveObjects ()));

  ACE_ASSERT (this->runUnboundedStack (allocator) != -1);

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\n# of live objects %d\n"),
              DataElement::numOfActiveObjects ()));

  delete allocator;
  return 0;
}
// Listing 1
// Listing 2 code/ch05
int StackExample::runUnboundedStack (ACE_Allocator* allocator)
{
  ACE_TRACE ("StackExample::runUnboundedStack");

  // Pass in an allocator during construction.
  ACE_Unbounded_Stack<DataElement> ustack (allocator);

  for (int m = 0; m < 100; m++)
    {
      DataElement elem (m);
      int result = ustack.push (elem);
      if (result == -1)
        ACE_ERROR_RETURN
          ((LM_ERROR, ACE_TEXT ("%p\n"),
            ACE_TEXT ("Push Next Element")),
           -1);
    }

  void* furtherMemory = 0;
  furtherMemory = allocator->malloc
    (sizeof(ACE_Node<DataElement>));
  ACE_ASSERT (furtherMemory == 0);

  // No memory left..
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%p\n"),
              ACE_TEXT ("No memory..")));

  // Free up some memory in the allocator.
  DataElement e;
  for (int n = 0; n < 10; n++)
    {
      ustack.pop (e);
    }

  furtherMemory =
    allocator->malloc (sizeof (ACE_Node<DataElement>));
  ACE_ASSERT (furtherMemory != 0);

  return 0;
}
// Listing 2

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  StackExample se;
  return se.run ();
}

