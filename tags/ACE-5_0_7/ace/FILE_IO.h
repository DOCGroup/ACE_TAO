/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    FILE_IO.h
//
// = AUTHOR
//    Doug Schmidt
//
// ============================================================================

#ifndef ACE_FILE_IO_H
#define ACE_FILE_IO_H

#include "ace/FILE.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/FILE_Addr.h"

class ACE_Export ACE_FILE_IO : public ACE_FILE
{
  // = TITLE
  //     Read/Write operations on Files
public:
  friend class ACE_FILE_Connector;

  // = Initialization method.
  ACE_FILE_IO (void);
  // Default constructor.

  ssize_t send (const void *buf, size_t n) const;
  // send upto <n> bytes in <buf>.

  ssize_t recv (void *buf, size_t n) const;
  // Recv upto <n> bytes in <buf>.

  ssize_t send_n (const void *buf, size_t n) const;
  // Send n bytes, keep trying until n are sent.

  ssize_t recv_n (void *buf, size_t n) const;
  // Recv n bytes, keep trying until n are received.

#if defined (ACE_HAS_STREAM_PIPES)
  ssize_t send (const ACE_Str_Buf *cntl,
                const ACE_Str_Buf *data,
                int flags = 0) const;
  // Send bytes via STREAM pipes.

  ssize_t recv (ACE_Str_Buf *cntl,
                ACE_Str_Buf *data,
                int *flags) const;
  // Recv bytes via STREAM pipes.

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

#endif /* ACE_HAS_STREAM_PIPES */

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

  ssize_t send (const void *buf,
                size_t n,
                ACE_OVERLAPPED *overlapped) const;
  // Send <n> bytes via Win32 WriteFile using overlapped I/O.

  ssize_t recv (void *buf,
                size_t n,
                ACE_OVERLAPPED *overlapped) const;
  // Recv <n> bytes via Win32 ReadFile using overlapped I/O.

  ssize_t sendv (const iovec iov[],
                 size_t n) const;
  // Send an <iovec> of size <n> to the file.

  ssize_t recvv (iovec *io_vec);
  // Allows a client to read from a file without having to provide a
  // buffer to read.  This method determines how much data is in the
  // file, allocates a buffer of this size, reads in the data, and
  // returns the number of bytes read.  The caller is responsible for
  // deleting the member in the <iov_base> field of <io_vec> using
  // delete [] io_vec->iov_base.

  ssize_t sendv_n (const iovec iov[],
                   size_t n) const;
  // Send an <iovec> of size <n> to the file.  Will block until all
  // bytes are sent or an error occurs.

  ssize_t recvv_n (iovec iov[],
                   size_t n) const;
  // Receive an <iovec> of size <n> to the file.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

  // = Meta-type info
  typedef ACE_FILE_Addr PEER_ADDR;
};

#if !defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "ace/FILE_IO.i"
#endif

#endif /* ACE_FILE_IO_H */
