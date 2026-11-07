/* -*- C-*- */

//=============================================================================
/**
 *  @file   FoxReactor.h
 *
 *  @author Dmitriy Nikitinskiy <nick@bel.ru>
 *  @author Based in part in the ACE_XtReactor implementation by
 *  @author Eric C. Newton's <ecn@clark.net>
 *  @author Kirill Rybaltchenko <Kirill.Rybaltchenko@cern.ch>
 *  @author Douglas C. Schmidt <d.schmidt@vanderbilt.edu>
 */
//=============================================================================


#ifndef ACE_FOXREACTOR_H
#define ACE_FOXREACTOR_H
#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Select_Reactor.h"
#include "ace/FoxReactor/ACE_FoxReactor_export.h"

#include <fx.h>

/**
 * @class ACE_FoxReactor
 *
 * @brief A Reactor implementation that uses the FOX toolkit (www.fox-toolkit.org)
 * for event demultiplexing.  This will let us integrate the FOX
 * toolkit with ACE and/or TAO.
 */
class ACE_FoxReactor_Export ACE_FoxReactor : public FXObject, public ACE_Select_Reactor
{
  FXDECLARE(ACE_FoxReactor)
public:
  ACE_FoxReactor (FXApp *a = nullptr,
                  size_t size = DEFAULT_SIZE,
                  bool restart = false,
                  ACE_Sig_Handler * = nullptr);
  ~ACE_FoxReactor () override;

  ACE_FoxReactor (ACE_FoxReactor const &) = delete;
  ACE_FoxReactor (ACE_FoxReactor &&) = delete;
  ACE_FoxReactor &operator = (ACE_FoxReactor const &) = delete;
  ACE_FoxReactor &operator = (ACE_FoxReactor &&) = delete;

  void fxapplication (FXApp *a);

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

  // = Integrate with the Fox callback function mechanism.
  long onFileEvents (FXObject *, FXSelector, void *);
  long onTimerEvents (FXObject *, FXSelector, void *);


protected:
  // = Register timers/handles with Fox.
  /// Register a single @a handler.
  int register_handler_i (ACE_HANDLE handle,
                          ACE_Event_Handler *handler,
                          ACE_Reactor_Mask mask) override;

  /// Register a set of <handlers>.
  int register_handler_i (ACE_Handle_Set const &handles,
                          ACE_Event_Handler *handler,
                          ACE_Reactor_Mask mask) override;

  /// Remove the <handler> associated with this @a handle.
  int remove_handler_i (ACE_HANDLE handle,
                        ACE_Reactor_Mask mask) override;

  /// Remove a set of <handles>.
  int remove_handler_i (ACE_Handle_Set const &handles,
                        ACE_Reactor_Mask) override;

  /// Wait for events to occur.
  int wait_for_multiple_events (ACE_Select_Reactor_Handle_Set &,
                                ACE_Time_Value *) override;

  int FoxWaitForMultipleEvents (int,
                                ACE_Select_Reactor_Handle_Set &,
                                ACE_Time_Value *);

private:
  /// This method ensures there's an Fox timeout for the first timeout
  /// in the Reactor's Timer_Queue.
  void reset_timeout ();

  FXApp *fxapp {};
};

#include /**/ "ace/post.h"
#endif /* ACE_FOXREACTOR_H */

