/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ACE_wrappers/ace
//
// = FILENAME
//    QOS_Decorator.h
//
// = AUTHOR
//    Vishal Kachroo <vishal@cs.wustl.edu>
//
// ============================================================================

#ifndef QOS_DECORATOR_H
#define QOS_DECORATOR_H
#include "ace/pre.h"

#include "ace/Reactor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/INET_Addr.h"
#include "ace/Event_Handler.h"
#include "ace/SOCK_Dgram_Mcast_QoS.h"

ACE_RCSID(QOS_Decorator, QOS_Decorator, "$Id$")
  
class ACE_Export ACE_QoS_Decorator_Base : public ACE_Event_Handler
{
  // = TITLE
  //     This class is the Decorator Pattern Base class for decorating
  //     ACE_Event_Handler.
  //
  // = DESCRIPTION
  //       It simply forwards the requests for get_handle (), 
  //       handle_input () and handle_qos () to its event_handler_ 
  //       component. Concrete decorators for ACE_Event_Handler will use 
  //       this class to access the basic event handler functionality and 
  //       decorate that by their own implementation.
  
public:
  
  // Initialization and termination methods.
  ACE_QoS_Decorator_Base (void);
  // Constructor.

  ACE_QoS_Decorator_Base (ACE_Event_Handler *event_handler);
  // Constructor.

  ~ACE_QoS_Decorator_Base (void);
  // Destructor.

  virtual ACE_HANDLE get_handle (void) const;
  // Forwards the request to its event_handler_ component.

  virtual int handle_input (ACE_HANDLE fd);
  // Forwards the request to its event_handler_ component.

  virtual int handle_qos (ACE_HANDLE fd);
  // Forwards the request to its event_handler_ component.

private:
  
  ACE_Event_Handler *event_handler_;
  // The event handler that is decorated by this class.

};

class ACE_Export ACE_QoS_Event_Handler : public ACE_Event_Handler 
{
  // = TITLE
  //     This Handler is registered with the Reactor for QoS events.
  //
  // = DESCRIPTION
  //       Concrete QoS decorator uses this class to receive QoS events 
  //       for RAPI.  It hides the application from knowing that it is 
  //       receiving QoS events on a different socket so the application
  //       doesnt have to be designed differently for RAPI and GQoS.

  ~ACE_QoS_Event_Handler (void);
  // Destructor.

  virtual ACE_HANDLE get_handle (void) const;
  // Returns the RAPI file descriptor for receiving QoS events.

  virtual int handle_input (ACE_HANDLE fd);
  // Calls the base class handle_input ().

  void qos_session (ACE_QoS_Session *qos_session);
  // Sets the QoS session.

  friend class ACE_QoS_Decorator;

private:

  ACE_QoS_Event_Handler (void);
  // Constructor is private because only ACE_QoS_Decorator should
  // create this object.

  ACE_QoS_Event_Handler (ACE_QoS_Decorator_Base *decorator_base);
  // The QoS Decorator passes in its base for this handler to use.

  ACE_QoS_Session *qos_session_;
  // Used to get to the RAPI file descriptor for QoS Events.

  ACE_QoS_Decorator_Base *decorator_base_;
  // Requests on the class are forwarded to this base class;

};

class ACE_Export ACE_QoS_Decorator : public ACE_QoS_Decorator_Base
{
  // = TITLE
  //     Concrete QoS Decorator.
  //
  // = DESCRIPTION
  //       Decorates the ACE_Event_Handler to additionally handle QoS 
  //       events uniformly for different QoS mechanisms like RAPI and 
  //       GQoS.

public:

  // Initialization and termination methods.
  ACE_QoS_Decorator (void);
  // Constructor.

  ACE_QoS_Decorator (ACE_Event_Handler *event_handler,
                     ACE_QoS_Session *qos_session,
                     ACE_Reactor *reactor = ACE_Reactor::instance ());
  // Constructor.

  ~ACE_QoS_Decorator (void);
  // Destructor.

  virtual ACE_HANDLE get_handle (void) const;
  // Calls the base class get_handle ().

  virtual int handle_input (ACE_HANDLE fd);
  // Calls the base class handle_input ().

  virtual int handle_qos (ACE_HANDLE fd);
  // Calls the base class handle_qos ().
  
  int init (void);
  // This method registers the QoS Event Handler with the Reactor
  // to receive RAPI events.

private:

  ACE_QoS_Decorator_Base *decorator_base_;
  // Requests on the class are forwarded to this base class;

  ACE_QoS_Event_Handler *qos_event_handler_;
  // Handles the QoS events and in that sense decorates the usual
  // ACE_Event_Handler.

  ACE_QoS_Session *qos_session_;
  // Passed to the ACE_QoS_Event_Handler for retrieving the RAPI
  // session specific information like rapi_fd.

  ACE_Reactor *reactor_;
  // If the application wants to use an instance of Reactor other
  // than the Singleton one.

};

#include "ace/post.h"
#endif  /* QOS_DECORATOR_H */
