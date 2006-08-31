// -*- C++ -*-

//=============================================================================
/**
 * @file        Event_Handler_Manager_T.h
 *
 * $Id$
 *
 * @author      James H. Hill
 */
//=============================================================================

#ifndef _CUTS_EVENT_HANDLER_MANAGER_T_H_
#define _CUTS_EVENT_HANDLER_MANAGER_T_H_

#include "cuts/Event_Handler_T.h"

//=============================================================================
/**
 * @class CUTS_Event_Handler_Manager_T
 *
 * Container class for managing event handler implemenatations. The
 * type of event handler can be altered by setting the mode.
 */
//=============================================================================

template <typename COMPONENT, typename EVENTTYPE>
class CUTS_Event_Handler_Manager_T :
  public CUTS_Event_Handler_T <COMPONENT, EVENTTYPE>
{
public:
  /// Type definition for the strategy type.
  typedef CUTS_Event_Handler_T <COMPONENT, EVENTTYPE> Event_Handler_Type;

  /// Type definition of the event handler configuration.
  typedef CUTS_Event_Handler_Config_T <COMPONENT,
                                       EVENTTYPE> Config_Type;

  /// Type definition for the component owning the event handler.
  typedef typename Config_Type::Component_Type Component_Type;

  /// Type definition for the event type.
  typedef typename Config_Type::Event_Type Event_Type;

  /// Type definition for the callback method.
  typedef typename Config_Type::Event_Method Event_Method;

  /**
   * Constructor.
   *
   * @param[in]       mode      Initial mode of the event handler.
   */
  CUTS_Event_Handler_Manager_T (
    CUTS_Event_Handler::Event_Mode mode = CUTS_Event_Handler::ASYNCHRONOUS);

  /// Destructor.
  virtual ~CUTS_Event_Handler_Manager_T (void);

  /// Bind the event handler to the port agent and callback method.
  void bind (Component_Type * component,
             Event_Method method);

  /// Unbind the event handler from the agent and method.
  void unbind (void);

  /// Handle the event. This will invoke the callback method.
  void handle_event (EVENTTYPE * ev);

  /// Activate the event handler.
  void activate (void);

  /// Deactivate the event handler.
  void deactivate (void);

  /// Get the port agent for the event handler.
  CUTS_Port_Agent & port_agent (void);

  /**
   * Set the implementation of the event handler. The caller does
   * not have to worry about managing the implemenatation.
   *
   * @param[in]     impl        Pointer to the implemenation.
   */
  void mode (CUTS_Event_Handler::Event_Mode mode);

  /**
   * Get the event handling mode.
   *
   * @return    The mode of the event handler, e.g, sync or async.
   */
  CUTS_Event_Handler::Event_Mode mode (void) const;

  /**
   * Set the priority.
   *
   * @param[in]     prio      The new priority.
   */
  void priority (int prio);

  /**
   * Get the priority.
   *
   * @return        The current priority.
   */
  int priority (void) const;

  /**
   * Set the number of threads.
   *
   * @param[in]     n     Number of threads.
   */
  void thread_count (size_t n);

  /**
   * Get the number of threads.
   *
   * @return        The number of threads.
   */
  size_t thread_count (void) const;

  /**
   * Get the name of the event handler.
   *
   * @return      Name of the event handler.
   */
  const char * name (void) const;

  /**
   * Set the name of the event handler.
   *
   * @param[in]     name      Name of the event handler.
   */
  void name (const char * name);

private:
  /// The implementation type for the event handler.
  ACE_Auto_Ptr <Event_Handler_Type> impl_;

  /// The current mode of the event handler.
  CUTS_Event_Handler::Event_Mode mode_;

  /// Configuration for the event handler.
  Config_Type config_;
};

#if defined (__CUTS_INLINE__)
#include "cuts/Event_Handler_Manager_T.inl"
#endif

#include "cuts/Event_Handler_Manager_T.cpp"

#endif  // !defined _CUTS_EVENT_HANDLER_MANAGER_T_H_
