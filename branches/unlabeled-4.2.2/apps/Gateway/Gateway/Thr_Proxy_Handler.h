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
  Thr_Consumer_Proxy (ACE_Event_Channel &, 
		      const ACE_INET_Addr &remote_addr,
		      const ACE_INET_Addr &local_addr,
		      ACE_INT32 conn_id);

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
  Thr_Supplier_Proxy (ACE_Event_Channel &, 
		      const ACE_INET_Addr &remote_addr,
		      const ACE_INET_Addr &local_addr,
		      ACE_INT32 conn_id);

  virtual int open (void *);
  // Initialize the object and spawn a new thread.

protected:
  virtual int svc (void);
  // Transmit peer messages.
};
#endif /* ACE_HAS_THREADS */
#endif /* _THR_IO_HANDLER */
