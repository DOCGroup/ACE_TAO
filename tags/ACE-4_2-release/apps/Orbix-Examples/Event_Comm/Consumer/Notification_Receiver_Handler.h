/* -*- C++ -*- */
// $Id$


// ============================================================================
//
// = LIBRARY
//    EventComm
// 
// = FILENAME
//    Notifier_Receiver_Handler.h
//
// = DESCRIPTION 
//    Subclass of Corba_Handler that sets up the Notification_Receiver handler
//    for use with the ACE ACE_Reactor.
//
// = AUTHOR
//    Douglas C. Schmidt (schmidt@cs.wustl.edu)
// 
// ============================================================================

#if !defined (_NOTIFICATION_RECEIVER_HANDLER_H)
#define _NOTIFICATION_RECEIVER_HANDLER_H

#include "ace/CORBA_Handler.h"
#include "Event_Comm_i.h"

#if defined (ACE_HAS_ORBIX)

class Notification_Receiver_Handler 
  // = TITLE
  //    Subclass of Corba_Handler that sets up the Notification Receiver handler
  //    for use with the ACE ACE_Reactor.
  //
  // = DESCRIPTION
  //    Note that this class doesn't inherit from ACE_ST_CORBA_Handler (unlike
  //    the Supplier's Notifier_Handler class).  Instead, it uses an
  //    alternative interface that can be called directly.
{
public:
  Notification_Receiver_Handler (int argc, char *argv[]);

  Event_Comm::Notification_Receiver *receiver (void);
  Event_Comm::Notifier *notifier (void);

  virtual int handle_close (ACE_HANDLE = ACE_INVALID_HANDLE, 
			    ACE_Reactor_Mask = ACE_Event_Handler::NULL_MASK);
  // Close down the handler.

private:
  ~Notification_Receiver_Handler (void);
  // Ensure dynamic allocation.

  Event_Comm::Notification_Receiver *receiver_;
  // Pointer to an IDL <Notification_Receiver> proxy object.

  Event_Comm::Notifier *notifier_;
  // Pointer to an IDL <Notifier> proxy object.
};

#endif /* ACE_HAS_ORBIX */
#endif /* _NOTIFICATION_RECEIVER_HANDLER_H */
