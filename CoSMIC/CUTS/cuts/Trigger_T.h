// -*- C++ -*-

//=============================================================================
/**
 * @file    Trigger_T.h
 *
 * $Id$
 *
 * @author James H. Hill <hillj@isis.vanderbilt.edu>
 */
//=============================================================================

#ifndef _CUTS_TRIGGER_H_
#define _CUTS_TRIGGER_H_

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif  // ACE_LACKS_PRAGMA_ONCE

#include "cuts/config.h"
#include "ace/Timer_Heap.h"
#include "ace/Timer_Queue_Adapters.h"

//=============================================================================
/**
 * @class CUTS_Periodic_Trigger_T
 *
 * This class defines a periodic trigger. A periodic trigger is one
 * that processes workload on a periodic basis. It also has an associated
 * probablity for performing the workload to create non-determinism.
 */
//=============================================================================

template <typename COMPONENT>
class CUTS_Periodic_Trigger_T : public ACE_Event_Handler
{
public:
  /// Type definition for the hosting component type.
  typedef COMPONENT Component_Type;

  /// Type definition for pointer to member funcntion.
  typedef void (COMPONENT::* Method_Pointer) (void);

  /**
   * Default constructor. The default settings for the trigger is
   * 1 Hz with a probability of 1.0 of firing.
   */
  CUTS_Periodic_Trigger_T (void);

  /// Destructor.
  virtual ~CUTS_Periodic_Trigger_T (void);

  /**
   * Initalize the periodic trigger. This method must be called before
   * the trigger can be activated. This method will set the target
   * component and the member function that is to be called when the
   * trigger fires.
   *
   * @param[in]     component         Pointer to the target component.
   * @param[in]     method            Target method to invoke on component.
   */
  void init (Component_Type * component,
             Method_Pointer method);

  /**
   * Activate the trigger with the current probability and timeout value.
   * The periodic trigger can only be activated once. If the trigger is
   * already activated it does nothing. If the timeout settings of the
   * trigger change, the it must be deactivated and activated again in
   * order for the change to take place. However, changing the probability
   * of the trigger takes affect instantaneously.
   */
  virtual void activate (long msec);

  /**
   * Reactivate the periodic task with a new timeout value. If the
   * object is not active, it will activate it.
   *
   * @param[in]     msec      Timeout value in milliseconds.
   */
  virtual void reactivate (long msec);

  /// Deactivate the trigger.
  virtual void deactivate (void);

  /// Get the current probability.
  double probability (void) const;

  /**
   * Set the probability. Setting the probability does not require
   * the object to not be active.
   *
   * @param[in]     p     The new probability; [0..1]
   */
  void probability (double p);

  /// Get the current timeout value.
  long timeout (void) const;

private:
  /// Shared implementation for activating
  void schedule_timeout (long msec);

  /// Shared implementation for canceling the timeout.
  void cancel_timeout (void);

  /// Handler for the timeout event.
  int handle_timeout (const ACE_Time_Value &current_time,
                      const void * act);

  /// Pointer the parent component of the stored method.
  Component_Type * component_;

  /// Pointer to the <COMPONENT> member function assigned
  /// to this trigger event.
  Method_Pointer method_;

  /// Timer ID for the trigger.
  long timer_;

  /// Hold the timeout value.
  long timeout_;

  /// The probability of the trigger firing.
  double probability_;

  /// The timer queue for the periodic task.
  ACE_Thread_Timer_Queue_Adapter <ACE_Timer_Heap> timer_queue_;
};

#if defined (__CUTS_INLINE__)
# include "cuts/Trigger_T.inl"
#endif

#include "cuts/Trigger_T.cpp"

#endif  // !defined _CUTS_TRIGGER_H_
