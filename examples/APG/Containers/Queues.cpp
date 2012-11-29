// $Id$

#include "ace/OS_Memory.h"
#include "ace/Log_Msg.h"
#include "ace/Containers.h"
#include "DataElement.h"

class QueueExample
{
public:
  // Illustrate the various ACE Queues.
  int run (void);

private:
  // Illustrate the ACE unbounded queue
  // that has copies of the data elements.
  int runStackUnboundedQueue (void);

  // Illustrate the ACE unbounded queue
  // with pointers to elements on the heap.
  int runHeapUnboundedQueue (void);
};

int QueueExample::run (void)
{
  ACE_TRACE ("QueueExample::run");

  // Illustrate the queue with elements on the stack.
  if (this->runStackUnboundedQueue () != 0)
    {
      return -1;
    }

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\n# of live objects %d\n"),
              DataElement::numOfActiveObjects ()));

  // Illustrate the queue with elements on the heap.
  if (this->runHeapUnboundedQueue () != 0)
    {
      return -1;
    }

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\n# of live objects %d\n"),
              DataElement::numOfActiveObjects ()));

  return 0;
}

// Listing 1 code/ch05
int QueueExample::runStackUnboundedQueue (void)
{
  ACE_TRACE ("QueueExample::runStackUnboundedQueue");

  ACE_Unbounded_Queue<DataElement> queue;
  DataElement elem1[10];
  int i;
  for (i = 0; i < 10; i++)
    {
      elem1[i].setData (9-i);
      queue.enqueue_head (elem1[i]);
    }

  DataElement elem2[10];
  for (i = 0; i < 10; i++)
    {
      elem2[i].setData (i+10);
      queue.enqueue_tail (elem2[i]);
    }

  for (ACE_Unbounded_Queue_Iterator<DataElement> iter (queue);
       !iter.done ();
       iter.advance ())
    {
      DataElement *elem = 0;
      iter.next (elem);
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%d:"), elem->getData ()));
    }

  return 0;
}
// Listing 1
// Listing 2 code/ch05
int QueueExample::runHeapUnboundedQueue (void)
{
  ACE_TRACE ("QueueExample::runHeapUnboundedQueue");

  ACE_Unbounded_Queue<DataElement*> queue;
  for (int i = 0; i < 20; i++)
    {
      DataElement *elem;
      ACE_NEW_RETURN(elem, DataElement (i), -1);
      queue.enqueue_head (elem);
    }

  for (ACE_Unbounded_Queue_Iterator<DataElement*> iter
         = queue.begin ();
       !iter.done ();
       iter.advance ())
    {
      DataElement **elem = 0;
      iter.next(elem);
      ACE_DEBUG
        ((LM_DEBUG, ACE_TEXT ("%d:"), (*elem)->getData ()));
      delete (*elem);
    }

  return 0;
}
// Listing 2
int ACE_TMAIN (int, ACE_TCHAR *[])
{
  QueueExample que;
  return que.run ();
}

