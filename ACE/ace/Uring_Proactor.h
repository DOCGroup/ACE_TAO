// -*- C++ -*-

//=============================================================================
/**
 *  @file    Uring_Proactor.h
 *
 *  The implementation classes for the Linux io_uring Proactor are
 *  defined here in this file.
 */
//=============================================================================

#ifndef ACE_URING_PROACTOR_H
#define ACE_URING_PROACTOR_H

#include /**/ "ace/pre.h"

#include /**/ "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (ACE_HAS_AIO_CALLS) && defined (ACE_HAS_IO_URING)

#include "ace/Atomic_Op.h"
#include "ace/OS_NS_Thread.h"
#include "ace/Proactor_Impl.h"
#include "ace/Thread_Mutex.h"

// cppcheck-suppress missingIncludeSystem
#include <liburing.h>

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

class ACE_Uring_Asynch_Result;

/**
 * @class ACE_Uring_Proactor
 *
 * @brief Linux io_uring implementation of the Proactor.
 *
 * This class provides the factory methods and completion dispatch
 * support needed by the ACE asynchronous operation classes when the
 * backend is implemented on top of Linux io_uring.
 */
class ACE_Export ACE_Uring_Proactor : public ACE_Proactor_Impl
{
public:
  /// Initialize an io_uring-backed Proactor with the requested ring depth.
  ACE_Uring_Proactor (size_t entries = 256);

  /// Release the ring and any wakeup resources owned by the Proactor.
  virtual ~ACE_Uring_Proactor (void);

  /// Shut down the ring and stop further completion dispatch.
  virtual int close (void);

  /// Register a handle with the backend if the implementation needs it.
  virtual int register_handle (ACE_HANDLE handle,
                               const void *completion_key);

  /// Dispatch completions until one is handled or @a wait_time expires.
  virtual int handle_events (ACE_Time_Value &wait_time);

  /// Block until at least one completion is dispatched.
  virtual int handle_events (void);

  /// Wake any threads blocked in completion dispatch.
  virtual int wake_up_dispatch_threads (void);

  /// Request that dispatch threads exit and optionally wait for them.
  virtual int close_dispatch_threads (int wait);

  /// Return the number of threads currently dispatching completions.
  virtual size_t number_of_threads (void) const;

  /// Set the expected number of dispatch threads.
  virtual void number_of_threads (size_t threads);

  /// Return the handle used to wake blocked dispatch threads.
  virtual ACE_HANDLE get_handle (void) const;

  // Methods used to create Asynch I/O factory and result objects.

  /// Create the io_uring read-stream initiator.
  virtual ACE_Asynch_Read_Stream_Impl *create_asynch_read_stream (void);

  /// Create the io_uring write-stream initiator.
  virtual ACE_Asynch_Write_Stream_Impl *create_asynch_write_stream (void);

  /// Create the io_uring read-file initiator.
  virtual ACE_Asynch_Read_File_Impl *create_asynch_read_file (void);

  /// Create the io_uring write-file initiator.
  virtual ACE_Asynch_Write_File_Impl *create_asynch_write_file (void);

  /// Create the io_uring accept initiator.
  virtual ACE_Asynch_Accept_Impl *create_asynch_accept (void);

  /// Create the io_uring connect initiator.
  virtual ACE_Asynch_Connect_Impl *create_asynch_connect (void);

  /// Create the io_uring transmit-file initiator.
  virtual ACE_Asynch_Transmit_File_Impl *create_asynch_transmit_file (void);

  /// Create the io_uring datagram-read initiator.
  virtual ACE_Asynch_Read_Dgram_Impl *create_asynch_read_dgram (void);

  /// Create the io_uring datagram-write initiator.
  virtual ACE_Asynch_Write_Dgram_Impl *create_asynch_write_dgram (void);

  /// Create a read-stream result object for one pending operation.
  virtual ACE_Asynch_Read_Stream_Result_Impl *
    create_asynch_read_stream_result (const ACE_Handler::Proxy_Ptr &handler_proxy,
                                      ACE_HANDLE handle,
                                      ACE_Message_Block &message_block,
                                      size_t bytes_to_read,
                                      const void *act,
                                      ACE_HANDLE event = ACE_INVALID_HANDLE,
                                      int priority = 0,
                                      int signal_number = ACE_SIGRTMIN);

  /// Create a write-stream result object for one pending operation.
  virtual ACE_Asynch_Write_Stream_Result_Impl *
    create_asynch_write_stream_result (const ACE_Handler::Proxy_Ptr &handler_proxy,
                                       ACE_HANDLE handle,
                                       ACE_Message_Block &message_block,
                                       size_t bytes_to_write,
                                       const void *act,
                                       ACE_HANDLE event = ACE_INVALID_HANDLE,
                                       int priority = 0,
                                       int signal_number = ACE_SIGRTMIN);

  /// Create a read-file result object for one pending operation.
  virtual ACE_Asynch_Read_File_Result_Impl *
    create_asynch_read_file_result (const ACE_Handler::Proxy_Ptr &handler_proxy,
                                    ACE_HANDLE handle,
                                    ACE_Message_Block &message_block,
                                    size_t bytes_to_read,
                                    const void *act,
                                    u_long offset,
                                    u_long offset_high,
                                    ACE_HANDLE event = ACE_INVALID_HANDLE,
                                    int priority = 0,
                                    int signal_number = ACE_SIGRTMIN);

  /// Create a write-file result object for one pending operation.
  virtual ACE_Asynch_Write_File_Result_Impl *
    create_asynch_write_file_result (const ACE_Handler::Proxy_Ptr &handler_proxy,
                                     ACE_HANDLE handle,
                                     ACE_Message_Block &message_block,
                                     size_t bytes_to_write,
                                     const void *act,
                                     u_long offset,
                                     u_long offset_high,
                                     ACE_HANDLE event = ACE_INVALID_HANDLE,
                                     int priority = 0,
                                     int signal_number = ACE_SIGRTMIN);

  /// Create a datagram-read result object for one pending operation.
  virtual ACE_Asynch_Read_Dgram_Result_Impl *
    create_asynch_read_dgram_result (const ACE_Handler::Proxy_Ptr &handler_proxy,
                                     ACE_HANDLE handle,
                                     ACE_Message_Block *message_block,
                                     size_t bytes_to_read,
                                     int flags,
                                     int protocol_family,
                                     const void *act,
                                     ACE_HANDLE event = ACE_INVALID_HANDLE,
                                     int priority = 0,
                                     int signal_number = ACE_SIGRTMIN);

  /// Create a datagram-write result object for one pending operation.
  virtual ACE_Asynch_Write_Dgram_Result_Impl *
    create_asynch_write_dgram_result (const ACE_Handler::Proxy_Ptr &handler_proxy,
                                      ACE_HANDLE handle,
                                      ACE_Message_Block *message_block,
                                      size_t bytes_to_write,
                                      int flags,
                                      const void *act,
                                      ACE_HANDLE event = ACE_INVALID_HANDLE,
                                      int priority = 0,
                                      int signal_number = ACE_SIGRTMIN);

  /// Create an accept result object for one pending operation.
  virtual ACE_Asynch_Accept_Result_Impl *
    create_asynch_accept_result (const ACE_Handler::Proxy_Ptr &handler_proxy,
                                 ACE_HANDLE listen_handle,
                                 ACE_HANDLE accept_handle,
                                 ACE_Message_Block &message_block,
                                 size_t bytes_to_read,
                                 const void *act,
                                 ACE_HANDLE event = ACE_INVALID_HANDLE,
                                 int priority = 0,
                                 int signal_number = ACE_SIGRTMIN);

  /// Create a connect result object for one pending operation.
  virtual ACE_Asynch_Connect_Result_Impl *
    create_asynch_connect_result (const ACE_Handler::Proxy_Ptr &handler_proxy,
                                  ACE_HANDLE connect_handle,
                                  const void *act,
                                  ACE_HANDLE event = ACE_INVALID_HANDLE,
                                  int priority = 0,
                                  int signal_number = ACE_SIGRTMIN);

  /// Create a transmit-file result object for one pending operation.
  virtual ACE_Asynch_Transmit_File_Result_Impl *
    create_asynch_transmit_file_result
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
       ACE_HANDLE event = ACE_INVALID_HANDLE,
       int priority = 0,
       int signal_number = ACE_SIGRTMIN);

  /// Create a timer completion result object.
  virtual ACE_Asynch_Result_Impl *
    create_asynch_timer (const ACE_Handler::Proxy_Ptr &handler_proxy,
                         const void *act,
                         const ACE_Time_Value &tv,
                         ACE_HANDLE event = ACE_INVALID_HANDLE,
                         int priority = 0,
                         int signal_number = 0);

  /// Post internal wakeup completions into the ring.
  virtual int post_wakeup_completions (int how_many);

  /// Serialize SQE preparation and submission.
  ACE_Thread_Mutex &sq_mutex (void);

  /// Return non-zero once the ring has been initialized successfully.
  bool is_initialized (void) const;

  /// Access to the underlying ring for operation implementations.
  struct io_uring_sqe *get_sqe (void);

  /// Submit all prepared SQEs to the kernel.
  int submit_sqe (void);

  /// Submit prepared SQEs if the ring currently needs a submit call.
  int submit_sqe_if_necessary (void);

  /// Submit queued SQEs while preserving batching behavior.
  int submit_pending_sqe (void);

  /// Wake a non-dispatch thread so it can flush pending submissions.
  int signal_submitter (void);

protected:
  /// Drain and dispatch up to @a max_to_process CQEs.
  int process_cqes (int max_to_process,
                    const ACE_Time_Value *wait_time = 0);

private:
  friend class ACE_Uring_Asynch_Operation;

  enum
  {
    DEFAULT_CQE_BATCH_SIZE = 256,
    DEFAULT_SUBMIT_BATCH_SIZE = 8
  };

  /// Arm the internal wakeup eventfd while the SQ mutex is held.
  int arm_submit_wakeup_locked (void);

  /// Wake a non-dispatch thread while the SQ mutex is already held.
  int signal_submitter_locked (void);

  /// Drain pending submit wakeups while the SQ mutex is held.
  void drain_submit_wakeup_locked (void);

  /// Return non-zero when called from a dispatching thread.
  bool on_dispatch_thread (void) const;

  struct io_uring ring_;
  bool is_initialized_;
  bool submit_signal_pending_;
  ACE_HANDLE submit_wakeup_handle_;
  ACE_Atomic_Op<ACE_Thread_Mutex, ACE_thread_t> dispatch_thread_id_;
  /// Lock order: dispatch_mutex_ -> sq_mutex_ -> cq_mutex_. Operation-local
  /// pending_results_lock_ nests below sq_mutex_ when both are needed.
  mutable ACE_Thread_Mutex dispatch_mutex_;
  mutable ACE_Thread_Mutex sq_mutex_;
  mutable ACE_Thread_Mutex cq_mutex_;
};

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* ACE_HAS_AIO_CALLS && ACE_HAS_IO_URING */

#include /**/ "ace/post.h"
#endif /* ACE_URING_PROACTOR_H */
