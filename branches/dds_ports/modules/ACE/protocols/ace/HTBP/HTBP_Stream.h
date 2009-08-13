// -*- C++ -*-

//=============================================================================
/**
 *  @file    HTBP_Stream.h
 *
 *  $Id$
 *
 *  @author Phil Mesnier, Priyanka Gontla
 */
//=============================================================================

#ifndef ACE_HTBP_STREAM_H
#define ACE_HTBP_STREAM_H
#include /**/ "ace/pre.h"

#include "ace/SOCK_Stream.h"
#include "ace/Message_Block.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "HTBP_Export.h"
#include "HTBP_Addr.h"
#include "HTBP_Filter.h"
#include "HTBP_Notifier.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace HTBP
  {
    // Forward declarations.
    class Session;

    /**
     * @class Stream
     *
     * @brief A stream facade for the HTTP Tunneling Bidirectional Protocol
     *
     * This adds provides the common interface for applications to do
     * I/O that ultimately is wrapped in HTTP for tunneling through
     * firewalls.
     *
     * This class is modelled after the ACE_SOCK_Stream class, and
     * provides all of the I/O methods available in that class. Since
     * this is a facade, it does not derive from ACE_SOCK_Stream. It
     * also does not provide the same performance as ACE_SOCK_Stream,
     * as some data may have to be stored by the HTBP classes prior to
     * transmission.
     *
     * <buf> is the buffer to write from or receive into.
     * <len> is the number of bytes to transfer.
     * The <timeout> parameter in the following methods indicates how
     * long to blocking trying to transfer data.  If <timeout> == 0,
     * then the call behaves as a normal send/recv call, i.e., for
     * blocking sockets, the call will block until action is possible;
     * for non-blocking sockets, EWOULDBLOCK will be returned if no
     * action is immediately possible.
     * If <timeout> != 0, the call will wait for data to arrive no longer
     * than the relative time specified in *<timeout>.
     * The "_n()" I/O methods keep looping until all the data has been
     * transferred.  These methods also work for sockets in non-blocking
     * mode i.e., they keep looping on EWOULDBLOCK.  <timeout> is used
     * to make sure we keep making progress, i.e., the same timeout
     * value is used for every I/O operation in the loop and the timeout
     * is not counted down.
     * The return values for the "*_n()" methods match the return values
     * from the non "_n()" methods and are specified as follows:
     * - On complete transfer, the number of bytes transferred is returned.
     * - On timeout, -1 is returned, errno == ETIME.
     * - On error, -1 is returned, errno is set to appropriate error.
     * - On EOF, 0 is returned, errno is irrelevant.
     *
     * On partial transfers, i.e., if any data is transferred before
     * timeout/error/EOF, <bytes_transferred> will contain the number of
     * bytes transferred.
     * Methods with <iovec> parameter are I/O vector variants of the I/O
     * operations.
     * Methods with the extra <flags> argument will always result in
     * <send> getting called. Methods without the extra <flags> argument
     * will result in <send> getting called on Win32 platforms, and
     * <write> getting called on non-Win32 platforms.
     */
    class HTBP_Export Stream
    {
    public:
      // Initialization and termination methods.
      /// Constructor.
      Stream (Session *s = 0);

      /// Destructor.
      ~Stream (void);

      // = I/O functions.

      /// The Stream is a sibling of the ACE_SOCK_IO class, rather
      /// than a decendant. This is due to the requirement to wrap all
      /// messages with an HTTP request or reply wrapper, and to send
      /// application data in only one direction on one stream.

      /// Recv an <n> byte buffer from the connected socket.
      ssize_t recv (void *buf,
                    size_t n,
                    int flags,
                    const ACE_Time_Value *timeout = 0) const;

      /// Recv an <n> byte buffer from the connected socket.
      ssize_t recv (void *buf,
                    size_t n,
                    const ACE_Time_Value *timeout = 0) const;

      /// Recv an <iovec> of size <n> from the connected socket.
      ssize_t recvv (iovec iov[],
                     int n,
                     const ACE_Time_Value *timeout = 0) const;

      /// Same as above.  Deprecated.
      ssize_t recv (iovec iov[],
                    size_t n,
                    const ACE_Time_Value *timeout = 0) const;

      /**
       * Allows a client to read from a socket without having to
       * provide a buffer to read.  This method determines how much
       * data is in the socket, allocates a buffer of this size, reads
       * in the data, and returns the number of bytes read.  The
       * caller is responsible for deleting the member in the
       * <iov_base> field of <io_vec> using delete []
       * io_vec->iov_base.
       */
      ssize_t recvv (iovec *io_vec,
                     const ACE_Time_Value *timeout = 0) const;

      /// Recv <n> bytes via Win32 <ReadFile> using overlapped I/O.
      ssize_t recv (void *buf,
                    size_t n,
                    ACE_OVERLAPPED *overlapped) const;

      /// Send an <n> byte buffer to the connected socket.
      ssize_t send (const void *buf,
                    size_t n,
                    int flags,
                    const ACE_Time_Value *timeout = 0) const;

      /// Send an <n> byte buffer to the connected socket.
      ssize_t send (const void *buf,
                    size_t n,
                    const ACE_Time_Value *timeout = 0) const;

      /// Send an <iovec> of size <n> to the connected socket.
      ssize_t sendv (const iovec iov[],
                     int n,
                     const ACE_Time_Value *timeout = 0) const;

      /// Send <n> bytes via Win32 <WriteFile> using overlapped I/O.
      ssize_t send (const void *buf,
                    size_t n,
                    ACE_OVERLAPPED *overlapped) const;


      /// Try to recv exactly <len> bytes into <buf> from the
      /// connected socket.
      ssize_t recv_n (void *buf,
                      size_t len,
                      int flags,
                      const ACE_Time_Value *timeout = 0,
                      size_t *bytes_transferred = 0) const;

      /// Try to recv exactly <len> bytes into <buf> from the
      /// connected socket.
      ssize_t recv_n (void *buf,
                      size_t len,
                      const ACE_Time_Value *timeout = 0,
                      size_t *bytes_transferred = 0) const;

      /// Receive an <iovec> of size <iovcnt> from the connected
      /// socket.
      ssize_t recvv_n (iovec iov[],
                       int iovcnt,
                       const ACE_Time_Value *timeout = 0,
                       size_t *bytes_transferred = 0) const;

      /// Try to send exactly <len> bytes from <buf> to the connection
      /// socket.
      ssize_t send_n (const void *buf,
                      size_t len,
                      int flags,
                      const ACE_Time_Value *timeout = 0,
                      size_t *bytes_transferred = 0) const;

      /// Try to send exactly <len> bytes from <buf> to the connected
      /// socket.
      ssize_t send_n (const void *buf,
                      size_t len,
                      const ACE_Time_Value *timeout = 0,
                      size_t *bytes_transferred = 0) const;

      /// Send all the <message_block>s chained through their <next>
      /// and <cont> pointers.  This call uses the underlying OS
      /// gather-write operation to reduce the domain-crossing
      /// penalty.
      ssize_t send_n (const ACE_Message_Block *message_block,
                      const ACE_Time_Value *timeout = 0,
                      size_t *bytes_transferred = 0) const;

      /// Send an <iovec> of size <iovcnt> to the connected socket.
      ssize_t sendv_n (const iovec iov[],
                       int iovcnt,
                       const ACE_Time_Value *timeout = 0,
                       size_t *bytes_transferred = 0) const;

      // = Selectively close endpoints.  / Close down the reader.
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
      typedef Addr PEER_ADDR;

      /// Dump the state of an object.
      void dump (void) const;

      /// Declare the dynamic allocation hooks.
      ACE_ALLOC_HOOK_DECLARE;

    public:
      ///@notes Added the following methods to continue with
      /// current compilation of HTIOP. Might not be needed in
      /// future. - Priyanka
      /// {@
      void set_handle (ACE_HANDLE h);
      ACE_HANDLE get_handle (void) const;

      Session *session (void) const;
      void session (Session *s);

      /**
       * Enable asynchronous I/O (ACE_SIGIO), urgent data (ACE_SIGURG),
       * non-blocking I/O (ACE_NONBLOCK), or close-on-exec (ACE_CLOEXEC),
       * which is passed as the <value>.
       */
      int enable (int value) const;

      /**
       * Disable asynchronous I/O (ACE_SIGIO), urgent data (ACE_SIGURG),
       * non-blocking I/O (ACE_NONBLOCK), or close-on-exec (ACE_CLOEXEC),
       * which is passed as the <value>.
       */
      int disable (int value) const;

      /**
       * Return the address of the remotely connected peer (if there is
       * one), in the referenced <ACE_Addr>. Returns 0 if successful, else
       * -1.
       */
      int get_remote_addr (Addr &) const;

      /**
       * Return the address of the remotely connected peer (if there is
       * one), in the referenced <ACE_Addr>. Returns 0 if successful, else
       * -1.
       */
      int get_local_addr (Addr &) const;

      //@}

    private:
      /// The session_ is a reference to the persistent session this stream is
      /// associated with. On the inside, sessions are created by the Connector,
      /// which then create streams on demand.
      Session *session_;

    };
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* ACE_HTBP_STREAM_H */
