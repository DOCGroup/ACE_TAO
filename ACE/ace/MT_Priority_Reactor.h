// -*- C++ -*-

//=============================================================================
/**
 *  @file    MT_Priority_Reactor.h
 *
 *  $Id$
 *
 *  The ACE_MT_Priority_Reactor, like the ACE_TP_Reactor, uses the
 *  Leader/Followers pattern to demultiplex events among a pool of
 *  threads.  When using a thread pool reactor, an application
 *  pre-spawns a _fixed_ number of threads.  When these threads
 *  invoke the ACE_MT_Priority_Reactor's <handle_events> method, one thread
 *  will become the leader and wait for an event.  The other
 *  follower threads will queue up waiting for their turn to become
 *  the leader.  When an event occurs, the leader will pick a
 *  follower to become the leader and go on to handle the event.
 *  The consequence of using ACE_TP_Reactor is the amortization of
 *  the costs used to creating threads.  The context switching cost
 *  will also reduce.  More over, the total resources used by
 *  threads are bounded because there are a fixed number of threads.
 *
 *  The ACE_MT_Priority_Reactor differs from the TP reactor in that it
 *  dispatches socket events in priority order, according to the priorites
 *  specified in each registered event handler
 *
 *  @author John Moore <ljohn7@gmail.com>
 */
//=============================================================================


#ifndef _ACE_MT_PRIORITY_REACTOR_H_
#define _ACE_MT_PRIORITY_REACTOR_H_

#include /**/ "ace/pre.h"

#include "ace/TP_Reactor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

#include "ace/Malloc_T.h"
#include "ace/Unbounded_Queue.h"

#define npriorities   ACE_Event_Handler::HI_PRIORITY-ACE_Event_Handler::LO_PRIORITY+1

/**
 * @class ACE_Handle_Dispatch_Info
 *
 * @brief This structure contains all information for needed to
 *        dispatch the corresponding event handler
 */
class ACE_Handle_Dispatch_Info
{
   public:
      
      /// Default constructor
      ACE_Handle_Dispatch_Info (void);
      
      /// Method for setting
      void set (ACE_HANDLE handle,
                ACE_Event_Handler *event_handler,
                ACE_Reactor_Mask mask,
                ACE_EH_PTMF callback);
      
      /// Dispatch indicates whether or not it's safe to dispatch 
      /// the associated handler
      bool dispatch (void) const;
      
      ACE_HANDLE handle_;
      ACE_Event_Handler *event_handler_;
      ACE_Reactor_Mask mask_;
      ACE_EH_PTMF callback_;
      int resume_flag_;
      bool reference_counting_required_;
      
   private:
      bool dispatch_;
};


typedef ACE_Cached_Allocator
< ACE_Node< ACE_Handle_Dispatch_Info >,
  ACE_SYNCH_NULL_MUTEX > TUPLE_ALLOCATOR;

typedef ACE_Unbounded_Queue_Iterator
< ACE_Handle_Dispatch_Info > QUEUE_ITERATOR;

/**
 * @class ACE_MT_Priority_Reactor
 *
 * @brief Specialization of Select Reactor to support prioritized, thread-pool
 * based event dispatching.
 *
 * The multi-threaded priority reactor combines the benefits of 
 * the thread-pool-based TP_Reactor with the prioritized dispatching of
 * event handlers - a feature found in the single-threaded priority reactor
 */
class ACE_Export ACE_MT_Priority_Reactor : public ACE_TP_Reactor
{
public:
   
   /// Initialize ACE_MT_Priority_Reactor with the default size.
   ACE_MT_Priority_Reactor (ACE_Sig_Handler * = 0,
                            ACE_Timer_Queue * = 0,
                            int mask_signals = 1,
                            int s_queue = ACE_Select_Reactor_Token::FIFO);
   
   /**
    * Initialize the ACE_MT_Priority_Reactor to manage
    * @a max_number_of_handles.  If @a restart is non-0 then the
    * ACE_Reactor's @c handle_events() method will be restarted
    * automatically when @c EINTR occurs.  If @a sh or
    * @a tq are non-0 they are used as the signal handler and
    * timer queue, respectively.
    */
   ACE_MT_Priority_Reactor (size_t max_number_of_handles,
                            int restart = 0,
                            ACE_Sig_Handler *sh = 0,
                            ACE_Timer_Queue *tq = 0,
                            int mask_signals = 1,
                            int s_queue = ACE_Select_Reactor_Token::FIFO);
   

   /**
    *Destructor 
    */
   virtual ~ACE_MT_Priority_Reactor();

   /**
    * This event loop driver that blocks for @a max_wait_time before
    * returning.  It will return earlier if timer events, I/O events,
    * or signal events occur.  Note that @a max_wait_time can be 0, in
    * which case this method blocks indefinitely until events occur.
    *
    * @a max_wait_time is decremented to reflect how much time this call
    * took.  For instance, if a time value of 3 seconds is passed to
    * handle_events and an event occurs after 2 seconds,
    * @a max_wait_time will equal 1 second.  This can be used if an
    * application wishes to handle events for some fixed amount of
    * time.
    *
    * Returns the total number of ACE_Event_Handlers that were
    * dispatched, 0 if the @a max_wait_time elapsed without dispatching
    * any handlers, or -1 if something goes wrong.
    */
   virtual int handle_events (ACE_Time_Value *max_wait_time = 0);
   
   virtual int handle_events (ACE_Time_Value &max_wait_time);
   
   
   /// Called from handle events
   static void no_op_sleep_hook (void *);
   
   /// Declare the dynamic allocation hooks.
   ACE_ALLOC_HOOK_DECLARE;

  protected:
   
   /// Template method from the base class.
   virtual void clear_dispatch_mask (ACE_HANDLE handle,
                                     ACE_Reactor_Mask mask);
   
   /// Dispatch just 1 signal, timer, notification handlers
   int dispatch_handler (ACE_Time_Value *max_wait_time,
                         ACE_TP_Token_Guard &guard);

   /// Template method called when new, non-empty set of events is
   /// returned from call to select
   virtual void preprocess_new_event_set () {}

   /// Template method called when an event handler has been
   /// selected for dispatch
   virtual void preprocess_chosen_handler (ACE_Handle_Dispatch_Info dispatch_info) {}

   /// Get the event that needs dispatching. It could be either a
   /// signal, timer, notification handlers or return possibly 1 I/O
   /// handler for dispatching. In the most common use case, this would
   /// return 1 I/O handler for dispatching
   int get_an_event_for_dispatching (ACE_Time_Value *max_wait_time);
   
   /// Handle timer events
   int handle_timer(int &event_count,
                    ACE_TP_Token_Guard &g);
   
   /// Handle notify events
   int handle_notify (int &event_count,
                      ACE_TP_Token_Guard &g);
   
   /// handle socket events
   int handle_socket(int &event_count,
                     ACE_TP_Token_Guard &g);
   
   /// This method shouldn't get called.
   virtual void notify_handle (ACE_HANDLE handle,
                               ACE_Reactor_Mask mask,
                               ACE_Handle_Set &,
                               ACE_Event_Handler *eh,
                               ACE_EH_PTMF callback);
  private:
   
   typedef ACE_Unbounded_Queue< ACE_Handle_Dispatch_Info > QUEUE;

   /// Deny access since member-wise won't work...
   ACE_MT_Priority_Reactor (const ACE_MT_Priority_Reactor &);
   ACE_MT_Priority_Reactor &operator = (const ACE_MT_Priority_Reactor &);
   
   /// Get the handle of the notify pipe from the ready set if there is
   /// an event in the notify pipe.
   ACE_HANDLE get_the_notify_handle (void);
   
   /// Get socket event dispatch information.
   int get_sock_event_info (ACE_Handle_Dispatch_Info &info);
   
   /// Allocates storage for event handler tokens
   void init_bucket (void);
   
   /// Notify the appropriate <callback> in the context of the <eh>
   /// associated with <handle> that a particular event has occurred.
   int dispatch_sock_event (ACE_Handle_Dispatch_Info &dispatch_info);
   
   /// Clear the @a handle from the read_set
   void clear_read_set (ACE_HANDLE handle);
   
   int post_process_sock_event (ACE_Handle_Dispatch_Info &dispatch_info,int status);
   
   
   /// Divide existing socket events into buckets by priority
   int bucketize_socket_events();
   
   // Private data members
   QUEUE**        _bucket;
   bool           _bucketize_socket_events;
   ACE_Allocator* _tuple_allocator;
   int            _num_socket_events;
   int            _current_priority;
   int            _min_priority;
   int            _max_priority;
};

ACE_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "ace/MT_Priority_Reactor.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* ACE_MT_PRIORITY_REACTOR_H */
