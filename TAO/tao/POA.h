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

// String
#include "ace/SString.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// Map
#include "ace/Hash_Map_Manager.h"

// Vector
#include "ace/Containers.h"

// Locking
#include "ace/Synch.h"

// Stubs
#include "tao/POAC.h"

// Servant
#include "tao/Servant_Base.h"

// Skeletons
#include "tao/POAS.h"

// Object_Key
#include "tao/Object_KeyC.h"

// Active Object Table
#include "tao/Active_Object_Map.h"

// POA Manager
#include "tao/POAManager.h"

// Object Adapter
#include "tao/Object_Adapter.h"

// This is to remove "inherits via dominance" warnings from MSVC.
// MSVC is being a little too paranoid.
#if defined (_MSC_VER)
# pragma warning (disable : 4250)
#endif /* _MSC_VER */

class TAO_POA;

#if !defined (TAO_HAS_MINIMUM_CORBA)

class TAO_Export TAO_Thread_Policy : public virtual PortableServer::RefCountServantBase,
                                     public virtual POA_PortableServer::ThreadPolicy
{
public:
  TAO_Thread_Policy (PortableServer::ThreadPolicyValue value,
                     PortableServer::POA_ptr poa);

  TAO_Thread_Policy (const TAO_Thread_Policy &new_policy);

  PortableServer::ThreadPolicyValue value (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  CORBA::Policy_ptr copy (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  void destroy (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  CORBA::PolicyType policy_type (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  PortableServer::POA_ptr _default_POA (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

protected:
  PortableServer::ThreadPolicyValue value_;

  PortableServer::POA_var poa_;
};

#endif /* TAO_HAS_MINIMUM_CORBA */

class TAO_Export TAO_Lifespan_Policy : public virtual PortableServer::RefCountServantBase,
                                       public virtual POA_PortableServer::LifespanPolicy
{
public:
  TAO_Lifespan_Policy (PortableServer::LifespanPolicyValue value,
                       PortableServer::POA_ptr poa);

  TAO_Lifespan_Policy (const TAO_Lifespan_Policy &rhs);

  PortableServer::LifespanPolicyValue value (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  CORBA::Policy_ptr copy (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  void destroy (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  CORBA::PolicyType policy_type (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  PortableServer::POA_ptr _default_POA (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

protected:
  PortableServer::LifespanPolicyValue value_;

  PortableServer::POA_var poa_;
};

class TAO_Export TAO_Id_Uniqueness_Policy : public virtual PortableServer::RefCountServantBase,
                                            public virtual POA_PortableServer::IdUniquenessPolicy
{
public:
  TAO_Id_Uniqueness_Policy (PortableServer::IdUniquenessPolicyValue value,
                            PortableServer::POA_ptr poa);

  TAO_Id_Uniqueness_Policy (const TAO_Id_Uniqueness_Policy &rhs);

  PortableServer::IdUniquenessPolicyValue value (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  CORBA::Policy_ptr copy (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  void destroy (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  CORBA::PolicyType policy_type (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  PortableServer::POA_ptr _default_POA (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

protected:
  PortableServer::IdUniquenessPolicyValue value_;

  PortableServer::POA_var poa_;
};

class TAO_Export TAO_Id_Assignment_Policy : public virtual PortableServer::RefCountServantBase,
                                            public virtual POA_PortableServer::IdAssignmentPolicy
{
public:
  TAO_Id_Assignment_Policy (PortableServer::IdAssignmentPolicyValue value,
                            PortableServer::POA_ptr poa);

  TAO_Id_Assignment_Policy (const TAO_Id_Assignment_Policy &rhs);

  PortableServer::IdAssignmentPolicyValue value (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  CORBA::Policy_ptr copy (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  void destroy (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  CORBA::PolicyType policy_type (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  PortableServer::POA_ptr _default_POA (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

protected:
  PortableServer::IdAssignmentPolicyValue value_;

  PortableServer::POA_var poa_;
};

#if !defined (TAO_HAS_MINIMUM_CORBA)

class TAO_Export TAO_Implicit_Activation_Policy : public virtual PortableServer::RefCountServantBase,
                                                  public virtual POA_PortableServer::ImplicitActivationPolicy
{
public:
  TAO_Implicit_Activation_Policy (PortableServer::ImplicitActivationPolicyValue value,
                                  PortableServer::POA_ptr poa);

  TAO_Implicit_Activation_Policy (const TAO_Implicit_Activation_Policy &rhs);

  PortableServer::ImplicitActivationPolicyValue value (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  CORBA::Policy_ptr copy (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  void destroy (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  CORBA::PolicyType policy_type (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  PortableServer::POA_ptr _default_POA (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

protected:
  PortableServer::ImplicitActivationPolicyValue value_;

  PortableServer::POA_var poa_;
};

class TAO_Export TAO_Servant_Retention_Policy : public virtual PortableServer::RefCountServantBase,
                                                public virtual POA_PortableServer::ServantRetentionPolicy
{
public:
  TAO_Servant_Retention_Policy (PortableServer::ServantRetentionPolicyValue value,
                                PortableServer::POA_ptr poa);

  TAO_Servant_Retention_Policy (const TAO_Servant_Retention_Policy &rhs);

  PortableServer::ServantRetentionPolicyValue value (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  CORBA::Policy_ptr copy (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  void destroy (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  CORBA::PolicyType policy_type (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  PortableServer::POA_ptr _default_POA (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

protected:
  PortableServer::ServantRetentionPolicyValue value_;

  PortableServer::POA_var poa_;
};

class TAO_Export TAO_Request_Processing_Policy : public virtual PortableServer::RefCountServantBase,
                                                 public virtual POA_PortableServer::RequestProcessingPolicy
{
public:
  TAO_Request_Processing_Policy (PortableServer::RequestProcessingPolicyValue value,
                                 PortableServer::POA_ptr poa);

  TAO_Request_Processing_Policy (const TAO_Request_Processing_Policy &rhs);

  PortableServer::RequestProcessingPolicyValue value (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  CORBA::Policy_ptr copy (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  void destroy (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  CORBA::PolicyType policy_type (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  PortableServer::POA_ptr _default_POA (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

protected:
  PortableServer::RequestProcessingPolicyValue value_;

  PortableServer::POA_var poa_;
};

#endif /* TAO_HAS_MINIMUM_CORBA */

class TAO_Export TAO_POA_Policies
{
public:

  TAO_POA_Policies (void);

#if !defined (TAO_HAS_MINIMUM_CORBA)

  PortableServer::ThreadPolicyValue thread (void) const;
  void thread (PortableServer::ThreadPolicyValue value);

#endif /* TAO_HAS_MINIMUM_CORBA */

  PortableServer::LifespanPolicyValue lifespan (void) const;
  void lifespan (PortableServer::LifespanPolicyValue value);

  PortableServer::IdUniquenessPolicyValue id_uniqueness (void) const;
  void id_uniqueness (PortableServer::IdUniquenessPolicyValue value);

  PortableServer::IdAssignmentPolicyValue id_assignment (void) const;
  void id_assignment (PortableServer::IdAssignmentPolicyValue value);

  PortableServer::ImplicitActivationPolicyValue implicit_activation (void) const;
  void implicit_activation (PortableServer::ImplicitActivationPolicyValue value);

  PortableServer::ServantRetentionPolicyValue servant_retention (void) const;
  void servant_retention (PortableServer::ServantRetentionPolicyValue value);

  PortableServer::RequestProcessingPolicyValue request_processing (void) const;
  void request_processing (PortableServer::RequestProcessingPolicyValue value);

  void parse_policies (const CORBA::PolicyList &policies,
                       CORBA_Environment &ACE_TRY_ENV);

protected:

  void parse_policy (const CORBA::Policy_ptr policy,
                     CORBA_Environment &ACE_TRY_ENV);

  int validity_check (void);

  PortableServer::ThreadPolicyValue thread_;

  PortableServer::LifespanPolicyValue lifespan_;

  PortableServer::IdUniquenessPolicyValue id_uniqueness_;

  PortableServer::IdAssignmentPolicyValue id_assignment_;

  PortableServer::ImplicitActivationPolicyValue implicit_activation_;

  PortableServer::ServantRetentionPolicyValue servant_retention_;

  PortableServer::RequestProcessingPolicyValue request_processing_;
};

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

class TAO_POA_Current;

class TAO_Export TAO_POA : public POA_PortableServer::POA
{
public:

  friend class TAO_Object_Adapter;
  friend class TAO_Object_Adapter::Outstanding_Requests;
  friend class TAO_Object_Adapter::Single_Threaded_POA_Lock;
  friend class TAO_POA_Current;
  friend class TAO_POA_Manager;

  typedef ACE_CString String;

  PortableServer::POA_ptr create_POA (const char *adapter_name,
                                      PortableServer::POAManager_ptr poa_manager,
                                      const CORBA::PolicyList &policies,
                                      CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  PortableServer::POA_ptr find_POA (const char *adapter_name,
                                    CORBA::Boolean activate_it,
                                    CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  void destroy (CORBA::Boolean etherealize_objects,
                CORBA::Boolean wait_for_completion,
                CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

#if !defined (TAO_HAS_MINIMUM_CORBA)

  PortableServer::ThreadPolicy_ptr create_thread_policy (PortableServer::ThreadPolicyValue value,
                                                         CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

#endif /* TAO_HAS_MINIMUM_CORBA */

  PortableServer::LifespanPolicy_ptr create_lifespan_policy (PortableServer::LifespanPolicyValue value,
                                                             CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  PortableServer::IdUniquenessPolicy_ptr create_id_uniqueness_policy (PortableServer::IdUniquenessPolicyValue value,
                                                                      CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  PortableServer::IdAssignmentPolicy_ptr create_id_assignment_policy (PortableServer::IdAssignmentPolicyValue value,
                                                                      CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

#if !defined (TAO_HAS_MINIMUM_CORBA)

  PortableServer::ImplicitActivationPolicy_ptr create_implicit_activation_policy (PortableServer::ImplicitActivationPolicyValue value,
                                                                                  CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  PortableServer::ServantRetentionPolicy_ptr create_servant_retention_policy (PortableServer::ServantRetentionPolicyValue value,
                                                                              CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  PortableServer::RequestProcessingPolicy_ptr create_request_processing_policy (PortableServer::RequestProcessingPolicyValue value,
                                                                                CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

#endif /* TAO_HAS_MINIMUM_CORBA */

  CORBA::String the_name (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  PortableServer::POA_ptr the_parent (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  PortableServer::POAManager_ptr the_POAManager (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

#if !defined (TAO_HAS_MINIMUM_CORBA)

  PortableServer::AdapterActivator_ptr the_activator (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  void the_activator (PortableServer::AdapterActivator_ptr adapter_activator,
                      CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  PortableServer::ServantManager_ptr get_servant_manager (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  void set_servant_manager (PortableServer::ServantManager_ptr imgr,
                            CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  PortableServer::Servant get_servant (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  void set_servant (PortableServer::Servant servant,
                    CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

#endif /* TAO_HAS_MINIMUM_CORBA */

  PortableServer::ObjectId *activate_object (PortableServer::Servant p_servant,
                                             CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  void activate_object_with_id (const PortableServer::ObjectId &id,
                                PortableServer::Servant p_servant,
                                CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  void deactivate_object (const PortableServer::ObjectId &oid,
                          CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  CORBA::Object_ptr create_reference (const char *intf,
                                      CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  CORBA::Object_ptr create_reference_with_id (const PortableServer::ObjectId &oid,
                                              const char *intf,
                                              CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());
  PortableServer::ObjectId *servant_to_id (PortableServer::Servant p_servant,
                                           CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  CORBA::Object_ptr servant_to_reference (PortableServer::Servant p_servant,
                                          CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  PortableServer::Servant reference_to_servant (CORBA::Object_ptr reference,
                                                CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  PortableServer::ObjectId *reference_to_id (CORBA::Object_ptr reference,
                                             CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  PortableServer::Servant id_to_servant (const PortableServer::ObjectId &oid,
                                         CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  CORBA::Object_ptr id_to_reference (const PortableServer::ObjectId &oid,
                                     CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

#if !defined (TAO_HAS_MINIMUM_CORBA)

  void forward_object (const PortableServer::ObjectId &oid,
                       CORBA::Object_ptr forward_to,
                       CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

#endif /* TAO_HAS_MINIMUM_CORBA */

  // Utility functions for the other
  static void encode_sequence_to_string (CORBA::String &str,
                                         const TAO_Unbounded_Sequence<CORBA::Octet> &seq);
  static void decode_string_to_sequence (TAO_Unbounded_Sequence<CORBA::Octet> &seq,
                                         const char *str);

  static char* ObjectId_to_string (const PortableServer::ObjectId &id);

  static CORBA::WChar* ObjectId_to_wstring (const PortableServer::ObjectId &id);

  static PortableServer::ObjectId *string_to_ObjectId (const char *id);

  static PortableServer::ObjectId *string_to_ObjectId (const char *string,
                                                       int size);

  static PortableServer::ObjectId *wstring_to_ObjectId (const CORBA::WChar *id);

  TAO_POA (const String &name,
           TAO_POA_Manager &poa_manager,
           const TAO_POA_Policies &policies,
           TAO_POA *parent,
           ACE_Lock &lock,
           ACE_SYNCH_MUTEX &thread_lock,
           TAO_ORB_Core &orb_core,
           CORBA_Environment &ACE_TRY_ENV);

  ~TAO_POA (void);

  static char name_separator (void);

  static CORBA::ULong name_separator_length (void);

  enum
  {
    TAO_OBJECTKEY_PREFIX_SIZE = 4
  };

  static CORBA::Octet objectkey_prefix[TAO_OBJECTKEY_PREFIX_SIZE];

  const TAO_Object_Adapter::poa_name &folded_name (void) const;

  const TAO_Object_Adapter::poa_name &system_name (void) const;

protected:

  const ACE_CString &name (void) const;

  TAO_POA *create_POA (const String &adapter_name,
                       TAO_POA_Manager &poa_manager,
                       const TAO_POA_Policies &policies,
                       CORBA_Environment &ACE_TRY_ENV);

  TAO_POA *create_POA_i (const String &adapter_name,
                         TAO_POA_Manager &poa_manager,
                         const TAO_POA_Policies &policies,
                         CORBA_Environment &ACE_TRY_ENV);

  TAO_POA *find_POA_i (const ACE_CString &child_name,
                       CORBA::Boolean activate_it,
                       CORBA::Environment &ACE_TRY_ENV);

  void destroy_i (CORBA::Boolean etherealize_objects,
                  CORBA::Boolean wait_for_completion,
                  CORBA_Environment &ACE_TRY_ENV);

#if !defined (TAO_HAS_MINIMUM_CORBA)

  PortableServer::ServantManager_ptr get_servant_manager_i (CORBA_Environment &ACE_TRY_ENV);

  void set_servant_manager_i (PortableServer::ServantManager_ptr imgr,
                              CORBA_Environment &ACE_TRY_ENV);

  PortableServer::Servant get_servant_i (CORBA_Environment &ACE_TRY_ENV);

  void set_servant_i (PortableServer::Servant servant,
                      CORBA_Environment &ACE_TRY_ENV);

#endif /* TAO_HAS_MINIMUM_CORBA */

  PortableServer::ObjectId *activate_object_i (PortableServer::Servant p_servant,
                                               CORBA_Environment &ACE_TRY_ENV);

  void activate_object_with_id_i (const PortableServer::ObjectId &id,
                                  PortableServer::Servant p_servant,
                                  CORBA_Environment &ACE_TRY_ENV);

  void deactivate_all_objects_i (CORBA::Boolean etherealize_objects,
                                 CORBA::Environment &ACE_TRY_ENV);

  void deactivate_object_i (const PortableServer::ObjectId &oid,
                            CORBA_Environment &ACE_TRY_ENV);

  void cleanup_servant (TAO_Active_Object_Map::Map_Entry *active_object_map_entry,
                        CORBA::Environment &ACE_TRY_ENV);

  CORBA::Object_ptr create_reference_i (const char *intf,
                                        CORBA_Environment &ACE_TRY_ENV);

  CORBA::Object_ptr create_reference_with_id_i (const PortableServer::ObjectId &oid,
                                                const char *intf,
                                                CORBA_Environment &ACE_TRY_ENV);
  PortableServer::ObjectId *servant_to_id_i (PortableServer::Servant servant,
                                             CORBA_Environment &ACE_TRY_ENV);

  PortableServer::ObjectId *servant_to_system_id (PortableServer::Servant p_servant,
                                                  CORBA_Environment &ACE_TRY_ENV);

  PortableServer::ObjectId *servant_to_system_id_i (PortableServer::Servant p_servant,
                                                    CORBA_Environment &ACE_TRY_ENV);

  PortableServer::Servant id_to_servant_i (const PortableServer::ObjectId &oid,
                                           CORBA_Environment &ACE_TRY_ENV);

  CORBA::Object_ptr id_to_reference_i (const PortableServer::ObjectId &oid,
                                       CORBA_Environment &ACE_TRY_ENV);

#if !defined (TAO_HAS_MINIMUM_CORBA)

  void forward_object_i (const PortableServer::ObjectId &oid,
                         CORBA::Object_ptr forward_to,
                         CORBA_Environment &ACE_TRY_ENV);

#endif /* TAO_HAS_MINIMUM_CORBA */

  ACE_Lock &lock (void);

  TAO_POA_Policies &policies (void);

  TAO_Active_Object_Map &active_object_map (void) const;

  int delete_child (const String &child);

  void set_folded_name (void);

  TAO_ObjectKey *create_object_key (const PortableServer::ObjectId &id);

  int is_poa_generated_id (const PortableServer::ObjectId &id);

  static int parse_key (const TAO_ObjectKey &key,
                        TAO_Object_Adapter::poa_name &poa_system_name,
                        PortableServer::ObjectId &system_id,
                        CORBA::Boolean &is_root,
                        CORBA::Boolean &is_persistent,
                        CORBA::Boolean &is_system_id,
                        TAO_Temporary_Creation_Time &poa_creation_time);

  // Should really be protected, but some compilers complain
public:
  enum LOCATION_RESULT
  {
    FOUND,
    DEFAULT_SERVANT,
    SERVANT_MANAGER,
    NOT_FOUND
  };

protected:
  LOCATION_RESULT locate_servant_i (const PortableServer::ObjectId &id,
                                    PortableServer::Servant &servant,
                                    CORBA_Environment &ACE_TRY_ENV);

  PortableServer::Servant locate_servant_i (const char *operation,
                                            const PortableServer::ObjectId &id,
                                            TAO_POA_Current *poa_current,
                                            CORBA_Environment &ACE_TRY_ENV);

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

  void establish_servant_lock (PortableServer::Servant servant);

  void teardown_servant_lock (PortableServer::Servant servant);

  String name_;

  TAO_Object_Adapter::poa_name folded_name_;

  TAO_Object_Adapter::poa_name_var system_name_;

  TAO_POA_Manager &poa_manager_;

  TAO_POA_Policies policies_;

  TAO_POA *parent_;

  TAO_Active_Object_Map *active_object_map_;

  int delete_active_object_map_;

#if !defined (TAO_HAS_MINIMUM_CORBA)

  PortableServer::AdapterActivator_var adapter_activator_;

  PortableServer::ServantActivator_var servant_activator_;

  PortableServer::ServantLocator_var servant_locator_;

  PortableServer::ServantBase_var default_servant_;

#endif /* TAO_HAS_MINIMUM_CORBA */

  typedef ACE_Hash_Map_Manager<ACE_CString, TAO_POA *, ACE_Null_Mutex>
  CHILDREN;

  CHILDREN children_;

  ACE_Lock &lock_;

  int persistent_;

  int system_id_;

  TAO_Creation_Time creation_time_;

  TAO_ORB_Core &orb_core_;

  CORBA::Boolean cleanup_in_progress_;

  CORBA::Boolean etherealize_objects_;

  CORBA::ULong outstanding_requests_;

  ACE_SYNCH_CONDITION outstanding_requests_condition_;

  CORBA::Boolean destroy_pending_;
};

#if !defined (TAO_HAS_MINIMUM_CORBA)

class TAO_Export TAO_Adapter_Activator : public POA_PortableServer::AdapterActivator
{
public:

  CORBA::Boolean unknown_adapter (PortableServer::POA_ptr parent,
                                  const char *name,
                                  CORBA_Environment &ACE_TRY_ENV);
};

#endif /* TAO_HAS_MINIMUM_CORBA */

class TAO_Export TAO_POA_Current : public POA_PortableServer::Current
{
  // = TITLE
  //
  // Implementation of the PortableServer::Current object.
  //
  // = DESCRIPTION
  //
  // Objects of this class hold state information regarding the
  // current POA invocation.  Savvy readers will notice that this
  // contains substantially more methods than the POA spec shows; they
  // exist because the ORB either (a) needs them or (b) finds them
  // useful for implementing a more efficient ORB.
  //
  // The intent is that instances of this class are held in
  // Thread-Specific Storage so that upcalls can get context
  // information regarding their invocation.  The POA itself must
  // insure that all <set_*> operations are performed in the execution
  // thread so that the proper <TAO_POA_Current> pointer is obtained
  // from TSS.

public:

  friend class TAO_POA;

  // = Specification-mandated methods

  PortableServer::POA_ptr get_POA (CORBA_Environment &ACE_TRY_ENV);
  // Return pointer to the invoking POA.  Raises the
  // <CORBA::NoContext> exception.

  PortableServer::ObjectId *get_object_id (CORBA_Environment &ACE_TRY_ENV);
  // Return pointer to the object id through which this was invoked.
  // This may be necessary in cases where a <Servant> is serving under
  // the guise of multiple object ids.  This has _out semantics Raises
  // the <CORBA::NoContext> exception.

  // = TAO Extensions

  void clear (void);
  // Clear any prior settings made.  This will make things which can
  // throw the <CORBA::NoContext> exception raise it if invoked
  // without a corresponding <set_*> operation.

  int context_is_valid (void);
  // Returns non-zero if the context is valid, i.e., if it would be
  // impossible for a <CORBA::NoContext> exception to be raised.

  void POA_impl (TAO_POA *impl);
  // Set the POA implementation.

  TAO_POA *POA_impl (void) const;
  // Get the POA imeplemantation

  void object_id (const PortableServer::ObjectId &id);
  // Set the object ID.

  const PortableServer::ObjectId &object_id (void) const;
  // Get the object ID.

  void object_key (const TAO_ObjectKey &key);
  // Set the object key.

  const TAO_ObjectKey &object_key (void) const;
  // Get the object key.

  void servant (PortableServer::Servant servant);
  // Set the servant for the current upcall.

  PortableServer::Servant servant (void) const;
  // Get the servant for the current upcall.

  int in_upcall (void) const;
  // Get whether we're in an upcall (non-zero is yes).

#if !defined (TAO_HAS_MINIMUM_CORBA)

  PortableServer::ServantLocator::Cookie locator_cookie (void) const;
  // Get the Servant Locator's cookie

  void locator_cookie (PortableServer::ServantLocator::Cookie cookie);
  // Set the Servant Locator's cookie

#endif /* TAO_HAS_MINIMUM_CORBA */

  TAO_POA_Current (void);
  // Constructor

  TAO_POA_Current (TAO_POA *impl,
                   const TAO_ObjectKey &key,
                   PortableServer::Servant servant,
                   const char *operation,
                   TAO_ORB_Core &orb_core);
  // Convenience constructor combining construction & initialization.

  ~TAO_POA_Current (void);
  // Destructor

protected:
  TAO_POA *poa_impl_;
  // The POA implementation invoking an upcall

  PortableServer::ObjectId object_id_;
  // The object ID of the current context.

  const TAO_ObjectKey *object_key_;
  // The object key of the current context.

#if !defined (TAO_HAS_MINIMUM_CORBA)

  PortableServer::ServantLocator::Cookie cookie_;
  // Servant Locator's cookie

#endif /* TAO_HAS_MINIMUM_CORBA */

  PortableServer::Servant servant_;
  // The servant for the current upcall.

  const char *operation_;
  // Operation name for this current.

  TAO_ORB_Core *orb_core_;
  // ORB Core for this current.

  TAO_POA_Current *previous_current_;
  // Current previous from <this>.

  TAO_Active_Object_Map::Map_Entry *active_object_map_entry_;
  // Pointer to the entry in the TAO_Active_Object_Map corresponding
  // to the servant for this request.

  // = Hidden because we don't allow these
  TAO_POA_Current (const TAO_POA_Current &);
  void operator= (const TAO_POA_Current &);
};

#if defined (__ACE_INLINE__)
# include "tao/POA.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_POA_H */
