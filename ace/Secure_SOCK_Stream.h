/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Secure_SOCK_Stream.h
//
// = AUTHOR
//    John Heitmann
//
// ============================================================================
#define ACE_HAS_OPENSSL
#if defined (ACE_HAS_OPENSSL)

#ifndef ACE_Secure_SOCK_STREAM_H
#define ACE_Secure_SOCK_STREAM_H
#include "ace/SOCK_Stream.h"
#include <openssl/ssl.h>

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ACE_Export ACE_Secure_SOCK_Stream
{
  // = TITLE
  //    Defines methods in the <ACE_Secure_SOCK_Stream> abstraction.
  //
  // = DESCRIPTION
  //    This adds ssl functionality to an <ACE_SOCK_IO> interface by
  //    wrapping around an <ACE_Secure_SOCK_Stream> implementation.
  //
public:
  // = Initializtion and termination functions.
  ACE_Secure_SOCK_Stream (void);
  // Constructor

  ACE_Secure_SOCK_Stream (ACE_HANDLE h);
  // Constructor (sets the underlying <ACE_HANDLE> with <H>)

  ~ACE_Secure_SOCK_Stream (void);
  //Destructor

  ssize_t send (const void *buf,
                size_t n,
                int flags) const;

  ssize_t recv (void *buf,
                size_t n,
                int flags) const;

  ssize_t send (const void *buf,
                size_t n) const;

  ssize_t recv (void *buf,
                size_t n) const;

  ssize_t sendv (const iovec iov[],
                 size_t n) const;

  ssize_t send (const iovec iov[],
                size_t n) const;

  ssize_t recvv (iovec *io_vec,
                 const ACE_Time_Value *timeout = 0) const;

  ssize_t recv (iovec *io_vec,
                const ACE_Time_Value *timeout = 0) const;

  ssize_t send (const void *buf,
                size_t n,
                int flags,
                const ACE_Time_Value *timeout) const;

  ssize_t recv (void *buf,
                size_t n,
                int flags,
                const ACE_Time_Value *timeout) const;

  ssize_t send (const void *buf,
                size_t n,
                const ACE_Time_Value *timeout) const;

  ssize_t recv (void *buf,
                size_t n,
                const ACE_Time_Value *timeout) const;

  ssize_t send (size_t n,
                ...) const;
  // Send <n> varargs messages to the connected socket.

  ssize_t recv (size_t n,
                ...) const;
  // Recv <n> varargs messages to the connected socket.

  // = The following methods all use <SSL_read (SSL*, char*)>
  //   and <SSL_write (SSL*, char*)>
  ssize_t send_n (const void *buf, int n) const;
  // Send n bytes, with blocking.

  ssize_t recv_n (void *buf, int n) const;
  // Recv n bytes, with blocking.

  ssize_t send_n (const void *buf, int n, int flags) const;
  // Send n bytes, with blocking.

  ssize_t recv_n (void *buf, int n, int flags) const;
  // Recv n bytes, with blocking.

  ssize_t send_n (const void *buf,
                  size_t len,
                  int flags,
                  const ACE_Time_Value *timeout) const;
  // Try to send n bytes,
  // jwh1: figure out timeout semantics.

  ssize_t recv_n (void *buf,
                  size_t len,
                  int flags,
                  const ACE_Time_Value *timeout) const;
  // Try to send n bytes.
  // jwh1: figure out timeout semantics.

  ssize_t sendv_n (const iovec iov[],
                   size_t n) const;
  // Send an <iovec> of size <n>, will block until finished.

  ssize_t recvv_n (iovec iov[],
                   size_t n) const;
  // Recv an <iovec> of size <n>, will block until finished.


  // = Selectively close endpoints.
  int close_reader (void);
  // Close down the reader.
  // jwh1: is this undefined with ssl?

  int close_writer (void);
  // Close down the writer.
  // jwh1: is this undefined with ssl?

  int close (void);
  //Close down the socket and jwh1: ? free the SSL session.

  // = Meta-type info
  typedef ACE_INET_Addr PEER_ADDR;

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

  // = Accessor methods
  //jwh1: Make me private.
  ACE_SOCK_Stream& peer ();
  // Return the underlying <ACE_SOCK_Stream> which ssl runs on top of.

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

  int get_local_addr (ACE_Addr &) const;
  // Return the local endpoint address in the referenced <ACE_Addr>.

  int get_remote_addr (ACE_Addr &) const;
  // Return the address of the remotely connected peer (if there is
  // one), in the referenced ACE_Addr. Returns 0 if successful, else -1.

  // = Accessors for the SSL* session
  void session (SSL*);
  SSL* session () const;

  ACE_HANDLE get_handle (void) const;
  // Get the underlying handle

  void set_handle (ACE_HANDLE handle);
  // Set the underlying handle
private:
  int enter_recv_timedwait (const ACE_Time_Value* timeout, int& val) const;
  void leave_recv_timedwait (const ACE_Time_Value* timeout, int val) const;
  int enter_send_timedwait (const ACE_Time_Value* timeout, int& val) const;
  void leave_send_timedwait (const ACE_Time_Value* timeout, int val) const;

  ACE_SOCK_Stream stream_;
  // The stream which works under the ssl connection.
  SSL* ssl_;
  // The ssl data structure itself.
};

#endif /* ACE_SECURE_SOCK_STREAM_H */
#endif /* defined (ACE_HAS_OPENSSL) */
