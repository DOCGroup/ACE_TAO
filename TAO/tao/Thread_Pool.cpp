// $Id$

#include "tao/Thread_Pool.h"

ACE_RCSID(tao, Thread_Pool, "$Id$")

#if (TAO_HAS_RT_CORBA == 1)

#include "tao/Exception.h"
#include "ace/Auto_Ptr.h"
#include "tao/ORB_Core.h"

#if !defined (__ACE_INLINE__)
# include "tao/Thread_Pool.i"
#endif /* ! __ACE_INLINE__ */

TAO_Thread_Pool_Threads::TAO_Thread_Pool_Threads (TAO_Thread_Lane &lane,
                                                  ACE_Thread_Manager &tm)
  : ACE_Task_Base (&tm),
    lane_ (lane)
{
}

TAO_Thread_Lane &
TAO_Thread_Pool_Threads::lane (void) const
{
  return this->lane_;
}

int
TAO_Thread_Pool_Threads::svc (void)
{
  CORBA::ORB_var orb =
    this->lane ().thread_pool ().orb ();

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      orb->run (ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_ERROR ((LM_ERROR,
                  "orb->run() raised exception for thread %t\n"));
    }
  ACE_ENDTRY;

  return 0;
}

TAO_Thread_Lane::TAO_Thread_Lane (TAO_Thread_Pool &thread_pool,
                                  CORBA::ULong id,
                                  CORBA::Short lane_priority,
                                  CORBA::ULong static_threads,
                                  CORBA::ULong dynamic_threads)
  : thread_pool_ (thread_pool),
    id_ (id),
    lane_priority_ (lane_priority),
    static_threads_ (static_threads),
    dynamic_threads_ (dynamic_threads),
    threads_ (*this,
              thread_pool_.thread_manager_)
{
}

int
TAO_Thread_Lane::create_static_threads (void)
{
  return this->create_dynamic_threads (this->static_threads_);
}


int
TAO_Thread_Lane::create_dynamic_threads (CORBA::ULong number_of_threads)
{
  // Overwritten parameters.
  int force_active = 1;

  // Default parameters.
  long default_flags = THR_NEW_LWP | THR_JOINABLE;
  int default_grp_id = -1;
  ACE_Task_Base *default_task = 0;
  ACE_hthread_t *default_thread_handles = 0;
  void **default_stack = 0;

  // Setting stack size.
  size_t *stack_size_array = 0;
  ACE_NEW_RETURN (stack_size_array,
                  size_t[number_of_threads],
                  -1);
  size_t index;
  for (index = 0;
       index != number_of_threads;
       ++index)
    stack_size_array[index] = this->thread_pool_.stack_size_;

  // Make sure the dynamically created stack size array is properly
  // deleted.
  ACE_Auto_Basic_Array_Ptr<size_t> auto_stack_size_array (stack_size_array);

  // Activate the threads.
  return this->threads_.activate (default_flags,
                                  number_of_threads,
                                  force_active,
                                  this->lane_priority_,
                                  default_grp_id,
                                  default_task,
                                  default_thread_handles,
                                  default_stack,
                                  stack_size_array);
}

CORBA::ULong
TAO_Thread_Lane::id (void) const
{
  return this->id_;
}

TAO_Thread_Pool &
TAO_Thread_Lane::thread_pool (void) const
{
  return this->thread_pool_;
}

CORBA::Short
TAO_Thread_Lane::lane_priority (void) const
{
  return this->lane_priority_;
}

CORBA::ULong
TAO_Thread_Lane::static_threads (void) const
{
  return this->static_threads_;
}

CORBA::ULong
TAO_Thread_Lane::dynamic_threads (void) const
{
  return this->dynamic_threads_;
}

TAO_Thread_Pool_Threads &
TAO_Thread_Lane::threads (void)
{
  return this->threads_;
}

TAO_Thread_Pool::TAO_Thread_Pool (CORBA::ULong id,
                                  CORBA::ULong stack_size,
                                  CORBA::ULong static_threads,
                                  CORBA::ULong dynamic_threads,
                                  CORBA::Short default_priority,
                                  CORBA::Boolean allow_request_buffering,
                                  CORBA::ULong max_buffered_requests,
                                  CORBA::ULong max_request_buffer_size,
                                  CORBA::ORB_ptr orb)
  : id_ (id),
    stack_size_ (stack_size),
    allow_borrowing_ (0),
    allow_request_buffering_ (allow_request_buffering),
    max_buffered_requests_ (max_buffered_requests),
    max_request_buffer_size_ (max_request_buffer_size),
    lanes_ (0),
    number_of_lanes_ (1),
    thread_manager_ (*orb->orb_core ()->thr_mgr ()),
    orb_ (CORBA::ORB::_duplicate (orb))
{
  this->lanes_ = new TAO_Thread_Lane *[this->number_of_lanes_];
  this->lanes_[0] = new TAO_Thread_Lane (*this,
                                         0,
                                         default_priority,
                                         static_threads,
                                         dynamic_threads);
}

TAO_Thread_Pool::TAO_Thread_Pool (CORBA::ULong id,
                                  CORBA::ULong stack_size,
                                  const RTCORBA::ThreadpoolLanes &lanes,
                                  CORBA::Boolean allow_borrowing,
                                  CORBA::Boolean allow_request_buffering,
                                  CORBA::ULong max_buffered_requests,
                                  CORBA::ULong max_request_buffer_size,
                                  CORBA::ORB_ptr orb)
  : id_ (id),
    stack_size_ (stack_size),
    allow_borrowing_ (allow_borrowing),
    allow_request_buffering_ (allow_request_buffering),
    max_buffered_requests_ (max_buffered_requests),
    max_request_buffer_size_ (max_request_buffer_size),
    lanes_ (0),
    number_of_lanes_ (lanes.length ()),
    thread_manager_ (*orb->orb_core ()->thr_mgr ()),
    orb_ (CORBA::ORB::_duplicate (orb))
{
  this->lanes_ = new TAO_Thread_Lane *[this->number_of_lanes_];
  for (CORBA::ULong i = 0;
       i != this->number_of_lanes_;
       ++i)
    this->lanes_[i] = new TAO_Thread_Lane (*this,
                                           i,
                                           lanes[i].lane_priority,
                                           lanes[i].static_threads,
                                           lanes[i].dynamic_threads);
}

TAO_Thread_Pool::~TAO_Thread_Pool (void)
{
  for (CORBA::ULong i = 0;
       i != this->number_of_lanes_;
       ++i)
    delete this->lanes_[i];

  delete[] this->lanes_;
}

int
TAO_Thread_Pool::create_static_threads (void)
{
  for (CORBA::ULong i = 0;
       i != this->number_of_lanes_;
       ++i)
    {
      // Ask each lane to create its set of static threads.
      int result = this->lanes_[i]->create_static_threads ();

      // Return on failure.
      if (result != 0)
        return result;
    }

  // Success.
  return 0;
}

CORBA::ULong
TAO_Thread_Pool::id (void) const
{
  return this->id_;
}

CORBA::ULong
TAO_Thread_Pool::stack_size (void) const
{
  return this->stack_size_;
}

CORBA::Boolean
TAO_Thread_Pool::allow_borrowing (void) const
{
  return this->allow_borrowing_;
}

CORBA::Boolean
TAO_Thread_Pool::allow_request_buffering (void) const
{
  return this->allow_request_buffering_;
}

CORBA::ULong
TAO_Thread_Pool::max_buffered_requests (void) const
{
  return this->max_buffered_requests_;
}

CORBA::ULong
TAO_Thread_Pool::max_request_buffer_size (void) const
{
  return this->max_request_buffer_size_;
}

TAO_Thread_Lane **
TAO_Thread_Pool::lanes (void)
{
  return this->lanes_;
}

CORBA::ULong
TAO_Thread_Pool::number_of_lanes (void) const
{
  return this->number_of_lanes_;
}

ACE_Thread_Manager &
TAO_Thread_Pool::thread_manager (void)
{
  return this->thread_manager_;
}

CORBA::ORB_ptr
TAO_Thread_Pool::orb (void) const
{
  return CORBA::ORB::_duplicate (this->orb_.in ());
}

#define TAO_THREAD_POOL_MANAGER_GUARD \
  ACE_GUARD_THROW_EX ( \
    ACE_SYNCH_MUTEX, \
    mon, \
    this->lock_, \
    CORBA::INTERNAL ( \
      CORBA_SystemException::_tao_minor_code ( \
        TAO_GUARD_FAILURE, \
        0), \
      CORBA::COMPLETED_NO));

TAO_Thread_Pool_Manager::TAO_Thread_Pool_Manager (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    thread_pools_ (),
    thread_pool_id_counter_ (0),
    lock_ ()
{
}

TAO_Thread_Pool_Manager::~TAO_Thread_Pool_Manager (void)
{
  for (THREAD_POOLS::ITERATOR iterator = this->thread_pools_.begin ();
       iterator != this->thread_pools_.end ();
       ++iterator)
    delete (*iterator).int_id_;
}

RTCORBA::ThreadpoolId
TAO_Thread_Pool_Manager::create_threadpool (CORBA::ULong stacksize,
                                            CORBA::ULong static_threads,
                                            CORBA::ULong dynamic_threads,
                                            RTCORBA::Priority default_priority,
                                            CORBA::Boolean allow_request_buffering,
                                            CORBA::ULong max_buffered_requests,
                                            CORBA::ULong max_request_buffer_size,
                                            CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_THREAD_POOL_MANAGER_GUARD;
  ACE_CHECK_RETURN (0);

  return this->create_threadpool_i (stacksize,
                                    static_threads,
                                    dynamic_threads,
                                    default_priority,
                                    allow_request_buffering,
                                    max_buffered_requests,
                                    max_request_buffer_size,
                                    ACE_TRY_ENV);
}

RTCORBA::ThreadpoolId
TAO_Thread_Pool_Manager::create_threadpool_with_lanes (CORBA::ULong stacksize,
                                                       const RTCORBA::ThreadpoolLanes & lanes,
                                                       CORBA::Boolean allow_borrowing,
                                                       CORBA::Boolean allow_request_buffering,
                                                       CORBA::ULong max_buffered_requests,
                                                       CORBA::ULong max_request_buffer_size,
                                                       CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_THREAD_POOL_MANAGER_GUARD;
  ACE_CHECK_RETURN (0);

  return this->create_threadpool_with_lanes_i (stacksize,
                                               lanes,
                                               allow_borrowing,
                                               allow_request_buffering,
                                               max_buffered_requests,
                                               max_request_buffer_size,
                                               ACE_TRY_ENV);
}

void
TAO_Thread_Pool_Manager::destroy_threadpool (RTCORBA::ThreadpoolId threadpool,
                                             CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   RTCORBA::RTORB::InvalidThreadpool))
{
  TAO_THREAD_POOL_MANAGER_GUARD;
  ACE_CHECK;

  this->destroy_threadpool_i (threadpool,
                              ACE_TRY_ENV);
}

RTCORBA::ThreadpoolId
TAO_Thread_Pool_Manager::create_threadpool_i (CORBA::ULong stacksize,
                                              CORBA::ULong static_threads,
                                              CORBA::ULong dynamic_threads,
                                              RTCORBA::Priority default_priority,
                                              CORBA::Boolean allow_request_buffering,
                                              CORBA::ULong max_buffered_requests,
                                              CORBA::ULong max_request_buffer_size,
                                              CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Create the thread pool.
  TAO_Thread_Pool *thread_pool = 0;

  ACE_NEW_THROW_EX (thread_pool,
                    TAO_Thread_Pool (this->thread_pool_id_counter_,
                                     stacksize,
                                     static_threads,
                                     dynamic_threads,
                                     default_priority,
                                     allow_request_buffering,
                                     max_buffered_requests,
                                     max_request_buffer_size,
                                     this->orb_.in ()),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  return this->create_threadpool_helper (thread_pool,
                                         ACE_TRY_ENV);
}

RTCORBA::ThreadpoolId
TAO_Thread_Pool_Manager::create_threadpool_with_lanes_i (CORBA::ULong stacksize,
                                                         const RTCORBA::ThreadpoolLanes &lanes,
                                                         CORBA::Boolean allow_borrowing,
                                                         CORBA::Boolean allow_request_buffering,
                                                         CORBA::ULong max_buffered_requests,
                                                         CORBA::ULong max_request_buffer_size,
                                                         CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Create the thread pool.
  TAO_Thread_Pool *thread_pool = 0;

  ACE_NEW_THROW_EX (thread_pool,
                    TAO_Thread_Pool (this->thread_pool_id_counter_,
                                     stacksize,
                                     lanes,
                                     allow_borrowing,
                                     allow_request_buffering,
                                     max_buffered_requests,
                                     max_request_buffer_size,
                                     this->orb_.in ()),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  return this->create_threadpool_helper (thread_pool,
                                         ACE_TRY_ENV);
}

RTCORBA::ThreadpoolId
TAO_Thread_Pool_Manager::create_threadpool_helper (TAO_Thread_Pool *thread_pool,
                                                   CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Make sure of safe deletion in case of errors.
  auto_ptr<TAO_Thread_Pool> safe_thread_pool (thread_pool);

  // Create the static threads.
  int result =
    thread_pool->create_static_threads ();

  // Throw exceptin in case of errors.
  if (result != 0)
    ACE_THROW_RETURN (CORBA::INTERNAL (),
                      result);

  // Bind thread to internal table.
  result =
    this->thread_pools_.bind (this->thread_pool_id_counter_,
                              thread_pool);

  // Throw exceptin in case of errors.
  if (result != 0)
    ACE_THROW_RETURN (CORBA::INTERNAL (),
                      result);

  //
  // Success.
  //

  // No need to delete thread pool.
  safe_thread_pool.release ();

  // Return current counter and perform post-increment.
  return this->thread_pool_id_counter_++;
};

void
TAO_Thread_Pool_Manager::destroy_threadpool_i (RTCORBA::ThreadpoolId thread_pool_id,
                                               CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   RTCORBA::RTORB::InvalidThreadpool))
{
  TAO_Thread_Pool *thread_pool = 0;

  // Unbind the thread pool from the map.
  int result =
    this->thread_pools_.unbind (thread_pool_id,
                                thread_pool);

  // If the thread pool is not found in our map.
  if (result != 0)
    ACE_THROW (RTCORBA::RTORB::InvalidThreadpool ());

  // Delete the thread pool.
  delete thread_pool;
}

TAO_Thread_Pool_Manager::THREAD_POOLS &
TAO_Thread_Pool_Manager::thread_pools (void)
{
  return this->thread_pools_;
}

#endif /* TAO_HAS_RT_CORBA == 1 */
