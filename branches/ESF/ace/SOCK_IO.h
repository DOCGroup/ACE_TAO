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
  //
  // = NOTES
  //
  //     If <timeout> == 0, then the call behaves as a normal
  //     send/recv call, i.e., for blocking sockets, the call will
  //     block until action is possible; for non-blocking sockets,
  //     EWOULDBLOCK will be returned if no action is immediately
  //     possible.
  //
  //     If <timeout> != 0, the call will wait until the relative time
  //     specified in *<timeout> elapses.
  //
  //     Errors are reported by -1 and 0 return values.  If the
  //     operation times out, -1 is returned with <errno == ETIME>.
  //     If it succeeds the number of bytes transferred is returned.
  //
  //     Methods with the extra <flags> argument will always result in
  //     <send> getting called. Methods without the extra <flags>
  //     argument will result in <send> getting called on Win32
  //     platforms, and <write> getting called on non-Win32 platforms.
  //
public:
  // = Initialization and termination methods.

  ACE_SOCK_IO (void);
  // Constructor.

  ~ACE_SOCK_IO (void);
  // Destructor.

  ssize_t recv (void *buf,
                size_t n,
                int flags,
                const ACE_Time_Value *timeout = 0) const;
  // Recv an <n> byte buffer from the connected socket.

  ssize_t recv (void *buf,
                size_t n,
                const ACE_Time_Value *timeout = 0) const;
  // Recv an <n> byte buffer from the connected socket.

  ssize_t recvv (iovec iov[],
                 size_t n,
                 const ACE_Time_Value *timeout = 0) const;
  // Recv an <iovec> of size <n> from the connected socket.

  ssize_t recv (iovec iov[],
                size_t n,
                const ACE_Time_Value *timeout = 0) const;
  // Same as above.  Deprecated.

  ssize_t recvv (iovec *io_vec,
                 const ACE_Time_Value *timeout = 0) const;
  // Allows a client to read from a socket without having to provide a
  // buffer to read.  This method determines how much data is in the
  // socket, allocates a buffer of this size, reads in the data, and
  // returns the number of bytes read.  The caller is responsible for
  // deleting the member in the <iov_base> field of <io_vec> using
  // delete [] io_vec->iov_base.

  ssize_t recv (iovec *io_vec,
                const ACE_Time_Value *timeout = 0) const;
  // Same as above.  Deprecated.

  ssize_t recv (size_t n,
                ...) const;
  // Recv <n> varargs messages to the connected socket.

  ssize_t recv (void *buf,
                size_t n,
                ACE_OVERLAPPED *overlapped) const;
  // Recv <n> bytes via Win32 <ReadFile> using overlapped I/O.

  ssize_t send (const void *buf,
                size_t n,
                int flags,
                const ACE_Time_Value *timeout = 0) const;
  // Send an <n> byte buffer to the connected socket.

  ssize_t send (const void *buf,
                size_t n,
                const ACE_Time_Value *timeout = 0) const;
  // Send an <n> byte buffer to the connected socket.

  ssize_t sendv (const iovec iov[],
                 size_t n,
                 const ACE_Time_Value *timeout = 0) const;
  // Send an <iovec> of size <n> to the connected socket.

  ssize_t send (const iovec iov[],
                size_t n,
                const ACE_Time_Value *timeout = 0) const;
  // Same as above.  Deprecated.

  ssize_t send (size_t n,
                ...) const;
  // Send <n> varargs messages to the connected socket.

  ssize_t send (const void *buf,
                size_t n,
                ACE_OVERLAPPED *overlapped) const;
  // Send <n> bytes via Win32 <WriteFile> using overlapped I/O.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.
};

#if !defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "ace/SOCK_IO.i"
#endif

#endif /* ACE_SOCK_IO_H */
