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

// Object_Key
#include "tao/Object_KeyC.h"

// RT CORBA
#include "tao/RTCORBAC.h"

// Local Object
#include "tao/LocalObject.h"

// Map
#include "ace/Hash_Map_Manager.h"

// Vector
#include "ace/Containers.h"

// Locking
#include "ace/Synch.h"

// This is to remove "inherits via dominance" warnings from MSVC.
// MSVC is being a little too paranoid.
#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_POA;
class TAO_ServerProtocolPolicy;
class TAO_PriorityBandedConnectionPolicy;
class TAO_Acceptor_Filter;

#if (TAO_HAS_MINIMUM_POA == 0)

class TAO_PortableServer_Export TAO_Thread_Policy :
  public PortableServer::ThreadPolicy,
  public TAO_Local_RefCounted_Object
{
public:
  TAO_Thread_Policy (PortableServer::ThreadPolicyValue value);

  PortableServer::ThreadPolicyValue value (CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::Policy_ptr copy (CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  void destroy (CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::PolicyType policy_type (CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

protected:
  PortableServer::ThreadPolicyValue value_;
};

#endif /* TAO_HAS_MINIMUM_POA == 0 */

class TAO_PortableServer_Export TAO_Lifespan_Policy :
  public PortableServer::LifespanPolicy,
  public TAO_Local_RefCounted_Object
{
public:
  TAO_Lifespan_Policy (PortableServer::LifespanPolicyValue value);

  PortableServer::LifespanPolicyValue value (CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::Policy_ptr copy (CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  void destroy (CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::PolicyType policy_type (CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

protected:
  PortableServer::LifespanPolicyValue value_;
};

class TAO_PortableServer_Export TAO_Id_Uniqueness_Policy :
  public PortableServer::IdUniquenessPolicy,
  public TAO_Local_RefCounted_Object
{
public:
  TAO_Id_Uniqueness_Policy (PortableServer::IdUniquenessPolicyValue value);

  PortableServer::IdUniquenessPolicyValue value (CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::Policy_ptr copy (CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  void destroy (CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::PolicyType policy_type (CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

protected:
  PortableServer::IdUniquenessPolicyValue value_;
};

class TAO_PortableServer_Export TAO_Id_Assignment_Policy :
  public PortableServer::IdAssignmentPolicy,
  public TAO_Local_RefCounted_Object
{
public:
  TAO_Id_Assignment_Policy (PortableServer::IdAssignmentPolicyValue value);

  PortableServer::IdAssignmentPolicyValue value (CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::Policy_ptr copy (CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  void destroy (CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::PolicyType policy_type (CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

protected:
  PortableServer::IdAssignmentPolicyValue value_;
};

#if (TAO_HAS_MINIMUM_POA == 0)

class TAO_PortableServer_Export TAO_Implicit_Activation_Policy :
  public PortableServer::ImplicitActivationPolicy,
  public TAO_Local_RefCounted_Object
{
public:
  TAO_Implicit_Activation_Policy (PortableServer::ImplicitActivationPolicyValue value);

  PortableServer::ImplicitActivationPolicyValue value (CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::Policy_ptr copy (CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  void destroy (CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::PolicyType policy_type (CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

protected:
  PortableServer::ImplicitActivationPolicyValue value_;
};

class TAO_PortableServer_Export TAO_Servant_Retention_Policy :
  public PortableServer::ServantRetentionPolicy,
  public TAO_Local_RefCounted_Object
{
public:
  TAO_Servant_Retention_Policy (PortableServer::ServantRetentionPolicyValue value);

  PortableServer::ServantRetentionPolicyValue value (CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::Policy_ptr copy (CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  void destroy (CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::PolicyType policy_type (CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

protected:
  PortableServer::ServantRetentionPolicyValue value_;
};

class TAO_PortableServer_Export TAO_Request_Processing_Policy :
  public PortableServer::RequestProcessingPolicy,
  public TAO_Local_RefCounted_Object
{
public:
  TAO_Request_Processing_Policy (PortableServer::RequestProcessingPolicyValue value);

  PortableServer::RequestProcessingPolicyValue value (CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::Policy_ptr copy (CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  void destroy (CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::PolicyType policy_type (CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

protected:
  PortableServer::RequestProcessingPolicyValue value_;
};

#endif /* TAO_HAS_MINIMUM_POA == 0 */

class TAO_PortableServer_Export TAO_POA_Policies
{
public:

  enum PriorityModel
  {
    CLIENT_PROPAGATED,
    SERVER_DECLARED
  };

  TAO_POA_Policies (TAO_ORB_Core &orb_core,
                    CORBA::Environment &ACE_TRY_ENV);

  TAO_POA_Policies (const TAO_POA_Policies &rhs);

  ~TAO_POA_Policies (void);

  PortableServer::ThreadPolicyValue thread (void) const;
  void thread (PortableServer::ThreadPolicyValue value);

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

  PriorityModel priority_model (void) const;
  void priority_model (PriorityModel value);

  CORBA::Short server_priority (void) const;
  void server_priority (CORBA::Short value);

  #if (TAO_HAS_RT_CORBA == 1)

  TAO_ServerProtocolPolicy *server_protocol (void) const;
  void server_protocol (TAO_ServerProtocolPolicy *policy);

  TAO_PriorityBandedConnectionPolicy *priority_bands (void) const;
  void priority_bands (TAO_PriorityBandedConnectionPolicy *policy);

  #endif /* TAO_HAS_RT_CORBA == 1 */

  TAO_Acceptor_Filter *make_filter (CORBA::Environment &ACE_TRY_ENV);
  // Create acceptor filter based on POA's policies.

  void parse_policies (const CORBA::PolicyList &policies,
                       CORBA_Environment &ACE_TRY_ENV);

  const CORBA::PolicyList &client_exposed_fixed_policies (void) const;

protected:

  void parse_policy (const CORBA::Policy_ptr policy,
                     CORBA_Environment &ACE_TRY_ENV);

  void validity_check (CORBA::Environment &ACE_TRY_ENV);

#if (TAO_HAS_RT_CORBA == 1)

  int validate_server_protocol (void);

  void validate_priorities (CORBA::Environment &ACE_TRY_ENV);

#endif /* TAO_HAS_RT_CORBA == 1 */

  PortableServer::ThreadPolicyValue thread_;

  PortableServer::LifespanPolicyValue lifespan_;

  PortableServer::IdUniquenessPolicyValue id_uniqueness_;

  PortableServer::IdAssignmentPolicyValue id_assignment_;

  PortableServer::ImplicitActivationPolicyValue implicit_activation_;

  PortableServer::ServantRetentionPolicyValue servant_retention_;

  PortableServer::RequestProcessingPolicyValue request_processing_;

  PriorityModel priority_model_;

  CORBA::Short server_priority_;

#if (TAO_HAS_RT_CORBA == 1)

  TAO_ServerProtocolPolicy *server_protocol_;
  TAO_PriorityBandedConnectionPolicy *priority_bands_;

#endif /* TAO_HAS_RT_CORBA == 1 */

  CORBA::PolicyList client_exposed_fixed_policies_;

  TAO_ORB_Core &orb_core_;
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

// Forward Declaration
class ServerObject_i;

#if (TAO_HAS_RT_CORBA == 1)
  typedef RTPortableServer::POA TAO_POA_Base;
#else
  typedef PortableServer::POA TAO_POA_Base;
#endif /* TAO_HAS_RT_CORBA == 1 */

class TAO_PortableServer_Export TAO_POA :
  public TAO_POA_Base,
  public TAO_Local_RefCounted_Object
{
public:

  friend class TAO_Object_Adapter;
  friend class TAO_Object_Adapter::Servant_Upcall;
  friend class TAO_Object_Adapter::Non_Servant_Upcall;
  friend class TAO_POA_Current_Impl;
  friend class TAO_POA_Manager;

#if (TAO_HAS_RT_CORBA == 1)

  friend class TAO_Object_Adapter::Priority_Model_Processing;

#endif /* TAO_HAS_RT_CORBA == 1 */

  typedef ACE_CString String;

  PortableServer::POA_ptr create_POA (const char *adapter_name,
                                      PortableServer::POAManager_ptr poa_manager,
                                      const CORBA::PolicyList &policies,
                                      CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::AdapterAlreadyExists,
                     PortableServer::POA::InvalidPolicy));

  PortableServer::POA_ptr find_POA (const char *adapter_name,
                                    CORBA::Boolean activate_it,
                                    CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::AdapterNonExistent));

  void destroy (CORBA::Boolean etherealize_objects,
                CORBA::Boolean wait_for_completion,
                CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

#if (TAO_HAS_MINIMUM_POA == 0)

  PortableServer::ThreadPolicy_ptr create_thread_policy (PortableServer::ThreadPolicyValue value,
                                                         CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

#endif /* TAO_HAS_MINIMUM_POA == 0 */

  PortableServer::LifespanPolicy_ptr create_lifespan_policy (PortableServer::LifespanPolicyValue value,
                                                             CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  PortableServer::IdUniquenessPolicy_ptr create_id_uniqueness_policy (PortableServer::IdUniquenessPolicyValue value,
                                                                      CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  PortableServer::IdAssignmentPolicy_ptr create_id_assignment_policy (PortableServer::IdAssignmentPolicyValue value,
                                                                      CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

#if (TAO_HAS_MINIMUM_POA == 0)

  PortableServer::ImplicitActivationPolicy_ptr create_implicit_activation_policy (PortableServer::ImplicitActivationPolicyValue value,
                                                                                  CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  PortableServer::ServantRetentionPolicy_ptr create_servant_retention_policy (PortableServer::ServantRetentionPolicyValue value,
                                                                              CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  PortableServer::RequestProcessingPolicy_ptr create_request_processing_policy (PortableServer::RequestProcessingPolicyValue value,
                                                                                CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

#endif /* TAO_HAS_MINIMUM_POA == 0 */

  char * the_name (CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  PortableServer::POA_ptr the_parent (CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  PortableServer::POAList *the_children (CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  PortableServer::POAManager_ptr the_POAManager (CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

#if (TAO_HAS_MINIMUM_POA == 0)

  PortableServer::AdapterActivator_ptr the_activator (CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  void the_activator (PortableServer::AdapterActivator_ptr adapter_activator,
                      CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  PortableServer::ServantManager_ptr get_servant_manager (CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::WrongPolicy));

  void set_servant_manager (PortableServer::ServantManager_ptr imgr,
                            CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::WrongPolicy));

  PortableServer::Servant get_servant (CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::NoServant,
                     PortableServer::POA::WrongPolicy));

  void set_servant (PortableServer::Servant servant,
                    CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::WrongPolicy));

#endif /* TAO_HAS_MINIMUM_POA == 0 */

  PortableServer::ObjectId *activate_object (PortableServer::Servant p_servant,
                                             CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::ServantAlreadyActive,
                     PortableServer::POA::WrongPolicy));

  void activate_object_with_id (const PortableServer::ObjectId &id,
                                PortableServer::Servant p_servant,
                                CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::ServantAlreadyActive,
                     PortableServer::POA::ObjectAlreadyActive,
                     PortableServer::POA::WrongPolicy));

  void deactivate_object (const PortableServer::ObjectId &oid,
                          CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::ObjectNotActive,
                     PortableServer::POA::WrongPolicy));

  CORBA::Object_ptr create_reference (const char *intf,
                                      CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::WrongPolicy));

  CORBA::Object_ptr create_reference_with_id (const PortableServer::ObjectId &oid,
                                              const char *intf,
                                              CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::WrongPolicy));

  PortableServer::ObjectId *servant_to_id (PortableServer::Servant p_servant,
                                           CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::ServantNotActive,
                     PortableServer::POA::WrongPolicy));

  CORBA::Object_ptr servant_to_reference (PortableServer::Servant p_servant,
                                          CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::ServantNotActive,
                     PortableServer::POA::WrongPolicy));

  PortableServer::Servant reference_to_servant (CORBA::Object_ptr reference,
                                                CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::ObjectNotActive,
                     PortableServer::POA::WrongAdapter,
                     PortableServer::POA::WrongPolicy));

  PortableServer::ObjectId *reference_to_id (CORBA::Object_ptr reference,
                                             CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::WrongAdapter,
                     PortableServer::POA::WrongPolicy));

  PortableServer::Servant id_to_servant (const PortableServer::ObjectId &oid,
                                         CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::ObjectNotActive,
                     PortableServer::POA::WrongPolicy));

  CORBA::Object_ptr id_to_reference (const PortableServer::ObjectId &oid,
                                     CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::ObjectNotActive,
                     PortableServer::POA::WrongPolicy));

#if (TAO_HAS_RT_CORBA == 1)

  CORBA::Object_ptr create_reference_with_priority (const char * intf,
                                                    RTCORBA::Priority priority,
                                                    CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::WrongPolicy));

  CORBA::Object_ptr create_reference_with_id_and_priority (const PortableServer::ObjectId & oid,
                                                           const char * intf,
                                                           RTCORBA::Priority priority,
                                                           CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::WrongPolicy));

  PortableServer::ObjectId * activate_object_with_priority (PortableServer::Servant p_servant,
                                                            RTCORBA::Priority priority,
                                                            CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::ServantAlreadyActive,
                     PortableServer::POA::WrongPolicy));

  void activate_object_with_id_and_priority (const PortableServer::ObjectId & oid,
                                             PortableServer::Servant p_servant,
                                             RTCORBA::Priority priority,
                                             CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::ServantAlreadyActive,
                     PortableServer::POA::ObjectAlreadyActive,
                     PortableServer::POA::WrongPolicy));

#endif /* TAO_HAS_RT_CORBA == 1 */

  TAO_POA_Policies &policies (void);
  // Accessor for POA policies.

  CORBA::PolicyList *client_exposed_policies (CORBA::Short object_priority,
                                              CORBA_Environment &ACE_TRY_ENV);
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
           const TAO_POA_Policies &policies,
           TAO_POA *parent,
           ACE_Lock &lock,
           TAO_SYNCH_MUTEX &thread_lock,
           TAO_ORB_Core &orb_core,
           TAO_Object_Adapter *object_adapter,
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

  static void check_for_valid_wait_for_completions (const TAO_ORB_Core &orb_core,
                                                    CORBA::Boolean wait_for_completion,
                                                    CORBA::Environment &ACE_TRY_ENV);

  TAO_ORB_Core &orb_core (void) const;
  // ORB Core for POA.

  CORBA::Boolean cleanup_in_progress (void);

  TAO_Object_Adapter &object_adapter (void);

  ACE_Lock &lock (void);


  TAO_Stub* key_to_stub (const TAO_ObjectKey &key,
                         const char *type_id,
                         CORBA::Short priority,
                         CORBA_Environment &ACE_TRY_ENV);
  // Create the correct stub, properly initialized with the attributes
  // and policies attached to the current POA.

protected:

  const ACE_CString &name (void) const;

  PortableServer::POA_ptr create_POA_i (const char *adapter_name,
                                        PortableServer::POAManager_ptr poa_manager,
                                        const CORBA::PolicyList &policies,
                                        CORBA_Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::AdapterAlreadyExists,
                     PortableServer::POA::InvalidPolicy));

  TAO_POA *create_POA_i (const String &adapter_name,
                         TAO_POA_Manager &poa_manager,
                         const TAO_POA_Policies &policies,
                         CORBA_Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::AdapterAlreadyExists,
                     PortableServer::POA::InvalidPolicy));

  TAO_POA *find_POA_i (const ACE_CString &child_name,
                       CORBA::Boolean activate_it,
                       CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::AdapterNonExistent));

  void destroy_i (CORBA::Boolean etherealize_objects,
                  CORBA::Boolean wait_for_completion,
                  CORBA_Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void complete_destruction_i (CORBA::Environment &ACE_TRY_ENV);

  PortableServer::POAList *the_children_i (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));

#if (TAO_HAS_MINIMUM_POA == 0)

  PortableServer::ServantManager_ptr get_servant_manager_i (CORBA_Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::WrongPolicy));

  void set_servant_manager_i (PortableServer::ServantManager_ptr imgr,
                              CORBA_Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::WrongPolicy));

  PortableServer::Servant get_servant_i (CORBA_Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::NoServant,
                     PortableServer::POA::WrongPolicy));

  void set_servant_i (PortableServer::Servant servant,
                      CORBA_Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::WrongPolicy));

#endif /* TAO_HAS_MINIMUM_POA == 0 */

//
// ImplRepo related.
//
#if (TAO_HAS_MINIMUM_CORBA == 0)

  void imr_notify_startup (CORBA_Environment &ACE_TRY_ENV);
  // ImplRepo helper method, notify the ImplRepo on startup

  void imr_notify_shutdown (void);
  // ImplRepo helper method, notify the ImplRepo on shutdown

#endif /* TAO_HAS_MINIMUM_CORBA */

  CORBA::Object_ptr key_to_object (const TAO_ObjectKey &key,
                                   const char *type_id,
                                   TAO_ServantBase *servant,
                                   CORBA::Boolean collocated,
                                   CORBA::Short priority,
                                   CORBA_Environment &ACE_TRY_ENV);
  // Wrapper for the ORB's key_to_object that will alter the object pointer
  // if the ImplRepo is used.

  TAO_Stub* key_to_stub_i (const TAO_ObjectKey &key,
                           const char *type_id,
                           CORBA::Short priority,
                           CORBA_Environment &ACE_TRY_ENV);
  // Like key_to_stub() but assume that the ORB is not shutting down.

  int is_servant_in_map (PortableServer::Servant servant);

  int is_user_id_in_map (const PortableServer::ObjectId &user_id,
                         CORBA::Short priority,
                         int &priorities_match);

  PortableServer::ObjectId *activate_object_i (PortableServer::Servant p_servant,
                                               CORBA::Short priority,
                                               CORBA_Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::ServantAlreadyActive,
                     PortableServer::POA::WrongPolicy));

  void activate_object_with_id_i (const PortableServer::ObjectId &id,
                                  PortableServer::Servant p_servant,
                                  CORBA::Short priority,
                                  CORBA_Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::ServantAlreadyActive,
                     PortableServer::POA::ObjectAlreadyActive,
                     PortableServer::POA::WrongPolicy));

  void deactivate_all_objects_i (CORBA::Boolean etherealize_objects,
                                 CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::WrongPolicy));

  void deactivate_all_objects_i (CORBA::Boolean etherealize_objects,
                                 CORBA::Boolean wait_for_completion,
                                 CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::WrongPolicy));

  void wait_for_completions (CORBA::Boolean wait_for_completion,
                             CORBA::Environment &ACE_TRY_ENV);

  void check_poa_manager_state (CORBA::Environment &ACE_TRY_ENV);

  void deactivate_object_i (const PortableServer::ObjectId &oid,
                            CORBA_Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::ObjectNotActive,
                   PortableServer::POA::WrongPolicy));

  void cleanup_servant (TAO_Active_Object_Map::Map_Entry *active_object_map_entry,
                        CORBA::Environment &ACE_TRY_ENV);

  CORBA::Object_ptr create_reference_i (const char *intf,
                                        CORBA::Short priority,
                                        CORBA_Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::WrongPolicy));

  CORBA::Object_ptr create_reference_with_id_i (const PortableServer::ObjectId &oid,
                                                const char *intf,
                                                CORBA::Short priority,
                                                CORBA_Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::WrongPolicy));

  PortableServer::ObjectId *servant_to_id_i (PortableServer::Servant servant,
                                             CORBA_Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::ServantNotActive,
                     PortableServer::POA::WrongPolicy));

  PortableServer::ObjectId *servant_to_system_id (PortableServer::Servant p_servant,
                                                  CORBA::Short &priority,
                                                  CORBA_Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::ServantNotActive,
                     PortableServer::POA::WrongPolicy));

  PortableServer::ObjectId *servant_to_system_id_i (PortableServer::Servant p_servant,
                                                    CORBA::Short &priority,
                                                    CORBA_Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::ServantNotActive,
                     PortableServer::POA::WrongPolicy));

  PortableServer::Servant id_to_servant_i (const PortableServer::ObjectId &oid,
                                           CORBA_Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::ObjectNotActive,
                     PortableServer::POA::WrongPolicy));

  CORBA::Object_ptr id_to_reference_i (const PortableServer::ObjectId &oid,
                                       CORBA_Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POA::ObjectNotActive,
                     PortableServer::POA::WrongPolicy));

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

#if (TAO_HAS_RT_CORBA == 1)

  int valid_priority (RTCORBA::Priority priority);

  void validate_policies (CORBA::Environment &ACE_TRY_ENV);


#endif /* TAO_HAS_RT_CORBA == 1 */

protected:

  TAO_SERVANT_LOCATION locate_servant_i (const PortableServer::ObjectId &id,
                                         PortableServer::Servant &servant,
                                         CORBA_Environment &ACE_TRY_ENV);

  PortableServer::Servant locate_servant_i (const char *operation,
                                            const PortableServer::ObjectId &id,
                                            TAO_Object_Adapter::Servant_Upcall &servant_upcall,
                                            TAO_POA_Current_Impl &poa_current_impl,
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

  TAO_SYNCH_RECURSIVE_MUTEX &single_threaded_lock (void) const;

  CORBA::Boolean waiting_destruction (void) const;

  String name_;

  TAO_Object_Adapter::poa_name folded_name_;

  TAO_Object_Adapter::poa_name_var system_name_;

  TAO_POA_Manager &poa_manager_;

  TAO_POA_Policies policies_;

  TAO_Acceptor_Filter *acceptor_filter_;
  // Strategy (selected based on POA policies) for deciding which
  // endpoints get embedded into object's IOR.

  TAO_POA *parent_;

  TAO_Active_Object_Map *active_object_map_;

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

  int persistent_;

  int system_id_;

  TAO_Creation_Time creation_time_;

  TAO_ORB_Core &orb_core_;

  TAO_Object_Adapter *object_adapter_;
  // The object adapter we belong to

  CORBA::Boolean cleanup_in_progress_;

  CORBA::Boolean etherealize_objects_;

  CORBA::ULong outstanding_requests_;

  TAO_SYNCH_CONDITION outstanding_requests_condition_;

  CORBA::Boolean wait_for_completion_pending_;

  CORBA::Boolean waiting_destruction_;

  TAO_SYNCH_CONDITION servant_deactivation_condition_;

  CORBA::ULong waiting_servant_deactivation_;

  TAO_SYNCH_RECURSIVE_MUTEX *single_threaded_lock_;
};


class TAO_POA_Guard
{
public:
  TAO_POA_Guard (TAO_POA &poa,
                 CORBA::Environment &ACE_TRY_ENV,
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
                                  const char *name,
                                  CORBA_Environment &ACE_TRY_ENV)
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
