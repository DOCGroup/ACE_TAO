// $Id$

#include "Thread_Pool.h"

ACE_RCSID(tao, Thread_Pool, "$Id$")

#include "tao/Exception.h"
#include "ace/Auto_Ptr.h"
#include "tao/ORB_Core.h"
#include "tao/Acceptor_Registry.h"
#include "tao/Transport_Cache_Manager.h"
#include "tao/debug.h"
#include "tao/RTCORBA/Priority_Mapping_Manager.h"
#include "tao/Leader_Follower.h"

#if !defined (__ACE_INLINE__)
# include "Thread_Pool.i"
#endif /* ! __ACE_INLINE__ */

TAO_RT_New_Leader_Generator::TAO_RT_New_Leader_Generator (TAO_Thread_Lane &lane)
  : lane_ (lane)
{
}

void
TAO_RT_New_Leader_Generator::no_leaders_available (void)
{
  // Note that we are checking this condition below without the lock
  // held.  The value of <static_threads> and <dynamic_threads> does
  // not change, but <current_threads> increases when new dynamic
  // threads are created.  Even if we catch <current_threads> in an
  // inconsistent state, we will double check later with the lock
  // held.  Therefore, this check should not cause any big problems.
  if (this->lane_.current_threads () ==
      this->lane_.static_threads () +
      this->lane_.dynamic_threads ())
    return;

  TAO_Thread_Pool_Manager &manager =
    this->lane_.pool ().manager ();

  ACE_GUARD (ACE_SYNCH_MUTEX,
             mon,
             manager.lock ());

  if (this->lane_.current_threads () <
      (this->lane_.static_threads () +
       this->lane_.dynamic_threads ()) &&
      !manager.orb_core ().has_shutdown ())
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO Process %P Pool %d Lane %d Thread %t\n")
                    ACE_TEXT ("Current number of threads = %d; static threads = %d; dynamic threads = %d\n")
                    ACE_TEXT ("No leaders available; creating new leader!\n"),
                    this->lane_.pool ().id (),
                    this->lane_.id (),
                    this->lane_.current_threads (),
                    this->lane_.static_threads (),
                    this->lane_.dynamic_threads ()));

      int result =
        this->lane_.create_dynamic_threads (1);

      if (result != 0)
        ACE_ERROR ((LM_ERROR,
                    "Pool %d Lane %d Thread %t: cannot create dynamic thread\n",
                    this->lane_.pool ().id (),
                    this->lane_.id ()));
    }
}

TAO_Thread_Pool_Threads::TAO_Thread_Pool_Threads (TAO_Thread_Lane &lane)
  : ACE_Task_Base (lane.pool ().manager ().orb_core ().thr_mgr ()),
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
  TAO_ORB_Core &orb_core =
    this->lane ().pool ().manager ().orb_core ();

  if (orb_core.has_shutdown ())
    return 0;

  // Set TSS resources for this thread.
  TAO_Thread_Pool_Threads::set_tss_resources (orb_core,
                                              this->lane_);

  CORBA::ORB_ptr orb =
    orb_core.orb ();

  ACE_TRY_NEW_ENV
    {
      // Run the ORB.
      orb->run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      // No point propagating this exception.  Print it out.
      ACE_ERROR ((LM_ERROR,
                  "orb->run() raised exception for thread %t\n"));

      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "");
    }
  ACE_ENDTRY;

  return 0;
}

void
TAO_Thread_Pool_Threads::set_tss_resources (TAO_ORB_Core &orb_core,
                                            TAO_Thread_Lane &thread_lane)
{
  /// Get the ORB_Core's TSS resources.
  TAO_ORB_Core_TSS_Resources &tss =
    *orb_core.get_tss_resources ();

  /// Set the lane attribute in TSS.
  tss.lane_ = &thread_lane;
}

TAO_Thread_Lane::TAO_Thread_Lane (TAO_Thread_Pool &pool,
                                  CORBA::ULong id,
                                  CORBA::Short lane_priority,
                                  CORBA::ULong static_threads,
                                  CORBA::ULong dynamic_threads
                                  ACE_ENV_ARG_DECL_NOT_USED)
  : pool_ (pool),
    id_ (id),
    lane_priority_ (lane_priority),
    static_threads_ (static_threads),
    dynamic_threads_ (dynamic_threads),
    current_threads_ (0),
    threads_ (*this),
    new_thread_generator_ (*this),
    resources_ (pool.manager ().orb_core (),
                &new_thread_generator_),
    native_priority_ (TAO_INVALID_PRIORITY)
{
}

void
TAO_Thread_Lane::validate_and_map_priority (ACE_ENV_SINGLE_ARG_DECL)
{
  // Check that the priority is in bounds.
  if (this->lane_priority_ < RTCORBA::minPriority ||
      this->lane_priority_ > RTCORBA::maxPriority)
    ACE_THROW (CORBA::BAD_PARAM ());

  CORBA::ORB_ptr orb =
    this->pool_.manager ().orb_core ().orb ();

  // Get the priority mapping manager.
  CORBA::Object_var obj =
    orb->resolve_initial_references (TAO_OBJID_PRIORITYMAPPINGMANAGER
                                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  TAO_Priority_Mapping_Manager_var mapping_manager =
    TAO_Priority_Mapping_Manager::_narrow (obj.in ()
                                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  RTCORBA::PriorityMapping *pm =
    mapping_manager.in ()->mapping ();

  // Map CORBA priority to native priority.
  CORBA::Boolean result =
    pm->to_native (this->lane_priority_,
                   this->native_priority_);

  if (!result)
    ACE_THROW (CORBA::DATA_CONVERSION ());

  if (TAO_debug_level > 3)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("TAO (%P|%t) - creating thread at ")
                ACE_TEXT ("(corba:native) priority %d:%d\n"),
                this->lane_priority_,
                this->native_priority_));
}

void
TAO_Thread_Lane::open (ACE_ENV_SINGLE_ARG_DECL)
{
  // Validate and map priority.
  this->validate_and_map_priority (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // Open the acceptor registry.
  int result = 0;
  result =
    this->resources_.open_acceptor_registry (1
                                             ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (result == -1)
    ACE_THROW (CORBA::INTERNAL (
                 CORBA_SystemException::_tao_minor_code (
                   TAO_ACCEPTOR_REGISTRY_OPEN_LOCATION_CODE,
                   0),
                 CORBA::COMPLETED_NO));
}

TAO_Thread_Lane::~TAO_Thread_Lane (void)
{
}

void
TAO_Thread_Lane::finalize (void)
{
  // Finalize resources.
  this->resources_.finalize ();
}

void
TAO_Thread_Lane::shutdown_reactor (void)
{
  this->resources_.shutdown_reactor ();
}

void
TAO_Thread_Lane::wait (void)
{
  this->threads_.wait ();
}

int
TAO_Thread_Lane::is_collocated (const TAO_MProfile &mprofile)
{
  return this->resources_.is_collocated (mprofile);
}

int
TAO_Thread_Lane::create_static_threads (void)
{
  // Create static threads.
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
    stack_size_array[index] =
      this->pool ().stack_size_;

  // Make sure the dynamically created stack size array is properly
  // deleted.
  ACE_Auto_Basic_Array_Ptr<size_t> auto_stack_size_array (stack_size_array);

  TAO_ORB_Core &orb_core =
    this->pool ().manager ().orb_core ();

  long flags =
    default_flags |
    orb_core.orb_params ()->scope_policy () |
    orb_core.orb_params ()->sched_policy ();

  // Activate the threads.
  int result =
    this->threads_.activate (flags,
                             number_of_threads,
                             force_active,
                             this->native_priority_,
                             default_grp_id,
                             default_task,
                             default_thread_handles,
                             default_stack,
                             stack_size_array);

  if (result != 0)
    return result;

  this->current_threads_ +=
    number_of_threads;

  return result;
}

CORBA::ULong
TAO_Thread_Lane::id (void) const
{
  return this->id_;
}

TAO_Thread_Pool &
TAO_Thread_Lane::pool (void) const
{
  return this->pool_;
}

CORBA::Short
TAO_Thread_Lane::lane_priority (void) const
{
  return this->lane_priority_;
}

CORBA::Short
TAO_Thread_Lane::native_priority (void) const
{
  return this->native_priority_;
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

CORBA::ULong
TAO_Thread_Lane::current_threads (void) const
{
  return this->current_threads_;
}

void
TAO_Thread_Lane::current_threads (CORBA::ULong current_threads)
{
  this->current_threads_ = current_threads;
}

TAO_Thread_Pool_Threads &
TAO_Thread_Lane::threads (void)
{
  return this->threads_;
}

TAO_Thread_Lane_Resources &
TAO_Thread_Lane::resources (void)
{
  return this->resources_;
}

TAO_Thread_Pool::TAO_Thread_Pool (TAO_Thread_Pool_Manager &manager,
                                  CORBA::ULong id,
                                  CORBA::ULong stack_size,
                                  CORBA::ULong static_threads,
                                  CORBA::ULong dynamic_threads,
                                  CORBA::Short default_priority,
                                  CORBA::Boolean allow_request_buffering,
                                  CORBA::ULong max_buffered_requests,
                                  CORBA::ULong max_request_buffer_size
                                  ACE_ENV_ARG_DECL)
  : manager_ (manager),
    id_ (id),
    stack_size_ (stack_size),
    allow_borrowing_ (0),
    allow_request_buffering_ (allow_request_buffering),
    max_buffered_requests_ (max_buffered_requests),
    max_request_buffer_size_ (max_request_buffer_size),
    lanes_ (0),
    number_of_lanes_ (1),
    with_lanes_ (0)
{
  // No support for buffering.
  if (allow_request_buffering)
    ACE_THROW (CORBA::NO_IMPLEMENT ());

  // Create one lane.
  this->lanes_ = new TAO_Thread_Lane *[this->number_of_lanes_];
  this->lanes_[0] =
    new TAO_Thread_Lane (*this,
                         0,
                         default_priority,
                         static_threads,
                         dynamic_threads
                         ACE_ENV_ARG_PARAMETER);
}

TAO_Thread_Pool::TAO_Thread_Pool (TAO_Thread_Pool_Manager &manager,
                                  CORBA::ULong id,
                                  CORBA::ULong stack_size,
                                  const RTCORBA::ThreadpoolLanes &lanes,
                                  CORBA::Boolean allow_borrowing,
                                  CORBA::Boolean allow_request_buffering,
                                  CORBA::ULong max_buffered_requests,
                                  CORBA::ULong max_request_buffer_size
                                  ACE_ENV_ARG_DECL)
  : manager_ (manager),
    id_ (id),
    stack_size_ (stack_size),
    allow_borrowing_ (allow_borrowing),
    allow_request_buffering_ (allow_request_buffering),
    max_buffered_requests_ (max_buffered_requests),
    max_request_buffer_size_ (max_request_buffer_size),
    lanes_ (0),
    number_of_lanes_ (lanes.length ()),
    with_lanes_ (1)
{
  // No support for buffering or borrowing.
  if (allow_borrowing ||
      allow_request_buffering)
    ACE_THROW (CORBA::NO_IMPLEMENT ());

  // Create multiple lane.
  this->lanes_ = new TAO_Thread_Lane *[this->number_of_lanes_];
  for (CORBA::ULong i = 0;
       i != this->number_of_lanes_;
       ++i)
    this->lanes_[i] =
      new TAO_Thread_Lane (*this,
                           i,
                           lanes[i].lane_priority,
                           lanes[i].static_threads,
                           lanes[i].dynamic_threads
                           ACE_ENV_ARG_PARAMETER);
}

void
TAO_Thread_Pool::open (ACE_ENV_SINGLE_ARG_DECL)
{
  // Open all the lanes.
  for (CORBA::ULong i = 0;
       i != this->number_of_lanes_;
       ++i)
    {
      this->lanes_[i]->open (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
    }
}

TAO_Thread_Pool::~TAO_Thread_Pool (void)
{
  // Delete all the lanes.
  for (CORBA::ULong i = 0;
       i != this->number_of_lanes_;
       ++i)
    delete this->lanes_[i];

  delete[] this->lanes_;
}

void
TAO_Thread_Pool::finalize (void)
{
  // Finalize all the lanes.
  for (CORBA::ULong i = 0;
       i != this->number_of_lanes_;
       ++i)
    this->lanes_[i]->finalize ();
}

void
TAO_Thread_Pool::shutdown_reactor (void)
{
  // Finalize all the lanes.
  for (CORBA::ULong i = 0;
       i != this->number_of_lanes_;
       ++i)
    this->lanes_[i]->shutdown_reactor ();
}

void
TAO_Thread_Pool::wait (void)
{
  // Finalize all the lanes.
  for (CORBA::ULong i = 0;
       i != this->number_of_lanes_;
       ++i)
    this->lanes_[i]->wait ();
}

int
TAO_Thread_Pool::is_collocated (const TAO_MProfile &mprofile)
{
  // Finalize all the lanes.
  for (CORBA::ULong i = 0;
       i != this->number_of_lanes_;
       ++i)
    {
      int result =
        this->lanes_[i]->is_collocated (mprofile);

      if (result)
        return result;
    }

  return 0;
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

int
TAO_Thread_Pool::with_lanes (void) const
{
  return this->with_lanes_;
}

TAO_Thread_Pool_Manager &
TAO_Thread_Pool::manager (void) const
{
  return this->manager_;
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

TAO_Thread_Pool_Manager::TAO_Thread_Pool_Manager (TAO_ORB_Core &orb_core)
  : orb_core_ (orb_core),
    thread_pools_ (),
    thread_pool_id_counter_ (1),
    lock_ ()
{
}

TAO_Thread_Pool_Manager::~TAO_Thread_Pool_Manager (void)
{
  // Delete all the pools.
  for (THREAD_POOLS::ITERATOR iterator = this->thread_pools_.begin ();
       iterator != this->thread_pools_.end ();
       ++iterator)
    delete (*iterator).int_id_;
}

void
TAO_Thread_Pool_Manager::finalize (void)
{
  // Finalize all the pools.
  for (THREAD_POOLS::ITERATOR iterator = this->thread_pools_.begin ();
       iterator != this->thread_pools_.end ();
       ++iterator)
    (*iterator).int_id_->finalize ();
}

void
TAO_Thread_Pool_Manager::shutdown_reactor (void)
{
  // Finalize all the pools.
  for (THREAD_POOLS::ITERATOR iterator = this->thread_pools_.begin ();
       iterator != this->thread_pools_.end ();
       ++iterator)
    (*iterator).int_id_->shutdown_reactor ();
}

void
TAO_Thread_Pool_Manager::wait (void)
{
  // Finalize all the pools.
  for (THREAD_POOLS::ITERATOR iterator = this->thread_pools_.begin ();
       iterator != this->thread_pools_.end ();
       ++iterator)
    (*iterator).int_id_->wait ();
}

int
TAO_Thread_Pool_Manager::is_collocated (const TAO_MProfile &mprofile)
{
  // Finalize all the pools.
  for (THREAD_POOLS::ITERATOR iterator = this->thread_pools_.begin ();
       iterator != this->thread_pools_.end ();
       ++iterator)
    {
      int result =
        (*iterator).int_id_->is_collocated (mprofile);

      if (result)
        return result;
    }

  return 0;
}

RTCORBA::ThreadpoolId
TAO_Thread_Pool_Manager::create_threadpool (CORBA::ULong stacksize,
                                            CORBA::ULong static_threads,
                                            CORBA::ULong dynamic_threads,
                                            RTCORBA::Priority default_priority,
                                            CORBA::Boolean allow_request_buffering,
                                            CORBA::ULong max_buffered_requests,
                                            CORBA::ULong max_request_buffer_size
                                            ACE_ENV_ARG_DECL)
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
                                    max_request_buffer_size
                                    ACE_ENV_ARG_PARAMETER);
}

RTCORBA::ThreadpoolId
TAO_Thread_Pool_Manager::create_threadpool_with_lanes (CORBA::ULong stacksize,
                                                       const RTCORBA::ThreadpoolLanes & lanes,
                                                       CORBA::Boolean allow_borrowing,
                                                       CORBA::Boolean allow_request_buffering,
                                                       CORBA::ULong max_buffered_requests,
                                                       CORBA::ULong max_request_buffer_size
                                                       ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_THREAD_POOL_MANAGER_GUARD;
  ACE_CHECK_RETURN (0);

  return this->create_threadpool_with_lanes_i (stacksize,
                                               lanes,
                                               allow_borrowing,
                                               allow_request_buffering,
                                               max_buffered_requests,
                                               max_request_buffer_size
                                               ACE_ENV_ARG_PARAMETER);
}

void
TAO_Thread_Pool_Manager::destroy_threadpool (RTCORBA::ThreadpoolId threadpool
                                             ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   RTCORBA::RTORB::InvalidThreadpool))
{
  TAO_THREAD_POOL_MANAGER_GUARD;
  ACE_CHECK;

  this->destroy_threadpool_i (threadpool
                              ACE_ENV_ARG_PARAMETER);
}

RTCORBA::ThreadpoolId
TAO_Thread_Pool_Manager::create_threadpool_i (CORBA::ULong stacksize,
                                              CORBA::ULong static_threads,
                                              CORBA::ULong dynamic_threads,
                                              RTCORBA::Priority default_priority,
                                              CORBA::Boolean allow_request_buffering,
                                              CORBA::ULong max_buffered_requests,
                                              CORBA::ULong max_request_buffer_size
                                              ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Create the thread pool.
  TAO_Thread_Pool *thread_pool = 0;

  ACE_NEW_THROW_EX (thread_pool,
                    TAO_Thread_Pool (*this,
                                     this->thread_pool_id_counter_,
                                     stacksize,
                                     static_threads,
                                     dynamic_threads,
                                     default_priority,
                                     allow_request_buffering,
                                     max_buffered_requests,
                                     max_request_buffer_size
                                     ACE_ENV_ARG_PARAMETER),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  return this->create_threadpool_helper (thread_pool
                                         ACE_ENV_ARG_PARAMETER);
}

RTCORBA::ThreadpoolId
TAO_Thread_Pool_Manager::create_threadpool_with_lanes_i (CORBA::ULong stacksize,
                                                         const RTCORBA::ThreadpoolLanes &lanes,
                                                         CORBA::Boolean allow_borrowing,
                                                         CORBA::Boolean allow_request_buffering,
                                                         CORBA::ULong max_buffered_requests,
                                                         CORBA::ULong max_request_buffer_size
                                                         ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Create the thread pool.
  TAO_Thread_Pool *thread_pool = 0;

  ACE_NEW_THROW_EX (thread_pool,
                    TAO_Thread_Pool (*this,
                                     this->thread_pool_id_counter_,
                                     stacksize,
                                     lanes,
                                     allow_borrowing,
                                     allow_request_buffering,
                                     max_buffered_requests,
                                     max_request_buffer_size
                                     ACE_ENV_ARG_PARAMETER),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  return this->create_threadpool_helper (thread_pool
                                         ACE_ENV_ARG_PARAMETER);
}

RTCORBA::ThreadpoolId
TAO_Thread_Pool_Manager::create_threadpool_helper (TAO_Thread_Pool *thread_pool
                                                   ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Make sure of safe deletion in case of errors.
  auto_ptr<TAO_Thread_Pool> safe_thread_pool (thread_pool);

  // Open the pool.
  thread_pool->open (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  // Create the static threads.
  int result =
    thread_pool->create_static_threads ();

  // Throw exception in case of errors.
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
}

void
TAO_Thread_Pool_Manager::destroy_threadpool_i (RTCORBA::ThreadpoolId thread_pool_id
                                               ACE_ENV_ARG_DECL)
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

  // Shutdown reactor.
  thread_pool->shutdown_reactor ();

  // Wait for the threads.
  thread_pool->wait ();

  // Finalize resources.
  thread_pool->finalize ();

  // Delete the thread pool.
  delete thread_pool;
}

TAO_ORB_Core &
TAO_Thread_Pool_Manager::orb_core (void) const
{
  return this->orb_core_;
}

ACE_SYNCH_MUTEX &
TAO_Thread_Pool_Manager::lock (void)
{
  return this->lock_;
}

TAO_Thread_Pool_Manager::THREAD_POOLS &
TAO_Thread_Pool_Manager::thread_pools (void)
{
  return this->thread_pools_;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Hash_Map_Manager<RTCORBA::ThreadpoolId, TAO_Thread_Pool *, ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<RTCORBA::ThreadpoolId, TAO_Thread_Pool *, ACE_Hash<RTCORBA::ThreadpoolId>, ACE_Equal_To<RTCORBA::ThreadpoolId>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Entry<RTCORBA::ThreadpoolId, TAO_Thread_Pool *>;
template class ACE_Hash_Map_Iterator_Base_Ex<RTCORBA::ThreadpoolId, TAO_Thread_Pool *, ACE_Hash<RTCORBA::ThreadpoolId>, ACE_Equal_To<RTCORBA::ThreadpoolId>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<unsigned int, TAO_Thread_Pool *, ACE_Hash<unsigned int>, ACE_Equal_To<unsigned int>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<unsigned int, TAO_Thread_Pool *, ACE_Hash<unsigned int>, ACE_Equal_To<unsigned int>, ACE_Null_Mutex>;

template class ACE_Auto_Basic_Ptr<TAO_Thread_Pool>;
template class ACE_Auto_Basic_Array_Ptr<size_t>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Hash_Map_Manager<RTCORBA::ThreadpoolId, TAO_Thread_Pool *, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager_Ex<RTCORBA::ThreadpoolId, TAO_Thread_Pool *, ACE_Hash<RTCORBA::ThreadpoolId>, ACE_Equal_To<RTCORBA::ThreadpoolId>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Entry<RTCORBA::ThreadpoolId, TAO_Thread_Pool *>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<RTCORBA::ThreadpoolId, TAO_Thread_Pool *, ACE_Hash<RTCORBA::ThreadpoolId>, ACE_Equal_To<RTCORBA::ThreadpoolId>, ACE_Null_Mutex>

#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<unsigned int, TAO_Thread_Pool *, ACE_Hash<unsigned int>, ACE_Equal_To<unsigned int>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<unsigned int, TAO_Thread_Pool *, ACE_Hash<unsigned int>, ACE_Equal_To<unsigned int>, ACE_Null_Mutex>

#pragma instantiate ACE_Auto_Basic_Ptr<TAO_Thread_Pool>
#pragma instantiate ACE_Auto_Basic_Array_Ptr<size_t>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
