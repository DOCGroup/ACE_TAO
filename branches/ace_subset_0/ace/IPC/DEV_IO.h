/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    DEV_IO.h
 *
 *  $Id$
 *
 *  @author Gerhard Lenzer
 @  @author Douglas C. Schmidt
 */
//=============================================================================


#ifndef ACE_DEV_IO_H
#define ACE_DEV_IO_H
#include "ace/pre.h"

#include "ace/IPC/DEV.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/IPC/DEV_Addr.h"

/**
 * @class ACE_DEV_IO
 *
 * @brief Read/Write operations on Devices.
 */
class ACE_Export ACE_DEV_IO : public ACE_DEV
{
public:
  friend class ACE_DEV_Connector;

  /// Default constructor.
  ACE_DEV_IO (void);

  // = Various send operations.
  /// send upto <n> bytes in <buf>.
  ssize_t send (const void *buf, size_t n) const;

  /// Recv upto <n> bytes in <buf>.
  ssize_t recv (void *buf, size_t n) const;

  /// Send n bytes, keep trying until n are sent.
  ssize_t send_n (const void *buf, size_t n) const;

  /// Recv n bytes, keep trying until n are received.
  ssize_t recv_n (void *buf, size_t n) const;

#if defined (ACE_HAS_STREAM_PIPES)
  /// Recv bytes via STREAM pipes using "band" mode.
  ssize_t recv (ACE_Str_Buf *cntl,
                ACE_Str_Buf *data,
                int *band,
                int *flags) const;

  /// Send bytes via STREAM pipes using "band" mode.
  ssize_t send (const ACE_Str_Buf *cntl,
                const ACE_Str_Buf *data,
                int band,
                int flags) const;

  /// Recv <cntl> and <data> via STREAM pipes.
  ssize_t recv (ACE_Str_Buf *cntl,
                ACE_Str_Buf *data,
                int *flags) const;

  /// Send <cntl> and <data> via STREAM pipes.
  ssize_t send (const ACE_Str_Buf *cntl,
                const ACE_Str_Buf *data,
                int flags = 0) const;
#endif /* ACE_HAS_STREAM_PIPES */

  /// Send iovecs via <::writev>.
  ssize_t send (const iovec iov[], size_t n) const;

  /// Recv iovecs via <::readv>.
  ssize_t recv (iovec iov[], size_t n) const;

  /**
   * Send N char *ptrs and int lengths.  Note that the char *'s
   * precede the ints (basically, an varargs version of writev).  The
   * count N is the *total* number of trailing arguments, *not* a
   * couple of the number of tuple pairs!
   */
  ssize_t send (size_t n, ...) const;

  /**
   * This is an interface to ::readv, that doesn't use the struct
   * iovec explicitly.  The ... can be passed as an arbitrary number
   * of (char *ptr, int len) tuples.  However, the count N is the
   * *total* number of trailing arguments, *not* a couple of the
   * number of tuple pairs!
   */
  ssize_t recv (size_t n, ...) const;

  /// Send <n> bytes via Win32 WriteFile using overlapped I/O.
  ssize_t send (const void *buf, size_t n, ACE_OVERLAPPED *overlapped) const;

  /// Recv <n> bytes via Win32 ReadFile using overlapped I/O.
  ssize_t recv (void *buf, size_t n, ACE_OVERLAPPED *overlapped) const;

  /// Dump the state of an object.
  void dump (void) const;

  // = The following two methods are no-ops to keep the
  // <ACE_Connector> happy.
  /// Return the local endpoint address.
  int get_local_addr (ACE_DEV_Addr &) const;

  /// Return the address of the remotely connected peer (if there is
  /// one).
  int get_remote_addr (ACE_DEV_Addr &) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

  // = Meta-type info
  typedef ACE_DEV_Addr PEER_ADDR;

private:
  /// Address of device we are connected to.
  ACE_DEV_Addr addr_;
};

#if !defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "ace/IPC/DEV_IO.i"
#endif

#include "ace/post.h"
#endif /* ACE_DEV_IO_H */
