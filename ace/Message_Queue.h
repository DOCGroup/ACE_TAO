/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    Message_Queue.h
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (ACE_MESSAGE_LIST_H)
#define ACE_MESSAGE_LIST_H

#include "ace/Message_Block.h"
#include "ace/IO_Cntl_Msg.h"

// Forward decls.
class ACE_Notification_Strategy;
template <ACE_SYNCH_1> class ACE_Message_Queue_Iterator;
template <ACE_SYNCH_1> class ACE_Message_Queue_Reverse_Iterator;

template <ACE_SYNCH_1>
class ACE_Message_Queue
  // = TITLE
  //     A threaded message queueing facility, modeled after the
  //     queueing facilities in System V STREAMs.
  //
  // = DESCRIPTION
  //     An <ACE_Message_Queue> is the central queueing facility for
  //     messages in the ASX framework.  If <ACE_SYNCH_1> is
  //     ACE_MT_SYNCH then all operations are thread-safe.  Otherwise,
  //     if it's <ACE_NULL_SYNCH> then there's no locking overhead.
{
friend class ACE_Message_Queue_Iterator<ACE_SYNCH_2>;
friend class ACE_Message_Queue_Reverse_Iterator<ACE_SYNCH_2>;
public:

  // = Traits
  typedef ACE_Message_Queue_Iterator<ACE_SYNCH_2> ITERATOR;
  typedef ACE_Message_Queue_Reverse_Iterator<ACE_SYNCH_2> REVERSE_ITERATOR;

  // = Default high and low water marks.
  enum 
  {
    DEFAULT_LWM = 0, 
    // Default low watermark.
    DEFAULT_HWM = 16 * 1024, 
    // Default high watermark (16 K).
    WAS_ACTIVE   = 1, 
    // Message queue was active before activate() or deactivate().
    WAS_INACTIVE = 2  
    // Message queue was inactive before activate() or deactivate().
  };

  // = Initialization and termination methods.
  ACE_Message_Queue (size_t hwm = DEFAULT_HWM, 
		     size_t lwm = DEFAULT_LWM, 
		     ACE_Notification_Strategy * = 0);

  // Create a message queue with all the defaults.
  int open (size_t hwm = DEFAULT_HWM, 
	    size_t lwm = DEFAULT_LWM,
	    ACE_Notification_Strategy * = 0);
  // Create a message queue with all the defaults.

  int close (void);
  // Close down the message queue and release all resources.

  virtual ~ACE_Message_Queue (void);
  // Close down the message queue and release all resources.

  int peek_dequeue_head (ACE_Message_Block *&first_item, 
			 ACE_Time_Value *tv = 0);
  // Retrieve the first ACE_Message_Block without removing it.
  // Returns -1 on failure, else the number of items still on the
  // queue.

  // = For all the following three routines if tv == 0, the caller
  // will block until action is possible, else will wait until the
  // absolute time specified in *tv elapses).  Calls will return,
  // however, when queue is closed, deactivated, when a signal occurs,
  // or if the time specified in tv elapses, (in which case errno =
  // EWOULDBLOCK).

  int enqueue_prio (ACE_Message_Block *new_item, ACE_Time_Value *tv = 0);
  // Enqueue an <ACE_Message_Block *> into the <Message_Queue> in
  // accordance with its <msg_priority> (0 is lowest priority).  FIFO
  // order is maintained when messages of the same priority are
  // inserted consecutively.  Returns -1 on failure, else the number
  // of items still on the queue.

  int enqueue_tail (ACE_Message_Block *new_item, ACE_Time_Value *tv = 0);
  // Enqueue an <ACE_Message_Block *> at the end of the queue.
  // Returns -1 on failure, else the number of items still on the
  // queue.

  int enqueue_head (ACE_Message_Block *new_item, ACE_Time_Value *tv = 0);
  // Enqueue an <ACE_Message_Block *> at the head of the queue.
  // Returns -1 on failure, else the number of items still on the
  // queue.

  int dequeue_head (ACE_Message_Block *&first_item, ACE_Time_Value *tv = 0);
  // Dequeue and return the <ACE_Message_Block *> at the head of the
  // queue.  Returns -1 on failure, else the number of items still on
  // the queue.

  // = Checks if queue is full/empty. 
  int is_full (void);
  // True if queue is full, else false.
  int is_empty (void);
  // True if queue is empty, else false.

  size_t message_bytes (void);
  // Number of total bytes on the queue.

  size_t message_count (void);
  // Number of total messages on the queue.

  // = Flow control routines 

  size_t high_water_mark (void);
  // Get high watermark.
  void high_water_mark (size_t hwm);
  // Set high watermark.
  size_t low_water_mark (void);
  // Get low watermark.
  void low_water_mark (size_t lwm);
  // Set low watermark.

  // = Activation control methods.

  int deactivate (void);
  // Deactivate the queue and wakeup all threads waiting on the queue
  // so they can continue.  No messages are removed from the queue,
  // however.  Any other operations called until the queue is
  // activated again will immediately return -1 with <errno> ==
  // ESHUTDOWN.  Returns WAS_INACTIVE if queue was inactive before the
  // call and WAS_ACTIVE if queue was active before the call.

  int activate (void);
  // Reactivate the queue so that threads can enqueue and dequeue
  // messages again.  Returns WAS_INACTIVE if queue was inactive
  // before the call and WAS_ACTIVE if queue was active before the
  // call.

  virtual int notify (void);
  // This hook is automatically invoked by <enqueue_head>,
  // <enqueue_tail>, and <enqueue_prio> when a new item is inserted
  // into the queue.  Subclasses can override this method to perform
  // specific notification strategies (e.g., signaling events for a
  // <ReactorEx>, notifying a <Reactor>, etc.).  In a multi-threaded
  // application with concurrent consumers, there is no guarantee that
  // the queue will be still be non-empty by the time the notification
  // occurs.

  // = Get/set the notification strategy for the <Message_Queue>
  ACE_Notification_Strategy *notification_strategy (void);
  void notification_strategy (ACE_Notification_Strategy *s);

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  // = Routines that actually do the enqueueing and dequeueing.
  // These routines assume that locks are held by the corresponding
  // public methods.

  int enqueue_i (ACE_Message_Block *new_item);
  // Enqueue an <ACE_Message_Block *> in accordance with its priority.

  int enqueue_tail_i (ACE_Message_Block *new_item);
  // Enqueue an <ACE_Message_Block *> at the end of the queue.

  int enqueue_head_i (ACE_Message_Block *new_item);
  // Enqueue an <ACE_Message_Block *> at the head of the queue.

  int dequeue_head_i (ACE_Message_Block *&first_item);
  // Dequeue and return the <ACE_Message_Block *> at the head of the
  // queue.

  // = Check the boundary conditions (assumes locks are held).
  int is_full_i (void);
  // True if queue is full, else false.
  int is_empty_i (void);
  // True if queue is empty, else false.

  // = Implementation of the public activate() and deactivate() methods above (assumes locks are held).
  int deactivate_i (void);
  // Deactivate the queue.
  int activate_i (void);
  // Activate the queue.

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
  ACE_SYNCH_MUTEX lock_;
  // Protect queue from concurrent access.

  ACE_SYNCH_CONDITION notempty_cond_;
  // Used to make threads sleep until the queue is no longer empty.

  ACE_SYNCH_CONDITION notfull_cond_;
  // Used to make threads sleep until the queue is no longer full.
};

template <ACE_SYNCH_1>
class ACE_Message_Queue_Iterator
  // = TITLE
  //     Iterator for the <ACE_Message_Queue>.
{
public:
  // = Initialization method.
  ACE_Message_Queue_Iterator (ACE_Message_Queue <ACE_SYNCH_2> &queue);

  // = Iteration methods.
  int next (ACE_Message_Block *&entry);
  // Pass back the <entry> that hasn't been seen in the queue.
  // Returns 0 when all items have been seen, else 1.

  int advance (void);
  // Move forward by one element in the queue

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  ACE_Message_Queue <ACE_SYNCH_2> &queue_;
  // Message_Queue we are iterating over.

  ACE_Message_Block *curr_;           
  // Keeps track of how far we've advanced...
};

template <ACE_SYNCH_1>
class ACE_Message_Queue_Reverse_Iterator
  // = TITLE
  //     Reverse Iterator for the <ACE_Message_Queue>.
{
public:
  // = Initialization method.
  ACE_Message_Queue_Reverse_Iterator (ACE_Message_Queue <ACE_SYNCH_2> &queue);

  // = Iteration methods.
  int next (ACE_Message_Block *&entry);
  // Pass back the <entry> that hasn't been seen in the queue.
  // Returns 0 when all items have been seen, else 1.

  int advance (void);
  // Move forward by one element in the queue

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  ACE_Message_Queue <ACE_SYNCH_2> &queue_;
  // Message_Queue we are iterating over.

  ACE_Message_Block *curr_;           
  // Keeps track of how far we've advanced...
};

// This must go here to avoid problems with circular includes.
#include "ace/Strategies.h"

#if defined (__ACE_INLINE__)
#include "ace/Message_Queue.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Message_Queue.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Message_Queue.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* ACE_MESSAGE_LIST_H */
