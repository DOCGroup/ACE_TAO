// $Id$

#include "tao/RT_ORB.h"
#include "tao/RT_Policy_i.h"
#include "tao/RT_Mutex.h"

#if ! defined (__ACE_INLINE__)
#include "tao/RT_ORB.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(TAO, RT_ORB, "$Id$")

#if (TAO_HAS_RT_CORBA == 1)

TAO_RT_ORB::TAO_RT_ORB (void)
{
}

TAO_RT_ORB::~TAO_RT_ORB (void)
{
}

RTCORBA::Mutex_ptr
TAO_RT_ORB::create_mutex (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->mutex_mgr_.create_mutex (ACE_TRY_ENV);
}

void
TAO_RT_ORB::destroy_mutex (RTCORBA::Mutex_ptr mutex,
                           CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->mutex_mgr_.destroy_mutex (mutex,
                                  ACE_TRY_ENV);
}


RTCORBA::Mutex_ptr
TAO_RT_ORB::create_named_mutex (const char *name,
                                CORBA::Boolean_out created_flag,
                                CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->mutex_mgr_.create_named_mutex (name,
                                              created_flag,
                                              ACE_TRY_ENV);
}

RTCORBA::Mutex_ptr
TAO_RT_ORB::open_named_mutex (const char *name,
                              CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   RTCORBA::RTORB::MutexNotFound))
{
  return this->mutex_mgr_.open_named_mutex (name,
                                            ACE_TRY_ENV);
}

////////////////////////////////////////////////////////////////////////////////

TAO_Named_RT_Mutex_Manager::TAO_Named_RT_Mutex_Manager (void)
{
}

TAO_Named_RT_Mutex_Manager::~TAO_Named_RT_Mutex_Manager (void)
{
}

RTCORBA::Mutex_ptr
TAO_Named_RT_Mutex_Manager::create_mutex (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_RT_Mutex *mutex = 0;
  ACE_NEW_THROW_EX (mutex,
                    TAO_RT_Mutex (),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (RTCORBA::Mutex::_nil ());

  return mutex;
}

// If Named RT_Mutexes aren't enabled, this function is a nop
// as also indicated by the comment below.
#if (TAO_HAS_NAMED_RT_MUTEXES == 1)
void
TAO_Named_RT_Mutex_Manager::destroy_mutex (RTCORBA::Mutex_ptr mutex,
                                           CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_RT_Mutex *tao_mutex =
    ACE_dynamic_cast (TAO_RT_Mutex *,
                      mutex);

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
      ACE_CHECK;

      int result =
        this->map_.unbind (name);

      if (result != 0)
        ACE_THROW (CORBA::INTERNAL ());
    }
}
#else /* TAO_HAS_NAMED_RT_MUTEXES == 1 */
void
TAO_Named_RT_Mutex_Manager::destroy_mutex (RTCORBA::Mutex_ptr,
                                           CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}
#endif /* TAO_HAS_NAMED_RT_MUTEXES == 1 */

RTCORBA::Mutex_ptr
TAO_Named_RT_Mutex_Manager::create_named_mutex (const char *name,
                                                CORBA::Boolean_out created_flag,
                                                CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
#if (TAO_HAS_NAMED_RT_MUTEXES == 1)
  // The following should be atomic.
  ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX,
                      monitor,
                      this->lock_,
                      CORBA::INTERNAL ());
  ACE_CHECK_RETURN (RTCORBA::Mutex::_nil ());

  // Optimistic that we'll find it.
  created_flag = 0;

  // If we find the mutex, simply return it.
  RTCORBA::Mutex_var mutex;
  if (this->map_.find (name,
                       mutex) != 0)
    {
      // Oops, we didn't find it.
      created_flag = 1;

      RTCORBA::Mutex_ptr tmp_mutex;

      // Create a new one.
      ACE_NEW_THROW_EX (tmp_mutex,
                        TAO_Named_RT_Mutex (name),
                        CORBA::NO_MEMORY (
                          CORBA::SystemException::_tao_minor_code (
                            TAO_DEFAULT_MINOR_CODE,
                            ENOMEM),
                          CORBA::COMPLETED_NO));
      ACE_CHECK_RETURN (RTCORBA::Mutex::_nil ());

      mutex = tmp_mutex;

      // Add it to the map.
      int result =
        this->map_.bind (name,
                         mutex);

      if (result != 0)
        ACE_THROW_RETURN (CORBA::INTERNAL (),
                          RTCORBA::Mutex::_nil ());
    }

  // Return the one we found or created.
  return mutex._retn ();
#else /* TAO_HAS_NAMED_RT_MUTEXES */
  ACE_UNUSED_ARG (name);
  ACE_UNUSED_ARG (created_flag);
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (),
                    RTCORBA::Mutex::_nil ());
#endif /* TAO_HAS_NAMED_RT_MUTEXES */
}

RTCORBA::Mutex_ptr
TAO_Named_RT_Mutex_Manager::open_named_mutex (const char *name,
                                              CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   RTCORBA::RTORB::MutexNotFound))
{
#if (TAO_HAS_NAMED_RT_MUTEXES == 1)
  // The following should be atomic.
  ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX,
                      monitor,
                      this->lock_,
                      CORBA::INTERNAL ());
  ACE_CHECK_RETURN (RTCORBA::Mutex::_nil ());

  // If we find the mutex, simply return it.
  RTCORBA::Mutex_var mutex;
  if (this->map_.find (name,
                       mutex) != 0)
    ACE_THROW_RETURN (RTCORBA::RTORB::MutexNotFound (),
                      RTCORBA::Mutex::_nil ());

  // Return the one we found.
  return mutex._retn ();
#else /* TAO_HAS_NAMED_RT_MUTEXES */
  ACE_UNUSED_ARG (name);
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (),
                    RTCORBA::Mutex::_nil ());
#endif /* TAO_HAS_NAMED_RT_MUTEXES */
}

///////////////////////////////////////////////////////////////////////////////

RTCORBA::TCPProtocolProperties_ptr
TAO_RT_ORB::create_tcp_protocol_properties (CORBA::Long send_buffer_size,
                                            CORBA::Long recv_buffer_size,
                                            CORBA::Boolean keep_alive,
                                            CORBA::Boolean dont_route,
                                            CORBA::Boolean no_delay,
                                            CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException ))
{
  TAO_TCP_Properties *tmp;
  ACE_NEW_THROW_EX (tmp,
                    TAO_TCP_Properties (send_buffer_size,
                                        recv_buffer_size,
                                        keep_alive,
                                        dont_route,
                                        no_delay),
                    CORBA::NO_MEMORY (TAO_DEFAULT_MINOR_CODE,
                                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (RTCORBA::TCPProtocolProperties::_nil ());

  return tmp;
}

RTCORBA::ThreadpoolId
TAO_RT_ORB::create_threadpool (CORBA::ULong /*stacksize*/,
                               CORBA::ULong /*static_threads*/,
                               CORBA::ULong /*dynamic_threads*/,
                               RTCORBA::Priority /*default_priority*/,
                               CORBA::Boolean /*allow_request_buffering*/,
                               CORBA::ULong /*max_buffered_requests*/,
                               CORBA::ULong /*max_request_buffer_size*/,
                               CORBA::Environment &/*ACE_TRY_ENV*/)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  //Add your implementation here
  return 0;
}

RTCORBA::ThreadpoolId
TAO_RT_ORB::create_threadpool_with_lanes (CORBA::ULong /*stacksize*/,
                                          const RTCORBA::ThreadpoolLanes & /*lanes*/,
                                          CORBA::Boolean /*allow_borrowing*/,
                                          CORBA::Boolean /*allow_request_buffering*/,
                                          CORBA::ULong /*max_buffered_requests*/,
                                          CORBA::ULong /*max_request_buffer_size*/,
                                          CORBA::Environment &/*ACE_TRY_ENV*/)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  //Add your implementation here
  return 0;
}

void
TAO_RT_ORB::destroy_threadpool (RTCORBA::ThreadpoolId /*threadpool*/,
                                CORBA::Environment &/*ACE_TRY_ENV*/)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   RTCORBA::RTORB::InvalidThreadpool))
{
  //Add your implementation here
}

RTCORBA::PriorityModelPolicy_ptr
TAO_RT_ORB::create_priority_model_policy (RTCORBA::PriorityModel priority_model,
                                          RTCORBA::Priority server_priority,
                                          CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_PriorityModelPolicy *tmp;
  ACE_NEW_THROW_EX (tmp,
                    TAO_PriorityModelPolicy (priority_model, server_priority),
                    CORBA::NO_MEMORY (TAO_DEFAULT_MINOR_CODE,
                                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (RTCORBA::PriorityModelPolicy::_nil ());

  return tmp;
}

RTCORBA::ThreadpoolPolicy_ptr
TAO_RT_ORB::create_threadpool_policy (RTCORBA::ThreadpoolId threadpool,
                                      CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_ThreadpoolPolicy *tmp;
  ACE_NEW_THROW_EX (tmp,
                    TAO_ThreadpoolPolicy (threadpool),
                    CORBA::NO_MEMORY (TAO_DEFAULT_MINOR_CODE,
                                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (RTCORBA::ThreadpoolPolicy::_nil ());

  return tmp;
}

RTCORBA::PriorityBandedConnectionPolicy_ptr
TAO_RT_ORB::create_priority_banded_connection_policy (const
                                                      RTCORBA::PriorityBands & priority_bands,
                                                      CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_PriorityBandedConnectionPolicy *tmp;
  ACE_NEW_THROW_EX (tmp,
                    TAO_PriorityBandedConnectionPolicy (priority_bands),
                    CORBA::NO_MEMORY (TAO_DEFAULT_MINOR_CODE,
                                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (RTCORBA::PriorityBandedConnectionPolicy::_nil ());

  return tmp;
}

RTCORBA::PrivateConnectionPolicy_ptr
TAO_RT_ORB::create_private_connection_policy (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_PrivateConnectionPolicy *tmp;
  ACE_NEW_THROW_EX (tmp,
                    TAO_PrivateConnectionPolicy (),
                    CORBA::NO_MEMORY (TAO_DEFAULT_MINOR_CODE,
                                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (RTCORBA::PrivateConnectionPolicy::_nil ());

  return tmp;
}

RTCORBA::ServerProtocolPolicy_ptr
TAO_RT_ORB::create_server_protocol_policy (const RTCORBA::ProtocolList & protocols,
                                           CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_ServerProtocolPolicy *tmp;
  ACE_NEW_THROW_EX (tmp,
                    TAO_ServerProtocolPolicy (protocols),
                    CORBA::NO_MEMORY (TAO_DEFAULT_MINOR_CODE,
                                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (RTCORBA::ServerProtocolPolicy::_nil ());

  return tmp;
}

RTCORBA::ClientProtocolPolicy_ptr
TAO_RT_ORB::create_client_protocol_policy (const RTCORBA::ProtocolList & protocols,
                                           CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_ClientProtocolPolicy *tmp;
  ACE_NEW_THROW_EX (tmp,
                    TAO_ClientProtocolPolicy (protocols),
                    CORBA::NO_MEMORY (TAO_DEFAULT_MINOR_CODE,
                                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (RTCORBA::ClientProtocolPolicy::_nil ());

  return tmp;
}

////////////////////////////////////////////////////////////////////////////////

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

#if (TAO_HAS_NAMED_RT_MUTEXES == 1)
template class ACE_Hash_Map_Manager_Ex<ACE_CString, RTCORBA::Mutex_var, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, RTCORBA::Mutex_var, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<ACE_CString, RTCORBA::Mutex_var, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, RTCORBA::Mutex_var, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Entry<ACE_CString, RTCORBA::Mutex_var>;
#endif /* TAO_HAS_NAMED_RT_MUTEXES == 1 */

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#if (TAO_HAS_NAMED_RT_MUTEXES == 1)
#pragma instantiate ACE_Hash_Map_Manager_Ex<ACE_CString, RTCORBA::Mutex_var, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, RTCORBA::Mutex_var, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<ACE_CString, RTCORBA::Mutex_var, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, RTCORBA::Mutex_var, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Entry<ACE_CString, RTCORBA::Mutex_var>
#endif /* TAO_HAS_NAMED_RT_MUTEXES == 1 */

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#endif /* TAO_HAS_RT_CORBA == 1 */
