/* -*- C++ -*- */
// $Id$


// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    SOCK_Stream.h
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (ACE_SOCK_STREAM_H)
#define ACE_SOCK_STREAM_H

#include "ace/SOCK_IO.h"
#include "ace/INET_Addr.h"

class ACE_Export ACE_SOCK_Stream : public ACE_SOCK_IO
  // = TITLE
  //     Defines the methods in the <ACE_SOCK_Stream> abstraction.
  //     
  // = DESCRIPTION
  //     This adds additional wrapper methods atop the <ACE_SOCK_IO> class.
{
public:
  //= The following two methods use write and read system calls.
  ssize_t send_n (const void *buf, int n) const;
  // Send n bytes, keep trying until n are sent.
  ssize_t recv_n (void *buf, int n) const;	      
  // Recv n bytes, keep trying until n are received.

  // = The following two methods use the send and recv system calls.
  ssize_t send_n (const void *buf, int n, int flags) const;
  // Send n bytes, keep trying until n are sent.
  ssize_t recv_n (void *buf, int n, int flags) const;	      
  // Recv n bytes, keep trying until n are received.

  ssize_t send_n (const void *buf, size_t len, int flags,
		  const ACE_Time_Value *timeout);
  // Try to send exactly <len> bytes into <buf> from <handle> (uses
  // the <send> call).  If <send> blocks for longer than <timeout> the
  // number of bytes actually sent is returned with <errno == ETIME>.
  // If a timeout does not occur, <send_n> return <len> (i.e., the
  // number of bytes requested to be sent).

  ssize_t recv_n (void *buf, size_t len, int flags, 
		  const ACE_Time_Value *timeout);
  // Wait up to <timeout> amount of time to receive up to <len> bytes
  // into <buf> from <handle> (uses the <recv> call).  If <recv> times
  // out a -1 is returned with <errno == ETIME>.  If it succeeds the
  // number of bytes received is returned.

  // = Send/receive an ``urgent'' character (see TCP specs...).
  ssize_t send_urg (void *ptr, int len = sizeof (char));
  ssize_t recv_urg (void *ptr, int len = sizeof (char));

  // = Selectively close endpoints.
  int close_reader (void);
  // Close down the reader.
  int close_writer (void);
  // Close down the writer.

  int close (void);
  // Close down the socket (we need this to make things work correctly
  // on Win32, which requires use to do a <close_writer> before doing
  // the close in order to avoid losing data).

  // = Meta-type info
  typedef ACE_INET_Addr PEER_ADDR;

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.
};

#include "ace/SOCK_Stream.i"

#endif /* ACE_SOCK_STREAM_H */
