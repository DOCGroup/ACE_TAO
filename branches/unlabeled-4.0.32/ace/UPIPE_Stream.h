/* -*- C++ -*- */
// $Id$


// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    UPIPE_Stream.h
//
// = AUTHOR
//    Gerhard Lenzer and Douglas C. Schmidt
// 
// ============================================================================

#if !defined ACE_UPIPE_STREAM_H
#define ACE_UPIPE_STREAM_H

#include "ace/Stream.h"
#include "ace/Synch.h"
#include "ace/SPIPE.h"
#include "ace/Message_Queue.h"
#include "ace/UPIPE_Addr.h"

#if defined (ACE_HAS_THREADS)

// Use a typedef to make life easier later on.
typedef ACE_Stream<ACE_MT_SYNCH> MT_Stream;

class ACE_Export ACE_UPIPE_Stream : public ACE_SPIPE
  // = TITLE
  //     Defines the method that transfer data on a UPIPE.
{
  friend class ACE_UPIPE_Acceptor;
  friend class ACE_UPIPE_Connector;
public:
  // = Termination.

  int close (void);
  // Shut down the UPIPE and release resources.

  ACE_HANDLE get_handle (void) const;
  // Return the underlying I/O handle.

  // = Send/recv ACE Message_Blocks.
  int send (ACE_Message_Block *mb_p, 
	    ACE_Time_Value *timeout  = 0);
  // Send a message through the message queue.  Returns -1 on error,
  // else 0.

  int recv (ACE_Message_Block *&mb_p, 
	    ACE_Time_Value *timeout  = 0);
  // Recv a message from the message queue.  Returns -1 on error, else
  // 0.

  // = Send/recv char buffers.
  int send (const char *buffer, 
	    size_t n, 
	    ACE_Time_Value *timeout = 0);
  // Send a buffer of <n> bytes through the message queue.  Returns -1
  // on error, else number of bytes sent.

  int recv (char *buffer, 
	    size_t n, 
	    ACE_Time_Value *timeout = 0);
  // Recv a buffer of upto <n> bytes from the message queue.  Returns
  // -1 on error, else number of bytes read.

  int send_n (const char *buffer, 
	      size_t n,
	      ACE_Time_Value *timeout = 0);
  // Send a buffer of exactly <n> bytes to the message queue.  Returns
  // -1 on error, else number of bytes written (which should == n).

  int recv_n (char *buffer, 
	      size_t n, 
	      ACE_Time_Value *timeout = 0);
  // Recv a buffer of exactly <n> bytes from the message queue.
  // Returns -1 on error, else the number of bytes read.

  int control (ACE_IO_Cntl_Msg::ACE_IO_Cntl_Cmds cmd, 
	       void *val);
  // Perform control operations on the UPIPE_Stream.

  int get_remote_addr (ACE_UPIPE_Addr &remote_sap) const;
  // Return the remote address we are connected to.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  ACE_Message_Block *mb_last_;
  // To hold the last ACE_Message_Block read out of the stream. Thus
  // allowing subsequent reads from one ACE_Message_Block

  size_t remaining_;            
  // Holds the number of bytes that are still available in mb_last_.

  ACE_UPIPE_Addr remote_addr_;
  // Address of who we are connected to.

  MT_Stream stream_;
  // Stream component used by the <UPIPE_Acceptor> and
  // <UPIPE_Connector> to link together two UPIPE_Streams.
};

#if defined (__ACE_INLINE__) 
#include "ace/UPIPE_Stream.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_HAS_THREADS */
#endif /*ACE_UPIPE_STREAM_H */
