/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ACE_wrappers/examples/QOS/
//
// = FILENAME
//    QOS_Event_Handler.h
//
// = AUTHOR
//    Vishal Kachroo
//
// ============================================================================

#ifndef QOS_EVENT_HANDLER_H
#define QOS_EVENT_HANDLER_H

#include "ace/Event_Handler.h"
#include "ace/Reactor.h"

#include "Read_Handler.h"

ACE_RCSID(QOS_Event_Handler, QOS_Event_Handler, "$Id$")

  class ACE_QOS_Event_Handler : public ACE_Event_Handler
  {
  public:
    // = Initialization and Termination methods.
    ACE_QOS_Event_Handler (void);
    // Constructor.

    ACE_QOS_Event_Handler::ACE_QOS_Event_Handler (const ACE_SOCK_Dgram_Mcast &dgram_mcast);
    // Constructor.

    ~ACE_QOS_Event_Handler (void);
    // Destructor. 

    virtual ACE_HANDLE get_handle (void) const;
    // Override this to return the handle of the Dgram_Mcast
    // that we are using.

    virtual int handle_qos (ACE_HANDLE fd);
    // Handles a QOS event. Right now, just 
    // registers a handler for the READ events.

  private:

    ACE_Read_Handler read_handler_;

  };

#endif  /* QOS_EVENT_HANDLER_H */
