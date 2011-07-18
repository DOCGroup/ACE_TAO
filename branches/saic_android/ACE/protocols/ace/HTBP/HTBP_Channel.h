/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    HTBP_Channel.h
 *
 *  $Id$
 *
 *  @author Phil Mesnier, Priyanka Gontla
 */
//=============================================================================

#ifndef ACE_HTBP_CHANNEL_H
#define ACE_HTBP_CHANNEL_H
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
     * @class ACE::HTBP::Channel
     *
     * @brief Defines the methods in the <Ht_Channel> abstraction.
     *
     * This adds additional wrapper methods atop the <IO>
     * class.
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
    class HTBP_Export Channel
    {
    public:
      friend class Notifier;
      // Initialization and termination methods.
      /// Constructor.
      Channel (Session *s = 0);

      Channel (ACE_SOCK_Stream &s);
      Channel (ACE_HANDLE h);

      /// Destructor.
      ~Channel (void);

      enum State {
        Init,
        Ready,
        Data_Queued,
        Ack_Sent,
        Header_Sent,
        Closed,
        Send_Ack,
        Detached,
        Wait_For_Ack,
        Header_Pending
      };

      // = I/O functions.

      /// The Channel is a sibling of the ACE_SOCK_IO class, rather than a
      /// decendant. This is due to the requirement to wrap all messages with
      /// an HTTP request or reply wrapper, and to send application data in only
      /// one direction on one stream.

      /// Recv an <n> byte buffer from the connected socket.
      ssize_t recv (void *buf,
                    size_t n,
                    int flags,
                    const ACE_Time_Value *timeout = 0);

      /// Recv an <n> byte buffer from the connected socket.
      ssize_t recv (void *buf,
                    size_t n,
                    const ACE_Time_Value *timeout = 0);

      /// Recv an <iovec> of size <n> from the connected socket.
      ssize_t recvv (iovec iov[],
                     int n,
                     const ACE_Time_Value *timeout = 0);

      /// Same as above.  Deprecated.
      ssize_t recv (iovec iov[],
                    size_t n,
                    const ACE_Time_Value *timeout = 0);

      /**
       * Allows a client to read from a socket without having to provide a
       * buffer to read.  This method determines how much data is in the
       * socket, allocates a buffer of this size, reads in the data, and
       * returns the number of bytes read.  The caller is responsible for
       * deleting the member in the <iov_base> field of <io_vec> using
       * delete [] io_vec->iov_base.
       */
      ssize_t recvv (iovec *io_vec,
                     const ACE_Time_Value *timeout = 0);

      /// Send an <n> byte buffer to the connected socket.
      ssize_t send (const void *buf,
                    size_t n,
                    int flags,
                    const ACE_Time_Value *timeout = 0);

      /// Send an <n> byte buffer to the connected socket.
      ssize_t send (const void *buf,
                    size_t n,
                    const ACE_Time_Value *timeout = 0);

      /// Send an <iovec> of size <n> to the connected socket.
      ssize_t sendv (const iovec iov[],
                     int n,
                     const ACE_Time_Value *timeout = 0);

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
      typedef ACE_INET_Addr PEER_ADDR;

      /// Dump the state of an object.
      void dump (void) const;

      /// Declare the dynamic allocation hooks.
      ACE_ALLOC_HOOK_DECLARE;

    public:
      Session *session (void) const;
      void session (Session *);

      Notifier *notifier (void);

      void register_notifier (ACE_Reactor *r);

      ACE_HANDLE get_handle (void) const;

      const ACE_SOCK_Stream &ace_stream (void) const;
      ACE_SOCK_Stream &ace_stream (void);

      ///@notes Added the following methods to continue with
      /// current compilation of HTIOP. Might not be needed in
      /// future. - Priyanka
      /// {@
      void set_handle (ACE_HANDLE h);

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
      //@}

      // buffer related methods.
      ACE_Message_Block &leftovers (void);
      size_t data_len (void) const;
      void data_len (size_t n);
      size_t data_consumed (void) const;
      void data_consumed (size_t n);
      int load_buffer (void);
      int flush_buffer (void);
      int recv_ack (void);
      int send_ack (void);
      unsigned long request_count();
      Filter *filter (void) const;
      void filter (Filter *);
      State state (void) const;
      void state (Channel::State s);
      int pre_recv(void);
      int consume_error (void);

    private:

      /// The filter_ is a pluggable component used to manage the
      /// wrapping of data in a way suitable for the proxy to
      /// manage. The actual filter instance is owned by the
      /// session. Different filters may be required depending on
      /// which side of the proxy we are on, and the direction of data
      /// flow.
      Filter *filter_;

      /// The session_ is a reference to the persistent session this
      /// stream is associated with. On the inside, sessions are
      /// created by the Connector, which then create streams on
      /// demand.
      Session *session_;

      /// Since this stream implements its own send and recv methods,
      /// the filter cannot simply call the send and recv methods of
      /// this class. Therefore an ACE_SOCK_Stream wrapper around the
      /// same socket instance is necessary to avoid recursion.
      ACE_SOCK_Stream ace_stream_;

      Notifier *notifier_;

      ACE_Message_Block leftovers_;
      size_t data_len_;
      size_t data_consumed_;
      State state_;
      ACE_Message_Block *error_buffer_;
      unsigned long request_count_;
    };
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "HTBP_Channel.inl"
#endif

#include /**/ "ace/post.h"
#endif /* ACE_HTBP_CHANNEL_H */
