// -*- C++ -*-

//=============================================================================
/**
 *  @file    Message_Queue.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt <schmidt@cs.wustl.edu>
 */
//=============================================================================

#ifndef ACE_MESSAGE_QUEUE_H
#define ACE_MESSAGE_QUEUE_H
#include /**/ "ace/pre.h"

#include "ace/Message_Block.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/IO_Cntl_Msg.h"
#if defined (ACE_HAS_WIN32_OVERLAPPED_IO)
# include "ace/Thread_Mutex.h"   /* Needed in ACE_Message_Queue_NT */
#endif

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

// Forward decls.
class ACE_Notification_Strategy;
template <ACE_SYNCH_DECL> class ACE_Message_Queue_Iterator;
template <ACE_SYNCH_DECL> class ACE_Message_Queue_Reverse_Iterator;

/**
 * @class ACE_Message_Queue_Base
 *
 * @brief Base class for ACE_Message_Queue, which is the central
 * queueing facility for messages in the ACE framework.
 *
 * For all the <ACE_Time_Value> pointer parameters the caller will
 * block until action is possible if <timeout> == 0.  Otherwise, it
 * will wait until the absolute time specified in *<timeout>
 * elapses.
 *
 * A queue is always in one of three states:
 * . ACTIVATED
 * . DEACTIVATED
 * . PULSED
 */
class ACE_Export ACE_Message_Queue_Base
{
public:
  enum
  {
    // Default high and low watermarks.

    /// Default high watermark (16 K).
    DEFAULT_HWM = 16 * 1024,
    /// Default low watermark (same as high water mark).
    DEFAULT_LWM = 16 * 1024,

    // Queue states.  Before PULSED state was added, the activate()
    // and deactivate() methods returned WAS_INACTIVE or WAS_ACTIVE
    // to indicate the previous condition.  Now those methods
    // return the state the queue was previously in.  WAS_ACTIVE
    // and WAS_INACTIVE are defined to match previous semantics for
    // applications that don't use the PULSED state.

    /// @deprecated Use ACTIVATED instead.
    WAS_ACTIVE = 1,
    /// Message queue is active and processing normally
    ACTIVATED = 1,

    /// @deprecated Use DEACTIVATED instead.
    WAS_INACTIVE = 2,
    /// Queue is deactivated; no enqueue or dequeue operations allowed.
    DEACTIVATED = 2,

    /// Message queue was pulsed; enqueue and dequeue may proceed normally.
    PULSED = 3

  };

  ACE_Message_Queue_Base (void);

  /// Close down the message queue and release all resources.
  virtual int close (void) = 0;

  /// Close down the message queue and release all resources.
  virtual ~ACE_Message_Queue_Base (void);

  // = Enqueue and dequeue methods.

  /**
   * Retrieve the first ACE_Message_Block without removing it.  Note
   * that <timeout> uses <{absolute}> time rather than <{relative}>
   * time.  If the <timeout> elapses without receiving a message -1 is
   * returned and <errno> is set to <EWOULDBLOCK>.  If the queue is
   * deactivated -1 is returned and <errno> is set to <ESHUTDOWN>.
   * Otherwise, returns -1 on failure, else the number of items still
   * on the queue.
   */
  virtual int peek_dequeue_head (ACE_Message_Block *&first_item,
                                 ACE_Time_Value *timeout = 0) = 0;

  /**
   * Enqueue a <ACE_Message_Block *> into the tail of the queue.
   * Returns number of items in queue if the call succeeds or -1
   * otherwise.  These calls return -1 when queue is closed,
   * deactivated (in which case <errno> == <ESHUTDOWN>), when a signal
   * occurs (in which case <errno> == <EINTR>, or if the time
   * specified in timeout elapses (in which case <errno> ==
   * <EWOULDBLOCK>).
   */
  virtual int enqueue_tail (ACE_Message_Block *new_item,
                            ACE_Time_Value *timeout = 0) = 0;
  virtual int enqueue (ACE_Message_Block *new_item,
                       ACE_Time_Value *timeout = 0) = 0;

  /**
   * Dequeue and return the <ACE_Message_Block *> at the head of the
   * queue.  Returns number of items in queue if the call succeeds or
   * -1 otherwise.  These calls return -1 when queue is closed,
   * deactivated (in which case <errno> == <ESHUTDOWN>), when a signal
   * occurs (in which case <errno> == <EINTR>, or if the time
   * specified in timeout elapses (in which case <errno> ==
   * <EWOULDBLOCK>).
   */
  virtual int dequeue_head (ACE_Message_Block *&first_item,
                            ACE_Time_Value *timeout = 0) = 0;
  virtual int dequeue (ACE_Message_Block *&first_item,
                       ACE_Time_Value *timeout = 0) = 0;

  // = Check if queue is full/empty.
  /// True if queue is full, else false.
  virtual int is_full (void) = 0;

  /// True if queue is empty, else false.
  virtual int is_empty (void) = 0;

  // = Queue statistic methods.

  /// Number of total bytes on the queue, i.e., sum of the message
  /// block sizes.
  virtual size_t message_bytes (void) = 0;

  /// Number of total length on the queue, i.e., sum of the message
  /// block lengths.
  virtual size_t message_length (void) = 0;

  /// Number of total messages on the queue.
  virtual size_t message_count (void) = 0;

  /// New value of the number of total bytes on the queue, i.e.,
  /// sum of the message block sizes.
  virtual void message_bytes (size_t new_size) = 0;

  /// New value of the number of total length on the queue, i.e.,
  /// sum of the message block lengths.
  virtual void message_length (size_t new_length) = 0;

  // = Activation control methods.

  /**
   * Deactivate the queue and wake up all threads waiting on the queue
   * so they can continue.  No messages are removed from the queue,
   * however.  Any other operations called until the queue is
   * activated again will immediately return -1 with @c errno
   * ESHUTDOWN.
   *
   * @retval  The queue's state before this call.
   */
  virtual int deactivate (void) = 0;

  /**
   * Reactivate the queue so that threads can enqueue and dequeue
   * messages again.
   *
   * @retval  The queue's state before this call.
   */
  virtual int activate (void) = 0;

  /**
   * Pulse the queue to wake up any waiting threads.  Changes the
   * queue state to PULSED; future enqueue/dequeue operations proceed
   * as in ACTIVATED state.
   *
   * @retval  The queue's state before this call.
   */
  virtual int pulse (void) = 0;

  /// Returns the current state of the queue.
  virtual int state (void);

  /// Returns 1 if the state of the queue is DEACTIVATED,
  /// and 0 if the queue's state is ACTIVATED or PULSED.
  virtual int deactivated (void) = 0;

  /// Get the notification strategy for the <Message_Queue>
  virtual ACE_Notification_Strategy *notification_strategy (void) = 0;

  /// Set the notification strategy for the <Message_Queue>
  virtual void notification_strategy (ACE_Notification_Strategy *s) = 0;

  // = Notification hook.

  /// Dump the state of an object.
  virtual void dump (void) const = 0;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

private:
  // = Disallow copying and assignment.
  ACE_Message_Queue_Base (const ACE_Message_Queue_Base &);
  void operator= (const ACE_Message_Queue_Base &);

protected:
  /// Indicates the state of the queue, which can be
  /// <ACTIVATED>, <DEACTIVATED>, or <PULSED>.
  int state_;

};

ACE_END_VERSIONED_NAMESPACE_DECL

// Include the templates here.
#include "ace/Message_Queue_T.h"

#if defined (ACE_VXWORKS)
# include /**/ <msgQLib.h>
# include "ace/Null_Mutex.h"
# include "ace/Null_Condition.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class ACE_Message_Queue_Vx
 *
 * @brief Wrapper for VxWorks message queues.
 *
 * Specialization of ACE_Message_Queue to simply wrap VxWorks
 * MsgQ.  It does not use any synchronization, because it relies
 * on the native MsgQ implementation to take care of that.  The
 * only system calls that it uses are VxWorks msgQLib calls, so
 * it is suitable for use in interrupt service routines.
 * @note *Many* ACE_Message_Queue features are not supported with
 * this specialization, including:
 * * The two size arguments to the constructor and <open> are
 * interpreted differently.  The first is interpreted as the
 * maximum number of bytes in a message.  The second is
 * interpreted as the maximum number of messages that can be
 * queued.
 * * <dequeue_head> *requires* that the ACE_Message_Block
 * pointer argument point to an ACE_Message_Block that was
 * allocated by the caller.  It must be big enough to support
 * the received message, without using continuation. The
 * pointer argument is not modified.
 * * Message priority.  MSG_Q_FIFO is hard-coded.
 * * enqueue method timeouts.
 * * <peek_dequeue_head>.
 * * <ACE_Message_Queue_Iterators>.
 * * The ability to change low and high water marks after creation.
 * * <Message_Block> chains.  The continuation field of ACE_Message_Block
 * *   is ignored; only the first block of a fragment chain is
 * *   recognized.
 */
class ACE_Message_Queue_Vx : public ACE_Message_Queue<ACE_NULL_SYNCH>
{
public:
  // = Initialization and termination methods.
  ACE_Message_Queue_Vx (size_t max_messages,
                        size_t max_message_length,
                        ACE_Notification_Strategy * = 0);

  // Create a message queue with all the defaults.
  /// Create a message queue with all the defaults.
  virtual int open (size_t max_messages,
                    size_t max_message_length,
                    ACE_Notification_Strategy * = 0);

  /// Close down the message queue and release all resources.
  virtual int close (void);

  /// Close down the message queue and release all resources.
  virtual ~ACE_Message_Queue_Vx (void);

  // = Queue statistic methods.
  /**
   * Number of total bytes on the queue, i.e., sum of the message
   * block sizes.
   */
  virtual size_t message_bytes (void);

  /**
   * Number of total length on the queue, i.e., sum of the message
   * block lengths.
   */
  virtual size_t message_length (void);

  /**
   * Number of total messages on the queue.
   */
  virtual size_t message_count (void);

  // = Manual changes to these stats (used when queued message blocks
  // change size or lengths).
  /**
   * New value of the number of total bytes on the queue, i.e., sum of
   * the message block sizes.
   */
  virtual void message_bytes (size_t new_size);
  /**
   * New value of the number of total length on the queue, i.e., sum
   * of the message block lengths.
   */
  virtual void message_length (size_t new_length);

  // = Flow control routines

  /// Get high watermark.
  virtual size_t high_water_mark (void);

  /// Set high watermark.
  virtual void high_water_mark (size_t hwm);

  /// Get low watermark.
  virtual size_t low_water_mark (void);

  /// Set low watermark.
  virtual void low_water_mark (size_t lwm);

  // = Activation control methods.

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

protected:
  /// Enqueue an <ACE_Message_Block *> in accordance with its priority.
  virtual int enqueue_i (ACE_Message_Block *new_item);

  /// Enqueue an <ACE_Message_Block *> in accordance with its deadline time.
  virtual int enqueue_deadline_i (ACE_Message_Block *new_item);

  /// Enqueue an <ACE_Message_Block *> at the end of the queue.
  virtual int enqueue_tail_i (ACE_Message_Block *new_item);

  /// Enqueue an <ACE_Message_Block *> at the head of the queue.
  virtual int enqueue_head_i (ACE_Message_Block *new_item);

  /// Dequeue and return the <ACE_Message_Block *> at the head of the
  /// queue.
  virtual int dequeue_head_i (ACE_Message_Block *&first_item);

  /// Dequeue and return the <ACE_Message_Block *> with the lowest
  /// priority.
  virtual int dequeue_prio_i (ACE_Message_Block *&dequeued);

  /// Dequeue and return the <ACE_Message_Block *> at the tail of the
  /// queue.
  virtual int dequeue_tail_i (ACE_Message_Block *&dequeued);

  /// Dequeue and return the <ACE_Message_Block *> that has the lowest
  /// deadline time.
  virtual int dequeue_deadline_i (ACE_Message_Block *&dequeued);

  // = Check the boundary conditions (assumes locks are held).
  /// True if queue is full, else false.
  virtual int is_full_i (void);

  /// True if queue is empty, else false.
  virtual int is_empty_i (void);

  // = Implementation of public <activate>/<deactivate> methods above.

  // These methods assume locks are held.

  // = Helper methods to factor out common #ifdef code.
  /// Wait for the queue to become non-full.
  virtual int wait_not_full_cond (ACE_Guard<ACE_Null_Mutex> &mon,
                                  ACE_Time_Value *tv);

  /// Wait for the queue to become non-empty.
  virtual int wait_not_empty_cond (ACE_Guard<ACE_Null_Mutex> &mon,
                                   ACE_Time_Value *tv);

  /// Inform any threads waiting to enqueue that they can procede.
  virtual int signal_enqueue_waiters (void);

  /// Inform any threads waiting to dequeue that they can procede.
  virtual int signal_dequeue_waiters (void);

  /// Access the underlying msgQ.
  MSG_Q_ID msgq (void);

private:

  // Disallow copying and assignment.
  ACE_Message_Queue_Vx (const ACE_Message_Queue_Vx &);
  void operator= (const ACE_Message_Queue_Vx &);

  ACE_UNIMPLEMENTED_FUNC (virtual int peek_dequeue_head
                            (ACE_Message_Block *&first_item,
                             ACE_Time_Value *tv = 0))

private:
  /// Maximum number of messages that can be queued.
  int max_messages_;

  /// Maximum message size, in bytes.
  int max_message_length_;

  /// Native message queue options.
  int options_;

};

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* ACE_VXWORKS */

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

#if defined (ACE_HAS_WIN32_OVERLAPPED_IO)
/**
 * @class ACE_Message_Queue_NT
 *
 * @brief Message Queue implementation using IO completion port on NT.
 *
 * Implementation of a strip-downed ACE_Message_Queue using NT's
 * IO completion port mechanism.
 * @note *Many* ACE_Message_Queue features are not supported with
 * this implementation, including:
 * * <open> method have different signatures.
 * * <dequeue_head> *requires* that the ACE_Message_Block
 * pointer argument point to an ACE_Message_Block that was
 * allocated by the caller.
 * * <peek_dequeue_head>.
 * * <ACE_Message_Queue_Iterators>.
 * * No flow control.
 */
class ACE_Export ACE_Message_Queue_NT : public ACE_Message_Queue_Base
{
public:
  // = Initialization and termination methods.
  ACE_Message_Queue_NT (DWORD max_threads = ACE_Message_Queue_Base::DEFAULT_HWM);

  /**
   * Initialize the Message Queue by creating a new NT I/O completion
   * port.  The first arguemnt specifies the number of threads
   * released by the MQ that are allowed to run concurrently.  Return
   * 0 when succeeds, -1 otherwise.
   */
  virtual int open (DWORD max_threads = ACE_Message_Queue_Base::DEFAULT_HWM);

  /// Close down the underlying I/O completion port.  You need to
  /// re-open the MQ after this function is executed.
  virtual int close (void);

  /// Close down the message queue and release all resources.
  virtual ~ACE_Message_Queue_NT (void);

  // = Enqueue and dequeue methods.

  /**
   * Enqueue an <ACE_Message_Block *> at the end of the queue.
   * Returns -1 on failure, else the number of items still on the
   * queue.
   */
  virtual int enqueue_tail (ACE_Message_Block *new_item,
                            ACE_Time_Value *timeout = 0);
  virtual int enqueue (ACE_Message_Block *new_item,
                       ACE_Time_Value *timeout = 0);

  /**
   * Dequeue and return the <ACE_Message_Block *> at the head of the
   * queue.  Returns -1 on failure, else the number of items still on
   * the queue.
   */
  virtual int dequeue_head (ACE_Message_Block *&first_item,
                            ACE_Time_Value *timeout = 0);
  virtual int dequeue (ACE_Message_Block *&first_item,
                       ACE_Time_Value *timeout = 0);

  // = Check if queue is full/empty.
  /**
   * Always return false.
   */

  virtual int is_full (void);
  /**
   * True if queue is empty, else false.  Notice the return value is
   * only transient.
   */
  virtual int is_empty (void);

  // = Queue statistic methods (transient.)
  /**
   * Number of total bytes on the queue, i.e., sum of the message
   * block sizes.
   */
  virtual size_t message_bytes (void);

  /**
   * Number of total length on the queue, i.e., sum of the message
   * block lengths.
   */
  virtual size_t message_length (void);

  /**
   * Number of total messages on the queue.
   */
  virtual size_t message_count (void);

  // = Manual changes to these stats (used when queued message blocks
  // change size or lengths).
  /**
   * New value of the number of total bytes on the queue, i.e., sum of
   * the message block sizes.
   */
  virtual void message_bytes (size_t new_size);

  /**
   * New value of the number of total length on the queue, i.e., sum
   * of the message block lengths.
   */
  virtual void message_length (size_t new_length);

  /// Get the max concurrent thread number.
  virtual DWORD max_threads (void);

  // = Activation control methods.

  /**
   * Deactivate the queue and wake up all threads waiting on the queue
   * so they can continue.  No messages are removed from the queue,
   * however.  Any other operations called until the queue is
   * activated again will immediately return -1 with @c errno
   * ESHUTDOWN.
   *
   * @retval  The queue's state before this call.
   */
  virtual int deactivate (void);

  /**
   * Reactivate the queue so that threads can enqueue and dequeue
   * messages again.  Returns the state of the queue before the call.
   */
  virtual int activate (void);

  /**
   * Pulse the queue to wake up any waiting threads.  Changes the
   * queue state to PULSED; future enqueue/dequeue operations proceed
   * as in ACTIVATED state.
   *
   * @retval  The queue's state before this call.
   */
  virtual int pulse (void);

  /// Returns true if the state of the queue is <DEACTIVATED>,
  /// but false if the queue's is <ACTIVATED> or <PULSED>.
  virtual int deactivated (void);

  // = Not currently implemented...
  int peek_dequeue_head (ACE_Message_Block *&first_item,
                         ACE_Time_Value *timeout = 0);
  ACE_Notification_Strategy *notification_strategy (void);
  void notification_strategy (ACE_Notification_Strategy *s);

  // = Notification hook.

  /// Dump the state of an object.
  virtual void dump (void) const;

  /// Get the handle to the underlying completion port.
  virtual ACE_HANDLE completion_port (void);

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

private:

  // Disallow copying and assignment.
  ACE_Message_Queue_NT (const ACE_Message_Queue_NT &);
  void operator= (const ACE_Message_Queue_NT &);

private:
  // = Internal states.

  /// Maximum threads that can be released (and run) concurrently.
  DWORD max_cthrs_;

  /// Current number of threads waiting to dequeue messages.
  DWORD cur_thrs_;

  /// Current number of bytes in queue.
  size_t cur_bytes_;

  /// Current length of messages in queue.
  size_t cur_length_;

  /// Current number of messages in the queue.
  size_t cur_count_;

  /**
   * Synchronizer.  This should really be an ACE_Recursive_Thread_Mutex
   * but since this class is only supported on NT, it's okay to use
   * ACE_Thread_Mutex here.
   */
  ACE_Thread_Mutex lock_;

  /// Underlying NT IoCompletionPort.
  ACE_HANDLE completion_port_;

};
#endif /* ACE_HAS_WIN32_OVERLAPPED_IO */

ACE_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "ace/Message_Queue.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* ACE_MESSAGE_QUEUE_H */
