/* -*- C++ -*- */
// @(#)Thr_Channel.h	1.1	10/18/96


// ============================================================================
//
// = LIBRARY
//    apps
// 
// = FILENAME
//    Thr_Channel.h
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (_THR_CHANNEL)
#define _THR_CHANNEL

#include "Channel.h"

#if defined (ACE_HAS_THREADS)
class Thr_Output_Channel : public Output_Channel
  // = TITLE
  //    Runs each Output Channel in a separate thread.
{
public:
  Thr_Output_Channel (ROUTING_TABLE *, 
		      Channel_Connector *, 
		      ACE_Thread_Manager *,
		      int socket_queue_size);

  virtual int open (void *);
  // Initialize the threaded Output_Channel object and spawn a new
  // thread.

  virtual int handle_input (ACE_HANDLE);
  // Called when Peer shutdown unexpectedly.

  virtual int put (ACE_Message_Block *, ACE_Time_Value * = 0);
  // Send a message to a peer.

  virtual int svc (void);
  // Transmit peer messages.
};

class Thr_Input_Channel : public Input_Channel
  // = TITLE
  //    Runs each Input Channel in a separate thread.
{
public:
  Thr_Input_Channel (ROUTING_TABLE *, 
		     Channel_Connector *, 
		     ACE_Thread_Manager *,
		     int socket_queue_size);

  virtual int open (void *);
  // Initialize the object and spawn a new thread.

  virtual int svc (void);
  // Transmit peer messages.
};
#endif /* ACE_HAS_THREADS */
#endif /* _THR_CHANNEL */
