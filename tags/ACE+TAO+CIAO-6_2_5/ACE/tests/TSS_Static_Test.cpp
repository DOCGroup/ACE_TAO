
//=============================================================================
/**
 *  @file    TSS_Static_Test.cpp
 *
 *  $Id$
 *
 *   This program tests the interaction between TSS and thread keys
 *   created during static construction.  VxWorks static construction
 *   is quite broken.  This test is designed to test changes to work
 *   around a bug in the VxWorks loader that constructs static objects
 *   multiple times.  It sounds hard to believe, but I've seen it!
 *
 *
 *  @author Chad Elliott <elliott_c@ociweb.com>
 */
//=============================================================================


#include "test_config.h"
#include "ace/ACE.h"
#include "ace/Thread.h"



#if defined (ACE_HAS_TSS_EMULATION)

class Some_Object
{
public:
  Some_Object (void);
  ~Some_Object (void);
};

Some_Object::Some_Object (void)
{
  ACE::init ();

  // Cause the ACE_Log_Msg to be constructed during static construction
  ACE_DEBUG ((LM_DEBUG, ""));

  // Assign something to TSS during static construction
  ACE_thread_key_t key;
  if (ACE_Thread::keycreate (&key, 0) == 0)
    {
      ACE_Thread::setspecific (key, this);
    }
}


Some_Object::~Some_Object (void)
{
  ACE::fini ();
}


static Some_Object sobject;

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("TSS_Static_Test"));

  int status = 0;
  ACE_thread_key_t key;
  if (ACE_Thread::keycreate (&key, 0) == 0)
    {
      void* specific = 0;
      if (ACE_Thread::getspecific (key, &specific) == 0)
        {
          if (specific == 0)
            {
              ACE_DEBUG ((LM_DEBUG, "Got back pointer: %x from key: %d. "
                                    "Good!\n",
                                    (size_t)specific, key));
            }
          else
            {
              ++status;
              ACE_ERROR ((LM_ERROR, "Something (%x) was found in tss "
                                    "slot %d.\n"
                                    "Nothing should be stored in our "
                                    "TSS slot!\n",
                                    (size_t)specific, key));
            }
        }
      else
        {
          ++status;
          ACE_ERROR ((LM_ERROR, "Unable to get the thread specific "
                                "storage.\n"));
        }
    }
  else
    {
      ++status;
      ACE_ERROR ((LM_ERROR, "Unable to create the thread specific "
                            "storage key.\n"));
    }
  ACE_END_TEST;
  return status;
}
#else
int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("TSS_Static_Test"));
  ACE_DEBUG ((LM_INFO, ACE_TEXT ("This test requires TSS Emulation.\n")));
  ACE_END_TEST;
  return 0;
}
#endif /* ACE_HAS_TSS_EMULATION */

