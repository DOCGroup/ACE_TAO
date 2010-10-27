// $Id$

#include "tao/RTCORBA/RT_ORB.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#include "tao/RTCORBA/RT_Policy_i.h"
#include "tao/RTCORBA/RT_Mutex.h"
#include "tao/RTCORBA/Priority_Mapping_Manager.h"
#include "tao/ORB_Core.h"
#include "tao/ORB.h"
#include "tao/SystemException.h"
#include "tao/RTCORBA/Thread_Pool.h"
#include "tao/RTCORBA/RT_Thread_Lane_Resources_Manager.h"
#include "ace/Sched_Params.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_RT_ORB::TAO_RT_ORB (TAO_ORB_Core *orb_core,
                        TAO_RT_ORBInitializer::TAO_RTCORBA_DT_LifeSpan lifespan,
                        ACE_Time_Value const &dynamic_thread_time)
  : orb_core_ (orb_core),
    mutex_mgr_ (),
    tp_manager_ (0),
    lifespan_ (lifespan),
    dynamic_thread_time_ (dynamic_thread_time)
{
  TAO_Thread_Lane_Resources_Manager *thread_lane_resources_manager =
    &this->orb_core_->thread_lane_resources_manager ();

  TAO_RT_Thread_Lane_Resources_Manager *rt_thread_lane_resources_manager =
    dynamic_cast <TAO_RT_Thread_Lane_Resources_Manager *> (thread_lane_resources_manager);

  if (!rt_thread_lane_resources_manager)
    throw CORBA::INTERNAL ();

  this->tp_manager_ =
    &rt_thread_lane_resources_manager->tp_manager ();
}

TAO_RT_ORB::~TAO_RT_ORB (void)
{
}

RTCORBA::Mutex_ptr
TAO_RT_ORB::create_mutex (void)
{
  return this->mutex_mgr_.create_mutex ();
}

void
TAO_RT_ORB::destroy_mutex (RTCORBA::Mutex_ptr mutex)
{
  this->mutex_mgr_.destroy_mutex (mutex);
}


RTCORBA::Mutex_ptr
TAO_RT_ORB::create_named_mutex (const char *name,
                                CORBA::Boolean_out created_flag)
{
  return this->mutex_mgr_.create_named_mutex (name, created_flag);
}

RTCORBA::Mutex_ptr
TAO_RT_ORB::open_named_mutex (const char *name)
{
  return this->mutex_mgr_.open_named_mutex (name);
}

////////////////////////////////////////////////////////////////////////////////

TAO_Named_RT_Mutex_Manager::TAO_Named_RT_Mutex_Manager (void)
{
}

TAO_Named_RT_Mutex_Manager::~TAO_Named_RT_Mutex_Manager (void)
{
}

RTCORBA::Mutex_ptr
TAO_Named_RT_Mutex_Manager::create_mutex (void)
{
  TAO_RT_Mutex *mutex = 0;
  ACE_NEW_THROW_EX (mutex,
                    TAO_RT_Mutex (),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));

  return mutex;
}

// If Named RT_Mutexes aren't enabled, this function is a nop
// as also indicated by the comment below.
#if (TAO_HAS_NAMED_RT_MUTEXES == 1)
void
TAO_Named_RT_Mutex_Manager::destroy_mutex (RTCORBA::Mutex_ptr mutex)
{
  TAO_RT_Mutex *tao_mutex =
    dynamic_cast<TAO_RT_Mutex *> (mutex);

  // If this mutex is named, then we need to remove it from our table.
  // Otherwise, we don't have to do anything.
  const char *name = tao_mutex->name ();
  if (name != 0)
    {
      // The following should be atomic.
      ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX,
                          monitor,
                          this->lock_,
                          CORBA::INTERNAL ());

      int result =
        this->map_.unbind (name);

      if (result != 0)
        throw ::CORBA::INTERNAL ();
    }
}
#else /* TAO_HAS_NAMED_RT_MUTEXES == 1 */
void
TAO_Named_RT_Mutex_Manager::destroy_mutex (RTCORBA::Mutex_ptr)
{
}
#endif /* TAO_HAS_NAMED_RT_MUTEXES == 1 */

RTCORBA::Mutex_ptr
TAO_Named_RT_Mutex_Manager::create_named_mutex (const char *name,
                                                CORBA::Boolean_out created_flag)
{
#if (TAO_HAS_NAMED_RT_MUTEXES == 1)
  // The following should be atomic.
  ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX,
                      monitor,
                      this->lock_,
                      CORBA::INTERNAL ());

  // Optimistic that we'll find it.
  created_flag = false;

  // If we find the mutex, simply return it.
  RTCORBA::Mutex_var mutex;
  if (this->map_.find (name, mutex) != 0)
    {
      // Oops, we didn't find it.
      created_flag = true;

      RTCORBA::Mutex_ptr tmp_mutex;

      // Create a new one.
      ACE_NEW_THROW_EX (tmp_mutex,
                        TAO_Named_RT_Mutex (name),
                        CORBA::NO_MEMORY (
                          CORBA::SystemException::_tao_minor_code (
                            TAO::VMCID,
                            ENOMEM),
                          CORBA::COMPLETED_NO));

      mutex = tmp_mutex;

      // Add it to the map.
      int const result = this->map_.bind (name, mutex);

      if (result != 0)
        throw ::CORBA::INTERNAL ();
    }

  // Return the one we found or created.
  return mutex._retn ();
#else /* TAO_HAS_NAMED_RT_MUTEXES */
  ACE_UNUSED_ARG (name);
  ACE_UNUSED_ARG (created_flag);
  throw ::CORBA::NO_IMPLEMENT ();
#endif /* TAO_HAS_NAMED_RT_MUTEXES */
}

RTCORBA::Mutex_ptr
TAO_Named_RT_Mutex_Manager::open_named_mutex (const char *name)
{
#if (TAO_HAS_NAMED_RT_MUTEXES == 1)
  // The following should be atomic.
  ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX,
                      monitor,
                      this->lock_,
                      CORBA::INTERNAL ());

  // If we find the mutex, simply return it.
  RTCORBA::Mutex_var mutex;
  if (this->map_.find (name, mutex) != 0)
    throw RTCORBA::RTORB::MutexNotFound ();

  // Return the one we found.
  return mutex._retn ();
#else /* TAO_HAS_NAMED_RT_MUTEXES */
  ACE_UNUSED_ARG (name);
  throw ::CORBA::NO_IMPLEMENT ();
#endif /* TAO_HAS_NAMED_RT_MUTEXES */
}

///////////////////////////////////////////////////////////////////////////////

RTCORBA::TCPProtocolProperties_ptr
TAO_RT_ORB::create_tcp_protocol_properties (CORBA::Long send_buffer_size,
                                            CORBA::Long recv_buffer_size,
                                            CORBA::Boolean keep_alive,
                                            CORBA::Boolean dont_route,
                                            CORBA::Boolean no_delay,
                                            CORBA::Boolean enable_network_priority
                                            )
{
  TAO_TCP_Protocol_Properties *tmp = 0;
  ACE_NEW_THROW_EX (tmp,
                    TAO_TCP_Protocol_Properties (send_buffer_size,
                                        recv_buffer_size,
                                        keep_alive,
                                        dont_route,
                                        no_delay,
                                        enable_network_priority),
                    CORBA::NO_MEMORY (TAO::VMCID,
                                      CORBA::COMPLETED_NO));

  return tmp;
}

RTCORBA::UnixDomainProtocolProperties_ptr
TAO_RT_ORB::create_unix_domain_protocol_properties (
                                                    CORBA::Long send_buffer_size,
                                                    CORBA::Long recv_buffer_size)
{
  TAO_UnixDomain_Protocol_Properties *tmp = 0;
  ACE_NEW_THROW_EX (tmp,
                    TAO_UnixDomain_Protocol_Properties (
                                               send_buffer_size,
                                               recv_buffer_size),
                    CORBA::NO_MEMORY (TAO::VMCID,
                                      CORBA::COMPLETED_NO));

  return tmp;
}

RTCORBA::SharedMemoryProtocolProperties_ptr
TAO_RT_ORB::create_shared_memory_protocol_properties (
                                                      CORBA::Long send_buffer_size,
                                                      CORBA::Long recv_buffer_size,
                                                      CORBA::Boolean keep_alive,
                                                      CORBA::Boolean dont_route,
                                                      CORBA::Boolean no_delay,
                                                      CORBA::Long preallocate_buffer_size,
                                                      const char *mmap_filename,
                                                      const char *mmap_lockname)
{
  TAO_SharedMemory_Protocol_Properties *tmp = 0;
  ACE_NEW_THROW_EX (tmp,
                    TAO_SharedMemory_Protocol_Properties (send_buffer_size,
                                                          recv_buffer_size,
                                                          keep_alive,
                                                          dont_route,
                                                          no_delay,
                                                          preallocate_buffer_size,
                                                          mmap_filename,
                                                          mmap_lockname),
                    CORBA::NO_MEMORY (TAO::VMCID,
                                      CORBA::COMPLETED_NO));

  return tmp;
}

RTCORBA::UserDatagramProtocolProperties_ptr
TAO_RT_ORB::create_user_datagram_protocol_properties (
                                                      CORBA::Long send_buffer_size,
                                                      CORBA::Long recv_buffer_size,
                                                      CORBA::Boolean enable_network_priority)
{
  TAO_UserDatagram_Protocol_Properties *tmp = 0;
  ACE_NEW_THROW_EX (tmp,
                    TAO_UserDatagram_Protocol_Properties (
                                                 send_buffer_size,
                                                 recv_buffer_size,
                                                 enable_network_priority),
                    CORBA::NO_MEMORY (TAO::VMCID,
                                      CORBA::COMPLETED_NO));

  return tmp;
}

RTCORBA::StreamControlProtocolProperties_ptr
TAO_RT_ORB::create_stream_control_protocol_properties (
                                                       CORBA::Long send_buffer_size,
                                                       CORBA::Long recv_buffer_size,
                                                       CORBA::Boolean keep_alive,
                                                       CORBA::Boolean dont_route,
                                                       CORBA::Boolean no_delay,
                                                       CORBA::Boolean enable_network_priority
                                                       )
{
  TAO_StreamControl_Protocol_Properties *tmp = 0;
  ACE_NEW_THROW_EX (tmp,
                    TAO_StreamControl_Protocol_Properties (
                                                  send_buffer_size,
                                                  recv_buffer_size,
                                                  keep_alive,
                                                  dont_route,
                                                  no_delay,
                                                  enable_network_priority),
                    CORBA::NO_MEMORY (TAO::VMCID,
                                      CORBA::COMPLETED_NO));

  return tmp;
}

RTCORBA::ThreadpoolId
TAO_RT_ORB::create_threadpool (CORBA::ULong stacksize,
                               CORBA::ULong static_threads,
                               CORBA::ULong dynamic_threads,
                               RTCORBA::Priority default_priority,
                               CORBA::Boolean allow_request_buffering,
                               CORBA::ULong max_buffered_requests,
                               CORBA::ULong max_request_buffer_size
                               )
{
  return this->tp_manager_->create_threadpool (stacksize,
                                               static_threads,
                                               dynamic_threads,
                                               default_priority,
                                               allow_request_buffering,
                                               max_buffered_requests,
                                               max_request_buffer_size,
                                               this->lifespan_,
                                               this->dynamic_thread_time_);
}

RTCORBA::ThreadpoolId
TAO_RT_ORB::create_threadpool_with_lanes (CORBA::ULong stacksize,
                                          const RTCORBA::ThreadpoolLanes &lanes,
                                          CORBA::Boolean allow_borrowing,
                                          CORBA::Boolean allow_request_buffering,
                                          CORBA::ULong max_buffered_requests,
                                          CORBA::ULong max_request_buffer_size)
{
  return this->tp_manager_->create_threadpool_with_lanes (stacksize,
                                                          lanes,
                                                          allow_borrowing,
                                                          allow_request_buffering,
                                                          max_buffered_requests,
                                                          max_request_buffer_size,
                                                          this->lifespan_,
                                                          this->dynamic_thread_time_);
}

void
TAO_RT_ORB::destroy_threadpool (RTCORBA::ThreadpoolId threadpool)
{
  this->tp_manager_->destroy_threadpool (threadpool);
}

RTCORBA::PriorityModelPolicy_ptr
TAO_RT_ORB::create_priority_model_policy (RTCORBA::PriorityModel priority_model,
                                          RTCORBA::Priority server_priority)
{
  TAO_PriorityModelPolicy *tmp = 0;
  ACE_NEW_THROW_EX (tmp,
                    TAO_PriorityModelPolicy (priority_model, server_priority),
                    CORBA::NO_MEMORY (TAO::VMCID,
                                      CORBA::COMPLETED_NO));

  return tmp;
}

RTCORBA::ThreadpoolPolicy_ptr
TAO_RT_ORB::create_threadpool_policy (RTCORBA::ThreadpoolId threadpool)
{
  TAO_ThreadpoolPolicy *tmp = 0;
  ACE_NEW_THROW_EX (tmp,
                    TAO_ThreadpoolPolicy (threadpool),
                    CORBA::NO_MEMORY (TAO::VMCID,
                                      CORBA::COMPLETED_NO));

  return tmp;
}

RTCORBA::PriorityBandedConnectionPolicy_ptr
TAO_RT_ORB::create_priority_banded_connection_policy (const
                                                      RTCORBA::PriorityBands & priority_bands)
{
  TAO_PriorityBandedConnectionPolicy *tmp = 0;
  ACE_NEW_THROW_EX (tmp,
                    TAO_PriorityBandedConnectionPolicy (priority_bands),
                    CORBA::NO_MEMORY (TAO::VMCID,
                                      CORBA::COMPLETED_NO));

  return tmp;
}

RTCORBA::PrivateConnectionPolicy_ptr
TAO_RT_ORB::create_private_connection_policy (void)
{
  TAO_PrivateConnectionPolicy *tmp = 0;
  ACE_NEW_THROW_EX (tmp,
                    TAO_PrivateConnectionPolicy (),
                    CORBA::NO_MEMORY (TAO::VMCID,
                                      CORBA::COMPLETED_NO));

  return tmp;
}

RTCORBA::ServerProtocolPolicy_ptr
TAO_RT_ORB::create_server_protocol_policy (const RTCORBA::ProtocolList & protocols)
{
  TAO_ServerProtocolPolicy *tmp = 0;
  ACE_NEW_THROW_EX (tmp,
                    TAO_ServerProtocolPolicy (protocols),
                    CORBA::NO_MEMORY (TAO::VMCID,
                                      CORBA::COMPLETED_NO));

  return tmp;
}

RTCORBA::ClientProtocolPolicy_ptr
TAO_RT_ORB::create_client_protocol_policy (const RTCORBA::ProtocolList & protocols)
{
  TAO_ClientProtocolPolicy *tmp = 0;
  ACE_NEW_THROW_EX (tmp,
                    TAO_ClientProtocolPolicy (protocols),
                    CORBA::NO_MEMORY (TAO::VMCID,
                                      CORBA::COMPLETED_NO));

  return tmp;
}

TAO_Thread_Pool_Manager &
TAO_RT_ORB::tp_manager (void) const
{
  return *this->tp_manager_;
}

TAO_ORB_Core *
TAO_RT_ORB::orb_core (void) const
{
  return this->orb_core_;
}

/* static */
int
TAO_RT_ORB::modify_thread_scheduling_policy (CORBA::ORB_ptr orb)
{
  //
  // This method changes the scheduling policy of the calling thread
  // to match the scheduling policy specified in the svc.conf file.
  // The priority of the calling thread will be set to the minimum
  // priority supported by that scheduling policy.
  //
  // This method make sense on those platform (e.g., Linux) where
  // PTHREAD_SCOPE_SYSTEM is the only scheduling scope supported.  On
  // other platforms, this method is a no-op since the only way to get
  // the real-time threading behavior is to setup the
  // PTHREAD_SCOPE_SYSTEM scheduling scope when a thread is being
  // created.  On such platforms, one can set the correct scheduling
  // scope and policy when creating the thread, thus not needing to
  // use this method.
  //

#if defined (linux)

  int const sched_policy = orb->orb_core ()->orb_params ()->ace_sched_policy ();

  int const minimum_priority = ACE_Sched_Params::priority_min (sched_policy);

  ACE_hthread_t thread_id;
  ACE_Thread::self (thread_id);

  return ACE_Thread::setprio (thread_id, minimum_priority, sched_policy);

#else /* linux */

  ACE_UNUSED_ARG (orb);
  ACE_NOTSUP_RETURN (-1);

#endif /* linux */

}

TAO_END_VERSIONED_NAMESPACE_DECL

///////////////////////////////////////////////////////////////////////////////

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */
