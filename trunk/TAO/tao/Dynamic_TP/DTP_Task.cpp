// $Id$

#include "tao/Dynamic_TP/DTP_Task.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#include "tao/CSD_ThreadPool/CSD_TP_Request.h"
#include "tao/CSD_ThreadPool/CSD_TP_Dispatchable_Visitor.h"
#include "tao/CSD_ThreadPool/CSD_TP_Cancel_Visitor.h"

#if !defined (__ACE_INLINE__)
# include "tao/Dynamic_TP/DTP_Task.inl"
#endif /* ! __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_DTP_Task::TAO_DTP_Task ()
  : aw_lock_ (),
    queue_lock_ (),
    work_lock_ (),
    work_available_ (this->work_lock_),
    active_workers_ (this->aw_lock_),
    active_count_ (0),
    accepting_requests_ (false),
    shutdown_ (false),
    opened_ (false),
    num_queue_requests_ ((size_t)0),
    init_pool_threads_ ((size_t)0),
    min_pool_threads_ ((size_t)0),
    max_pool_threads_ ((size_t)0),
    max_request_queue_depth_ ((size_t)0),
    thread_stack_size_ ((size_t)0)
{
}

TAO_DTP_Task::~TAO_DTP_Task()
{
}

bool
TAO_DTP_Task::add_request (TAO::CSD::TP_Request* request)
{
  {
    ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, guard, this->queue_lock_, false);
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
            TAOLIB_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("TAO (%P|%t) - DTP_Task::add_request() ")
                        ACE_TEXT ("not accepting requests.\n")
                        ACE_TEXT ("TAO (%P|%t) - DTP_Task::add_request() ")
                        ACE_TEXT ("num_queue_requests_ : [%d]\n")
                        ACE_TEXT ("TAO (%P|%t) - DTP_Task::add_request() ")
                        ACE_TEXT ("max_request_queue_depth_ : [%d]\n"),
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
  }

  {
    ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, guard, this->work_lock_, false);
    this->work_available_.signal ();
    if (TAO_debug_level > 4 )
      {
        TAOLIB_DEBUG((LM_DEBUG,
                   ACE_TEXT ("TAO (%P|%t) - DTP_Task::add_request() - ")
                   ACE_TEXT ("work available\n")));
      }
  }

  return true;
}

size_t
TAO_DTP_Task::get_init_pool_threads ()
{
  return this->init_pool_threads_;
}

size_t
TAO_DTP_Task::get_min_pool_threads ()
{
  return this->min_pool_threads_;
}

size_t TAO_DTP_Task::get_max_pool_threads ()
{
  return this->max_pool_threads_;
}

size_t
TAO_DTP_Task::get_max_request_queue_depth ()
{
  return this->max_request_queue_depth_;
}

size_t
TAO_DTP_Task::get_thread_stack_size ()
{
  return this->thread_stack_size_;
}

time_t
TAO_DTP_Task::get_thread_idle_time ()
{
  return this->thread_idle_time_.sec();
}

int
TAO_DTP_Task::open (void* /* args */)
{
  size_t num = 1;

  // Open_Args* tmp = static_cast<Open_Args *> (args);

  //if (tmp == 0)
  //  {
  //    //FUZZ: disable check_for_lack_ACE_OS
  //    TAOLIB_ERROR_RETURN ((LM_ERROR,
  //                       ACE_TEXT ("(%P|%t) DTP_Task::open() failed to open.  ")
  //                       ACE_TEXT ("Invalid argument type passed to open().\n")),
  //                      -1);
  //    //FUZZ: enable check_for_lack_ACE_OS
  //  }

  num = this->init_pool_threads_;

  // Set the busy_threads_ to the number of init_threads
  // now. When they startup they will decrement themselves
  // as they go into a wait state.

  this->busy_threads_ = 0;

  if (TAO_debug_level > 4)
  {
    TAOLIB_DEBUG((LM_DEBUG,
        ACE_TEXT ("TAO (%P|%t) - DTP_Task::open() initialized with:\n")
        ACE_TEXT ("TAO (%P|%t) - DTP_Task::open() init_threads_ \t\t: [%d]\n")
        ACE_TEXT ("TAO (%P|%t) - DTP_Task::open() min_pool_threads_ \t\t: [%d]\n")
        ACE_TEXT ("TAO (%P|%t) - DTP_Task::open() max_pool_threads_ \t\t: [%d]\n")
        ACE_TEXT ("TAO (%P|%t) - DTP_Task::open() max_request_queue_depth_ \t: [%d]\n")
        ACE_TEXT ("TAO (%P|%t) - DTP_Task::open() thread_stack_size_ \t\t: [%d]\n")
        ACE_TEXT ("TAO (%P|%t) - DTP_Task::open() thread_idle_time_ \t\t: [%d]\n"),
        this->init_pool_threads_,
        this->min_pool_threads_,
        this->max_pool_threads_,
        this->max_request_queue_depth_,
        this->thread_stack_size_,
        this->thread_idle_time_.sec ())
        );
  }

  // We can't activate 0 threads.  Make sure this isn't the case.
  if (num < 1)
    {
      TAOLIB_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("TAO (%P|%t) DTP_Task::open() failed to open.  ")
                         ACE_TEXT ("num_threads (%u) is less-than 1.\n"),
                         num),
                        -1);
    }

  // We need the lock acquired from here on out.
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, guard, this->aw_lock_, -1);

  // We can assume that we are in the proper state to handle this open()
  // call as long as we haven't been open()'ed before.
  if (this->opened_)
    {
      //FUZZ: disable check_for_lack_ACE_OS
      TAOLIB_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("TAO (%P|%t) DTP_Task::open() failed to open.  ")
                         ACE_TEXT ("Task has previously been open()'ed.\n")),
                        -1);
      //FUZZ: enable check_for_lack_ACE_OS
    }
  // Create the stack size arrays if the stack size is set > 0.

  // Activate this task object with 'num' worker threads.
  if (this->thread_stack_size_ == 0)
    {
      if (this->activate (THR_NEW_LWP | THR_DETACHED, num, 1) != 0)
        {
          TAOLIB_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("(%P|%t) DTP_Task::open() failed to activate ")
                             ACE_TEXT ("(%d) worker threads.\n"),
                             num),
                            -1);
        }
    }
  else
    {
      size_t * stack_sz_arr = new size_t[num];
      for (size_t z = 0; z < num; z++)
        {
          stack_sz_arr[z] = this->thread_stack_size_;
        }

      if (this->activate (THR_NEW_LWP | THR_DETACHED,
                          num,
                          1,
                          ACE_DEFAULT_THREAD_PRIORITY,
                          -1,
                          0,
                          0,
                          0,
                          stack_sz_arr) != 0)
        {
          TAOLIB_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("(%P|%t) DTP_Task::open() failed to activate ")
                             ACE_TEXT ("(%d) worker threads.\n"),
                             num),
                            -1);
        }

      delete[] stack_sz_arr;
    }

  this->active_count_ = num;

  this->opened_ = true;
  this->accepting_requests_ = true;

  return 0;
}

bool
TAO_DTP_Task::request_ready (TAO::CSD::TP_Dispatchable_Visitor &v,
                                    TAO::CSD::TP_Request_Handle &r)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, guard, this->queue_lock_, false);
  if (!this->queue_.is_empty())
    {
      this->queue_.accept_visitor(v);
      r = v.request();
      return !r.is_nil();
    }
  return false;
}

void
TAO_DTP_Task::clear_request (TAO::CSD::TP_Request_Handle &r)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->queue_lock_);
  --this->num_queue_requests_;
  if (this->max_request_queue_depth_ > 0)
    {
      this->accepting_requests_ = true;
    }

  if (TAO_debug_level > 4 )
    {
      TAOLIB_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) - DTP_Task::svc() ")
                  ACE_TEXT ("Decrementing num_queue_requests.")
                  ACE_TEXT ("New queue depth:%d\n"),
                  this->num_queue_requests_));
    }

  r->mark_as_ready ();
}

void
TAO_DTP_Task::add_busy (void)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, mon, this->aw_lock_);
  ++this->busy_threads_;
}

void
TAO_DTP_Task::remove_busy (void)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, mon, this->aw_lock_);
  --this->busy_threads_;
}

void
TAO_DTP_Task::add_active (void)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, mon, this->aw_lock_);
  ++this->active_count_;
}

bool
TAO_DTP_Task::remove_active (bool force)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, mon, this->aw_lock_, false);
  if (force || this->above_minimum())
    {
      --this->active_count_;
      this->active_workers_.signal ();
      return true;
    }
  return false;
}

bool
TAO_DTP_Task::need_active (void)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, mon, this->aw_lock_, false);
  return ((this->busy_threads_ == this->active_count_) &&
          ((this->max_pool_threads_ < 1) ||
           (this->active_count_ < this->max_pool_threads_)));
}

bool
TAO_DTP_Task::above_minimum (void)
{
  return this->min_pool_threads_ > 0 &&
    this->active_count_ > this->min_pool_threads_;
}

int
TAO_DTP_Task::svc (void)
{
  this->add_busy ();
  if (TAO_debug_level > 4)
    {
      TAOLIB_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) - DTP_Task::svc() ")
                  ACE_TEXT ("New thread created.\n")));
    }
  TAO::CSD::TP_Dispatchable_Visitor dispatchable_visitor;
  while (!this->shutdown_)
    {
      TAO::CSD::TP_Request_Handle request;

      while (!this->shutdown_ && request.is_nil ())
        {
          if (!this->request_ready (dispatchable_visitor, request))
            {
              this->remove_busy ();

              if (TAO_debug_level > 4)
                {
                  TAOLIB_DEBUG ((LM_DEBUG,
                              ACE_TEXT ("TAO (%P|%t) - DTP_Task::svc() ")
                              ACE_TEXT ("Decrementing busy_threads_. ")
                              ACE_TEXT ("Busy thread count:%d\n"),
                              this->busy_threads_.value()));
                }

              ACE_Time_Value tmp_sec = this->thread_idle_time_.to_absolute_time();

              {
                ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, guard, this->work_lock_, false);
                int wait_state = this->thread_idle_time_.sec () == 0
                                 ? this->work_available_.wait ()
                                 : this->work_available_.wait (&tmp_sec);

                // Check for timeout
                if (this->shutdown_)
                  return 0;
                if (wait_state == -1)
                  {
                    if (errno != ETIME || this->remove_active (false))
                      {
                        if (TAO_debug_level > 4)
                          {
                            TAOLIB_DEBUG ((LM_DEBUG,
                                        ACE_TEXT ("TAO (%P|%t) - DTP_Task::svc() ")
                                        ACE_TEXT ("Existing thread expiring.\n")));
                          }
                        return 0;
                      }
                  }
              }

              this->add_busy ();
              if (TAO_debug_level > 4)
                {
                  TAOLIB_DEBUG ((LM_DEBUG,
                              ACE_TEXT ("TAO (%P|%t) - DTP_Task::svc() ")
                              ACE_TEXT ("Incrementing busy_threads_. ")
                              ACE_TEXT ("Busy thread count:%d\n"),
                              this->busy_threads_.value ()));
                }
            }
        }

      if (this->need_active ())
        {
          if (this->activate (THR_NEW_LWP | THR_DETACHED,
                              1,
                              1,
                              ACE_DEFAULT_THREAD_PRIORITY,
                              -1,
                              0,
                              0,
                              0,
                              this->thread_stack_size_ == 0 ? 0 :
                              &this->thread_stack_size_) != 0)
            {
              TAOLIB_ERROR ((LM_ERROR,
                             ACE_TEXT ("(%P|%t) DTP_Task::svc() failed to ")
                             ACE_TEXT ("grow thread pool.\n")));
            }
          else
            {
              this->add_active ();
              if (TAO_debug_level > 4)
                {
                  TAOLIB_DEBUG ((LM_DEBUG,
                                 ACE_TEXT ("TAO (%P|%t) - DTP_Task::svc() ")
                                 ACE_TEXT ("Growing threadcount. ")
                                 ACE_TEXT ("New thread count:%d\n"),
                                 this->thr_count ()));
                }
            }
        }

      request->dispatch ();
      this->clear_request (request);
      dispatchable_visitor.reset ();
    }
  this->remove_active (true);
  return 0;
}


int
TAO_DTP_Task::close (u_long flag)
{
  {
    ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, guard, this->aw_lock_, 0);
    if (flag == 0)
      {
        this->active_workers_.signal ();
        return 0;
      }

    if (!this->opened_)
      {
        return 0;
      }
    this->opened_ = false;
    this->shutdown_ = true;
    this->accepting_requests_ = false;
  }

  {
    ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, guard, this->work_lock_, 0);
    this->work_available_.broadcast();
  }

  size_t in_task = (this->thr_mgr ()->task () == this) ? 1 : 0;
  if (TAO_debug_level > 4)
    {
      TAOLIB_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) - DTP_Task::close() ")
                  ACE_TEXT ("shutting down. in_task = %d,  Count = %d \n"),
                  in_task,  this->thr_count ()));
    }

  while (this->thr_count () > in_task)
    {
      ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, guard, this->aw_lock_, 0);
      this->active_workers_.wait ();
    }

  {
    ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, guard, this->queue_lock_, 0);
    TAO::CSD::TP_Cancel_Visitor v;
    this->queue_.accept_visitor (v);
  }
  return 0;
}


void
TAO_DTP_Task::set_init_pool_threads (size_t thr_count)
{
  this->init_pool_threads_ = thr_count;
}

void
TAO_DTP_Task::set_min_pool_threads (size_t thr_count)
{
  this->min_pool_threads_ = thr_count;
}

void
TAO_DTP_Task::set_max_pool_threads (size_t thr_count)
{
  this->max_pool_threads_ = thr_count;
}

void
TAO_DTP_Task::set_thread_stack_size (size_t stack_sz)
{
  this->thread_stack_size_ = stack_sz;
}

void
TAO_DTP_Task::set_thread_idle_time(ACE_Time_Value thr_timeout)
{
  this->thread_idle_time_ = thr_timeout;
}

void
TAO_DTP_Task::set_max_request_queue_depth (size_t queue_depth)
{
  this->max_request_queue_depth_ = queue_depth;
}

void
TAO_DTP_Task::cancel_servant (PortableServer::Servant servant)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->queue_lock_);

  // Cancel the requests targeted for the provided servant.
  TAO::CSD::TP_Cancel_Visitor cancel_visitor (servant);
  this->queue_.accept_visitor (cancel_visitor);
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0 */
