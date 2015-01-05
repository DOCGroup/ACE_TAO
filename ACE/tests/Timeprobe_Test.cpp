
//=============================================================================
/**
 *  @file    Timeprobe_Test.cpp
 *
 *  $Id$
 *
 *    This is a simple test of ACE Timeprobes.
 *
 *
 *  @author Irfan Pyarali <irfan@cs.wustl.edu>
 */
//=============================================================================


//#define ACE_ENABLE_TIMEPROBES
//#define ACE_MT_TIMEPROBES
//#define ACE_TSS_TIMEPROBES

#include "test_config.h"
#include "ace/Singleton.h"
#include "ace/Synch_Traits.h"
#include "ace/Recursive_Thread_Mutex.h"
#include "ace/Null_Mutex.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Timeprobe.h"



#if defined (ACE_ENABLE_TIMEPROBES)

static const char *events_descriptions_0[] =
{
  "Event Zero",
  "Event One",
  "Event Two",
  "Event Three",
  "Event Four",
  "Event Five",
  "Event Six",
  "Event Seven",
  "Event Eight",
  "Event Nine"
};

enum
{
  EVENT_ZERO = 0,
  EVENT_ONE,
  EVENT_TWO,
  EVENT_THREE,
  EVENT_FOUR,
  EVENT_FIVE,
  EVENT_SIX,
  EVENT_SEVEN,
  EVENT_EIGHT,
  EVENT_NINE
};

static const char *events_descriptions_1[] =
{
  "Work start",
  "Work end"
};

enum
{
  WORK_START = 100,
  WORK_END
};

ACE_TIMEPROBE_EVENT_DESCRIPTIONS (events_descriptions_1, WORK_START);
ACE_TIMEPROBE_EVENT_DESCRIPTIONS (events_descriptions_0, EVENT_ZERO);

#endif /* ACE_ENABLE_TIMEPROBES */

static void
work (int time)
{
  ACE_FUNCTION_TIMEPROBE (WORK_START);
  ACE_OS::sleep (time);
}

#if !defined (ACE_HAS_PURIFY)
// Test creation of ACE_Singletons during static object construction.
// Timeprobes can do that, when they're enabled.  Purify would notice
// the memory in use at program termination.
static int
create_singleton ()
{
  int *i = ACE_Singleton <int, ACE_SYNCH_RECURSIVE_MUTEX>::instance ();
  *i = 3;

  return *i;
}

int static_singleton_creator = create_singleton ();
#endif /* ! ACE_HAS_PURIFY */

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Timeprobe_Test"));

  ACE_TIMEPROBE ("Starting Test");

  for (int i = 0; i < 3; i++)
    {
      work (i);
      ACE_TIMEPROBE (EVENT_ZERO + i);
    }

  ACE_TIMEPROBE ("Ending Test");

  ACE_TIMEPROBE_PRINT;

  ACE_END_TEST;

  return 0;
}

