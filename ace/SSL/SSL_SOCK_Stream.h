// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    SSL_SOCK_Stream.h
//
// = AUTHOR
//    John Heitmann
//    Carlos O'Ryan <coryan@cs.wustl.edu>
//    Ossama Othman <othman@cs.wustl.edu>
//
// ============================================================================

#ifndef ACE_SSL_SOCK_STREAM_H
#define ACE_SSL_SOCK_STREAM_H

#include "SSL.h"

#if defined (ACE_HAS_SSL)

#include "ace/SOCK_Stream.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ACE_Export ACE_SSL_SOCK_Stream : public ACE_SSL
{
  // = TITLE
  //    Defines methods in the <ACE_SSL_SOCK_Stream> abstraction.
  //
  // = DESCRIPTION
  //    This adds ssl functionality to an <ACE_SOCK_IO> interface by
  //    wrapping around an <ACE_SSL_SOCK_Stream> implementation.
  //
public:
  // = Initializtion and termination functions.

  ACE_SSL_SOCK_Stream (void);
  // Constructor (sets the underlying <ACE_HANDLE> with <h>, and
  // <SSL*> with <session>). If the handle in <session> does not
  // match <h>, it will set <session's> handle to <h>.

  ACE_SSL_SOCK_Stream (ACE_HANDLE h);
  // Constructor (sets <ACE_HANDLE> with the handle in <session>
  // and the underlying <SSL*> with session.

  ~ACE_SSL_SOCK_Stream (void);
  //Destructor

  ssize_t send (const void *buf,
                size_t n,
                int flags) const;
  // Send an <n> byte buffer to the ssl socket using
  // the semantics of <send(3n)>. ACE+SSL supports no
  // flags for sending at this time.

  ssize_t recv (void *buf,
                size_t n,
                int flags) const;
  // Recv an <n> byte buffer from the ssl socket using
  // the semantics of <recv(3n)>. ACE+SSL supports MSG_PEEK,
  // but no other flags at this time.


  ssize_t send (const void *buf,
                size_t n) const;
  // Send an <n> byte buffer to the ssl socket using
  // the semantics of <write(2)>.

  ssize_t recv (void *buf,
                size_t n) const;
  // Recv an <n> byte buffer from the ssl socket using
  // the semantics of <read(2)>.

  ssize_t sendv (const iovec iov[],
                 size_t n) const;
  // Send an <iovec> of size <n> to the ssl socket.


  ssize_t recvv (iovec *io_vec,
                 const ACE_Time_Value *timeout = 0) const;
  // Allows a client to read from a socket without having to provide a
  // buffer to read.  This method determines how much data is in the
  // socket, allocates a buffer of this size, reads in the data, and
  // returns the number of bytes read.  The caller is responsible for
  // deleting the member in the <iov_base> field of <io_vec> using
  // delete [] io_vec->iov_base.


  ssize_t send (const void *buf,
                size_t n,
                int flags,
                const ACE_Time_Value *timeout) const;
  // Wait to to <timeout> amount of time to send up to <n> bytes into
  // <buf> (uses the <send> call).  If <send> times out
  // a -1 is returned with <errno == ETIME>.  If it succeeds the
  // number of bytes sent is returned. No flags are supported.

  ssize_t recv (void *buf,
                size_t n,
                int flags,
                const ACE_Time_Value *timeout) const;
  // Wait up to <timeout> amount of time to receive up to <n> bytes
  // into <buf> (uses the <recv> call).  If <recv> times
  // out a -1 is returned with <errno == ETIME>.  If it succeeds the
  // number of bytes received is returned. MSG_PEEK is the only
  // supported flag.

  ssize_t send (const void *buf,
                size_t n,
                const ACE_Time_Value *timeout) const;
  // Wait to to <timeout> amount of time to send up to <n> bytes into
  // <buf> (uses the <send> call).  If <send> times out
  // a -1 is returned with <errno == ETIME>.  If it succeeds the
  // number of bytes sent is returned.

  ssize_t recv (void *buf,
                size_t n,
                const ACE_Time_Value *timeout) const;
  // Wait up to <timeout> amount of time to receive up to <n> bytes
  // into <buf> (uses the <recv> call).  If <recv> times
  // out a -1 is returned with <errno == ETIME>.  If it succeeds the
  // number of bytes received is returned.

  ssize_t send (size_t n,
                ...) const;
  // Send <n> varargs messages to the connected ssl socket.

  ssize_t recv (size_t n,
                ...) const;
  // Recv <n> varargs messages to the connected ssl socket.

  ssize_t send_n (const void *buf, int n) const;
  // Send <n> bytes, keep trying until <n> are sent.

  ssize_t recv_n (void *buf, int n) const;
  // Recv <n> bytes, keep trying until <n> are received.

  // = In the following four methods, only MSG_PEEK is supported
  // for recv_n, and no flags are supported for send_n.
  ssize_t send_n (const void *buf, int n, int flags) const;
  // Send <n> bytes, keep trying until <n> are sent.

  ssize_t recv_n (void *buf, int n, int flags) const;
  // Recv <n> bytes, keep trying until <n> are sent.

  ssize_t send_n (const void *buf,
                  size_t len,
                  int flags,
                  const ACE_Time_Value *timeout) const;
  // Try to send exactly <len> bytes into <buf> (uses
  // the <send> call).  If <send> blocks for longer than <timeout> the
  // number of bytes actually sent is returned with <errno == ETIME>.
  // If a timeout does not occur, <send_n> return <len> (i.e., the
  // number of bytes requested to be sent).

  ssize_t recv_n (void *buf,
                  size_t len,
                  int flags,
                  const ACE_Time_Value *timeout) const;
  // Try to recv exactly <len> bytes into <buf> (uses
  // the <recv> call).  The <ACE_Time_Value> indicates how long
  // to blocking trying to receive.  If <timeout> == 0, the caller
  // will block until action is possible, else will wait until the
  // relative time specified in *<timeout> elapses).  If <recv> blocks
  // for longer than <timeout> the number of bytes actually read is
  // returned with <errno == ETIME>.  If a timeout does not occur,
  // <recv_n> return <len> (i.e., the number of bytes requested to be
  // read).

  ssize_t sendv_n (const iovec iov[],
                   size_t n) const;
  // Send an <iovec> of size <n> to the connected socket.
  // Will block until all bytes are sent or an error
  // occurs.

  ssize_t recvv_n (iovec iov[],
                   size_t n) const;
  // Receive an <iovec> of size <n> to the connected socket.


  // = Selectively close endpoints.
  int close_reader (void);
  // Close down the reader.

  int close_writer (void);
  // Close down the writer.

  int close (void);
  //Close down the socket.

  // = Meta-type info
  typedef ACE_INET_Addr PEER_ADDR;

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

  int set_option (int level,
                  int option,
                  void *optval,
                  int optlen) const;
  // Wrapper around the setsockopt() system call.

  int get_option (int level,
                  int option,
                  void *optval,
                  int *optlen) const;
  // Wrapper around the getsockopt() system call.

  int control (int cmd, void *) const;
  // Interface for ioctl.

  // = Common I/O handle options related to sockets.

  int enable (int value) const;
  // Enable asynchronous I/O (ACE_SIGIO), urgent data (ACE_SIGURG),
  // non-blocking I/O (ACE_NONBLOCK), or close-on-exec (ACE_CLOEXEC),
  // which is passed as the <value>.

  int disable (int value) const;
  // Disable asynchronous I/O (ACE_SIGIO), urgent data (ACE_SIGURG),
  // non-blocking I/O (ACE_NONBLOCK), or close-on-exec (ACE_CLOEXEC),
  // which is passed as the <value>.

  int get_local_addr (ACE_Addr &) const;
  // Return the local endpoint address in the referenced <ACE_Addr>.

  int get_remote_addr (ACE_Addr &) const;
  // Return the address of the remotely connected peer (if there is
  // one), in the referenced ACE_Addr. Returns 0 if successful, else -1.

  ACE_HANDLE get_handle (void) const;
  // Get the underlying handle

  void set_handle (ACE_HANDLE handle);
  // Set the underlying handle

  friend class ACE_SSL_SOCK_Connector;
  friend class ACE_SSL_SOCK_Acceptor;

private:
  ACE_SOCK_Stream& peer (void);
  // Return the underlying <ACE_SOCK_Stream> which ssl runs on top of.

  ACE_SOCK_Stream stream_;
  // The stream which works under the ssl connection.
};

#if !defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "SSL_SOCK_Stream.i"
#endif /* ACE_LACKS_INLINE_FUNCTIONS */

#endif /* ACE_SSL_SOCK_STREAM_H */
#endif /* ACE_HAS_SSL */
