// -*- C++ -*-

//=============================================================================
/**
 *  @file    XtReactor.h
 *
 *  @author Eric C. Newton <ecn@clark.net>
 *  @author Kirill Rybaltchenko <Kirill.Rybaltchenko@cern.ch>
 *  @author Douglas C. Schmidt <d.schmidt@vanderbilt.edu>
 */
//=============================================================================


#ifndef ACE_XTREACTOR_H
#define ACE_XTREACTOR_H
#include /**/ "ace/pre.h"

#include /**/ "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */
#include "ace/XtReactor/ACE_XtReactor_export.h"
#include "ace/Select_Reactor.h"

#include /**/ <X11/Intrinsic.h>

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class ACE_XtReactorID
 *
 * @brief This little class is necessary due to the way that Microsoft
 * implements sockets to be pointers rather than indices.
 */
class ACE_XtReactor_Export ACE_XtReactorID
{
public:
  ACE_XtReactorID () = default;

  ACE_XtReactorID (ACE_XtReactorID const &) = delete;
  ACE_XtReactorID (ACE_XtReactorID &&) = delete;
  ACE_XtReactorID &operator = (ACE_XtReactorID const &) = delete;
  ACE_XtReactorID &operator = (ACE_XtReactorID &&) = delete;

  /// Magic cookie.
  XtInputId id_ {};

  /// Underlying handle.
  ACE_HANDLE handle_ {};

  /// Pointer to next node in the linked list.
  ACE_XtReactorID *next_ {}; // Non-owning link; ACE_XtReactor deletes the list nodes.
};

/**
 * @class ACE_XtReactor
 *
 * @brief An object-oriented event demultiplexor and event handler
 * dispatcher that uses the X Toolkit functions.
 */
class ACE_XtReactor_Export ACE_XtReactor : public ACE_Select_Reactor
{
public:
  ACE_XtReactor (XtAppContext context = nullptr,
                 size_t size = DEFAULT_SIZE,
                 bool restart = false,
                 ACE_Sig_Handler * = nullptr);
  ~ACE_XtReactor () override;

  ACE_XtReactor (ACE_XtReactor const &) = delete;
  ACE_XtReactor (ACE_XtReactor &&) = delete;
  ACE_XtReactor &operator = (ACE_XtReactor const &) = delete;
  ACE_XtReactor &operator = (ACE_XtReactor &&) = delete;

  XtAppContext context () const;
  void context (XtAppContext);

  // = Timer operations.
  long schedule_timer (ACE_Event_Handler *event_handler,
                       void const *arg,
                       ACE_Time_Value const &delay,
                       ACE_Time_Value const &interval) override;
  int reset_timer_interval (long timer_id,
                            ACE_Time_Value const &interval) override;
  int cancel_timer (ACE_Event_Handler *handler,
                    int dont_call_handle_close = 1) override;

  int cancel_timer (long timer_id,
                    void const **arg = nullptr,
                    int dont_call_handle_close = 1) override;

protected:
  // = Register timers/handles with Xt.
  /// Register a single @a handler.
  int register_handler_i (ACE_HANDLE handle,
                          ACE_Event_Handler *handler,
                          ACE_Reactor_Mask mask) override;

  /// Register a set of <handlers>.
  int register_handler_i (ACE_Handle_Set const &handles,
                          ACE_Event_Handler *handler,
                          ACE_Reactor_Mask mask) override;

  /// Remove the @a handler associated with this @a handle.
  int remove_handler_i (ACE_HANDLE handle,
                        ACE_Reactor_Mask mask) override;

  /// Remove a set of <handles>.
  int remove_handler_i (ACE_Handle_Set const &handles,
                        ACE_Reactor_Mask) override;

  /// Suspend the <Event_Handler> associated with @a handle
  int suspend_i (ACE_HANDLE handle) override;

  /// Resume the <Event_Handler> associated with @a handle
  int resume_i (ACE_HANDLE handle) override;

  /// Synchronize XtInputHandler for @a handle
  virtual void synchronize_XtInput (ACE_HANDLE handle);

  /// Compute needed Xt condition by looking at base class wait set.
  virtual int compute_Xt_condition (ACE_HANDLE handle);

  /// Wait for events to occur.
  int wait_for_multiple_events (ACE_Select_Reactor_Handle_Set &,
                                ACE_Time_Value *) override;

  ///Wait for Xt events to occur.
  virtual int XtWaitForMultipleEvents (int,
                                      ACE_Select_Reactor_Handle_Set &,
                                      ACE_Time_Value *);

  XtAppContext context_ {}; // Borrowed from the X toolkit application.
  ACE_XtReactorID *ids_ {}; // Head of the manually managed linked list.
  XtIntervalId timeout_ {};

private:
  /// This method ensures there's an Xt timeout for the first timeout
  /// in the Reactor's Timer_Queue.
  void reset_timeout ();

  // = Integrate with the X callback function mechanism.
  static void TimerCallbackProc (XtPointer closure, XtIntervalId *id);
  static void InputCallbackProc (XtPointer closure,
                                 int *source,
                                 XtInputId *id);
};

ACE_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* ACE_XTREACTOR_H */
