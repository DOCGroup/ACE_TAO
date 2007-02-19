#ifndef ACE_NOTIFICATION_QUEUE_H
#define ACE_NOTIFICATION_QUEUE_H

#include /**/ "ace/pre.h"

#include "ace/Event_Handler.h"

/**
 *  @file Notification_Queue.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan <coryan@atdesk.com>
 */
#include "ace/Copy_Disabled.h"
#include "ace/Unbounded_Queue.h"

/**
 * @class ACE_Notification_Queue
 *
 * @brief Implements a user-space queue to send Reactor notifications.
 *
 * The ACE_Reactor uses a pipe to send wake up the thread running the
 * event loop from other threads.  This pipe can be limited in size
 * under some operating systems.  For some applications, this limit
 * presents a problem.  A user-space notification queue is used to
 * overcome those limitations.  The queue tries to use as few
 * resources on the pipe as possible, while keeping all the data in
 * user space.
 *
 * This code was refactored from Select_Reactor_Base.
 */
class ACE_Notification_Queue : private ACE_Copy_Disabled
{
public:
  ACE_Notification_Queue();
  ~ACE_Notification_Queue();

  /**
   * @brief Pre-allocate resources in the queue
   */
  int open();

  /**
   * @brief Release all resources in the queue
   */
  void reset();

  /**
   * @brief Remove all elements in the queue matching @c eh and @c mask
   *
   * I suggest reading the documentation in ACE_Reactor to find a more
   * detailed description.  This is just a helper function.
   */
  int purge_pending_notifications(ACE_Event_Handler * eh,
				  ACE_Reactor_Mask mask);

  /**
   * @brief Add a new notification to the queue
   *
   * @return -1 on failure, 1 if a new message should be sent through
   * the pipe and 0 otherwise.
   */
  int push_new_notification(ACE_Notification_Buffer const & buffer);

  /**
   * @brief Extract the next notification from the queue
   *
   * @return -1 on failure, 1 if a message was popped, 0 otherwise
   */
  int pop_next_notification(
      ACE_Notification_Buffer & current,
      bool & more_messages_queued,
      ACE_Notification_Buffer & next);

private:
  /**
   * @brief Allocate more memory for the queue
   */
  int allocate_more_buffers();

private:
  /// Keeps track of allocated arrays of type
  /// ACE_Notification_Buffer.  The idea is to amortize allocation
  /// costs by allocating multiple ACE_Notification_Buffer objects at
  /// a time.
  ACE_Unbounded_Queue <ACE_Notification_Buffer *> alloc_queue_;

  /// Keeps track of all pending notifications.
  ACE_Unbounded_Queue <ACE_Notification_Buffer *> notify_queue_;

  /// Keeps track of all free buffers.
  ACE_Unbounded_Queue <ACE_Notification_Buffer *> free_queue_;

  /// Synchronization for handling of queues.
  ACE_SYNCH_MUTEX notify_queue_lock_;
};

#include /**/ "ace/post.h"

#endif /* ACE_NOTIFICATION_QUEUE_H */
