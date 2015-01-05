// $Id$

// The following is another test that exercises the Eric C. Newton's
// <ecn@clark.net> XtReactor implementation.

#include "ace/OS_main.h"
#include "ace/XtReactor/XtReactor.h"
#include "ace/Reactor.h"
#include "ace/Message_Block.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_fcntl.h"

#include <Xm/PushB.h>

class Stdout : public ACE_Event_Handler
{
public:
  Stdout (ACE_Reactor * r)
    : reactor_ (r),
      msg_ (1000000) // Make a very big message block.
  {
    int flags = ACE_OS::fcntl (ACE_STDOUT, F_GETFL);

    if (flags != -1
        && ACE_OS::fcntl (ACE_STDOUT,
                          F_SETFL, flags | O_NONBLOCK) != -1)
      return;
    else
      ACE_DEBUG ((LM_DEBUG,
                  "Unable to set stdout to non-block."));
  }

  ACE_HANDLE get_handle (void) const { return ACE_STDOUT; }

  int handle_output (ACE_HANDLE)
  {
    char *s = msg_.rd_ptr ();

    if (ACE_OS::write (ACE_STDOUT, s, 1) == 1)
      {
        ACE_DEBUG ((LM_DEBUG,
                    "wrote output '%d'\n",
                    (int) *s));
        msg_.rd_ptr (1);
      }

    if (msg_.length () == 0)
      {
        reactor_->remove_handler (this,
                                  ACE_Event_Handler::WRITE_MASK);
        msg_.rd_ptr (msg_.base ());
        msg_.wr_ptr (msg_.base ());
      }
    return 0;
  }

  void put (char c)
  {
    if (msg_.length () == 0)
      reactor_->register_handler (this,
                                  ACE_Event_Handler::WRITE_MASK);

    if (msg_.wr_ptr () < msg_.end ())
      {
        *msg_.wr_ptr () = c;
        msg_.wr_ptr (1);
      }
    else
      ACE_DEBUG ((LM_DEBUG,
                  "Oops... data falling off the end of the buffer!\n"));
  }

private:
  ACE_Reactor *reactor_;
  ACE_Message_Block msg_;
};

class Stdin : public ACE_Event_Handler
{
public:
  Stdin (Stdout *out)
    : out_ (out) {}

  ACE_HANDLE get_handle () const { return ACE_STDIN; }

  int handle_input (ACE_HANDLE)
  {
    char c;

    if (ACE_OS::read (ACE_STDIN, &c, 1) == 1)
      out_->put (c);

    return 0;
  }

  int handle_timeout (const ACE_Time_Value &tv, const void *)
  {
    ACE_DEBUG ((LM_DEBUG,
                "Timeout! %f\n",
                (double) (tv.msec () / 1000.)));
    return 0;
  }

private:
  Stdout *out_;
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
  Widget top_level = XtVaAppInitialize (0,
                                        "buttontest",
                                        0,
                                        0,
                                        &argc,
                                        argv,
                                        0,
                                        static_cast<void *>(0));
  char change[] = "change"; // XmCreatePushButton() wants a non-const
                            // string.
  Widget button = XmCreatePushButton (top_level,
                                      change,
                                      0,
                                      0);
  XtManageChild (button);
  XtAddCallback (button,
                 XmNactivateCallback,
                 ActivateCB,
                 0);

  // A reactor beastie.
  ACE_XtReactor xreactor (XtWidgetToApplicationContext (top_level));
  ACE_Reactor reactor (&xreactor);

  // Print a message when data is recv'd on stdin...
  ACE_Event_Handler *stdin_;
  ACE_NEW_RETURN (stdin_,
                  Stdin (new Stdout (&reactor)),
                  -1);
  reactor.register_handler (stdin_,
                            ACE_Event_Handler::READ_MASK);

  // Print a message every 10 seconds.
  if (reactor.schedule_timer (stdin_, 0,
                              ACE_Time_Value (10),
                              ACE_Time_Value (10)) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "schedule_timer"),
                      -1);

  // Show the top_level widget.
  XtRealizeWidget (top_level);

  // Demonstrate Reactor/Xt event loop unification.
  XtAppMainLoop (XtWidgetToApplicationContext (top_level));

  return 0;
}
