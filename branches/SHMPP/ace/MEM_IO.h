/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    MEM_IO.h
//
// = AUTHOR
//    Nanbor Wang
//
// ============================================================================

#ifndef ACE_MEM_IO_H
#define ACE_MEM_IO_H

#include "ace/SOCK.h"
#include "ace/MEM_SAP.h"
#include "ace/Memory_Pool.h"
#include "ace/Malloc_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ACE_Export ACE_MEM_IO : public ACE_SOCK, public ACE_MEM_SAP
{
  // = TITLE
  //     Defines the methods for the ACE shared memeory wrapper I/O routines
  //     (e.g., send/recv).
public:
  // = Initialization and termination methods.
  ACE_MEM_IO ();
  // Constructor.

  ~ACE_MEM_IO (void);
  // Destructor.

  ssize_t send (const void *buf,
                size_t n,
                int flags) ;
  // Send an <n> byte buffer to the other process using shm_malloc_
  // connected thru the socket.

  ssize_t recv (void *buf,
                size_t n,
                int flags) ;
  // Recv an <n> byte buffer from the shm_malloc_ thru connected socket.

  ssize_t send (const void *buf,
                size_t n) ;
  // Send an <n> byte buffer to the other process using shm_malloc_
  // connected thru the socket.

  ssize_t recv (void *buf,
                size_t n) ;
  // Recv an <n> byte buffer from the shm_malloc_ thru connected socket.

  ssize_t fetch_recv_buf (int flags, ACE_Time_Value *timeout = 0);
  // @@ Please fill in here.

  /*
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
  // deleting the member in the <iov_base> field of <io_vec> using
  // delete [] io_vec->iov_base.

  ssize_t recv (iovec *io_vec,
                const ACE_Time_Value *timeout = 0);
  // Same as above.  Deprecated.
  */

  ssize_t send (const void *buf,
                size_t n,
                int flags,
                const ACE_Time_Value *timeout);
  // Wait to to <timeout> amount of time to send up to <n> bytes into
  // <buf> from <handle> (uses the <send> call).  If <send> times out
  // a -1 is returned with <errno == ETIME>.  If it succeeds the
  // number of bytes sent is returned.

  ssize_t recv (void *buf,
                size_t n,
                int flags,
                const ACE_Time_Value *timeout);
  // Wait up to <timeout> amount of time to receive up to <n> bytes
  // into <buf> from <handle> (uses the <recv> call).  If <recv> times
  // out a -1 is returned with <errno == ETIME>.  If it succeeds the
  // number of bytes received is returned.

  ssize_t send (const void *buf,
                size_t n,
                const ACE_Time_Value *timeout);
  // Wait to to <timeout> amount of time to send up to <n> bytes into
  // <buf> from <handle> (uses the <send> call).  If <send> times out
  // a -1 is returned with <errno == ETIME>.  If it succeeds the
  // number of bytes sent is returned.

  ssize_t recv (void *buf,
                size_t n,
                const ACE_Time_Value *timeout);
  // Wait up to <timeout> amount of time to receive up to <n> bytes
  // into <buf> from <handle> (uses the <recv> call).  If <recv> times
  // out a -1 is returned with <errno == ETIME>.  If it succeeds the
  // number of bytes received is returned.

  /*
  ssize_t send (size_t n,
                ...) const;
  // Send <n> varargs messages to the connected socket.

  ssize_t recv (size_t n,
                ...) const;
  // Recv <n> varargs messages to the connected socket.

  ssize_t send (const void *buf,
                size_t n,
                ACE_OVERLAPPED *overlapped) const;
  // Send <n> bytes via Win32 <WriteFile> using overlapped I/O.

  ssize_t recv (void *buf,
                size_t n,
                ACE_OVERLAPPED *overlapped) const;
  // Recv <n> bytes via Win32 <ReadFile> using overlapped I/O.
  */

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  void *recv_buffer_;
  // Internal pointer for support recv/send.

  ssize_t buf_size_;
  // total buffer size.

  ssize_t cur_offset_;
  // current read pointer location.
};

#if !defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "ace/MEM_IO.i"
#endif

#endif /* ACE_SOCK_IO_H */
