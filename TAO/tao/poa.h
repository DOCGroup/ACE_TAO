// -*- C++ -*-

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    poa.h
//
// = DESCRIPTION
//     POA
//
// = AUTHOR
//     Irfan Pyarali
//
// ============================================================================

#if !defined (POA_H)
#define POA_H

// CORBA
#include "tao/corba.h"

// Stubs
#include "tao/poaC.h"

// Servant
#include "tao/servant_base.h"

// Skeletons
#include "tao/poaS.h"

// String
#include "ace/SString.h"
//#include "string"

// Map
//#include "map"
#include "ace/Map_Manager.h"

// Vector
//#include "vector"
#include "ace/Containers.h"

// Locking
#include "ace/Synch.h"

// Active Object Table
#include "tao/objtable.h"

class TAO_POA;
class TAO_POA_Manager;

class TAO_Export TAO_Thread_Policy : public POA_PortableServer::ThreadPolicy
{
public:
  TAO_Thread_Policy (PortableServer::ThreadPolicyValue value);

  TAO_Thread_Policy (const TAO_Thread_Policy &new_policy);

  virtual PortableServer::ThreadPolicyValue value (CORBA::Environment &env);

  virtual PortableServer::Policy_ptr copy (CORBA::Environment &env);

  virtual void destroy (CORBA::Environment &env);

protected:
  PortableServer::ThreadPolicyValue value_;
};

class TAO_Export TAO_Lifespan_Policy : public POA_PortableServer::LifespanPolicy
{
public:
  TAO_Lifespan_Policy (PortableServer::LifespanPolicyValue value);

  TAO_Lifespan_Policy (const TAO_Lifespan_Policy &rhs);

  virtual PortableServer::LifespanPolicyValue value (CORBA::Environment &env);

  virtual PortableServer::Policy_ptr copy (CORBA::Environment &env);

  virtual void destroy (CORBA::Environment &env);

protected:
  PortableServer::LifespanPolicyValue value_;
};

class TAO_Export TAO_Id_Uniqueness_Policy : public POA_PortableServer::IdUniquenessPolicy
{
public:
  TAO_Id_Uniqueness_Policy (PortableServer::IdUniquenessPolicyValue value);

  TAO_Id_Uniqueness_Policy (const TAO_Id_Uniqueness_Policy &rhs);

  virtual PortableServer::IdUniquenessPolicyValue value (CORBA::Environment &env);

  virtual PortableServer::Policy_ptr copy (CORBA::Environment &env);

  virtual void destroy (CORBA::Environment &env);

protected:
  PortableServer::IdUniquenessPolicyValue value_;
};

class TAO_Export TAO_Id_Assignment_Policy : public POA_PortableServer::IdAssignmentPolicy
{
public:
  TAO_Id_Assignment_Policy (PortableServer::IdAssignmentPolicyValue value);

  TAO_Id_Assignment_Policy (const TAO_Id_Assignment_Policy &rhs);

  virtual PortableServer::IdAssignmentPolicyValue value (CORBA::Environment &env);

  virtual PortableServer::Policy_ptr copy (CORBA::Environment &env);

  virtual void destroy (CORBA::Environment &env);

protected:
  PortableServer::IdAssignmentPolicyValue value_;
};

class TAO_Export TAO_Implicit_Activation_Policy : public POA_PortableServer::ImplicitActivationPolicy
{
public:
  TAO_Implicit_Activation_Policy (PortableServer::ImplicitActivationPolicyValue value);

  TAO_Implicit_Activation_Policy (const TAO_Implicit_Activation_Policy &rhs);

  virtual PortableServer::ImplicitActivationPolicyValue value (CORBA::Environment &env);

  virtual PortableServer::Policy_ptr copy (CORBA::Environment &env);

  virtual void destroy (CORBA::Environment &env);

protected:
  PortableServer::ImplicitActivationPolicyValue value_;
};

class TAO_Export TAO_Servant_Retention_Policy : public POA_PortableServer::ServantRetentionPolicy
{
public:
  TAO_Servant_Retention_Policy (PortableServer::ServantRetentionPolicyValue value);

  TAO_Servant_Retention_Policy (const TAO_Servant_Retention_Policy &rhs);

  virtual PortableServer::ServantRetentionPolicyValue value (CORBA::Environment &env);

  virtual PortableServer::Policy_ptr copy (CORBA::Environment &env);

  virtual void destroy (CORBA::Environment &env);

protected:
  PortableServer::ServantRetentionPolicyValue value_;
};

class TAO_Export TAO_Request_Processing_Policy : public POA_PortableServer::RequestProcessingPolicy
{
public:
  TAO_Request_Processing_Policy (PortableServer::RequestProcessingPolicyValue value);

  TAO_Request_Processing_Policy (const TAO_Request_Processing_Policy &rhs);

  virtual PortableServer::RequestProcessingPolicyValue value (CORBA::Environment &env);

  virtual PortableServer::Policy_ptr copy (CORBA::Environment &env);

  virtual void destroy (CORBA::Environment &env);

protected:
  PortableServer::RequestProcessingPolicyValue value_;
};

class TAO_Export TAO_POA_Policies
{
public:

  TAO_POA_Policies (void);

  virtual PortableServer::ThreadPolicyValue thread (void) const;
  virtual void thread (PortableServer::ThreadPolicyValue value);

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

  virtual void parse_policies (const PortableServer::PolicyList &policies,
                               CORBA::Environment &env);

protected:

  virtual void parse_policy (const PortableServer::Policy_ptr policy,
                             CORBA::Environment &env);

  virtual int validity_check (void);

  PortableServer::ThreadPolicyValue thread_;

  PortableServer::LifespanPolicyValue lifespan_;

  PortableServer::IdUniquenessPolicyValue id_uniqueness_;

  PortableServer::IdAssignmentPolicyValue id_assignment_;

  PortableServer::ImplicitActivationPolicyValue implicit_activation_;

  PortableServer::ServantRetentionPolicyValue servant_retention_;

  PortableServer::RequestProcessingPolicyValue request_processing_;
};

class TAO_Export TAO_POA : public POA_PortableServer::POA
{
public:

  typedef ACE_CString String;
  //typedef std::string String;

  virtual PortableServer::POA_ptr create_POA (const char *adapter_name,
                                              PortableServer::POAManager_ptr poa_manager,
                                              const PortableServer::PolicyList &policies,
                                              CORBA::Environment &env);

  virtual PortableServer::POA_ptr find_POA (const char *adapter_name,
                                            CORBA::Boolean activate_it,
                                            CORBA::Environment &env);

  virtual void destroy (CORBA::Boolean etherealize_objects,
                        CORBA::Boolean wait_for_completion,
                        CORBA::Environment &env);

  virtual PortableServer::ThreadPolicy_ptr create_thread_policy (PortableServer::ThreadPolicyValue value,
                                                                 CORBA::Environment &env);

  virtual PortableServer::LifespanPolicy_ptr create_lifespan_policy (PortableServer::LifespanPolicyValue value,
                                                                     CORBA::Environment &env);

  virtual PortableServer::IdUniquenessPolicy_ptr create_id_uniqueness_policy (PortableServer::IdUniquenessPolicyValue value,
                                                                              CORBA::Environment &env);

  virtual PortableServer::IdAssignmentPolicy_ptr create_id_assignment_policy (PortableServer::IdAssignmentPolicyValue value,
                                                                              CORBA::Environment &env);

  virtual PortableServer::ImplicitActivationPolicy_ptr create_implicit_activation_policy (PortableServer::ImplicitActivationPolicyValue value,
                                                                                          CORBA::Environment &env);

  virtual PortableServer::ServantRetentionPolicy_ptr create_servant_retention_policy (PortableServer::ServantRetentionPolicyValue value,
                                                                                      CORBA::Environment &env);

  virtual PortableServer::RequestProcessingPolicy_ptr create_request_processing_policy (PortableServer::RequestProcessingPolicyValue value,
                                                                                        CORBA::Environment &env);

  virtual CORBA::String the_name (CORBA::Environment &env);

  virtual PortableServer::POA_ptr the_parent (CORBA::Environment &env);

  virtual PortableServer::POAManager_ptr the_POAManager (CORBA::Environment &env);

  virtual PortableServer::AdapterActivator_ptr the_activator (CORBA::Environment &env);

  virtual void the_activator (PortableServer::AdapterActivator_ptr adapter_activator,
                              CORBA::Environment &env);

  virtual PortableServer::ServantManager_ptr get_servant_manager (CORBA::Environment &env);

  virtual void set_servant_manager (PortableServer::ServantManager_ptr imgr,
                                    CORBA::Environment &env);

  virtual PortableServer::Servant get_servant (CORBA::Environment &env);

  virtual void set_servant (PortableServer::Servant servant,
                            CORBA::Environment &env);

  virtual PortableServer::ObjectId *activate_object (PortableServer::Servant p_servant,
                                                     CORBA::Environment &env);

  virtual void activate_object_with_id (const PortableServer::ObjectId &id,
                                        PortableServer::Servant p_servant,
                                        CORBA::Environment &env);

  virtual void deactivate_object (const PortableServer::ObjectId &oid,
                                  CORBA::Environment &env);

  virtual CORBA::Object_ptr create_reference (const char *intf,
                                              CORBA::Environment &env);

  virtual CORBA::Object_ptr create_reference_with_id (const PortableServer::ObjectId &oid,
                                                      const char *intf,
                                                      CORBA::Environment &env);
  virtual PortableServer::ObjectId *servant_to_id (PortableServer::Servant p_servant,
                                                   CORBA::Environment &env);

  virtual CORBA::Object_ptr servant_to_reference (PortableServer::Servant p_servant,
                                                  CORBA::Environment &env);

  virtual PortableServer::Servant reference_to_servant (CORBA::Object_ptr reference,
                                                        CORBA::Environment &env);

  virtual PortableServer::ObjectId *reference_to_id (CORBA::Object_ptr reference,
                                                     CORBA::Environment &env);

  virtual PortableServer::Servant id_to_servant (const PortableServer::ObjectId &oid,
                                                 CORBA::Environment &env);

  virtual CORBA::Object_ptr id_to_reference (const PortableServer::ObjectId &oid,
                                             CORBA::Environment &env);

  static char *ObjectId_to_string (const PortableServer::ObjectId &id);

  static wchar_t *ObjectId_to_wstring (const PortableServer::ObjectId &id);

  static PortableServer::ObjectId *string_to_ObjectId (const char *id);

  static PortableServer::ObjectId *wstring_to_ObjectId (const wchar_t *id);

  TAO_POA (const String &adapter_name,
           TAO_POA_Manager &poa_manager,
           const TAO_POA_Policies &policies,
           TAO_POA *parent,
           CORBA::Environment &env);

  TAO_POA (const String &adapter_name,
           TAO_POA_Manager &poa_manager,
           const TAO_POA_Policies &policies,
           TAO_POA *parent,
           TAO_Object_Table &active_object_map,
           CORBA::Environment &env);

  virtual TAO_POA *clone (const String &adapter_name,
                          TAO_POA_Manager &poa_manager,
                          const TAO_POA_Policies &policies,
                          TAO_POA *parent,
                          CORBA::Environment &env);

  virtual TAO_POA *clone (const String &adapter_name,
                          TAO_POA_Manager &poa_manager,
                          const TAO_POA_Policies &policies,
                          TAO_POA *parent,
                          TAO_Object_Table &active_object_map,
                          CORBA::Environment &env);

  virtual ~TAO_POA (void);

  virtual void dispatch_servant (const TAO::ObjectKey &key,
                                 CORBA::ServerRequest &req,
                                 void *context,
                                 CORBA::Environment &env);

  virtual int locate_servant (const TAO::ObjectKey &key,
                              CORBA::Environment &env);

  static char name_separator (void);

  static char id_separator (void);

  static CORBA::ULong name_separator_length (void);

  static CORBA::ULong id_separator_length (void);

protected:

  virtual TAO_POA *create_POA (const String &adapter_name,
                               TAO_POA_Manager &poa_manager,
                               const TAO_POA_Policies &policies,
                               CORBA::Environment &env);

  virtual TAO_POA *create_POA_i (const String &adapter_name,
                                 TAO_POA_Manager &poa_manager,
                                 const TAO_POA_Policies &policies,
                                 CORBA::Environment &env);

  virtual TAO_POA *find_POA (const String &adapter_name,
                             CORBA::Boolean activate_it,
                             CORBA::Environment &env);

  virtual TAO_POA *find_POA_i (const String &adapter_name,
                               CORBA::Boolean activate_it,
                               CORBA::Environment &env);

  virtual TAO_POA *find_POA_i_optimized (const String &adapter_name,
                                         CORBA::Boolean activate_it,
                                         CORBA::Environment &env);

  virtual void destroy_i (CORBA::Boolean etherealize_objects,
                          CORBA::Boolean wait_for_completion,
                          CORBA::Environment &env);

  virtual PortableServer::ServantManager_ptr get_servant_manager_i (CORBA::Environment &env);

  virtual void set_servant_manager_i (PortableServer::ServantManager_ptr imgr,
                                      CORBA::Environment &env);

  virtual PortableServer::Servant get_servant_i (CORBA::Environment &env);

  virtual void set_servant_i (PortableServer::Servant servant,
                              CORBA::Environment &env);

  virtual PortableServer::ObjectId *activate_object_i (PortableServer::Servant p_servant,
                                                       CORBA::Environment &env);

  virtual void activate_object_with_id_i (const PortableServer::ObjectId &id,
                                          PortableServer::Servant p_servant,
                                          CORBA::Environment &env);

  virtual void deactivate_object_i (const PortableServer::ObjectId &oid,
                                    CORBA::Environment &env);

  virtual CORBA::Object_ptr create_reference_i (const char *intf,
                                                CORBA::Environment &env);

  virtual PortableServer::ObjectId *servant_to_id_i (PortableServer::Servant servant,
                                                     CORBA::Environment &env);

  virtual PortableServer::Servant id_to_servant_i (const PortableServer::ObjectId &oid,
                                                   CORBA::Environment &env);

  virtual CORBA::Object_ptr id_to_reference_i (const PortableServer::ObjectId &oid,
                                               CORBA::Environment &env);

  virtual ACE_Lock &lock (void);

  virtual TAO_POA_Policies &policies (void);

  virtual TAO_Object_Table &active_object_map (void);

  virtual void delete_child (const String &child,
                             CORBA::Environment &env);

  virtual void delete_child_i (const String &child,
                               CORBA::Environment &env);

  virtual String complete_name (void);

  virtual void set_complete_name (void);

  virtual int leaf_poa_name (const String &adapter_name,
                             CORBA::Environment &env);

  virtual void parse_poa_name (const TAO_POA::String &adapter_name,
                               TAO_POA::String &topmost_poa_name,
                               TAO_POA::String &tail_poa_name,
                               CORBA::Environment &env);

  virtual PortableServer::ObjectId *create_object_id (void);

  virtual TAO::ObjectKey *create_object_key (const PortableServer::ObjectId &id);

  virtual int is_poa_generated_id (const PortableServer::ObjectId &id);

  virtual int is_poa_generated_key (const TAO::ObjectKey &key);

  virtual int parse_key (const TAO::ObjectKey &key,
                         String &poa_name,
                         PortableServer::ObjectId_out id,
                         CORBA::Boolean &persistent,
                         ACE_Time_Value &poa_creation_time);

  virtual int locate_servant_i (const TAO::ObjectKey &key,
                                CORBA::Environment &env);

  virtual PortableServer::Servant locate_poa_and_servant_i (const TAO::ObjectKey &key,
                                                            const char *operation,
                                                            PortableServer::ObjectId_out id,
                                                            TAO_POA *&poa_impl,
                                                            CORBA::Environment &env);

  virtual TAO_POA *locate_poa_i (const TAO::ObjectKey &key,
                                 PortableServer::ObjectId_out id,
                                 CORBA::Environment &env);

  virtual void dispatch_servant_i (const TAO::ObjectKey &key,
                                   CORBA::ServerRequest &req,
                                   void *context,
                                   CORBA::Environment &env);

  virtual void pre_invoke (const TAO::ObjectKey &key,
                           const PortableServer::ObjectId &id,
                           PortableServer::Servant servant,
                           CORBA::Environment &env);

  virtual void post_invoke (PortableServer::Servant servant,
                            const char *operation,
                            CORBA::Environment &env);

  virtual CORBA::Boolean persistent (void);

  virtual const ACE_Time_Value &creation_time (void);

  virtual const String &object_key_type (void);

  static const String &persistent_key_type (void);

  static const String &transient_key_type (void);

  static const char *ObjectId_to_const_string (const PortableServer::ObjectId &id);

  static const wchar_t *ObjectId_to_const_wstring (const PortableServer::ObjectId &id);

  static const char *ObjectKey_to_const_string (const TAO::ObjectKey &key);

  String name_;

  String complete_name_;

  TAO_POA_Manager &poa_manager_;

  TAO_POA_Policies policies_;

  TAO_POA *parent_;

  TAO_Object_Table *active_object_map_;

  int delete_active_object_map_;

  PortableServer::AdapterActivator_var adapter_activator_;

  PortableServer::ServantActivator_var servant_activator_;

  PortableServer::ServantLocator_var servant_locator_;

  PortableServer::Servant default_servant_;

  typedef ACE_Map_Manager<String, TAO_POA *, ACE_Null_Mutex> CHILDREN;

  CHILDREN children_;

  ACE_Lock_Adapter<ACE_Null_Mutex> lock_;

  int closing_down_;

  CORBA::ULong counter_;

  ACE_Time_Value creation_time_;

  static const int max_space_required_for_ulong;
};

class TAO_Export TAO_POA_Manager : public POA_PortableServer::POAManager
{
  friend class TAO_POA;

public:
  enum Processing_State
  {
    ACTIVE,
    DISCARDING,
    HOLDING,
    INACTIVE,
    UNKNOWN
  };

  virtual void activate (CORBA::Environment &env);

  virtual void hold_requests (CORBA::Boolean wait_for_completion,
                              CORBA::Environment &env);

  virtual void discard_requests (CORBA::Boolean wait_for_completion,
                                 CORBA::Environment &env);

  virtual void deactivate (CORBA::Boolean etherealize_objects,
                           CORBA::Boolean wait_for_completion,
                           CORBA::Environment &env);

  TAO_POA_Manager (void);

  virtual TAO_POA_Manager *clone (void);

  virtual ~TAO_POA_Manager (void);

  virtual Processing_State state (CORBA::Environment &env);

protected:

  virtual ACE_Lock &lock (void);

  virtual void remove_poa (TAO_POA *poa,
                           CORBA::Environment &env);

  virtual void remove_poa_i (TAO_POA *poa,
                             CORBA::Environment &env);

  virtual void register_poa (TAO_POA *poa,
                             CORBA::Environment &env);

  virtual void register_poa_i (TAO_POA *poa,
                               CORBA::Environment &env);

  Processing_State state_;

  int closing_down_;

  ACE_Lock_Adapter<ACE_Null_Mutex> lock_;

  typedef ACE_Unbounded_Set<TAO_POA *> POA_COLLECTION;

  POA_COLLECTION poa_collection_;
};

class TAO_Export TAO_Adapter_Activator : public POA_PortableServer::AdapterActivator
{
public:

  virtual CORBA::Boolean unknown_adapter (PortableServer::POA_ptr parent,
                                          const char *name,
                                          CORBA::Environment &env);
};

class TAO_Export TAO_Strategy_POA : public TAO_POA
{
public:

  TAO_Strategy_POA (const String &adapter_name,
                    TAO_POA_Manager &poa_manager,
                    const TAO_POA_Policies &policies,
                    TAO_POA *parent,
                    CORBA::Environment &env);

  TAO_Strategy_POA (const String &adapter_name,
                    TAO_POA_Manager &poa_manager,
                    const TAO_POA_Policies &policies,
                    TAO_POA *parent,
                    TAO_Object_Table &active_object_map,
                    CORBA::Environment &env);

  virtual TAO_POA *clone (const String &adapter_name,
                          TAO_POA_Manager &poa_manager,
                          const TAO_POA_Policies &policies,
                          TAO_POA *parent,
                          CORBA::Environment &env);

  virtual TAO_POA *clone (const String &adapter_name,
                          TAO_POA_Manager &poa_manager,
                          const TAO_POA_Policies &policies,
                          TAO_POA *parent,
                          TAO_Object_Table &active_object_map,
                          CORBA::Environment &env);

  virtual ~TAO_Strategy_POA (void);

protected:

  virtual ACE_Lock &lock (void);

  ACE_Lock *lock_;

  typedef TAO_Strategy_POA SELF;
};

class TAO_Export TAO_Strategy_POA_Manager : public TAO_POA_Manager
{
public:

  TAO_Strategy_POA_Manager (void);

  virtual TAO_POA_Manager *clone (void);

  virtual ~TAO_Strategy_POA_Manager (void);

protected:

  virtual ACE_Lock &lock (void);

  ACE_Lock *lock_;

  typedef TAO_Strategy_POA_Manager SELF;
};

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
  // = Specification-mandated methods

  virtual PortableServer::POA_ptr get_POA (CORBA::Environment &env);
  // Return pointer to the invoking POA.  Raises the
  // <CORBA::NoContext> exception.

  virtual PortableServer::ObjectId *get_object_id (CORBA::Environment &env);
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

  virtual void object_key (const TAO::ObjectKey &key);
  // Set the object key.

  virtual const TAO::ObjectKey &object_key (void) const;
  // Get the object key.

  virtual void servant (PortableServer::Servant servant);
  // Set the servant for the current upcall.

  virtual PortableServer::Servant servant (void) const;
  // Get the servant for the current upcall.

  virtual int in_upcall (void) const;
  // Get whether we're in an upcall (non-zero is yes).

  virtual PortableServer::ServantLocator::Cookie locator_cookie (void) const;
  // Get the Servant Locator's cookie

  virtual void locator_cookie (PortableServer::ServantLocator::Cookie cookie);
  // Set the Servant Locator's cookie

  TAO_POA_Current (void);
  // Constructor

  virtual ~TAO_POA_Current (void);
  // Destructor

private:
  TAO_POA *poa_impl_;
  // The POA implementation invoking an upcall

  const PortableServer::ObjectId *object_id_;
  // The object ID of the current context.

  const TAO::ObjectKey *object_key_;
  // The object key of the current context.

  PortableServer::ServantLocator::Cookie cookie_;
  // Servant Locator's cookie

  PortableServer::Servant servant_;
  // The servant for the current upcall.

  // = Hidden because we don't allow these
  TAO_POA_Current (const TAO_POA_Current &);
  void operator= (const TAO_POA_Current &);
};

// Include the templates here.
#include "tao/poa_T.h"

#endif /* POA_H */
