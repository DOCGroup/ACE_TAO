// -*- C++ -*-

//=============================================================================
/**
 *  @file    SSL_SOCK_Stream.h
 *
 *  $Id$
 *
 *  @author John Heitmann
 *  @author Carlos O'Ryan <coryan@uci.edu>
 *  @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================


#ifndef ACE_SSL_SOCK_STREAM_H
#define ACE_SSL_SOCK_STREAM_H

#include "ace/pre.h"

#include "SSL_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include <openssl/err.h>

#include "SSL_SOCK.h"
#include "SSL_Context.h"

#include "ace/Synch_T.h"
#include "ace/SOCK_Stream.h"


/**
 * @class ACE_SSL_SOCK_Stream
 *
 * @brief Defines methods in the ACE_SSL_SOCK_Stream abstraction.
 *
 * This adds SSL functionality to an ACE_SOCK_IO interface by
 * wrapping around an ACE_SSL_SOCK_Stream implementation.
 *
 * @note The user must currently ensure that assignment or copy
 * operations are atomic!
 */
class ACE_SSL_Export ACE_SSL_SOCK_Stream : public ACE_SSL_SOCK
{

  friend class ACE_SSL_SOCK_Connector;
  friend class ACE_SSL_SOCK_Acceptor;

public:

  /// Constructor
  ACE_SSL_SOCK_Stream (ACE_SSL_Context *context =
                         ACE_SSL_Context::instance ());

  /// Destructor
  ~ACE_SSL_SOCK_Stream (void);

  /**
   * Send an <n> byte buffer to the ssl socket using
   * the semantics of <send(3n)>. ACE+SSL supports no
   * flags for sending at this time.
   */
  ssize_t send (const void *buf,
                size_t n,
                int flags) const;

  /**
   * Recv an <n> byte buffer from the ssl socket using
   * the semantics of <recv(3n)>. ACE+SSL supports MSG_PEEK,
   * but no other flags at this time.
   */
  ssize_t recv (void *buf,
                size_t n,
                int flags) const;


  /// Send an <n> byte buffer to the ssl socket using
  /// the semantics of <write(2)>.
  ssize_t send (const void *buf,
                size_t n) const;

  /// Recv an <n> byte buffer from the ssl socket using
  /// the semantics of <read(2)>.
  ssize_t recv (void *buf,
                size_t n) const;

  /// Send an <iovec> of size <n> to the ssl socket.
  ssize_t sendv (const iovec iov[],
                 size_t n) const;

  /**
   * Allows a client to read from a socket without having to provide a
   * buffer to read.  This method determines how much data is in the
   * socket, allocates a buffer of this size, reads in the data, and
   * returns the number of bytes read.  The caller is responsible for
   * deleting the member in the <iov_base> field of <io_vec> using
   * delete [] io_vec->iov_base.
   */
  ssize_t recvv (iovec *io_vec,
                 const ACE_Time_Value *timeout = 0) const;

  /**
   * Wait to to <timeout> amount of time to send up to <n> bytes into
   * <buf> (uses the <send> call).  If <send> times out
   * a -1 is returned with <errno == ETIME>.  If it succeeds the
   * number of bytes sent is returned. No flags are supported.
   */
  ssize_t send (const void *buf,
                size_t n,
                int flags,
                const ACE_Time_Value *timeout) const;

  /**
   * Wait up to <timeout> amount of time to receive up to <n> bytes
   * into <buf> (uses the <recv> call).  If <recv> times
   * out a -1 is returned with <errno == ETIME>.  If it succeeds the
   * number of bytes received is returned. MSG_PEEK is the only
   * supported flag.
   */
  ssize_t recv (void *buf,
                size_t n,
                int flags,
                const ACE_Time_Value *timeout) const;

  /**
   * Wait to to <timeout> amount of time to send up to <n> bytes into
   * <buf> (uses the <send> call).  If <send> times out
   * a -1 is returned with <errno == ETIME>.  If it succeeds the
   * number of bytes sent is returned.
   */
  ssize_t send (const void *buf,
                size_t n,
                const ACE_Time_Value *timeout) const;

  /**
   * Wait up to <timeout> amount of time to receive up to <n> bytes
   * into <buf> (uses the <recv> call).  If <recv> times
   * out a -1 is returned with <errno == ETIME>.  If it succeeds the
   * number of bytes received is returned.
   */
  ssize_t recv (void *buf,
                size_t n,
                const ACE_Time_Value *timeout) const;

  /// Send <n> varargs messages to the connected ssl socket.
  ssize_t send (size_t n,
                ...) const;

  /// Recv <n> varargs messages to the connected ssl socket.
  ssize_t recv (size_t n,
                ...) const;

  /// Send <n> bytes, keep trying until <n> are sent.
  ssize_t send_n (const void *buf, int n) const;

  /// Recv <n> bytes, keep trying until <n> are received.
  ssize_t recv_n (void *buf, int n) const;

  /**
   * @note In the following four methods, only MSG_PEEK is supported
   * for recv_n, and no flags are supported for send_n.
   */
  //@{
  /// Send <n> bytes, keep trying until <n> are sent.
  ssize_t send_n (const void *buf, int n, int flags) const;

  /// Recv <n> bytes, keep trying until <n> are sent.
  ssize_t recv_n (void *buf, int n, int flags) const;

  /**
   * Try to send exactly <len> bytes into <buf> (uses
   * the <send> call).  If <send> blocks for longer than <timeout> the
   * number of bytes actually sent is returned with <errno == ETIME>.
   * If a timeout does not occur, <send_n> return <len> (i.e., the
   * number of bytes requested to be sent).
   */
  ssize_t send_n (const void *buf,
                  size_t len,
                  int flags,
                  const ACE_Time_Value *timeout) const;

  /**
   * Try to recv exactly <len> bytes into <buf> (uses
   * the <recv> call).  The <ACE_Time_Value> indicates how long
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
                  const ACE_Time_Value *timeout) const;
  //@}

  /**
   * Send an <iovec> of size <n> to the connected socket.
   * Will block until all bytes are sent or an error
   * occurs.
   */
  ssize_t sendv_n (const iovec iov[],
                   size_t n) const;

  /// Receive an <iovec> of size <n> to the connected socket.
  ssize_t recvv_n (iovec iov[],
                   size_t n) const;


  /**
   * Selectively close endpoints.
   */
  //@{
  /// Close down the reader.
  int close_reader (void);

  /// Close down the writer.
  int close_writer (void);
  //@}

  ///Close down the socket.
  int close (void);

  /// Meta-type info
  typedef ACE_INET_Addr PEER_ADDR;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

  /**
   * Overridden set_handle() method.
   *
   * Only an ACE_SSL_SOCK_Acceptor or ACE_SSL_SOCK_Connector should
   * access this method since some state in the underlying "ssl_" data
   * structure is set during SSL connection establishment.
   */
  void set_handle (ACE_HANDLE fd);

  /// Return a pointer to the underlying SSL structure.
  SSL *ssl (void) const;

protected:

  /// Return the underlying <ACE_SOCK_Stream> which SSL runs atop of.
  ACE_SOCK_Stream & peer (void);

  /// Underlying send() helper method common to all public send()
  /// methods.
  ssize_t send_i (const void *buf,
                  size_t n,
                  int flags) const;

  /// Underlying send() helper method common to all public send()
  /// methods.
  ssize_t recv_i (void *buf,
                  size_t n,
                  int flags) const;

private:

  ACE_UNIMPLEMENTED_FUNC (void operator= (const ACE_SSL_SOCK_Stream &))
  ACE_UNIMPLEMENTED_FUNC (ACE_SSL_SOCK_Stream (const ACE_SSL_SOCK_Stream &))

protected:

  /// The SSL session.
  SSL *ssl_;

  /// The stream which works under the ssl connection.
  ACE_SOCK_Stream stream_;

};

#if !defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "SSL_SOCK_Stream.i"
#endif /* ACE_LACKS_INLINE_FUNCTIONS */

#include "ace/post.h"

#endif /* ACE_SSL_SOCK_STREAM_H */

