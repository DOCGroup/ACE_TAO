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

#include "test_config.h"
#include "ace/Message_Queue.h"
#include "ace/Synch.h"

#if defined(__BORLANDC__) && __BORLANDC__ >= 0x0530
USELIB("..\ace\aced.lib");
//---------------------------------------------------------------------------
#endif /* defined(__BORLANDC__) && __BORLANDC__ >= 0x0530 */

typedef ACE_Message_Queue <ACE_NULL_SYNCH> QUEUE;
typedef ACE_Message_Queue_Iterator <ACE_NULL_SYNCH> ITERATOR;
typedef ACE_Message_Queue_Reverse_Iterator <ACE_NULL_SYNCH> REVERSE_ITERATOR;

int
main (int, ASYS_TCHAR *[])
{
  ACE_START_TEST (ASYS_TEXT ("Message_Queue_Test"));

  const int ITERATIONS = 5;
  ASYS_TCHAR buffer[ITERATIONS][BUFSIZ];
  QUEUE queue (32 * 1024);
  int i;

  for (i = 0; i < ITERATIONS; i++)
    {
      ACE_OS::sprintf (buffer[i], ASYS_TEXT ("%d"), i+1);

      ACE_Message_Block *entry;
      ACE_NEW_RETURN (entry, ACE_Message_Block ((char *) buffer[i], sizeof buffer[i]), -1);

      if (queue.is_full ())
        ACE_ERROR_RETURN ((LM_ERROR,
                           ASYS_TEXT ("QUEUE:: the message queue is full on iteration %u!\n"),
                           i+1),
                          -1);

      if (queue.enqueue (entry) == -1)
        ACE_ERROR_RETURN ((LM_ERROR, ASYS_TEXT ("QUEUE::enqueue\n")), -1);
    }

  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("\nForward Iterations\n")));
  {
    ITERATOR iterator (queue);

    for (ACE_Message_Block *entry = 0;
         iterator.next (entry) != 0;
         iterator.advance ())
      ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("%s\n"), entry->base ()));
  }

  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("\nReverse Iterations\n")));
  {
    REVERSE_ITERATOR iterator (queue);

    for (ACE_Message_Block *entry = 0;
         iterator.next (entry) != 0;
         iterator.advance ())
      ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("%s\n"), entry->base ()));
  }

  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("\nForward Iterations\n")));
  {
    QUEUE::ITERATOR iterator (queue);

    for (ACE_Message_Block *entry = 0;
         iterator.next (entry) != 0;
         iterator.advance ())
      ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("%s\n"), entry->base ()));
  }

  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("\nReverse Iterations\n")));
  {
    QUEUE::REVERSE_ITERATOR iterator (queue);

    for (ACE_Message_Block *entry = 0;
         iterator.next (entry) != 0;
         iterator.advance ())
      ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("%s\n"), entry->base ()));
  }

  ACE_END_TEST;
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Message_Queue_Iterator<ACE_NULL_SYNCH>;
template class ACE_Message_Queue_Reverse_Iterator<ACE_NULL_SYNCH>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Message_Queue_Iterator<ACE_NULL_SYNCH>
#pragma instantiate ACE_Message_Queue_Reverse_Iterator<ACE_NULL_SYNCH>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
