// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Simple_Message_Block_Test.cpp
//
// = DESCRIPTION
//
//      This test program is a torture test that illustrates how
//      ACE_Message_Block reference counting works, how and when locks
//      are used, how memory is managed, and how continuation chains
//      of message blocks are made. Ideally used with purify :-)
//
// = AUTHOR
//    Irfan Pyarali (irfan@cs.wustl.edu)
//
// ============================================================================

#include "ace/Message_Block.h"
#include "ace/Synch.h"
#include "test_config.h"

int
main (void)
{
  ACE_START_TEST ("Simple_Message_Block_Test");

  {
    // Checks normal stack deletes.
    ACE_Message_Block mb;
  }

  {
    // Checks normal heap deletes.
    ACE_Message_Block *mb;

    ACE_NEW_RETURN (mb, ACE_Message_Block, -1);
    mb->release ();
  }

  {
    // Checks continuation of message blocks on the stack.
    ACE_Message_Block mb1 (1024);
    ACE_Message_Block mb2 (1024);

    mb1.cont (&mb2);
  }

  {
    // Checks continuation of message blocks on the heap.
    ACE_Message_Block *mb1;
    ACE_Message_Block *mb2;

    ACE_NEW_RETURN (mb1, ACE_Message_Block (1024), -1);
    ACE_NEW_RETURN (mb2, ACE_Message_Block (1024), -1);

    mb1->cont (mb2);
    mb1->release ();
  }

  // Same set of tests but with locking_strategy set.
  {
    ACE_Lock_Adapter <ACE_SYNCH_MUTEX> mutex;
    ACE_Lock *lock = &mutex;

    {
      // Checks normal stack deletes.
      ACE_Message_Block mb;
      mb.locking_strategy (lock);
    }

    {
      // Checks normal heap deletes.
      ACE_Message_Block *mb;
      ACE_NEW_RETURN (mb, ACE_Message_Block, -1);
      mb->locking_strategy (lock);
      mb->release ();
    }

    {
      // Checks continuation of message blocks on the stack with one
      // lock strategy.
      ACE_Message_Block mb1 (1024);
      ACE_Message_Block mb2 (1024);

      mb1.locking_strategy (lock);

      mb1.cont (&mb2);
    }

    {
      // Checks continuation of message blocks on the heap with one
      // lock strategy.
      ACE_Message_Block *mb1;
      ACE_Message_Block *mb2;

      ACE_NEW_RETURN (mb1, ACE_Message_Block (1024), -1);
      ACE_NEW_RETURN (mb2, ACE_Message_Block (1024), -1);

      mb1->locking_strategy (lock);

      mb1->cont (mb2);
      mb1->release ();
    }

    {
      // Checks continuation of message blocks on the stack with two
      // lock strategy.
      ACE_Message_Block mb1 (1024);
      ACE_Message_Block mb2 (1024);

      mb1.locking_strategy (lock);
      mb2.locking_strategy (lock);

      mb1.cont (&mb2);
    }

    {
      // Checks continuation of message blocks on the heap with two
      // lock strategy
      ACE_Message_Block *mb1;
      ACE_Message_Block *mb2;

      ACE_NEW_RETURN (mb1, ACE_Message_Block (1024), -1);
      ACE_NEW_RETURN (mb2, ACE_Message_Block (1024), -1);

      mb1->locking_strategy (lock);
      mb2->locking_strategy (lock);

      mb1->cont (mb2);
      mb1->release ();
    }

    {
      // Checks continuation of message blocks on the heap with two
      // lock strategy where the second one is a duplicate of the
      // first
      ACE_Message_Block *mb1;
      ACE_NEW_RETURN (mb1, ACE_Message_Block (1024), -1);
      mb1->locking_strategy (lock);

      ACE_Message_Block *mb2 = mb1->duplicate ();

      mb1->cont (mb2);
      mb1->release ();
    }
  }

  {
    // Checks continuation of message blocks on the heap with two
    // different lock strategies

    ACE_Lock_Adapter <ACE_SYNCH_MUTEX> lock1;
    ACE_Lock_Adapter <ACE_SYNCH_MUTEX> lock2;

    ACE_Message_Block *mb1;
    ACE_Message_Block *mb2;

    ACE_NEW_RETURN (mb1, ACE_Message_Block (1024), -1);
    ACE_NEW_RETURN (mb2, ACE_Message_Block (1024), -1);

    mb1->locking_strategy (&lock1);
    mb2->locking_strategy (&lock2);

    mb1->cont (mb2);
    mb1->release ();
  }

  ACE_END_TEST;
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Lock_Adapter<ACE_SYNCH_MUTEX>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Lock_Adapter<ACE_SYNCH_MUTEX>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

