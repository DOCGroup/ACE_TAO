// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Sig_Handlers_Test.cpp
//
// = DESCRIPTION
//    This is a simple program that tests whether the ACE_Sig_Handlers
//    class works properly.  To run this test, start the application
//    and then type ^C.  If everything is working properly the test
//    will shutdown gracefully.
//
// = AUTHOR
//    Douglas C. Schmidt <schmidt@dre.vanderbilt.edu> and Andreas Drescher <ace at anticat dot ch>
//
// ============================================================================

#include "test_config.h"
#include "ace/Reactor.h"
#include "ace/WFMO_Reactor.h"
#include "ace/Select_Reactor.h"
#include "ace/Log_Msg.h"
#include "ace/Signal.h"
#include "ace/Assert.h"
#include "ace/SString.h"



class Test_SIGINT_Handler : public ACE_Event_Handler
{
public:
  Test_SIGINT_Handler (ACE_Reactor *reactor, const char *message)
    : message_ (message)
  {
    int result = reactor->register_handler (SIGINT, this);
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT("Main::Test_SIGINT_Handler (%u) - Result %i\n"),
                this,
                result));
    Test_SIGINT_Handler::registration_count_++;
  }

  ~Test_SIGINT_Handler()
  {
    ACE_ASSERT (Test_SIGINT_Handler::handle_signal_count_ == Test_SIGINT_Handler::registration_count_);
  }

  virtual int handle_signal (int signal, siginfo_t *, ucontext_t *)
  {
    ACE_ASSERT (signal == SIGINT);
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT("Main::Test_SIGINT_Handler (%u) - %s\n"),
                this,
                this->message_.c_str()));
    Test_SIGINT_Handler::handle_signal_count_++;
    return 0;
  }

private:
  ACE_CString message_;

  static int handle_signal_count_;

  static int registration_count_;
};

int Test_SIGINT_Handler::handle_signal_count_ = 0;
int Test_SIGINT_Handler::registration_count_ = 0;

class Test_SIGINT_Shutdown_Handler : public ACE_Event_Handler
{
public:
  Test_SIGINT_Shutdown_Handler (ACE_Reactor *reactor)
  {
    int result = reactor->register_handler (SIGINT, this);
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT("Main::Test_SIGINT_Shutdown_Handler (%u) - Result %i\n"),
                this,
                result));
  }

  virtual int handle_signal (int signal, siginfo_t *, ucontext_t *)
  {
    ACE_ASSERT (signal == SIGINT);
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT("Main::Test_SIGINT_Shutdown_Handler (%u)\n"),
                this));
    ACE_Reactor::instance ()->end_reactor_event_loop ();
    return 0;
  }
};

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Sig_Handlers_Test"));

  ACE_Sig_Handlers multi_handlers;
  ACE_Select_Reactor reactor_impl (&multi_handlers);
  ACE_Reactor reactor (&reactor_impl);
  ACE_Reactor::instance (&reactor);

  // Create a bevy of handlers.

  Test_SIGINT_Handler h1 (ACE_Reactor::instance (), "h1");
  Test_SIGINT_Handler h2 (ACE_Reactor::instance (), "h2");
  Test_SIGINT_Handler h3 (ACE_Reactor::instance (), "h3");
  Test_SIGINT_Handler h4 (ACE_Reactor::instance (), "h4");
  Test_SIGINT_Handler h5 (ACE_Reactor::instance (), "h5");
  Test_SIGINT_Handler h6 (ACE_Reactor::instance (), "h6");
  Test_SIGINT_Handler h7 (ACE_Reactor::instance (), "h7");
  Test_SIGINT_Handler h8 (ACE_Reactor::instance (), "h8");

  Test_SIGINT_Shutdown_Handler h0 (ACE_Reactor::instance ());

  // Wait for user to type SIGINT.
  while (!ACE_Reactor::instance ()->reactor_event_loop_done ())
    {
      ACE_DEBUG ((LM_DEBUG,"\nwaiting for SIGINT\n"));
      if (ACE_Reactor::instance ()->handle_events () == -1)
        ACE_ERROR ((LM_ERROR,"%p\n","handle_events"));
    }

  ACE_END_TEST;
  return 0;
}
