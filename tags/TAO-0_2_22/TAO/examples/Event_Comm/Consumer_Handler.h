/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    EventComm
//
// = FILENAME
//    Consumer_Handler.h
//
// = DESCRIPTION
//    Subclass of Corba_Handler that sets up the Consumer handler
//    for use with the ACE ACE_Reactor.
//
// = AUTHOR
//    Douglas C. Schmidt (schmidt@cs.wustl.edu)
//
// ============================================================================

#if !defined (_CONSUMER_HANDLER_H)
#define _CONSUMER_HANDLER_H

#include "ace/CORBA_Handler.h"
#include "Event_Comm_i.h"

class Consumer_Handler
{
  // = TITLE
  //    Subclass of Corba_Handler that sets up the Notification
  //    Receiver handler for use with the ACE_Reactor.
  //
  // = DESCRIPTION
  //    Note that this class doesn't inherit from ACE_ST_CORBA_Handler
  //    (unlike the Supplier's Notifier_Handler class).  Instead, it
  //    uses an alternative interface that can be called directly.
public:
  Consumer_Handler (int argc, char *argv[]);

  Event_Comm::Consumer *receiver (void);
  Event_Comm::Notifier *notifier (void);

  virtual int handle_close (ACE_HANDLE = ACE_INVALID_HANDLE,
			    ACE_Reactor_Mask = ACE_Event_Handler::NULL_MASK);
  // Close down the handler.

private:
  ~Consumer_Handler (void);
  // Ensure dynamic allocation.

  Event_Comm::Consumer *receiver_;
  // Pointer to an IDL <Consumer> proxy object.

  Event_Comm::Notifier *notifier_;
  // Pointer to an IDL <Notifier> proxy object.
};

#endif /* _CONSUMER_HANDLER_H */
