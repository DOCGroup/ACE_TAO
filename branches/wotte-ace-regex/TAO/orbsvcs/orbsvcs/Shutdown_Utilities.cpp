// $Id$

//ACE_RCSID(orbsvcs, Shutdown_Utilities, "$Id$")

#include "Shutdown_Utilities.h"

Service_Shutdown::Service_Shutdown (Shutdown_Functor& sf)
  : functor_(sf)
{
  ACE_Sig_Set std_signals;
#if !defined(ACE_LACKS_UNIX_SIGNALS)
  std_signals.sig_add (SIGINT);
  std_signals.sig_add (SIGTERM);
#endif
  this->set_signals (std_signals);
}

Service_Shutdown::Service_Shutdown (Shutdown_Functor& sf, ACE_Sig_Set& which_signals)
  : functor_(sf)
{
  this->set_signals (which_signals);
}

// It would be nice if we could rely on a portable #define that
// declared the largest signal held in sigset_t, but we can't.
// So, for now, we'll make a possibly bold assumption that sigset_t
// will be at least four bytes.  If somebody wants to use a signal
// greater than that, then they'll have to redefine ACE_NSIG.
//
// It would be even nicer if the register_handler() method just took
// an ACE_Sig_Set as an argument and handled all this stuff itself.
// 
void
Service_Shutdown::set_signals (ACE_Sig_Set& which_signals)
{
  // iterate over all the signals in which_signals and register them...
  int did_register = 0;
  for (int i = 1; i < ACE_NSIG; i++)
    if (which_signals.is_member (i))
      {
        if (this->shutdown_.register_handler (i, this) == -1)
          ACE_DEBUG ((LM_WARNING,
                      "WARNING: Failed to register signal handler for signal %d: %p\n",
                      i));
        else
          did_register = 1;
      }

  if (! did_register)
    ACE_DEBUG ((LM_WARNING,
                "WARNING: Service_Shutdown did not register any signals.\n"));
}

Service_Shutdown::~Service_Shutdown ()
{
}

int
Service_Shutdown::handle_signal (int signum,
                                 siginfo_t*, ucontext_t*)
{
  this->functor_(signum);
  return 0;
}
