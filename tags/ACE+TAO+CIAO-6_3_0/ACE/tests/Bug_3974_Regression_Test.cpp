/**
 * @file Bug_3974_Regression_Test.cpp
 *
 * $Id$
 *
 * Reproduces the problems reported in bug 3974
 *   http://deuce.doc.wustl.edu/bugzilla/show_bug.cgi?id=3974
 */

#include "test_config.h"

#include "ace/Dev_Poll_Reactor.h"
#include "ace/Select_Reactor.h"
#include "ace/TP_Reactor.h"
#include "ace/WFMO_Reactor.h"
#include "ace/OS_NS_errno.h"

bool
testit (ACE_Reactor_Impl *ri)
{
  int ret = 0;
  ACE_Reactor r (ri);
  ACE_Time_Value one (1);
  r.end_reactor_event_loop ();
  if ((ret = r.handle_events (one)) != -1)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Return value %d should be -1\n"), ret));
      return false;
    }
  if (errno != ESHUTDOWN)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("errno %d should be %d (ESHUTDOWN)\n"),
                  errno, ESHUTDOWN));
      return false;
    }

  return true;
}

// Main function.

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Bug_3974_Regression_Test"));
  int result = 0;

  ACE_Select_Reactor r1;
  if (!testit (&r1))
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Select_Reactor failed\n")));
      result = 1;
    }

  ACE_TP_Reactor r2;
  if (!testit (&r2))
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("TP_Reactor failed\n")));
      result = 1;
    }

#ifdef ACE_WIN32
  ACE_WFMO_Reactor r3;
  if (!testit (&r3))
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("WFMO_Reactor failed\n")));
      result = 1;
    }
#endif /* ACE_WIN32 */

#if defined (ACE_HAS_EVENT_POLL) || defined (ACE_HAS_DEV_POLL)
  ACE_Dev_Poll_Reactor r4;
  if (!testit (&r4))
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Dev_Poll_Reactor failed\n")));
      result = 1;
    }
#endif /* ACE_HAS_EVENT_POLL || ACE_HAS_DEV_POLL */

  ACE_END_TEST;
  return result;
}

