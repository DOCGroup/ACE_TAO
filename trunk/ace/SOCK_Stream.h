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
  //= The following two methods use write and read system calls,
  //= which are faster than the send and recv library functions
  //= used by the following two methods.
  ssize_t send_n (const void *buf, int n) const;
  // Send n bytes, keep trying until n are sent.
  ssize_t recv_n (void *buf, int n) const;	      
  // Recv n bytes, keep trying until n are received.

  // = The following two methods use the send and recv system
  // = calls.
  ssize_t send_n (const void *buf, int n, int flags) const;
  // Send n bytes, keep trying until n are sent.
  ssize_t recv_n (void *buf, int n, int flags) const;	      
  // Recv n bytes, keep trying until n are received.

  // = Send/receive an ``urgent'' character (see TCP specs...).
  ssize_t send_urg (void *ptr, int len = sizeof (char));
  ssize_t recv_urg (void *ptr, int len = sizeof (char));

  // = Selectively close endpoints.
  int close_reader (void);
  // Close down the reader.
  int close_writer (void);
  // Close down the writer.

  // = Meta-type info
  typedef ACE_INET_Addr PEER_ADDR;

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.
};

#include "ace/SOCK_Stream.i"

#endif /* ACE_SOCK_STREAM_H */
