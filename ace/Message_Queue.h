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

#if !defined (ACE_MESSAGE_QUEUE_H)
#define ACE_MESSAGE_QUEUE_H

#include "ace/Message_Block.h"
#include "ace/IO_Cntl_Msg.h"

// Forward decls.
class ACE_Notification_Strategy;
template <ACE_SYNCH_DECL> class ACE_Message_Queue_Iterator;
template <ACE_SYNCH_DECL> class ACE_Message_Queue_Reverse_Iterator;

class ACE_Export ACE_Message_Queue_Base
{
  // = TITLE
  //   Workaround HP/C++ compiler bug with enums in templates.
  //
  // = DESCRIPTION
  //   The ever lamest HP/C++ compiler seems to fail if enums are
  //   defined inside a template, hence we have to move them into a
  //   base class.
public:
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
};

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
  //     ACE_MT_SYNCH then all operations are thread-safe.  Otherwise,
  //     if it's <ACE_NULL_SYNCH> then there's no locking overhead.
public:
  friend class ACE_Message_Queue_Iterator<ACE_SYNCH_USE>;
  friend class ACE_Message_Queue_Reverse_Iterator<ACE_SYNCH_USE>;

  // = Traits
  typedef ACE_Message_Queue_Iterator<ACE_SYNCH_USE> ITERATOR;
  typedef ACE_Message_Queue_Reverse_Iterator<ACE_SYNCH_USE> REVERSE_ITERATOR;

  // = Initialization and termination methods.
  ACE_Message_Queue (size_t hwm = DEFAULT_HWM,
		     size_t lwm = DEFAULT_LWM,
		     ACE_Notification_Strategy * = 0);

  // Create a message queue with all the defaults.
  virtual int open (size_t hwm = DEFAULT_HWM,
                    size_t lwm = DEFAULT_LWM,
                    ACE_Notification_Strategy * = 0);
  // Create a message queue with all the defaults.

  virtual int close (void);
  // Close down the message queue and release all resources.

  virtual ~ACE_Message_Queue (void);
  // Close down the message queue and release all resources.

  // = Enqueue and dequeue methods.

  // For all the following routines if <timeout> == 0, the caller will
  // block until action is possible, else will wait until the absolute
  // time specified in *<timeout> elapses).  These calls will return,
  // however, when queue is closed, deactivated, when a signal occurs,
  // or if the time specified in timeout elapses, (in which case errno
  // = EWOULDBLOCK).

  virtual int peek_dequeue_head (ACE_Message_Block *&first_item,
                                 ACE_Time_Value *tv = 0);
  // Retrieve the first <ACE_Message_Block> without removing it.
  // Returns -1 on failure, else the number of items still on the
  // queue.

  virtual int enqueue_prio (ACE_Message_Block *new_item,
                            ACE_Time_Value *timeout = 0);
  // Enqueue an <ACE_Message_Block *> into the <Message_Queue> in
  // accordance with its <msg_priority> (0 is lowest priority).  FIFO
  // order is maintained when messages of the same priority are
  // inserted consecutively.  Returns -1 on failure, else the number
  // of items still on the queue.

  virtual int enqueue (ACE_Message_Block *new_item, ACE_Time_Value *timeout = 0);
  // This is an alias for <enqueue_prio>.  It's only here for
  // backwards compatibility and will go away in a subsequent release.
  // Please use <enqueue_prio> instead.

  virtual int enqueue_tail (ACE_Message_Block *new_item, ACE_Time_Value *timeout = 0);
  // Enqueue an <ACE_Message_Block *> at the end of the queue.
  // Returns -1 on failure, else the number of items still on the
  // queue.

  virtual int enqueue_head (ACE_Message_Block *new_item, ACE_Time_Value *timeout = 0);
  // Enqueue an <ACE_Message_Block *> at the head of the queue.
  // Returns -1 on failure, else the number of items still on the
  // queue.

  virtual int dequeue_head (ACE_Message_Block *&first_item, ACE_Time_Value *timeout = 0);
  // Dequeue and return the <ACE_Message_Block *> at the head of the
  // queue.  Returns -1 on failure, else the number of items still on
  // the queue.

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

  // = Flow control routines
  virtual size_t high_water_mark (void);
  // Get high watermark.
  virtual void high_water_mark (size_t hwm);
  // Set high watermark.
  virtual size_t low_water_mark (void);
  // Get low watermark.
  virtual void low_water_mark (size_t lwm);
  // Set low watermark.

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

  void dump (void) const;
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
                                  ACE_Time_Value *tv);
  // Wait for the queue to become non-full.

  virtual int wait_not_empty_cond (ACE_Guard<ACE_SYNCH_MUTEX_T> &mon,
                                   ACE_Time_Value *tv);
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

class ACE_Export ACE_Dynamic_Message_Strategy
{
  // = TITLE
  //     An abstract base class which provides dynamic priority evaluation
  //     methods for use by the ACE_Dynamic_Message_Queue class
  //     or any other class which needs to manage the priorities
  //     of a collection of ACE_Message_Blocks dynamically 
  //
  // = DESCRIPTION
  //     Methods for deadline and laxity based priority evaluation
  //     are provided.  These methods assume a specific partitioning
  //     of the message priority number into a higher order dynamic
  //     bit field and a lower order static priority bit field.  The
  //     default partitioning assumes an unsigned dynamic message
  //     priority field of 22 bits and an unsigned static message
  //     priority field of 10 bits.  This corresponds to the initial
  //     values of the static class members.  To provide a different
  //     partitioning, assign a different set of values to the static
  //     class memebers before using the static member functions.  
public:

  ACE_Dynamic_Message_Strategy (u_long static_bit_field_mask,
                                u_long static_bit_field_shift,
                                u_long pending_threshold,
                                u_long dynamic_priority_max,
                                u_long dynamic_priority_offset);
  // ctor
  
  virtual ~ACE_Dynamic_Message_Strategy ();
  // virtual dtor

  virtual int update_priority (ACE_Message_Block & mb, 
                               const ACE_Time_Value & tv) = 0;
  // abstract dynamic priority evaluation function:
  // updates the synamic priority bit field but does not
  // alter the static priority bit field

  int is_pending (const ACE_Message_Block & mb,     
                  const ACE_Time_Value & tv);
  // returns true if the message has a pending (not late) priority value

  virtual int is_beyond_late (const ACE_Message_Block & mb, 
                              const ACE_Time_Value & tv) = 0;
  // returns true if the message is later than can can be represented

  u_long static_bit_field_mask (void);
  // get static bit field mask

  void static_bit_field_mask (u_long);
  // set static bit field mask

  u_long static_bit_field_shift (void);
  // get left shift value to make room for static bit field

  void static_bit_field_shift (u_long);
  // set left shift value to make room for static bit field

  u_long pending_threshold (void);
  // get pending threshold priority value

  void pending_threshold (u_long);
  // set pending threshold priority value

  u_long dynamic_priority_max (void);
  // get maximum supported priority value

  void dynamic_priority_max (u_long);
  // set maximum supported priority value

  u_long dynamic_priority_offset (void);
  // get axis shift to map signed range into unsigned range

  void dynamic_priority_offset (u_long);
  // set axis shift to map signed range into unsigned range

protected:

  u_long static_bit_field_mask_;
  // this is a bit mask with all ones in the static bit field

  u_long static_bit_field_shift_;
  // this is a left shift value to make room for static bit 
  // field: this value should be the logarithm base 2 of
  // (static_bit_field_mask_ + 1)

  u_long pending_threshold_;
  // threshold priority value below which a message is considered late

  u_long dynamic_priority_max_;
  // maximum supported priority value

  u_long dynamic_priority_offset_;
  // axis shift added to all values, in order to map signed 
  // range into unsigned range (priority is an unsigned value).
};

class ACE_Export ACE_Deadline_Message_Strategy : public ACE_Dynamic_Message_Strategy
{
public:

  ACE_Deadline_Message_Strategy (u_long static_bit_field_mask = 0x3FFUL,        // 2^(10) - 1
                                 u_long static_bit_field_shift = 10,            // 10 low order bits
                                 u_long pending_threshold = 0x200000UL,         // 2^(22-1)
                                 u_long dynamic_priority_max = 0x3FFFFFUL,      // 2^(22)-1
                                 u_long dynamic_priority_offset =  0x200000UL); // 2^(22-1)
  // ctor, with all arguments defaulted

  virtual ~ACE_Deadline_Message_Strategy ();
  // virtual dtor

  virtual int update_priority (ACE_Message_Block & mb, 
                            const ACE_Time_Value & tv);
  // dynamic priority evaluation function based on time to
  // deadline: updates the synamic priority bit field but
  // does not alter the static priority bit field

  int is_beyond_late (const ACE_Message_Block & mb,
                      const ACE_Time_Value & tv);
  // returns true if the message is later than can can be represented  
};

class ACE_Export ACE_Laxity_Message_Strategy : public ACE_Dynamic_Message_Strategy
{
public:

  ACE_Laxity_Message_Strategy (u_long static_bit_field_mask = 0x3FFUL,        // 2^(10) - 1
                               u_long static_bit_field_shift = 10,            // 10 low order bits
                               u_long pending_threshold = 0x200000UL,         // 2^(22-1)
                               u_long dynamic_priority_max = 0x3FFFFFUL,      // 2^(22)-1
                               u_long dynamic_priority_offset =  0x200000UL); // 2^(22-1)
  // ctor, with all arguments defaulted

  virtual ~ACE_Laxity_Message_Strategy ();
  // virtual dtor

  virtual int update_priority (ACE_Message_Block & mb, 
                            const ACE_Time_Value & tv);
  // dynamic priority evaluation function based on time to
  // deadline: updates the dynamic priority bit field but
  // does not alter the static priority bit field
  
  int is_beyond_late (const ACE_Message_Block & mb,
                      const ACE_Time_Value & tv);
  // returns true if the message is later than can can be represented  
};


template <ACE_SYNCH_DECL>
class ACE_Dynamic_Message_Queue : public ACE_Message_Queue<ACE_SYNCH_USE>
{
  // = TITLE
  //     A derived class which adapts the ACE_Message_Queue
  //     class in order to maintain dynamic priorities for enqueued 
  //     ACE_Message_Blocks and manage the queue dynamically.  
  //
  // = DESCRIPTION
  //     Priorities and queue orderings are refreshed at each enqueue and 
  //     dequeue operation.  Head and tail enqueue methods were made private
  //     to prevent out-of-order messages from confusing the pending 
  //     and late portions of the queue.  Messages in the pending portion of
  //     the queue whose dynamic priority becomes negative are placed into
  //     the late portion of the queue.  Messages in the late portion of
  //     the queue whose dynamic priority becomes positive are dropped.
  //     These behaviors support a limited schedule overrun corresponding
  //     to one full cycle through dynamic priority values.  These behaviors 
  //     can be modified in derived classes by providing alternative 
  //     definitions for the appropriate virtual methods.
  //
public:

  // = Initialization and termination methods.
  ACE_Dynamic_Message_Queue (ACE_Dynamic_Message_Strategy & message_strategy,
                             size_t hwm = DEFAULT_HWM,
                             size_t lwm = DEFAULT_LWM,
                             ACE_Notification_Strategy * = 0);

  virtual ~ACE_Dynamic_Message_Queue (void);
  // Close down the message queue and release all resources.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:

  virtual int enqueue_i (ACE_Message_Block *new_item);
  // Enqueue an <ACE_Message_Block *> in accordance with its priority.
  // priority may be *dynamic* or *static* or a combination or *both*
  // It calls the priority evaluation function passed into the Dynamic
  // Message Queue constructor to update the priorities of all enqueued 
  // messages.

  virtual int dequeue_head_i (ACE_Message_Block *&first_item);
  // Dequeue and return the <ACE_Message_Block *> at the head of the
  // queue.

  virtual int refresh_priorities (const ACE_Time_Value & tv);
  // refresh the priorities in the queue according
  // to a specific priority assignment function

  virtual int refresh_queue (const ACE_Time_Value & tv);
  // refresh the order of messages in the queue
  // after refreshing their priorities

  ACE_Message_Block *pending_list_tail_;
  // Pointer to tail of the pending messages (those whose priority is 
  // and has been non-negative) in the ACE_Message_Block list.

  ACE_Dynamic_Message_Strategy & message_strategy_;
  // Pointer to a dynamic priority evaluation function

private:

  // = Disallow these operations.
  ACE_UNIMPLEMENTED_FUNC (void operator= (const ACE_Message_Queue<ACE_SYNCH_USE> &))
  ACE_UNIMPLEMENTED_FUNC (ACE_Dynamic_Message_Queue (const ACE_Dynamic_Message_Queue<ACE_SYNCH_USE> &))

  // External invocation of these could cause extreme wierdness
  // in a dynamically prioritized queue: disallow their use until 
  // and unless a coherent semantics for head and tail enqueueing
  // can be identified.
  ACE_UNIMPLEMENTED_FUNC (virtual int 
                          enqueue_tail (ACE_Message_Block *new_item, 
                                        ACE_Time_Value *timeout = 0))
  ACE_UNIMPLEMENTED_FUNC (virtual int 
                          enqueue_head (ACE_Message_Block *new_item, 
                                        ACE_Time_Value *timeout = 0))

  // As messages are *dynamically* prioritized, it is not possible to
  // guarantee that the message at the head of the queue when this 
  // method is called will still be at the head when the next method
  // is called: disallow its use until and unless a coherent semantics
  // for peeking at the head of the queue can be identified.
  ACE_UNIMPLEMENTED_FUNC (virtual int 
                          peek_dequeue_head (ACE_Message_Block *&first_item,
                                             ACE_Time_Value *tv = 0))
};


template <ACE_SYNCH_DECL>
class ACE_Export ACE_Message_Queue_Factory
{
public:

  static ACE_Message_Queue<ACE_SYNCH_USE> * 
    create_static_message_queue (size_t hwm = DEFAULT_HWM,
                                 size_t lwm = DEFAULT_LWM,
                                 ACE_Notification_Strategy * = 0);

  static ACE_Dynamic_Message_Queue<ACE_SYNCH_USE> * 
    create_deadline_message_queue (size_t hwm = DEFAULT_HWM,
                                   size_t lwm = DEFAULT_LWM,
                                   ACE_Notification_Strategy * = 0,
                                   u_long static_bit_field_mask = 0x3FFUL,        // 2^(10) - 1
                                   u_long static_bit_field_shift = 10,            // 10 low order bits
                                   u_long pending_threshold = 0x200000UL,         // 2^(22-1)
                                   u_long dynamic_priority_max = 0x3FFFFFUL,      // 2^(22)-1
                                   u_long dynamic_priority_offset =  0x200000UL); // 2^(22-1)

  static ACE_Dynamic_Message_Queue<ACE_SYNCH_USE> * 
    create_laxity_message_queue (size_t hwm = DEFAULT_HWM, 
                                 size_t lwm = DEFAULT_LWM,
                                 ACE_Notification_Strategy * = 0,
                                 u_long static_bit_field_mask = 0x3FFUL,        // 2^(10) - 1
                                 u_long static_bit_field_shift = 10,            // 10 low order bits
                                 u_long pending_threshold = 0x200000UL,         // 2^(22-1)
                                 u_long dynamic_priority_max = 0x3FFFFFUL,      // 2^(22)-1
                                 u_long dynamic_priority_offset =  0x200000UL); // 2^(22-1)
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

#endif /* ACE_MESSAGE_QUEUE_H */
