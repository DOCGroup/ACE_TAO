/* -*- C++ -*- */
// $Id$


// ============================================================================
//
// = LIBRARY
//    EventComm
// 
// = FILENAME
//    Input_Handler.h
//
// = DESCRIPTION 
//    Subclass of ACE ACE_Service_Object that receives unsubscribes from
//    the Notifier when input is received from the keyboard.
//
// = AUTHOR
//    Douglas C. Schmidt (schmidt@cs.wustl.edu)
// 
// ============================================================================

#if !defined (_INPUT_HANDLER_H)
#define _INPUT_HANDLER_

#include "ace/Service_Config.h"

#if defined (ACE_HAS_ORBIX)
// Forward declaration.
class Notification_Receiver_Handler;

class Input_Handler : public ACE_Service_Object
  // = TITLE
  //   Handles input events generated from a keyboard.
  //
  // = DESCRIPTION
{
public:
  Input_Handler (Notification_Receiver_Handler *, ACE_HANDLE h = 0);
  
  virtual int handle_input (ACE_HANDLE);
  // Dispatch the callback when events occur.

  virtual int handle_close (ACE_HANDLE = ACE_INVALID_HANDLE, 
			    ACE_Reactor_Mask = ACE_Event_Handler::NULL_MASK);
  // Close down the handler.

  int consumer_initiated_shutdown (void);
  // Report whether the Consumer initiated the shutdown.

  void consumer_initiated_shutdown (int);
  // Indicate that the Consumer initiated the shutdown.

private:
  ~Input_Handler (void);
  // Ensure dynamic allocation.

  virtual ACE_HANDLE get_handle (void) const;

  ACE_HANDLE handle_;
  // ACE_HANDLE where the input comes from. 

  Notification_Receiver_Handler *receiver_handler_;
  // Pointer to the <Notification_Receiver_Handler> that 
  // receives notifications from the <Event_Comm::Notifier>.

  int consumer_initiated_shutdown_;
  // Keep track of whether the Consumer initiated the shutdown.
};

#endif /* ACE_HAS_ORBIX */
#endif /* _INPUT_HANDLER_H */
