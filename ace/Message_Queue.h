/* -*- C++ -*- */

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
#include "ace/pre.h"

#include "ace/Message_Block.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/IO_Cntl_Msg.h"

// Forward decls.
class ACE_Notification_Strategy;
template <ACE_SYNCH_DECL> class ACE_Message_Queue_Iterator;
template <ACE_SYNCH_DECL> class ACE_Message_Queue_Reverse_Iterator;

/**
 * @class ACE_Message_Queue_Base
 *
 * @brief Base class for <ACE_Message_Queue>, which is the central
 * queueing facility for messages in the ACE framework.
 * 
 * For all the <ACE_Time_Value> pointer parameters the caller will
 * block until action is possible if <timeout> == 0.  Otherwise, it
 * will wait until the absolute time specified in *<timeout>
 * elapses.  
 */
class ACE_Export ACE_Message_Queue_Base
{
public:
  // = Default high and low water marks.
  enum
  {
    /// Default high watermark (16 K).
    DEFAULT_HWM = 16 * 1024,
    /// Default low watermark (same as high water mark).
    DEFAULT_LWM = 16 * 1024,
    /// Message queue was active before <activate> or <deactivate>.
    WAS_ACTIVE = 1,
    /// Message queue was inactive before <activate> or <deactivate>.
    WAS_INACTIVE = 2
  };

  ACE_Message_Queue_Base (void);

  /// Close down the message queue and release all resources.
  virtual int close (void) = 0;

  /// Close down the message queue and release all resources.
  virtual ~ACE_Message_Queue_Base (void);

  // = Enqueue and dequeue methods.

  /**
   * Retrieve the first <ACE_Message_Block> without removing it.  Note
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
  /// True if queue is empty, else false.
  virtual int is_full (void) = 0;
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
   * Deactivate the queue and wakeup all threads waiting on the queue
   * so they can continue.  No messages are removed from the queue,
   * however.  Any other operations called until the queue is
   * activated again will immediately return -1 with <errno> ==
   * ESHUTDOWN.  Returns WAS_INACTIVE if queue was inactive before the
   * call and WAS_ACTIVE if queue was active before the call.
   */
  virtual int deactivate (void) = 0;

  /**
   * Reactivate the queue so that threads can enqueue and dequeue
   * messages again.  Returns WAS_INACTIVE if queue was inactive
   * before the call and WAS_ACTIVE if queue was active before the
   * call.
   */
  virtual int activate (void) = 0;

  /// Returns true if <deactivated_> is enabled.
  virtual int deactivated (void) = 0;

  // = Get/set the notification strategy for the <Message_Queue>
  virtual ACE_Notification_Strategy *notification_strategy (void) = 0;
  virtual void notification_strategy (ACE_Notification_Strategy *s) = 0;

  // = Notification hook.

  /// Dump the state of an object.
  virtual void dump (void) const = 0;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

private:
  // = Disallow these operations.
  ACE_UNIMPLEMENTED_FUNC (void operator= (const ACE_Message_Queue_Base &))
  ACE_UNIMPLEMENTED_FUNC (ACE_Message_Queue_Base (const ACE_Message_Queue_Base &))
};

// Include the templates here.
#include "ace/Message_Queue_T.h"

#if defined (VXWORKS)
# include /**/ <msgQLib.h>

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
 * NOTE: *Many* ACE_Message_Queue features are not supported with
 * this specialization, including:
 * * The two size arguments to the constructor and <open> are
 * interpreted differently.  The first is interpreted as the
 * maximum number of bytes in a message.  The second is
 * interpreted as the maximum number of messages that can be
 * queued.
 * * <dequeue_head> *requires* that the ACE_Message_Block
 * pointer argument point to an ACE_Message_Block that was
 * allocated by the caller.  It must be big enough to support
 * the received message, without using continutation.  The
 * pointer argument is not modified.
 * * Message priority.  MSG_Q_FIFO is hard-coded.
 * * enqueue method timeouts.
 * * <peek_dequeue_head>.
 * * <ACE_Message_Queue_Iterators>.
 * * The ability to change low and high water marks after creation.
 * * <Message_Block> chains.  The continuation field of <ACE_Message_Block>
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
   * Number of total length on the queue, i.e., sum of the message
   * block lengths.
   * Number of total messages on the queue.
   */
  virtual size_t message_bytes (void);
  virtual size_t message_length (void);
  virtual size_t message_count (void);

  // = Manual changes to these stats (used when queued message blocks
  // change size or lengths).
  /**
   * New value of the number of total bytes on the queue, i.e., sum of
   * the message block sizes.
   * New value of the number of total length on the queue, i.e., sum
   * of the message block lengths.
   */
  virtual void message_bytes (size_t new_size);
  virtual void message_length (size_t new_length);

  // = Flow control routines
  /**
   * Get high watermark.
   * Set high watermark.
   * Get low watermark.
   * Set low watermark.
   */
  virtual size_t high_water_mark (void);
  virtual void high_water_mark (size_t hwm);
  virtual size_t low_water_mark (void);
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
  /// True if queue is empty, else false.
  virtual int is_full_i (void);
  virtual int is_empty_i (void);

  // = Implementation of public <activate>/<deactivate> methods above.

  // These methods assume locks are held.

  /// Deactivate the queue.
  /// Activate the queue.
  virtual int deactivate_i (void);
  virtual int activate_i (void);

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
  /// Maximum number of messages that can be queued.
  int max_messages_;

  /// Maximum message size, in bytes.
  int max_message_length_;

  /// Native message queue options.
  int options_;

  // = Disallow these operations.
  ACE_UNIMPLEMENTED_FUNC (void operator= (const ACE_Message_Queue_Vx &))
  ACE_UNIMPLEMENTED_FUNC (ACE_Message_Queue_Vx (const ACE_Message_Queue_Vx &))

  ACE_UNIMPLEMENTED_FUNC (virtual int peek_dequeue_head
                            (ACE_Message_Block *&first_item,
                             ACE_Time_Value *tv = 0))
};
#endif /* VXWORKS */

#if defined (ACE_WIN32) && (ACE_HAS_WINNT4 != 0)
/**
 * @class ACE_Message_Queue_NT
 *
 * @brief Message Queue implementation using IO completion port on NT.
 *
 * Implementation of a strip-downed ACE_Message_Queue using NT's
 * IO completion port mechanism.
 * NOTE: *Many* ACE_Message_Queue features are not supported with
 * this implementation, including:
 * * <open> method have different signatures.
 * * <dequeue_head> *requires* that the <ACE_Message_Block>
 * pointer argument point to an <ACE_Message_Block> that was
 * allocated by the caller.
 * * <peek_dequeue_head>.
 * * <ACE_Message_Queue_Iterators>.
 * * No flow control.
 */
class ACE_Export ACE_Message_Queue_NT : public ACE_Message_Queue_Base
{
public:
  // = Initialization and termination methods.
  ACE_Message_Queue_NT (size_t max_threads = ACE_Message_Queue_Base::DEFAULT_HWM);

  /**
   * Initialize the Message Queue by creating a new NT I/O completion
   * port.  The first arguemnt specifies the number of threads
   * released by the MQ that are allowed to run concurrently.  Return
   * 0 when succeeds, -1 otherwise.
   */
  virtual int open (size_t max_threads = ACE_Message_Queue_Base::DEFAULT_HWM);

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
   * True if queue is empty, else false.  Notice the return value is
   * only transient.
   */
  virtual int is_full (void);
  virtual int is_empty (void);

  // = Queue statistic methods (transient.)
  /**
   * Number of total bytes on the queue, i.e., sum of the message
   * block sizes.
   * Number of total length on the queue, i.e., sum of the message
   * block lengths.
   * Number of total messages on the queue.
   */
  virtual size_t message_bytes (void);
  virtual size_t message_length (void);
  virtual size_t message_count (void);

  // = Manual changes to these stats (used when queued message blocks
  // change size or lengths).
  /**
   * New value of the number of total bytes on the queue, i.e., sum of
   * the message block sizes.
   * New value of the number of total length on the queue, i.e., sum
   * of the message block lengths.
   */
  virtual void message_bytes (size_t new_size);
  virtual void message_length (size_t new_length);

  /// Get the max concurrent thread number.
  virtual size_t max_threads (void);

  // = Activation control methods.

  /**
   * Deactivate the queue and wakeup all threads waiting on the queue
   * so they can continue.  Messages already in the queue get removed.
   * If there are more messages in the queue than there are threads
   * waiting on the queue, the left over messages will not be removed.
   * Any other enqueue/dequeue operations called until the queue is
   * activated again will immediately return -1 with <errno> ==
   * ESHUTDOWN.  Returns WAS_INACTIVE if queue was inactive before the
   * call and WAS_ACTIVE if queue was active before the call.
   */
  virtual int deactivate (void);

  /**
   * Reactivate the queue so that threads can enqueue and dequeue
   * messages again.  Returns WAS_INACTIVE if queue was inactive
   * before the call and WAS_ACTIVE if queue was active before the
   * call.
   */
  virtual int activate (void);

  /// Returns true if <deactivated_> is enabled.
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
  // = Internal states.

  /// Maximum threads that can be released (and run) concurrently.
  size_t max_cthrs_;

  /// Current number of threads waiting to dequeue messages.
  size_t cur_thrs_;

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

  /// Indicates that the queue is inactive.
  int deactivated_;

  /// Underlying NT IoCompletionPort.
  ACE_HANDLE completion_port_;

  // = Disallow these operations.
  ACE_UNIMPLEMENTED_FUNC (void operator= (const ACE_Message_Queue_NT &))
  ACE_UNIMPLEMENTED_FUNC (ACE_Message_Queue_NT (const ACE_Message_Queue_NT &))
};
#endif /* ACE_WIN32 && ACE_HAS_WINNT4 != 0 */


#if defined (__ACE_INLINE__)
#include "ace/Message_Queue.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_MESSAGE_QUEUE_H */
