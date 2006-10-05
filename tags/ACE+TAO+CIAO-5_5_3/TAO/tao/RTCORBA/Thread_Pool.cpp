#include "tao/RTCORBA/Thread_Pool.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

ACE_RCSID (RTCORBA,
           Thread_Pool,
           "$Id$")

#if ! defined (__ACE_INLINE__)
#include "tao/RTCORBA/Thread_Pool.inl"
#endif /* __ACE_INLINE__ */

#include "tao/Exception.h"
#include "tao/ORB_Core.h"
#include "tao/ORB_Core_TSS_Resources.h"
#include "tao/ORB.h"
#include "tao/Acceptor_Registry.h"
#include "tao/Transport_Cache_Manager.h"
#include "tao/debug.h"
#include "tao/RTCORBA/Priority_Mapping_Manager.h"
#include "tao/LF_Follower.h"
#include "tao/Leader_Follower.h"
#include "ace/Auto_Ptr.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_RT_New_Leader_Generator::TAO_RT_New_Leader_Generator (
  TAO_Thread_Lane &lane)
  : lane_ (lane)
{
}

void
TAO_RT_New_Leader_Generator::no_leaders_available (void)
{
  // Request a new dynamic thread from the Thread Lane
  this->lane_.new_dynamic_thread ();
}

TAO_Thread_Pool_Threads::TAO_Thread_Pool_Threads (TAO_Thread_Lane &lane)
  : ACE_Task_Base (lane.pool ().manager ().orb_core ().thr_mgr ()),
    lane_ (lane)
{
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

  ACE_TRY_NEW_ENV
    {
      // Do the work
      this->run (orb_core);
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

int
TAO_Thread_Pool_Threads::run (TAO_ORB_Core &orb_core ACE_ENV_ARG_PARAMETER)
{
  CORBA::ORB_ptr orb = orb_core.orb ();

  // Run the ORB.
  orb->run (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

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

TAO_Dynamic_Thread_Pool_Threads::TAO_Dynamic_Thread_Pool_Threads (TAO_Thread_Lane &lane)
  : TAO_Thread_Pool_Threads (lane)
{
}

int
TAO_Dynamic_Thread_Pool_Threads::run (TAO_ORB_Core &orb_core ACE_ENV_ARG_PARAMETER)
{
  CORBA::ORB_ptr orb = orb_core.orb ();

  if (this->lane_.dynamic_thread_idle_timeout () == ACE_Time_Value::zero)
    {
      // No timeout specified, run the ORB until it shutdowns
      orb->run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  else
    {
      // A timeout is specified, run the ORB in an idle loop, if we
      // don't handle any operations for the given timeout we just
      // exit the loop and this thread ends itself.
      ACE_Time_Value tv (this->lane_.dynamic_thread_idle_timeout ());
      while (!orb_core.has_shutdown () && orb->work_pending (tv))
        {
          orb->perform_work ();
          tv = this->lane_.dynamic_thread_idle_timeout ();
        }

      if (TAO_debug_level > 7)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO Process %P Pool %d Lane %d Thread %t\n")
                    ACE_TEXT ("Current number of dynamic threads left = %d; ")
                    ACE_TEXT ("RTCorba worker thread is ending!\n"),
                    this->lane_.pool ().id (),
                    this->lane_.id (),
                    this->thr_count () - 1));
    }

  return 0;
}

TAO_Thread_Lane::TAO_Thread_Lane (TAO_Thread_Pool &pool,
                                  CORBA::ULong id,
                                  CORBA::Short lane_priority,
                                  CORBA::ULong static_threads,
                                  CORBA::ULong dynamic_threads,
                                  ACE_Time_Value const &dynamic_thread_idle_timeout
                                  ACE_ENV_ARG_DECL_NOT_USED)
  : pool_ (pool),
    id_ (id),
    lane_priority_ (lane_priority),
    shutdown_ (false),
    static_threads_number_ (static_threads),
    dynamic_threads_number_ (dynamic_threads),
    static_threads_ (*this),
    dynamic_threads_ (*this),
    new_thread_generator_ (*this),
    resources_ (pool.manager ().orb_core (),
                &new_thread_generator_),
    native_priority_ (TAO_INVALID_PRIORITY),
    dynamic_thread_idle_timeout_ (dynamic_thread_idle_timeout)
{
}

bool
TAO_Thread_Lane::new_dynamic_thread (void)
{
  // Note that we are checking this condition below without the lock
  // held.
  if (this->dynamic_threads_.thr_count () >= this->dynamic_threads_number_)
    return false;

  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX,
                    mon,
                    this->lock_,
                    false);

  TAO_Thread_Pool_Manager &manager =
    this->pool_.manager ();

  if (!manager.orb_core ().has_shutdown () && !this->shutdown_&&
      this->dynamic_threads_.thr_count () < this->dynamic_threads_number_)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO Process %P Pool %d Lane %d Thread %t\n")
                    ACE_TEXT ("Current number of dynamic threads = %d; ")
                    ACE_TEXT ("static threads = %d; max dynamic threads = %d\n")
                    ACE_TEXT ("No leaders available; creating new leader!\n"),
                    this->pool_.id (),
                    this->id_,
                    this->dynamic_threads_.thr_count (),
                    this->static_threads_number_,
                    this->dynamic_threads_number_));

      int result =
        this->create_threads_i (this->dynamic_threads_,
                                1,
                                THR_BOUND | THR_DETACHED);

      if (result != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                          ACE_TEXT ("Pool %d Lane %d Thread %t: ")
                          ACE_TEXT ("cannot create dynamic thread\n"),
                          this->pool_.id (),
                          this->id_),
                          false);
    }

  return true;
}

void
TAO_Thread_Lane::shutting_down (void)
{
  ACE_GUARD (ACE_SYNCH_MUTEX,
             mon,
             this->lock_);

  // We are shutting down, this way we are not creating any more new dynamic
  // threads
  this->shutdown_ = true;
}

void
TAO_Thread_Lane::validate_and_map_priority (ACE_ENV_SINGLE_ARG_DECL)
{
  // Make sure that static_threads_number_ is not zero.
  if (this->static_threads_number_ == 0)
    ACE_THROW (CORBA::BAD_PARAM ());

  // Check that the priority is in bounds.
  if (this->lane_priority_ < RTCORBA::minPriority
           // The line below will always be false unless the value of
           // RTCORBA::maxPriority, which is now assigned the value of
           // 32767, is changed in RTCORBA.pidl.
//      || this->lane_priority_ > RTCORBA::maxPriority
     )
    {
      ACE_THROW (CORBA::BAD_PARAM ());
    }

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
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) - creating thread at ")
                  ACE_TEXT ("(corba:native) priority %d:%d\n"),
                  this->lane_priority_,
                  this->native_priority_));
    }
}

void
TAO_Thread_Lane::open (ACE_ENV_SINGLE_ARG_DECL)
{
  // Validate and map priority.
  this->validate_and_map_priority (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // Create a string with the pool:thread id.
  char pool_lane_id[10];
  ACE_OS::sprintf (pool_lane_id,
                   "%d:%d",
                   this->pool ().id (),
                   this->id ());

  TAO_ORB_Parameters *params =
    this->pool ().manager ().orb_core ().orb_params ();

  TAO_EndpointSet endpoint_set;
  bool ignore_address;

  // Get the endpoints for this lane.
  params->get_endpoint_set (pool_lane_id,
                            endpoint_set);

  if (endpoint_set.is_empty ())
    {
      // If endpoints are not specified for this lane, use the
      // endpoints specified for the default lane but ignore their
      // addresses.
      params->get_endpoint_set (TAO_DEFAULT_LANE,
                                endpoint_set);

      ignore_address = true;
    }
  else
    {
      // If endpoints are specified for this lane, use them with thier
      // addresses.
      ignore_address = false;
    }

  // Open the acceptor registry.
  int result = 0;
  result =
    this->resources_.open_acceptor_registry (endpoint_set,
                                             ignore_address
                                             ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (result == -1)
    ACE_THROW (CORBA::INTERNAL (
                 CORBA::SystemException::_tao_minor_code (
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
  this->static_threads_.wait ();
  this->dynamic_threads_.wait ();
}

int
TAO_Thread_Lane::is_collocated (const TAO_MProfile &mprofile)
{
  return this->resources_.is_collocated (mprofile);
}

CORBA::ULong
TAO_Thread_Lane::current_threads (void) const
{
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX,
                    mon,
                    this->lock_,
                    0);

  return (this->static_threads_.thr_count () +
         this->dynamic_threads_.thr_count ());
}


int
TAO_Thread_Lane::create_static_threads (void)
{
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX,
                    mon,
                    this->lock_,
                    0);

  // Create static threads.
  return this->create_threads_i (this->static_threads_,
                                 this->static_threads_number_,
                                 THR_NEW_LWP | THR_JOINABLE);
}

int
TAO_Thread_Lane::create_dynamic_threads (CORBA::ULong number_of_threads)
{
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX,
                    mon,
                    this->lock_,
                    0);

  return this->create_threads_i (this->dynamic_threads_,
                                 number_of_threads,
                                 THR_BOUND | THR_DETACHED);
}

int
TAO_Thread_Lane::create_threads_i (TAO_Thread_Pool_Threads &thread_pool,
                                   CORBA::ULong number_of_threads,
                                   long thread_flags)
{
  // Overwritten parameters.
  int force_active = 1;

  // Default parameters.
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
      this->pool ().stack_size ();

  // Make sure the dynamically created stack size array is properly
  // deleted.
  ACE_Auto_Basic_Array_Ptr<size_t> auto_stack_size_array (stack_size_array);

  TAO_ORB_Core &orb_core =
    this->pool ().manager ().orb_core ();

  long flags =
    thread_flags |
    orb_core.orb_params ()->thread_creation_flags ();

  // Activate the threads.
  int result =
    thread_pool.activate (flags,
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

  return result;
}

TAO_Thread_Pool::TAO_Thread_Pool (TAO_Thread_Pool_Manager &manager,
                                  CORBA::ULong id,
                                  CORBA::ULong stack_size,
                                  CORBA::ULong static_threads,
                                  CORBA::ULong dynamic_threads,
                                  CORBA::Short default_priority,
                                  CORBA::Boolean allow_request_buffering,
                                  CORBA::ULong max_buffered_requests,
                                  CORBA::ULong max_request_buffer_size,
                                  ACE_Time_Value const &dynamic_thread_idle_timeout
                                  ACE_ENV_ARG_DECL)
  : manager_ (manager),
    id_ (id),
    stack_size_ (stack_size),
    allow_borrowing_ (0),
    allow_request_buffering_ (allow_request_buffering),
    max_buffered_requests_ (max_buffered_requests),
    max_request_buffer_size_ (max_request_buffer_size),
    dynamic_thread_idle_timeout_ (dynamic_thread_idle_timeout),
    lanes_ (0),
    number_of_lanes_ (1),
    with_lanes_ (false)
{
  // No support for buffering.
  if (allow_request_buffering)
    ACE_THROW (CORBA::NO_IMPLEMENT ());

  // Create one lane.
  ACE_NEW (this->lanes_,
           TAO_Thread_Lane *[this->number_of_lanes_]);
  ACE_NEW (this->lanes_[0],
           TAO_Thread_Lane (*this,
                            0,
                            default_priority,
                            static_threads,
                            dynamic_threads,
                            dynamic_thread_idle_timeout
                            ACE_ENV_ARG_PARAMETER));
}

TAO_Thread_Pool::TAO_Thread_Pool (TAO_Thread_Pool_Manager &manager,
                                  CORBA::ULong id,
                                  CORBA::ULong stack_size,
                                  const RTCORBA::ThreadpoolLanes &lanes,
                                  CORBA::Boolean allow_borrowing,
                                  CORBA::Boolean allow_request_buffering,
                                  CORBA::ULong max_buffered_requests,
                                  CORBA::ULong max_request_buffer_size,
                                  ACE_Time_Value const &dynamic_thread_idle_timeout
                                  ACE_ENV_ARG_DECL)
  : manager_ (manager),
    id_ (id),
    stack_size_ (stack_size),
    allow_borrowing_ (allow_borrowing),
    allow_request_buffering_ (allow_request_buffering),
    max_buffered_requests_ (max_buffered_requests),
    max_request_buffer_size_ (max_request_buffer_size),
    dynamic_thread_idle_timeout_ (dynamic_thread_idle_timeout),
    lanes_ (0),
    number_of_lanes_ (lanes.length ()),
    with_lanes_ (true)
{
  // No support for buffering or borrowing.
  if (allow_borrowing ||
      allow_request_buffering)
    ACE_THROW (CORBA::NO_IMPLEMENT ());

  // Create multiple lane.
  ACE_NEW (this->lanes_,
           TAO_Thread_Lane *[this->number_of_lanes_]);
  for (CORBA::ULong i = 0;
       i != this->number_of_lanes_;
       ++i)
    ACE_NEW (this->lanes_[i],
             TAO_Thread_Lane (*this,
                              i,
                              lanes[i].lane_priority,
                              lanes[i].static_threads,
                              lanes[i].dynamic_threads,
                              dynamic_thread_idle_timeout
                              ACE_ENV_ARG_PARAMETER));
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
TAO_Thread_Pool::shutting_down (void)
{
  // Finalize all the lanes.
  for (CORBA::ULong i = 0;
       i != this->number_of_lanes_;
       ++i)
    this->lanes_[i]->shutting_down ();
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

#define TAO_THREAD_POOL_MANAGER_GUARD \
  ACE_GUARD_THROW_EX ( \
    ACE_SYNCH_MUTEX, \
    mon, \
    this->lock_, \
    CORBA::INTERNAL ( \
      CORBA::SystemException::_tao_minor_code ( \
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
                                            CORBA::ULong max_request_buffer_size,
                                            ACE_Time_Value const &dynamic_thread_idle_timeout
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
                                    max_request_buffer_size,
                                    dynamic_thread_idle_timeout
                                    ACE_ENV_ARG_PARAMETER);
}

RTCORBA::ThreadpoolId
TAO_Thread_Pool_Manager::create_threadpool_with_lanes (CORBA::ULong stacksize,
                                                       const RTCORBA::ThreadpoolLanes & lanes,
                                                       CORBA::Boolean allow_borrowing,
                                                       CORBA::Boolean allow_request_buffering,
                                                       CORBA::ULong max_buffered_requests,
                                                       CORBA::ULong max_request_buffer_size,
                                                       ACE_Time_Value const &dynamic_thread_idle_timeout
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
                                               max_request_buffer_size,
                                               dynamic_thread_idle_timeout
                                               ACE_ENV_ARG_PARAMETER);
}

void
TAO_Thread_Pool_Manager::destroy_threadpool (RTCORBA::ThreadpoolId threadpool
                                             ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   RTCORBA::RTORB::InvalidThreadpool))
{
  TAO_Thread_Pool *tao_thread_pool = 0;

  // The guard is just for the map, don't do a wait inside the guard, because
  // during the wait other threads can try to access the thread pool manager
  // also, this can be one of the threads we are waiting for, which then
  // results in a deadlock
  {
    TAO_THREAD_POOL_MANAGER_GUARD;
    ACE_CHECK;

    // Unbind the thread pool from the map.
    int result =
      this->thread_pools_.unbind (threadpool,
                                  tao_thread_pool);

    // If the thread pool is not found in our map.
    if (result != 0)
      ACE_THROW (RTCORBA::RTORB::InvalidThreadpool ());
  }

  // Mark the thread pool that we are shutting down.
  tao_thread_pool->shutting_down ();

  // Shutdown reactor.
  tao_thread_pool->shutdown_reactor ();

  // Wait for the threads.
  tao_thread_pool->wait ();

  // Finalize resources.
  tao_thread_pool->finalize ();

  // Delete the thread pool.
  delete tao_thread_pool;

}

RTCORBA::ThreadpoolId
TAO_Thread_Pool_Manager::create_threadpool_i (CORBA::ULong stacksize,
                                              CORBA::ULong static_threads,
                                              CORBA::ULong dynamic_threads,
                                              RTCORBA::Priority default_priority,
                                              CORBA::Boolean allow_request_buffering,
                                              CORBA::ULong max_buffered_requests,
                                              CORBA::ULong max_request_buffer_size,
                                              ACE_Time_Value const &dynamic_thread_idle_timeout
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
                                     max_request_buffer_size,
                                     dynamic_thread_idle_timeout
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
                                                         CORBA::ULong max_request_buffer_size,
                                                         ACE_Time_Value const &dynamic_thread_idle_timeout
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
                                     max_request_buffer_size,
                                     dynamic_thread_idle_timeout
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
    {
      // Finalize thread pool related resources.
      thread_pool->finalize ();

      ACE_THROW_RETURN (
        CORBA::INTERNAL (
          CORBA::SystemException::_tao_minor_code (
            TAO_RTCORBA_THREAD_CREATION_LOCATION_CODE,
            errno),
          CORBA::COMPLETED_NO),
        result);
    }

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

TAO_Thread_Pool *
TAO_Thread_Pool_Manager::get_threadpool (RTCORBA::ThreadpoolId thread_pool_id ACE_ENV_ARG_DECL)
{
  TAO_THREAD_POOL_MANAGER_GUARD;
  ACE_CHECK_RETURN (0);

  TAO_Thread_Pool *thread_pool = 0;
  int result =
    thread_pools_.find (thread_pool_id,
                        thread_pool);

  ACE_UNUSED_ARG (result);

  return thread_pool;
}

TAO_ORB_Core &
TAO_Thread_Pool_Manager::orb_core (void) const
{
  return this->orb_core_;
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */
