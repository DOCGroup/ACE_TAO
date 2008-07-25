// $Id$

#include "ace/MT_Priority_Reactor.h"
#include "ace/Thread.h"
#include "ace/Timer_Queue.h"
#include "ace/Sig_Handler.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_sys_time.h"

#include <iostream> // for debugging

#if !defined (__ACE_INLINE__)
#include "ace/MT_Priority_Reactor.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID (ace,
           MT_Priority_Reactor,
           "$Id$")

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_ALLOC_HOOK_DEFINE (ACE_MT_Priority_Reactor)

ACE_MT_Priority_Reactor::ACE_MT_Priority_Reactor (ACE_Sig_Handler *sh,
                                                  ACE_Timer_Queue *tq,
                                                  int mask_signals,
                                                  int s_queue)
: ACE_TP_Reactor (sh, tq, mask_signals, s_queue)
   , _bucket(0)
   , _bucketize_socket_events(false)
   , _current_priority(ACE_Event_Handler::HI_PRIORITY)
   , _max_priority(0)
   , _min_priority(0)
   , _num_socket_events(0)
   , _tuple_allocator(0)
{
   ACE_TRACE ("ACE_MT_Priority_Reactor::ACE_MT_Priority_Reactor");
   this->init_bucket();
}

ACE_MT_Priority_Reactor::ACE_MT_Priority_Reactor (size_t max_number_of_handles,
                                                  int restart,
                                                  ACE_Sig_Handler *sh,
                                                  ACE_Timer_Queue *tq,
                                                  int mask_signals,
                                                  int s_queue)
  : ACE_TP_Reactor (max_number_of_handles, restart, sh, tq, mask_signals, s_queue)
  , _bucket(0)
  , _current_priority(ACE_Event_Handler::HI_PRIORITY)
  , _max_priority(0)
  , _min_priority(0)
  , _num_socket_events(0)
  , _tuple_allocator(0)
{
  ACE_TRACE ("ACE_MT_Priority_Reactor::ACE_MT_Priority_Reactor");
  this->init_bucket();
}

ACE_MT_Priority_Reactor::~ACE_MT_Priority_Reactor()
{
   ACE_TRACE ("ACE_MT_Priority_Reactor::~ACE_MT_Priority_Reactor");
   for (int index = 0; index < npriorities; ++index)
   {
      delete _bucket[index];
   }

   delete [] _bucket;
   delete _tuple_allocator;
}

int
ACE_MT_Priority_Reactor::handle_events (ACE_Time_Value &max_wait_time)
{
   return this->handle_events (&max_wait_time);
}

int
ACE_MT_Priority_Reactor::handle_events (ACE_Time_Value *max_wait_time)
{
  ACE_TRACE ("ACE_MT_Priority_Reactor::handle_events");

  // Stash the current time -- the destructor of this object will
  // automatically compute how much time elapsed since this method was
  // called.
  ACE_Countdown_Time countdown (max_wait_time);


  // Instantiate the token guard which will try grabbing the token for
  // this thread.
  ACE_TP_Token_Guard guard (this->token_);

  int const result = guard.acquire_read_token (max_wait_time);

  // If the guard is NOT the owner just return the retval
  if (!guard.is_owner ())
    return result;

  // After getting the lock just just for deactivation.
  if (this->deactivated_)
    return -1;

  // Update the countdown to reflect time waiting for the token.
  countdown.update ();

  return this->dispatch_handler (max_wait_time,
                                guard);
}

int
ACE_MT_Priority_Reactor::dispatch_handler (ACE_Time_Value *max_wait_time,
                                          ACE_TP_Token_Guard &guard)
{
  int num_events = 
    this->get_an_event_for_dispatching (max_wait_time);

  int initial_event_count = num_events;
  int result = 0;

  result = this->handle_timer (num_events,
                               guard);

  // If we've dispatched a timer event handler, return the thread
  if (result > 0)
     return result;

  // Else just go ahead fall through for further handling.
  if (num_events > 0)
  {
     // Next dispatch the notification handlers (if there are any to
     // dispatch).  These are required to handle multiple-threads
     // that are trying to update the <Reactor>.
     result = this->handle_notify (_num_socket_events,
                                   guard);
     
     if (result > 0)
        return result;

     // Else just fall through for further handling
  }
  
  if (num_events > 0)
  {
     // Sort events into priority buckets
     if (_bucketize_socket_events)
     {
        bucketize_socket_events();
     }
     
     // Handle the highest priority event
     result = this->handle_socket(num_events,
                                   guard);
    }

  return result;
}

int
ACE_MT_Priority_Reactor::get_an_event_for_dispatching (ACE_Time_Value *max_wait_time)
{
   // If the reactor handler state has changed, clear any remembered
   // ready bits and re-scan from the master wait_set.
   if (this->state_changed_)
   {
      this->ready_set_.rd_mask_.reset ();
      this->ready_set_.wr_mask_.reset ();
      this->ready_set_.ex_mask_.reset ();
      
      this->state_changed_ = false;
   }
   else
   {
      // This is a hack... somewhere, under certain conditions (which
      // I don't understand...) the mask will have all of its bits clear,
      // yet have a size_ > 0. This is an attempt to remedy the affect,
      // without knowing why it happens.
      
      this->ready_set_.rd_mask_.sync (this->ready_set_.rd_mask_.max_set ());
      this->ready_set_.wr_mask_.sync (this->ready_set_.wr_mask_.max_set ());
      this->ready_set_.ex_mask_.sync (this->ready_set_.ex_mask_.max_set ());
   }
   
   if (_num_socket_events > 0)
      return _num_socket_events;
   
   else
   {
      int num_events = wait_for_multiple_events (this->ready_set_,
                                                 max_wait_time);
      
      _bucketize_socket_events = true;
      
      if (num_events > 0)
         preprocess_new_event_set();
      
      return num_events;
   }
}

int
ACE_MT_Priority_Reactor::handle_timer(int & /*event_count*/,
                                      ACE_TP_Token_Guard &guard)
{
   if (this->timer_queue_ == 0 || this->timer_queue_->is_empty())
   { // Empty timer queue so cannot have any expired timers.
      return 0;
   }
   
   // Get the current time
   ACE_Time_Value cur_time (this->timer_queue_->gettimeofday () +
                            this->timer_queue_->timer_skew ());
   
  // Look for a node in the timer queue whose timer <= the present
  // time.
   ACE_Timer_Node_Dispatch_Info info;
   
   if (this->timer_queue_->dispatch_info (cur_time,
                                          info))
   {
      const void *upcall_act = 0;
      
      // Preinvoke.
      this->timer_queue_->preinvoke (info,
                                     cur_time,
                                     upcall_act);
      
      // Release the token before dispatching notifies...
      guard.release_token ();

      // call the functor
      this->timer_queue_->upcall (info,
                                  cur_time);
      
      // Postinvoke
      this->timer_queue_->postinvoke (info,
                                      cur_time,
                                      upcall_act);
      
      // We have dispatched a timer
      return 1;
   }

   return 0;
}

int
ACE_MT_Priority_Reactor::handle_notify(int & /*event_count*/,
                                       ACE_TP_Token_Guard &guard)
{

   // Get the handle on which notify calls could have occured
   ACE_HANDLE notify_handle =
      this->get_the_notify_handle ();
   
   int result = 0;
   
   // The notify was not in the list returned by
   // wait_for_multiple_events ().
   if (notify_handle == ACE_INVALID_HANDLE)
      return result;
   
   // Now just do a read on the pipe..
   ACE_Notification_Buffer buffer;
   
   // Clear the handle of the read_mask of our <ready_set_>
   this->ready_set_.rd_mask_.clr_bit (notify_handle);
   
   // Keep reading notifies till we empty it or till we have a
   // dispatchable buffer
   while (this->notify_handler_->read_notify_pipe (notify_handle,
                                                   buffer) > 0)
   {
      // Just figure out whether we can read any buffer that has
      // dispatchable info. If not we have just been unblocked by
      // another thread trying to update the reactor. If we get any
      // buffer that needs dispatching we will dispatch that after
      // releasing the lock
      if (this->notify_handler_->is_dispatchable (buffer) > 0)
      {
         // Release the token before dispatching notifies...
         guard.release_token ();       
         
         // Dispatch the upcall for the notify
         this->notify_handler_->dispatch_notify (buffer);
         
         // We had a successful dispatch.
         result = 1;
         
         // break out of the while loop
         break;
      }
      
      
   }
   
   // If we did some work, then we just return 1 which will allow us
   // to get out of here. If we return 0, then we will be asked to do
   // some work ie. dispacth socket events
   return result;
}

int
ACE_MT_Priority_Reactor::handle_socket(int &event_count,
                                       ACE_TP_Token_Guard &guard)
{   
   // We got the lock, lets handle some I/O events.
   ACE_Handle_Dispatch_Info dispatch_info;
   
   if (this->get_sock_event_info (dispatch_info) == 0)
   {
      return 0;
   }
   
   // If there is any event handler that is ready to be dispatched, the
   // dispatch information is recorded in dispatch_info.
   if (!dispatch_info.dispatch ())
   {
      // Check for removed handlers.
      if (dispatch_info.event_handler_ == 0)
      {
         this->handler_rep_.unbind(dispatch_info.handle_,
                                   dispatch_info.mask_);
      }
      
      return 0;
   }
   
   // Suspend the handler so that other threads don't start dispatching
   // it, if we can't suspend then return directly
   if (dispatch_info.event_handler_ != this->notify_handler_)
      if (this->suspend_i (dispatch_info.handle_) == -1)
         return 0;
   
   // Call add_reference() if needed.
   if (dispatch_info.reference_counting_required_)
      dispatch_info.event_handler_->add_reference ();
   
   // Do any pre-dispatch processing
   preprocess_chosen_handler(dispatch_info);

   // Release the lock so that another thread can begin dispatching
   guard.release_token ();
   
   int result = 0;
   
   // Dispatched an event
   if (this->dispatch_sock_event (dispatch_info) == 0)
      ++result;
   
   return result;
}

ACE_HANDLE
ACE_MT_Priority_Reactor::get_the_notify_handle (void)
{
   // Call the notify handler to get a handle on which we would have a
   // notify waiting
   ACE_HANDLE const read_handle =
      this->notify_handler_->notify_handle ();
   
   // Check whether the rd_mask has been set on that handle. If so
   // return the handle.
   if (read_handle != ACE_INVALID_HANDLE &&
      this->ready_set_.rd_mask_.is_set (read_handle))
   {
      return read_handle;
   }
   
   // None found..
   return ACE_INVALID_HANDLE;
}

int
ACE_MT_Priority_Reactor::get_sock_event_info (ACE_Handle_Dispatch_Info &event)
{
   int found_io = 0;
   
   // Iterate through buckets to find the highest-priority socket event
   if (_num_socket_events > 0)
   {      
      ACE_Handle_Dispatch_Info event_info;
      _current_priority = ACE_Event_Handler::HI_PRIORITY;
      while (_current_priority >= _min_priority)
      {
         if (!_bucket[_current_priority]->is_empty())
         {
            size_t size = _bucket[_current_priority]->size();
            size_t count = 0;
            while (count < size)
            {
               ++count;
               --_num_socket_events;
               _bucket[_current_priority]->dequeue_head(event_info);
               if (!this->is_suspended_i(event_info.handle_))
               {
                  found_io = 1;
                  event = event_info;
                  break;
               }
            }
            if (found_io)
            {       
               break;
            }
         }  
         else
         {
             // There are no more messages at this priority, so continue
             // to the next lowest priority
             --_current_priority;
         }
      }
   }     
   
   return found_io;
}

void ACE_MT_Priority_Reactor::init_bucket()
{
   // Create a tuple allocator
   ACE_NEW (this->_tuple_allocator,
            TUPLE_ALLOCATOR (ACE_Select_Reactor::DEFAULT_SIZE));
   
   // Create a list of pointers, one per priority level
   ACE_NEW (this->_bucket,
            QUEUE *[npriorities]);
   
   // Create a bucket for each priority level
   for (int i = 0; i < npriorities; ++i)
      ACE_NEW (this->_bucket[i],
               QUEUE (this->_tuple_allocator));
}

// Dispatches a single event handler
int
ACE_MT_Priority_Reactor::dispatch_sock_event (ACE_Handle_Dispatch_Info &dispatch_info)
{
   ACE_TRACE ("ACE_MT_Priority_Reactor::dispatch_socket_event");
   
   ACE_Event_Handler * const event_handler = dispatch_info.event_handler_;
   ACE_EH_PTMF const callback = dispatch_info.callback_;
   
   // Check for removed handlers.
   if (event_handler == 0)
      return -1;

   // Upcall. If the handler returns positive value (requesting a
   // reactor callback) don't set the ready-bit because it will be
   // ignored if the reactor state has changed. Just call back
   // as many times as the handler requests it. Other threads are off
   // handling other things.
   int status = 1;
   
   while (status > 0)
      status = (event_handler->*callback) (dispatch_info.handle_);
   
   // Post process socket event
   return this->post_process_sock_event (dispatch_info, status);
}

int
ACE_MT_Priority_Reactor::post_process_sock_event (ACE_Handle_Dispatch_Info &dispatch_info,
                                                  int status)
{
   int result = 0;
   
   // First check if we really have to post process something, if not, then
   // we don't acquire the token which saves us a lot of time.
   if (status < 0 ||
       (dispatch_info.event_handler_ != this->notify_handler_ &&
        dispatch_info.resume_flag_ ==
        ACE_Event_Handler::ACE_REACTOR_RESUMES_HANDLER))
   {
      // Get the reactor token and with this token acquired remove first the
      // handler and resume it at the same time. This must be atomic, see also
      // bugzilla 2395. When this is not atomic it can be that we resume the
      // handle after it is reused by the OS.
      ACE_TP_Token_Guard guard (this->token_);
      
      result = guard.acquire_token ();
      
      // If the guard is NOT the owner just return the retval
      if (!guard.is_owner ())
         return result;
      
      // A different event handler may have been registered during the
      // upcall if the handle was closed and then reopened, for
      // example.  Make sure we're removing and/or resuming the event
      // handler used during the upcall.
      ACE_Event_Handler const * const eh =
         this->handler_rep_.find (dispatch_info.handle_);
      
      // Only remove or resume the event handler used during the
      // upcall.
      if (eh == dispatch_info.event_handler_)
      {      
         if (status < 0)
         {
            result =
               this->remove_handler_i (dispatch_info.handle_,
                                       dispatch_info.mask_);
         }
         
         // Resume handler if required.
         if (dispatch_info.event_handler_ != this->notify_handler_ &&
             dispatch_info.resume_flag_ ==
             ACE_Event_Handler::ACE_REACTOR_RESUMES_HANDLER)
            this->resume_i (dispatch_info.handle_);
      }
   }
   
   // Call remove_reference() if needed.
   if (dispatch_info.reference_counting_required_)
      dispatch_info.event_handler_->remove_reference ();
   
   return result;
}

void
ACE_MT_Priority_Reactor::notify_handle (ACE_HANDLE,
                                        ACE_Reactor_Mask,
                                        ACE_Handle_Set &,
                                        ACE_Event_Handler *eh,
                                        ACE_EH_PTMF)
{
   ACE_ERROR ((LM_ERROR,
               ACE_LIB_TEXT ("ACE_MT_Priority_Reactor::notify_handle: ")
               ACE_LIB_TEXT ("Wrong version of notify_handle() got called \n")));
   
   ACE_ASSERT (eh == 0);
   ACE_UNUSED_ARG (eh);
}

int ACE_MT_Priority_Reactor::bucketize_socket_events()
{
   int result(0);
   _bucketize_socket_events = false;
   ACE_HANDLE handle;
   
   // Place each active handle in the appropriate priority bucket
   ACE_Handle_Set_Iterator wr_handle_iter (this->ready_set_.wr_mask_);      
   while ((handle = wr_handle_iter ()) != ACE_INVALID_HANDLE)
   {
      ACE_Handle_Dispatch_Info event;
      
      // To avoid dispatching an event whose associated handle
      // is already suspended, don't place that event in the bucket
      if (this->is_suspended_i (handle))
         continue;

      // Create a token containing all information
      // necessary to dispatch the event at the appropriate
      // moment
      event.set (handle,
                 this->handler_rep_.find (handle),
                 ACE_Event_Handler::WRITE_MASK,
                 &ACE_Event_Handler::handle_output);
      
      
      ++_num_socket_events;
      
      // Store the token in the appropriate bucket
      _bucket[event.event_handler_->priority()]->enqueue_tail(event);
      this->ready_set_.wr_mask_.clr_bit (handle);
      this->ready_set_.ex_mask_.clr_bit (handle);
      this->ready_set_.rd_mask_.clr_bit (handle);      
    }
   
   ACE_Handle_Set_Iterator ex_handle_iter (this->ready_set_.ex_mask_);      
   while ((handle = ex_handle_iter ()) != ACE_INVALID_HANDLE)
   {
      ACE_Handle_Dispatch_Info event;
      
      if (this->is_suspended_i (handle))
        continue;
      
      // Remember this info
      event.set (handle,
                 this->handler_rep_.find (handle),
                 ACE_Event_Handler::EXCEPT_MASK,
                 &ACE_Event_Handler::handle_exception);
      
      this->ready_set_.wr_mask_.clr_bit (handle);
      this->ready_set_.ex_mask_.clr_bit (handle);
      this->ready_set_.rd_mask_.clr_bit (handle); 
   }
   
   ACE_Handle_Set_Iterator rd_handle_iter (this->ready_set_.rd_mask_);
   while ((handle = rd_handle_iter ()) != ACE_INVALID_HANDLE)
   {
      ACE_Handle_Dispatch_Info event;
      
      if (this->is_suspended_i (handle))
         continue;
      
      // Remember this info
      event.set (handle,
                 this->handler_rep_.find (handle),
                 ACE_Event_Handler::READ_MASK,
                 &ACE_Event_Handler::handle_input);
      
      ++_num_socket_events;
      _bucket[event.event_handler_->priority()]->enqueue_tail(event);
      this->ready_set_.wr_mask_.clr_bit (handle);
      this->ready_set_.ex_mask_.clr_bit (handle);
      this->ready_set_.rd_mask_.clr_bit (handle);      
   }    
   
   return result;
}

ACE_END_VERSIONED_NAMESPACE_DECL
