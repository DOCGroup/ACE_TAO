/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    SOCK_IO.h
//
// = AUTHOR
//    Doug Schmidt
//
// ============================================================================

#ifndef ACE_SOCK_IO_H
#define ACE_SOCK_IO_H

#include "ace/SOCK.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ACE_Export ACE_SOCK_IO : public ACE_SOCK
{
  // = TITLE
  //     Defines the methods for the ACE socket wrapper I/O routines
  //     (e.g., send/recv).
public:
  // = Initialization and termination methods.

  ACE_SOCK_IO (void);
  // Constructor.

  ~ACE_SOCK_IO (void);
  // Destructor.

  ssize_t send (const void *buf,
                size_t n,
                int flags) const;
  // Send an <n> byte buffer to the connected socket (uses
  // <send(3n)>).

  ssize_t recv (void *buf,
                size_t n,
                int flags) const;
  // Recv an <n> byte buffer from the connected socket (uses
  // <recv(3n)>).

  ssize_t send (const void *buf,
                size_t n) const;
  // Send an <n> byte buffer to the connected socket (uses
  // <write(2)>).

  ssize_t recv (void *buf,
                size_t n) const;
  // Recv an <n> byte buffer from the connected socket (uses
  // <read(2)>).

  ssize_t sendv (const iovec iov[],
                 size_t n) const;
  // Send an <iovec> of size <n> to the connected socket.

  ssize_t send (const iovec iov[],
                size_t n) const;
  // Same as above.  Deprecated.

  ssize_t recvv (iovec *io_vec,
                 const ACE_Time_Value *timeout = 0);
  // Allows a client to read from a socket without having to provide a
  // buffer to read.  This method determines how much data is in the
  // socket, allocates a buffer of this size, reads in the data, and
  // returns the number of bytes read.  The caller is responsible for
  // deleting the member in the <iov_base> field of <io_vec>.

  ssize_t recv (iovec *io_vec,
                const ACE_Time_Value *timeout = 0);
  // Same as above.  Deprecated.

  ssize_t send (const void *buf,
                size_t n,
                int flags,
                const ACE_Time_Value *timeout) const;
  // Wait to to <timeout> amount of time to send up to <n> bytes into
  // <buf> from <handle> (uses the <send> call).  If <send> times out
  // a -1 is returned with <errno == ETIME>.  If it succeeds the
  // number of bytes sent is returned.

  ssize_t recv (void *buf,
                size_t n,
                int flags,
                const ACE_Time_Value *timeout) const;
  // Wait up to <timeout> amount of time to receive up to <n> bytes
  // into <buf> from <handle> (uses the <recv> call).  If <recv> times
  // out a -1 is returned with <errno == ETIME>.  If it succeeds the
  // number of bytes received is returned.

  ssize_t send (const void *buf,
                size_t n,
                const ACE_Time_Value *timeout) const;
  // Wait to to <timeout> amount of time to send up to <n> bytes into
  // <buf> from <handle> (uses the <send> call).  If <send> times out
  // a -1 is returned with <errno == ETIME>.  If it succeeds the
  // number of bytes sent is returned.

  ssize_t recv (void *buf,
                size_t n,
                const ACE_Time_Value *timeout) const;
  // Wait up to <timeout> amount of time to receive up to <n> bytes
  // into <buf> from <handle> (uses the <recv> call).  If <recv> times
  // out a -1 is returned with <errno == ETIME>.  If it succeeds the
  // number of bytes received is returned.

  ssize_t send (size_t n,
                ...) const;
  // Send <n> varargs messages to the connected socket.

  ssize_t recv (size_t n,
                ...) const;
  // Recv <n> varargs messages to the connected socket.

  ssize_t send (const void *buf,
                size_t n,
                ACE_OVERLAPPED *overlapped) const;
  // Send <n> bytes via Win32 WriteFile using overlapped I/O.

  ssize_t recv (void *buf,
                size_t n,
                ACE_OVERLAPPED *overlapped) const;
  // Recv <n> bytes via Win32 ReadFile using overlapped I/O.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.
};

#if !defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "ace/SOCK_IO.i"
#endif

#endif /* ACE_SOCK_IO_H */
