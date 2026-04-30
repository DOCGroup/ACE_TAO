// -*- C++ -*-

//=============================================================================
/**
 *  @file    Uring_Asynch_IO.h
 *
 *  The implementation classes for the Linux io_uring asynchronous
 *  operations are defined here in this file.
 */
//=============================================================================

#ifndef ACE_URING_ASYNCH_IO_H
#define ACE_URING_ASYNCH_IO_H

#include /**/ "ace/pre.h"

#include /**/ "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (ACE_HAS_AIO_CALLS) && defined (ACE_HAS_IO_URING)

#include "ace/Asynch_IO_Impl.h"
#include "ace/Atomic_Op.h"
#include "ace/Guard_T.h"
#include "ace/Time_Value.h"
#include "ace/Unbounded_Set.h"
#include "ace/Uring_Proactor.h"

#include "ace/os_include/sys/os_socket.h"
#include "ace/os_include/sys/os_uio.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

class ACE_Uring_Asynch_Operation;

/**
 * @class ACE_Uring_Asynch_Result
 *
 * @brief Base class for io_uring completion tokens.
 *
 * Concrete asynchronous result types derive from this class and adapt
 * an io_uring completion back into the corresponding ACE result
 * callback.
 */
class ACE_Export ACE_Uring_Asynch_Result
  : public virtual ACE_Asynch_Result_Impl
{
public:
  /// Initialize the shared completion state for an io_uring result.
  ACE_Uring_Asynch_Result (const ACE_Handler::Proxy_Ptr &handler_proxy,
                           const void *act,
                           ACE_HANDLE handle,
                           u_long offset,
                           u_long offset_high,
                           ACE_Proactor *proactor);

  /// Release any resources retained by the result wrapper.
  virtual ~ACE_Uring_Asynch_Result (void);

  /// Dispatch the completed operation to the concrete ACE handler hook.
  virtual void complete (size_t bytes_transferred,
                         int success,
                         const void *completion_key,
                         u_long error = 0) = 0;

  /// Return the number of bytes reported by the completion.
  virtual size_t bytes_transferred (void) const;

  /// Store the number of bytes reported by the completion.
  virtual void set_bytes_transferred (size_t n);

  /// Return the translated completion error code.
  virtual u_long error (void) const;

  /// Store the translated completion error code.
  virtual void set_error (u_long err);

  /// Return the user-supplied asynchronous completion token.
  virtual const void *act (void) const;

  /// Return non-zero if the operation completed successfully.
  virtual int success (void) const;

  /// Return the completion key associated with the handler.
  virtual const void *completion_key (void) const;

  /// Return the event handle associated with the result, if any.
  virtual ACE_HANDLE event (void) const;

  /// Return the low 32 bits of the operation file offset.
  virtual u_long offset (void) const;

  /// Return the high 32 bits of the operation file offset.
  virtual u_long offset_high (void) const;

  /// Return the completion priority stored with the result.
  virtual int priority (void) const;

  /// Return the signal number stored with the result.
  virtual int signal_number (void) const;

  /// Requeue the completion through the Proactor.
  virtual int post_completion (ACE_Proactor_Impl *proactor);

  /// Return the original handler associated with the result.
  ACE_Handler *handler (void) const;

  /// Return the handler that should receive completion dispatch.
  ACE_Handler *dispatch_handler (void) const;

  /// Record the operation that currently owns this result.
  void owner (ACE_Uring_Asynch_Operation *operation);

  /// Return the operation that currently owns this result.
  ACE_Uring_Asynch_Operation *owner (void) const;

  /// Record the completion key associated with this result.
  void completion_key (const void *completion_key);

protected:
  ACE_Handler *handler_;
  ACE_Handler::Proxy_Ptr handler_proxy_;
  const void *act_;
  const void *completion_key_;
  ACE_HANDLE handle_;
  u_long offset_;
  u_long offset_high_;
  ACE_Proactor *proactor_;
  size_t bytes_transferred_;
  u_long error_;
  ACE_Atomic_Op<ACE_Thread_Mutex, ACE_Uring_Asynch_Operation *> owner_;
};

/**
 * @class ACE_Uring_Asynch_Timer
 *
 * @brief Result object used for timer completions posted into the ring.
 */
class ACE_Export ACE_Uring_Asynch_Timer : public ACE_Uring_Asynch_Result
{
public:
  /// Create a timer completion result for the specified timeout.
  ACE_Uring_Asynch_Timer (const ACE_Handler::Proxy_Ptr &handler_proxy,
                          const void *act,
                          const ACE_Time_Value &tv,
                          ACE_Proactor *proactor);

  /// Deliver the timer expiration callback.
  virtual void complete (size_t bytes_transferred,
                         int success,
                         const void *completion_key,
                         u_long error = 0);

private:
  ACE_Time_Value time_;
};

/**
 * @class ACE_Uring_Asynch_Operation
 *
 * @brief Base class for io_uring-backed asynchronous operations.
 */
class ACE_Export ACE_Uring_Asynch_Operation
  : public virtual ACE_Asynch_Operation_Impl
{
public:
  /// Associate the operation with its handler, handle, and Proactor.
  virtual int open (const ACE_Handler::Proxy_Ptr &handler_proxy,
                    ACE_HANDLE handle,
                    const void *completion_key,
                    ACE_Proactor *proactor);

  /// Cancel any results currently tracked by this operation.
  virtual int cancel (void);

  /// Return the owning Proactor.
  virtual ACE_Proactor *proactor (void) const;

  /// Return the handler currently bound to the operation.
  ACE_Handler *handler (void);

  /// Queue a completion for later submission to the Proactor.
  /// The caller must already hold the Proactor SQ mutex.
  int queue_result (ACE_Uring_Asynch_Result *result);

  /// Submit a completion immediately to the Proactor.
  /// The caller must already hold the Proactor SQ mutex.
  int submit_result (ACE_Uring_Asynch_Result *result);

  /// Add a result to the set of in-flight operations.
  void register_result (ACE_Uring_Asynch_Result *result);

  /// Remove a result from the set of in-flight operations.
  void unregister_result (ACE_Uring_Asynch_Result *result);

protected:
  /// Construct an operation helper bound to an io_uring Proactor.
  ACE_Uring_Asynch_Operation (ACE_Uring_Proactor *proactor);

  /// Destroy the operation helper after all results have been released.
  virtual ~ACE_Uring_Asynch_Operation (void);

  ACE_Uring_Proactor *uring_proactor_;
  ACE_Proactor *proactor_;
  ACE_Handler::Proxy_Ptr handler_proxy_;
  const void *completion_key_;
  ACE_HANDLE handle_;
  ACE_Thread_Mutex pending_results_lock_;
  ACE_Unbounded_Set<ACE_Uring_Asynch_Result *> pending_results_;
};

// ---------------------------------------------------------------------------
// Read stream and file
// ---------------------------------------------------------------------------

/**
 * @class ACE_Uring_Asynch_Read_Stream_Result
 *
 * @brief io_uring implementation of the read stream/file result.
 */
class ACE_Export ACE_Uring_Asynch_Read_Stream_Result
  : public ACE_Uring_Asynch_Result,
    public ACE_Asynch_Read_File_Result_Impl
{
public:
  /// Create a stream-read result wrapper for stream and file reads.
  ACE_Uring_Asynch_Read_Stream_Result (const ACE_Handler::Proxy_Ptr &handler_proxy,
                                       ACE_HANDLE handle,
                                       ACE_Message_Block *message_block,
                                       size_t bytes_to_read,
                                       const void *act,
                                       ACE_Proactor *proactor,
                                       u_long offset = 0,
                                       u_long offset_high = 0,
                                       bool vectored = false,
                                       struct iovec *iovec = 0);

  /// Release any temporary vectored-I/O state.
  virtual ~ACE_Uring_Asynch_Read_Stream_Result (void);

  /// Deliver the read completion callback.
  virtual void complete (size_t bytes_transferred,
                         int success,
                         const void *completion_key,
                         u_long error = 0);

  /// Return the requested transfer size.
  virtual size_t bytes_to_read (void) const;

  /// Return the message block used for the transfer.
  virtual ACE_Message_Block &message_block (void) const;

  /// Return the handle associated with the transfer.
  virtual ACE_HANDLE handle (void) const;

protected:
  ACE_Message_Block *message_block_;
  size_t bytes_to_read_;
  bool vectored_;
  struct iovec *iovec_;
};

/**
 * @class ACE_Uring_Asynch_Read_Stream
 *
 * @brief io_uring implementation of asynchronous stream reads.
 */
class ACE_Export ACE_Uring_Asynch_Read_Stream
  : public ACE_Uring_Asynch_Operation,
    public virtual ACE_Asynch_Read_Stream_Impl
{
public:
  /// Construct a stream-read initiator for the given Proactor.
  ACE_Uring_Asynch_Read_Stream (ACE_Uring_Proactor *proactor);

  /// Start a single-buffer asynchronous stream read.
  virtual int read (ACE_Message_Block &message_block,
                    size_t num_bytes_to_read,
                    const void *act,
                    int priority,
                    int signal_number);

  /// Start a vectored asynchronous stream read.
  virtual int readv (ACE_Message_Block &message_block,
                     size_t num_bytes_to_read,
                     const void *act,
                     int priority,
                     int signal_number);
};

/**
 * @class ACE_Uring_Asynch_Read_File
 *
 * @brief io_uring implementation of asynchronous file reads.
 */
class ACE_Export ACE_Uring_Asynch_Read_File
  : public ACE_Uring_Asynch_Read_Stream,
    public ACE_Asynch_Read_File_Impl
{
public:
  /// Construct a file-read initiator for the given Proactor.
  ACE_Uring_Asynch_Read_File (ACE_Uring_Proactor *proactor);

  /// Start a positioned file read.
  virtual int read (ACE_Message_Block &message_block,
                    size_t num_bytes_to_read,
                    u_long offset,
                    u_long offset_high,
                    const void *act,
                    int priority,
                    int signal_number);

  /// Start a sequential file read using the current file position.
  virtual int read (ACE_Message_Block &message_block,
                    size_t num_bytes_to_read,
                    const void *act,
                    int priority,
                    int signal_number);

  /// Start a positioned vectored file read.
  virtual int readv (ACE_Message_Block &message_block,
                     size_t num_bytes_to_read,
                     u_long offset,
                     u_long offset_high,
                     const void *act,
                     int priority,
                     int signal_number);

  /// Start a sequential vectored file read.
  virtual int readv (ACE_Message_Block &message_block,
                     size_t num_bytes_to_read,
                     const void *act,
                     int priority,
                     int signal_number);
};

/**
 * @class ACE_Uring_Asynch_Read_File_Result
 *
 * @brief io_uring implementation of the file-read result.
 */
class ACE_Export ACE_Uring_Asynch_Read_File_Result
  : public ACE_Uring_Asynch_Read_Stream_Result
{
public:
  /// Create a file-read result wrapper.
  ACE_Uring_Asynch_Read_File_Result (const ACE_Handler::Proxy_Ptr &handler_proxy,
                                     ACE_HANDLE handle,
                                     ACE_Message_Block *message_block,
                                     size_t bytes_to_read,
                                     const void *act,
                                     ACE_Proactor *proactor,
                                     u_long offset = 0,
                                     u_long offset_high = 0,
                                     bool vectored = false,
                                     struct iovec *iovec = 0);

  /// Deliver the file-read completion callback.
  virtual void complete (size_t bytes_transferred,
                         int success,
                         const void *completion_key,
                         u_long error = 0);
};

// ---------------------------------------------------------------------------
// Write stream and file
// ---------------------------------------------------------------------------

/**
 * @class ACE_Uring_Asynch_Write_Stream_Result
 *
 * @brief io_uring implementation of the write stream/file result.
 */
class ACE_Export ACE_Uring_Asynch_Write_Stream_Result
  : public ACE_Uring_Asynch_Result,
    public ACE_Asynch_Write_File_Result_Impl
{
public:
  /// Create a stream-write result wrapper for stream and file writes.
  ACE_Uring_Asynch_Write_Stream_Result
    (const ACE_Handler::Proxy_Ptr &handler_proxy,
     ACE_HANDLE handle,
     ACE_Message_Block *message_block,
     size_t bytes_to_write,
     const void *act,
     ACE_Proactor *proactor,
     u_long offset = 0,
     u_long offset_high = 0,
     bool vectored = false,
     struct iovec *iovec = 0);

  /// Release any temporary vectored-I/O state.
  virtual ~ACE_Uring_Asynch_Write_Stream_Result (void);

  /// Deliver the write completion callback.
  virtual void complete (size_t bytes_transferred,
                         int success,
                         const void *completion_key,
                         u_long error = 0);

  /// Return the requested transfer size.
  virtual size_t bytes_to_write (void) const;

  /// Return the message block used for the transfer.
  virtual ACE_Message_Block &message_block (void) const;

  /// Return the handle associated with the transfer.
  virtual ACE_HANDLE handle (void) const;

protected:
  ACE_Message_Block *message_block_;
  size_t bytes_to_write_;
  bool vectored_;
  struct iovec *iovec_;
};

/**
 * @class ACE_Uring_Asynch_Write_Stream
 *
 * @brief io_uring implementation of asynchronous stream writes.
 */
class ACE_Export ACE_Uring_Asynch_Write_Stream
  : public ACE_Uring_Asynch_Operation,
    public virtual ACE_Asynch_Write_Stream_Impl
{
public:
  /// Construct a stream-write initiator for the given Proactor.
  ACE_Uring_Asynch_Write_Stream (ACE_Uring_Proactor *proactor);

  /// Start a single-buffer asynchronous stream write.
  virtual int write (ACE_Message_Block &message_block,
                     size_t bytes_to_write,
                     const void *act,
                     int priority,
                     int signal_number);

  /// Start a vectored asynchronous stream write.
  virtual int writev (ACE_Message_Block &message_block,
                      size_t bytes_to_write,
                      const void *act,
                      int priority,
                      int signal_number);
};

/**
 * @class ACE_Uring_Asynch_Write_File
 *
 * @brief io_uring implementation of asynchronous file writes.
 */
class ACE_Export ACE_Uring_Asynch_Write_File
  : public ACE_Uring_Asynch_Write_Stream,
    public ACE_Asynch_Write_File_Impl
{
public:
  /// Construct a file-write initiator for the given Proactor.
  ACE_Uring_Asynch_Write_File (ACE_Uring_Proactor *proactor);

  /// Start a positioned file write.
  virtual int write (ACE_Message_Block &message_block,
                     size_t bytes_to_write,
                     u_long offset,
                     u_long offset_high,
                     const void *act,
                     int priority,
                     int signal_number);

  /// Start a sequential file write using the current file position.
  virtual int write (ACE_Message_Block &message_block,
                     size_t bytes_to_write,
                     const void *act,
                     int priority,
                     int signal_number);

  /// Start a positioned vectored file write.
  virtual int writev (ACE_Message_Block &message_block,
                      size_t bytes_to_write,
                      u_long offset,
                      u_long offset_high,
                      const void *act,
                      int priority,
                      int signal_number);

  /// Start a sequential vectored file write.
  virtual int writev (ACE_Message_Block &message_block,
                      size_t bytes_to_write,
                      const void *act,
                      int priority,
                      int signal_number);
};

/**
 * @class ACE_Uring_Asynch_Write_File_Result
 *
 * @brief io_uring implementation of the file-write result.
 */
class ACE_Export ACE_Uring_Asynch_Write_File_Result
  : public ACE_Uring_Asynch_Write_Stream_Result
{
public:
  /// Create a file-write result wrapper.
  ACE_Uring_Asynch_Write_File_Result
    (const ACE_Handler::Proxy_Ptr &handler_proxy,
     ACE_HANDLE handle,
     ACE_Message_Block *message_block,
     size_t bytes_to_write,
     const void *act,
     ACE_Proactor *proactor,
     u_long offset = 0,
     u_long offset_high = 0,
     bool vectored = false,
     struct iovec *iovec = 0);

  /// Deliver the file-write completion callback.
  virtual void complete (size_t bytes_transferred,
                         int success,
                         const void *completion_key,
                         u_long error = 0);
};

// ---------------------------------------------------------------------------
// Accept and connect
// ---------------------------------------------------------------------------

/**
 * @class ACE_Uring_Asynch_Accept_Result
 *
 * @brief io_uring implementation of the accept result.
 */
class ACE_Export ACE_Uring_Asynch_Accept_Result
  : public ACE_Uring_Asynch_Result,
    public ACE_Asynch_Accept_Result_Impl
{
public:
  /// Create an accept result wrapper for a pending accept operation.
  ACE_Uring_Asynch_Accept_Result (const ACE_Handler::Proxy_Ptr &handler_proxy,
                                  ACE_HANDLE listen_handle,
                                  ACE_HANDLE accept_handle,
                                  ACE_Message_Block *message_block,
                                  size_t bytes_to_read,
                                  const void *act,
                                  ACE_Proactor *proactor);

  /// Deliver the accept completion callback.
  virtual void complete (size_t bytes_transferred,
                         int success,
                         const void *completion_key,
                         u_long error = 0);

  /// Return the socket accepted by the operation.
  virtual ACE_HANDLE accept_handle (void) const;

  /// Return the message block used for the optional initial read.
  virtual ACE_Message_Block &message_block (void) const;

  /// Return the listening socket on which accept was started.
  virtual ACE_HANDLE listen_handle (void) const;

  /// Return the requested size of the initial read.
  virtual size_t bytes_to_read (void) const;

  /// Return the storage used for the peer address.
  struct sockaddr *addr (void);

  /// Return the length field used with the peer address storage.
  socklen_t *addrlen (void);

private:
  ACE_HANDLE accept_handle_;
  ACE_Message_Block *message_block_;
  size_t bytes_to_read_;
  struct sockaddr_storage client_addr_;
  socklen_t addr_len_;
};

/**
 * @class ACE_Uring_Asynch_Accept
 *
 * @brief io_uring implementation of asynchronous accepts.
 */
class ACE_Export ACE_Uring_Asynch_Accept
  : public ACE_Uring_Asynch_Operation,
    public ACE_Asynch_Accept_Impl
{
public:
  /// Construct an accept initiator for the given Proactor.
  ACE_Uring_Asynch_Accept (ACE_Uring_Proactor *proactor);

  /// Start an asynchronous accept operation.
  virtual int accept (ACE_Message_Block &message_block,
                      size_t bytes_to_read,
                      ACE_HANDLE accept_handle,
                      const void *act,
                      int priority,
                      int signal_number,
                      int addr_family);
};

/**
 * @class ACE_Uring_Asynch_Connect_Result
 *
 * @brief io_uring implementation of the connect result.
 */
class ACE_Export ACE_Uring_Asynch_Connect_Result
  : public ACE_Uring_Asynch_Result,
    public ACE_Asynch_Connect_Result_Impl
{
public:
  /// Create a connect result wrapper.
  ACE_Uring_Asynch_Connect_Result (const ACE_Handler::Proxy_Ptr &handler_proxy,
                                   ACE_HANDLE connect_handle,
                                   const void *act,
                                   ACE_Proactor *proactor);

  /// Deliver the connect completion callback.
  virtual void complete (size_t bytes_transferred,
                         int success,
                         const void *completion_key,
                         u_long error = 0);

  /// Return the socket associated with the connection attempt.
  virtual ACE_HANDLE connect_handle (void) const;

  /// Update the socket associated with the connection attempt.
  void connect_handle (ACE_HANDLE handle);

private:
  ACE_HANDLE connect_handle_;
};

/**
 * @class ACE_Uring_Asynch_Connect
 *
 * @brief io_uring implementation of asynchronous connects.
 */
class ACE_Export ACE_Uring_Asynch_Connect
  : public ACE_Uring_Asynch_Operation,
    public ACE_Asynch_Connect_Impl
{
public:
  /// Construct a connect initiator for the given Proactor.
  ACE_Uring_Asynch_Connect (ACE_Uring_Proactor *proactor);

  /// Bind the initiator to a handler, handle, and Proactor.
  virtual int open (const ACE_Handler::Proxy_Ptr &handler_proxy,
                    ACE_HANDLE handle,
                    const void *completion_key,
                    ACE_Proactor *proactor);

  /// Start an asynchronous connect operation.
  virtual int connect (ACE_HANDLE connect_handle,
                       const ACE_Addr &remote_sap,
                       const ACE_Addr &local_sap,
                       int reuse_addr,
                       const void *act,
                       int priority,
                       int signal_number);
};

// ---------------------------------------------------------------------------
// Datagram (UDP)
// ---------------------------------------------------------------------------

/**
 * @class ACE_Uring_Asynch_Read_Dgram_Result
 *
 * @brief io_uring implementation of the datagram read result.
 */
class ACE_Export ACE_Uring_Asynch_Read_Dgram_Result
  : public ACE_Uring_Asynch_Result,
    public ACE_Asynch_Read_Dgram_Result_Impl
{
public:
  /// Create a datagram-read result wrapper.
  ACE_Uring_Asynch_Read_Dgram_Result
    (const ACE_Handler::Proxy_Ptr &handler_proxy,
     ACE_HANDLE handle,
     ACE_Message_Block *message_block,
     size_t bytes_to_read,
     int flags,
     int protocol_family,
     const void *act,
     ACE_Proactor *proactor);

  /// Deliver the datagram-read completion callback.
  virtual void complete (size_t bytes_transferred,
                         int success,
                         const void *completion_key,
                         u_long error = 0);

  /// Return the message block used for the transfer.
  virtual ACE_Message_Block *message_block (void) const;

  /// Copy out the remote peer address captured for the datagram.
  virtual int remote_address (ACE_Addr &addr) const;

  /// Return the receive flags stored with the result.
  virtual int flags (void) const;

  /// Return the socket handle used for the transfer.
  virtual ACE_HANDLE handle (void) const;

  /// Return the requested size of the datagram read.
  virtual size_t bytes_to_read (void) const;

  /// Return the msghdr used to submit the receive request.
  struct msghdr *msg (void);

private:
  ACE_Message_Block *message_block_;
  size_t bytes_to_read_;
  int flags_;
  struct msghdr msg_;
  struct iovec iov_;
  struct sockaddr_storage remote_addr_;
};

/**
 * @class ACE_Uring_Asynch_Read_Dgram
 *
 * @brief io_uring implementation of asynchronous datagram receives.
 */
class ACE_Export ACE_Uring_Asynch_Read_Dgram
  : public ACE_Uring_Asynch_Operation,
    public ACE_Asynch_Read_Dgram_Impl
{
public:
  /// Construct a datagram-receive initiator for the given Proactor.
  ACE_Uring_Asynch_Read_Dgram (ACE_Uring_Proactor *proactor);

  /// Start an asynchronous datagram receive.
  virtual ssize_t recv (ACE_Message_Block *message_block,
                        size_t &number_of_bytes_recvd,
                        int flags,
                        int protocol_family,
                        const void *act,
                        int priority,
                        int signal_number);
};

/**
 * @class ACE_Uring_Asynch_Write_Dgram_Result
 *
 * @brief io_uring implementation of the datagram write result.
 */
class ACE_Export ACE_Uring_Asynch_Write_Dgram_Result
  : public ACE_Uring_Asynch_Result,
    public ACE_Asynch_Write_Dgram_Result_Impl
{
public:
  /// Create a datagram-write result wrapper.
  ACE_Uring_Asynch_Write_Dgram_Result
    (const ACE_Handler::Proxy_Ptr &handler_proxy,
     ACE_HANDLE handle,
     ACE_Message_Block *message_block,
     size_t bytes_to_write,
     int flags,
     const void *act,
     ACE_Proactor *proactor);

  /// Deliver the datagram-write completion callback.
  virtual void complete (size_t bytes_transferred,
                         int success,
                         const void *completion_key,
                         u_long error = 0);

  /// Return the message block used for the transfer.
  virtual ACE_Message_Block *message_block (void) const;

  /// Return the send flags stored with the result.
  virtual int flags (void) const;

  /// Return the socket handle used for the transfer.
  virtual ACE_HANDLE handle (void) const;

  /// Return the requested size of the datagram write.
  virtual size_t bytes_to_write (void) const;

  /// Return the msghdr used to submit the send request.
  struct msghdr *msg (void);

  /// Store the destination address for the datagram send.
  int remote_address (const ACE_Addr &addr);

private:
  ACE_Message_Block *message_block_;
  size_t bytes_to_write_;
  int flags_;
  struct msghdr msg_;
  struct iovec iov_;
  struct sockaddr_storage remote_addr_;
  socklen_t remote_addr_len_;
};

/**
 * @class ACE_Uring_Asynch_Write_Dgram
 *
 * @brief io_uring implementation of asynchronous datagram sends.
 */
class ACE_Export ACE_Uring_Asynch_Write_Dgram
  : public ACE_Uring_Asynch_Operation,
    public ACE_Asynch_Write_Dgram_Impl
{
public:
  /// Construct a datagram-send initiator for the given Proactor.
  ACE_Uring_Asynch_Write_Dgram (ACE_Uring_Proactor *proactor);

  /// Start an asynchronous datagram send.
  virtual ssize_t send (ACE_Message_Block *message_block,
                        size_t &number_of_bytes_sent,
                        int flags,
                        const ACE_Addr &remote_addr,
                        const void *act,
                        int priority,
                        int signal_number);
};

// ---------------------------------------------------------------------------
// Transmit file
// ---------------------------------------------------------------------------

/**
 * @class ACE_Uring_Asynch_Transmit_File_Result
 *
 * @brief io_uring implementation of the transmit file result.
 */
class ACE_Export ACE_Uring_Asynch_Transmit_File_Result
  : public ACE_Uring_Asynch_Result,
    public ACE_Asynch_Transmit_File_Result_Impl
{
public:
  /// Create a transmit-file result wrapper.
  ACE_Uring_Asynch_Transmit_File_Result
    (const ACE_Handler::Proxy_Ptr &handler_proxy,
     ACE_HANDLE socket,
     ACE_HANDLE file,
     ACE_Asynch_Transmit_File::Header_And_Trailer *header_and_trailer,
     size_t bytes_to_write,
     u_long offset,
     u_long offset_high,
     size_t bytes_per_send,
     u_long flags,
     const void *act,
     ACE_Proactor *proactor);

  /// Deliver the transmit-file completion callback.
  virtual void complete (size_t bytes_transferred,
                         int success,
                         const void *completion_key,
                         u_long error = 0);

  /// Return the socket receiving the transmitted data.
  virtual ACE_HANDLE socket (void) const;

  /// Return the file supplying the transmitted data.
  virtual ACE_HANDLE file (void) const;

  /// Return the optional header and trailer buffers.
  virtual ACE_Asynch_Transmit_File::Header_And_Trailer *
    header_and_trailer (void) const;

  /// Return the requested file byte count.
  virtual size_t bytes_to_write (void) const;

  /// Return the per-send throttling limit.
  virtual size_t bytes_per_send (void) const;

  /// Return the transmit-file flags.
  virtual u_long flags (void) const;

private:
  ACE_HANDLE file_;
  ACE_Asynch_Transmit_File::Header_And_Trailer *header_and_trailer_;
  size_t bytes_to_write_;
  size_t bytes_per_send_;
  u_long flags_;
};

/**
 * @class ACE_Uring_Asynch_Transmit_File
 *
 * @brief io_uring implementation of asynchronous transmit-file requests.
 */
class ACE_Export ACE_Uring_Asynch_Transmit_File
  : public ACE_Uring_Asynch_Operation,
    public ACE_Asynch_Transmit_File_Impl
{
public:
  /// Construct a transmit-file initiator for the given Proactor.
  ACE_Uring_Asynch_Transmit_File (ACE_Uring_Proactor *proactor);

  /// Start an asynchronous transmit-file request.
  virtual int transmit_file
    (ACE_HANDLE file,
     ACE_Asynch_Transmit_File::Header_And_Trailer *header_and_trailer,
     size_t bytes_to_write,
     u_long offset,
     u_long offset_high,
     size_t bytes_per_send,
     u_long flags,
     const void *act,
     int priority,
     int signal_number);
};

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* ACE_HAS_AIO_CALLS && ACE_HAS_IO_URING */

#include /**/ "ace/post.h"
#endif /* ACE_URING_ASYNCH_IO_H */
