// Signal.cpp
// $Id$

#define ACE_BUILD_DLL
#include "ace/Synch_T.h"
#include "ace/Signal.h"
#include "ace/Object_Manager.h"

#if !defined (__ACE_INLINE__)
#include "ace/Signal.i"
#endif /* __ACE_INLINE__ */

// Static definitions.

#if defined (ACE_HAS_SIG_C_FUNC)
extern "C" void
ace_sig_handler_dispatch (int signum, siginfo_t *info, ucontext_t *context)
{
  ACE_TRACE ("ace_signal_handler_dispatch");
  ACE_Sig_Handler::dispatch (signum, info, context);
}

static ACE_SignalHandler ace_signal_handler_dispatcher = ACE_SignalHandler (ace_sig_handler_dispatch);

#if !defined (ACE_HAS_BROKEN_HPUX_TEMPLATES)
extern "C" void
ace_sig_handlers_dispatch (int signum, siginfo_t *info, ucontext_t *context)
{
  ACE_TRACE ("ace_signal_handlers_dispatch");
  ACE_Sig_Handlers::dispatch (signum, info, context);
}

static ACE_SignalHandler ace_signal_handlers_dispatcher = ACE_SignalHandler (ace_sig_handlers_dispatch);
#endif /* ACE_HAS_BROKEN_HPUX_TEMPLATES */

#else
static ACE_SignalHandler ace_signal_handler_dispatcher = ACE_SignalHandler (ACE_Sig_Handler::dispatch);

#if !defined (ACE_HAS_BROKEN_HPUX_TEMPLATES)
static ACE_SignalHandler ace_signal_handlers_dispatcher = ACE_SignalHandler (ACE_Sig_Handlers::dispatch);
#endif /* ACE_HAS_BROKEN_HPUX_TEMPLATES */
#endif /* ACE_HAS_SIG_C_FUNC */

// Array of Event_Handlers that will handle the signals.
ACE_Event_Handler *ACE_Sig_Handler::signal_handlers_[NSIG];

// Remembers if a signal has occurred.
sig_atomic_t ACE_Sig_Handler::sig_pending_ = 0;

ACE_ALLOC_HOOK_DEFINE(ACE_Sig_Action)

void
ACE_Sig_Action::dump (void) const
{
  ACE_TRACE ("ACE_Sig_Action::dump");
}

ACE_ALLOC_HOOK_DEFINE(ACE_Sig_Set)

void
ACE_Sig_Set::dump (void) const
{
  ACE_TRACE ("ACE_Sig_Set::dump");
}

ACE_ALLOC_HOOK_DEFINE(ACE_Sig_Guard)

void
ACE_Sig_Guard::dump (void) const
{
  ACE_TRACE ("ACE_Sig_Guard::dump");
}

ACE_Sig_Action::ACE_Sig_Action (void)
{
  // ACE_TRACE ("ACE_Sig_Action::ACE_Sig_Action");
  this->sa_.sa_flags = 0;

  // Since Service_Config::signal_handler_ is static and has an
  // ACE_Sig_Action instance, Win32 will get errno set unless this is
  // commented out.
#if !defined (ACE_WIN32)
  ACE_OS::sigemptyset (&this->sa_.sa_mask);
#endif /* ACE_WIN32 */
  this->sa_.sa_handler = 0;
}

ACE_Sig_Action::ACE_Sig_Action (ACE_SignalHandler sig_handler,
                                sigset_t *sig_mask,
                                int sig_flags)
{
  // ACE_TRACE ("ACE_Sig_Action::ACE_Sig_Action");
  this->sa_.sa_flags = sig_flags;

  if (sig_mask == 0)
    ACE_OS::sigemptyset (&this->sa_.sa_mask);
  else
    this->sa_.sa_mask = *sig_mask; // Structure assignment...

#if !defined(ACE_HAS_TANDEM_SIGNALS)
  this->sa_.sa_handler = ACE_SignalHandlerV (sig_handler);
#else
  this->sa_.sa_handler = (void (*)()) ACE_SignalHandlerV (sig_handler);
#endif /* !ACE_HAS_TANDEM_SIGNALS */
}

ACE_Sig_Action::ACE_Sig_Action (ACE_SignalHandler sig_handler,
                                ACE_Sig_Set &sig_mask,
                                int sig_flags)
{
  // ACE_TRACE ("ACE_Sig_Action::ACE_Sig_Action");
  this->sa_.sa_flags = sig_flags;

  // Structure assignment...
  this->sa_.sa_mask = sig_mask.sigset ();

#if !defined(ACE_HAS_TANDEM_SIGNALS)
  this->sa_.sa_handler = ACE_SignalHandlerV (sig_handler);
#else
  this->sa_.sa_handler = (void (*)()) ACE_SignalHandlerV (sig_handler);
#endif /* !ACE_HAS_TANDEM_SIGNALS */
}

ACE_Sig_Action::ACE_Sig_Action (ACE_SignalHandler sig_handler,
                                int signum,
                                sigset_t *sig_mask,
                                int sig_flags)
{
  // ACE_TRACE ("ACE_Sig_Action::ACE_Sig_Action");
  this->sa_.sa_flags = sig_flags;

  if (sig_mask == 0)
    ACE_OS::sigemptyset (&this->sa_.sa_mask);
  else
    this->sa_.sa_mask = *sig_mask; // Structure assignment...

#if !defined(ACE_HAS_TANDEM_SIGNALS)
  this->sa_.sa_handler = ACE_SignalHandlerV (sig_handler);
#else
  this->sa_.sa_handler = (void (*)()) ACE_SignalHandlerV (sig_handler);
#endif /* !ACE_HAS_TANDEM_SIGNALS */
  ACE_OS::sigaction (signum, &this->sa_, 0);
}

ACE_Sig_Action::ACE_Sig_Action (ACE_SignalHandler sig_handler,
                                int signum,
                                ACE_Sig_Set &sig_mask,
                                int sig_flags)
{
  // ACE_TRACE ("ACE_Sig_Action::ACE_Sig_Action");
  this->sa_.sa_flags = sig_flags;

  // Structure assignment...
  this->sa_.sa_mask = sig_mask.sigset (); 

#if !defined(ACE_HAS_TANDEM_SIGNALS)
  this->sa_.sa_handler = ACE_SignalHandlerV (sig_handler);
#else
  this->sa_.sa_handler = (void (*)()) ACE_SignalHandlerV (sig_handler);
#endif /* !ACE_HAS_TANDEM_SIGNALS */
  ACE_OS::sigaction (signum, &this->sa_, 0);
}

ACE_ALLOC_HOOK_DEFINE(ACE_Sig_Handler)

void
ACE_Sig_Handler::dump (void) const
{
  ACE_TRACE ("ACE_Sig_Handler::dump");
}

int
ACE_Sig_Handler::sig_pending (void)
{
  ACE_TRACE ("ACE_Sig_Handler::sig_pending");
  ACE_MT (ACE_Recursive_Thread_Mutex *lock =
	  ACE_Managed_Object<ACE_Recursive_Thread_Mutex>::get_preallocated_object
	  (ACE_Object_Manager::ACE_SIG_HANDLER_LOCK);
	  ACE_Guard<ACE_Recursive_Thread_Mutex> m (*lock));
  return ACE_Sig_Handler::sig_pending_ != 0;
}

void
ACE_Sig_Handler::sig_pending (int pending)
{
  ACE_TRACE ("ACE_Sig_Handler::sig_pending");
  ACE_MT (ACE_Recursive_Thread_Mutex *lock =
	  ACE_Managed_Object<ACE_Recursive_Thread_Mutex>::get_preallocated_object
	  (ACE_Object_Manager::ACE_SIG_HANDLER_LOCK);
	  ACE_Guard<ACE_Recursive_Thread_Mutex> m (*lock));
  ACE_Sig_Handler::sig_pending_ = pending;
}

ACE_Event_Handler *
ACE_Sig_Handler::handler (int signum)
{
  ACE_TRACE ("ACE_Sig_Handler::handler");
  ACE_MT (ACE_Recursive_Thread_Mutex *lock =
    ACE_Managed_Object<ACE_Recursive_Thread_Mutex>::get_preallocated_object
      (ACE_Object_Manager::ACE_SIG_HANDLER_LOCK);
    ACE_Guard<ACE_Recursive_Thread_Mutex> m (*lock));

  if (ACE_Sig_Handler::in_range (signum))
    return ACE_Sig_Handler::signal_handlers_[signum];
  else
    return 0;
}

ACE_Event_Handler *
ACE_Sig_Handler::handler (int signum, ACE_Event_Handler *new_sh)
{
  ACE_TRACE ("ACE_Sig_Handler::handler");
  ACE_MT (ACE_Recursive_Thread_Mutex *lock =
    ACE_Managed_Object<ACE_Recursive_Thread_Mutex>::get_preallocated_object
      (ACE_Object_Manager::ACE_SIG_HANDLER_LOCK);
    ACE_Guard<ACE_Recursive_Thread_Mutex> m (*lock));

  if (ACE_Sig_Handler::in_range (signum))
    {
      ACE_Event_Handler *sh = ACE_Sig_Handler::signal_handlers_[signum];

      ACE_Sig_Handler::signal_handlers_[signum] = new_sh;
      return sh;
    }
  else
    return 0;
}

// Register an ACE_Event_Handler along with the corresponding SIGNUM.

int
ACE_Sig_Handler::register_handler (int signum,
                                   ACE_Event_Handler *new_sh,
                                   ACE_Sig_Action *new_disp,
                                   ACE_Event_Handler **old_sh,
                                   ACE_Sig_Action *old_disp)
{
  ACE_TRACE ("ACE_Sig_Handler::register_handler");
  ACE_MT (ACE_Recursive_Thread_Mutex *lock =
    ACE_Managed_Object<ACE_Recursive_Thread_Mutex>::get_preallocated_object
      (ACE_Object_Manager::ACE_SIG_HANDLER_LOCK);
    ACE_Guard<ACE_Recursive_Thread_Mutex> m (*lock));

  if (ACE_Sig_Handler::in_range (signum))
    {
      ACE_Sig_Action sa; // Define a "null" action.
      ACE_Event_Handler *sh = this->handler (signum, new_sh);

      // Stack the old ACE_Sig_Handler if the user gives us a pointer
      // to a object.
      if (old_sh != 0)
        *old_sh = sh;

      // Make sure that new_disp points to a valid location if the
      // user doesn't care...
      if (new_disp == 0)
        new_disp = &sa;

      new_disp->handler (ace_signal_handler_dispatcher);
      new_disp->flags (new_disp->flags () | SA_SIGINFO);
      return new_disp->register_action (signum, old_disp);
    }
  else
    return -1;
}

// Remove an ACE_Event_Handler.

int
ACE_Sig_Handler::remove_handler (int signum,
                                 ACE_Sig_Action *new_disp,
                                 ACE_Sig_Action *old_disp,
                                 int)
{
  ACE_TRACE ("ACE_Sig_Handler::remove_handler");
  ACE_MT (ACE_Recursive_Thread_Mutex *lock =
    ACE_Managed_Object<ACE_Recursive_Thread_Mutex>::get_preallocated_object
      (ACE_Object_Manager::ACE_SIG_HANDLER_LOCK);
    ACE_Guard<ACE_Recursive_Thread_Mutex> m (*lock));

  if (ACE_Sig_Handler::in_range (signum))
    {
      ACE_Sig_Action sa (SIG_DFL, (sigset_t *) 0); // Define the default disposition.

      if (new_disp == 0)
        new_disp = &sa;

      ACE_Sig_Handler::signal_handlers_[signum] = 0;

      // Register either the new disposition or restore the default.
      return new_disp->register_action (signum, old_disp);
    }
  else
    return -1;
}

// Master dispatcher function that gets called by a signal handler and
// dispatches one handler...

void
ACE_Sig_Handler::dispatch (int signum,
                           siginfo_t *siginfo,
                           ucontext_t *ucontext)
{
  ACE_TRACE ("ACE_Sig_Handler::dispatch");
  ACE_MT (ACE_Recursive_Thread_Mutex *lock =
          ACE_Managed_Object<ACE_Recursive_Thread_Mutex>::get_preallocated_object (ACE_Object_Manager::ACE_SIG_HANDLER_LOCK);
          ACE_TSS_Guard<ACE_Recursive_Thread_Mutex> m (*lock));

  // Preserve errno across callbacks!
  int old_errno = errno;

  ACE_Sig_Handler::sig_pending (1);

  // Darn well better be in range since the OS dispatched this...
  ACE_ASSERT (ACE_Sig_Handler::in_range (signum));

  ACE_Event_Handler *eh = ACE_Sig_Handler::signal_handlers_[signum];

  if (eh != 0 && eh->handle_signal (signum, siginfo, ucontext) == -1)
    {
      // Define the default disposition.
      ACE_Sig_Action sa (SIG_DFL, (sigset_t *) 0);

      ACE_Sig_Handler::signal_handlers_[signum] = 0;

      // Remove the current disposition by registering the default
      // disposition.
      sa.register_action (signum);
    }

  // Restore error when callback completes.
  errno = old_errno;
}

ACE_Sig_Adapter::ACE_Sig_Adapter (ACE_Sig_Action &sa, int sigkey)
  : sigkey_ (sigkey),
    type_ (SIG_ACTION),
    sa_ (sa)
{
  // ACE_TRACE ("ACE_Sig_Adapter::ACE_Sig_Adapter");
}

ACE_Sig_Adapter::ACE_Sig_Adapter (ACE_Event_Handler *eh,
                                  int sigkey)
  : sigkey_ (sigkey),
    type_ (ACE_HANDLER),
    eh_ (eh)
{
  // ACE_TRACE ("ACE_Sig_Adapter::ACE_Sig_Adapter");
}

ACE_Sig_Adapter::ACE_Sig_Adapter (ACE_Sig_Handler_Ex sig_func,
                                  int sigkey)
  : sigkey_ (sigkey),
    type_ (C_FUNCTION),
    sig_func_ (sig_func)
{
  // ACE_TRACE ("ACE_Sig_Adapter::ACE_Sig_Adapter");
}

int
ACE_Sig_Adapter::sigkey (void)
{
  ACE_TRACE ("ACE_Sig_Adapter::sigkey");
  return this->sigkey_;
}

int
ACE_Sig_Adapter::handle_signal (int signum,
                                siginfo_t *siginfo,
                                ucontext_t *ucontext)
{
  ACE_TRACE ("ACE_Sig_Adapter::handle_signal");

  switch (this->type_)
    {
    case SIG_ACTION:
      {
        // We have to dispatch a handler that was registered by a
        // third-party library.

        ACE_Sig_Action old_disp;

        // Make sure this handler executes in the context it was
        // expecting...
        this->sa_.register_action (signum, &old_disp);

        ACE_Sig_Handler_Ex sig_func = ACE_Sig_Handler_Ex (this->sa_.handler ());

        (*sig_func) (signum, siginfo, ucontext);
        // Restore the original disposition.
        old_disp.register_action (signum);
        break;
      }
    case ACE_HANDLER:
      this->eh_->handle_signal (signum, siginfo, ucontext);
      break;
    case C_FUNCTION:
      (*this->sig_func_) (signum, siginfo, ucontext);
      break;
    }
  return 0;
}

// ----------------------------------------
// The following classes are local to this file.

// There are bugs with HP/UX's C++ compiler that prevents this stuff
// from compiling...
#if !defined (ACE_HAS_BROKEN_HPUX_TEMPLATES)
#define ACE_MAX_SIGNAL_HANDLERS ((size_t) 20)

// Keeps track of the id that uniquely identifies each registered
// signal handler.  This id can be used to cancel a timer via the
// <remove_handler> method.
int ACE_Sig_Handlers::sigkey_ = 0;

// If this is > 0 then a 3rd party library has registered a
// handler...
int ACE_Sig_Handlers::third_party_sig_handler_ = 0;

// Make life easier by defining typedefs...
typedef ACE_Fixed_Set <ACE_Event_Handler *, ACE_MAX_SIGNAL_HANDLERS>
        ACE_SIG_HANDLERS_SET;
typedef ACE_Fixed_Set_Iterator <ACE_Event_Handler *, ACE_MAX_SIGNAL_HANDLERS>
        ACE_SIG_HANDLERS_ITERATOR;

class ACE_Sig_Handlers_Set
{
public:
  static ACE_SIG_HANDLERS_SET *instance (int signum);

private:
  static ACE_SIG_HANDLERS_SET *sig_handlers_[NSIG];
};

/* static */
ACE_SIG_HANDLERS_SET *ACE_Sig_Handlers_Set::sig_handlers_[NSIG];

/* static */
ACE_SIG_HANDLERS_SET *
ACE_Sig_Handlers_Set::instance (int signum)
{
  if (signum <= 0 || signum >= NSIG)
    return 0; // This will cause problems...
  else if (ACE_Sig_Handlers_Set::sig_handlers_[signum] == 0)
    ACE_NEW_RETURN (ACE_Sig_Handlers_Set::sig_handlers_[signum], ACE_SIG_HANDLERS_SET, 0);

  return ACE_Sig_Handlers_Set::sig_handlers_[signum];
}

ACE_ALLOC_HOOK_DEFINE(ACE_Sig_Handlers)

void
ACE_Sig_Handlers::dump (void) const
{
  ACE_TRACE ("ACE_Sig_Handlers::dump");
}

// This is the method that does all the dirty work...  The basic
// structure of this method was devised by Detlef Becker
// (beckerd@erlh.siemens.de).

int
ACE_Sig_Handlers::register_handler (int signum,
                                    ACE_Event_Handler *new_sh,
                                    ACE_Sig_Action *new_disp,
                                    ACE_Event_Handler **,
                                    ACE_Sig_Action *old_disp)
{
  ACE_TRACE ("ACE_Sig_Handlers::register_handler");
  ACE_MT (ACE_Recursive_Thread_Mutex *lock =
    ACE_Managed_Object<ACE_Recursive_Thread_Mutex>::get_preallocated_object
      (ACE_Object_Manager::ACE_SIG_HANDLER_LOCK);
    ACE_Guard<ACE_Recursive_Thread_Mutex> m (*lock));

  if (ACE_Sig_Handler::in_range (signum))
    {
      ACE_Sig_Adapter *ace_sig_adapter = 0; // Our signal handler.
      ACE_Sig_Adapter *extern_sh = 0; // An external signal handler.
      ACE_Sig_Action sa;

      // Get current signal disposition.
      sa.retrieve_action (signum);

      // Check whether we are already in control of the signal
      // handling disposition...

      if (!(sa.handler () == ace_signal_handlers_dispatcher
          || sa.handler () == ACE_SignalHandler (SIG_IGN)
          || sa.handler () == ACE_SignalHandler (SIG_DFL)))
        {
          // Drat, a 3rd party library has already installed a signal ;-(

          // Upto here we never disabled RESTART_MODE.  Thus,
          // RESTART_MODE can only be changed by 3rd party libraries.

          if (ACE_BIT_DISABLED (sa.flags (), SA_RESTART)
              && ACE_Sig_Handlers::third_party_sig_handler_)
            // Toggling is disallowed since we might break 3rd party
            // code.
            return -1;

          // Note that we've seen a 3rd party handler...
          ACE_Sig_Handlers::third_party_sig_handler_ = 1;

          // Create a new 3rd party disposition, remembering its
          // preferred signal blocking etc...;
          ACE_NEW_RETURN (extern_sh, ACE_Sig_Adapter (sa, ++ACE_Sig_Handlers::sigkey_), -1);

          // Add the external signal handler to the set of handlers
          // for this signal.
          if (ACE_Sig_Handlers_Set::instance (signum)->insert (extern_sh) == -1)
            {
              delete extern_sh;
              return -1;
            }
        }
      // Add our new handler at this point.
      ACE_NEW_RETURN (ace_sig_adapter, ACE_Sig_Adapter (new_sh, ++ACE_Sig_Handlers::sigkey_), -1);

      // Add the ACE signal handler to the set of handlers for this
      // signal (make sure it goes before the external one if there is
      // one of these).
      if (ACE_Sig_Handlers_Set::instance (signum)->insert (ace_sig_adapter) == -1)
        {
          // We couldn't reinstall our handler, so let's pretend like
          // none of this happened...
          if (extern_sh)
            {
              ACE_Sig_Handlers_Set::instance (signum)->remove (extern_sh);
              delete extern_sh;
            }
          delete ace_sig_adapter;
          return -1;
        }
      // If ACE_Sig_Handlers::dispatch() was set we're done.
      else if (sa.handler () == ace_signal_handlers_dispatcher)
        return ace_sig_adapter->sigkey ();

      // Otherwise, we need to register our handler function so that
      // all signals will be dispatched through ACE.
      else
        {
          // Make sure that new_disp points to a valid location if the
          // user doesn't care...
          if (new_disp == 0)
            new_disp = &sa;

          new_disp->handler (ace_signal_handlers_dispatcher);

          // Default is to restart signal handlers.
          new_disp->flags (new_disp->flags () | SA_RESTART);
          new_disp->flags (new_disp->flags () | SA_SIGINFO);

          // Finally install (possibly reinstall) the ACE signal
          // handler disposition with the SA_RESTART mode enabled.
          if (new_disp->register_action (signum, old_disp) == -1)
            {
              // Yikes, lots of roll back at this point...
              ACE_Sig_Handlers_Set::instance (signum)->remove (ace_sig_adapter);
              delete ace_sig_adapter;

              if (extern_sh)
                {
                  ACE_Sig_Handlers_Set::instance (signum)->remove (extern_sh);
                  delete extern_sh;
                }
              return -1;
            }
          else // Return the signal key so that programs can cancel this
            // handler if they want!
            return ace_sig_adapter->sigkey ();
        }
    }
  else
    return -1;
}

// Remove the ACE_Event_Handler currently associated with <signum>.
// Install the new disposition (if given) and return the previous
// disposition (if desired by the caller).  Returns 0 on success and
// -1 if <signum> is invalid.

int
ACE_Sig_Handlers::remove_handler (int signum,
                                  ACE_Sig_Action *new_disp,
                                  ACE_Sig_Action *old_disp,
                                  int sigkey)
{
  ACE_TRACE ("ACE_Sig_Handlers::remove_handler");
  ACE_MT (ACE_Recursive_Thread_Mutex *lock =
    ACE_Managed_Object<ACE_Recursive_Thread_Mutex>::get_preallocated_object
      (ACE_Object_Manager::ACE_SIG_HANDLER_LOCK);
    ACE_Guard<ACE_Recursive_Thread_Mutex> m (*lock));

  if (ACE_Sig_Handler::in_range (signum))
    {
      ACE_SIG_HANDLERS_SET *handler_set =
        ACE_Sig_Handlers_Set::instance (signum);

      ACE_SIG_HANDLERS_ITERATOR handler_iterator (*handler_set);

      // Iterate through the set of handlers for this signal.

      for (ACE_Event_Handler **eh;
           handler_iterator.next (eh) != 0;
           handler_iterator.advance ())
        {
          // Type-safe downcast would be nice here...
          ACE_Sig_Adapter *sh = (ACE_Sig_Adapter *) *eh;

          // Remove the handler if (1) it's key matches the key we've
          // been told to remove or (2) if we've been told to remove
          // *all* handlers (i.e., <sigkey> == -1).

          if (sh->sigkey () == sigkey || sigkey == -1)
            {
              handler_set->remove (*eh);
              delete *eh;
            }
        }

      if (handler_set->size () == 0)
        {
          // If there are no more handlers left for a signal then
          // register the new disposition or restore the default
          // disposition.

          ACE_Sig_Action sa (SIG_DFL, (sigset_t *) 0);

          if (new_disp == 0)
            new_disp = &sa;

          return new_disp->register_action (signum, old_disp);
        }
      return 0;
    }
  else
    return -1;
}

// Master dispatcher function that gets called by a signal handler and
// dispatches *all* the handlers...

void
ACE_Sig_Handlers::dispatch (int signum,
                            siginfo_t *siginfo,
                            ucontext_t *ucontext)
{
  ACE_TRACE ("ACE_Sig_Handlers::dispatch");
  ACE_MT (ACE_Recursive_Thread_Mutex *lock =
    ACE_Managed_Object<ACE_Recursive_Thread_Mutex>::get_preallocated_object
      (ACE_Object_Manager::ACE_SIG_HANDLER_LOCK);
    ACE_TSS_Guard<ACE_Recursive_Thread_Mutex> m (*lock));

  // Preserve errno across callbacks!
  int old_errno = errno;

  ACE_Sig_Handler::sig_pending (1);

  // Darn well better be in range since the OS dispatched this...
  ACE_ASSERT (ACE_Sig_Handler::in_range (signum));

  ACE_SIG_HANDLERS_SET *handler_set =
    ACE_Sig_Handlers_Set::instance (signum);

  ACE_SIG_HANDLERS_ITERATOR handler_iterator (*handler_set);

  for (ACE_Event_Handler **eh = 0;
       handler_iterator.next (eh) != 0;
       handler_iterator.advance ())
    {
      if ((*eh)->handle_signal (signum, siginfo, ucontext) == -1)
        {
          handler_set->remove (*eh);
          delete *eh;
        }
    }

  // Restore error when callback completes.
  errno = old_errno;
}

// Return the first item in the list of handlers.  Note that this will
// trivially provide the same behavior as the ACE_Sig_Handler
// version if there is only 1 handler registered!

ACE_Event_Handler *
ACE_Sig_Handlers::handler (int signum)
{
  ACE_TRACE ("ACE_Sig_Handlers::handler");
  ACE_SIG_HANDLERS_SET *handler_set =
    ACE_Sig_Handlers_Set::instance (signum);
  ACE_SIG_HANDLERS_ITERATOR handler_iterator (*handler_set);
  ACE_Event_Handler **eh = 0;
  handler_iterator.next (eh);
  return *eh;
}

// The following is a strange bit of logic that tries to give the same
// semantics as what happens in ACE_Sig_Handler when we replace the
// current signal handler with a new one.  Note that if there is only
// one signal handler the behavior will be identical.  If there is
// more than one handler then things get weird...

ACE_Event_Handler *
ACE_Sig_Handlers::handler (int signum, ACE_Event_Handler *new_sh)
{
  ACE_TRACE ("ACE_Sig_Handlers::handler");
  ACE_SIG_HANDLERS_SET *handler_set =
    ACE_Sig_Handlers_Set::instance (signum);
  ACE_SIG_HANDLERS_ITERATOR handler_iterator (*handler_set);
  ACE_Event_Handler **eh = 0;

  // Find the first handler...
  handler_iterator.next (eh);

  // ... then remove it from the set ...
  handler_set->remove (*eh);

  // ... and then insert the new signal handler into the beginning of
  // the set (note, this is a bit too tied up in the implementation of
  // ACE_Unbounded_Set...).
  ACE_Sig_Adapter *temp;

  ACE_NEW_RETURN (temp, ACE_Sig_Adapter (new_sh, ++ACE_Sig_Handlers::sigkey_), 0);
  handler_set->insert (temp);
  return *eh;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
ACE_MT (template class ACE_TSS_Guard<ACE_Recursive_Thread_Mutex>);
ACE_MT (template class ACE_Guard<ACE_Recursive_Thread_Mutex>);
template class ACE_Fixed_Set<ACE_Event_Handler *, ACE_MAX_SIGNAL_HANDLERS>;
template class ACE_Fixed_Set_Iterator<ACE_Event_Handler *, ACE_MAX_SIGNAL_HANDLERS>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0)
#pragma instantiate ACE_TSS_Guard<ACE_Recursive_Thread_Mutex>
#pragma instantiate ACE_Guard<ACE_Recursive_Thread_Mutex>
#endif /* ACE_MT_SAFE */
#pragma instantiate ACE_Fixed_Set<ACE_Event_Handler *, ACE_MAX_SIGNAL_HANDLERS>
#pragma instantiate ACE_Fixed_Set_Iterator<ACE_Event_Handler *, ACE_MAX_SIGNAL_HANDLERS>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#endif /* ACE_HAS_BROKEN_HPUX_TEMPLATES */
