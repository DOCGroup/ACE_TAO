// $Id$

#include "tao/CSD_ThreadPool/CSD_TP_Queue.h"
#include "tao/CSD_ThreadPool/CSD_TP_Request.h"
#include "tao/CSD_ThreadPool/CSD_TP_Queue_Visitor.h"

#if !defined (__ACE_INLINE__)
# include "tao/CSD_ThreadPool/CSD_TP_Queue.inl"
#endif /* ! __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

void
TAO::CSD::TP_Queue::put(TP_Request* request)
{
  // The request is passed in as an "in" argument, and we would like to
  // hold on to a "copy" within the queue (the linked list).  We will
  // perform an _add_ref() on the request now to make the queue's "copy".
  request->_add_ref();

  if (this->tail_ == 0)
    {
      // The tail_ is a NULL pointer only when the queue is empty.
      // Make the request be the only element in the queue.
      this->head_ = this->tail_ = request;

      // Make sure the request's prev_ and next_ pointers are set to NULL.
      request->prev_ = request->next_ = 0;
    }
  else
    {
      // There is at least one request already in the queue.  "Append" the
      // supplied request object to the end of the queue.
      request->prev_ = this->tail_;
      request->next_ = 0;
      this->tail_->next_ = request;
      this->tail_ = request;
    }
}


void
TAO::CSD::TP_Queue::accept_visitor(TP_Queue_Visitor& visitor)
{
  TP_Request* cur = this->head_;

  while (cur != 0)
    {
      TP_Request* prev = cur->prev_;
      TP_Request* next = cur->next_;

      // Pass the current request to the visitor.  Also pass-in a reference
      // to the remove_from_queue flag.  The visitor may decide that it
      // wants to keep the current request for itself, and desires that the
      // request be (surgically) removed from the queue.  The visitor also
      // gets to decide, via its return value, whether or not visitation
      // should continue (or cease to continue).
      bool remove_from_queue = false;

      bool continue_visitation = visitor.visit_request(cur,remove_from_queue);

      if (remove_from_queue)
        {
          // Create a local handle to release the current request once
          // the handle falls out of scope.  We need to do this because the
          // queue "owns" a "copy" of each request in the queue.
          TP_Request_Handle handle = cur;

          if (this->head_ == cur)
            {
              // The current request is at the front (the head_) of the queue.

              // Move the head_ to the next request in the queue.
              this->head_ = next;

              if (this->head_ == 0)
                {
                  // Not only was the current request at the front of the
                  // queue - it was the *only* request in the queue.
                  // Update the tail_ pointer now that the queue is empty.
                  this->tail_ = 0;
                }
              else
                {
                  // Set the (new) head_ request's prev_ pointer to be NULL.
                  this->head_->prev_ = 0;
                }
            }
          else if (this->tail_ == cur)
            {
              // The current request is not at the front of the queue,
              // but it is at the back of the queue.  This implies that
              // the queue currently contains at least two requests -
              // the current request (cur), and the previous request (prev).
              // The point is that we can now assume that the 'prev' pointer
              // is never NULL in this case.
              this->tail_ = prev;
              this->tail_->next_ = 0;
            }
          else
            {
              // The current request is not at the front or at the back.
              // This implies that there are at least three requests in
              // the queue.  We can assume that the 'next' and 'prev'
              // pointers are never NULL in this case.
              prev->next_ = next;
              next->prev_ = prev;
            }
        }

      if (!continue_visitation)
        {
          // The visitor doesn't want to procede with any further visitation.
          // Break out of the visitation loop now.
          break;
        }

      // Move on to the next request in the queue.
      cur = next;
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL
