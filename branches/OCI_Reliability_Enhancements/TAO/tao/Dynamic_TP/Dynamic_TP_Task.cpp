// $Id$

#include "tao/Dynamic_TP/Dynamic_TP_Task.h"
#include "tao/CSD_ThreadPool/CSD_TP_Request.h"
#include "tao/CSD_ThreadPool/CSD_TP_Dispatchable_Visitor.h"
#include "tao/CSD_ThreadPool/CSD_TP_Cancel_Visitor.h"

#if !defined (__ACE_INLINE__)
# include "tao/Dynamic_TP/Dynamic_TP_Task.inl"
#endif /* ! __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL


TAO_Dynamic_TP_Task::~TAO_Dynamic_TP_Task()
{
}

bool
TAO_Dynamic_TP_Task::add_request(TAO::CSD::TP_Request* request)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, guard, this->lock_, false);

  ++this->num_queue_requests_;
  if ((this->num_queue_requests_ > this->max_request_queue_depth_) &&
      (this->max_request_queue_depth_ != 0))
  {
    this->accepting_requests_ = false;
  }

  if (!this->accepting_requests_)
    {
      if (TAO_debug_level > 4)
      {
          ACE_DEBUG((LM_DEBUG,
        ACE_TEXT ("TAO (%P|%t) - Dynamic_TP_Task::add_request() not accepting requests.\n")
            ACE_TEXT ("TAO (%P|%t) - Dynamic_TP_Task::add_request() num_queue_requests_ : [%d]\n")
            ACE_TEXT ("TAO (%P|%t) - Dynamic_TP_Task::add_request() max_request_queue_depth_ : [%d]\n"),
            this->num_queue_requests_,
           this->max_request_queue_depth_));
        }
      --this->num_queue_requests_;
      return false;
    }

  // We have made the decision that the request is going to be placed upon
  // the queue_.  Inform the request that it is about to be placed into
  // a request queue.  Some requests may not need to do anything in
  // preparation of being placed into a queue.  Others, however, may need
  // to perfom a "clone" operation on some underlying request data before
  // the request can be properly placed into a queue.
  request->prepare_for_queue();

  this->queue_.put(request);

  this->work_available_.signal();
  if (TAO_debug_level > 4 )
  {
    ACE_DEBUG((LM_DEBUG,"TAO (%P|%t) - Dynamic_TP_Task::add_request() - "
            "work available\n"));
  }

  return true;
}

int
TAO_Dynamic_TP_Task::open(void* args)
{
  TAO_Dynamic_TP_Thread_Counter num = 1;

  Open_Args* tmp = static_cast<Open_Args *> (args);



  if (tmp == 0)
    {
      //FUZZ: disable check_for_lack_ACE_OS
      ACE_ERROR_RETURN((LM_ERROR,
                        ACE_TEXT ("(%P|%t) Dynamic_TP_Task::open() failed to open.  ")
                        ACE_TEXT ("Invalid argument type passed to open().\n")),
                        -1);
      //FUZZ: enable check_for_lack_ACE_OS
    }

  num = tmp->task_thread_config.init_threads_;

  // Set the busy_threads_ to the number of init_threads
  // now. When they startup they will decrement themselves
  // as they go into a wait state.

  this->busy_threads_ = 0; //tmp->task_thread_config.init_threads_; //
  this->min_pool_threads_ = tmp->task_thread_config.min_threads_;
  this->max_pool_threads_ = tmp->task_thread_config.max_threads_;
  this->max_request_queue_depth_ = tmp->task_thread_config.queue_depth_;
  this->thread_stack_size_ = tmp->task_thread_config.stack_size_;
  this->thread_idle_time_ = tmp->task_thread_config.timeout_;

  if (TAO_debug_level > 4)
  {
    ACE_DEBUG((LM_DEBUG,
        ACE_TEXT ("TAO (%P|%t) - Dynamic_TP_Task::open() initialized with:\n")
        ACE_TEXT ("TAO (%P|%t) - Dynamic_TP_Task::open() init_threads_ \t\t: [%d]\n")
        ACE_TEXT ("TAO (%P|%t) - Dynamic_TP_Task::open() min_pool_threads_ \t\t: [%d]\n")
        ACE_TEXT ("TAO (%P|%t) - Dynamic_TP_Task::open() max_pool_threads_ \t\t: [%d]\n")
        ACE_TEXT ("TAO (%P|%t) - Dynamic_TP_Task::open() max_request_queue_depth_ \t: [%d]\n")
        ACE_TEXT ("TAO (%P|%t) - Dynamic_TP_Task::open() thread_stack_size_ \t\t: [%d]\n")
        ACE_TEXT ("TAO (%P|%t) - Dynamic_TP_Task::open() thread_idle_time_ \t\t: [%d]\n"),
        num,
        this->min_pool_threads_,
        this->max_pool_threads_,
        this->max_request_queue_depth_,
        this->thread_stack_size_,
        this->thread_idle_time_.sec())
        );
  }

  // We can't activate 0 threads.  Make sure this isn't the case.
  if (num < 1)
    {
      ACE_ERROR_RETURN((LM_ERROR,
                        ACE_TEXT ("TAO (%P|%t) Dynamic_TP_Task::open() failed to open.  ")
                        ACE_TEXT ("num_threads (%u) is less-than 1.\n"),
                        num),
                       -1);
    }

  // We need the lock acquired from here on out.
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, guard, this->lock_, -1);

  // We can assume that we are in the proper state to handle this open()
  // call as long as we haven't been open()'ed before.
  if (this->opened_)
    {
      //FUZZ: disable check_for_lack_ACE_OS
      ACE_ERROR_RETURN((LM_ERROR,
                        ACE_TEXT ("TAO (%P|%t) Dynamic_TP_Task::open() failed to open.  ")
                        ACE_TEXT ("Task has previously been open()'ed.\n")),
                       -1);
      //FUZZ: enable check_for_lack_ACE_OS
    }
  // Create the stack size arrays if the stack size is set > 0.

  // Activate this task object with 'num' worker threads.
  if (this->thread_stack_size_ == 0)
  {

    if (this->activate(THR_NEW_LWP | THR_DETACHED,
                       num,
                       1) != 0)
      {
        // Assumes that when activate returns non-zero return code that
        // no threads were activated.
        ACE_ERROR_RETURN((LM_ERROR,
                          ACE_TEXT ("(%P|%t) Dynamic_TP_Task::open() failed to activate ")
                          ACE_TEXT ("(%d) worker threads.\n"),
                          num),
                         -1);
      }
  } else
  {
    size_t * stack_sz_arr = new size_t[num];
    for (int z=0;z<num;z++)
    {
      stack_sz_arr[z] = this->thread_stack_size_;
    }

    if (this->activate(THR_NEW_LWP | THR_DETACHED,
                       num,
                       1,
                       ACE_DEFAULT_THREAD_PRIORITY,
                       -1,
                       0,
                       0,
                       0,
                       stack_sz_arr) != 0)
      {
        // Assumes that when activate returns non-zero return code that
        // no threads were activated.
        ACE_ERROR_RETURN((LM_ERROR,
                          ACE_TEXT ("(%P|%t) Dynamic_TP_Task::open() failed to activate ")
                          ACE_TEXT ("(%d) worker threads.\n"),
                          num),
                         -1);
      }

    // After the threads get allocated we need to clean up the allocated array.
    delete[] stack_sz_arr;
  }

  // Now we have past the point where we can say we've been open()'ed before.
  this->opened_ = true;

  // Now we wait until all of the threads have started.
  while (this->num_threads_ != num)
    {
      this->active_workers_.wait();
    }

  // We can now accept requests (via our add_request() method).
  this->accepting_requests_ = true;

  return 0;
}

int
TAO_Dynamic_TP_Task::svc()
{
  // This captures whether the wait was triggered by a timeout
  // or from a signal. A timeout will result in a -1.
  int wait_state = 0;

  // Account for this current worker thread having started the
  // execution of this svc() method.

      {
        ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, guard, this->lock_, false);
        // Put the thread id into a collection which is used to check whether
        // the orb shutdown is called by one of the threads in the pool.
        ACE_thread_t thr_id = ACE_OS::thr_self ();
        this->activated_threads_.push_back(thr_id);
        ++this->num_threads_;
        ++this->busy_threads_;
        this->active_workers_.signal();

        if (TAO_debug_level > 4)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) - Dynamic_TP_Task::svc() ")
                      ACE_TEXT ("New thread created. ")
                      ACE_TEXT ("Current thread count:%d, busy_threads:%d\n"),
                      this->num_threads_, this->busy_threads_.value()));
        }
      }

  // This visitor object will be re-used over and over again as part of
  // the "GetWork" logic below.
  TAO::CSD::TP_Dispatchable_Visitor dispatchable_visitor;

  // Start the "GetWork-And-PerformWork" loop for the current worker thread.
  while (1)
    {
      TAO::CSD::TP_Request_Handle request;

      // Start the "GetWork" loop.
      while (request.is_nil())
        {
          if (this->shutdown_initiated_)
            {
              // This breaks us out of all loops with one fell swoop.
              return 0;
            }

          if (this->deferred_shutdown_initiated_)
            {
              this->deferred_shutdown_initiated_  = false;
              return 0;
            }


          {
            // Acquire the lock until just before we decide to "PerformWork".
            ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, guard, this->lock_, 0);

            // There is no need to visit the queue if it is empty.
            if (!this->queue_.is_empty())
              {
                // Visit the requests in the queue in hopes of
                // locating the first "dispatchable" (ie, not busy) request.
                // If a dispatchable request is located, it is extracted
                // from the queue and saved in a handle data member in the
                // visitor object.
                this->queue_.accept_visitor(dispatchable_visitor);

                // If a dispatchable request is located, it is extracted
                // from the queue and saved in a handle data member in the
                // visitor object.  Let's get a "copy" (or a NULL pointer
                // if the visitor didn't locate/extract one).
                request = dispatchable_visitor.request();
              }

          }


          // Either the queue is empty or we couldn't find any dispatchable
          // requests in the queue at this time.
          if (request.is_nil())
            {
              // Let's wait until we hear about the possibility of
              // work before we go look again.

              // When the task is initialized the busy_threads_ is set to the
              // number of threads intialized and then each thread is decremented
              // here when they go into a wait state.

              --this->busy_threads_;

              if (TAO_debug_level > 4)
                {
                  ACE_DEBUG ((LM_DEBUG,
                              ACE_TEXT ("TAO (%P|%t) - Dynamic_TP_Task::svc() ")
                              ACE_TEXT ("Decrementing busy_threads_. ")
                              ACE_TEXT ("Busy thread count:%d\n"),
                              this->busy_threads_.value()));
                }

              ACE_Time_Value tmp_sec = this->thread_idle_time_.to_absolute_time();

              wait_state = this->work_available_.wait(&tmp_sec);

              // Check for timeout
              if ((wait_state == -1) &&
                  (this->num_threads_ > this->min_pool_threads_) &&
                  (this->num_threads_ > 1))
              {
                --this->num_threads_;
                if (TAO_debug_level > 4)
                {
                  ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%P|%t) - Dynamic_TP_Task::svc() ")
                    ACE_TEXT ("Existing thread expiring. ")
                    ACE_TEXT ("New thread count:%d\n"),
                    this->num_threads_));
                }
                return 0;
              }
            }

          // The thread has dropped out of the wait state and has not
          // returned based on a timeout, so it has now become busy.
          ++this->busy_threads_;
          if (TAO_debug_level > 4)
          {
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("TAO (%P|%t) - Dynamic_TP_Task::svc() ")
                        ACE_TEXT ("Incrementing busy_threads_. ")
                        ACE_TEXT ("Busy thread count:%d\n"),
                        this->busy_threads_.value()));
          }
        }

      // Grow the pool if the configuration allows it.

      if ((this->busy_threads_ == this->num_threads_) &&
          ((this->max_pool_threads_ == -1) ||
           (this->num_threads_ < this->max_pool_threads_)))
      {
        // Increment the busy_threads_ count until the new thread can hit a wait state
        // and decrement itself.

        //        ++this->busy_threads_;

        if (this->thread_stack_size_ == 0)
        {

          if (this->activate(THR_NEW_LWP | THR_DETACHED,
                             1,
                             1) != 0)
            {
              // Assumes that when activate returns non-zero return code that
              // no threads were activated.
              ACE_DEBUG((LM_ERROR,
              ACE_TEXT ("(%P|%t) Dynamic_TP_Task::svc() failed to activate ")
              ACE_TEXT ("(%d) worker threads.\n"),
              this-num_threads_));
            }
        } else
        {
          if (this->activate(THR_NEW_LWP | THR_DETACHED,
                             1,
                             1,
                             ACE_DEFAULT_THREAD_PRIORITY,
                             -1,
                             0,
                             0,
                             0,
                             &this->thread_stack_size_) != 0)
            {
              // Assumes that when activate returns non-zero return code that
              // no threads were activated.
              ACE_DEBUG((LM_ERROR,
              ACE_TEXT ("(%P|%t) Dynamic_TP_Task::svc() failed to activate ")
              ACE_TEXT ("(%d) worker threads.\n"),
              this-num_threads_));
            }
        }
      }

      // Do the "PerformWork" step.  We don't need the lock_ to do this.
      request->dispatch();

      // Now that the request has been dispatched, we need to mark the target
      // servant as no longer being busy, and we need to signal any wait()'ing
      // worker threads that there may be some dispatchable requests in the
      // queue now for this not-busy servant.  We need the lock_ to do this.
      {
        ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, guard, this->lock_, 0);
        --this->num_queue_requests_;
        if (this->max_request_queue_depth_ > 0)
        {
          this->accepting_requests_ = true;
        }

        if (TAO_debug_level > 4 )
        {
        ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("TAO (%P|%t) - Dynamic_TP_Task::svc() ")
              ACE_TEXT ("Decrementing num_queue_requests.")
              ACE_TEXT ("New queue depth:%d\n"),
              this->num_queue_requests_));
        }

        request->mark_as_ready();
        this->work_available_.signal();
      }

      // Reset the visitor since we use it over and over.  This
      // will cause the visitor to drop any reference to
      // the dispatched request.
      dispatchable_visitor.reset();

      // Note that the request will be "released" here when the request
      // handle falls out of scope and its destructor performs the
      // _remove_ref() call on the underlying TP_Request object.
    }
}

void
TAO_Dynamic_TP_Task::cancel_servant (PortableServer::Servant servant)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->lock_);

  // Cancel the requests targeted for the provided servant.
  TAO::CSD::TP_Cancel_Visitor cancel_visitor(servant);
  this->queue_.accept_visitor(cancel_visitor);
}

TAO_END_VERSIONED_NAMESPACE_DECL
