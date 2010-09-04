// -*- C++ -*-

//=============================================================================
/**
 *  @file    Root_POA.h
 *
 *  $Id$
 *
 *  Header file for CORBA's ORB type.
 *
 *  @author  Irfan Pyarali <irfan@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_ROOT_POA_H
#define TAO_ROOT_POA_H

#include /**/ "ace/pre.h"

#include "tao/PortableServer/portableserver_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// Object Adapter
#include "tao/PortableServer/Object_Adapter.h"

// POA Policy Set
#include "tao/PortableServer/POA_Policy_Set.h"

// Cached POA Policies
#include "tao/PortableServer/POA_Cached_Policies.h"
#include "tao/PortableServer/Active_Policy_Strategies.h"

#include "tao/PortableServer/ORT_Adapter.h"

#include "tao/PortableServer/PortableServer.h"

// Object_Key
#include "tao/Object_KeyC.h"

// Local Object
#include "tao/LocalObject.h"

// Portable Interceptor
#include "tao/PI_ForwardC.h"

// OctetSeq
#include "tao/OctetSeqC.h"

#include "ace/SString.h"
#include "ace/Hash_Map_Manager_T.h"
#include "ace/Array_Base.h"
#include "ace/Synch_Traits.h"
#include "ace/Thread_Mutex.h"
#include "ace/Recursive_Thread_Mutex.h"
#include "ace/Null_Mutex.h"

// This is to remove "inherits via dominance" warnings from MSVC.
// MSVC is being a little too paranoid.
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Forward Declaration
class TAO_Acceptor_Filter;

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)
class TAO_Acceptor_Filter_Factory;
class TAO_POAManager_Factory;
#endif

class TAO_Network_Priority_Hook;
class TAO_Acceptor_Registry;
class TAO_IORInfo;
class TAO_Regular_POA;
class TAO_Active_Object_Map;

namespace PortableInterceptor
{
  class IORInfo;
  typedef IORInfo *IORInfo_ptr;
}

namespace TAO
{
  class ORT_Adapter;
  class ORT_Adapter_Factory;

  namespace Portable_Server
  {
    class Servant_Upcall;
    class POA_Current_Impl;
    class Temporary_Creation_Time;
  }
}

namespace PortableServer
{
  class POAManager;
  typedef POAManager *POAManager_ptr;
}

/**
 * @class TAO_Root_POA
 *
 * @brief Implementation of the PortableServer::POA interface.
 *
 * Implementation of the PortableServer::POA interface.
 */
class TAO_PortableServer_Export TAO_Root_POA
  : public virtual PortableServer::POA,
    public virtual ::CORBA::LocalObject
{
public:

  friend class TAO_Object_Adapter;
  friend class TAO::Portable_Server::Servant_Upcall;
  friend class TAO::Portable_Server::Non_Servant_Upcall;
  friend class TAO_POA_Manager;
  friend class TAO_RT_Collocation_Resolver;
  friend class TAO_IORInfo;

  typedef ACE_CString String;

  /// @note Temporarily for debugging, useful for debugging and status retrieval,
  /// could be removed with future rework of the AOM.
  TAO_Active_Object_Map * get_active_object_map() const;

#if !defined (CORBA_E_MICRO)
  PortableServer::POA_ptr create_POA (
      const char *adapter_name,
      PortableServer::POAManager_ptr poa_manager,
      const CORBA::PolicyList &policies);

  PortableServer::POA_ptr find_POA (const char *adapter_name,
                                    CORBA::Boolean activate_it);
#endif

  void destroy (CORBA::Boolean etherealize_objects,
                CORBA::Boolean wait_for_completion);

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)
  PortableServer::ThreadPolicy_ptr create_thread_policy (
      PortableServer::ThreadPolicyValue value);
#endif /* TAO_HAS_MINIMUM_POA == 0 */

#if !defined (CORBA_E_MICRO)
  PortableServer::LifespanPolicy_ptr create_lifespan_policy (
      PortableServer::LifespanPolicyValue value);
#endif

#if !defined (CORBA_E_MICRO)
  PortableServer::IdUniquenessPolicy_ptr create_id_uniqueness_policy (
      PortableServer::IdUniquenessPolicyValue value);
#endif

#if !defined (CORBA_E_MICRO)
  PortableServer::IdAssignmentPolicy_ptr create_id_assignment_policy (
      PortableServer::IdAssignmentPolicyValue value);
#endif

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)
  PortableServer::ImplicitActivationPolicy_ptr
  create_implicit_activation_policy (
      PortableServer::ImplicitActivationPolicyValue value);

  PortableServer::ServantRetentionPolicy_ptr
  create_servant_retention_policy (
      PortableServer::ServantRetentionPolicyValue value);

  PortableServer::RequestProcessingPolicy_ptr
  create_request_processing_policy (
      PortableServer::RequestProcessingPolicyValue value);

#endif /* TAO_HAS_MINIMUM_POA == 0 && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO) */

  char * the_name (void);

  PortableServer::POA_ptr the_parent (void);

  PortableServer::POAList *the_children (void);

  PortableServer::POAManager_ptr the_POAManager (void);

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)
  PortableServer::POAManagerFactory_ptr the_POAManagerFactory (void);
#endif

  /// This method returns the adapter_name as a sequence of strings of
  /// length one or more or just a fixed name depending on the Object
  /// Adapter. Added wrt to ORT Spec.
  PortableInterceptor::AdapterName *adapter_name (void);

  /// Store the given TaggedComponent for eventual insertion into all
  /// object reference profiles.
  void save_ior_component (const IOP::TaggedComponent &component);

  /// Store the given TaggedComponent for eventual insertion into all
  /// object reference profiles with the given ProfileId.
  void save_ior_component_and_profile_id (
    const IOP::TaggedComponent &component,
    IOP::ProfileId profile_id);

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)
  PortableServer::AdapterActivator_ptr the_activator (void);

  void the_activator (PortableServer::AdapterActivator_ptr adapter_activator);

  PortableServer::ServantManager_ptr get_servant_manager (void);

  void set_servant_manager (PortableServer::ServantManager_ptr imgr);

  PortableServer::Servant get_servant (void);

  void set_servant (PortableServer::Servant servant);
#endif /* TAO_HAS_MINIMUM_POA == 0 !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO) */

  PortableServer::ObjectId *activate_object (PortableServer::Servant p_servant);

  void activate_object_with_id (const PortableServer::ObjectId &id,
                                PortableServer::Servant p_servant);

  void deactivate_object (const PortableServer::ObjectId &oid);

  CORBA::Object_ptr create_reference (const char *intf);

#if !defined (CORBA_E_MICRO)
  CORBA::Object_ptr create_reference_with_id (
      const PortableServer::ObjectId &oid,
      const char *intf);
#endif

  PortableServer::ObjectId *servant_to_id (PortableServer::Servant p_servant);

  PortableServer::ObjectId *servant_to_user_id (PortableServer::Servant p_servant);

  CORBA::Object_ptr servant_to_reference (PortableServer::Servant p_servant);

  PortableServer::Servant reference_to_servant (CORBA::Object_ptr reference);

  PortableServer::ObjectId *reference_to_id (CORBA::Object_ptr reference);

  PortableServer::Servant id_to_servant (const PortableServer::ObjectId &oid);

  CORBA::Object_ptr id_to_reference (const PortableServer::ObjectId &oid);

  CORBA::OctetSeq *id (void);

  /// Accessor for POA policies.
  TAO_POA_Policy_Set &policies (void);

  /// Get the set policy of the given type.
  CORBA::Policy_ptr get_policy (CORBA::PolicyType policy);

  /// This method gives the policies that are exposed to the client.
  /// These policies are shipped within the IOR.
  virtual CORBA::PolicyList *client_exposed_policies (
      CORBA::Short object_priority);

  TAO_Root_POA (const String &name,
                PortableServer::POAManager_ptr poa_manager,
                const TAO_POA_Policy_Set &policies,
                TAO_Root_POA *parent,
                ACE_Lock &lock,
                TAO_SYNCH_MUTEX &thread_lock,
                TAO_ORB_Core &orb_core,
                TAO_Object_Adapter *object_adapter);

  virtual ~TAO_Root_POA (void);

  static char name_separator (void);

  static CORBA::ULong name_separator_length (void);

  enum
  {
    TAO_OBJECTKEY_PREFIX_SIZE = 4
  };

  static CORBA::Octet const objectkey_prefix[TAO_OBJECTKEY_PREFIX_SIZE];

  const TAO_Object_Adapter::poa_name &folded_name (void) const;

  const TAO_Object_Adapter::poa_name &system_name (void) const;

  static void check_for_valid_wait_for_completions (
      const TAO_ORB_Core &orb_core,
      CORBA::Boolean wait_for_completion);

  /// ORB Core for POA.
  TAO_ORB_Core &orb_core (void) const;

  /// obtain a reference to the cached_profiles
  TAO::Portable_Server::Cached_Policies& cached_policies (void);

  /// obtain a handle to the network priority hooks
  TAO_Network_Priority_Hook* network_priority_hook (void);

  TAO::Portable_Server::Cached_Policies::PriorityModel priority_model (void) const;

  CORBA::Boolean cleanup_in_progress (void);

  /// Calls protected static method used when POACurrent is not appropriate.
  static int parse_ir_object_key (const TAO::ObjectKey &object_key,
                                  PortableServer::ObjectId &user_id);

  TAO_Object_Adapter &object_adapter (void);

  ACE_Lock &lock (void);

  /// Create the correct stub, properly initialized with the
  /// attributes and policies attached to the current POA.
  TAO_Stub* key_to_stub (const TAO::ObjectKey &key,
                         const char *type_id,
                         CORBA::Short priority);

  /// Accessor methods to POA state.
  /**
   * The POA can be in one of HOLDING, ACTIVE, DISCARDING, INACTIVE
   * and NON_EXISTENT states.
   */
  PortableInterceptor::AdapterState get_adapter_state (void);

  virtual void *thread_pool (void) const;

  virtual CORBA::Policy *server_protocol (void);

  CORBA::ULong outstanding_requests (void) const;

  const ACE_CString &name (void) const;

  CORBA::Boolean waiting_destruction (void) const;

  static void ort_adapter_factory_name (const char *name);

  static const char *ort_adapter_factory_name (void);

  /// Sets the value of TAO_POA_Static_Resources::imr_client_adapter_name_.
  static void imr_client_adapter_name (const char *name);

  /// Gets the value of TAO_POA_Static_Resources::imr_client_adapter_name_.
  static const char *imr_client_adapter_name (void);

  CORBA::Object_ptr invoke_key_to_object (void);

  CORBA::Boolean system_id (void);

  CORBA::ULong waiting_servant_deactivation (void) const;

  /// Return the POA Manager related to this POA
  TAO_POA_Manager &tao_poa_manager ();

  bool is_poa_generated (CORBA::Object_ptr reference,
                         PortableServer::ObjectId &system_id);

  /*
   * Validate if the servant may be activated
   * @retval true This servant may be activated
   * @retval false This servant may not be activated
   */
  bool is_servant_activation_allowed (
    PortableServer::Servant servant,
    bool &wait_occurred_restart_call);

  int rebind_using_user_id_and_system_id (
    PortableServer::Servant servant,
    const PortableServer::ObjectId &user_id,
    const PortableServer::ObjectId &system_id,
    TAO::Portable_Server::Servant_Upcall &servant_upcall);

  CORBA::Boolean servant_has_remaining_activations (
    PortableServer::Servant servant);

  bool allow_implicit_activation (void) const;

  bool allow_multiple_activations (void) const;

  int is_servant_active (
    PortableServer::Servant servant,
    bool &wait_occurred_restart_call);

  void deactivate_object_i (const PortableServer::ObjectId &oid);

  CORBA::Boolean is_persistent (void) const;

  CORBA::Short server_priority (void) const;

  bool has_system_id (void) const;

  PortableServer::Servant find_servant (const PortableServer::ObjectId &system_id);

  TAO_SERVANT_LOCATION servant_present (
    const PortableServer::ObjectId &system_id,
    PortableServer::Servant &servant);

  PortableServer::Servant find_servant (
        const PortableServer::ObjectId &system_id,
        TAO::Portable_Server::Servant_Upcall &servant_upcall,
        TAO::Portable_Server::POA_Current_Impl &poa_current_impl);

  /**
   * Find the the servant with ObjectId <system_id>, and retrieve
   * its priority. Usually used in RT CORBA with SERVER_DECLARED
   * priority model.
   *
   * @return -1 if servant does not exist, else 0 indicating the
   * servant exists and priority successfully retrieved.
   */
  int find_servant_priority (
        const PortableServer::ObjectId &system_id,
        CORBA::Short &priority);

  int unbind_using_user_id (const PortableServer::ObjectId &user_id);

  void cleanup_servant (
    PortableServer::Servant servant,
    const PortableServer::ObjectId &user_id);

  void post_invoke_servant_cleanup(
    const PortableServer::ObjectId &system_id,
    const TAO::Portable_Server::Servant_Upcall &servant_upcall);

   bool validate_lifespan (
    CORBA::Boolean is_persistent,
    const TAO::Portable_Server::Temporary_Creation_Time& creation_time) const;

  PortableServer::ObjectId *activate_object_i (
      PortableServer::Servant p_servant,
      CORBA::Short priority,
      bool &wait_occurred_restart_call);

  CORBA::Object_ptr id_to_reference_i (const PortableServer::ObjectId &oid,
                                       bool indirect);

  PortableServer::ObjectId *servant_to_id_i (PortableServer::Servant servant);

  TAO_SYNCH_CONDITION &servant_deactivation_condition (void);

  int is_poa_generated_id (const PortableServer::ObjectId &id);

  /// Check the state of this POA
  void check_state (void);

  int delete_child (const String &child);

  PortableServer::Servant user_id_to_servant_i (const PortableServer::ObjectId &oid);

  virtual CORBA::ORB_ptr _get_orb (void);

  /// These hooks are needed by the CSD strategy to override
  /// and no-ops by default.

  /// Hook - The POA has been (or is being) activated.
  virtual void poa_activated_hook ();

  /// Hook - The POA has been deactivated.
  virtual void poa_deactivated_hook ();

  /// Hook - A servant has been activated.
  virtual void servant_activated_hook (PortableServer::Servant servant,
                               const PortableServer::ObjectId& oid);

  /// Hook - A servant has been deactivated.
  virtual void servant_deactivated_hook (PortableServer::Servant servant,
                                 const PortableServer::ObjectId& oid);

protected:

#if (TAO_HAS_MINIMUM_POA == 0)
  int enter (void);

  int exit (void);
#endif /* TAO_HAS_MINIMUM_POA == 0 */

#if ! defined (CORBA_E_MICRO)
  /// Template method for creating new POA's of this type.
  virtual TAO_Root_POA *new_POA (const String &name,
                            PortableServer::POAManager_ptr poa_manager,
                            const TAO_POA_Policy_Set &policies,
                            TAO_Root_POA *parent,
                            ACE_Lock &lock,
                            TAO_SYNCH_MUTEX &thread_lock,
                            TAO_ORB_Core &orb_core,
                            TAO_Object_Adapter *object_adapter);

  PortableServer::POA_ptr create_POA_i (
      const char *adapter_name,
      PortableServer::POAManager_ptr poa_manager,
      const CORBA::PolicyList &policies);

  PortableServer::POA_ptr create_POA_i (const String &adapter_name,
                                        PortableServer::POAManager_ptr poa_manager,
                                        const TAO_POA_Policy_Set &policies);

  TAO_Root_POA *find_POA_i (const ACE_CString &child_name,
                            CORBA::Boolean activate_it);
#endif

  void destroy_i (CORBA::Boolean etherealize_objects,
                  CORBA::Boolean wait_for_completion);

  void complete_destruction_i (void);

  PortableServer::POAList *the_children_i (void);

  /// This method returns the adapter_name as a sequence of strings of
  /// length one or more or just a fixed name depending on the Object
  /// Adapter. Added wrt to ORT Spec.
  PortableInterceptor::AdapterName *adapter_name_i (void);

  /// Method to notify the IOR Interceptors when there is a state
  /// changed not related to POAManager.
  void adapter_state_changed (
      const TAO::ORT_Array &array_obj_ref_template,
      PortableInterceptor::AdapterState state);

  /// Add the given tagged component to all profiles.
  void add_ior_component (TAO_MProfile & mprofile,
                          const IOP::TaggedComponent & component);

  /// Add the given tagged component to all profiles matching the given
  /// ProfileId.
  void add_ior_component_to_profile (TAO_MProfile & mprofile,
                                     const IOP::TaggedComponent & component,
                                     IOP::ProfileId profile_id);

  /// Wrapper for the ORB's key_to_object that will alter the object pointer
  /// if the ImplRepo is used.
  CORBA::Object_ptr key_to_object (const TAO::ObjectKey &key,
                                   const char *type_id,
                                   TAO_ServantBase *servant,
                                   CORBA::Boolean collocated,
                                   CORBA::Short priority,
                                   bool indirect);

  /// Like key_to_stub() but assume that the ORB is not shutting down.
  virtual TAO_Stub* key_to_stub_i (const TAO::ObjectKey &key,
                                   const char *type_id,
                                   CORBA::Short priority);

  TAO_Stub *create_stub_object (const TAO::ObjectKey &object_key,
                                const char *type_id,
                                CORBA::PolicyList *policy_list,
                                TAO_Acceptor_Filter *filter,
                                TAO_Acceptor_Registry &acceptor_registry);

  PortableServer::Servant get_servant_i (void);

protected:
  void activate_object_with_id_i (const PortableServer::ObjectId &id,
                                  PortableServer::Servant p_servant,
                                  CORBA::Short priority,
                                  bool &wait_occurred_restart_call);

  virtual void remove_from_parent_i (void);

  void deactivate_all_objects_i (CORBA::Boolean etherealize_objects);

  void deactivate_all_objects_i (CORBA::Boolean etherealize_objects,
                                 CORBA::Boolean wait_for_completion);

  void wait_for_completions (CORBA::Boolean wait_for_completion);

  CORBA::Object_ptr create_reference_i (const char *intf,
                                        CORBA::Short priority);

  CORBA::Object_ptr create_reference_with_id_i (
      const PortableServer::ObjectId &oid,
      const char *intf,
      CORBA::Short priority);

  PortableServer::Servant reference_to_servant_i (CORBA::Object_ptr reference);

  CORBA::Object_ptr servant_to_reference_i (PortableServer::Servant p_servant);

  PortableServer::Servant id_to_servant_i (const PortableServer::ObjectId &oid);

  /// Call the IORInterceptor::establish_components() method on all
  /// registered IORInterceptors.
  /**
   * This method calls IORInterceptor::establish_components() method
   * on all registered IORInterceptors, and
   * IORInterceptor::components_established() once the former is
   * completed.
   */
  void establish_components (void);

  /// Call the IORInterceptor::components_established() method on all
  /// registered IORInterceptors.
  void components_established (PortableInterceptor::IORInfo_ptr info);

  void set_folded_name (TAO_Root_POA *parent);

  void set_id (TAO_Root_POA *parent);

  TAO::ObjectKey *create_object_key (const PortableServer::ObjectId &id);

  static int parse_key (const TAO::ObjectKey &key,
                        TAO_Object_Adapter::poa_name &poa_system_name,
                        PortableServer::ObjectId &system_id,
                        CORBA::Boolean &is_root,
                        CORBA::Boolean &is_persistent,
                        CORBA::Boolean &is_system_id,
                        TAO::Portable_Server::Temporary_Creation_Time &poa_creation_time);

protected:
  /// Accessor methods to ObjectReferenceTemplate
  PortableInterceptor::ObjectReferenceTemplate *
    get_adapter_template (void);

  /// Accessor methods to ObjectReferenceTemplate, non locked version
  PortableInterceptor::ObjectReferenceTemplate *get_adapter_template_i (void);

  /// Accessor methods to PortableInterceptor::ObjectReferenceFactory
  PortableInterceptor::ObjectReferenceFactory *get_obj_ref_factory (void);

  /// Set the object reference factory
  void set_obj_ref_factory (
    PortableInterceptor::ObjectReferenceFactory *current_factory);


  TAO_SERVANT_LOCATION locate_servant_i (const PortableServer::ObjectId &id,
                                         PortableServer::Servant &servant);

  PortableServer::Servant locate_servant_i (
      const char *operation,
      const PortableServer::ObjectId &id,
      TAO::Portable_Server::Servant_Upcall &servant_upcall,
      TAO::Portable_Server::POA_Current_Impl &poa_current_impl,
      bool &wait_occurred_restart_call);

public:
  /// @todo Temporarily for servant retention
  CORBA::Object_ptr
  invoke_key_to_object_helper_i (const char * repository_id,
                                 const PortableServer::ObjectId & id);
protected:

  /// Get the ORT adapter, in case there is no adapter yet, this method will
  /// try to create one and hold the POA lock
  TAO::ORT_Adapter *ORT_adapter (void);

  /// Get the ORT adapter, in case there is no adapter yet, this method will
  /// try to create one but assumes the POA lock is already hold
  TAO::ORT_Adapter *ORT_adapter_i (void);

  TAO::ORT_Adapter_Factory *ORT_adapter_factory (void);

  CORBA::Boolean persistent (void);

  static char persistent_key_char (void);

  static char transient_key_char (void);

  static CORBA::ULong persistent_key_type_length (void);

  static char system_id_key_char (void);

  static char user_id_key_char (void);

  static CORBA::ULong system_id_key_type_length (void);

  virtual CORBA::Boolean root (void) const;

  virtual char root_key_type (void);

  static char root_key_char (void);

  static char non_root_key_char (void);

  static CORBA::ULong root_key_type_length (void);

  void outstanding_requests (CORBA::ULong new_outstanding_requests);

  CORBA::ULong increment_outstanding_requests (void);

  CORBA::ULong decrement_outstanding_requests (void);

  String name_;

  /// Reference to the POAManager that this poa assicuates with.
  TAO_POA_Manager &poa_manager_;

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)
  /// Reference to the POAManagerFactory that generate the POAManager.
  TAO_POAManager_Factory& poa_manager_factory_;
#endif

  IOP::TaggedComponentSeq tagged_component_;

  IOP::TaggedComponentSeq tagged_component_id_;

  ACE_Array_Base <IOP::ProfileId> profile_id_array_;

  TAO_POA_Policy_Set policies_;

  TAO_Object_Adapter::poa_name folded_name_;

  TAO_Object_Adapter::poa_name_var system_name_;

  CORBA::OctetSeq id_;

  /// Pointer to the object reference template adapter.
  TAO::ORT_Adapter *ort_adapter_;

  /// Adapter can be accepting, rejecting etc.
  PortableInterceptor::AdapterState adapter_state_;

  TAO::Portable_Server::Cached_Policies cached_policies_;

  TAO_Network_Priority_Hook *network_priority_hook_;

  TAO::Portable_Server::Active_Policy_Strategies active_policy_strategies_;

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)
  PortableServer::AdapterActivator_var adapter_activator_;
#endif /* TAO_HAS_MINIMUM_POA == 0 !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO) */

  typedef
    ACE_Hash_Map_Manager_Ex<
        ACE_CString, TAO_Root_POA *,
        ACE_Hash<ACE_CString>,
        ACE_Equal_To<ACE_CString>,
        ACE_Null_Mutex
      >
    CHILDREN;

  CHILDREN children_;

  ACE_Lock &lock_;

  TAO_ORB_Core &orb_core_;

  /// The object adapter we belong to
  TAO_Object_Adapter *object_adapter_;

  CORBA::Boolean cleanup_in_progress_;

  CORBA::ULong outstanding_requests_;

  TAO_SYNCH_CONDITION outstanding_requests_condition_;

  CORBA::Boolean wait_for_completion_pending_;

  CORBA::Boolean waiting_destruction_;

  TAO_SYNCH_CONDITION servant_deactivation_condition_;

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)
  TAO_Acceptor_Filter_Factory * filter_factory_;
#endif

public:
    // @todo Temporarily for servant retention
  CORBA::ULong caller_key_to_object_;

  PortableServer::Servant servant_for_key_to_object_;

  struct Key_To_Object_Params
  {
    PortableServer::ObjectId_var *system_id_;
    const char *type_id_;
    TAO_ServantBase *servant_;
    CORBA::Boolean collocated_;
    CORBA::Short priority_;
    bool indirect_;

    void set (PortableServer::ObjectId_var &system_id_,
              const char *type_id_,
              TAO_ServantBase *servant_,
              CORBA::Boolean collocated_,
              CORBA::Short priority_,
              bool indirect);
  };

  Key_To_Object_Params key_to_object_params_;
};

/**
 * @class TAO_POA_Static_Resources
 *
 * @brief The static (global) resoures of all POA's.
 *
 * This class is used by the POA to store the resources global to
 * all POA's.  All instance variables that would have been
 * declared "static" in TAO_Regular_POA, should be declared in this class
 * to avoid the "static initialization order fiasco" as described in
 * http://www.parashift.com/c++-faq-lite/ctors.html#faq-10.11.
 * Briefly, this is the problem that occurs if any static initializers
 * in any other code call into set static members of TAO_Regular_POA.
 * Since the order in which these initializers execute is unspecified,
 * uninitialized members can be accessed.
 */
class TAO_PortableServer_Export TAO_POA_Static_Resources
{
public:

  /// Return the singleton instance.
  static TAO_POA_Static_Resources* instance (void);

  /// Cleanup the static singleton
  static void fini (void) ACE_GCC_DESTRUCTOR_ATTRIBUTE;

public:

  /**
   * Name of the factory object used to adapt function calls on the
   * PortableInterceptor interfaces ORT. The default value is
   * "ObjectReferenceTemplate_Adapter_Factory". If the ORT library is linked,
   * the corresponding accessor function
   * objectreferencefactory_adapter_factory_name() will be called to set
   * the value to "Concrete_ObjectReferenceTemplate_Adapter_Factory".
   */
  ACE_CString ort_adapter_factory_name_;

  /**
   * Name of the service object for functions that make calls on
   * the Implementation Repository. The default value is "ImR_Client_Adapter".
   * If TAO_IMR_CLient is linked, imr_client_adapter_name() will be
   * called to set the value to "Concrete_ImR_Client_Adapter".
   */
  ACE_CString imr_client_adapter_name_;
private:
  /// Constructor.
  TAO_POA_Static_Resources (void);

private:
  /// The singleton instance.
  static TAO_POA_Static_Resources* instance_;

  /// Mostly unused variable whose sole purpose is to enforce
  /// the instantiation of a TAO_POA_Static_Resources instance
  /// at initialization time.
  static TAO_POA_Static_Resources* initialization_reference_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

// ****************************************************************

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#if defined (__ACE_INLINE__)
# include "tao/PortableServer/Root_POA.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_ROOT_POA_H */
