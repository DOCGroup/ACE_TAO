// -*- C++ -*-
//
// $Id$

// Exception macros
#include "poa_macros.h"
#include "tao/Environment.h"

ACE_INLINE CORBA::Boolean
TAO_POA::cleanup_in_progress (void)
{
  return this->cleanup_in_progress_;
}

ACE_INLINE ACE_Lock &
TAO_POA::lock (void)
{
  return this->lock_;
}

ACE_INLINE CORBA::ULong
TAO_Creation_Time::creation_time_length (void)
{
  return 2 * sizeof (CORBA::ULong);
}

ACE_INLINE
TAO_Creation_Time::TAO_Creation_Time (const ACE_Time_Value &creation_time)
{
  this->time_stamp_[TAO_Creation_Time::SEC_FIELD]  = (CORBA::ULong) creation_time.sec ();
  this->time_stamp_[TAO_Creation_Time::USEC_FIELD] = (CORBA::ULong) creation_time.usec ();
}

ACE_INLINE
TAO_Creation_Time::TAO_Creation_Time (void)
{
  this->time_stamp_[TAO_Creation_Time::SEC_FIELD]  = 0;
  this->time_stamp_[TAO_Creation_Time::USEC_FIELD] = 0;
}

ACE_INLINE void
TAO_Creation_Time::creation_time (const void *creation_time)
{
  ACE_OS::memcpy (&this->time_stamp_,
                  creation_time,
                  TAO_Creation_Time::creation_time_length ());
}

ACE_INLINE const void *
TAO_Creation_Time::creation_time (void) const
{
  return &this->time_stamp_;
}

ACE_INLINE int
TAO_Creation_Time::operator== (const TAO_Creation_Time &rhs) const
{
#if (POA_NO_TIMESTAMP == 1)
  ACE_UNUSED_ARG (rhs);
  return 1;
#else
  return ACE_OS::memcmp (&this->time_stamp_,
                         &rhs.time_stamp_,
                         TAO_Creation_Time::creation_time_length ()) == 0;
#endif /* POA_NO_TIMESTAMP */
}

ACE_INLINE int
TAO_Creation_Time::operator!= (const TAO_Creation_Time &rhs) const
{
#if (POA_NO_TIMESTAMP == 1)
  ACE_UNUSED_ARG (rhs);
  return 0;
#else
  return ACE_OS::memcmp (&this->time_stamp_,
                         &rhs.time_stamp_,
                         TAO_Creation_Time::creation_time_length ()) != 0;
#endif /* POA_NO_TIMESTAMP */
}

ACE_INLINE int
TAO_Temporary_Creation_Time::operator== (const TAO_Creation_Time &rhs) const
{
#if (POA_NO_TIMESTAMP == 1)
  ACE_UNUSED_ARG (rhs);
  return 1;
#else
  return ACE_OS::memcmp (this->time_stamp_,
                         rhs.creation_time (),
                         TAO_Creation_Time::creation_time_length ()) == 0;
#endif /* POA_NO_TIMESTAMP */
}

ACE_INLINE int
TAO_Temporary_Creation_Time::operator!= (const TAO_Creation_Time &rhs) const
{
#if (POA_NO_TIMESTAMP == 1)
  ACE_UNUSED_ARG (rhs);
  return 0;
#else
  return ACE_OS::memcmp (this->time_stamp_,
                         rhs.creation_time (),
                         TAO_Creation_Time::creation_time_length ()) != 0;
#endif /* POA_NO_TIMESTAMP */
}

ACE_INLINE int
TAO_Creation_Time::operator== (const TAO_Temporary_Creation_Time &rhs) const
{
  return rhs == *this;
}

ACE_INLINE int
TAO_Creation_Time::operator!= (const TAO_Temporary_Creation_Time &rhs) const
{
  return rhs != *this;
}

ACE_INLINE
TAO_Temporary_Creation_Time::TAO_Temporary_Creation_Time (void)
  : time_stamp_ (0)
{
}

ACE_INLINE void
TAO_Temporary_Creation_Time::creation_time (const void *creation_time)
{
  this->time_stamp_ = (void *) creation_time;
}

ACE_INLINE PortableServer::POA_ptr
TAO_POA::create_POA (const char *adapter_name,
                     PortableServer::POAManager_ptr poa_manager,
                     const CORBA::PolicyList &policies
                     ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::AdapterAlreadyExists,
                   PortableServer::POA::InvalidPolicy))
{
  // Lock access for the duration of this transaction.
  TAO_POA_GUARD_RETURN (0);

  return this->create_POA_i (adapter_name,
                             poa_manager,
                             policies
                             ACE_ENV_ARG_PARAMETER);
}

ACE_INLINE void
TAO_POA::destroy (CORBA::Boolean etherealize_objects,
                  CORBA::Boolean wait_for_completion
                  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Lock access for the duration of this transaction.
  TAO_POA_Guard poa_guard (*this ACE_ENV_ARG_PARAMETER, 0);
  ACE_CHECK;
  ACE_UNUSED_ARG (poa_guard);

  this->destroy_i (etherealize_objects,
                   wait_for_completion
                   ACE_ENV_ARG_PARAMETER);
}

ACE_INLINE TAO_POA_Policy_Set &
TAO_POA::policies (void)
{
  return this->policies_;
}

ACE_INLINE TAO_Active_Object_Map &
TAO_POA::active_object_map (void) const
{
  return *this->active_object_map_;
}

#if (TAO_HAS_MINIMUM_POA == 0)

ACE_INLINE PortableServer::ServantManager_ptr
TAO_POA::get_servant_manager (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::WrongPolicy))
{
  // Lock access for the duration of this transaction.
  TAO_POA_GUARD_RETURN (PortableServer::ServantManager::_nil ());

  return this->get_servant_manager_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

ACE_INLINE void
TAO_POA::set_servant_manager (PortableServer::ServantManager_ptr imgr
                              ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::WrongPolicy))
{
  // Lock access for the duration of this transaction.
  TAO_POA_GUARD;

  this->set_servant_manager_i (imgr
                               ACE_ENV_ARG_PARAMETER);
}

ACE_INLINE PortableServer::Servant
TAO_POA::get_servant (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::NoServant,
                   PortableServer::POA::WrongPolicy))
{
  // Lock access for the duration of this transaction.
  TAO_POA_GUARD_RETURN (0);

  return this->get_servant_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

ACE_INLINE void
TAO_POA::set_servant (PortableServer::Servant servant
                      ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::WrongPolicy))
{
  // Lock access for the duration of this transaction.
  TAO_POA_GUARD;

  this->set_servant_i (servant
                       ACE_ENV_ARG_PARAMETER);
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

ACE_INLINE PortableServer::ObjectId *
TAO_POA::activate_object (PortableServer::Servant servant
                          ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::ServantAlreadyActive,
                   PortableServer::POA::WrongPolicy))
{
  // Lock access for the duration of this transaction.
  TAO_POA_GUARD_RETURN (0);

  return this->activate_object_i (servant,
                                  this->cached_policies_.server_priority ()
                                  ACE_ENV_ARG_PARAMETER);
}

ACE_INLINE void
TAO_POA::activate_object_with_id (const PortableServer::ObjectId &id,
                                  PortableServer::Servant servant
                                  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::ServantAlreadyActive,
                   PortableServer::POA::ObjectAlreadyActive,
                   PortableServer::POA::WrongPolicy))
{
  // Lock access for the duration of this transaction.
  TAO_POA_GUARD;

  this->activate_object_with_id_i (id,
                                   servant,
                                   this->cached_policies_.server_priority ()
                                   ACE_ENV_ARG_PARAMETER);
}

ACE_INLINE void
TAO_POA::deactivate_object (const PortableServer::ObjectId &oid
                            ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::ObjectNotActive,
                   PortableServer::POA::WrongPolicy))
{
  // Lock access for the duration of this transaction.
  TAO_POA_GUARD;

  this->deactivate_object_i (oid
                             ACE_ENV_ARG_PARAMETER);
}

ACE_INLINE CORBA::Object_ptr
TAO_POA::create_reference (const char *intf
                           ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::WrongPolicy))
{
  // Lock access for the duration of this transaction.
  TAO_POA_GUARD_RETURN (CORBA::Object::_nil ());

  return this->create_reference_i (intf,
                                   this->cached_policies_.server_priority ()
                                   ACE_ENV_ARG_PARAMETER);
}

ACE_INLINE CORBA::Object_ptr
TAO_POA::create_reference_with_id (const PortableServer::ObjectId &id,
                                   const char *intf
                                   ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::WrongPolicy))
{
  // Lock access for the duration of this transaction.
  TAO_POA_GUARD_RETURN (CORBA::Object::_nil ());

  return this->create_reference_with_id_i (id,
                                           intf,
                                           this->cached_policies_.server_priority ()
                                           ACE_ENV_ARG_PARAMETER);
}

ACE_INLINE PortableServer::ObjectId *
TAO_POA::servant_to_id (PortableServer::Servant servant
                        ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::ServantNotActive,
                   PortableServer::POA::WrongPolicy))
{
  // If we had upgradeable locks, this would initially be a read lock
  //
  // Lock access for the duration of this transaction.
  TAO_POA_GUARD_RETURN (0);

  return this->servant_to_id_i (servant
                                ACE_ENV_ARG_PARAMETER);
}

ACE_INLINE PortableServer::ObjectId *
TAO_POA::servant_to_system_id (PortableServer::Servant servant,
                               CORBA::Short &priority
                               ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::ServantNotActive,
                   PortableServer::POA::WrongPolicy))
{
  // Lock access for the duration of this transaction.
  TAO_POA_GUARD_RETURN (0);

  return this->servant_to_system_id_i (servant,
                                       priority
                                       ACE_ENV_ARG_PARAMETER);
}

ACE_INLINE PortableServer::Servant
TAO_POA::reference_to_servant (CORBA::Object_ptr reference
                               ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::ObjectNotActive,
                   PortableServer::POA::WrongAdapter,
                   PortableServer::POA::WrongPolicy))
{
  // Lock access for the duration of this transaction.
  TAO_POA_GUARD_RETURN (0);

  return this->reference_to_servant_i (reference
                                       ACE_ENV_ARG_PARAMETER);
}

ACE_INLINE PortableServer::Servant
TAO_POA::id_to_servant (const PortableServer::ObjectId &oid
                        ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::ObjectNotActive,
                   PortableServer::POA::WrongPolicy))
{
  // Lock access for the duration of this transaction.
  TAO_POA_GUARD_RETURN (0);

  return this->id_to_servant_i (oid
                                ACE_ENV_ARG_PARAMETER);
}

ACE_INLINE CORBA::Object_ptr
TAO_POA::id_to_reference (const PortableServer::ObjectId &oid
                          ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::ObjectNotActive,
                   PortableServer::POA::WrongPolicy))
{
  // Lock access for the duration of this transaction.
  TAO_POA_GUARD_RETURN (0);

  return this->id_to_reference_i (oid ACE_ENV_ARG_PARAMETER);
}

ACE_INLINE PortableServer::POA_ptr
TAO_POA::the_parent (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (this->parent_ != 0)
    return PortableServer::POA::_duplicate (this->parent_);
  else
    return PortableServer::POA::_nil ();
}

ACE_INLINE PortableServer::POAList *
TAO_POA::the_children (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Lock access for the duration of this transaction.
  TAO_POA_GUARD_RETURN (0);

  return this->the_children_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

ACE_INLINE PortableServer::POAManager_ptr
TAO_POA::the_POAManager (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return PortableServer::POAManager::_duplicate (&this->poa_manager_);
}

ACE_INLINE PortableInterceptor::AdapterManagerId
TAO_POA::get_manager_id (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->poa_manager_.get_manager_id (ACE_ENV_SINGLE_ARG_PARAMETER);
}

ACE_INLINE PortableInterceptor::AdapterName *
TAO_POA::adapter_name (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->adapter_name_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

ACE_INLINE PortableInterceptor::ObjectReferenceTemplate *
TAO_POA::get_adapter_template ()
{
  return this->ort_template_;
}

ACE_INLINE PortableInterceptor::ObjectReferenceFactory *
TAO_POA::get_obj_ref_factory ()
{
  return this->obj_ref_factory_;
}

#if (TAO_HAS_MINIMUM_POA == 0)

ACE_INLINE PortableServer::AdapterActivator_ptr
TAO_POA::the_activator (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Lock access for the duration of this transaction.
  TAO_POA_GUARD_RETURN (PortableServer::AdapterActivator::_nil ());

  return PortableServer::AdapterActivator::_duplicate (this->adapter_activator_.in ());
}

ACE_INLINE void
TAO_POA::the_activator (PortableServer::AdapterActivator_ptr adapter_activator
                        ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Lock access for the duration of this transaction.
  TAO_POA_GUARD;

  this->adapter_activator_ = PortableServer::AdapterActivator::_duplicate (adapter_activator);
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

ACE_INLINE const TAO_Creation_Time &
TAO_POA::creation_time (void)
{
  return this->creation_time_;
}

ACE_INLINE CORBA::Boolean
TAO_POA::system_id (void)
{
  return (this->cached_policies_.id_assignment () == PortableServer::SYSTEM_ID);
}


ACE_INLINE CORBA::Boolean
TAO_POA::root (void)
{
  return this->parent_ == 0;
}


ACE_INLINE CORBA::Boolean
TAO_POA::persistent (void)
{
  return (this->cached_policies_.lifespan () == PortableServer::PERSISTENT);
}

ACE_INLINE const ACE_CString &
TAO_POA::name (void) const
{
  return this->name_;
}

ACE_INLINE char *
TAO_POA::the_name (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup (this->name_.c_str ());
}

ACE_INLINE const TAO_Object_Adapter::poa_name &
TAO_POA::folded_name (void) const
{
  return this->folded_name_;
}

ACE_INLINE const TAO_Object_Adapter::poa_name &
TAO_POA::system_name (void) const
{
  return this->system_name_.in ();
}

ACE_INLINE char
TAO_POA::name_separator (void)
{
  return '\0';
}

ACE_INLINE CORBA::ULong
TAO_POA::name_separator_length (void)
{
  return sizeof (char);
}

ACE_INLINE char
TAO_POA::persistent_key_char (void)
{
  return 'P';
}

ACE_INLINE char
TAO_POA::transient_key_char (void)
{
  return 'T';
}

ACE_INLINE char
TAO_POA::persistent_key_type (void)
{
  if (this->persistent ())
    return TAO_POA::persistent_key_char ();
  else
    return TAO_POA::transient_key_char ();
}

ACE_INLINE CORBA::ULong
TAO_POA::persistent_key_type_length (void)
{
  return sizeof (char);
}

ACE_INLINE char
TAO_POA::system_id_key_char (void)
{
  return 'S';
}

ACE_INLINE char
TAO_POA::user_id_key_char (void)
{
  return 'U';
}

ACE_INLINE char
TAO_POA::system_id_key_type (void)
{
  if (this->system_id ())
    return TAO_POA::system_id_key_char ();
  else
    return TAO_POA::user_id_key_char ();
}

ACE_INLINE CORBA::ULong
TAO_POA::system_id_key_type_length (void)
{
  return sizeof (char);
}

ACE_INLINE char
TAO_POA::root_key_char (void)
{
  return 'R';
}

ACE_INLINE char
TAO_POA::non_root_key_char (void)
{
  return 'N';
}

ACE_INLINE char
TAO_POA::root_key_type (void)
{
  if (this->root ())
    return TAO_POA::root_key_char ();
  else
    return TAO_POA::non_root_key_char ();
}

ACE_INLINE CORBA::ULong
TAO_POA::root_key_type_length (void)
{
  return sizeof (char);
}

ACE_INLINE CORBA::ULong
TAO_POA::outstanding_requests (void) const
{
  return this->outstanding_requests_;
}

ACE_INLINE void
TAO_POA::outstanding_requests (CORBA::ULong new_outstanding_requests)
{
  this->outstanding_requests_ = new_outstanding_requests;
}

ACE_INLINE CORBA::ULong
TAO_POA::increment_outstanding_requests (void)
{
  return ++this->outstanding_requests_;
}

ACE_INLINE CORBA::ULong
TAO_POA::decrement_outstanding_requests (void)
{
  return --this->outstanding_requests_;
}

ACE_INLINE CORBA::Boolean
TAO_POA::waiting_destruction (void) const
{
  return this->waiting_destruction_;
}

ACE_INLINE TAO_SYNCH_RECURSIVE_MUTEX &
TAO_POA::single_threaded_lock (void) const
{
  return *this->single_threaded_lock_;
}

ACE_INLINE TAO_ORB_Core &
TAO_POA::orb_core (void) const
{
  return this->orb_core_;
}

ACE_INLINE PortableServer::ThreadPolicyValue
TAO_POA::thread_policy (void) const
{
  return this->cached_policies_.thread ();
}

ACE_INLINE PortableInterceptor::AdapterState
TAO_POA::get_adapter_state (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  return this->adapter_state_;
}

ACE_INLINE TAO_POA_Cached_Policies &
TAO_POA::cached_policies (void)
{
  return this->cached_policies_;
}

ACE_INLINE void *
TAO_POA::thread_pool (void) const
{
  return 0;
}
