// -*- C++ -*-

//=============================================================================
/**
 * @file      EventHandler_T.h
 *
 * $Id$
 *
 * @author    James H. Hill
 */
//=============================================================================

#ifndef _CUTS_EVENT_HANDLER_T_H_
#define _CUTS_EVENT_HANDLER_T_H_

#include "cuts/Event_Handler.h"
#include "cuts/PortAgent.h"
#include "ace/Auto_Ptr.h"
#include "ace/SString.h"

//=============================================================================
/**
 * @class CUTS_Event_Handler_Config_T
 *
 * Shared configuration object for event handlers. This allows the
 * CUTS_Event_Handler_Manager_T object to communicate with different
 * implementations.
 */
//=============================================================================

template <typename COMPONENT, typename EVENTTYPE>
class CUTS_Event_Handler_Config_T
{
public:
  /// Type definition for the component owning the event handler.
  typedef COMPONENT Component_Type;

  /// Type definition for the event type.
  typedef EVENTTYPE Event_Type;

  /// Type definition for the callback method.
  typedef void (COMPONENT::*Event_Method) (EVENTTYPE *,
                                           CUTS_Activation_Record *);

  /// Default constructor.
  CUTS_Event_Handler_Config_T (void);

  /**
   * Helper method to dispatch an event
   *
   * @param[in]     ev              Source event.
   * @param[in]     record          Pointer to activation record.
   */
  bool dispatch_event (EVENTTYPE * ev,
                       CUTS_Activation_Record * record) const;

  /// Port agent for the event handler.
  CUTS_Port_Agent port_agent_;

  /// Pointer to the component that owns the event handler.
  Component_Type * component_;

  /// Method invoked when an event is received.
  Event_Method method_;
};

//=============================================================================
/**
 * @class CUTS_Event_Handler_T
 *
 * Event handler type for components. This class it parameterized by
 * the component and the event type it receieves. This allows the event
 * handler to be used in any component and send any type of valid event.
 */
//=============================================================================

template <typename COMPONENT, typename EVENTTYPE>
class CUTS_Event_Handler_T : public CUTS_Event_Handler
{
public:
  /// Type definition for the component owning the event handler.
  typedef COMPONENT Component_Type;

  /// Type definition for the event type.
  typedef EVENTTYPE Event_Type;

  /// Type definition for the callback method.
  typedef void (COMPONENT::*Event_Method) (EVENTTYPE *,
                                           CUTS_Activation_Record *);

  /**
   * Handle an event.
   *
   * @param[in]       ev      Pointer to the event.
   */
  virtual void handle_event (EVENTTYPE * ev) = 0;

  /// Activate the event handler.
  virtual void activate (void) = 0;

  /// Deactivate the event handler.
  virtual void deactivate (void) = 0;

  /// Destructor.
  virtual ~CUTS_Event_Handler_T (void);

protected:
  /// Default constructor.
  CUTS_Event_Handler_T (void);

private:
  // prevent the following operations.
  CUTS_Event_Handler_T (const CUTS_Event_Handler_T &);
  const CUTS_Event_Handler_T & operator = (const CUTS_Event_Handler_T &);
};

//=============================================================================
/**
 * @class CUTS_Event_Handler_Base_T
 *
 * Base implemetatation for all event handlers.
 */
//=============================================================================

template <typename COMPONENT, typename EVENTTYPE>
class CUTS_Event_Handler_Base_T :
  public CUTS_Event_Handler_T <COMPONENT, EVENTTYPE>
{
public:
  /// Type definition of the event handler configuration.
  typedef CUTS_Event_Handler_Config_T <COMPONENT,
                                       EVENTTYPE> Config_Type;

  /// Type definition for the component owning the event handler.
  typedef typename Config_Type::Component_Type Component_Type;

  /// Type definition for the event type.
  typedef typename Config_Type::Event_Type Event_Type;

  /// Type definition for the callback method.
  typedef typename Config_Type::Event_Method Event_Method;

  /// Destructor.
  virtual ~CUTS_Event_Handler_Base_T (void);

  /// Activate the event handler.
  virtual void activate (void);

  /// Deactivate the event handler.
  virtual void deactivate (void);

  /**
   * Handle an event.
   *
   * @param[in]       ev      Pointer to the event.
   */
  virtual void handle_event (EVENTTYPE * ev) = 0;

  /**
   * Return the active state of the event handler.
   *
   * @retval    true      The event handler is active.
   * @retval    false     The event handler is not active.
   */
  bool is_active (void) const;

protected:
  /**
   * Constructor.
   *
   * @param[in]     agent       Port agent for the event handler.
   */
  CUTS_Event_Handler_Base_T (Config_Type & config);

  /**
   * Shared implementation for handing an event. This method performs
   * the necessary operations to benchmark the event handling method.
   *
   * @param[in]     ev            Pointer to the event.
   * @param[in]     queue_time    Amount of time the event was queued.
   */
  void handle_event_i (EVENTTYPE *ev,
                       const ACE_Time_Value & queue_time);

  /// Pointer to the component that owns the event handler.
  Component_Type * component_;

  /// Method invoked when an event is received.
  Event_Method method_;

  /**
   * Get the contained port agent.
   *
   * @return      Reference to the port agent.
   */
  CUTS_Port_Agent & port_agent (void);

  /**
   * Get the name of the event handler.
   *
   * @return      Name of the event handler.
   */
  const char * name (void) const;

private:
  /// Active state of the event handler.
  bool active_;

  /// Reference to the configuration type.
  Config_Type & config_;
};

#if defined (__CUTS_INLINE__)
#include "cuts/Event_Handler_T.inl"
#endif

#include "cuts/Event_Handler_T.cpp"

#endif  // !defined _CUTS_EVENT_HANDLER_T_H_
