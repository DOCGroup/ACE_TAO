/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    SPIPE_Stream.h
//
// = AUTHOR
//    Doug Schmidt
//
// ============================================================================

#ifndef ACE_SPIPE_STREAM_H
#define ACE_SPIPE_STREAM_H

#include "ace/SPIPE.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/SPIPE_Addr.h"

class ACE_Export ACE_SPIPE_Stream : public ACE_SPIPE
{
  // = TITLE
  //     Define an ACE_SPIPE_Stream.
public:
  friend class ACE_SPIPE_Acceptor;
  friend class ACE_SPIPE_Connector;

  // = Initialization method.
  ACE_SPIPE_Stream (void);
  // Default constructor.

  int get_remote_addr (ACE_SPIPE_Addr &remote_sap) const;
  // Obtain the address of whom we are connected with.

  int send_handle (ACE_HANDLE handle) const;
  // Send an open FD to another process.

  int recv_handle (ACE_HANDLE &handle) const;
  // Recv an open FD from another process.

  int recv_handle (strrecvfd &recvfd) const;
  // Recv an open FD from another process.

  ssize_t send_n (const void *buf, size_t n) const;
  // Send n bytes, keep trying until n are sent.

  ssize_t recv_n (void *buf, size_t n) const;
  // Recv n bytes, keep trying until n are received.

  ssize_t send (const void *buf, size_t n) const;
  // Send bytes via STREAM pipes using "band" mode.

  ssize_t recv (void *buf, size_t n) const;
  // Recv bytes via STREAM pipes using "band" mode.

  ssize_t send (const ACE_Str_Buf *cntl,
                const ACE_Str_Buf *data,
                int flags = 0) const;
  // Send <cntl> and <data> via STREAM pipes.

  ssize_t recv (ACE_Str_Buf *cntl,
                ACE_Str_Buf *data,
                int *flags) const;
  // Recv <cntl> and <data> via STREAM pipes.

  ssize_t send (const ACE_Str_Buf *cntl,
                const ACE_Str_Buf *data,
                int band,
                int flags) const;
  // Send bytes via STREAM pipes using "band" mode.

  ssize_t recv (ACE_Str_Buf *cntl,
                ACE_Str_Buf *data,
                int *band,
                int *flags) const;
  // Recv bytes via STREAM pipes using "band" mode.

  ssize_t send (const iovec iov[], size_t n) const;
  // Send iovecs via <::writev>.

  ssize_t recv (iovec iov[], size_t n) const;
  // Recv iovecs via <::readv>.

  ssize_t send (size_t n, ...) const;
  // Send N char *ptrs and int lengths.  Note that the char *'s
  // precede the ints (basically, an varargs version of writev).  The
  // count N is the *total* number of trailing arguments, *not* a
  // couple of the number of tuple pairs!

  ssize_t recv (size_t n, ...) const;
  // This is an interface to ::readv, that doesn't use the struct
  // iovec explicitly.  The ... can be passed as an arbitrary number
  // of (char *ptr, int len) tuples.  However, the count N is the
  // *total* number of trailing arguments, *not* a couple of the
  // number of tuple pairs!

  ssize_t send (const void *buf, size_t n, ACE_OVERLAPPED *overlapped) const;
  // Send <n> bytes via Win32 WriteFile using overlapped I/O.

  ssize_t recv (void *buf, size_t n, ACE_OVERLAPPED *overlapped) const;
  // Recv <n> bytes via Win32 ReadFile using overlapped I/O.

  // = Meta-type info
  typedef ACE_SPIPE_Addr PEER_ADDR;

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  ACE_SPIPE_Addr remote_addr_;
};

#if !defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "ace/SPIPE_Stream.i"
#endif

#endif /* ACE_SPIPE_STREAM_H */
