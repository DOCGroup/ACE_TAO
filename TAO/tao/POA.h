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

class TAO_POA;

#if !defined (TAO_HAS_MINIMUM_CORBA)

class TAO_Export TAO_Thread_Policy : public POA_PortableServer::ThreadPolicy
{
public:
  TAO_Thread_Policy (PortableServer::ThreadPolicyValue value,
                     PortableServer::POA_ptr poa);

  TAO_Thread_Policy (const TAO_Thread_Policy &new_policy);

  virtual PortableServer::ThreadPolicyValue value (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  virtual CORBA::Policy_ptr copy (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  virtual void destroy (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  virtual CORBA::PolicyType policy_type (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  virtual PortableServer::POA_ptr _default_POA (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

protected:
  PortableServer::ThreadPolicyValue value_;

  PortableServer::POA_var poa_;
};

#endif /* TAO_HAS_MINIMUM_CORBA */

class TAO_Export TAO_Lifespan_Policy : public POA_PortableServer::LifespanPolicy
{
public:
  TAO_Lifespan_Policy (PortableServer::LifespanPolicyValue value,
                       PortableServer::POA_ptr poa);

  TAO_Lifespan_Policy (const TAO_Lifespan_Policy &rhs);

  virtual PortableServer::LifespanPolicyValue value (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  virtual CORBA::Policy_ptr copy (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  virtual void destroy (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  virtual CORBA::PolicyType policy_type (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  virtual PortableServer::POA_ptr _default_POA (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

protected:
  PortableServer::LifespanPolicyValue value_;

  PortableServer::POA_var poa_;
};

class TAO_Export TAO_Id_Uniqueness_Policy : public POA_PortableServer::IdUniquenessPolicy
{
public:
  TAO_Id_Uniqueness_Policy (PortableServer::IdUniquenessPolicyValue value,
                            PortableServer::POA_ptr poa);

  TAO_Id_Uniqueness_Policy (const TAO_Id_Uniqueness_Policy &rhs);

  virtual PortableServer::IdUniquenessPolicyValue value (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  virtual CORBA::Policy_ptr copy (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  virtual void destroy (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  virtual CORBA::PolicyType policy_type (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  virtual PortableServer::POA_ptr _default_POA (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

protected:
  PortableServer::IdUniquenessPolicyValue value_;

  PortableServer::POA_var poa_;
};

class TAO_Export TAO_Id_Assignment_Policy : public POA_PortableServer::IdAssignmentPolicy
{
public:
  TAO_Id_Assignment_Policy (PortableServer::IdAssignmentPolicyValue value,
                            PortableServer::POA_ptr poa);

  TAO_Id_Assignment_Policy (const TAO_Id_Assignment_Policy &rhs);

  virtual PortableServer::IdAssignmentPolicyValue value (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  virtual CORBA::Policy_ptr copy (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  virtual void destroy (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  virtual CORBA::PolicyType policy_type (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  virtual PortableServer::POA_ptr _default_POA (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

protected:
  PortableServer::IdAssignmentPolicyValue value_;

  PortableServer::POA_var poa_;
};

#if !defined (TAO_HAS_MINIMUM_CORBA)

class TAO_Export TAO_Implicit_Activation_Policy : public POA_PortableServer::ImplicitActivationPolicy
{
public:
  TAO_Implicit_Activation_Policy (PortableServer::ImplicitActivationPolicyValue value,
                                  PortableServer::POA_ptr poa);

  TAO_Implicit_Activation_Policy (const TAO_Implicit_Activation_Policy &rhs);

  virtual PortableServer::ImplicitActivationPolicyValue value (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  virtual CORBA::Policy_ptr copy (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  virtual void destroy (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  virtual CORBA::PolicyType policy_type (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  virtual PortableServer::POA_ptr _default_POA (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

protected:
  PortableServer::ImplicitActivationPolicyValue value_;

  PortableServer::POA_var poa_;
};

class TAO_Export TAO_Servant_Retention_Policy : public POA_PortableServer::ServantRetentionPolicy
{
public:
  TAO_Servant_Retention_Policy (PortableServer::ServantRetentionPolicyValue value,
                                PortableServer::POA_ptr poa);

  TAO_Servant_Retention_Policy (const TAO_Servant_Retention_Policy &rhs);

  virtual PortableServer::ServantRetentionPolicyValue value (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  virtual CORBA::Policy_ptr copy (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  virtual void destroy (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  virtual CORBA::PolicyType policy_type (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  virtual PortableServer::POA_ptr _default_POA (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

protected:
  PortableServer::ServantRetentionPolicyValue value_;

  PortableServer::POA_var poa_;
};

class TAO_Export TAO_Request_Processing_Policy : public POA_PortableServer::RequestProcessingPolicy
{
public:
  TAO_Request_Processing_Policy (PortableServer::RequestProcessingPolicyValue value,
                                 PortableServer::POA_ptr poa);

  TAO_Request_Processing_Policy (const TAO_Request_Processing_Policy &rhs);

  virtual PortableServer::RequestProcessingPolicyValue value (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  virtual CORBA::Policy_ptr copy (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  virtual void destroy (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  virtual CORBA::PolicyType policy_type (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  virtual PortableServer::POA_ptr _default_POA (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

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

  virtual PortableServer::ThreadPolicyValue thread (void) const;
  virtual void thread (PortableServer::ThreadPolicyValue value);

#endif /* TAO_HAS_MINIMUM_CORBA */

  virtual PortableServer::LifespanPolicyValue lifespan (void) const;
  virtual void lifespan (PortableServer::LifespanPolicyValue value);

  virtual PortableServer::IdUniquenessPolicyValue id_uniqueness (void) const;
  virtual void id_uniqueness (PortableServer::IdUniquenessPolicyValue value);

  virtual PortableServer::IdAssignmentPolicyValue id_assignment (void) const;
  virtual void id_assignment (PortableServer::IdAssignmentPolicyValue value);

  virtual PortableServer::ImplicitActivationPolicyValue implicit_activation (void) const;
  virtual void implicit_activation (PortableServer::ImplicitActivationPolicyValue value);

  virtual PortableServer::ServantRetentionPolicyValue servant_retention (void) const;
  virtual void servant_retention (PortableServer::ServantRetentionPolicyValue value);

  virtual PortableServer::RequestProcessingPolicyValue request_processing (void) const;
  virtual void request_processing (PortableServer::RequestProcessingPolicyValue value);

  virtual void parse_policies (const CORBA::PolicyList &policies,
                               CORBA_Environment &ACE_TRY_ENV);

protected:

  virtual void parse_policy (const CORBA::Policy_ptr policy,
                             CORBA_Environment &ACE_TRY_ENV);

  virtual int validity_check (void);

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

  virtual void creation_time (const void *creation_time);

  virtual const void *creation_time (void) const;

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

  virtual void creation_time (const void *creation_time);

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
  friend class TAO_POA_Current;

  typedef ACE_CString String;

  virtual PortableServer::POA_ptr create_POA (const char *adapter_name,
                                              PortableServer::POAManager_ptr poa_manager,
                                              const CORBA::PolicyList &policies,
                                              CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  virtual PortableServer::POA_ptr find_POA (const char *adapter_name,
                                            CORBA::Boolean activate_it,
                                            CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  virtual void destroy (CORBA::Boolean etherealize_objects,
                        CORBA::Boolean wait_for_completion,
                        CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

#if !defined (TAO_HAS_MINIMUM_CORBA)

  virtual PortableServer::ThreadPolicy_ptr create_thread_policy (PortableServer::ThreadPolicyValue value,
                                                                 CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

#endif /* TAO_HAS_MINIMUM_CORBA */

  virtual PortableServer::LifespanPolicy_ptr create_lifespan_policy (PortableServer::LifespanPolicyValue value,
                                                                     CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  virtual PortableServer::IdUniquenessPolicy_ptr create_id_uniqueness_policy (PortableServer::IdUniquenessPolicyValue value,
                                                                              CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  virtual PortableServer::IdAssignmentPolicy_ptr create_id_assignment_policy (PortableServer::IdAssignmentPolicyValue value,
                                                                              CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

#if !defined (TAO_HAS_MINIMUM_CORBA)

  virtual PortableServer::ImplicitActivationPolicy_ptr create_implicit_activation_policy (PortableServer::ImplicitActivationPolicyValue value,
                                                                                          CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  virtual PortableServer::ServantRetentionPolicy_ptr create_servant_retention_policy (PortableServer::ServantRetentionPolicyValue value,
                                                                                      CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  virtual PortableServer::RequestProcessingPolicy_ptr create_request_processing_policy (PortableServer::RequestProcessingPolicyValue value,
                                                                                        CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

#endif /* TAO_HAS_MINIMUM_CORBA */

  virtual CORBA::String the_name (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  virtual PortableServer::POA_ptr the_parent (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  virtual PortableServer::POAManager_ptr the_POAManager (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

#if !defined (TAO_HAS_MINIMUM_CORBA)

  virtual PortableServer::AdapterActivator_ptr the_activator (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  virtual void the_activator (PortableServer::AdapterActivator_ptr adapter_activator,
                              CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  virtual PortableServer::ServantManager_ptr get_servant_manager (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  virtual void set_servant_manager (PortableServer::ServantManager_ptr imgr,
                                    CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  virtual PortableServer::Servant get_servant (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  virtual void set_servant (PortableServer::Servant servant,
                            CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

#endif /* TAO_HAS_MINIMUM_CORBA */

  virtual PortableServer::ObjectId *activate_object (PortableServer::Servant p_servant,
                                                     CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  virtual void activate_object_with_id (const PortableServer::ObjectId &id,
                                        PortableServer::Servant p_servant,
                                        CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  virtual void deactivate_object (const PortableServer::ObjectId &oid,
                                  CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  virtual CORBA::Object_ptr create_reference (const char *intf,
                                              CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  virtual CORBA::Object_ptr create_reference_with_id (const PortableServer::ObjectId &oid,
                                                      const char *intf,
                                                      CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());
  virtual PortableServer::ObjectId *servant_to_id (PortableServer::Servant p_servant,
                                                   CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  virtual CORBA::Object_ptr servant_to_reference (PortableServer::Servant p_servant,
                                                  CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  virtual PortableServer::Servant reference_to_servant (CORBA::Object_ptr reference,
                                                        CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  virtual PortableServer::ObjectId *reference_to_id (CORBA::Object_ptr reference,
                                                     CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  virtual PortableServer::Servant id_to_servant (const PortableServer::ObjectId &oid,
                                                 CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  virtual CORBA::Object_ptr id_to_reference (const PortableServer::ObjectId &oid,
                                             CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

#if !defined (TAO_HAS_MINIMUM_CORBA)

  virtual void forward_object (const PortableServer::ObjectId &oid,
                               CORBA::Object_ptr forward_to,
                               CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

#endif /* TAO_HAS_MINIMUM_CORBA */

  // Utility functions for the other
  static void encode_sequence_to_string (CORBA::String &str,
                                         const TAO_Unbounded_Sequence<CORBA::Octet> &seq);
  static void decode_string_to_sequence (TAO_Unbounded_Sequence<CORBA::Octet> &seq,
                                         const char *str);

  static CORBA::String ObjectId_to_string (const PortableServer::ObjectId &id);

  static CORBA::WString ObjectId_to_wstring (const PortableServer::ObjectId &id);

  static PortableServer::ObjectId *string_to_ObjectId (const char *id);

  static PortableServer::ObjectId *string_to_ObjectId (const char *string,
                                                       int size);

  static PortableServer::ObjectId *wstring_to_ObjectId (const CORBA::WChar *id);

  TAO_POA (const String &name,
           TAO_POA_Manager &poa_manager,
           const TAO_POA_Policies &policies,
           TAO_POA *parent,
           ACE_Lock &lock,
           TAO_ORB_Core &orb_core,
           CORBA_Environment &ACE_TRY_ENV);

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

protected:

  const ACE_CString &name (void) const;

  virtual TAO_POA *create_POA (const String &adapter_name,
                               TAO_POA_Manager &poa_manager,
                               const TAO_POA_Policies &policies,
                               CORBA_Environment &ACE_TRY_ENV);

  virtual TAO_POA *create_POA_i (const String &adapter_name,
                                 TAO_POA_Manager &poa_manager,
                                 const TAO_POA_Policies &policies,
                                 CORBA_Environment &ACE_TRY_ENV);

  virtual TAO_POA *find_POA_i (const ACE_CString &child_name,
                               CORBA::Boolean activate_it,
                               CORBA::Environment &ACE_TRY_ENV);

  virtual void destroy_i (CORBA::Boolean etherealize_objects,
                          CORBA::Boolean wait_for_completion,
                          CORBA_Environment &ACE_TRY_ENV);

#if !defined (TAO_HAS_MINIMUM_CORBA)

  virtual PortableServer::ServantManager_ptr get_servant_manager_i (CORBA_Environment &ACE_TRY_ENV);

  virtual void set_servant_manager_i (PortableServer::ServantManager_ptr imgr,
                                      CORBA_Environment &ACE_TRY_ENV);

  virtual PortableServer::Servant get_servant_i (CORBA_Environment &ACE_TRY_ENV);

  virtual void set_servant_i (PortableServer::Servant servant,
                              CORBA_Environment &ACE_TRY_ENV);

#endif /* TAO_HAS_MINIMUM_CORBA */

  virtual PortableServer::ObjectId *activate_object_i (PortableServer::Servant p_servant,
                                                       CORBA_Environment &ACE_TRY_ENV);

  virtual void activate_object_with_id_i (const PortableServer::ObjectId &id,
                                          PortableServer::Servant p_servant,
                                          CORBA_Environment &ACE_TRY_ENV);

  virtual void deactivate_object_i (const PortableServer::ObjectId &oid,
                                    CORBA_Environment &ACE_TRY_ENV);

  virtual CORBA::Object_ptr create_reference_i (const char *intf,
                                                CORBA_Environment &ACE_TRY_ENV);

  virtual CORBA::Object_ptr create_reference_with_id_i (const PortableServer::ObjectId &oid,
                                                        const char *intf,
                                                        CORBA_Environment &ACE_TRY_ENV);
  virtual PortableServer::ObjectId *servant_to_id_i (PortableServer::Servant servant,
                                                     CORBA_Environment &ACE_TRY_ENV);

  virtual PortableServer::ObjectId *servant_to_system_id (PortableServer::Servant p_servant,
                                                          CORBA_Environment &ACE_TRY_ENV);

  virtual PortableServer::ObjectId *servant_to_system_id_i (PortableServer::Servant p_servant,
                                                            CORBA_Environment &ACE_TRY_ENV);

  virtual PortableServer::Servant id_to_servant_i (const PortableServer::ObjectId &oid,
                                                   CORBA_Environment &ACE_TRY_ENV);

  virtual CORBA::Object_ptr id_to_reference_i (const PortableServer::ObjectId &oid,
                                               CORBA_Environment &ACE_TRY_ENV);

#if !defined (TAO_HAS_MINIMUM_CORBA)

  virtual void forward_object_i (const PortableServer::ObjectId &oid,
                                 CORBA::Object_ptr forward_to,
                                 CORBA_Environment &ACE_TRY_ENV);

#endif /* TAO_HAS_MINIMUM_CORBA */

  virtual ACE_Lock &lock (void);

  virtual TAO_POA_Policies &policies (void);

  virtual TAO_Active_Object_Map &active_object_map (void) const;

  virtual int delete_child (const String &child);

  virtual void set_folded_name (void);

  virtual TAO_ObjectKey *create_object_key (const PortableServer::ObjectId &id);

  virtual int is_poa_generated_id (const PortableServer::ObjectId &id);

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
  virtual LOCATION_RESULT locate_servant_i (const PortableServer::ObjectId &id,
                                            PortableServer::Servant &servant,
                                            CORBA_Environment &ACE_TRY_ENV);

  virtual PortableServer::Servant locate_servant_i (const char *operation,
                                                    const PortableServer::ObjectId &id,
                                                    TAO_POA_Current *poa_current,
                                                    CORBA_Environment &ACE_TRY_ENV);

  virtual const TAO_Creation_Time &creation_time (void);

  virtual CORBA::Boolean persistent (void);

  virtual char persistent_key_type (void);

  static char persistent_key_char (void);

  static char transient_key_char (void);

  static CORBA::ULong persistent_key_type_length (void);

  virtual CORBA::Boolean system_id (void);

  virtual char system_id_key_type (void);

  static char system_id_key_char (void);

  static char user_id_key_char (void);

  static CORBA::ULong system_id_key_type_length (void);

  virtual CORBA::Boolean root (void);

  virtual char root_key_type (void);

  static char root_key_char (void);

  static char non_root_key_char (void);

  static CORBA::ULong root_key_type_length (void);

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

  PortableServer::Servant default_servant_;

#endif /* TAO_HAS_MINIMUM_CORBA */

  typedef ACE_Hash_Map_Manager<ACE_CString, TAO_POA *, ACE_Null_Mutex>
          CHILDREN;

  CHILDREN children_;

  ACE_Lock &lock_;

  int closing_down_;

  int persistent_;

  int system_id_;

  TAO_Creation_Time creation_time_;

  TAO_ORB_Core &orb_core_;
};

#if !defined (TAO_HAS_MINIMUM_CORBA)

class TAO_Export TAO_Adapter_Activator : public POA_PortableServer::AdapterActivator
{
public:

  virtual CORBA::Boolean unknown_adapter (PortableServer::POA_ptr parent,
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

  virtual PortableServer::POA_ptr get_POA (CORBA_Environment &ACE_TRY_ENV);
  // Return pointer to the invoking POA.  Raises the
  // <CORBA::NoContext> exception.

  virtual PortableServer::ObjectId *get_object_id (CORBA_Environment &ACE_TRY_ENV);
  // Return pointer to the object id through which this was invoked.
  // This may be necessary in cases where a <Servant> is serving under
  // the guise of multiple object ids.  This has _out semantics Raises
  // the <CORBA::NoContext> exception.

  // = TAO Extensions

  virtual void clear (void);
  // Clear any prior settings made.  This will make things which can
  // throw the <CORBA::NoContext> exception raise it if invoked
  // without a corresponding <set_*> operation.

  virtual int context_is_valid (void);
  // Returns non-zero if the context is valid, i.e., if it would be
  // impossible for a <CORBA::NoContext> exception to be raised.

  virtual void POA_impl (TAO_POA *impl);
  // Set the POA implementation.

  virtual TAO_POA *POA_impl (void) const;
  // Get the POA imeplemantation

  virtual void object_id (const PortableServer::ObjectId &id);
  // Set the object ID.

  virtual const PortableServer::ObjectId &object_id (void) const;
  // Get the object ID.

  virtual void object_key (const TAO_ObjectKey &key);
  // Set the object key.

  virtual const TAO_ObjectKey &object_key (void) const;
  // Get the object key.

  virtual void servant (PortableServer::Servant servant);
  // Set the servant for the current upcall.

  virtual PortableServer::Servant servant (void) const;
  // Get the servant for the current upcall.

  virtual int in_upcall (void) const;
  // Get whether we're in an upcall (non-zero is yes).

#if !defined (TAO_HAS_MINIMUM_CORBA)

  virtual PortableServer::ServantLocator::Cookie locator_cookie (void) const;
  // Get the Servant Locator's cookie

  virtual void locator_cookie (PortableServer::ServantLocator::Cookie cookie);
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

  virtual ~TAO_POA_Current (void);
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

  // = Hidden because we don't allow these
  TAO_POA_Current (const TAO_POA_Current &);
  void operator= (const TAO_POA_Current &);
};

#if defined (__ACE_INLINE__)
# include "tao/POA.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_POA_H */
