// $Id$

#include "orbsvcs/Shutdown_Utilities.h"
#include "orbsvcs/Log_Macros.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

Service_Shutdown::Service_Shutdown (Shutdown_Functor& sf)
  : functor_(sf)
{
  ACE_Sig_Set std_signals;
#if !defined(ACE_LACKS_UNIX_SIGNALS)
  std_signals.sig_add (SIGTERM);
  std_signals.sig_add (SIGINT);
#elif defined(ACE_WIN32)
  std_signals.sig_add (SIGINT);
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
  bool did_register = false;
  for (int i = 1; i < ACE_NSIG; ++i)
  {
    if (which_signals.is_member (i))
      {
        if (this->shutdown_.register_handler (i, this) == -1)
          {
#if defined(__TANDEM)
// Tandem NSK platform has no signal 10 so do not emit a warning for it
            if (i != 10)
#endif
              {
                ORBSVCS_DEBUG ((LM_WARNING,
                            "WARNING: Failed to register signal handler "
                            "for signal %d: %p\n",
                            i, ACE_TEXT ("register_handler")));
              }
          }
        else
          {
            // Store that we have registered for this signal
            // we have to unregister later for just these signals
            this->registered_signals_.sig_add (i);
            did_register = true;
          }
      }
  }
  if (! did_register)
  {
    ORBSVCS_DEBUG ((LM_WARNING,
                "WARNING: Service_Shutdown did not register any signals.\n"));
  }
}

Service_Shutdown::~Service_Shutdown ()
{
  for (int i = 1; i < ACE_NSIG; ++i)
    {
      if (this->registered_signals_.is_member (i))
        {
          this->shutdown_.remove_handler(i);
        }
    }
}

int
Service_Shutdown::handle_signal (int signum, siginfo_t*, ucontext_t*)
{
  this->functor_(signum);
  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
