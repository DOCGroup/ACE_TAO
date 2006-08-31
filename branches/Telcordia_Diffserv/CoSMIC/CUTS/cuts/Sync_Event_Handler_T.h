// -*- C++ -*-

//=============================================================================
/**
 * @file        Sync_Event_Handler_T.h
 *
 * $Id$
 *
 * @author      James H. Hill
 */
//=============================================================================

#ifndef _CUTS_SYNC_EVENT_HANDLER_T_H_
#define _CUTS_SYNC_EVENT_HANDLER_T_H_

#include "cuts/Event_Handler_T.h"

//=============================================================================
/**
 * @class CUTS_Sync_Event_Handler_T
 *
 * Implementation of a synchronous event handler. Unlike the
 * CUTS_Async_Event_Handler_T, this event handler uses the ORB thread
 * that invoked is event handling method. It does not return control to
 * the invoking component until it has finished processing it's event.
 * This can also be dependent on whether or not this event is sending
 * sending out any events, and the of it's target event handlers.
 */
//=============================================================================

template <typename COMPONENT, typename EVENTTYPE>
class CUTS_Sync_Event_Handler_T :
  public CUTS_Event_Handler_Base_T <COMPONENT, EVENTTYPE>
{
public:
  /// Type definition for the super class.
  typedef CUTS_Event_Handler_Base_T <COMPONENT,
                                     EVENTTYPE> Event_Handler_Base;

  /// Type definition of the event handler configuration.
  typedef typename Event_Handler_Base::Config_Type Config_Type;

  /// Type definition for the component owning the event handler.
  typedef typename Event_Handler_Base::Component_Type Component_Type;

  /// Type definition for the event type.
  typedef typename Event_Handler_Base::Event_Type Event_Type;

  /// Type definition for the callback method.
  typedef typename Event_Handler_Base::Event_Method Event_Method;

  /**
   * Constructor
   *
   * @param[in]       config      Shared configuration for the handler.
   */
  CUTS_Sync_Event_Handler_T (Config_Type & config);

  /// Destructor.
  virtual ~CUTS_Sync_Event_Handler_T (void);

  /**
   * Handle the event. This will invoke the callback method.
   *
   * @param[in]     ev        Source event.
   */
  void handle_event (EVENTTYPE * ev);

  /**
   * Set the priority.
   *
   * @param[in]     prio      The new priority.
   */
  virtual void priority (int prio);

  /**
   * Get the priority.
   *
   * @return        The current priority.
   */
  virtual int priority (void) const;

  /**
   * Set the number of threads.
   *
   * @param[in]     n     Number of threads.
   */
  virtual void thread_count (size_t n);

  /**
   * Get the number of threads.
   *
   * @return        The number of threads.
   */
  virtual size_t thread_count (void) const;

private:
  /// The priority of the event handler.
  int priority_;
};

#if defined (__CUTS_INLINE__)
#include "cuts/Sync_Event_Handler_T.inl"
#endif

#include "cuts/Sync_Event_Handler_T.cpp"

#endif  // !defined _CUTS_SYNC_EVENT_HANDLER_T_H_
