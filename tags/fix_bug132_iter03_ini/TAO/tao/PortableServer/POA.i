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

ACE_INLINE
TAO_POA_Guard::TAO_POA_Guard (TAO_POA &poa,
                              CORBA::Environment &ACE_TRY_ENV,
                              int check_for_destruction)
  : guard_ (poa.lock ())
{
  if (!this->guard_.locked ())
    ACE_THROW (
      CORBA::INTERNAL (
        CORBA_SystemException::_tao_minor_code (
          TAO_GUARD_FAILURE,
          0),
        CORBA::COMPLETED_NO));

  // Check if a non-servant upcall is in progress.  If a non-servant
  // upcall is in progress, wait for it to complete.  Unless of
  // course, the thread making the non-servant upcall is this thread.
  poa.object_adapter ().wait_for_non_servant_upcalls_to_complete (ACE_TRY_ENV);
  ACE_CHECK;

  if (check_for_destruction &&
      poa.cleanup_in_progress ())
    ACE_THROW (
      CORBA::BAD_INV_ORDER (
        CORBA_SystemException::_tao_minor_code (
          TAO_POA_BEING_DESTROYED,
          0),
        CORBA::COMPLETED_NO));
}

ACE_INLINE PortableServer::ThreadPolicyValue
TAO_POA_Policies::thread (void) const
{
  return this->thread_;
}

ACE_INLINE void
TAO_POA_Policies::thread (PortableServer::ThreadPolicyValue value)
{
  this->thread_ = value;
}

ACE_INLINE PortableServer::LifespanPolicyValue
TAO_POA_Policies::lifespan (void) const
{
  return this->lifespan_;
}

ACE_INLINE void
TAO_POA_Policies::lifespan (PortableServer::LifespanPolicyValue value)
{
  this->lifespan_ = value;
}

ACE_INLINE PortableServer::IdUniquenessPolicyValue
TAO_POA_Policies::id_uniqueness (void) const
{
  return this->id_uniqueness_;
}

ACE_INLINE void
TAO_POA_Policies::id_uniqueness (PortableServer::IdUniquenessPolicyValue value)
{
  this->id_uniqueness_ = value;
}

ACE_INLINE PortableServer::IdAssignmentPolicyValue
TAO_POA_Policies::id_assignment (void) const
{
  return this->id_assignment_;
}

ACE_INLINE void
TAO_POA_Policies::id_assignment (PortableServer::IdAssignmentPolicyValue value)
{
  this->id_assignment_ = value;
}

ACE_INLINE PortableServer::ImplicitActivationPolicyValue
TAO_POA_Policies::implicit_activation (void) const
{
  return this->implicit_activation_;
}

ACE_INLINE void
TAO_POA_Policies::implicit_activation (PortableServer::ImplicitActivationPolicyValue value)
{
  this->implicit_activation_ = value;
}

ACE_INLINE PortableServer::ServantRetentionPolicyValue
TAO_POA_Policies::servant_retention (void) const
{
  return this->servant_retention_;
}

ACE_INLINE void
TAO_POA_Policies::servant_retention (PortableServer::ServantRetentionPolicyValue value)
{
  this->servant_retention_ = value;
}

ACE_INLINE PortableServer::RequestProcessingPolicyValue
TAO_POA_Policies::request_processing (void) const
{
  return this->request_processing_;
}

ACE_INLINE void
TAO_POA_Policies::request_processing (PortableServer::RequestProcessingPolicyValue value)
{
  this->request_processing_ = value;
}

ACE_INLINE TAO_POA_Policies::PriorityModel
TAO_POA_Policies::priority_model (void) const
{
  return this->priority_model_;
}

ACE_INLINE void
TAO_POA_Policies::priority_model (PriorityModel value)
{
  this->priority_model_ = value;
}

ACE_INLINE CORBA::Short
TAO_POA_Policies::server_priority (void) const
{
  return this->server_priority_;
}

ACE_INLINE void
TAO_POA_Policies::server_priority (CORBA::Short value)
{
  this->server_priority_ = value;
}

#if (TAO_HAS_RT_CORBA == 1)

ACE_INLINE TAO_ServerProtocolPolicy *
TAO_POA_Policies::server_protocol (void) const
{
  return this->server_protocol_;
}

ACE_INLINE TAO_PriorityBandedConnectionPolicy *
TAO_POA_Policies::priority_bands (void) const
{
  return this->priority_bands_;
}

#endif /* TAO_HAS_RT_CORBA == 1 */

ACE_INLINE const CORBA::PolicyList &
TAO_POA_Policies::client_exposed_fixed_policies (void) const
{
  return this->client_exposed_fixed_policies_;
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
                     const CORBA::PolicyList &policies,
                     CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::AdapterAlreadyExists,
                   PortableServer::POA::InvalidPolicy))
{
  // Lock access for the duration of this transaction.
  TAO_POA_GUARD_RETURN (0);

  return this->create_POA_i (adapter_name,
                             poa_manager,
                             policies,
                             ACE_TRY_ENV);
}

ACE_INLINE void
TAO_POA::destroy (CORBA::Boolean etherealize_objects,
                  CORBA::Boolean wait_for_completion,
                  CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Lock access for the duration of this transaction.
  TAO_POA_Guard poa_guard (*this, ACE_TRY_ENV, 0);
  ACE_CHECK;
  ACE_UNUSED_ARG (poa_guard);

  this->destroy_i (etherealize_objects,
                   wait_for_completion,
                   ACE_TRY_ENV);
}

ACE_INLINE TAO_POA_Policies &
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
TAO_POA::get_servant_manager (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::WrongPolicy))
{
  // Lock access for the duration of this transaction.
  TAO_POA_GUARD_RETURN (PortableServer::ServantManager::_nil ());

  return this->get_servant_manager_i (ACE_TRY_ENV);
}

ACE_INLINE void
TAO_POA::set_servant_manager (PortableServer::ServantManager_ptr imgr,
                              CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::WrongPolicy))
{
  // Lock access for the duration of this transaction.
  TAO_POA_GUARD;

  this->set_servant_manager_i (imgr,
                               ACE_TRY_ENV);
}

ACE_INLINE PortableServer::Servant
TAO_POA::get_servant (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::NoServant,
                   PortableServer::POA::WrongPolicy))
{
  // Lock access for the duration of this transaction.
  TAO_POA_GUARD_RETURN (0);

  return this->get_servant_i (ACE_TRY_ENV);
}

ACE_INLINE void
TAO_POA::set_servant (PortableServer::Servant servant,
                      CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::WrongPolicy))
{
  // Lock access for the duration of this transaction.
  TAO_POA_GUARD;

  this->set_servant_i (servant,
                       ACE_TRY_ENV);
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

ACE_INLINE PortableServer::ObjectId *
TAO_POA::activate_object (PortableServer::Servant servant,
                          CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::ServantAlreadyActive,
                   PortableServer::POA::WrongPolicy))
{
  // Lock access for the duration of this transaction.
  TAO_POA_GUARD_RETURN (0);

  return this->activate_object_i (servant,
                                  this->policies ().server_priority (),
                                  ACE_TRY_ENV);
}

ACE_INLINE void
TAO_POA::activate_object_with_id (const PortableServer::ObjectId &id,
                                  PortableServer::Servant servant,
                                  CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::ServantAlreadyActive,
                   PortableServer::POA::ObjectAlreadyActive,
                   PortableServer::POA::WrongPolicy))
{
  // Lock access for the duration of this transaction.
  TAO_POA_GUARD;

  this->activate_object_with_id_i (id,
                                   servant,
                                   this->policies ().server_priority (),
                                   ACE_TRY_ENV);
}

ACE_INLINE void
TAO_POA::deactivate_object (const PortableServer::ObjectId &oid,
                            CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::ObjectNotActive,
                   PortableServer::POA::WrongPolicy))
{
  // Lock access for the duration of this transaction.
  TAO_POA_GUARD;

  this->deactivate_object_i (oid,
                             ACE_TRY_ENV);
}

ACE_INLINE CORBA::Object_ptr
TAO_POA::create_reference (const char *intf,
                           CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::WrongPolicy))
{
  // Lock access for the duration of this transaction.
  TAO_POA_GUARD_RETURN (CORBA::Object::_nil ());

  return this->create_reference_i (intf,
                                   this->policies ().server_priority (),
                                   ACE_TRY_ENV);
}

ACE_INLINE CORBA::Object_ptr
TAO_POA::create_reference_with_id (const PortableServer::ObjectId &id,
                                   const char *intf,
                                   CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::WrongPolicy))
{
  // Lock access for the duration of this transaction.
  TAO_POA_GUARD_RETURN (CORBA::Object::_nil ());

  return this->create_reference_with_id_i (id,
                                           intf,
                                           this->policies ().server_priority (),
                                           ACE_TRY_ENV);
}

ACE_INLINE PortableServer::ObjectId *
TAO_POA::servant_to_id (PortableServer::Servant servant,
                        CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::ServantNotActive,
                   PortableServer::POA::WrongPolicy))
{
  // If we had upgradeable locks, this would initially be a read lock
  //
  // Lock access for the duration of this transaction.
  TAO_POA_GUARD_RETURN (0);

  return this->servant_to_id_i (servant,
                                ACE_TRY_ENV);
}

ACE_INLINE PortableServer::ObjectId *
TAO_POA::servant_to_system_id (PortableServer::Servant servant,
                               CORBA::Short &priority,
                               CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::ServantNotActive,
                   PortableServer::POA::WrongPolicy))
{
  // Lock access for the duration of this transaction.
  TAO_POA_GUARD_RETURN (0);

  return this->servant_to_system_id_i (servant,
                                       priority,
                                       ACE_TRY_ENV);
}

ACE_INLINE PortableServer::Servant
TAO_POA::id_to_servant (const PortableServer::ObjectId &oid,
                        CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::ObjectNotActive,
                   PortableServer::POA::WrongPolicy))
{
  // Lock access for the duration of this transaction.
  TAO_POA_GUARD_RETURN (0);

  return this->id_to_servant_i (oid,
                                ACE_TRY_ENV);
}

ACE_INLINE CORBA::Object_ptr
TAO_POA::id_to_reference (const PortableServer::ObjectId &oid,
                          CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::ObjectNotActive,
                   PortableServer::POA::WrongPolicy))
{
  // Lock access for the duration of this transaction.
  TAO_POA_GUARD_RETURN (0);

  return this->id_to_reference_i (oid, ACE_TRY_ENV);
}

#if (TAO_HAS_RT_CORBA == 1)

ACE_INLINE CORBA::Object_ptr
TAO_POA::create_reference_with_priority (const char * intf,
                                         RTCORBA::Priority priority,
                                         CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::WrongPolicy))
{
  this->validate_policies (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  if (!this->valid_priority (priority))
    {
      ACE_THROW_RETURN (CORBA::BAD_PARAM (),
                        0);
    }

  // Lock access for the duration of this transaction.
  TAO_POA_GUARD_RETURN (0);

  return this->create_reference_i (intf,
                                   priority,
                                   ACE_TRY_ENV);
}

ACE_INLINE CORBA::Object_ptr
TAO_POA::create_reference_with_id_and_priority (const PortableServer::ObjectId & oid,
                                                const char * intf,
                                                RTCORBA::Priority priority,
                                                CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::WrongPolicy))
{
  this->validate_policies (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  if (!this->valid_priority (priority))
    {
      ACE_THROW_RETURN (CORBA::BAD_PARAM (),
                        0);
    }

  // Lock access for the duration of this transaction.
  TAO_POA_GUARD_RETURN (0);

  return this->create_reference_with_id_i (oid,
                                           intf,
                                           priority,
                                           ACE_TRY_ENV);
}

ACE_INLINE PortableServer::ObjectId *
TAO_POA::activate_object_with_priority (PortableServer::Servant servant,
                                        RTCORBA::Priority priority,
                                        CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::ServantAlreadyActive,
                   PortableServer::POA::WrongPolicy))
{
  this->validate_policies (ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  if (!this->valid_priority (priority))
    {
      ACE_THROW_RETURN (CORBA::BAD_PARAM (),
                        0);
    }

  // Lock access for the duration of this transaction.
  TAO_POA_GUARD_RETURN (0);

  return this->activate_object_i (servant,
                                  priority,
                                  ACE_TRY_ENV);
}

ACE_INLINE void
TAO_POA::activate_object_with_id_and_priority (const PortableServer::ObjectId & oid,
                                               PortableServer::Servant servant,
                                               RTCORBA::Priority priority,
                                               CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::ServantAlreadyActive,
                   PortableServer::POA::ObjectAlreadyActive,
                   PortableServer::POA::WrongPolicy))
{
  this->validate_policies (ACE_TRY_ENV);
  ACE_CHECK;

  if (!this->valid_priority (priority))
    {
      ACE_THROW (CORBA::BAD_PARAM ());
    }

  // Lock access for the duration of this transaction.
  TAO_POA_GUARD;

  this->activate_object_with_id_i (oid,
                                   servant,
                                   priority,
                                   ACE_TRY_ENV);
}

#endif /* TAO_HAS_RT_CORBA */


ACE_INLINE PortableServer::POA_ptr
TAO_POA::the_parent (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (this->parent_ != 0)
    return PortableServer::POA::_duplicate (this->parent_);
  else
    return PortableServer::POA::_nil ();
}

ACE_INLINE PortableServer::POAList *
TAO_POA::the_children (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Lock access for the duration of this transaction.
  TAO_POA_GUARD_RETURN (0);

  return this->the_children_i (ACE_TRY_ENV);
}

ACE_INLINE PortableServer::POAManager_ptr
TAO_POA::the_POAManager (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return PortableServer::POAManager::_duplicate (&this->poa_manager_);
}

#if (TAO_HAS_MINIMUM_POA == 0)

ACE_INLINE PortableServer::AdapterActivator_ptr
TAO_POA::the_activator (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Lock access for the duration of this transaction.
  TAO_POA_GUARD_RETURN (PortableServer::AdapterActivator::_nil ());

  return PortableServer::AdapterActivator::_duplicate (this->adapter_activator_.in ());
}

ACE_INLINE void
TAO_POA::the_activator (PortableServer::AdapterActivator_ptr adapter_activator,
                        CORBA::Environment &ACE_TRY_ENV)
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
  return (CORBA::Boolean) this->system_id_;
}


ACE_INLINE CORBA::Boolean
TAO_POA::root (void)
{
  return this->parent_ == 0;
}


ACE_INLINE CORBA::Boolean
TAO_POA::persistent (void)
{
  return (CORBA::Boolean) this->persistent_;
}

ACE_INLINE const ACE_CString &
TAO_POA::name (void) const
{
  return this->name_;
}

ACE_INLINE char *
TAO_POA::the_name (CORBA::Environment &)
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
