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

#ifndef ACE_SOCK_STREAM_H
#define ACE_SOCK_STREAM_H

#include "ace/SOCK_IO.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/INET_Addr.h"

class ACE_Export ACE_SOCK_Stream : public ACE_SOCK_IO
{
  // = TITLE
  //     Defines the methods in the <ACE_SOCK_Stream> abstraction.
  //
  // = DESCRIPTION
  //     This adds additional wrapper methods atop the <ACE_SOCK_IO>
  //     class.
  //
  // = NOTES
  //
  //     The "_n" I/O methods keep looping until all the data has been
  //     transferred.  These methods also work for sockets in
  //     non-blocking mode i.e., they keep looping on EWOULDBLOCK.
  //     <timeout> is used to make sure we keep making progress, i.e.,
  //     the same timeout value is used for every I/O operation in the
  //     loop and the timeout is not counted down.  If the transfer
  //     times out, the number of bytes transferred so far are
  //     returned.
  //
  //     Errors are reported by -1 and 0 return values.
  //
  //     Methods with the extra <flags> argument will always result in
  //     <send> getting called. Methods without the extra <flags>
  //     argument will result in <send> getting called on Win32
  //     platforms, and <write> getting called on non-Win32 platforms.
  //
public:
  // Initialization and termination methods.
  ACE_SOCK_Stream (void);
  // Constructor.

  ACE_SOCK_Stream (ACE_HANDLE h);
  // Constructor (sets the underlying <ACE_HANDLE> with <h>).

  ~ACE_SOCK_Stream (void);
  // Destructor.

  // = I/O functions.

  ssize_t recv_n (void *buf,
                  size_t len,
                  int flags,
                  const ACE_Time_Value *timeout = 0) const;
  // Try to recv exactly <len> bytes into <buf> from <handle>.

  ssize_t recv_n (void *buf,
                  size_t len,
                  const ACE_Time_Value *timeout = 0) const;
  // Try to recv exactly <len> bytes into <buf> from <handle>.

  ssize_t recvv_n (iovec iov[],
                   size_t iovcnt,
                   const ACE_Time_Value *timeout = 0) const;
  // Receive an <iovec> of size <iovcnt> to the connected socket.

  ssize_t send_n (const void *buf,
                  size_t len,
                  int flags,
                  const ACE_Time_Value *timeout = 0) const;
  // Try to send exactly <len> bytes into <buf> from <handle>.

  ssize_t send_n (const void *buf,
                  size_t len,
                  const ACE_Time_Value *timeout = 0) const;
  // Try to send exactly <len> bytes into <buf> from <handle>.

  ssize_t sendv_n (const iovec iov[],
                   size_t iovcnt,
                   const ACE_Time_Value *timeout = 0) const;
  // Send an <iovec> of size <iovcnt> to the connected socket.

  // = Send/receive ``urgent'' data (see TCP specs...).
  ssize_t send_urg (const void *ptr,
                    size_t len = sizeof (char),
                    const ACE_Time_Value *timeout = 0) const;

  ssize_t recv_urg (void *ptr,
                    size_t len = sizeof (char),
                    const ACE_Time_Value *timeout = 0) const;

  // = Selectively close endpoints.
  int close_reader (void);
  // Close down the reader.
  int close_writer (void);
  // Close down the writer.

  int close (void);
  // Close down the socket (we need this to make things work correctly
  // on Win32, which requires use to do a <close_writer> before doing
  // the close to avoid losing data).

  // = Meta-type info
  typedef ACE_INET_Addr PEER_ADDR;

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.
};

#if !defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "ace/SOCK_Stream.i"
#endif

#endif /* ACE_SOCK_STREAM_H */
