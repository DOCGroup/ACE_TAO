// $Id$

// Test the ability of the Reactor/Signal_Handler to register multiple
// handler per-signal.

/*  This test works as follows:

    1. To test the "original" semantics of ACE (i.e., only one
    ACE_Event_Handler can be registered per signal), you don't
    need to do anything special.  Existing programs work the
    same since giving the Reactor's constructor a 0 value
    (which is the default argument, BTW) instructs it to behave
    as before.  When a 0 is given, the ACE_Reactor's
    constructor/open method creates an instance of
    ACE_Sig_Handler and assigns this to an internal pointer.
    This pointer is then used to dispatch all signal-related
    methods within the Reactor.  The default ACE_Sig_Handler
    only allows *one* ACE_Event_Handler to be registered
    per-signal.

    To run this version of the test do the following:

    % ./test-signal
    ./test_signals
    waiting for SIGINT or SIGQUIT
    ^C
    signal Interrupt occurred in Sig_Handler_2 (fruity, 0, 0) with count = 1
    waiting for SIGINT or SIGQUIT
    ^\
    signal Quit occurred in Sig_Handler_2 (fruity, 0, 0) with count = 2
    shutting down SIGQUIT in Sig_Handler_2 (fruity, 0, 0)
    waiting for SIGINT or SIGQUIT
    ^C
    signal Interrupt occurred in Sig_Handler_2 (fruity, 0, 0) with count = 3
    waiting for SIGINT or SIGQUIT
    ^\Quit (core dumped)

           Note that in this test only one handler (the last one --
    "Sig_Handler_2 (fruity)") is actually registered.  BTW, the
    core dump is the expected behavior since the default
    disposition is restored when there are no more handlers
    (see the code below).

    2. To test the "multiple handlers per-signal semantics", you
    need to pass the constructor/open method of the ACE_Reactor
    a pointer to a an instance of ACE_Sig_Handlers (note the
    plural "s").  ACE_Sig_Handlers is a class that derives from
    ACE_Sig_Handler.  The difference between these two classes
    is that (1) ACE_Sig_Handlers::register_signal allows
    multiple ACE_Event_Handlers to be registered per-signal and
    (2) it enables SA_RESTART by default.  This class also
    implements Detlef Becker's algorithm for integrating ACE
    signal handling with 3rd party libraries.

    To run this version of the test do the following:

    % ./test_signals 1

    waiting for SIGINT or SIGQUIT
    ^C
    signal Interrupt occurred in external handler!
    signal Interrupt occurred in Sig_Handler_1 (howdy, 3, 1) with count = 1
    shutting down SIGINT in Sig_Handler_1 (howdy, 3, 1)
    signal Interrupt occurred in Sig_Handler_1 (doody, 5, 4) with count = 1
    shutting down SIGINT in Sig_Handler_1 (doody, 5, 4)
    signal Interrupt occurred in Sig_Handler_2 (tutty, 7, 6) with count = 1
    signal Interrupt occurred in Sig_Handler_2 (fruity, 9, 8) with count = 1
    waiting for SIGINT or SIGQUIT
    ^\
    signal Quit occurred in Sig_Handler_1 (howdy, 3, 1) with count = 2
    shutting down SIGQUIT in Sig_Handler_1 (howdy, 3, 1)
    signal Quit occurred in Sig_Handler_1 (doody, 5, 4) with count = 2
    shutting down SIGQUIT in Sig_Handler_1 (doody, 5, 4)
    signal Quit occurred in Sig_Handler_2 (tutty, 7, 6) with count = 2
    shutting down SIGQUIT in Sig_Handler_2 (tutty, 7, 6)
    signal Quit occurred in Sig_Handler_2 (fruity, 9, 8) with count = 2
    shutting down SIGQUIT in Sig_Handler_2 (fruity, 9, 8)
    waiting for SIGINT or SIGQUIT
    ^C
    signal Interrupt occurred in external handler!
    signal Interrupt occurred in Sig_Handler_2 (tutty, 7, 6) with count = 3
    signal Interrupt occurred in Sig_Handler_2 (fruity, 9, 8) with count = 3
    waiting for SIGINT or SIGQUIT
    ^\Quit (core dumped)

    When this test begins all four handlers are registered and
    dispatched when a SIGINT or SIGQUIT occurs.  After the
    first SIGINT, the handle_signal method of the Sig_Handler_1
    objects unregister themselves.  At that point there are 4
    SIGQUIT handlers left, but only 2 of our SIGINT handlers
    left (and the 1 external handler).  After the first
    SIGQUIT, there are no SIGQUIT handlers left since they all
    deregister themselves (which restores the "SIG_DFL"
    disposition).  On the second SIGINT there are only 3
    handlers left (2 of ours and 1 external).  Finally, on the
    second SIGQUIT we exit and dump core since that's what
    happens with the default disposition for SIGQUIT. */


#include "ace/Reactor.h"
#include "ace/WFMO_Reactor.h"
#include "ace/Select_Reactor.h"
#include "ace/Log_Msg.h"
#include "ace/Signal.h"



class Sig_Handler_1 : public ACE_Event_Handler
{
public:
  Sig_Handler_1 (ACE_Reactor &reactor, const char *msg)
    : msg_ (msg),
      count_ (0),
      reactor_ (reactor)
  {
    // Register the signal handlers.
    this->quit_sigkey_ =
      reactor.register_handler (SIGQUIT, this);
    this->int_sigkey_ =
      reactor.register_handler (SIGINT, this);

    if (this->quit_sigkey_ == -1 || this->int_sigkey_ == -1)
      ACE_ERROR ((LM_ERROR,
                  "%p\n",
                  "register_handler"));
  }

  // @@ Note that this code is not portable to all OS platforms since
  // it does print statements within the signal handler.
  virtual int handle_signal (int signum,
                             siginfo_t *,
                             ucontext_t *)
  {
    this->count_++;
    ACE_DEBUG ((LM_DEBUG,
                "\nsignal %S occurred in Sig_Handler_1 (%s, %d, %d) with count = %d",
                signum,
                this->msg_,
                this->int_sigkey_,
                this->quit_sigkey_,
                this->count_));

    if (this->count_ != 1 && signum == SIGQUIT)
      {
        if (this->reactor_.remove_handler (SIGQUIT,
                                           0,
                                           0,
                                           this->quit_sigkey_) == -1)
          ACE_ERROR ((LM_ERROR,
                      "\n%p",
                      "remove_handler"));
        else
          ACE_DEBUG ((LM_DEBUG,
                      "\nshutting down SIGQUIT in Sig_Handler_1 (%s, %d, %d)",
                      this->msg_,
                      this->int_sigkey_,
                      this->quit_sigkey_));
      }
    else if (this->count_ != 2 && signum == SIGINT)
      {
        if (this->reactor_.remove_handler (SIGINT,
                                           0,
                                           0,
                                           this->int_sigkey_) == -1)
          ACE_ERROR ((LM_ERROR,
                      "\n%p",
                      "remove_handler"));
        else
          ACE_DEBUG ((LM_DEBUG,
                      "\nshutting down SIGINT in Sig_Handler_1 (%s, %d, %d)",
                      this->msg_,
                      this->int_sigkey_,
                      this->quit_sigkey_));
      }
    return 0;
  }

protected:
  const char *msg_;
  int count_;
  int int_sigkey_;
  int quit_sigkey_;
  ACE_Reactor &reactor_;
};

class Sig_Handler_2 : public Sig_Handler_1
{
public:
  Sig_Handler_2 (ACE_Reactor &reactor, const char *msg)
    : Sig_Handler_1 (reactor, msg)
  {
  }

  virtual int handle_signal (int signum,
                             siginfo_t *,
                             ucontext_t *)
  {
    this->count_++;
    ACE_DEBUG ((LM_DEBUG,
                "\nsignal %S occurred in Sig_Handler_2 (%s, %d, %d) with count = %d",
                signum,
                this->msg_,
                this->int_sigkey_,
                this->quit_sigkey_,
                this->count_));
    if (this->count_ != 0 && signum == SIGQUIT)
      {
        if (this->reactor_.remove_handler (SIGQUIT, 0, 0, this->quit_sigkey_) == -1)
          ACE_ERROR ((LM_ERROR,
                      "\n%p",
                      "remove_handler"));
          else
            ACE_DEBUG ((LM_DEBUG,
                        "\nshutting down SIGQUIT in Sig_Handler_2 (%s, %d, %d)",
                        this->msg_,
                        this->int_sigkey_,
                        this->quit_sigkey_));
      }
    return 0;
  }
};

static void
external_handler (int signum)
{
  ACE_DEBUG ((LM_DEBUG,
              "\nsignal %S occurred in external handler!",
              signum));
}

int
ACE_TMAIN (int argc, ACE_TCHAR *[])
{
  // If argc > 1 then allow multiple handlers per-signal, else just
  // allow 1 handler per-signal.
  ACE_Sig_Handlers multi_handlers;

#if defined (ACE_WIN32)
  ACE_WFMO_Reactor reactor_impl (argc > 1
                                 ? &multi_handlers
                                 : (ACE_Sig_Handler *) 0);
#else
  ACE_Select_Reactor reactor_impl (argc > 1
                                   ? &multi_handlers
                                   : (ACE_Sig_Handler *) 0);
#endif /* ACE_WIN32 */
  ACE_Reactor reactor (&reactor_impl);

  if (argc > 1)
    {
      // Register an "external" signal handler so that the
      // ACE_Sig_Handlers code will have something to incorporate!

      ACE_SignalHandler eh = (ACE_SignalHandler) external_handler;
      ACE_Sig_Action sa (eh);

      sa.register_action (SIGINT);
    }

  // Create a bevy of handlers.
  Sig_Handler_1 h1 (reactor, "howdy");
  Sig_Handler_1 h2 (reactor, "doody");
  Sig_Handler_2 h3 (reactor, "tutty");
  Sig_Handler_2 h4 (reactor, "fruity");

  // Wait for user to type SIGINT and SIGQUIT.

  for (;;)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "\nwaiting for SIGINT or SIGQUIT\n"));

      if (reactor.handle_events () == -1)
        ACE_ERROR ((LM_ERROR,
                    "%p\n",
                    "handle_events"));
    }

  ACE_NOTREACHED (return 0);
}
