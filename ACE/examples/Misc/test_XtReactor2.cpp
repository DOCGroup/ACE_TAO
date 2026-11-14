// The following test exercises the Eric C. Newton's <ecn@clark.net>
// XtReactor implementation.

#include "ace/OS_main.h"
#include "ace/XtReactor/XtReactor.h"
#include "ace/Reactor.h"
#include "ace/Message_Block.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_unistd.h"

#include <Xm/PushB.h>

class Stdin : public ACE_Event_Handler
{
public:
  ACE_HANDLE get_handle () const override { return ACE_STDIN; }

  int handle_input (ACE_HANDLE) override
  {
    char c;
    if (ACE_OS::read (ACE_STDIN, &c, 1) == 1)
      ACE_DEBUG ((LM_DEBUG,
                  "Got input '%d'\n",
                  static_cast<int> (c)));
    return 0;
  }

  int handle_timeout (ACE_Time_Value const &tv,
                      void const *) override
  {
    ACE_DEBUG ((LM_DEBUG,
                "Timeout! %f\n",
                static_cast<double> (tv.msec () / 1000.)));
    return 0;
  }

  Stdin (Stdin const &) = delete;
  Stdin (Stdin &&) = delete;
  Stdin &operator= (Stdin const &) = delete;
  Stdin &operator= (Stdin &&) = delete;
};

static void
ActivateCB (Widget, XtPointer, XtPointer)
{
  ACE_DEBUG ((LM_DEBUG,
              "Button pushed!\n"));
}

int
ACE_TMAIN (int argc, ACE_TCHAR**argv)
{
  // The worlds most useless user interface
  Widget const top_level = XtVaAppInitialize (nullptr,
                                        "buttontest",
                                        nullptr,
                                        0,
                                        &argc,
                                        argv,
                                        nullptr,
                                        static_cast<void *> (nullptr));
  char change[] = "change"; // XmCreatePushButton() wants a non-const
                            // string.
  Widget const button = XmCreatePushButton (top_level,
                                      change,
                                      nullptr,
                                      0);
  XtManageChild (button);
  XtAddCallback (button,
                 XmNactivateCallback,
                 ActivateCB,
                 nullptr);

  // A reactor beastie.
  ACE_XtReactor xreactor (XtWidgetToApplicationContext (top_level));
  ACE_Reactor reactor (&xreactor);

  // Print a message when data is recv'd on stdin...
  Stdin stdin_handler;
  reactor.register_handler (&stdin_handler,
                            ACE_Event_Handler::READ_MASK);

  // Print a message every 10 seconds
  if (reactor.schedule_timer (&stdin_handler, nullptr,
                              ACE_Time_Value (10),
                              ACE_Time_Value (10)) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "schedule_timer"), -1);

  // Show the top_level widget
  XtRealizeWidget (top_level);

  // Demonstrate Reactor/Xt event loop unification:
  XtAppMainLoop (XtWidgetToApplicationContext (top_level));

  reactor.cancel_timer (&stdin_handler);
  reactor.remove_handler (&stdin_handler, ACE_Event_Handler::READ_MASK);
}
