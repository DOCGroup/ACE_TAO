/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Message_Queue_T.h
//
// = AUTHOR
//    Doug Schmidt
//
// ============================================================================

#ifndef ACE_MESSAGE_QUEUE_T_H
#define ACE_MESSAGE_QUEUE_T_H

#include "ace/Synch.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (VXWORKS)
class ACE_Message_Queue_Vx;
#endif /* defined (VXWORKS) */

#if defined (ACE_WIN32) && (ACE_HAS_WINNT4 != 0)
class ACE_Message_Queue_NT;
#endif /* ACE_WIN32 && ACE_HAS_WINNT4 != 0 */

template <ACE_SYNCH_DECL>
class ACE_Message_Queue : public ACE_Message_Queue_Base
{
  // = TITLE
  //     A threaded message queueing facility, modeled after the
  //     queueing facilities in System V STREAMs.
  //
  // = DESCRIPTION
  //     An <ACE_Message_Queue> is the central queueing facility for
  //     messages in the ASX framework.  If <ACE_SYNCH_DECL> is
  //     <ACE_MT_SYNCH> then all operations are thread-safe.
  //     Otherwise, if it's <ACE_NULL_SYNCH> then there's no locking
  //     overhead.
public:
  friend class ACE_Message_Queue_Iterator<ACE_SYNCH_USE>;
  friend class ACE_Message_Queue_Reverse_Iterator<ACE_SYNCH_USE>;

  // = Traits
  typedef ACE_Message_Queue_Iterator<ACE_SYNCH_USE>
          ITERATOR;
  typedef ACE_Message_Queue_Reverse_Iterator<ACE_SYNCH_USE>
          REVERSE_ITERATOR;

  // = Initialization and termination methods.
  ACE_Message_Queue (size_t high_water_mark = ACE_Message_Queue_Base::DEFAULT_HWM,
                     size_t low_water_mark = ACE_Message_Queue_Base::DEFAULT_LWM,
                     ACE_Notification_Strategy * = 0);
  // Initialize an <ACE_Message_Queue>.  The <high_water_mark>
  // determines how many bytes can be stored in a queue before it's
  // considered "full."  Supplier threads must block until the queue
  // is no longer full.  The <low_water_mark> determines how many
  // bytes must be in the queue before supplier threads are allowed to
  // enqueue additional <ACE_Message_Block>s.  By default, the
  // <high_water_mark> equals the <low_water_mark>, which means that
  // suppliers will be able to enqueue new messages as soon as a
  // consumer removes any message from the queue.  Making the
  // <low_water_mark> smaller than the <high_water_mark> forces
  // consumers to drain more messages from the queue before suppliers
  // can enqueue new messages, which can minimize the "silly window
  // syndrome."

  virtual int open (size_t hwm = ACE_Message_Queue_Base::DEFAULT_HWM,
                    size_t lwm = ACE_Message_Queue_Base::DEFAULT_LWM,
                    ACE_Notification_Strategy * = 0);
  // Initialize an <ACE_Message_Queue>.  The <high_water_mark>
  // determines how many bytes can be stored in a queue before it's
  // considered "full."  Supplier threads must block until the queue
  // is no longer full.  The <low_water_mark> determines how many
  // bytes must be in the queue before supplier threads are allowed to
  // enqueue additional <ACE_Message_Block>s.  By default, the
  // <high_water_mark> equals the <low_water_mark>, which means that
  // suppliers will be able to enqueue new messages as soon as a
  // consumer removes any message from the queue.  Making the
  // <low_water_mark> smaller than the <high_water_mark> forces
  // consumers to drain more messages from the queue before suppliers
  // can enqueue new messages, which can minimize the "silly window
  // syndrome."

  virtual int close (void);
  // Close down the message queue and release all resources.

  virtual ~ACE_Message_Queue (void);
  // Close down the message queue and release all resources.

  // = Enqueue and dequeue methods.

  // For the following enqueue and dequeue methods if <timeout> == 0,
  // the caller will block until action is possible, else will wait
  // until the absolute time specified in *<timeout> elapses).  These
  // calls will return, however, when queue is closed, deactivated,
  // when a signal occurs, or if the time specified in timeout
  // elapses, (in which case errno = EWOULDBLOCK).

  virtual int peek_dequeue_head (ACE_Message_Block *&first_item,
                                 ACE_Time_Value *timeout = 0);
  // Retrieve the first <ACE_Message_Block> without removing it.
  // Returns -1 on failure, else the number of items still on the
  // queue.  Note that <timeout> uses <{absolute}> time rather than
  // <{relative}> time.

  virtual int enqueue_prio (ACE_Message_Block *new_item,
                            ACE_Time_Value *timeout = 0);
  // Enqueue an <ACE_Message_Block *> into the <Message_Queue> in
  // accordance with its <msg_priority> (0 is lowest priority).  FIFO
  // order is maintained when messages of the same priority are
  // inserted consecutively.  Note that <timeout> uses <{absolute}>
  // time rather than <{relative}> time.  Returns -1 on failure, else
  // the number of items still on the queue.

  virtual int enqueue (ACE_Message_Block *new_item,
                       ACE_Time_Value *timeout = 0);
  // This is an alias for <enqueue_prio>.  It's only here for
  // backwards compatibility and will go away in a subsequent release.
  // Please use <enqueue_prio> instead.  Note that <timeout> uses
  // <{absolute}> time rather than <{relative}> time.

  virtual int enqueue_tail (ACE_Message_Block *new_item,
                            ACE_Time_Value *timeout = 0);
  // Enqueue an <ACE_Message_Block *> at the end of the queue.  Note
  // that <timeout> uses <{absolute}> time rather than <{relative}>
  // time.  Returns -1 on failure, else the number of items still on
  // the queue.

  virtual int enqueue_head (ACE_Message_Block *new_item,
                            ACE_Time_Value *timeout = 0);
  // Enqueue an <ACE_Message_Block *> at the head of the queue.  Note
  // that <timeout> uses <{absolute}> time rather than <{relative}>
  // time.  Returns -1 on failure, else the number of items still on
  // the queue.

  virtual int dequeue (ACE_Message_Block *&first_item,
                       ACE_Time_Value *timeout = 0);
  // This method is an alias for the following method:
  virtual int dequeue_head (ACE_Message_Block *&first_item,
                            ACE_Time_Value *timeout = 0);
  // Dequeue and return the <ACE_Message_Block *> at the head of the
  // queue.  Note that <timeout> uses <{absolute}> time rather than
  // <{relative}> time.  Returns -1 on failure, else the number of
  // items still on the queue.

  // = Check if queue is full/empty.
  virtual int is_full (void);
  // True if queue is full, else false.
  virtual int is_empty (void);
  // True if queue is empty, else false.

  // = Queue statistic methods.
  virtual size_t message_bytes (void);
  // Number of total bytes on the queue.
  virtual size_t message_count (void);
  // Number of total messages on the queue.

  // = Flow control methods.

  virtual size_t high_water_mark (void);
  // Get high watermark.
  virtual void high_water_mark (size_t hwm);
  // Set the high watermark, which determines how many bytes can be
  // stored in a queue before it's considered "full."

  virtual size_t low_water_mark (void);
  // Get low watermark.
  virtual void low_water_mark (size_t lwm);
  // Set the low watermark, which determines how many bytes must be in
  // the queue before supplier threads are allowed to enqueue
  // additional <ACE_Message_Block>s.

  // = Activation control methods.

  virtual int deactivate (void);
  // Deactivate the queue and wakeup all threads waiting on the queue
  // so they can continue.  No messages are removed from the queue,
  // however.  Any other operations called until the queue is
  // activated again will immediately return -1 with <errno> ==
  // ESHUTDOWN.  Returns WAS_INACTIVE if queue was inactive before the
  // call and WAS_ACTIVE if queue was active before the call.

  virtual int activate (void);
  // Reactivate the queue so that threads can enqueue and dequeue
  // messages again.  Returns WAS_INACTIVE if queue was inactive
  // before the call and WAS_ACTIVE if queue was active before the
  // call.

  virtual int deactivated (void);
  // Returns true if <deactivated_> is enabled.

  // = Notification hook.

  virtual int notify (void);
  // This hook is automatically invoked by <enqueue_head>,
  // <enqueue_tail>, and <enqueue_prio> when a new item is inserted
  // into the queue.  Subclasses can override this method to perform
  // specific notification strategies (e.g., signaling events for a
  // <WFMO_Reactor>, notifying a <Reactor>, etc.).  In a
  // multi-threaded application with concurrent consumers, there is no
  // guarantee that the queue will be still be non-empty by the time
  // the notification occurs.

  // = Get/set the notification strategy for the <Message_Queue>
  virtual ACE_Notification_Strategy *notification_strategy (void);
  virtual void notification_strategy (ACE_Notification_Strategy *s);

  virtual void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  // = Routines that actually do the enqueueing and dequeueing.
  // These routines assume that locks are held by the corresponding
  // public methods.  Since they are virtual, you can change the
  // queueing mechanism by subclassing from <ACE_Message_Queue>.

  virtual int enqueue_i (ACE_Message_Block *new_item);
  // Enqueue an <ACE_Message_Block *> in accordance with its priority.

  virtual int enqueue_tail_i (ACE_Message_Block *new_item);
  // Enqueue an <ACE_Message_Block *> at the end of the queue.

  virtual int enqueue_head_i (ACE_Message_Block *new_item);
  // Enqueue an <ACE_Message_Block *> at the head of the queue.

  virtual int dequeue_head_i (ACE_Message_Block *&first_item);
  // Dequeue and return the <ACE_Message_Block *> at the head of the
  // queue.

  // = Check the boundary conditions (assumes locks are held).
  virtual int is_full_i (void);
  // True if queue is full, else false.
  virtual int is_empty_i (void);
  // True if queue is empty, else false.

  // = Implementation of the public activate() and deactivate() methods above (assumes locks are held).
  virtual int deactivate_i (void);
  // Deactivate the queue.
  virtual int activate_i (void);
  // Activate the queue.

  // = Helper methods to factor out common #ifdef code.
  virtual int wait_not_full_cond (ACE_Guard<ACE_SYNCH_MUTEX_T> &mon,
                                  ACE_Time_Value *timeout);
  // Wait for the queue to become non-full.

  virtual int wait_not_empty_cond (ACE_Guard<ACE_SYNCH_MUTEX_T> &mon,
                                   ACE_Time_Value *timeout);
  // Wait for the queue to become non-empty.

  virtual int signal_enqueue_waiters (void);
  // Inform any threads waiting to enqueue that they can procede.

  virtual int signal_dequeue_waiters (void);
  // Inform any threads waiting to dequeue that they can procede.

  ACE_Message_Block *head_;
  // Pointer to head of ACE_Message_Block list.

  ACE_Message_Block *tail_;
  // Pointer to tail of ACE_Message_Block list.

  size_t low_water_mark_;
  // Lowest number before unblocking occurs.

  size_t high_water_mark_;
  // Greatest number of bytes before blocking.

  size_t cur_bytes_;
  // Current number of bytes in the queue.

  size_t cur_count_;
  // Current number of messages in the queue.

  int deactivated_;
  // Indicates that the queue is inactive.

  ACE_Notification_Strategy *notification_strategy_;
  // The notification strategy used when a new message is enqueued.

  // = Synchronization primitives for controlling concurrent access.
  ACE_SYNCH_MUTEX_T lock_;
  // Protect queue from concurrent access.

#if defined (ACE_HAS_OPTIMIZED_MESSAGE_QUEUE)
  ACE_SYNCH_SEMAPHORE_T not_empty_cond_;
  // Used to make threads sleep until the queue is no longer empty.

  ACE_SYNCH_SEMAPHORE_T not_full_cond_;
  // Used to make threads sleep until the queue is no longer full.

  size_t dequeue_waiters_;
  // Number of threads waiting to dequeue a <Message_Block>.

  size_t enqueue_waiters_;
  // Number of threads waiting to enqueue a <Message_Block>.
#else
  ACE_SYNCH_CONDITION_T not_empty_cond_;
  // Used to make threads sleep until the queue is no longer empty.

  ACE_SYNCH_CONDITION_T not_full_cond_;
  // Used to make threads sleep until the queue is no longer full.
#endif /* ACE_HAS_OPTIMIZED_MESSAGE_QUEUE */

private:

  // = Disallow these operations.
  ACE_UNIMPLEMENTED_FUNC (void operator= (const ACE_Message_Queue<ACE_SYNCH_USE> &))
  ACE_UNIMPLEMENTED_FUNC (ACE_Message_Queue (const ACE_Message_Queue<ACE_SYNCH_USE> &))
};

template <ACE_SYNCH_DECL>
class ACE_Message_Queue_Iterator
{
  // = TITLE
  //     Iterator for the <ACE_Message_Queue>.
public:
  // = Initialization method.
  ACE_Message_Queue_Iterator (ACE_Message_Queue <ACE_SYNCH_USE> &queue);

  // = Iteration methods.
  int next (ACE_Message_Block *&entry);
  // Pass back the <entry> that hasn't been seen in the queue.
  // Returns 0 when all items have been seen, else 1.

  int done (void) const;
  // Returns 1 when all items have been seen, else 0.

  int advance (void);
  // Move forward by one element in the queue.  Returns 0 when all the
  // items in the set have been seen, else 1.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  ACE_Message_Queue <ACE_SYNCH_USE> &queue_;
  // Message_Queue we are iterating over.

  ACE_Message_Block *curr_;
  // Keeps track of how far we've advanced...
};

template <ACE_SYNCH_DECL>
class ACE_Message_Queue_Reverse_Iterator
{
  // = TITLE
  //     Reverse Iterator for the <ACE_Message_Queue>.
public:
  // = Initialization method.
  ACE_Message_Queue_Reverse_Iterator (ACE_Message_Queue <ACE_SYNCH_USE> &queue);

  // = Iteration methods.
  int next (ACE_Message_Block *&entry);
  // Pass back the <entry> that hasn't been seen in the queue.
  // Returns 0 when all items have been seen, else 1.

  int done (void) const;
  // Returns 1 when all items have been seen, else 0.

  int advance (void);
  // Move forward by one element in the queue.  Returns 0 when all the
  // items in the set have been seen, else 1.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  ACE_Message_Queue <ACE_SYNCH_USE> &queue_;
  // Message_Queue we are iterating over.

  ACE_Message_Block *curr_;
  // Keeps track of how far we've advanced...
};

template <ACE_SYNCH_DECL>
class ACE_Dynamic_Message_Queue : public ACE_Message_Queue<ACE_SYNCH_USE>
{
  // = TITLE
  //     A derived class which adapts the <ACE_Message_Queue>
  //     class in order to maintain dynamic priorities for enqueued
  //     <ACE_Message_Blocks> and manage the queue order according
  //     to these dynamic priorities.
  //
  // = DESCRIPTION
  //
  //     The messages in the queue are managed so as to preserve
  //     a logical ordering with minimal overhead per enqueue and
  //     dequeue operation.  For this reason, the actual order of
  //     messages in the linked list of the queue may differ from
  //     their priority order.  As time passes, a message may change
  //     from pending status to late status, and eventually to beyond
  //     late status.  To minimize reordering overhead under this
  //     design force, three separate boundaries are maintained
  //     within the linked list of messages.  Messages are dequeued
  //     preferentially from the head of the pending portion, then
  //     the head of the late portion, and finally from the head
  //     of the beyond late portion.  In this way, only the boundaries
  //     need to be maintained (which can be done efficiently, as
  //     aging messages maintain the same linked list order as they
  //     progress from one status to the next), with no reordering
  //     of the messages themselves, while providing correct priority
  //     ordered dequeueing semantics.
  //
  //     Head and tail enqueue methods inherited from ACE_Message_Queue
  //     are made private to prevent out-of-order messages from confusing
  //     management of the various portions of the queue.  Messages in
  //     the pending portion of the queue whose priority becomes late
  //     (according to the specific dynamic strategy) advance into
  //     the late portion of the queue.  Messages in the late portion
  //     of the queue whose priority becomes later than can be represented
  //     advance to the beyond_late portion of the queue.  These behaviors
  //     support a limited schedule overrun, with pending messages prioritized
  //     ahead of late messages, and late messages ahead of beyond late
  //     messages.  These behaviors can be modified in derived classes by
  //     providing alternative definitions for the appropriate virtual methods.
  //
  //     When filled with messages, the queue's linked list should look like:
  //
  //          H                                           T
  //          |                                           |
  //
  //          B - B - B - B - L - L - L - P - P - P - P - P
  //
  //          |           |   |       |   |               |
  //         BH          BT   LH     LT   PH             PT
  //
  //     Where the symbols are as follows:
  //
  //     H  = Head of the entire list
  //     T  = Tail of the entire list
  //     B  = Beyond late message
  //     BH = Beyond late messages Head
  //     BT = Beyond late messages Tail
  //     L  = Late message
  //     LH = Late messages Head
  //     LT = Late messages Tail
  //     P  = Pending message
  //     PH = Pending messages Head
  //     PT = Pending messages Tail
  //
  //     Caveat: the virtual methods enqueue_tail, enqueue_head,
  //             and peek_dequeue_head have semantics for the static
  //             message queues that cannot be guaranteed for dynamic
  //             message queues.  The peek_dequeue_head method just
  //             calls the base class method, while the two enqueue
  //             methods call the priority enqueue method.  The
  //             order of messages in the dynamic queue is a function
  //             of message deadlines and how long they are in the
  //             queues.  You can manipulate these in some cases to
  //             ensure the correct semantics, but that is not a
  //             very stable or portable approach (discouraged).
  //
public:
  // = Initialization and termination methods.
  ACE_Dynamic_Message_Queue (ACE_Dynamic_Message_Strategy & message_strategy,
                             size_t hwm = ACE_Message_Queue_Base::DEFAULT_HWM,
                             size_t lwm = ACE_Message_Queue_Base::DEFAULT_LWM,
                             ACE_Notification_Strategy * = 0);

  virtual ~ACE_Dynamic_Message_Queue (void);
  // Close down the message queue and release all resources.

  virtual int remove_messages (ACE_Message_Block *&list_head,
                               ACE_Message_Block *&list_tail,
                               u_int status_flags);
  // Detach all messages with status given in the passed flags from
  // the queue and return them by setting passed head and tail pointers
  // to the linked list they comprise.  This method is intended primarily
  // as a means of periodically harvesting messages that have missed
  // their deadlines, but is available in its most general form.  All
  // messages are returned in priority order, from head to tail, as of
  // the time this method was called.

  virtual int dequeue_head (ACE_Message_Block *&first_item,
                            ACE_Time_Value *timeout = 0);
  // Dequeue and return the <ACE_Message_Block *> at the head of the
  // queue.  Returns -1 on failure, else the number of items still on
  // the queue.

  virtual void dump (void) const;
  // Dump the state of the queue.

  virtual int enqueue_tail (ACE_Message_Block *new_item,
                            ACE_Time_Value *timeout = 0);
  // just call priority enqueue method: tail enqueue semantics for dynamic
  // message queues are unstable: the message may or may not be where
  // it was placed after the queue is refreshed prior to the next
  // enqueue or dequeue operation.

  virtual int enqueue_head (ACE_Message_Block *new_item,
                            ACE_Time_Value *timeout = 0);
  // just call priority enqueue method: head enqueue semantics for dynamic
  // message queues are unstable: the message may or may not be where
  // it was placed after the queue is refreshed prior to the next
  // enqueue or dequeue operation.


  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:

  virtual int enqueue_i (ACE_Message_Block *new_item);
  // Enqueue an <ACE_Message_Block *> in accordance with its priority.
  // priority may be *dynamic* or *static* or a combination or *both*
  // It calls the priority evaluation function passed into the Dynamic
  // Message Queue constructor to update the priorities of all
  // enqueued messages.

  virtual int sublist_enqueue_i (ACE_Message_Block *new_item,
                                 const ACE_Time_Value &current_time,
                                 ACE_Message_Block *&sublist_head,
                                 ACE_Message_Block *&sublist_tail,
                                 ACE_Dynamic_Message_Strategy::Priority_Status status);
  // enqueue a message in priority order within a given priority status sublist

  virtual int dequeue_head_i (ACE_Message_Block *&first_item);
  // Dequeue and return the <ACE_Message_Block *> at the head of the
  // logical queue.  Attempts first to dequeue from the pending
  // portion of the queue, or if that is empty from the late portion,
  // or if that is empty from the beyond late portion, or if that is
  // empty just sets the passed pointer to zero and returns -1.

  virtual int refresh_queue (const ACE_Time_Value & current_time);
  // Refresh the queue using the strategy
  // specific priority status function.

  virtual int refresh_pending_queue (const ACE_Time_Value & current_time);
  // Refresh the pending queue using the strategy
  // specific priority status function.

  virtual int refresh_late_queue (const ACE_Time_Value & current_time);
  // Refresh the late queue using the strategy
  // specific priority status function.

  ACE_Message_Block *pending_head_;
  // Pointer to head of the pending messages

  ACE_Message_Block *pending_tail_;
  // Pointer to tail of the pending messages

  ACE_Message_Block *late_head_;
  // Pointer to head of the late messages

  ACE_Message_Block *late_tail_;
  // Pointer to tail of the late messages

  ACE_Message_Block *beyond_late_head_;
  // Pointer to head of the beyond late messages

  ACE_Message_Block *beyond_late_tail_;
  // Pointer to tail of the beyond late messages

  ACE_Dynamic_Message_Strategy &message_strategy_;
  // Pointer to a dynamic priority evaluation function.

private:
  // = Disallow public access to these operations.

  ACE_UNIMPLEMENTED_FUNC (void operator= (const ACE_Dynamic_Message_Queue<ACE_SYNCH_USE> &))
  ACE_UNIMPLEMENTED_FUNC (ACE_Dynamic_Message_Queue (const ACE_Dynamic_Message_Queue<ACE_SYNCH_USE> &))

  // provide definitions for these (just call base class method),
  // but make them private so they're not accessible outside the class

  virtual int peek_dequeue_head (ACE_Message_Block *&first_item,
                                 ACE_Time_Value *timeout = 0);
  // private method to hide public base class method: just calls base class method

};

template <ACE_SYNCH_DECL>
class ACE_Message_Queue_Factory
{
  // = TITLE
  //     ACE_Message_Queue_Factory is a static factory class template which
  //     provides a separate factory method for each of the major kinds of
  //     priority based message dispatching: static, earliest deadline first
  //     (EDF), and minimum laxity first (MLF).
  //
  // = DESCRIPTION
  //     The ACE_Dynamic_Message_Queue class assumes responsibility for
  //     releasing the resources of the strategy with which it was
  //     constructed: the user of a message queue constructed by
  //     any of these factory methods is only responsible for
  //     ensuring destruction of the message queue itself.

public:
  static ACE_Message_Queue<ACE_SYNCH_USE> *
    create_static_message_queue (size_t hwm = ACE_Message_Queue_Base::DEFAULT_HWM,
                                 size_t lwm = ACE_Message_Queue_Base::DEFAULT_LWM,
                                 ACE_Notification_Strategy * = 0);
  // factory method for a statically prioritized ACE_Message_Queue

  static ACE_Dynamic_Message_Queue<ACE_SYNCH_USE> *
    create_deadline_message_queue (size_t hwm = ACE_Message_Queue_Base::DEFAULT_HWM,
                                   size_t lwm = ACE_Message_Queue_Base::DEFAULT_LWM,
                                   ACE_Notification_Strategy * = 0,
                                   u_long static_bit_field_mask = 0x3FFUL,        // 2^(10) - 1
                                   u_long static_bit_field_shift = 10,            // 10 low order bits
                                   u_long dynamic_priority_max = 0x3FFFFFUL,      // 2^(22)-1
                                   u_long dynamic_priority_offset =  0x200000UL); // 2^(22-1)
  // factory method for a dynamically prioritized (by time to deadline) ACE_Dynamic_Message_Queue

  static ACE_Dynamic_Message_Queue<ACE_SYNCH_USE> *
    create_laxity_message_queue (size_t hwm = ACE_Message_Queue_Base::DEFAULT_HWM,
                                 size_t lwm = ACE_Message_Queue_Base::DEFAULT_LWM,
                                 ACE_Notification_Strategy * = 0,
                                 u_long static_bit_field_mask = 0x3FFUL,        // 2^(10) - 1
                                 u_long static_bit_field_shift = 10,            // 10 low order bits
                                 u_long dynamic_priority_max = 0x3FFFFFUL,      // 2^(22)-1
                                 u_long dynamic_priority_offset =  0x200000UL); // 2^(22-1)
  // factory method for a dynamically prioritized (by laxity) ACE_Dynamic_Message_Queue


#if defined (VXWORKS)

  static ACE_Message_Queue_Vx *
    create_Vx_message_queue (size_t max_messages, size_t max_message_length,
                             ACE_Notification_Strategy *ns = 0);
  // factory method for a wrapped VxWorks message queue

#endif /* defined (VXWORKS) */

#if defined (ACE_WIN32) && (ACE_HAS_WINNT4 != 0)

  static ACE_Message_Queue_NT *
  create_NT_message_queue (size_t max_threads);
  // factory method for a NT message queue.

#endif /* ACE_WIN32 && ACE_HAS_WINNT4 != 0 */
};

#if defined (__ACE_INLINE__)
#include "ace/Message_Queue_T.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Message_Queue_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Message_Queue_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* ACE_MESSAGE_QUEUE_T_H */
