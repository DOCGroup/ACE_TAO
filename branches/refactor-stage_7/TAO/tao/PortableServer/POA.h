// -*- C++ -*-

//=============================================================================
/**
 *  @file    POA.h
 *
 *  $Id$
 *
 *  Header file for CORBA's ORB type.
 *
 *  @author  Irfan Pyarali <irfan@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_POA_H
#define TAO_POA_H

#include /**/ "ace/pre.h"

#include "ace/SString.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// Object Adapter
#include "Object_Adapter.h"

// POAManager
#include "POAManager.h"

// POA Policy Set
#include "POA_Policy_Set.h"

// Cached POA Policies
#include "POA_Cached_Policies.h"

// Object_Key
#include "tao/Object_KeyC.h"

// Local Object
#include "tao/LocalObject.h"

// Portable Interceptor
#include "tao/PortableInterceptorC.h"

// Map
#include "ace/Hash_Map_Manager_T.h"

// ACE_Array_Base
#include "ace/Array_Base.h"

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
class TAO_ObjectReferenceTemplate;

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

  /// Timestamp
  CORBA::ULong time_stamp_[2];

};

//
/**
 * @class TAO_Temporary_Creation_Time
 *
 * @brief Special creation time only useful in the lifetime of the
 *        upcall.
 *
 * Special creation time only useful in the lifetime of the upcall.
 */
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

namespace PortableInterceptor
{
  class IORInfo;
  typedef IORInfo *IORInfo_ptr;
};

/**
 * @class TAO_POA
 *
 * @brief Implementation of the PortableServer::POA interface.
 *
 * Implementation of the PortableServer::POA interface.
 */
class TAO_PortableServer_Export TAO_POA
  : public virtual PortableServer::POA,
    public virtual TAO_Local_RefCounted_Object
{
public:

  friend class TAO_Object_Adapter;
  friend class TAO_Object_Adapter::Servant_Upcall;
  friend class TAO_Object_Adapter::Non_Servant_Upcall;
  friend class TAO_POA_Current_Impl;
  friend class TAO_POA_Manager;
  friend class TAO_RT_Collocation_Resolver;
  friend class TAO_ObjectReferenceTemplate;

  typedef ACE_CString String;

  /**
   * This method is used to downcast safely an instance of
   * PortableServer::POA to an instance of TAO_POA when RTTI is not
   * enabled.
   */
  virtual TAO_POA* _tao_poa_downcast (void);

  PortableServer::POA_ptr create_POA (
      const char *adapter_name,
      PortableServer::POAManager_ptr poa_manager,
      const CORBA::PolicyList &policies
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::AdapterAlreadyExists,
                     PortableServer::POA::InvalidPolicy));

  PortableServer::POA_ptr find_POA (const char *adapter_name,
                                    CORBA::Boolean activate_it
                                    ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::AdapterNonExistent));

  void destroy (CORBA::Boolean etherealize_objects,
                CORBA::Boolean wait_for_completion
                ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

#if (TAO_HAS_MINIMUM_POA == 0)

  PortableServer::ThreadPolicy_ptr create_thread_policy (
      PortableServer::ThreadPolicyValue value
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

#endif /* TAO_HAS_MINIMUM_POA == 0 */

  PortableServer::LifespanPolicy_ptr create_lifespan_policy (
      PortableServer::LifespanPolicyValue value
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  PortableServer::IdUniquenessPolicy_ptr create_id_uniqueness_policy (
      PortableServer::IdUniquenessPolicyValue value
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  PortableServer::IdAssignmentPolicy_ptr create_id_assignment_policy (
      PortableServer::IdAssignmentPolicyValue value
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

#if (TAO_HAS_MINIMUM_POA == 0)

  PortableServer::ImplicitActivationPolicy_ptr 
  create_implicit_activation_policy (
      PortableServer::ImplicitActivationPolicyValue value
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  PortableServer::ServantRetentionPolicy_ptr 
  create_servant_retention_policy (
      PortableServer::ServantRetentionPolicyValue value
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  PortableServer::RequestProcessingPolicy_ptr 
  create_request_processing_policy (
      PortableServer::RequestProcessingPolicyValue value
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

#endif /* TAO_HAS_MINIMUM_POA == 0 */

  char * the_name (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  PortableServer::POA_ptr the_parent (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  PortableServer::POAList *the_children (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  PortableServer::POAManager_ptr the_POAManager (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  PortableInterceptor::AdapterManagerId get_manager_id (
      ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// This method returns the adapter_name as a sequence of strings of
  /// length one or more or just a fixed name depending on the Object
  /// Adapter. Added wrt to ORT Spec.
  PortableInterceptor::AdapterName *adapter_name (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Accessor methods to ObjectReferenceTemplate
  PortableInterceptor::ObjectReferenceTemplate * get_adapter_template (void);

  void set_adapter_template (PortableInterceptor::ObjectReferenceTemplate *
                             object_ref_template
                             ACE_ENV_ARG_DECL);

  /// Accessor methods to PortableInterceptor::ObjectReferenceFactory
  PortableInterceptor::ObjectReferenceFactory * get_obj_ref_factory (void);

  void set_obj_ref_factory (
    PortableInterceptor::ObjectReferenceFactory *current_factory
    ACE_ENV_ARG_DECL);

  /// Store the given TaggedComponent for eventual insertion into all
  /// object reference profiles.
  void save_ior_component (const IOP::TaggedComponent &component
                           ACE_ENV_ARG_DECL);

  /// Store the given TaggedComponent for eventual insertion into all
  /// object reference profiles with the given ProfileId.
  void save_ior_component_and_profile_id (
    const IOP::TaggedComponent &component,
    IOP::ProfileId profile_id
    ACE_ENV_ARG_DECL);

#if (TAO_HAS_MINIMUM_POA == 0)

  PortableServer::AdapterActivator_ptr the_activator (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void the_activator (PortableServer::AdapterActivator_ptr adapter_activator
                      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  PortableServer::ServantManager_ptr get_servant_manager (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::WrongPolicy));

  void set_servant_manager (PortableServer::ServantManager_ptr imgr
                            ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::WrongPolicy));

  PortableServer::Servant get_servant (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::NoServant,
                     PortableServer::POA::WrongPolicy));

  void set_servant (PortableServer::Servant servant
                    ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::WrongPolicy));

#endif /* TAO_HAS_MINIMUM_POA == 0 */

  PortableServer::ObjectId *activate_object (PortableServer::Servant p_servant
                                             ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::ServantAlreadyActive,
                     PortableServer::POA::WrongPolicy));

  void activate_object_with_id (const PortableServer::ObjectId &id,
                                PortableServer::Servant p_servant
                                ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::ServantAlreadyActive,
                     PortableServer::POA::ObjectAlreadyActive,
                     PortableServer::POA::WrongPolicy));

  void deactivate_object (const PortableServer::ObjectId &oid
                          ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::ObjectNotActive,
                     PortableServer::POA::WrongPolicy));

  CORBA::Object_ptr create_reference (const char *intf
                                      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::WrongPolicy));

  CORBA::Object_ptr create_reference_with_id (
      const PortableServer::ObjectId &oid,
      const char *intf
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::WrongPolicy));

  PortableServer::ObjectId *servant_to_id (PortableServer::Servant p_servant
                                           ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::ServantNotActive,
                     PortableServer::POA::WrongPolicy));

  CORBA::Object_ptr servant_to_reference (PortableServer::Servant p_servant
                                          ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::ServantNotActive,
                     PortableServer::POA::WrongPolicy));

  PortableServer::Servant reference_to_servant (
      CORBA::Object_ptr reference
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::ObjectNotActive,
                     PortableServer::POA::WrongAdapter,
                     PortableServer::POA::WrongPolicy));

  PortableServer::ObjectId *reference_to_id (CORBA::Object_ptr reference
                                             ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::WrongAdapter,
                     PortableServer::POA::WrongPolicy));

  PortableServer::Servant id_to_servant (const PortableServer::ObjectId &oid
                                         ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::ObjectNotActive,
                     PortableServer::POA::WrongPolicy));

  CORBA::Object_ptr id_to_reference (const PortableServer::ObjectId &oid
                                     ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::ObjectNotActive,
                     PortableServer::POA::WrongPolicy));

  CORBA::OctetSeq *id (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

#if (TAO_HAS_MINIMUM_POA == 0)
  // Methods added by the MIOP specification.

  virtual PortableServer::ObjectId * create_id_for_reference (
      CORBA::Object_ptr the_ref
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      PortableServer::NotAGroupObject
    ));


  virtual PortableServer::IDs * reference_to_ids (
      CORBA::Object_ptr the_ref
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      PortableServer::NotAGroupObject
    ));

  virtual void associate_reference_with_id (
      CORBA::Object_ptr ref,
      const PortableServer::ObjectId & oid
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      PortableServer::NotAGroupObject
    ));

  virtual void disassociate_reference_with_id (
      CORBA::Object_ptr ref,
      const PortableServer::ObjectId & oid
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
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

  virtual CORBA::PolicyList *client_exposed_policies (
      CORBA::Short object_priority
      ACE_ENV_ARG_DECL
    );
  // This method gives the policies that are exposed to the client.
  // These policies are shipped within the IOR.


  // Utility functions for the other
  static char* ObjectId_to_string (const PortableServer::ObjectId &id);

  static CORBA::WChar* ObjectId_to_wstring (
      const PortableServer::ObjectId &id
    );

  static PortableServer::ObjectId *string_to_ObjectId (const char *id);

  static PortableServer::ObjectId *string_to_ObjectId (const char *string,
                                                       int size);

  static PortableServer::ObjectId *wstring_to_ObjectId (
      const CORBA::WChar *id
    );

  TAO_POA (const String &name,
           TAO_POA_Manager &poa_manager,
           const TAO_POA_Policy_Set &policies,
           TAO_POA *parent,
           ACE_Lock &lock,
           TAO_SYNCH_MUTEX &thread_lock,
           TAO_ORB_Core &orb_core,
           TAO_Object_Adapter *object_adapter
           ACE_ENV_ARG_DECL);

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

  static void check_for_valid_wait_for_completions (
      const TAO_ORB_Core &orb_core,
      CORBA::Boolean wait_for_completion
      ACE_ENV_ARG_DECL
    );

  TAO_ORB_Core &orb_core (void) const;
  // ORB Core for POA.

  CORBA::Boolean cleanup_in_progress (void);

  static int parse_ir_object_key (const TAO::ObjectKey &object_key,
                                  PortableServer::ObjectId &user_id);
  // Calls protected static method used when POACurrent is not appropriate.

  TAO_Object_Adapter &object_adapter (void);

  ACE_Lock &lock (void);

  /// Create the correct stub, properly initialized with the
  /// attributes and policies attached to the current POA.
  TAO_Stub* key_to_stub (const TAO::ObjectKey &key,
                         const char *type_id,
                         CORBA::Short priority
                         ACE_ENV_ARG_DECL);


  /// Accessor for the current thread policy of this POA.
  PortableServer::ThreadPolicyValue thread_policy (void) const;

  ///Accessor methods to POA state.
  /**
   * The POA can be in one of HOLDING, ACTIVE, DISCARDING, INACTIVE
   * and NON_EXISTENT states.
   */
  PortableInterceptor::AdapterState get_adapter_state (
    ACE_ENV_SINGLE_ARG_DECL);

  virtual void *thread_pool (void) const;

  virtual CORBA::Policy *server_protocol (void);

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
                            ACE_ENV_ARG_DECL);

  PortableServer::POA_ptr create_POA_i (
      const char *adapter_name,
      PortableServer::POAManager_ptr poa_manager,
      const CORBA::PolicyList &policies
      ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::AdapterAlreadyExists,
                     PortableServer::POA::InvalidPolicy));

  TAO_POA *create_POA_i (const String &adapter_name,
                         TAO_POA_Manager &poa_manager,
                         const TAO_POA_Policy_Set &policies
                         ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::AdapterAlreadyExists,
                     PortableServer::POA::InvalidPolicy));

  TAO_POA *find_POA_i (const ACE_CString &child_name,
                       CORBA::Boolean activate_it
                       ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::AdapterNonExistent));

  void destroy_i (CORBA::Boolean etherealize_objects,
                  CORBA::Boolean wait_for_completion
                  ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void complete_destruction_i (ACE_ENV_SINGLE_ARG_DECL);

  PortableServer::POAList *the_children_i (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// This method returns the adapter_name as a sequence of strings of
  /// length one or more or just a fixed name depending on the Object
  /// Adapter. Added wrt to ORT Spec.
  PortableInterceptor::AdapterName *adapter_name_i (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Method to notify the IOR Interceptors when there is a state
  /// changed not related to POAManager.
  void adapter_state_changed (
      const PortableInterceptor::ObjectReferenceTemplateSeq &seq_obj_ref_template,
      PortableInterceptor::AdapterState state
      ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Add the given tagged component to all profiles.
  void add_ior_component (TAO_MProfile & mprofile,
                          const IOP::TaggedComponent & component
                          ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Add the given tagged component to all profiles matching the given
  /// ProfileId.
  void add_ior_component_to_profile (TAO_MProfile & mprofile,
                                     const IOP::TaggedComponent & component,
                                     IOP::ProfileId profile_id
                                     ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

#if (TAO_HAS_MINIMUM_POA == 0)

  PortableServer::ServantManager_ptr get_servant_manager_i (
      ACE_ENV_SINGLE_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::WrongPolicy));

  void set_servant_manager_i (PortableServer::ServantManager_ptr imgr
                              ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::WrongPolicy));

  PortableServer::Servant get_servant_i (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::NoServant,
                     PortableServer::POA::WrongPolicy));

  void set_servant_i (PortableServer::Servant servant
                      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::WrongPolicy));

#endif /* TAO_HAS_MINIMUM_POA == 0 */

//
// ImplRepo related.
//
#if (TAO_HAS_MINIMUM_CORBA == 0)

  void imr_notify_startup (ACE_ENV_SINGLE_ARG_DECL);
  // ImplRepo helper method, notify the ImplRepo on startup

  void imr_notify_shutdown (void);
  // ImplRepo helper method, notify the ImplRepo on shutdown

#endif /* TAO_HAS_MINIMUM_CORBA */

  CORBA::Object_ptr invoke_key_to_object (ACE_ENV_SINGLE_ARG_DECL);

  CORBA::Object_ptr key_to_object (const TAO::ObjectKey &key,
                                   const char *type_id,
                                   TAO_ServantBase *servant,
                                   CORBA::Boolean collocated,
                                   CORBA::Short priority
                                   ACE_ENV_ARG_DECL);
  // Wrapper for the ORB's key_to_object that will alter the object pointer
  // if the ImplRepo is used.

  virtual TAO_Stub* key_to_stub_i (const TAO::ObjectKey &key,
                                   const char *type_id,
                                   CORBA::Short priority
                                   ACE_ENV_ARG_DECL);
  // Like key_to_stub() but assume that the ORB is not shutting down.

  TAO_Stub *create_stub_object (const TAO::ObjectKey &object_key,
                                const char *type_id,
                                CORBA::PolicyList *policy_list,
                                TAO_Acceptor_Filter *filter,
                                TAO_Acceptor_Registry &acceptor_registry
                                ACE_ENV_ARG_DECL);

  int is_servant_in_map (PortableServer::Servant servant,
                         int &wait_occurred_restart_call);

  int is_user_id_in_map (const PortableServer::ObjectId &user_id,
                         CORBA::Short priority,
                         int &priorities_match,
                         int &wait_occurred_restart_call);

  PortableServer::ObjectId *activate_object_i (
      PortableServer::Servant p_servant,
      CORBA::Short priority,
      int &wait_occurred_restart_call
      ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::ServantAlreadyActive,
                     PortableServer::POA::WrongPolicy));

  void activate_object_with_id_i (const PortableServer::ObjectId &id,
                                  PortableServer::Servant p_servant,
                                  CORBA::Short priority,
                                  int &wait_occurred_restart_call
                                  ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::ServantAlreadyActive,
                     PortableServer::POA::ObjectAlreadyActive,
                     PortableServer::POA::WrongPolicy));

  void deactivate_all_objects_i (CORBA::Boolean etherealize_objects
                                 ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::WrongPolicy));

  void deactivate_all_objects_i (CORBA::Boolean etherealize_objects,
                                 CORBA::Boolean wait_for_completion
                                 ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::WrongPolicy));

  void wait_for_completions (CORBA::Boolean wait_for_completion
                             ACE_ENV_ARG_DECL);

  void check_poa_manager_state (ACE_ENV_SINGLE_ARG_DECL);

  void deactivate_object_i (const PortableServer::ObjectId &oid
                            ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::ObjectNotActive,
                   PortableServer::POA::WrongPolicy));

  void cleanup_servant (
      TAO_Active_Object_Map::Map_Entry *active_object_map_entry
      ACE_ENV_ARG_DECL
    );

  void deactivate_map_entry (
      TAO_Active_Object_Map::Map_Entry *active_object_map_entry
      ACE_ENV_ARG_DECL
    );

  CORBA::Object_ptr create_reference_i (const char *intf,
                                        CORBA::Short priority
                                        ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::WrongPolicy));

  CORBA::Object_ptr create_reference_with_id_i (
      const PortableServer::ObjectId &oid,
      const char *intf,
      CORBA::Short priority
      ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::WrongPolicy));

  PortableServer::ObjectId *servant_to_id_i (PortableServer::Servant servant
                                             ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::ServantNotActive,
                     PortableServer::POA::WrongPolicy));

  PortableServer::ObjectId *servant_to_system_id (
      PortableServer::Servant p_servant,
      CORBA::Short &priority
      ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::ServantNotActive,
                     PortableServer::POA::WrongPolicy));

  PortableServer::ObjectId *servant_to_system_id_i (
      PortableServer::Servant p_servant,
      CORBA::Short &priority
      ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::ServantNotActive,
                     PortableServer::POA::WrongPolicy));

  PortableServer::Servant reference_to_servant_i (
      CORBA::Object_ptr reference
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::ObjectNotActive,
                     PortableServer::POA::WrongAdapter,
                     PortableServer::POA::WrongPolicy));

  PortableServer::Servant id_to_servant_i (const PortableServer::ObjectId &oid
                                           ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::ObjectNotActive,
                     PortableServer::POA::WrongPolicy));

  CORBA::Object_ptr id_to_reference_i (const PortableServer::ObjectId &oid
                                       ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::ObjectNotActive,
                     PortableServer::POA::WrongPolicy));

  TAO_Active_Object_Map &active_object_map (void) const;

  /// Call the IORInterceptor::establish_components() method on all
  /// registered IORInterceptors.
  /**
   * This method calls IORInterceptor::establish_components() method
   * on all registered IORInterceptors, and
   * IORInterceptor::components_established() once the former is
   * completed.
   */
  void establish_components (ACE_ENV_SINGLE_ARG_DECL);

  /// Call the IORInterceptor::components_established() method on all
  /// registered IORInterceptors.
  void components_established (PortableInterceptor::IORInfo_ptr info
                               ACE_ENV_ARG_DECL);

  int delete_child (const String &child);

  void set_folded_name (void);

  void set_id (void);

  TAO::ObjectKey *create_object_key (const PortableServer::ObjectId &id);

  int is_poa_generated_id (const PortableServer::ObjectId &id);

  static int parse_key (const TAO::ObjectKey &key,
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
                                         ACE_ENV_ARG_DECL);

  PortableServer::Servant locate_servant_i (
      const char *operation,
      const PortableServer::ObjectId &id,
      TAO_Object_Adapter::Servant_Upcall &servant_upcall,
      TAO_POA_Current_Impl &poa_current_impl,
      int &wait_occurred_restart_call
      ACE_ENV_ARG_DECL
    );

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

  IOP::TaggedComponentList tagged_component_;

  IOP::TaggedComponentList tagged_component_id_;

  ACE_Array_Base <IOP::ProfileId> profile_id_array_;

  TAO_POA_Policy_Set policies_;

  TAO_POA *parent_;

  TAO_Active_Object_Map *active_object_map_;

  TAO_Object_Adapter::poa_name folded_name_;

  TAO_Object_Adapter::poa_name_var system_name_;

  CORBA::OctetSeq id_;

  /// Keep a copy of the pointer to the actual implementation around
  /// so that we can call some TAO-specific methods on it.
  TAO_ObjectReferenceTemplate * def_ort_template_;

  PortableInterceptor::ObjectReferenceTemplate_var ort_template_;

  PortableInterceptor::ObjectReferenceFactory_var obj_ref_factory_;

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

  /// Implementation Repository Server Object
  ServerObject_i *server_object_;

  /// Flag for whether the ImR should be used or not.
  int use_imr_;

#endif /* TAO_HAS_MINIMUM_CORBA */

  typedef 
    ACE_Hash_Map_Manager_Ex<
        ACE_CString, TAO_POA *, 
        ACE_Hash<ACE_CString>, 
        ACE_Equal_To<ACE_CString>, 
        ACE_Null_Mutex
      >
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

  struct Key_To_Object_Params
  {
    PortableServer::ObjectId_var *system_id_;
    const char *type_id_;
    TAO_ServantBase *servant_;
    CORBA::Boolean collocated_;
    CORBA::Short priority_;

    void set (PortableServer::ObjectId_var &system_id_,
              const char *type_id_,
              TAO_ServantBase *servant_,
              CORBA::Boolean collocated_,
              CORBA::Short priority_);
  };

  Key_To_Object_Params key_to_object_params_;
};


/**
 * @class TAO_POA_Guard
 *
 * @brief TAO_POA_Guard
 *
 * TAO_POA_Guard
 */
class TAO_PortableServer_Export TAO_POA_Guard
{
public:
  TAO_POA_Guard (TAO_POA &poa
                 ACE_ENV_ARG_DECL,
                 int check_for_destruction = 1);

private:
  ACE_Guard<ACE_Lock> guard_;
};

#if (TAO_HAS_MINIMUM_POA == 0)

class TAO_PortableServer_Export TAO_Adapter_Activator 
  : public PortableServer::AdapterActivator
{
public:

  TAO_Adapter_Activator (PortableServer::POAManager_ptr poa_manager);

  CORBA::Boolean unknown_adapter (PortableServer::POA_ptr parent,
                                  const char *name
                                  ACE_ENV_ARG_DECL)
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

#include /**/ "ace/post.h"

#endif /* TAO_POA_H */
