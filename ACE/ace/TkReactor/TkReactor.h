// -*- C++ -*-

//=============================================================================
/**
 *  @file    TkReactor.h
 *
 *  @author Nagarajan Surendran <naga@cs.wustl.edu>
 */
//=============================================================================


#ifndef ACE_TKREACTOR_H
#define ACE_TKREACTOR_H
#include /**/ "ace/pre.h"

#include /**/ "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/TkReactor/ACE_TkReactor_export.h"
#include "ace/Select_Reactor.h"
#include /**/ <tk.h>
#include <memory>

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

class ACE_TkReactor;
class ACE_TkReactor_Input_Callback;

/**
 * @class ACE_TkReactorID
 *
 * @brief This little class is necessary due to the way that Microsoft
 * implements sockets to be pointers rather than indices.
 */
class ACE_TkReactor_Export ACE_TkReactorID
{
public:
  ACE_TkReactorID () = default;
  ~ACE_TkReactorID ();

  ACE_TkReactorID (ACE_TkReactorID const &) = delete;
  ACE_TkReactorID (ACE_TkReactorID &&) = delete;
  ACE_TkReactorID &operator = (ACE_TkReactorID const &) = delete;
  ACE_TkReactorID &operator = (ACE_TkReactorID &&) = delete;

  /// Underlying handle.
  ACE_HANDLE handle_ {};

  /// Pointer to next node in the linked list.
  ACE_TkReactorID *next_ {}; // Non-owning link; ACE_TkReactor deletes the list nodes.

  /// Owns the Tcl callback record registered for this handle.
  std::unique_ptr<ACE_TkReactor_Input_Callback> callback_;
};

class ACE_TkReactor_Export ACE_TkReactor_Input_Callback
{
public:
  ACE_TkReactor_Input_Callback () = default;

  ACE_TkReactor_Input_Callback (ACE_TkReactor_Input_Callback const &) = delete;
  ACE_TkReactor_Input_Callback (ACE_TkReactor_Input_Callback &&) = delete;
  ACE_TkReactor_Input_Callback &operator = (ACE_TkReactor_Input_Callback const &) = delete;
  ACE_TkReactor_Input_Callback &operator = (ACE_TkReactor_Input_Callback &&) = delete;

  ACE_TkReactor *reactor_ {};
  ACE_HANDLE handle_ {};
};

/**
 * @class ACE_TkReactor
 *
 * @brief An object-oriented event demultiplexor and event handler
 * dispatcher that uses the Tk functions.
 */
class ACE_TkReactor_Export ACE_TkReactor : public ACE_Select_Reactor
{
public:
  ACE_TkReactor (size_t size = DEFAULT_SIZE,
                 bool restart = false,
                 ACE_Sig_Handler * = nullptr);

  ~ACE_TkReactor () override;

  ACE_TkReactor (ACE_TkReactor const &) = delete;
  ACE_TkReactor (ACE_TkReactor &&) = delete;
  ACE_TkReactor &operator = (ACE_TkReactor const &) = delete;
  ACE_TkReactor &operator = (ACE_TkReactor &&) = delete;

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
  // = Register timers/handles with Tk.
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

  /// Removes an Tk FileHandler.
  virtual void remove_TkFileHandler (ACE_HANDLE handle);

  /// Wait for events to occur.
  int wait_for_multiple_events (ACE_Select_Reactor_Handle_Set &,
                                ACE_Time_Value *) override;

  ///Wait for Tk events to occur.
  virtual int TkWaitForMultipleEvents (int,
                                      ACE_Select_Reactor_Handle_Set &,
                                      ACE_Time_Value *);

  ACE_TkReactorID *ids_ {}; // Head of the manually managed linked list.
  Tk_TimerToken timeout_ {};

private:
  /// This method ensures there's a Tk timeout for the first timeout in
  /// the Reactor's Timer_Queue.
  void reset_timeout ();

  // = Integrate with the X callback function mechanism.
  static void TimerCallbackProc (ClientData cd);
  static void InputCallbackProc (ClientData cd, int mask);
};

ACE_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* ACE_TK_REACTOR_H */
