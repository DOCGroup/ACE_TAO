/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    apps
// 
// = FILENAME
//    Concrete_Proxy_Handlers.h
//
// = DESCRIPTION
//    These are all the subclasses of Proxy_Handler that define the
//    appropriate threaded/reactive Consumer/Supplier behavior.
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (_CONCRETE_PROXY_HANDLER)
#define _CONCRETE_PROXY_HANDLER

#include "Proxy_Handler.h"

class Supplier_Proxy : public Proxy_Handler
  // = TITLE
  //     Handles reception of Events from Suppliers
  //
  // = DESCRIPTION
  //     Performs framing and error checking.
{
public:
  // = Initialization method.
  Supplier_Proxy (const Proxy_Config_Info &);

protected:
  // = All the following methods are upcalls, so they can be protected.

  virtual int handle_input (ACE_HANDLE = ACE_INVALID_HANDLE);
  // Receive and process peer events.

  virtual int recv (ACE_Message_Block *&);
  // Receive an event from a Supplier.

  int forward (ACE_Message_Block *event);
  // Forward the <event> to its appropriate Consumer.  This delegates
  // to the <ACE_Event_Channel> to do the actual forwarding.

  ACE_Message_Block *msg_frag_;
  // Keep track of event fragment to handle non-blocking recv's from
  // Suppliers.
};

class Consumer_Proxy : public Proxy_Handler
  // = TITLE
  //     Handles transmission of events to Consumers.
  //
  // = DESCRIPTION
  //     Performs queueing and error checking.  Uses a single-threaded
  //     Reactive approach to handle flow control.
{
public:
  // = Initialization method.
  Consumer_Proxy (const Proxy_Config_Info &);

  virtual int put (ACE_Message_Block *event, 
		   ACE_Time_Value * = 0);
  // Send an event to a Consumer (may be queued if necessary).

protected:
  // = We'll allow up to 16 megabytes to be queued per-output proxy.
  enum {MAX_QUEUE_SIZE = 1024 * 1024 * 16};

  virtual int handle_output (ACE_HANDLE);
  // Finish sending event when flow control conditions abate.

  int nonblk_put (ACE_Message_Block *mb);
  // Perform a non-blocking put().

  virtual ssize_t send (ACE_Message_Block *);
  // Send an event to a Consumer.

  virtual int handle_input (ACE_HANDLE);
  // Receive and process shutdowns from a Consumer.
};

class Thr_Consumer_Proxy : public Consumer_Proxy
  // = TITLE
  //    Runs each Output Proxy_Handler in a separate thread.
{
public:
  Thr_Consumer_Proxy (const Proxy_Config_Info &);

  virtual int open (void *);
  // Initialize the threaded Consumer_Proxy object and spawn a new
  // thread.

  virtual int put (ACE_Message_Block *, ACE_Time_Value * = 0);
  // Send a message to a peer.

protected:
  virtual int handle_input (ACE_HANDLE);
  // Called when Peer shutdown unexpectedly.

  virtual int svc (void);
  // Transmit peer messages.
};

class Thr_Supplier_Proxy : public Supplier_Proxy
  // = TITLE
  //    Runs each Input Proxy_Handler in a separate thread.
{
public:
  Thr_Supplier_Proxy (const Proxy_Config_Info &pci);

  virtual int open (void *);
  // Initialize the object and spawn a new thread.

protected:
  virtual int svc (void);
  // Transmit peer messages.
};

#endif /* _CONCRETE_PROXY_HANDLER */
