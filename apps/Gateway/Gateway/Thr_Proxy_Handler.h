/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    apps
// 
// = FILENAME
//    Thr_Proxy_Handler.h
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (_THR_IO_HANDLER)
#define _THR_IO_HANDLER

#include "Proxy_Handler.h"

#if defined (ACE_HAS_THREADS)
class Thr_Consumer_Proxy : public Consumer_Proxy
  // = TITLE
  //    Runs each Output Proxy_Handler in a separate thread.
{
public:
  Thr_Consumer_Proxy (Event_Forwarding_Discriminator *, 
			Proxy_Handler_Connector *, 
			ACE_Thread_Manager *,
			int socket_queue_size);

  virtual int open (void *);
  // Initialize the threaded Consumer_Proxy object and spawn a new
  // thread.

  virtual int handle_input (ACE_HANDLE);
  // Called when Peer shutdown unexpectedly.

  virtual int put (ACE_Message_Block *, ACE_Time_Value * = 0);
  // Send a message to a peer.

  virtual int svc (void);
  // Transmit peer messages.
};

class Thr_Supplier_Proxy : public Supplier_Proxy
  // = TITLE
  //    Runs each Input Proxy_Handler in a separate thread.
{
public:
  Thr_Supplier_Proxy (Event_Forwarding_Discriminator *, 
			Proxy_Handler_Connector *, 
			ACE_Thread_Manager *,
			int socket_queue_size);

  virtual int open (void *);
  // Initialize the object and spawn a new thread.

  virtual int svc (void);
  // Transmit peer messages.
};
#endif /* ACE_HAS_THREADS */
#endif /* _THR_IO_HANDLER */
