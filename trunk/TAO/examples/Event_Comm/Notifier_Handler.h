/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    EventComm
//
// = FILENAME
//    Notifier_Handler.h
//
// = AUTHOR
//    Douglas C. Schmidt (schmidt@cs.wustl.edu)
//
// ============================================================================

#if !defined (_NOTIFIER_HANDLER_H)
#define _NOTIFIER_HANDLER_H

#include "ace/CORBA_Handler.h"
#include "Event_Comm_i.h"

class Notifier_Handler
  // = TITLE
  //     Integrate CORBA with the ACE ACE_Reactor.
{
public:
  Notifier_Handler ();
  // This constructor gets the notifier object from the Naming Service.

  Notifier_Handler (const char *service_location,
		    const char *marker = "notifier",
		    int putit = 1); // Default marker name.
  // This constructor starts up the ORB etc and
  // registers the notifier object with the Naming Sxervice.

  Event_Comm::Notifier *notifier (void);
  void notifier (Event_Comm::Notifier *);

  virtual int handle_close (ACE_HANDLE = ACE_INVALID_HANDLE,
			    ACE_Reactor_Mask = ACE_Event_Handler::NULL_MASK);
  // Close down the handler.

private:
  ~Notifier_Handler (void);
  // Ensure dynamic allocation.

  Event_Comm::Notifier *notifier_;
  // Pointer to an <Event_Comm::Notifier> object.
};

#endif /* _NOTIFIER_HANDLER_H */
