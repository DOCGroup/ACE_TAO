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

#ifndef _NOTIFIER_HANDLER_H
#define _NOTIFIER_HANDLER_H

#include "ace/CORBA_Handler.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Event_Comm_i.h"

#if defined (ACE_HAS_ORBIX) && (ACE_HAS_ORBIX != 0)

class Notifier_Handler
  // = TITLE
  //     Integrate CORBA with the ACE ACE_Reactor.
{
public:
  Notifier_Handler (const char *service_location,
                    const char *marker = "notifier",
                    int putit = 1); // Default marker name.

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

#endif /* ACE_HAS_ORBIX */
#endif /* _NOTIFIER_HANDLER_H */
