/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    POSIX_Proactor.h
 *
 *  $Id$
 *
 *  @author Irfan Pyarali <irfan@cs.wustl.edu>
 *  @author Tim Harrison <harrison@cs.wustl.edu>
 *  @author Alexander Babu Arulanthu <alex@cs.wustl.edu>
 *  @author Roger Tragin <r.tragin@computer.org>
 *  @author Alexander Libman <alibman@baltimore.com>
 */
//=============================================================================

#ifndef ACE_POSIX_PROACTOR_H
#define ACE_POSIX_PROACTOR_H

#include "ace/Demux/Proactor_Impl.h"

#if defined (ACE_HAS_AIO_CALLS)
// POSIX implementation of Proactor depends on the <aio_> family of
// system calls.

#include "ace/Utils/Templates/Free_List.h"
#include "ace/IPC/Pipe.h"
#include "ace/Connection/POSIX_Asynch_IO.h"
#include "ace/Utils/Unbounded_Queue.h"

#define ACE_AIO_MAX_SIZE     2048
#define ACE_AIO_DEFAULT_SIZE 1024

/**
 * @class ACE_POSIX_Proactor
 *
 * @brief POSIX implementation of the Proactor.
 *
 * There are two different strategies by which Proactor can get
 * to know the completion of <aio> operations. One is based on
 * Asynchronous I/O Control Blocks (AIOCB) where a list of
 * AIOCBs are stored and completion status of the corresponding
 * operations are queried on them. The other one is based on
 * POSIX Real Time signals. This class abstracts out the common
 * code needed for both the strategies. <ACE_AIOCB_Proactor> and
 * <ACE_SIG_Proactor> specialize this class for each strategy.
 */
class ACE_Export ACE_POSIX_Proactor : public ACE_Proactor_Impl
{
  /**
   * For <POSIX_SIG_Asynch_Accept> operation, this handler class does
   * the actual work, has to register the real-time signal with the
   * Proactor.
   */
  friend class ACE_POSIX_SIG_Asynch_Accept_Handler;

public:
  enum Proactor_Type
  {
    PROACTOR_POSIX = 0,  // base class type
    PROACTOR_AIOCB = 1,
    PROACTOR_SIG   = 2,
    PROACTOR_SUN   = 3
  };

  enum SystemType  // open for future extention
  {
    OS_UNDEFINED= 0x0000,
    OS_WIN      = 0x0100,          // for future
    OS_WIN_NT   = OS_WIN | 0x0001,
    OS_WIN_2000 = OS_WIN | 0x0002,
    OS_SUN      = 0x0200,          // Sun Solaris family
    OS_SUN_55   = OS_SUN | 0x0001,
    OS_SUN_56   = OS_SUN | 0x0002,
    OS_SUN_57   = OS_SUN | 0x0004,
    OS_SUN_58   = OS_SUN | 0x0008,
    OS_HPUX     = 0x0400,          // HPUX family
    OS_HPUX_11  = OS_HPUX | 0x0001,
    OS_LINUX    = 0x0800,          // Linux family
    OS_FREEBSD  = 0x1000           // FreeBSD family
  };

  virtual Proactor_Type  get_impl_type (void);

  /// Virtual destructor.
  virtual ~ACE_POSIX_Proactor (void);

  /// Close down the Proactor.
  virtual int close (void);

  /// This function is a no-op function for Unix systems. Returns 0.
  virtual int register_handle (ACE_HANDLE handle,
			       const void *completion_key);

  /**
   * Post a result to the completion port of the Proactor.  If errors
   * occur, the result will be deleted by this method.  If successful,
   * the result will be deleted by the Proactor when the result is
   * removed from the completion port. Therefore, the result should
   * have been dynamically allocated and should be orphaned by the
   * user once this method is called.
   */
  virtual int post_completion (ACE_POSIX_Asynch_Result *result) = 0;

  /// @@ This is a no-op on POSIX platforms. Returns 0.
  int wake_up_dispatch_threads (void);

  /// @@ This is a no-op on POSIX platforms. Returns 0.
  int close_dispatch_threads (int wait);

  /// @@ This is a no-op on POSIX platforms. Returns 0.
  size_t number_of_threads (void) const;
  void number_of_threads (size_t threads);

  /// This is a no-op in POSIX. Returns ACE_INVALID_HANDLE.
  virtual ACE_HANDLE get_handle (void) const;

  // Methods used to create Asynch_IO_Result objects. We create the right
  // objects here in these methods.

  virtual ACE_Asynch_Read_Stream_Result_Impl *create_asynch_read_stream_result (ACE_Handler &handler,
                                                                                ACE_HANDLE handle,
                                                                                ACE_Message_Block &message_block,
                                                                                u_long bytes_to_read,
                                                                                const void *act,
                                                                                ACE_HANDLE event = ACE_INVALID_HANDLE,
                                                                                int priority = 0,
                                                                                int signal_number = ACE_SIGRTMIN);
  
  virtual ACE_Asynch_Write_Stream_Result_Impl *create_asynch_write_stream_result (ACE_Handler &handler,
                                                                                  ACE_HANDLE handle,
                                                                                  ACE_Message_Block &message_block,
                                                                                  u_long bytes_to_write,
                                                                                  const void *act,
                                                                                  ACE_HANDLE event = ACE_INVALID_HANDLE,
                                                                                  int priority = 0,
                                                                                  int signal_number = ACE_SIGRTMIN);

  virtual ACE_Asynch_Read_File_Result_Impl *create_asynch_read_file_result (ACE_Handler &handler,
                                                                            ACE_HANDLE handle,
                                                                            ACE_Message_Block &message_block,
                                                                            u_long bytes_to_read,
                                                                            const void *act,
                                                                            u_long offset,
                                                                            u_long offset_high,
                                                                            ACE_HANDLE event = ACE_INVALID_HANDLE,
                                                                            int priority = 0,
                                                                            int signal_number = ACE_SIGRTMIN);

  virtual ACE_Asynch_Write_File_Result_Impl *create_asynch_write_file_result (ACE_Handler &handler,
                                                                              ACE_HANDLE handle,
                                                                              ACE_Message_Block &message_block,
                                                                              u_long bytes_to_write,
                                                                              const void *act,
                                                                              u_long offset,
                                                                              u_long offset_high,
                                                                              ACE_HANDLE event = ACE_INVALID_HANDLE,
                                                                              int priority = 0,
                                                                              int signal_number = ACE_SIGRTMIN);

  virtual ACE_Asynch_Read_Dgram_Result_Impl *create_asynch_read_dgram_result (ACE_Handler &handler,
                                                                              ACE_HANDLE handle,
                                                                              ACE_Message_Block *message_block,
                                                                              size_t bytes_to_read,
                                                                              int flags,
                                                                              int protocol_family,
                                                                              const void* act,
                                                                              ACE_HANDLE event = ACE_INVALID_HANDLE,
                                                                              int priority = 0,
                                                                              int signal_number = ACE_SIGRTMIN); 
	 
  virtual ACE_Asynch_Write_Dgram_Result_Impl *create_asynch_write_dgram_result (ACE_Handler &handler,
                                                                                ACE_HANDLE handle,
                                                                                ACE_Message_Block *message_block,
                                                                                size_t bytes_to_write,
                                                                                int flags,
                                                                                const void* act,
                                                                                ACE_HANDLE event = ACE_INVALID_HANDLE,
                                                                                int priority = 0,
                                                                                int signal_number = ACE_SIGRTMIN);
											 
  virtual ACE_Asynch_Accept_Result_Impl *create_asynch_accept_result (ACE_Handler &handler,
                                                                      ACE_HANDLE listen_handle,
                                                                      ACE_HANDLE accept_handle,
                                                                      ACE_Message_Block &message_block,
                                                                      u_long bytes_to_read,
                                                                      const void *act,
                                                                      ACE_HANDLE event = ACE_INVALID_HANDLE,
                                                                      int priority = 0,
                                                                      int signal_number = ACE_SIGRTMIN);
  
  virtual ACE_Asynch_Transmit_File_Result_Impl *create_asynch_transmit_file_result (ACE_Handler &handler,
                                                                                    ACE_HANDLE socket,
                                                                                    ACE_HANDLE file,
                                                                                    ACE_Asynch_Transmit_File::Header_And_Trailer *header_and_trailer,
                                                                                    u_long bytes_to_write,
                                                                                    u_long offset,
                                                                                    u_long offset_high,
                                                                                    u_long bytes_per_send,
                                                                                    u_long flags,
                                                                                    const void *act,
                                                                                    ACE_HANDLE event = ACE_INVALID_HANDLE,
                                                                                    int priority = 0,
                                                                                    int signal_number = ACE_SIGRTMIN);

  /// Create a timer result object which can be used with the Timer
  /// mechanism of the Proactor.
  virtual ACE_Asynch_Result_Impl *create_asynch_timer (ACE_Handler &handler,
                                                       const void *act,
                                                       const ACE_Time_Value &tv,
                                                       ACE_HANDLE event = ACE_INVALID_HANDLE,
                                                       int priority = 0,
                                                       int signal_number = ACE_SIGRTMIN);
protected:
  /// Constructor.
  ACE_POSIX_Proactor (void);

  /**
   * Protect against structured exceptions caused by user code when
   * dispatching handles. The <completion_key> is not very useful
   * compared to <AST> that can be associated each asynchronous
   * operation. <completion_key> is implemented right now for the
   * POSIX Proators.
   */
  void application_specific_code (ACE_POSIX_Asynch_Result *asynch_result,
				  u_long bytes_transferred,
				  int success,
				  const void *completion_key,
				  u_long error);

  /**
   * Post <how_many> completions to the completion port so that all
   * threads can wake up. This is used in conjunction with the
   * <run_event_loop>.
   */
  virtual int post_wakeup_completions (int how_many);

protected:
  /// Handler to handle the wakeups. This works in conjunction with the
  /// <ACE_Proactor::run_event_loop>.
  ACE_Handler wakeup_handler_;
  int os_id_ ;
};

// Forward declarations.
class ACE_AIOCB_Notify_Pipe_Manager;

/**
 * @class ACE_POSIX_AIOCB_Proactor
 *
 * @brief This Proactor makes use of Asynchronous I/O Control Blocks
 * (AIOCB) to notify/get the completion status of the <aio_>
 * operations issued.
 */
class ACE_Export ACE_POSIX_AIOCB_Proactor : public ACE_POSIX_Proactor
{

  /// Handler needs to call application specific code.
  friend class ACE_AIOCB_Notify_Pipe_Manager;

  /// This class does the registering of Asynch Operations with the
  /// Proactor which is necessary in the AIOCB strategy.
  friend class ACE_POSIX_AIOCB_Asynch_Operation;

  // friend class ACE_POSIX_AIOCB_Asynch_Accept_Handler; For
  // <Asynch_Accept> operation class, this helper class takes care of
  // doing the <Asynch_Accept>.

public:
  /// Constructor defines max number asynchronous operations
  /// which can be started at the same time
  ACE_POSIX_AIOCB_Proactor (size_t nmaxop = ACE_AIO_DEFAULT_SIZE);

  virtual Proactor_Type  get_impl_type (void);

  /// Destructor.
  virtual ~ACE_POSIX_AIOCB_Proactor (void);

  /**
   * Dispatch a single set of events.  If <wait_time> elapses before
   * any events occur, return 0.  Return 1 on success i.e., when a
   * completion is dispatched, non-zero (-1) on errors and errno is
   * set accordingly.
   */
  virtual int handle_events (ACE_Time_Value &wait_time);

  /**
   * Block indefinitely until at least one event is dispatched.
   * Dispatch a single set of events.  If <wait_time> elapses before
   * any events occur, return 0.  Return 1 on success i.e., when a
   * completion is dispatched, non-zero (-1) on errors and errno is
   * set accordingly.
   */
  virtual int handle_events (void);

  /// Post a result to the completion port of the Proactor.
  virtual int post_completion (ACE_POSIX_Asynch_Result *result);

  // = Methods used to create Asynch_IO objects. We create the right
  //   objects here in these methods.

  virtual ACE_Asynch_Read_Stream_Impl *create_asynch_read_stream (void);

  virtual ACE_Asynch_Write_Stream_Impl *create_asynch_write_stream (void);

  virtual ACE_Asynch_Read_Dgram_Impl *create_asynch_read_dgram (void);

  virtual ACE_Asynch_Write_Dgram_Impl *create_asynch_write_dgram (void);

  virtual ACE_Asynch_Read_File_Impl *create_asynch_read_file (void);

  virtual ACE_Asynch_Write_File_Impl *create_asynch_write_file (void);

  virtual ACE_Asynch_Accept_Impl *create_asynch_accept (void);

  virtual ACE_Asynch_Transmit_File_Impl *create_asynch_transmit_file (void);

  /**
  * This method should be called from  
  * ACE_POSIX_Asynch_Operation::cancel()
  * instead of usual ::aio_cancel.
  * For all deferred AIO requests with handle "h"
  * it removes its from the lists and notifies user.
  * For all running AIO requests with handle "h"
  * it calls ::aio_cancel. According to the POSIX standards
  * we will receive ECANCELED  for all ::aio_canceled AIO requests
  * later on return from ::aio_suspend
  */
  virtual int cancel_aio (ACE_HANDLE h);

protected:

  /// Special constructor for ACE_SUN_Proactor 
  /// and ACE_POSIX_SIG_Proactor
  ACE_POSIX_AIOCB_Proactor (size_t nmaxop, 
                            ACE_POSIX_Proactor::Proactor_Type ptype);

  /// Call these methods from derived class when virtual table is
  /// built.
  void create_notify_manager (void);
  void delete_notify_manager (void);

  /// Define the maximum number of asynchronous I/O requests
  /// for the current OS
  void check_max_aio_num (void) ;
 
  /// To identify requests from Notify_Pipe_Manager
  void set_notify_handle (ACE_HANDLE h);

  /**
   * Dispatch a single set of events.  If <milli_seconds> elapses
   * before any events occur, return 0. Return 1 if a completion
   * dispatched. Return -1 on errors.
   */
  virtual int handle_events (u_long milli_seconds);

  /// We will call the base class's application_specific_code from
  /// here.
  void application_specific_code (ACE_POSIX_Asynch_Result *asynch_result,
				  u_long bytes_transferred,
				  int success,
				  const void *completion_key,
				  u_long error);

  virtual int register_and_start_aio (ACE_POSIX_Asynch_Result *result,
                                      int op);

  /// Op code now is saved in ACE_POSIX_Asynch_Result
  virtual int start_aio (ACE_POSIX_Asynch_Result *result);

  /// Start deferred AIO if necessary
  int start_deferred_aio();

  /// Cancel running or deferred AIO
  virtual int cancel_aiocb ( ACE_POSIX_Asynch_Result * result );

  /// Extract the results of aio.
  ACE_POSIX_Asynch_Result *find_completed_aio (int &error_status,
                                               int &return_status,
                                               size_t &index,
                                               size_t &count);

  /// Find free slot to store result and aiocb pointer
  virtual int allocate_aio_slot (ACE_POSIX_Asynch_Result *result);


  /// Notify queue of "post_completed" ACE_POSIX_Asynch_Results
  /// called from post_completion method
  virtual int notify_completion ( int sig_num );

  /// Put "post_completed" result into the internal queue
  int  putq_result (ACE_POSIX_Asynch_Result *result);

  /// Get "post_completed" result from the internal queue
  ACE_POSIX_Asynch_Result * getq_result (void);

  /// Clear the internal results queue
  int clear_result_queue (void);

  /// Process the internal results queue
  int process_result_queue (void);


  /// This class takes care of doing <accept> when we use
  /// AIO_CONTROL_BLOCKS strategy.
  ACE_AIOCB_Notify_Pipe_Manager *aiocb_notify_pipe_manager_;

  /// Use a dynamically allocated array to keep track of all the aio's
  /// issued currently.
  aiocb **aiocb_list_;
  ACE_POSIX_Asynch_Result **result_list_;

  /// To maintain the maximum size of the array (list).
  size_t aiocb_list_max_size_;

  /// To maintain the current size of the array (list).
  size_t aiocb_list_cur_size_;

  /// Mutex to protect work with lists.
  ACE_SYNCH_MUTEX mutex_;

  /// The purpose of this member is only to identify asynchronous request
  /// from NotifyManager. We will reserve for it always slot 0
  /// in the list of aiocb's to be sure that don't lose notifications. 
 ACE_HANDLE notify_pipe_read_handle_ ;
 
  /// number of ACE_POSIX_Asynch_Result's waiting for start
  /// i.e. deferred AIOs
  size_t num_deferred_aiocb_ ;

  /// Number active,i.e. running requests
  size_t num_started_aio_ ;

  /// Queue which keeps "post_completed" ACE_POSIX_Asynch_Result's
  ACE_Unbounded_Queue<ACE_POSIX_Asynch_Result *> result_queue_;
};

/**
 * @class ACE_POSIX_SIG_Proactor
 *
 * @brief This Proactor implementation does compeltion querying using
 * POSIX Real Time signals. <sigtimedwait>/<sigwaitinfo> call is
 * used to get the notify/get the completions.
 * The real-time signals that are going to be used with this
 * Proactor should be given apriori in the constructor, so that
 * those signals can be masked from asynchornous delivery.
 */
class ACE_Export ACE_POSIX_SIG_Proactor : public ACE_POSIX_AIOCB_Proactor
{

  /**
   * This class does the registering of Asynch Operations with the
   * Proactor which is necessary in the SIG strategy, because we need
   * to store the signal number.
   */
  friend class ACE_POSIX_SIG_Asynch_Operation;

public:
  /**
   * This constructor masks only the <ACE_SIGRTMIN>
   * real-time signal. Only this signal should be used to issue
   * asynchronous operations using this Proctor.
   */
  ACE_POSIX_SIG_Proactor (size_t nmaxop = ACE_AIO_DEFAULT_SIZE);

  virtual Proactor_Type  get_impl_type (void);

  /**
   * This constructor should be used to tell the Proactor to mask and
   * wait for the real-time signals specified in this set. Only these
   * signals should be used by the asynchronous operations when they
   * use this Proactor.
   */
  ACE_POSIX_SIG_Proactor (const sigset_t mask_set,
                          size_t nmaxop = ACE_AIO_DEFAULT_SIZE);

  /// Destructor.
  virtual ~ACE_POSIX_SIG_Proactor (void);

  /**
   * Dispatch a single set of events.  If <wait_time> elapses before
   * any events occur, return 0.  Return 1 on success i.e., when a
   * completion is dispatched, non-zero (-1) on errors and errno is
   * set accordingly.
   */
  virtual int handle_events (ACE_Time_Value &wait_time);

  /**
   * Block indefinitely until at least one event is dispatched.
   * Dispatch a single set of events.  If <wait_time> elapses before
   * any events occur, return 0.  Return 1 on success i.e., when a
   * completion is dispatched, non-zero (-1) on errors and errno is
   * set accordingly.
   */
  virtual int handle_events (void);

  /// Post a result to the completion port of the Proactor.
  /// now it is implemented in base ACE_POSIX_AIOCB_Proactor class
  ///virtual int post_completion (ACE_POSIX_Asynch_Result *result);

  /**
   * If <signal_number> is -1, check with the Proactor and use one of
   * the signals that is present in the mask set (i.e. the signals for
   * which the Proactor will be waiting) of the Proactor. If there are
   * more than one signal, the higher numbered signal will be chosen.
   */
  virtual ACE_Asynch_Result_Impl *create_asynch_timer (ACE_Handler &handler,
                                                       const void *act,
                                                       const ACE_Time_Value &tv,
                                                       ACE_HANDLE event = ACE_INVALID_HANDLE,
                                                       int priority = 0,
                                                       int signal_number = ACE_SIGRTMIN);

protected:
  /// To setup the handler for a real-time signbal.
  int setup_signal_handler (int signal_number) const;

  /// To mask all the specified signals in a thread.
  int mask_signals (const sigset_t *signals) const;

  /**
   * Dispatch a single set of events.  If <milli_seconds> elapses
   * before any events occur, return 0. Return 1 if a completion is
   * dispatched. Return -1 on errors.
   */
  virtual int handle_events (u_long milli_seconds);

  /**
   * These signals are used for completion notification by the
   * Proactor. The signals specified while issueing <Asynch
   * Operation>s are stored here in this set. These signals are masked
   * for a thread when it calls the Proactor::handle_events.
   */

  /// Find free slot to store result and aiocb pointer
  virtual int allocate_aio_slot (ACE_POSIX_Asynch_Result *result);


  /// Notify queue of "post_completed" ACE_POSIX_Asynch_Results
  /// called from post_completion method
  virtual int notify_completion ( int sig_num );

  sigset_t RT_completion_signals_;
};

/**
 * @class ACE_POSIX_Asynch_Timer
 *
 * @brief This class is posted to the completion port when a timer
 * expires. When the <complete method> of this object is
 * called, the <handler>'s <handle_timeout> method will be
 * called.
 */
class ACE_Export ACE_POSIX_Asynch_Timer : public ACE_POSIX_Asynch_Result
{

  /// The factory method for this class is with the POSIX_Proactor
  /// class.
  friend class ACE_POSIX_Proactor;
  friend class ACE_POSIX_SIG_Proactor;

protected:
  /// Constructor.
  ACE_POSIX_Asynch_Timer (ACE_Handler &handler,
                          const void *act,
                          const ACE_Time_Value &tv,
                          ACE_HANDLE event = ACE_INVALID_HANDLE,
                          int priority = 0,
                          int signal_number = ACE_SIGRTMIN);

  /// Destructor.
  virtual ~ACE_POSIX_Asynch_Timer (void) {}

  /// This method calls the <handler>'s handle_timeout method.
  virtual void complete (u_long bytes_transferred,
                         int success,
                         const void *completion_key,
                         u_long error = 0);

  /// Time value requested by caller
  ACE_Time_Value time_;
};

#if defined (__ACE_INLINE__)
#include "ace/Demux/POSIX_Proactor.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_HAS_AIO_CALLS */
#endif /* ACE_POSIX_PROACTOR_H */
