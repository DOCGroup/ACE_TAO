// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = DESCRIPTION
//    This program exercises the ACE_Stack_Trace class.
//
// = AUTHOR
//    Chris Cleeland <cleeland @ ociweb . com>
//
// ============================================================================

#include "ace/Stack_Trace.h"
#include "ace/OS_NS_string.h"
#include "test_config.h"



/*
 * Ultra-basic test of stack trace.
 *
 * Other things to test:
 * - throwing exceptions that contain the stack trace, and catching
 * - capturing multiple traces
 * - capture traces with different numbers of frames
 * - capture traces with different numbers of skipped frames
 */


struct SomeException
{
  static const ssize_t SKIP = 1; //@TODO: #ifdef this for MACOSX which needs 2
  SomeException () : where_(SKIP), explanation_ ("") { }
  SomeException (const char *explanation)
    : where_(SKIP), explanation_(explanation) { }
  ACE_Stack_Trace where_;
  const char *explanation_;
};

void
func ()
{
  throw SomeException();
}

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Stack_Trace_Test"));

  ACE_Stack_Trace t1;
  ACE_DEBUG ((LM_DEBUG, "t1 trace is %C\n", t1.c_str()));

  ACE_Stack_Trace t2(t1);
  ACE_DEBUG ((LM_DEBUG, "t2 (copied) trace is %C\n",
        (ACE_OS::strcmp (t1.c_str(), t2.c_str()) == 0) ? "same" : "different"));

  ACE_Stack_Trace t3;
  ACE_DEBUG ((LM_DEBUG, "t3 trace before assignment is %C\n",
        (ACE_OS::strcmp (t1.c_str(), t3.c_str()) == 0) ? "same" : "different"));
  t3 = t1;
  ACE_DEBUG ((LM_DEBUG, "t3 trace after assignment is %C\n",
        (ACE_OS::strcmp (t1.c_str(), t3.c_str()) == 0) ? "same" : "different"));

  try {
    func();
  }
  catch (SomeException& e) {
    ACE_DEBUG ((LM_DEBUG,
    "SomeException caught at\n%?\n; thrown at\n%C",
    e.where_.c_str()));
  }

  ACE_Stack_Trace one_frame_only(0, 1);
  ACE_DEBUG ((LM_DEBUG,
              "stack trace of only one frame:\n%C",
              one_frame_only.c_str()));

  ACE_DEBUG ((LM_DEBUG, "getting ready to end the test at %?\n"));

  ACE_END_TEST;
  return 0;
}
