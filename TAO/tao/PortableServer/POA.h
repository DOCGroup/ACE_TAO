// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    POA.h
//
// = DESCRIPTION
//     POA
//
// = AUTHOR
//     Irfan Pyarali
//
// ============================================================================

#ifndef TAO_POA_H
#define TAO_POA_H
#include "ace/pre.h"

// String
#include "ace/SString.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// Object Adapter
#include "Object_Adapter.h"

// POA Manager
#include "POAManager.h"

// POA Policy Set
#include "POA_Policy_Set.h"

// Cached POA Policies
#include "POA_Cached_Policies.h"

// Object_Key
#include "tao/Object_KeyC.h"

// Local Object
#include "tao/LocalObject.h"

// Map
#include "ace/Hash_Map_Manager.h"

// Vector
#include "ace/Containers.h"

// Locking
#include "ace/Synch.h"

// OctetSeq
#include "tao/OctetSeqC.h"

// This is to remove "inherits via dominance" warnings from MSVC.
// MSVC is being a little too paranoid.
#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_Acceptor_Filter;
class TAO_Acceptor_Registry;
class TAO_Temporary_Creation_Time;

class TAO_Creation_Time
{
public:

  TAO_Creation_Time (const ACE_Time_Value &creation_time);

  TAO_Creation_Time (void);

  void creation_time (const void *creation_time);

  const void *creation_time (void) const;

  static CORBA::ULong creation_time_length (void);

  int operator== (const TAO_Creation_Time &rhs) const;

  int operator!= (const TAO_Creation_Time &rhs) const;

  int operator== (const TAO_Temporary_Creation_Time &rhs) const;

  int operator!= (const TAO_Temporary_Creation_Time &rhs) const;

protected:

  enum
  {
    SEC_FIELD = 0,
    USEC_FIELD = 1
  };

  // Timestamp
  CORBA::ULong time_stamp_[2];

};

// Special creation time only useful in the lifetime of the upcall
class TAO_Temporary_Creation_Time
{
public:

  TAO_Temporary_Creation_Time (void);

  void creation_time (const void *creation_time);

  int operator== (const TAO_Creation_Time &rhs) const;

  int operator!= (const TAO_Creation_Time &rhs) const;

protected:

  void *time_stamp_;
};

// Forward Declaration
class ServerObject_i;

class TAO_PortableServer_Export TAO_POA :
  public virtual PortableServer::POA ,
  public virtual TAO_Local_RefCounted_Object
{
public:

  friend class TAO_Object_Adapter;
  friend class TAO_Object_Adapter::Servant_Upcall;
  friend class TAO_Object_Adapter::Non_Servant_Upcall;
  friend class TAO_POA_Current_Impl;
  friend class TAO_POA_Manager;
  friend class TAO_RT_Collocation_Resolver;
  friend class TAO_ObjectReferenceFactory;
  friend class TAO_ObjectReferenceTemplate;

  typedef ACE_CString String;

  /**
   * This method is used to downcast safely an instance of PortableServer::POA
   * to an instance of TAO_POA when RTTI is not enabled.
   */
  virtual TAO_POA* _tao_poa_downcast (void);

  PortableServer::POA_ptr create_POA (const char *adapter_name,
                                      PortableServer::POAManager_ptr poa_manager,
                                      const CORBA::PolicyList &policies
                                      TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::AdapterAlreadyExists,
                     PortableServer::POA::InvalidPolicy));

  PortableServer::POA_ptr find_POA (const char *adapter_name,
                                    CORBA::Boolean activate_it
                                    TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::AdapterNonExistent));

  void destroy (CORBA::Boolean etherealize_objects,
                CORBA::Boolean wait_for_completion
                TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

#if (TAO_HAS_MINIMUM_POA == 0)

  PortableServer::ThreadPolicy_ptr create_thread_policy (PortableServer::ThreadPolicyValue value
                                                         TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

#endif /* TAO_HAS_MINIMUM_POA == 0 */

  PortableServer::LifespanPolicy_ptr create_lifespan_policy (PortableServer::LifespanPolicyValue value
                                                             TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  PortableServer::IdUniquenessPolicy_ptr create_id_uniqueness_policy (PortableServer::IdUniquenessPolicyValue value
                                                                      TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  PortableServer::IdAssignmentPolicy_ptr create_id_assignment_policy (PortableServer::IdAssignmentPolicyValue value
                                                                      TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

#if (TAO_HAS_MINIMUM_POA == 0)

  PortableServer::ImplicitActivationPolicy_ptr create_implicit_activation_policy (PortableServer::ImplicitActivationPolicyValue value
                                                                                  TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  PortableServer::ServantRetentionPolicy_ptr create_servant_retention_policy (PortableServer::ServantRetentionPolicyValue value
                                                                              TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  PortableServer::RequestProcessingPolicy_ptr create_request_processing_policy (PortableServer::RequestProcessingPolicyValue value
                                                                                TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

#endif /* TAO_HAS_MINIMUM_POA == 0 */

  char * the_name (TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  PortableServer::POA_ptr the_parent (TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  PortableServer::POAList *the_children (TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  PortableServer::POAManager_ptr the_POAManager (TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  PortableInterceptor::AdapterManagerId get_manager_id (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// This method returns the adapter_name as a sequence of strings of
  /// length one or more or just a fixed name depending on the Object
  /// Adapter. Added wrt to ORT Spec.
  CORBA::StringSeq *adapter_name (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// {@ Accessor methods to ObjectReferenceTemplate
  PortableInterceptor::ObjectReferenceTemplate * get_adapter_template ();

  void set_adapter_template (PortableInterceptor::ObjectReferenceTemplate *
                             object_ref_template
                             TAO_ENV_ARG_DECL);
  /// @}

  /// {@ Accessor methods to PortableInterceptor::ObjectReferenceFactory
  PortableInterceptor::ObjectReferenceFactory * get_obj_ref_factory ();

  void set_obj_ref_factory (PortableInterceptor::ObjectReferenceFactory *
                            current_factory
                            TAO_ENV_ARG_DECL);
  /// @}

  /// Call the establish components.
  void tao_establish_components (TAO_ENV_SINGLE_ARG_DECL);

  /// Give each registered IOR interceptor the opportunity to add
  /// tagged components to profiles of each created servant.
  void establish_components (PortableInterceptor::IORInfo *info
                             TAO_ENV_ARG_DECL);

  /// TAO_IORInfo requests these members.
  CORBA::PolicyList *get_policy_list ();

  TAO_MProfile *get_mprofile ();

  void
  save_ior_component (const IOP::TaggedComponent &component
                      TAO_ENV_ARG_DECL);

  void
  save_ior_component_and_profile_id (const IOP::TaggedComponent &component,
                                     IOP::ProfileId profile_id
                                     TAO_ENV_ARG_DECL);


#if (TAO_HAS_MINIMUM_POA == 0)

  PortableServer::AdapterActivator_ptr the_activator (TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void the_activator (PortableServer::AdapterActivator_ptr adapter_activator
                      TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  PortableServer::ServantManager_ptr get_servant_manager (TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::WrongPolicy));

  void set_servant_manager (PortableServer::ServantManager_ptr imgr
                            TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::WrongPolicy));

  PortableServer::Servant get_servant (TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::NoServant,
                     PortableServer::POA::WrongPolicy));

  void set_servant (PortableServer::Servant servant
                    TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::WrongPolicy));

#endif /* TAO_HAS_MINIMUM_POA == 0 */

  PortableServer::ObjectId *activate_object (PortableServer::Servant p_servant
                                             TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::ServantAlreadyActive,
                     PortableServer::POA::WrongPolicy));

  void activate_object_with_id (const PortableServer::ObjectId &id,
                                PortableServer::Servant p_servant
                                TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::ServantAlreadyActive,
                     PortableServer::POA::ObjectAlreadyActive,
                     PortableServer::POA::WrongPolicy));

  void deactivate_object (const PortableServer::ObjectId &oid
                          TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::ObjectNotActive,
                     PortableServer::POA::WrongPolicy));

  CORBA::Object_ptr create_reference (const char *intf
                                      TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::WrongPolicy));

  CORBA::Object_ptr create_reference_with_id (const PortableServer::ObjectId &oid,
                                              const char *intf
                                              TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::WrongPolicy));

  PortableServer::ObjectId *servant_to_id (PortableServer::Servant p_servant
                                           TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::ServantNotActive,
                     PortableServer::POA::WrongPolicy));

  CORBA::Object_ptr servant_to_reference (PortableServer::Servant p_servant
                                          TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::ServantNotActive,
                     PortableServer::POA::WrongPolicy));

  PortableServer::Servant reference_to_servant (CORBA::Object_ptr reference
                                                TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::ObjectNotActive,
                     PortableServer::POA::WrongAdapter,
                     PortableServer::POA::WrongPolicy));

  PortableServer::ObjectId *reference_to_id (CORBA::Object_ptr reference
                                             TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::WrongAdapter,
                     PortableServer::POA::WrongPolicy));

  PortableServer::Servant id_to_servant (const PortableServer::ObjectId &oid
                                         TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::ObjectNotActive,
                     PortableServer::POA::WrongPolicy));

  CORBA::Object_ptr id_to_reference (const PortableServer::ObjectId &oid
                                     TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::ObjectNotActive,
                     PortableServer::POA::WrongPolicy));

  CORBA::OctetSeq *id (TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

#if (TAO_HAS_MINIMUM_POA == 0)
  // Methods added by the MIOP specification.

  virtual PortableServer::ObjectId * create_id_for_reference (
      CORBA::Object_ptr the_ref
      TAO_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      PortableServer::NotAGroupObject
    ));


  virtual PortableServer::IDs * reference_to_ids (
      CORBA::Object_ptr the_ref
      TAO_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      PortableServer::NotAGroupObject
    ));

  virtual void associate_reference_with_id (
      CORBA::Object_ptr ref,
      const PortableServer::ObjectId & oid
      TAO_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      PortableServer::NotAGroupObject
    ));

  virtual void disassociate_reference_with_id (
      CORBA::Object_ptr ref,
      const PortableServer::ObjectId & oid
      TAO_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      PortableServer::NotAGroupObject
    ));

  // End methods added by MIOP.
#endif /* TAO_HAS_MINIMUM_POA == 0 */


  /// Accessor for POA policies.
  TAO_POA_Policy_Set &policies (void);

  /// Accessor for cached POA policies.
  TAO_POA_Cached_Policies &cached_policies (void);

  virtual CORBA::PolicyList *client_exposed_policies (CORBA::Short object_priority
                                                      TAO_ENV_ARG_DECL);
  // This method gives the policies that are exposed to the client.
  // These policies are shipped within the IOR.


  // Utility functions for the other
  static char* ObjectId_to_string (const PortableServer::ObjectId &id);

  static CORBA::WChar* ObjectId_to_wstring (const PortableServer::ObjectId &id);

  static PortableServer::ObjectId *string_to_ObjectId (const char *id);

  static PortableServer::ObjectId *string_to_ObjectId (const char *string,
                                                       int size);

  static PortableServer::ObjectId *wstring_to_ObjectId (const CORBA::WChar *id);

  TAO_POA (const String &name,
           TAO_POA_Manager &poa_manager,
           const TAO_POA_Policy_Set &policies,
           TAO_POA *parent,
           ACE_Lock &lock,
           TAO_SYNCH_MUTEX &thread_lock,
           TAO_ORB_Core &orb_core,
           TAO_Object_Adapter *object_adapter
           TAO_ENV_ARG_DECL);

  virtual ~TAO_POA (void);

  static char name_separator (void);

  static CORBA::ULong name_separator_length (void);

  enum
  {
    TAO_OBJECTKEY_PREFIX_SIZE = 4
  };

  static CORBA::Octet objectkey_prefix[TAO_OBJECTKEY_PREFIX_SIZE];

  const TAO_Object_Adapter::poa_name &folded_name (void) const;

  const TAO_Object_Adapter::poa_name &system_name (void) const;

  static void check_for_valid_wait_for_completions (const TAO_ORB_Core &orb_core,
                                                    CORBA::Boolean wait_for_completion
                                                    TAO_ENV_ARG_DECL);

  TAO_ORB_Core &orb_core (void) const;
  // ORB Core for POA.

  CORBA::Boolean cleanup_in_progress (void);

  TAO_Object_Adapter &object_adapter (void);

  ACE_Lock &lock (void);


  TAO_Stub* key_to_stub (const TAO_ObjectKey &key,
                         const char *type_id,
                         CORBA::Short priority
                         TAO_ENV_ARG_DECL);
  // Create the correct stub, properly initialized with the attributes
  // and policies attached to the current POA.

  /// Accessor for the current thread policy of this POA.
  PortableServer::ThreadPolicyValue thread_policy (void) const;

  /// {@ Accesor methods to POA state. The POA can be in one of
  /// HOLDING, ACTIVE, DISCARDING, INACTIVE and NON_EXISTENT states.
  PortableInterceptor::AdapterState
  get_adapter_state (TAO_ENV_SINGLE_ARG_DECL);

  void set_adapter_state (PortableInterceptor::AdapterState
                          adapter_state,
                          CORBA::Environment &ACE_TRY_ENV);
  /// @}

  virtual void *thread_pool (void) const;

protected:

  const ACE_CString &name (void) const;

  /// Template method for creating new POA's of this type.
  virtual TAO_POA *new_POA (const String &name,
                            TAO_POA_Manager &poa_manager,
                            const TAO_POA_Policy_Set &policies,
                            TAO_POA *parent,
                            ACE_Lock &lock,
                            TAO_SYNCH_MUTEX &thread_lock,
                            TAO_ORB_Core &orb_core,
                            TAO_Object_Adapter *object_adapter
                            TAO_ENV_ARG_DECL);

  PortableServer::POA_ptr create_POA_i (const char *adapter_name,
                                        PortableServer::POAManager_ptr poa_manager,
                                        const CORBA::PolicyList &policies
                                        TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::AdapterAlreadyExists,
                     PortableServer::POA::InvalidPolicy));

  TAO_POA *create_POA_i (const String &adapter_name,
                         TAO_POA_Manager &poa_manager,
                         const TAO_POA_Policy_Set &policies
                         TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::AdapterAlreadyExists,
                     PortableServer::POA::InvalidPolicy));

  TAO_POA *find_POA_i (const ACE_CString &child_name,
                       CORBA::Boolean activate_it
                       TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::AdapterNonExistent));

  void destroy_i (CORBA::Boolean etherealize_objects,
                  CORBA::Boolean wait_for_completion
                  TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void complete_destruction_i (TAO_ENV_SINGLE_ARG_DECL);

  PortableServer::POAList *the_children_i (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// This method returns the adapter_name as a sequence of strings of
  /// length one or more or just a fixed name depending on the Object
  /// Adapter. Added wrt to ORT Spec.
  CORBA::StringSeq *adapter_name_i (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Method to notify the IOR Interceptors when there is a state
  /// changed not related to POAManager.
  void adapter_state_changed (
   const PortableInterceptor::ObjectReferenceTemplateSeq *seq_obj_ref_template,
   PortableInterceptor::AdapterState state);

  void set_policy_list (CORBA::PolicyList *policy_list);
  void set_mprofile (TAO_MProfile *mp);

  /// Add the given tagged component to all profiles.
  void tao_add_ior_component (
      const IOP::TaggedComponent & component
      TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Add the given tagged component to all profiles matching the given
  /// ProfileId.
  void tao_add_ior_component_to_profile (
      const IOP::TaggedComponent & component,
      IOP::ProfileId profile_id
      TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

#if (TAO_HAS_MINIMUM_POA == 0)

  PortableServer::ServantManager_ptr get_servant_manager_i (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::WrongPolicy));

  void set_servant_manager_i (PortableServer::ServantManager_ptr imgr
                              TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::WrongPolicy));

  PortableServer::Servant get_servant_i (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::NoServant,
                     PortableServer::POA::WrongPolicy));

  void set_servant_i (PortableServer::Servant servant
                      TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::WrongPolicy));

#endif /* TAO_HAS_MINIMUM_POA == 0 */

//
// ImplRepo related.
//
#if (TAO_HAS_MINIMUM_CORBA == 0)

  void imr_notify_startup (TAO_ENV_SINGLE_ARG_DECL);
  // ImplRepo helper method, notify the ImplRepo on startup

  void imr_notify_shutdown (void);
  // ImplRepo helper method, notify the ImplRepo on shutdown

  CORBA::Object_ptr invoke_key_to_object (const char *intf,
                                          PortableServer::ObjectId
                                          &user_id
                                          TAO_ENV_ARG_DECL);


#endif /* TAO_HAS_MINIMUM_CORBA */

  CORBA::Object_ptr key_to_object (const TAO_ObjectKey &key,
                                   const char *type_id,
                                   TAO_ServantBase *servant,
                                   CORBA::Boolean collocated,
                                   CORBA::Short priority
                                   TAO_ENV_ARG_DECL);
  // Wrapper for the ORB's key_to_object that will alter the object pointer
  // if the ImplRepo is used.

  virtual TAO_Stub* key_to_stub_i (const TAO_ObjectKey &key,
                                   const char *type_id,
                                   CORBA::Short priority
                                   TAO_ENV_ARG_DECL);
  // Like key_to_stub() but assume that the ORB is not shutting down.

  TAO_Stub *create_stub_object (const TAO_ObjectKey &object_key,
                                const char *type_id,
                                CORBA::PolicyList *policy_list,
                                TAO_Acceptor_Filter *filter,
                                TAO_Acceptor_Registry &acceptor_registry
                                TAO_ENV_ARG_DECL);

  int is_servant_in_map (PortableServer::Servant servant);

  int is_user_id_in_map (const PortableServer::ObjectId &user_id,
                         CORBA::Short priority,
                         int &priorities_match);

  PortableServer::ObjectId *activate_object_i (PortableServer::Servant p_servant,
                                               CORBA::Short priority
                                               TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::ServantAlreadyActive,
                     PortableServer::POA::WrongPolicy));

  void activate_object_with_id_i (const PortableServer::ObjectId &id,
                                  PortableServer::Servant p_servant,
                                  CORBA::Short priority
                                  TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::ServantAlreadyActive,
                     PortableServer::POA::ObjectAlreadyActive,
                     PortableServer::POA::WrongPolicy));

  void deactivate_all_objects_i (CORBA::Boolean etherealize_objects
                                 TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::WrongPolicy));

  void deactivate_all_objects_i (CORBA::Boolean etherealize_objects,
                                 CORBA::Boolean wait_for_completion
                                 TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::WrongPolicy));

  void wait_for_completions (CORBA::Boolean wait_for_completion
                             TAO_ENV_ARG_DECL);

  void check_poa_manager_state (TAO_ENV_SINGLE_ARG_DECL);

  void deactivate_object_i (const PortableServer::ObjectId &oid
                            TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::ObjectNotActive,
                   PortableServer::POA::WrongPolicy));

  void cleanup_servant (TAO_Active_Object_Map::Map_Entry *active_object_map_entry
                        TAO_ENV_ARG_DECL);

  void deactivate_map_entry (TAO_Active_Object_Map::Map_Entry *active_object_map_entry
                             TAO_ENV_ARG_DECL);

  CORBA::Object_ptr create_reference_i (const char *intf
                                        TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::WrongPolicy));

  CORBA::Object_ptr create_reference_with_id_i (const PortableServer::ObjectId &oid,
                                                const char *intf
                                                TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::WrongPolicy));

  PortableServer::ObjectId *servant_to_id_i (PortableServer::Servant servant
                                             TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::ServantNotActive,
                     PortableServer::POA::WrongPolicy));

  PortableServer::ObjectId *servant_to_system_id (PortableServer::Servant p_servant,
                                                  CORBA::Short &priority
                                                  TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::ServantNotActive,
                     PortableServer::POA::WrongPolicy));

  PortableServer::ObjectId *servant_to_system_id_i (PortableServer::Servant p_servant,
                                                    CORBA::Short &priority
                                                    TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::ServantNotActive,
                     PortableServer::POA::WrongPolicy));

  PortableServer::Servant id_to_servant_i (const PortableServer::ObjectId &oid
                                           TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::ObjectNotActive,
                     PortableServer::POA::WrongPolicy));

  CORBA::Object_ptr id_to_reference_i (const PortableServer::ObjectId &oid
                                       TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::ObjectNotActive,
                     PortableServer::POA::WrongPolicy));

  TAO_Active_Object_Map &active_object_map (void) const;

  void components_established_i (PortableInterceptor::IORInfo *info
                                 TAO_ENV_ARG_DECL);

  int delete_child (const String &child);

  void set_folded_name (void);

  void set_id (void);

  TAO_ObjectKey *create_object_key (const PortableServer::ObjectId &id);

  int is_poa_generated_id (const PortableServer::ObjectId &id);

  static int parse_key (const TAO_ObjectKey &key,
                        TAO_Object_Adapter::poa_name &poa_system_name,
                        PortableServer::ObjectId &system_id,
                        CORBA::Boolean &is_root,
                        CORBA::Boolean &is_persistent,
                        CORBA::Boolean &is_system_id,
                        TAO_Temporary_Creation_Time &poa_creation_time);

  /// Access the list of default POA policies.  This list is used as a
  /// prototype for creating new POA's.  It should
  static TAO_POA_Policy_Set &default_poa_policies (void);

protected:

  TAO_SERVANT_LOCATION locate_servant_i (const PortableServer::ObjectId &id,
                                         PortableServer::Servant &servant
                                         TAO_ENV_ARG_DECL);

  PortableServer::Servant locate_servant_i (const char *operation,
                                            const PortableServer::ObjectId &id,
                                            TAO_Object_Adapter::Servant_Upcall &servant_upcall,
                                            TAO_POA_Current_Impl &poa_current_impl
                                            TAO_ENV_ARG_DECL);

  const TAO_Creation_Time &creation_time (void);

  CORBA::Boolean persistent (void);

  char persistent_key_type (void);

  static char persistent_key_char (void);

  static char transient_key_char (void);

  static CORBA::ULong persistent_key_type_length (void);

  CORBA::Boolean system_id (void);

  char system_id_key_type (void);

  static char system_id_key_char (void);

  static char user_id_key_char (void);

  static CORBA::ULong system_id_key_type_length (void);

  CORBA::Boolean root (void);

  char root_key_type (void);

  static char root_key_char (void);

  static char non_root_key_char (void);

  static CORBA::ULong root_key_type_length (void);

  CORBA::ULong outstanding_requests (void) const;

  void outstanding_requests (CORBA::ULong new_outstanding_requests);

  CORBA::ULong increment_outstanding_requests (void);

  CORBA::ULong decrement_outstanding_requests (void);

  TAO_SYNCH_RECURSIVE_MUTEX &single_threaded_lock (void) const;

  CORBA::Boolean waiting_destruction (void) const;

  String name_;

  TAO_POA_Manager &poa_manager_;

  CORBA::PolicyList *policy_list_;

  TAO_MProfile *mprofile_;

  IOP::TaggedComponent tagged_component_;

  IOP::ProfileId profile_id_;

  CORBA::Boolean add_component_support_;

  TAO_POA_Policy_Set policies_;

  TAO_POA *parent_;

  TAO_Active_Object_Map *active_object_map_;

  TAO_Object_Adapter::poa_name folded_name_;

  TAO_Object_Adapter::poa_name_var system_name_;

  CORBA::OctetSeq id_;

  PortableInterceptor::ObjectReferenceTemplate *ort_template_;

  PortableInterceptor::ObjectReferenceFactory *obj_ref_factory_;

  /// Adapter can be accepting, rejecting etc.
  PortableInterceptor::AdapterState adapter_state_;

  TAO_POA_Cached_Policies cached_policies_;

  int delete_active_object_map_;

#if (TAO_HAS_MINIMUM_POA == 0)

  PortableServer::AdapterActivator_var adapter_activator_;

  PortableServer::ServantActivator_var servant_activator_;

  PortableServer::ServantLocator_var servant_locator_;

  PortableServer::ServantBase_var default_servant_;

#endif /* TAO_HAS_MINIMUM_POA == 0 */

#if (TAO_HAS_MINIMUM_CORBA == 0)

  ServerObject_i *server_object_;
  // Implementation Repository Server Object

  int use_imr_;
  // Flag for whether the IR should be used or not.

#endif /* TAO_HAS_MINIMUM_CORBA */

  typedef ACE_Hash_Map_Manager<ACE_CString, TAO_POA *, ACE_Null_Mutex>
  CHILDREN;

  CHILDREN children_;

  ACE_Lock &lock_;

  TAO_Creation_Time creation_time_;

  TAO_ORB_Core &orb_core_;

  // The object adapter we belong to
  TAO_Object_Adapter *object_adapter_;

  CORBA::Boolean cleanup_in_progress_;

  CORBA::Boolean etherealize_objects_;

  CORBA::ULong outstanding_requests_;

  TAO_SYNCH_CONDITION outstanding_requests_condition_;

  CORBA::Boolean wait_for_completion_pending_;

  CORBA::Boolean waiting_destruction_;

  TAO_SYNCH_CONDITION servant_deactivation_condition_;

  CORBA::ULong waiting_servant_deactivation_;

  TAO_SYNCH_RECURSIVE_MUTEX *single_threaded_lock_;

  CORBA::ULong caller_key_to_object_;

  PortableServer::Servant servant_for_key_to_object_;
};


class TAO_PortableServer_Export TAO_POA_Guard
{
public:
  TAO_POA_Guard (TAO_POA &poa
                 TAO_ENV_ARG_DECL,
                 int check_for_destruction = 1);

private:
  ACE_Guard<ACE_Lock> guard_;
};

#if (TAO_HAS_MINIMUM_POA == 0)

class TAO_PortableServer_Export TAO_Adapter_Activator : public PortableServer::AdapterActivator
{
public:

  TAO_Adapter_Activator (PortableServer::POAManager_ptr poa_manager);

  CORBA::Boolean unknown_adapter (PortableServer::POA_ptr parent,
                                  const char *name
                                  TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

protected:

  PortableServer::POAManager_var poa_manager_;
  // POA Manager
};

#endif /* TAO_HAS_MINIMUM_POA == 0 */

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#if defined (__ACE_INLINE__)
# include "POA.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_POA_H */
