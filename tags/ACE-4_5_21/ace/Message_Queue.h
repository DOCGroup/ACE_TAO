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

// Include the templates here.
#include "ace/Message_Queue_T.h"

#if defined (VXWORKS)
# include /**/ <msgQLib.h>

class ACE_Message_Queue_Vx : public ACE_Message_Queue<ACE_NULL_SYNCH>
{
  // = TITLE
  //     Wrapper for VxWorks message queues.
  //
  // = DESCRIPTION
  //     Specialization of ACE_Message_Queue to simply wrap VxWorks
  //     MsgQ.  It does not use any synchronization, because it relies
  //     on the native MsgQ implementation to take care of that.  The
  //     only system calls that it uses are VxWorks msgQLib calls, so
  //     it is suitable for use in iterrupt service routines.
  //
  //     NOTE: *Many* ACE_Message_Queue features are not supported with
  //     this specialization, including:
  //     * The two size arguments to the constructor and open () are
  //       interpreted differently.  The first is interpreted as the
  //       maximum number of bytes in a message.  The second is
  //       interpreted as the maximum number of messages that can be
  //       queued.
  //     * dequeue_head () *requires* that the ACE_Message_Block
  //       pointer argument point to an ACE_Message_Block that was
  //       allocated by the caller.  It must be big enough to support
  //       the received message, without using continutation.  The
  //       pointer argument is not modified.
  //     * Message priority.  MSG_Q_FIFO is hard-coded.
  //     * enqueue method timeouts.
  //     * peek_dequeue_head ().
  //     * ACE_Message_Queue_Iterators.
  //     * The ability to change low and high water marks after creation.
  //     * Message_Block chains.  The continuation field of ACE_Message_Block
  //     *   is ignored; only the first block of a fragment chain is
  //     *   recognized.
public:
  // = Initialization and termination methods.
  ACE_Message_Queue_Vx (size_t max_messages,
                        size_t max_message_length,
                        ACE_Notification_Strategy * = 0);

  // Create a message queue with all the defaults.
  virtual int open (size_t max_messages,
                    size_t max_message_length,
                    ACE_Notification_Strategy * = 0);
  // Create a message queue with all the defaults.

  virtual int close (void);
  // Close down the message queue and release all resources.

  virtual ~ACE_Message_Queue_Vx (void);
  // Close down the message queue and release all resources.

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

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
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
  virtual int wait_not_full_cond (ACE_Guard<ACE_Null_Mutex> &mon,
                                  ACE_Time_Value *tv);
  // Wait for the queue to become non-full.

  virtual int wait_not_empty_cond (ACE_Guard<ACE_Null_Mutex> &mon,
                                   ACE_Time_Value *tv);
  // Wait for the queue to become non-empty.

  virtual int signal_enqueue_waiters (void);
  // Inform any threads waiting to enqueue that they can procede.

  virtual int signal_dequeue_waiters (void);
  // Inform any threads waiting to dequeue that they can procede.

  MSG_Q_ID msgq ();
  // Access the underlying msgQ.

private:
  int max_messages_;
  // Maximum number of messages that can be queued.

  int max_message_length_;
  // Maximum message size, in bytes.

  int options_;
  // Native message queue options.

  // = Disallow these operations.
  ACE_UNIMPLEMENTED_FUNC (void operator= (const ACE_Message_Queue_Vx &))
  ACE_UNIMPLEMENTED_FUNC (ACE_Message_Queue_Vx (const ACE_Message_Queue_Vx &))

  ACE_UNIMPLEMENTED_FUNC (virtual int peek_dequeue_head
                            (ACE_Message_Block *&first_item,
                             ACE_Time_Value *tv = 0))
};
#endif /* VXWORKS */

// This must go here to avoid problems with circular includes.
#include "ace/Strategies.h"

#if defined (__ACE_INLINE__)
#include "ace/Message_Queue.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_MESSAGE_QUEUE_H */
