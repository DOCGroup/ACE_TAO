/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    Signal.h
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (ACE_SIGNAL_HANDLER_H)
#define ACE_SIGNAL_HANDLER_H

#include "ace/Synch.h"
#include "ace/Event_Handler.h"
#include "ace/Containers.h"

// This worksaround a horrible bug with HP/UX C++...
typedef struct sigaction ACE_SIGACTION;

class ACE_Export ACE_Sig_Set
  // = TITLE
  //     Provide a C++ wrapper for the C sigset_t interface.
  // 
  // = DESCRIPTION 
  //     Handle signals via a more elegant C++ interface (e.g.,
  //     doesn't require the use of global variables or global
  //     functions in an application).
{
public:
  // = Initialization and termination methods.
  ACE_Sig_Set (sigset_t *);
  ACE_Sig_Set (int fill = 0);
  ~ACE_Sig_Set (void);

  int empty_set (void);
  // Create a set that excludes all signals defined by the system.

  int fill_set (void);
  // Create a set that includes all signals defined by the system.

  int sig_add (int signo);
  // Adds the individual signal specified by <signo> to the set. 

  int sig_del (int signo);
  // Deletes the individual signal specified by <signo> from the set.

  int is_member (int signo) const;
  // Checks whether the signal specified by <signo> is in the set.

  operator sigset_t *();
  // Returns a pointer to the underlying sigset_t.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  sigset_t sigset_;
  // Set of signals.
};

class ACE_Export ACE_Sig_Action
  // = TITLE
  //     C++ wrapper around struct sigaction.
{
public:
  // = Initialization methods.
  ACE_Sig_Action (void);
  ACE_Sig_Action (ACE_SignalHandler handler,
		  sigset_t *sigmask = 0,
		  int flags = 0);
  ACE_Sig_Action (ACE_SignalHandler handler,
		  int signum,
		  sigset_t *sigmask = 0,
		  int flags = 0);
  ACE_Sig_Action (const ACE_Sig_Action &s);
  ACE_Sig_Action (struct sigaction *);

  // = Signal action management.
  int register_action (int signum, ACE_Sig_Action *oaction = 0);
  // Register <this> as the current disposition and store old
  // disposition into <oaction> if it is non-NULL.

  int restore_action (int signum, ACE_Sig_Action &oaction);
  // Assign the value of <oaction> to <this> and make it become the
  // new signal disposition.

  int retrieve_action (int signum);
  // Retrieve the current disposition into <this>.

  // = Set/get current signal action.
  void set (struct sigaction *);
  struct sigaction *get (void);
  operator ACE_SIGACTION *();

  // = Set/get current signal flags.
  void flags (int);
  int flags (void);

  // = Set/get current signal mask.
  void mask (sigset_t *);
  sigset_t *mask (void);

  // = Set/get current signal handler (pointer to function).
  void handler (ACE_SignalHandler);
  ACE_SignalHandler handler (void);

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  struct sigaction sa_;
  // Controls signal behavior.
};

class ACE_Export ACE_Sig_Guard
  // = TITLE
  //     Hold signals in MASK for duration of a C++ statement block.
  //     Note that a "0" for mask causes all signals to be held.
{
public:
  // = Set/remove mask.
  ACE_Sig_Guard (ACE_Sig_Set *mask = 0);
  ~ACE_Sig_Guard (void);

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  ACE_Sig_Set omask_; 
  // Original signal mask.
};

class ACE_Export ACE_Sig_Handler
  // = TITLE
  //    This is the main dispatcher of signals for ACE.  It improves
  //    the existing UNIX signal handling mechanism by allowing C++
  //    objects to handle signals in a way that avoids the use of
  //    global/static variables and functions.
  //
  // = DESCRIPTION
  //    Using this class a program can register an <ACE_Event_Handler>
  //    with the <ACE_Sig_Handler> in order to handle a designated
  //    <signum>.  When a signal occurs that corresponds to this
  //    <signum>, the <handle_signal> method of the registered
  //    <ACE_Event_Handler> is invoked automatically.
{
public:
  // = Registration and removal methods.
  virtual int register_handler (int signum, 
				ACE_Event_Handler *new_sh, 
				ACE_Sig_Action *new_disp = 0, 
				ACE_Event_Handler **old_sh = 0,
				ACE_Sig_Action *old_disp = 0);
  // Add a new <ACE_Event_Handler> and a new sigaction associated with
  // <signum>.  Passes back the existing ACE_Event_Handler and its
  // sigaction if pointers are non-zero.  Returns -1 on failure and >=
  // 0 on success.

  virtual int remove_handler (int signum, 
			      ACE_Sig_Action *new_disp = 0,
			      ACE_Sig_Action *old_disp = 0,
			      int sigkey = -1);
  // Remove the <ACE_Event_Handler> currently associated with
  // <signum>.  <sigkey> is ignored in this implementation since there
  // is only one instance of a signal handler.  Install the new
  // disposition (if given) and return the previous disposition (if
  // desired by the caller).  Returns 0 on success and -1 if <signum>
  // is invalid.

  // Set/get signal status.
  static sig_atomic_t sig_pending (void);
  // True if there is a pending signal. 

  static void sig_pending (sig_atomic_t);
  // Reset the value of <sig_pending_> so that no signal is pending. 

  // = Set/get the handler associated with a particular signal.

  virtual ACE_Event_Handler *handler (int signum);
  // Return the list of <ACE_Sig_Handlers> associated with <signum>. 
  
  virtual ACE_Event_Handler *handler (int signum, ACE_Event_Handler *);
  // Set a new <ACE_Event_Handler> that is associated with <signum>.
  // Return the existing handler.

  static void dispatch (int, siginfo_t *, ucontext_t *);
  // Callback routine registered with sigaction(2) that dispatches the
  // handle_signal() method of the appropriate pre-registered
  // ACE_Event_Handler.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected: 
  // = These methods and data members are shared by derived classes. 

  static int in_range (int signum);
  // Check whether the SIGNUM is within the legal range of signals. 

  static sig_atomic_t sig_pending_;
  // Keeps track of whether a signal is pending. 
  
#if defined (ACE_MT_SAFE)
  static ACE_Recursive_Thread_Mutex ace_sig_handler_lock_;
#endif /* ACE_MT_SAFE */

private:
  static ACE_Event_Handler *signal_handlers_[NSIG];
  // Array used to store one user-defined Event_Handler for every
  // signal.
};

class ACE_Export ACE_Sig_Adapter : public ACE_Event_Handler
  // = TITLE
  //     Provide an adapter that transforms various types of signal
  //     handlers into the scheme used by the <ACE_Reactor>.

{
public:
  ACE_Sig_Adapter (ACE_Sig_Action &, int sigkey);
  ACE_Sig_Adapter (ACE_Event_Handler *, int sigkey);
  ACE_Sig_Adapter (ACE_Sig_Handler_Ex, int sigkey = 0);
  
  int sigkey (void);
  // Returns this signal key that's used to remove this from the
  // <ACE_Reactor>'s internal table.

  virtual int handle_signal (int, siginfo_t *, ucontext_t *);
  // Called by the <Reactor> to dispatch the signal handler.

private:
  int sigkey_;
  // Key for this signal handler (used to remove it).

  enum 
  {
    ACE_HANDLER, // We're just wrapping an ACE_Event_Handler.
    SIG_ACTION,  // An ACE_Sig_Action.
    C_FUNCTION     // A normal C function.
  } type_;
  // Is this an external handler or an ACE handler?

  // = This should be a union, but C++ won't allow that because the
  // <ACE_Sig_Action> has a constructor.
  ACE_Sig_Action sa_;
  // This is an external handler (ugh).

  ACE_Event_Handler *eh_;
  // This is an ACE hander.

  ACE_Sig_Handler_Ex sig_func_;
  // This is a normal C function.
};

#if !defined (ACE_HAS_BROKEN_HPUX_TEMPLATES)
class ACE_Export ACE_Sig_Handlers : public ACE_Sig_Handler
  // = TITLE
  //    This is an alternative signal handling dispatcher for ACE.  It
  //    allows a list of signal handlers to be registered for each
  //    signal.  It also makes SA_RESTART the default mode.
  //
  // = DESCRIPTION
  //    Using this class a program can register one or more
  //    ACE_Event_Handler with the ACE_Sig_Handler in order to
  //    handle a designated <signum>.  When a signal occurs that
  //    corresponds to this <signum>, the <handle_signal> methods of
  //    all the registered ACE_Event_Handlers are invoked
  //    automatically.
{
public:
  // = Registration and removal methods.
  virtual int register_handler (int signum, 
				ACE_Event_Handler *new_sh, 
				ACE_Sig_Action *new_disp = 0, 
				ACE_Event_Handler **old_sh = 0,
				ACE_Sig_Action *old_disp = 0);
  // Add a new ACE_Event_Handler and a new sigaction associated with
  // <signum>.  Passes back the existing ACE_Event_Handler and its
  // sigaction if pointers are non-zero.  Returns -1 on failure and 
  // a <sigkey> that is >= 0 on success.

  virtual int remove_handler (int signum,
			      ACE_Sig_Action *new_disp = 0,
			      ACE_Sig_Action *old_disp = 0,
			      int sigkey = -1);
  // Remove the ACE_Event_Handler currently associated with <signum>.
  // Install the new disposition (if given) and return the previous
  // disposition (if desired by the caller).  Returns 0 on success and
  // -1 if <signum> is invalid.

  // = Set/get the handler associated with a particular signal.

  virtual ACE_Event_Handler *handler (int signum);
  // Return the head of the list of ACE_Sig_Handlers associated
  // with SIGNUM.
  
  virtual ACE_Event_Handler *handler (int signum, ACE_Event_Handler *);
  // Set a new ACE_Event_Handler that is associated with SIGNUM at the
  // head of the list of signals.  Return the existing handler that
  // was at the head.

  static void dispatch (int signum, siginfo_t *, ucontext_t *);
  // Callback routine registered with sigaction(2) that dispatches the
  // handle_signal() method of all the pre-registered
  // ACE_Event_Handlers for <signum>

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  static int sigkey_;
  // Keeps track of the id that uniquely identifies each registered
  // signal handler.  This id can be used to cancel a timer via the
  // <remove_handler> method.

  static int third_party_sig_handler_;
  // If this is > 0 then a 3rd party library has registered a
  // handler...  
};
#endif /* ACE_HAS_BROKEN_HPUX_TEMPLATES */

#if defined (__ACE_INLINE__)
#include "ace/Signal.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_SIGNAL_HANDLER_H */
