/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    MEM_Stream.h
 *
 *  $Id$
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 */
//=============================================================================


#ifndef ACE_MEM_STREAM_H
#define ACE_MEM_STREAM_H
#include "ace/pre.h"

#include "ace/MEM_IO.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if (ACE_HAS_POSITION_INDEPENDENT_POINTERS == 1)

#include "ace/INET_Addr.h"

class ACE_MEM_Acceptor;
class ACE_MEM_Connector;

/**
 * @class ACE_MEM_Stream
 *
 * @brief Defines the methods in the <ACE_MEM_Stream> abstraction.
 *
 * This adds additional wrapper methods atop the <ACE_MEM_IO>
 * class.  Notice that ACE_MEM_Stream can only send messages no bigger
 * than the underlying MMAP file size minus sizeof (ACE_MEM_SAP_Node).
 * The size of the MMAP file is default to 4096.  If you need to
 * increase the size of allowable message ACE_MEM_Stream can handle,
 * you can set it to
 * @c ACE_MEM_Acceptor::malloc_options(). minimal_bytes_
 * before accepting a connection.
 */
class ACE_Export ACE_MEM_Stream : public ACE_MEM_IO
{
public:

  friend class ACE_MEM_Acceptor;
  friend class ACE_MEM_Connector;

  // Initialization and termination methods.
  /// Constructor.
  ACE_MEM_Stream (void);

  /// Constructor (sets the underlying <ACE_HANDLE> with <h>).
  ACE_MEM_Stream (ACE_HANDLE h);

  /// Destructor.
  ~ACE_MEM_Stream (void);

  //= The following two methods use write and read system calls.
  /// Send n bytes, keep trying until n are sent.
  /// Recv n bytes, keep trying until n are received.
  ssize_t send_n (const void *buf, int n);
  ssize_t recv_n (void *buf, int n);

  // = The following two methods use the send and recv system calls.
  /// Send n bytes, keep trying until n are sent.
  /// Recv n bytes, keep trying until n are received.
  ssize_t send_n (const void *buf, int n, int flags);
  ssize_t recv_n (void *buf, int n, int flags);

#if 0
  /**
   * Try to send exactly <len> bytes into <buf> from <handle> (uses
   * the <send> call).  If <send> blocks for longer than <timeout> the
   * number of bytes actually sent is returned with <errno == ETIME>.
   * If a timeout does not occur, <send_n> return <len> (i.e., the
   * number of bytes requested to be sent).
   */
  ssize_t send_n (const void *buf,
                  size_t len,
                  int flags,
                  const ACE_Time_Value *timeout);

  /**
   * Try to recv exactly <len> bytes into <buf> from <handle> (uses
   * the <ACE::recv_n> call).  The <ACE_Time_Value> indicates how long
   * to blocking trying to receive.  If <timeout> == 0, the caller
   * will block until action is possible, else will wait until the
   * relative time specified in *<timeout> elapses).  If <recv> blocks
   * for longer than <timeout> the number of bytes actually read is
   * returned with <errno == ETIME>.  If a timeout does not occur,
   * <recv_n> return <len> (i.e., the number of bytes requested to be
   * read).
   */
  ssize_t recv_n (void *buf,
                  size_t len,
                  int flags,
                  const ACE_Time_Value *timeout);

  /**
   * Send an <iovec> of size <n> to the connected socket (uses
   * <ACE::sendv_n>).  Will block until all bytes are sent or an error
   * occurs.
   */
  ssize_t sendv_n (const iovec iov[],
                   size_t n) const;

  /// Receive an <iovec> of size <n> to the connected socket.
  ssize_t recvv_n (iovec iov[],
                   size_t n) const;
#endif /* 0 */

  // = Selectively close endpoints.

  /// Close down the reader.
  int close_reader (void);

  /// Close down the writer.
  int close_writer (void);

  /**
   * Close down the socket (we need this to make things work correctly
   * on Win32, which requires use to do a <close_writer> before doing
   * the close to avoid losing data).
   */
  int close (void);

  // = Meta-type info
  typedef ACE_Addr PEER_ADDR;

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;
};

#if !defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "ace/MEM_Stream.i"
#endif /* ACE_LACKS_INLINE_FUNCTIONS */

#endif /* ACE_HAS_POSITION_INDEPENDENT_POINTERS == 1 */

#include "ace/post.h"
#endif /* ACE_MEM_STREAM_H */
