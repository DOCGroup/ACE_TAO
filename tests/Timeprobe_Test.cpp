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
//      This is a simple test of Timeprobes.
//
// = AUTHOR
//    Irfan Pyarali 
//
// ============================================================================

#define ACE_COMPILE_TIMEPROBES
#define ACE_ENABLE_TIMEPROBES
//#define ACE_MT_TIMEPROBES
//#define ACE_TSS_TIMEPROBES

#include "tests/test_config.h"
#include "ace/Timeprobe.h"

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
  "Event Nine",
};

static const char *events_descriptions_1[] = 
{ 
  "Work start",
  "Work end"
};

static void
work (int time)
{
  ACE_FUNCTION_TIMEPROBE (100);
  ACE_OS::sleep (time);
}

int
main (int, ASYS_TCHAR *[])
{
  ACE_START_TEST (ASYS_TEXT ("Timeprobe_Test"));

  ACE_TIMEPROBE ("Staring Test");

  for (int i = 1; i < 3; i++)
    {
      work (i);
      ACE_TIMEPROBE (i);
    }
  
  ACE_TIMEPROBE ("Ending Test");

  ACE_TIMEPROBE_EVENT_DESCRIPTIONS (events_descriptions_0, 0);
  ACE_TIMEPROBE_EVENT_DESCRIPTIONS (events_descriptions_1, 100);

  ACE_TIMEPROBE_PRINT;

  return 0;
}

