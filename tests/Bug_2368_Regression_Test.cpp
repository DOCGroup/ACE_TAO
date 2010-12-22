/**
 * @file Bug_2368_Regression_Test.cpp
 *
 * $Id$
 *
 * Reproduces the problems reported in bug 2368:
 *   http://deuce.doc.wustl.edu/bugzilla/show_bug.cgi?id=2368
 *
 * @author Johnny Willemsen  <jwillemsen@remedy.nl>
 */

#include "test_config.h"
#include "ace/Service_Config.h"
#include "ace/Reactor.h"
#include "ace/Log_Msg.h"
#include "ace/Signal.h"

static bool handleA_close_called = false;
static bool handleB_close_called = false;

class My_HandlerA : public ACE_Event_Handler
{
public:
  virtual int handle_close (ACE_HANDLE,
                            ACE_Reactor_Mask)
  {
    ACE_DEBUG ((LM_DEBUG,
                "Handle close called\n"));
    handleA_close_called = true;

    return 0;
  }

  virtual int handle_signal (int,
                             siginfo_t *,
                             ucontext_t *)
  {
    ACE_DEBUG ((LM_DEBUG,
                "Handle signal called\n"));

    return 0;
  }
};

class My_HandlerB : public ACE_Event_Handler
{
public:
  virtual int handle_close (ACE_HANDLE,
                            ACE_Reactor_Mask)
  {
    ACE_DEBUG ((LM_DEBUG,
                "Handle close called\n"));
    handleB_close_called = true;

    return 0;
  }

  virtual int handle_signal (int,
                             siginfo_t *,
                             ucontext_t *)
  {
    ACE_DEBUG ((LM_DEBUG,
                "Handle signal called\n"));

    return 0;
  }
};

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Bug_2368_Regression_Test"));

  My_HandlerA my_handlerA;
  My_HandlerB my_handlerB;

  // Set up an ACE signal handler.
  if (ACE_Reactor::instance ()->register_handler
      (SIGINT,
       &my_handlerA) == -1)
    ACE_ERROR_RETURN ((LM_DEBUG,
                       "%p\n",
                       "register_handlerA"),
                      -1);

  if (ACE_Reactor::instance ()->register_handler
      (SIGINT,
       &my_handlerB) == -1)
    ACE_ERROR_RETURN ((LM_DEBUG,
                       "%p\n",
                       "register_handlerB"),
                      -1);

  ACE_Sig_Action *new_disp = 0;
  if (ACE_Reactor::instance ()->remove_handler
      (SIGINT,
       new_disp) == -1)
    ACE_ERROR_RETURN ((LM_DEBUG,
                       "%p\n",
                       "remove_handlerB"),
                      -1);

  if (ACE_Reactor::instance ()->close () == -1)
    ACE_ERROR ((LM_ERROR,
                "%p\n",
                "close"));

  if (!handleA_close_called)
    ACE_ERROR ((LM_ERROR,
                "Handle close hasn't been called for A. "
                "This test failure caused by the unresolved bug is EXPECTED!\n"));

  if (!handleB_close_called)
    ACE_ERROR ((LM_ERROR,
                "Handle close hasn't been called for B. "
                "This test failure caused by the unresolved bug is EXPECTED!\n"));

  ACE_END_TEST;

  return 0;
}
