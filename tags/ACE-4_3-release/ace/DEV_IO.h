/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    DEV_IO.h
//
// = AUTHOR
//    Gerhard Lenzer and Douglas C. Schmidt
// 
// ============================================================================

#if !defined (ACE_DEV_IO_H) 
#define ACE_DEV_IO_H

#include "ace/DEV.h"
#include "ace/DEV_Addr.h"

class ACE_Export ACE_DEV_IO : public ACE_DEV
  // = TITLE
  //     Read/Write operations on Devices.
{
friend class ACE_DEV_Connector;
public:
  ACE_DEV_IO (void);
  // Default constructor.

  // = Various send operations.
  ssize_t send (const void *buf, size_t n) const;
  // send upto <n> bytes in <buf>.

  ssize_t recv (void *buf, size_t n) const;
  // Recv upto <n> bytes in <buf>.

  ssize_t send_n (const void *buf, size_t n) const; 
  // Send n bytes, keep trying until n are sent. 

  ssize_t recv_n (void *buf, size_t n) const;	      
  // Recv n bytes, keep trying until n are received. 

#if defined (ACE_HAS_STREAM_PIPES)
  ssize_t recv (ACE_Str_Buf *cntl, 
		ACE_Str_Buf *data, 
		int *band, 
		int *flags) const;
  // Recv bytes via STREAM pipes using "band" mode.

  ssize_t send (const ACE_Str_Buf *cntl, 
		const ACE_Str_Buf *data, 
		int band, 
		int flags) const;
  // Send bytes via STREAM pipes using "band" mode.

  ssize_t recv (ACE_Str_Buf *cntl, 
		ACE_Str_Buf *data, 
		int *flags) const;
  // Recv <cntl> and <data> via STREAM pipes.

  ssize_t send (const ACE_Str_Buf *cntl, 
		const ACE_Str_Buf *data, 
		int flags = 0) const;
  // Send <cntl> and <data> via STREAM pipes.
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

  ssize_t send (const void *buf, size_t n, ACE_OVERLAPPED *overlapped) const;
  // Send <n> bytes via Win32 WriteFile using overlapped I/O.

  ssize_t recv (void *buf, size_t n, ACE_OVERLAPPED *overlapped) const;
  // Recv <n> bytes via Win32 ReadFile using overlapped I/O.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.
};

#include "ace/DEV_IO.i"

#endif /* ACE_DEV_IO_H */
