/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ACE_wrappers/examples/QOS
//
// = FILENAME
//    RAPI_Event_Handler.h
//
// = AUTHOR
//    Vishal Kachroo <vishal@cs.wustl.edu>
//
// ============================================================================

#ifndef RAPI_EVENT_HANDLER_H
#define RAPI_EVENT_HANDLER_H

#include "ace/Event_Handler.h"
// #include "ace/Reactor.h"
// #include "ace/INET_Addr.h"
// #include "ace/SOCK_Dgram_Mcast.h"
#include "ace/QoS_Session.h"

ACE_RCSID(RAPI_Event_Handler, RAPI_Event_Handler, "$Id$")

  class ACE_RAPI_Event_Handler : public ACE_Event_Handler
  {
  public:
    // = Initialization and Termination methods.
    ACE_RAPI_Event_Handler (void);
    // Constructor.
    
    ACE_RAPI_Event_Handler (ACE_QoS_Session *qos_session);
    // Constructor.

    ~ACE_RAPI_Event_Handler (void);
    // Destructor. 

    virtual ACE_HANDLE get_handle (void) const;
    // Override this to return the handle of the UNIX domain socket
    // that RAPI uses to communicate with the RAPI daemon.
    
    virtual int handle_qos (ACE_HANDLE fd);
    // Handles a QOS event. Right now, just 
    // prints a message.

    virtual int handle_input (ACE_HANDLE fd);
    // Handles a READ event.
    
  private:

    ACE_QoS_Session *qos_session_;
    
  };

#endif  /* RAPI_EVENT_HANDLER_H */

