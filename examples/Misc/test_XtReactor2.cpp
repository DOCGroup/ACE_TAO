// $Id$

// The following test exercises the Eric C. Newton's <ecn@clark.net>
// XtReactor implementation.

#include "ace/XtReactor.h"
#include "ace/Reactor.h"
#include "ace/Message_Block.h"

ACE_RCSID(Misc, test_XtReactor2, "$Id$")

#if defined (ACE_HAS_XT)
#define String XtString
#include <Xm/PushB.h>

class Stdin : public ACE_Event_Handler
{
public:
  ACE_HANDLE get_handle (void) const { return ACE_STDIN; }

  int handle_input (ACE_HANDLE fd)
  {
    char c;
    if (ACE_OS::read (ACE_STDIN, &c, 1)==1)
      ACE_DEBUG ((LM_DEBUG,
                  "Got input '%d'\n",
                  (int) c));
    return 0;
  }

  int handle_timeout (const ACE_Time_Value &tv,
                      const void *arg)
  {
    ACE_DEBUG ((LM_DEBUG,
                "Timeout! %f\n",
                (double) (tv.msec ()/1000.)));
    return 0;
  }
};

static void
ActivateCB (Widget w, XtPointer, XtPointer)
{
  ACE_DEBUG ((LM_DEBUG,
              "Button pushed!\n"));
}

int
main (int argc, ACE_TCHAR**argv)
{
  // The worlds most useless user interface
  Widget top_level = XtVaAppInitialize (NULL,
                                        "buttontest",
                                        NULL,
                                        0,
					&argc,
                                        argv,
                                        NULL,
                                        NULL);
  Widget button = XmCreatePushButton (top_level,
                                      "change",
                                      0,
                                      0);
  XtManageChild (button);
  XtAddCallback (button,
                 XmNactivateCallback,
                 ActivateCB,
                 NULL);

  // A reactor beastie.
  ACE_XtReactor xreactor (XtWidgetToApplicationContext (top_level));
  ACE_XtReactor reactor (&xreactor);

  // Print a message when data is recv'd on stdin...
  ACE_Event_Handler * stdin_;
  ACE_NEW_RETURN (stdin_,
                  Stdin,
                  -1);
  reactor.register_handler (stdin_,
                            ACE_Event_Handler::READ_MASK);

  // Print a message every 10 seconds
  if (reactor.schedule_timer (stdin_, 0,
			      ACE_Time_Value (10),
			      ACE_Time_Value (10)) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "schedule_timer"), -1);

  // Show the top_level widget
  XtRealizeWidget (top_level);

  // Demonstrate Reactor/Xt event loop unification:
  XtAppMainLoop (XtWidgetToApplicationContext (top_level));
}
#else
int
main (int, ACE_TCHAR *[])
{
  ACE_ERROR_RETURN ((LM_ERROR,
                     "XT not configured for this platform\n"),
                    0);
}
#endif /* ACE_HAS_XT */
