/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Signal.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt <schmidt@cs.wustl.edu>
 */
//=============================================================================

#ifndef ACE_SIGNAL_HANDLER_H
#define ACE_SIGNAL_HANDLER_H
#include "ace/pre.h"

#if defined (ACE_DONT_INCLUDE_ACE_SIGNAL_H)
# error ace/Signal.h was #included instead of signal.h by ace/OS.h:  fix!!!!
#endif /* ACE_DONT_INCLUDE_ACE_SIGNAL_H */

#include "ace/Threads/Synch.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Event_Handler/Event_Handler.h"

// This worksaround a horrible bug with HP/UX C++...
typedef struct sigaction ACE_SIGACTION;

/**
 * @class ACE_Sig_Set
 *
 * @brief Provide a C++ wrapper for the C sigset_t interface.
 *
 * Handle signals via a more elegant C++ interface (e.g.,
 * doesn't require the use of global variables or global
 * functions in an application).
 */
class ACE_Export ACE_Sig_Set
{
public:
  // = Initialization and termination methods.
  /// Initialize <sigset_> with <sigset>.  If <sigset> == 0 then fill
  /// the set.
  ACE_Sig_Set (sigset_t *sigset);

  /// Initialize <sigset_> with <sigset>.  If <sigset> == 0 then fill
  /// the set.
  ACE_Sig_Set (ACE_Sig_Set *sigset);

  /// If <fill> == 0 then initialize the <sigset_> to be empty, else
  /// full.
  ACE_Sig_Set (int fill = 0);

  ~ACE_Sig_Set (void);

  /// Create a set that excludes all signals defined by the system.
  int empty_set (void);

  /// Create a set that includes all signals defined by the system.
  int fill_set (void);

  /// Adds the individual signal specified by <signo> to the set.
  int sig_add (int signo);

  /// Deletes the individual signal specified by <signo> from the set.
  int sig_del (int signo);

  /// Checks whether the signal specified by <signo> is in the set.
  int is_member (int signo) const;

  /// Returns a pointer to the underlying <sigset_t>.
  operator sigset_t *();

  /// Returns a copy of the underlying <sigset_t>.
  sigset_t sigset (void) const;

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

private:
  /// Set of signals.
  sigset_t sigset_;
};

/**
 * @class ACE_Sig_Action
 *
 * @brief C++ wrapper facade for the <sigaction> struct.
 */
class ACE_Export ACE_Sig_Action
{
public:
  // = Initialization methods.
  /// Default constructor.  Initializes everything to 0.
  ACE_Sig_Action (void);

  /// Assigns the various fields of a <sigaction> struct but doesn't
  /// register for signal handling via the <sigaction> function.
  ACE_Sig_Action (ACE_SignalHandler handler,
                  sigset_t *sigmask = 0,
                  int flags = 0);

  /// Assigns the various fields of a <sigaction> struct but doesn't
  /// register for signal handling via the <sigaction> function.
  ACE_Sig_Action (ACE_SignalHandler handler,
                  const ACE_Sig_Set &sigmask,
                  int flags = 0);

  /**
   * Assigns the various fields of a <sigaction> struct and registers
   * the <handler> to process signal <signum> via the <sigaction>
   * function.
   */
  ACE_Sig_Action (ACE_SignalHandler handler,
                  int signum,
                  sigset_t *sigmask = 0,
                  int flags = 0);

  /**
   * Assigns the various fields of a <sigaction> struct and registers
   * the <handler> to process signal <signum> via the <sigaction>
   * function.
   */
  ACE_Sig_Action (ACE_SignalHandler handler,
                  int signum,
                  const ACE_Sig_Set &sigmask,
                  int flags = 0);


  // @@ The next two methods have a parameter as "signalss". Please do
  // not change the argument name as "signals". This causes the
  // following problem as reported by
  // <James.Briggs@dsto.defence.gov.au>.

  // In the file Signal.h two of the functions have and argument name
  // of signals. signals is a Qt macro (to do with their meta object
  // stuff.
  // We could as well have it as "signal", but I am nost sure whether
  // that would cause a problem with something else - Bala <bala@cs>

  /**
   * Assigns the various fields of a <sigaction> struct and registers
   * the <handler> to process all <signals> via the <sigaction>
   * function.
   */
  ACE_Sig_Action (const ACE_Sig_Set &signalss,
                  ACE_SignalHandler handler,
                  const ACE_Sig_Set &sigmask,
                  int flags = 0);

  /**
   * Assigns the various fields of a <sigaction> struct and registers
   * the <handler> to process all <signals> via the <sigaction>
   * function.
   */
  ACE_Sig_Action (const ACE_Sig_Set &signalss,
                  ACE_SignalHandler handler,
                  sigset_t *sigmask = 0,
                  int flags = 0);

  /// Copy constructor.
  ACE_Sig_Action (const ACE_Sig_Action &s);

  /// Default dtor.
  ~ACE_Sig_Action (void);

  // = Signal action management.
  /// Register <this> as the current disposition and store old
  /// disposition into <oaction> if it is non-NULL.
  int register_action (int signum,
                       ACE_Sig_Action *oaction = 0);

  /// Assign the value of <oaction> to <this> and make it become the
  /// new signal disposition.
  int restore_action (int signum,
                      ACE_Sig_Action &oaction);

  /// Retrieve the current disposition into <this>.
  int retrieve_action (int signum);

  // = Set/get current signal action.
  void set (struct sigaction *);
  struct sigaction *get (void);
  operator ACE_SIGACTION *();

  // = Set/get current signal flags.
  void flags (int);
  int flags (void);

  // = Set/get current signal mask.
  void mask (sigset_t *);
  void mask (ACE_Sig_Set &);
  sigset_t *mask (void);

  // = Set/get current signal handler (pointer to function).
  void handler (ACE_SignalHandler);
  ACE_SignalHandler handler (void);

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

private:
  /// Controls signal behavior.
  struct sigaction sa_;
};

/**
 * @class ACE_Sig_Guard
 *
 * @brief Hold signals in MASK for duration of a C++ statement block.
 * Note that a "0" for mask causes all signals to be held.
 */
class ACE_Export ACE_Sig_Guard
{
public:
  // = Initialization and termination methods.
  /// Block out signals in <mask>.  Default is to block all signals!
  ACE_Sig_Guard (ACE_Sig_Set *mask = 0);

  /// Restore blocked signals.
  ~ACE_Sig_Guard (void);

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

private:
  /// Original signal mask.
  ACE_Sig_Set omask_;
};

/**
 * @class ACE_Sig_Handler
 *
 * @brief This is the main dispatcher of signals for ACE.  It improves
 * the existing UNIX signal handling mechanism by allowing C++
 * objects to handle signals in a way that avoids the use of
 * global/static variables and functions.
 *
 * Using this class a program can register an <ACE_Event_Handler>
 * with the <ACE_Sig_Handler> in order to handle a designated
 * <signum>.  When a signal occurs that corresponds to this
 * <signum>, the <handle_signal> method of the registered
 * <ACE_Event_Handler> is invoked automatically.
 */
class ACE_Export ACE_Sig_Handler
{
public:
#if defined (ACE_HAS_WINCE)
  /// Default ctor/dtor.
  ACE_Sig_Handler (void);
  virtual ~ACE_Sig_Handler (void);
#endif /* ACE_HAS_WINCE */

  // = Registration and removal methods.
  /**
   * Add a new <ACE_Event_Handler> and a new sigaction associated with
   * <signum>.  Passes back the existing <ACE_Event_Handler> and its
   * sigaction if pointers are non-zero.  Returns -1 on failure and >=
   * 0 on success.
   */
  virtual int register_handler (int signum,
                                ACE_Event_Handler *new_sh,
                                ACE_Sig_Action *new_disp = 0,
                                ACE_Event_Handler **old_sh = 0,
                                ACE_Sig_Action *old_disp = 0);

  /**
   * Remove the <ACE_Event_Handler> currently associated with
   * <signum>.  <sigkey> is ignored in this implementation since there
   * is only one instance of a signal handler.  Install the new
   * disposition (if given) and return the previous disposition (if
   * desired by the caller).  Returns 0 on success and -1 if <signum>
   * is invalid.
   */
  virtual int remove_handler (int signum,
                              ACE_Sig_Action *new_disp = 0,
                              ACE_Sig_Action *old_disp = 0,
                              int sigkey = -1);

  // Set/get signal status.
  /// True if there is a pending signal.
  static int sig_pending (void);

  /// Reset the value of <sig_pending_> so that no signal is pending.
  static void sig_pending (int);

  // = Set/get the handler associated with a particular signal.

  /// Return the <ACE_Sig_Handler> associated with <signum>.
  virtual ACE_Event_Handler *handler (int signum);

  /// Set a new <ACE_Event_Handler> that is associated with <signum>.
  /// Return the existing handler.
  virtual ACE_Event_Handler *handler (int signum,
                                      ACE_Event_Handler *);

  /**
   * Callback routine registered with sigaction(2) that dispatches the
   * <handle_signal> method of the appropriate pre-registered
   * ACE_Event_Handler.
   */
  static void dispatch (int, siginfo_t *,
                        ucontext_t *);

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

protected:
  // = These methods and data members are shared by derived classes.

  /**
   * Set a new <ACE_Event_Handler> that is associated with <signum>.
   * Return the existing handler.  Does not acquire any locks so that
   * it can be called from a signal handler, such as <dispatch>.
   */
  static ACE_Event_Handler *handler_i (int signum,
                                       ACE_Event_Handler *);

  /**
   * This implementation method is called by <register_handler> and
   * <dispatch>.  It doesn't do any locking so that it can be called
   * within a signal handler, such as <dispatch>.  It adds a new
   * <ACE_Event_Handler> and a new sigaction associated with <signum>.
   * Passes back the existing <ACE_Event_Handler> and its sigaction if
   * pointers are non-zero.  Returns -1 on failure and >= 0 on
   * success.
   */
  static int register_handler_i (int signum,
                                 ACE_Event_Handler *new_sh,
                                 ACE_Sig_Action *new_disp = 0,
                                 ACE_Event_Handler **old_sh = 0,
                                 ACE_Sig_Action *old_disp = 0);

  /// Check whether the SIGNUM is within the legal range of signals.
  static int in_range (int signum);

  /// Keeps track of whether a signal is pending.
  static sig_atomic_t sig_pending_;

private:
  /// Array used to store one user-defined Event_Handler for every
  /// signal.
  static ACE_Event_Handler *signal_handlers_[ACE_NSIG];
};

/**
 * @class ACE_Sig_Adapter
 *
 * @brief Provide an adapter that transforms various types of signal
 * handlers into the scheme used by the <ACE_Reactor>.
 */
class ACE_Export ACE_Sig_Adapter : public ACE_Event_Handler
{
public:
  ACE_Sig_Adapter (ACE_Sig_Action &, int sigkey);
  ACE_Sig_Adapter (ACE_Event_Handler *, int sigkey);
  ACE_Sig_Adapter (ACE_Sig_Handler_Ex, int sigkey = 0);
  ~ACE_Sig_Adapter (void);

  /// Returns this signal key that's used to remove this from the
  /// <ACE_Reactor>'s internal table.
  int sigkey (void);

  /// Called by the <Reactor> to dispatch the signal handler.
  virtual int handle_signal (int, siginfo_t *, ucontext_t *);

private:
  /// Key for this signal handler (used to remove it).
  int sigkey_;

  /// Is this an external handler or an ACE handler?
  enum
  {
    /// We're just wrapping an ACE_Event_Handler.
    ACE_HANDLER,
    /// An ACE_Sig_Action.
    SIG_ACTION,
    /// A normal C function.
    C_FUNCTION
  } type_;

  // = This should be a union, but C++ won't allow that because the
  // <ACE_Sig_Action> has a constructor.
  /// This is an external handler (ugh).
  ACE_Sig_Action sa_;

  /// This is an ACE hander.
  ACE_Event_Handler *eh_;

  /// This is a normal C function.
  ACE_Sig_Handler_Ex sig_func_;
};

#if !defined (ACE_HAS_BROKEN_HPUX_TEMPLATES)
/**
 * @class ACE_Sig_Handlers
 *
 * @brief This is an alternative signal handling dispatcher for ACE.  It
 * allows a list of signal handlers to be registered for each
 * signal.  It also makes SA_RESTART the default mode.
 *
 * Using this class a program can register one or more
 * ACE_Event_Handler with the ACE_Sig_Handler in order to
 * handle a designated <signum>.  When a signal occurs that
 * corresponds to this <signum>, the <handle_signal> methods of
 * all the registered ACE_Event_Handlers are invoked
 * automatically.
 */
class ACE_Export ACE_Sig_Handlers : public ACE_Sig_Handler
{
public:
  // = Registration and removal methods.
  /**
   * Add a new ACE_Event_Handler and a new sigaction associated with
   * <signum>.  Passes back the existing ACE_Event_Handler and its
   * sigaction if pointers are non-zero.  Returns -1 on failure and
   * a <sigkey> that is >= 0 on success.
   */
  virtual int register_handler (int signum,
                                ACE_Event_Handler *new_sh,
                                ACE_Sig_Action *new_disp = 0,
                                ACE_Event_Handler **old_sh = 0,
                                ACE_Sig_Action *old_disp = 0);

  /**
   * Remove an <ACE_Event_Handler> currently associated with <signum>.
   * We remove the handler if (1) its <sigkey> matches the <sigkey>
   * passed as a parameter or (2) if we've been told to remove all the
   * handlers, i.e., <sigkey> == -1.  If a new disposition is given it
   * is installed and the previous disposition is returned (if desired
   * by the caller).  Returns 0 on success and -1 if <signum> is
   * invalid.
   */
  virtual int remove_handler (int signum,
                              ACE_Sig_Action *new_disp = 0,
                              ACE_Sig_Action *old_disp = 0,
                              int sigkey = -1);

  // = Set/get the handler associated with a particular signal.

  /// Return the head of the list of <ACE_Sig_Handler>s associated with
  /// SIGNUM.
  virtual ACE_Event_Handler *handler (int signum);

  /**
   * Set a new <ACE_Event_Handler> that is associated with SIGNUM at
   * the head of the list of signals.  Return the existing handler
   * that was at the head.
   */
  virtual ACE_Event_Handler *handler (int signum,
                                      ACE_Event_Handler *);

  /**
   * Callback routine registered with sigaction(2) that dispatches the
   * <handle_signal> method of all the pre-registered
   * ACE_Event_Handlers for <signum>
   */
  static void dispatch (int signum, siginfo_t *, ucontext_t *);

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

private:
  /**
   * Keeps track of the id that uniquely identifies each registered
   * signal handler.  This id can be used to cancel a timer via the
   * <remove_handler> method.
   */
  static int sigkey_;

  /// If this is > 0 then a 3rd party library has registered a
  /// handler...
  static int third_party_sig_handler_;
};
#endif /* ACE_HAS_BROKEN_HPUX_TEMPLATES */

#if defined (ACE_HAS_SIG_C_FUNC)
extern "C" void
ace_sig_handler_dispatch (int signum, siginfo_t *info, ucontext_t *context);

#if !defined (ACE_HAS_BROKEN_HPUX_TEMPLATES)
extern "C" void
ace_sig_handlers_dispatch (int signum, siginfo_t *info, ucontext_t *context);
#endif /* ACE_HAS_BROKEN_HPUX_TEMPLATES */

#endif /* ACE_HAS_SIG_C_FUNC */

#if defined (__ACE_INLINE__)
#include "ace/IPC/Signal.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_SIGNAL_HANDLER_H */
