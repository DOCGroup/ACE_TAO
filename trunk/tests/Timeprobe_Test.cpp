// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Timeprobe_Test.cpp
//
// = DESCRIPTION
//      This is a simple test of ACE Timeprobes.
//
// = AUTHOR
//    Irfan Pyarali
//
// ============================================================================

//#define ACE_ENABLE_TIMEPROBES
//#define ACE_MT_TIMEPROBES
//#define ACE_TSS_TIMEPROBES

#include "tests/test_config.h"
#include "ace/Timeprobe.h"

ACE_RCSID(tests, Timeprobe_Test, "$Id$")

#if defined(__BORLANDC__) && __BORLANDC__ >= 0x0530
USELIB("..\ace\aced.lib");
//---------------------------------------------------------------------------
#endif /* defined(__BORLANDC__) && __BORLANDC__ >= 0x0530 */

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

int
main (int, ASYS_TCHAR *[])
{
  ACE_START_TEST (ASYS_TEXT ("Timeprobe_Test"));

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
