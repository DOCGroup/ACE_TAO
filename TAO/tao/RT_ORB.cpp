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
  TAO_RT_Mutex *mutex;

  ACE_NEW_THROW_EX (mutex,
                    TAO_RT_Mutex (),
                    CORBA::NO_MEMORY (TAO_DEFAULT_MINOR_CODE,
                                      CORBA::COMPLETED_NO));

  return mutex;
}

void
TAO_RT_ORB::destroy_mutex (RTCORBA::Mutex_ptr,
                           CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Don't need to do anything.  The destructor on the mutex
  // will automatically reclaim resources.
}


RTCORBA::Mutex_ptr
TAO_RT_ORB::create_named_mutex (const char *name,
                                CORBA::Boolean_out created_flag,
                                CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_RT_Mutex *mutex;

  mutex = mutex_mgr_.find_mutex (name);
  if (mutex != 0)
    {
      created_flag = 0;

      // Increment the reference count to pass ownership
      // to the caller.
      mutex->_add_ref ();
    }
  else
    {
      ACE_NEW_THROW_EX (mutex,
                        TAO_Named_RT_Mutex (name, mutex_mgr_),
                        CORBA::NO_MEMORY (TAO_DEFAULT_MINOR_CODE,
                                          CORBA::COMPLETED_NO));

      // registration should always succeed
      if ( mutex_mgr_.register_mutex (name, mutex) != 0)
        {
          delete mutex;
          ACE_THROW_RETURN (CORBA::INTERNAL (), 0);
        }
      created_flag = 1;
    }

  return mutex;
}

RTCORBA::Mutex_ptr
TAO_RT_ORB::open_named_mutex (const char *name,
                              CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   RTCORBA::RTORB::MutexNotFound))
{
  TAO_RT_Mutex *mutex;

  mutex = mutex_mgr_.find_mutex (name);
  if (mutex == 0)
    {
      ACE_THROW_RETURN (RTCORBA::RTORB::MutexNotFound (), 0);
    }
  else
    {
      // Increment the reference count to pass ownership
      // to the caller.
      mutex->_add_ref ();
    }

  return mutex;
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


TAO_Named_RT_Mutex_Manager::TAO_Named_RT_Mutex_Manager (void)
{
}


TAO_Named_RT_Mutex_Manager::~TAO_Named_RT_Mutex_Manager (void)
{
}


TAO_RT_Mutex *
TAO_Named_RT_Mutex_Manager::find_mutex (const char *name)
{
  TAO_RT_Mutex *mutex;

  if (mutex_map_.find (name, mutex) == 0)
    return mutex;
  else
    return 0;
}

int
TAO_Named_RT_Mutex_Manager::register_mutex (const char *name,
                                            TAO_RT_Mutex *mutex)
{
  // Only return success if the bind is successful
  // and there is not already something else bound to
  // the specified name.
  if (mutex_map_.bind (name, mutex) == 0)
    return 0;
  else
    return -1;
}

void
TAO_Named_RT_Mutex_Manager::unregister_mutex (const char *name)
{
  mutex_map_.unbind (name);
}

////////////////////////////////////////////////////////////////////////////////

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Hash_Map_Manager_Ex<char const *, TAO_RT_Mutex *, ACE_Hash<char const *>, ACE_Equal_To<char const *>, TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator_Base_Ex<char const *, TAO_RT_Mutex *, ACE_Hash<char const *>, ACE_Equal_To<char const *>, TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator_Ex<char const *, TAO_RT_Mutex *, ACE_Hash<char const *>, ACE_Equal_To<char const *>, TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<char const *, TAO_RT_Mutex *, ACE_Hash<char const *>, ACE_Equal_To<char const *>, TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Entry<char const *, TAO_RT_Mutex *>;

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Hash_Map_Manager_Ex<char const *, TAO_RT_Mutex *, ACE_Hash<char const *>, ACE_Equal_To<char const *>, TAO_SYNCH_MUTEX>;
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<char const *, TAO_RT_Mutex *, ACE_Hash<char const *>, ACE_Equal_To<char const *>, TAO_SYNCH_MUTEX>;
#pragma instantiate ACE_Hash_Map_Iterator_Ex<char const *, TAO_RT_Mutex *, ACE_Hash<char const *>, ACE_Equal_To<char const *>, TAO_SYNCH_MUTEX>;
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<char const *, TAO_RT_Mutex *, ACE_Hash<char const *>, ACE_Equal_To<char const *>, TAO_SYNCH_MUTEX>;
#pragma instantiate ACE_Hash_Map_Entry<char const *, TAO_RT_Mutex *>;

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#endif /* TAO_HAS_RT_CORBA == 1 */
