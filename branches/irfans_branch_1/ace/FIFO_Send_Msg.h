/* -*- C++ -*- */
// $Id$


// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    FIFO_Send_Msg.h
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (ACE_FIFO_SEND_MSG_H)
#define ACE_FIFO_SEND_MSG_H

#include "ace/FIFO_Send.h"

class ACE_Export ACE_FIFO_Send_Msg : public ACE_FIFO_Send
  // = TITLE
  //     Sender side for the Record oriented C++ wrapper for UNIX
  //     FIFOs. 
{
public:
  // = Initialization methods.
  ACE_FIFO_Send_Msg (void);
  // Default constructor.

  ACE_FIFO_Send_Msg (const char *rendezvous, 
		     int flags = O_WRONLY, 
		     int perms = ACE_DEFAULT_FILE_PERMS,
                     LPSECURITY_ATTRIBUTES sa = 0);
  // Open up a record-oriented named pipe for writing.

  int open (const char *rendezvous, 
	    int flags = O_WRONLY, 
	    int perms = ACE_DEFAULT_FILE_PERMS,
            LPSECURITY_ATTRIBUTES sa = 0);
  // Open up a record-oriented named pipe for writing.

  ssize_t send (const ACE_Str_Buf &msg);
  // Send <buf> of up to <len> bytes.

  ssize_t send (const void *buf, size_t len);
  // Send <buf> of exactly <len> bytes (block until done).

#if defined (ACE_HAS_STREAM_PIPES)
  ssize_t send (const ACE_Str_Buf *data, 
		const ACE_Str_Buf *cntl = 0, 
		int flags = 0);
  // Send <data> and <cntl> message via Stream pipes.

  ssize_t send (int band, 
		const ACE_Str_Buf *data, 
		const ACE_Str_Buf *cntl = 0, 
		int flags = MSG_BAND);
  // Send <data> and <cntl> message via Stream pipes in "band" mode.
#endif /* ACE_HAS_STREAM_PIPES */

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.
};

#include "ace/FIFO_Send_Msg.i"

#endif /* ACE_FIFO_SEND_MSG_H */

