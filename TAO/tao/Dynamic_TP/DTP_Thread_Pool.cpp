// $Id$

#include "tao/Dynamic_TP/DTP_Thread_Pool.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#if ! defined (__ACE_INLINE__)
#include "tao/Dynamic_TP/DTP_Thread_Pool.inl"
#endif /* __ACE_INLINE__ */

#include "tao/Exception.h"
#include "tao/ORB_Core.h"
#include "tao/ORB_Core_TSS_Resources.h"
#include "tao/TSS_Resources.h"
#include "tao/ORB.h"
#include "tao/Acceptor_Registry.h"
#include "tao/debug.h"
#include "tao/LF_Follower.h"
#include "tao/Leader_Follower.h"
#include "ace/Auto_Ptr.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_DTP_New_Leader_Generator::TAO_DTP_New_Leader_Generator (
  TAO_DTP_Thread_Lane &lane)
  : lane_ (lane)
{
}

void
TAO_DTP_New_Leader_Generator::no_leaders_available (void)
{
  // Request a new dynamic thread from the Thread Lane
  this->lane_.new_dynamic_thread ();
}

TAO_DTP_Thread_Pool_Threads::TAO_DTP_Thread_Pool_Threads (TAO_DTP_Thread_Lane &lane)
  : ACE_Task_Base (lane.pool ().manager ().orb_core ().thr_mgr ()),
    lane_ (lane)
{
}

int
TAO_DTP_Thread_Pool_Threads::svc (void)
{
  TAO_ORB_Core &orb_core =
    this->lane ().pool ().manager ().orb_core ();

  if (orb_core.has_shutdown ())
    return 0;

  try
    {
      // Do the work
      this->run (orb_core);
    }
  catch (const ::CORBA::Exception& ex)
    {
      // No point propagating this exception.  Print it out.
      ACE_ERROR ((LM_ERROR,
                  "orb->run() raised exception for thread %t\n"));

      ex._tao_print_exception ("");
    }

  return 0;
}

int
TAO_DTP_Thread_Pool_Threads::run (TAO_ORB_Core &orb_core)
{
  CORBA::ORB_ptr orb = orb_core.orb ();
  // A timeout is specified, run the ORB in an idle loop, if we
  // don't handle any operations for the given timeout we just
  // exit the loop and this thread ends itself.
  ACE_Time_Value tv (this->lane_.dynamic_thread_time ());
  while (!orb_core.has_shutdown () && orb->work_pending (tv))
    {
      // Run the ORB for the specified timeout, this prevents looping
      // between work_pending/handle_events
      tv = this->lane_.dynamic_thread_time ();
      orb->run (tv);
      // Reset the idle timeout
      tv = this->lane_.dynamic_thread_time ();
    }

  if (TAO_debug_level > 7)
    {
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

TAO_DTP_Thread_Lane::TAO_DTP_Thread_Lane (TAO_DTP_Thread_Pool &pool,
                                          CORBA::ULong id,
                                          CORBA::ULong minimum_threads,
                                          CORBA::ULong initial_threads,
                                          CORBA::ULong maximum_threads,
                                          ACE_Time_Value const &dynamic_thread_time)
  : pool_ (pool),
    id_ (id),
    shutdown_ (false),
    minimum_threads_number_ (minimum_threads),
    initial_threads_number_ (initial_threads),
    maximum_threads_number_ (maximum_threads),
    threads_ (*this),
    new_thread_generator_ (*this),
    resources_ (pool.manager ().orb_core (),
                &new_thread_generator_),
    dynamic_thread_time_ (dynamic_thread_time)
{
}

bool
TAO_DTP_Thread_Lane::new_dynamic_thread (void)
{
  // Note that we are checking this condition below without the lock
  // held.
  if (this->threads_.thr_count () >= this->maximum_threads_number_)
    return false;

  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                    mon,
                    this->lock_,
                    false);

  TAO_DTP_Thread_Pool_Manager &manager = this->pool_.manager ();

  if (!manager.orb_core ().has_shutdown () && !this->shutdown_&&
      this->threads_.thr_count () < this->maximum_threads_number_)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO Process %P Pool %d Lane %d Thread %t\n")
                    ACE_TEXT ("Current number of threads = %d; ")
                    ACE_TEXT ("static threads = %d; max dynamic threads = %d\n")
                    ACE_TEXT ("No leaders available; creating new leader!\n"),
                    this->pool_.id (),
                    this->id_,
                    this->threads_.thr_count (),
                    this->minimum_threads_number_,
                    this->maximum_threads_number_));

      int result =
        this->create_threads_i (this->threads_,
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
TAO_DTP_Thread_Lane::shutting_down (void)
{
  ACE_GUARD (TAO_SYNCH_MUTEX,
             mon,
             this->lock_);

  // We are shutting down, this way we are not creating any more new dynamic
  // threads
  this->shutdown_ = true;
}


void
TAO_DTP_Thread_Lane::open (void)
{

  char pool_lane_id[10];
  TAO_ORB_Parameters *params =
    this->pool ().manager ().orb_core ().orb_params ();
  TAO_EndpointSet endpoint_set;

  // Create a string just *:* which means all pools all thread id's
  ACE_OS::sprintf (pool_lane_id,
                   "*:*");

  // Get the endpoints for all
  params->get_endpoint_set (pool_lane_id, endpoint_set);

  // Create a string with pool:* which means all lanes for this pool
  ACE_OS::sprintf (pool_lane_id,
                   "%d:*",
                   this->pool ().id ());

  // Get the endpoints for this pool.
  params->get_endpoint_set (pool_lane_id, endpoint_set);

  // Create a string with *:lane which means a lan of all pools
  ACE_OS::sprintf (pool_lane_id,
                   "*:%d",
                   this->id ());

  // Get the endpoints for this lane.
  params->get_endpoint_set (pool_lane_id, endpoint_set);

  // Create a string with the pool:thread id.
  ACE_OS::sprintf (pool_lane_id,
                   "%d:%d",
                   this->pool ().id (),
                   this->id ());

  // Get the endpoints for this lane.
  params->get_endpoint_set (pool_lane_id, endpoint_set);

  bool ignore_address = false;

  if (endpoint_set.is_empty ())
    {
      // If endpoints are not specified for this lane, use the
      // endpoints specified for the default lane but ignore their
      // addresses.
      params->get_endpoint_set (TAO_DEFAULT_LANE, endpoint_set);

      ignore_address = true;
    }
  else
    {
      // If endpoints are specified for this lane, use them with their
      // addresses.
      ignore_address = false;
    }

  // Open the acceptor registry.
  int const result =
    this->resources_.open_acceptor_registry (endpoint_set, ignore_address);

  if (result == -1)
    throw ::CORBA::INTERNAL (
                 CORBA::SystemException::_tao_minor_code (
                   TAO_ACCEPTOR_REGISTRY_OPEN_LOCATION_CODE,
                   0),
                 CORBA::COMPLETED_NO);
}

TAO_DTP_Thread_Lane::~TAO_DTP_Thread_Lane (void)
{
}

void
TAO_DTP_Thread_Lane::finalize (void)
{
  // Finalize resources.
  this->resources_.finalize ();
}

void
TAO_DTP_Thread_Lane::shutdown_reactor (void)
{
  this->resources_.shutdown_reactor ();
}

void
TAO_DTP_Thread_Lane::wait (void)
{
  this->threads_.wait ();
}

int
TAO_DTP_Thread_Lane::is_collocated (const TAO_MProfile &mprofile)
{
  return this->resources_.is_collocated (mprofile);
}

CORBA::ULong
TAO_DTP_Thread_Lane::current_threads (void) const
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                    mon,
                    this->lock_,
                    0);

  return (this->threads_.thr_count ());
}


int
TAO_DTP_Thread_Lane::create_initial_threads (void)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                    mon,
                    this->lock_,
                    0);

  // Create initial threads.
  // first, create the minimum number of threads as static
  int result = this->create_threads_i (this->threads_,
                                       this->minimum_threads_number_,
                                       THR_NEW_LWP | THR_JOINABLE);
  if (result != -1 && this->initial_threads_number_ > this->minimum_threads_number_)
    {
      CORBA::ULong count = this->initial_threads_number_ - this->minimum_threads_number_;
      result = this->create_threads_i (this->threads_,
                                       count,
                                       THR_BOUND | THR_DETACHED);
    }
  return result;
}

int
TAO_DTP_Thread_Lane::create_dynamic_threads (CORBA::ULong number_of_threads)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                    mon,
                    this->lock_,
                    0);

  return this->create_threads_i (this->threads_,
                                 number_of_threads,
                                 THR_BOUND | THR_DETACHED);
}

int
TAO_DTP_Thread_Lane::create_threads_i (TAO_DTP_Thread_Pool_Threads &thread_pool,
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

  int default_priority = 0;

  // Activate the threads.
  int result =
    thread_pool.activate (flags,
                          number_of_threads,
                          force_active,
                          default_grp_id,
                          default_priority,
                          default_task,
                          default_thread_handles,
                          default_stack,
                          stack_size_array);

  if (result != 0)
    return result;

  return result;
}

TAO_DTP_Thread_Pool::TAO_DTP_Thread_Pool (TAO_DTP_Thread_Pool_Manager &manager,
                                          CORBA::ULong id,
                                          TAO_DTP_Definition &def)
  : manager_ (manager),
    id_ (id),
    definition_ (def)
{
  // Create one lane.
  ACE_NEW (this->lane_,
           TAO_DTP_Thread_Lane (*this,
                                0,
                                def.min_threads_,
                                def.init_threads_,
                                def.max_threads_,
                                def.timeout_
                                ));
}

void
TAO_DTP_Thread_Pool::open (void)
{
  this->lane_->open ();
}

TAO_DTP_Thread_Pool::~TAO_DTP_Thread_Pool (void)
{
  delete this->lane_;
}

void
TAO_DTP_Thread_Pool::finalize (void)
{
  this->lane_->finalize ();
}

void
TAO_DTP_Thread_Pool::shutdown_reactor (void)
{
  this->lane_->shutdown_reactor ();
}

void
TAO_DTP_Thread_Pool::shutting_down (void)
{
  this->lane_->shutting_down ();
}


void
TAO_DTP_Thread_Pool::wait (void)
{
  this->lane_->wait ();
}

int
TAO_DTP_Thread_Pool::is_collocated (const TAO_MProfile &mprofile)
{
  return this->lane_->is_collocated (mprofile);
}

int
TAO_DTP_Thread_Pool::create_initial_threads (void)
{
  return this->lane_->create_initial_threads ();
}

#define TAO_THREAD_POOL_MANAGER_GUARD \
  ACE_GUARD_THROW_EX ( \
    TAO_SYNCH_MUTEX, \
    mon, \
    this->lock_, \
    CORBA::INTERNAL ( \
      CORBA::SystemException::_tao_minor_code ( \
        TAO_GUARD_FAILURE, \
        0), \
      CORBA::COMPLETED_NO));

TAO_DTP_Thread_Pool_Manager::TAO_DTP_Thread_Pool_Manager (TAO_ORB_Core &orb_core)
  : orb_core_ (orb_core),
    thread_pools_ (),
    thread_pool_id_counter_ (1),
    lock_ ()
{
}

TAO_DTP_Thread_Pool_Manager::~TAO_DTP_Thread_Pool_Manager (void)
{
  // Delete all the pools.
  for (THREAD_POOLS::ITERATOR iterator = this->thread_pools_.begin ();
       iterator != this->thread_pools_.end ();
       ++iterator)
    delete (*iterator).int_id_;
}

void
TAO_DTP_Thread_Pool_Manager::finalize (void)
{
  // Finalize all the pools.
  for (THREAD_POOLS::ITERATOR iterator = this->thread_pools_.begin ();
       iterator != this->thread_pools_.end ();
       ++iterator)
    (*iterator).int_id_->finalize ();
}

void
TAO_DTP_Thread_Pool_Manager::shutdown_reactor (void)
{
  // Finalize all the pools.
  for (THREAD_POOLS::ITERATOR iterator = this->thread_pools_.begin ();
       iterator != this->thread_pools_.end ();
       ++iterator)
    (*iterator).int_id_->shutdown_reactor ();
}

void
TAO_DTP_Thread_Pool_Manager::wait (void)
{
  // Finalize all the pools.
  for (THREAD_POOLS::ITERATOR iterator = this->thread_pools_.begin ();
       iterator != this->thread_pools_.end ();
       ++iterator)
    (*iterator).int_id_->wait ();
}

int
TAO_DTP_Thread_Pool_Manager::is_collocated (const TAO_MProfile &mprofile)
{
  // Finalize all the pools.
  for (THREAD_POOLS::ITERATOR iterator = this->thread_pools_.begin ();
       iterator != this->thread_pools_.end ();
       ++iterator)
    {
      int const result = (*iterator).int_id_->is_collocated (mprofile);

      if (result)
        return result;
    }

  return 0;
}

CORBA::ULong
TAO_DTP_Thread_Pool_Manager::create_threadpool (TAO_DTP_Definition &def)
{
  TAO_THREAD_POOL_MANAGER_GUARD;

  return this->create_threadpool_i (def);
}

void
TAO_DTP_Thread_Pool_Manager::destroy_threadpool (CORBA::ULong threadpool)
{
  TAO_DTP_Thread_Pool *tao_thread_pool = 0;

  // The guard is just for the map, don't do a wait inside the guard, because
  // during the wait other threads can try to access the thread pool manager
  // also, this can be one of the threads we are waiting for, which then
  // results in a deadlock
  {
    TAO_THREAD_POOL_MANAGER_GUARD;

    // Unbind the thread pool from the map.
    int const result = this->thread_pools_.unbind (threadpool, tao_thread_pool);

    // If the thread pool is not found in our map.
    if (result != 0)
      return; //throw RTCORBA::RTORB::InvalidThreadpool ();
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

CORBA::ULong
TAO_DTP_Thread_Pool_Manager::create_threadpool_i (TAO_DTP_Definition &def)
{
  // Create the thread pool.
  TAO_DTP_Thread_Pool *thread_pool = 0;

  ACE_NEW_THROW_EX (thread_pool,
                    TAO_DTP_Thread_Pool (*this,
                                     this->thread_pool_id_counter_,
                                     def
                                    ),
                    CORBA::NO_MEMORY ());

  return this->create_threadpool_helper (thread_pool);
}

CORBA::ULong
TAO_DTP_Thread_Pool_Manager::create_threadpool_helper (TAO_DTP_Thread_Pool *thread_pool)
{
  // Make sure of safe deletion in case of errors.
  auto_ptr<TAO_DTP_Thread_Pool> safe_thread_pool (thread_pool);

  // Open the pool.
  thread_pool->open ();

  // Create the static threads.
  int result = thread_pool->create_initial_threads ();

  // Throw exception in case of errors.
  if (result != 0)
    {
      // Finalize thread pool related resources.
      thread_pool->finalize ();

      throw ::CORBA::INTERNAL
        (
         CORBA::SystemException::_tao_minor_code
         (
          0, //TAO_RTCORBA_THREAD_CREATION_LOCATION_CODE,
          errno),
        CORBA::COMPLETED_NO);
    }

  // Bind thread to internal table.
  result = this->thread_pools_.bind (this->thread_pool_id_counter_, thread_pool);

  // Throw exceptin in case of errors.
  if (result != 0)
    throw ::CORBA::INTERNAL ();

  //
  // Success.
  //

  // No need to delete thread pool.
  safe_thread_pool.release ();

  // Return current counter and perform post-increment.
  return this->thread_pool_id_counter_++;
}

TAO_DTP_Thread_Pool *
TAO_DTP_Thread_Pool_Manager::get_threadpool (CORBA::ULong thread_pool_id )
{
  TAO_THREAD_POOL_MANAGER_GUARD;

  TAO_DTP_Thread_Pool *thread_pool = 0;
  int const result = thread_pools_.find (thread_pool_id, thread_pool);

  ACE_UNUSED_ARG (result);

  return thread_pool;
}

TAO_ORB_Core &
TAO_DTP_Thread_Pool_Manager::orb_core (void) const
{
  return this->orb_core_;
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */
