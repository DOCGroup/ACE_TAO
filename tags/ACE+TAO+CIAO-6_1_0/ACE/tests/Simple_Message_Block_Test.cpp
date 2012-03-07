
//=============================================================================
/**
 *  @file    Simple_Message_Block_Test.cpp
 *
 *  $Id$
 *
 *    This test program is a torture test that illustrates how
 *    ACE_Message_Block reference counting works, how and when locks
 *    are used, how memory is managed, and how continuation chains
 *    of message blocks are made. Ideally used with purify :-)
 *
 *
 *  @author Irfan Pyarali <irfan@cs.wustl.edu>
 */
//=============================================================================


#include "test_config.h"
#include "ace/Message_Block.h"
#include "ace/Synch_Traits.h"
#include "ace/Lock_Adapter_T.h"
#include "ace/OS_NS_string.h"
#include "ace/Thread_Mutex.h"



int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Simple_Message_Block_Test"));

  {
    // Checks normal stack deletes.
    ACE_Message_Block mb;
  }

  {
    // Checks normal heap deletes.
    ACE_Message_Block *mb = 0;

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
      ACE_Message_Block *mb = 0;
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

  {
    // Checks failure of copy when "virtual" allocation (using mark)
    // is too small
    char message[]="abcdefghijklmnop";
    ACE_Message_Block mb1 (ACE_OS::strlen (message) + 1);
    ACE_Message_Block mb2 (ACE_OS::strlen (message) + 1);

    // Resize mb2 so that we mark for use less than the allocated buffer
    if (mb2.size (ACE_OS::strlen (message) + 1 - 10) == -1)
      {
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("(%P|%t) Resize test failed ..\n")));
      }

    // We expect this to succeed
    if (mb1.copy (message, ACE_OS::strlen (message) + 1) == -1)
      {
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("(%P|%t) Copy test failed ..\n")));
      }

    // We expect this to fail
    if (mb2.copy (message, ACE_OS::strlen (message) + 1) != -1)
      {
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("(%P|%t) Copy test succeeded when it should have failed ..\n")));
      }

    // We also expect this to fail
    if (mb2.copy (message) != -1)
      {
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("(%P|%t) Copy test succeeded when it should have failed ..\n")));
      }
  }
  ACE_END_TEST;
  return 0;
}

