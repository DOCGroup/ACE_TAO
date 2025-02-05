// -*- C++ -*-

//=============================================================================
/**
 *  @file    RT_POA.h
 *
 *  RT_POA
 *
 *  @author  Irfan Pyarali
 */
// ===================================================================

#ifndef TAO_RT_POA_H
#define TAO_RT_POA_H

#include /**/ "ace/pre.h"

#include "tao/RTPortableServer/rtportableserver_export.h"

#include "tao/PortableServer/Regular_POA.h"
#include "tao/orbconf.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#define TAO_RT_PORTABLESERVER_SAFE_INCLUDE
#include "tao/RTPortableServer/RTPortableServerC.h"
#undef TAO_RT_PORTABLESERVER_SAFE_INCLUDE

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// This is to remove "inherits via dominance" warnings from MSVC.
// MSVC is being a little too paranoid.
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Thread_Pool;
class TAO_Thread_Lane;
class TAO_PriorityBandedConnectionPolicy;

class TAO_RTPortableServer_Export TAO_RT_POA
  : public virtual RTPortableServer::POA
  , public virtual TAO_Regular_POA
{
public:
  // RTCORBA specific methods
  CORBA::Object_ptr create_reference_with_priority (const char * intf,
                                                    RTCORBA::Priority priority) override;

#if !defined (CORBA_E_MICRO)
  CORBA::Object_ptr create_reference_with_id_and_priority (const PortableServer::ObjectId & oid,
                                                           const char * intf,
                                                           RTCORBA::Priority priority) override;
#endif

  PortableServer::ObjectId * activate_object_with_priority (PortableServer::Servant p_servant,
                                                            RTCORBA::Priority priority) override;

#if !defined (CORBA_E_MICRO)
  void activate_object_with_id_and_priority (const PortableServer::ObjectId & oid,
                                             PortableServer::Servant p_servant,
                                             RTCORBA::Priority priority) override;
#endif
  // End RTCORBA specific methods

  // Standard POA interface methods
  PortableServer::POA_ptr create_POA (const char *adapter_name,
                                      PortableServer::POAManager_ptr poa_manager,
                                      const CORBA::PolicyList &policies) override;

  PortableServer::POA_ptr find_POA (const char *adapter_name,
                                    CORBA::Boolean activate_it) override;

  void destroy (CORBA::Boolean etherealize_objects,
                CORBA::Boolean wait_for_completion) override;

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)

  PortableServer::ThreadPolicy_ptr create_thread_policy (
    PortableServer::ThreadPolicyValue value) override;

#endif /* TAO_HAS_MINIMUM_POA == 0 */

#if !defined (CORBA_E_MICRO)
  PortableServer::LifespanPolicy_ptr create_lifespan_policy (
    PortableServer::LifespanPolicyValue value) override;
#endif

#if !defined (CORBA_E_MICRO)
  PortableServer::IdUniquenessPolicy_ptr create_id_uniqueness_policy (
    PortableServer::IdUniquenessPolicyValue value) override;
#endif

#if !defined (CORBA_E_MICRO)
  PortableServer::IdAssignmentPolicy_ptr create_id_assignment_policy (
    PortableServer::IdAssignmentPolicyValue value) override;
#endif

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)
  PortableServer::ImplicitActivationPolicy_ptr
    create_implicit_activation_policy (
      PortableServer::ImplicitActivationPolicyValue value) override;

  PortableServer::ServantRetentionPolicy_ptr
    create_servant_retention_policy (
      PortableServer::ServantRetentionPolicyValue value) override;

  PortableServer::RequestProcessingPolicy_ptr
    create_request_processing_policy (
      PortableServer::RequestProcessingPolicyValue value) override;

#endif /* TAO_HAS_MINIMUM_POA == 0 && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO) */

  char * the_name () override;

  PortableServer::POA_ptr the_parent () override;

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)
  PortableServer::POAList *the_children () override;
#endif /* TAO_HAS_MINIMUM_POA == 0 */

  PortableServer::POAManager_ptr the_POAManager () override;

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)
  PortableServer::AdapterActivator_ptr the_activator () override;

  void the_activator (PortableServer::AdapterActivator_ptr adapter_activator) override;

  PortableServer::ServantManager_ptr get_servant_manager () override;

  void set_servant_manager (PortableServer::ServantManager_ptr imgr) override;

  PortableServer::Servant get_servant () override;

  void set_servant (PortableServer::Servant servant) override;
#endif /* TAO_HAS_MINIMUM_POA == 0 */

  PortableServer::ObjectId *activate_object (PortableServer::Servant p_servant) override;

#if !defined (CORBA_E_MICRO)
  void activate_object_with_id (const PortableServer::ObjectId &id,
                                PortableServer::Servant p_servant) override;
#endif

  void deactivate_object (const PortableServer::ObjectId &oid) override;

  CORBA::Object_ptr create_reference (const char *intf) override;

#if !defined (CORBA_E_MICRO)
  CORBA::Object_ptr create_reference_with_id (const PortableServer::ObjectId &oid,
                                              const char *intf) override;
#endif

  PortableServer::ObjectId *servant_to_id (PortableServer::Servant p_servant) override;

  CORBA::Object_ptr servant_to_reference (PortableServer::Servant p_servant) override;

  PortableServer::Servant reference_to_servant (CORBA::Object_ptr reference) override;

  PortableServer::ObjectId *reference_to_id (CORBA::Object_ptr reference) override;

  PortableServer::Servant id_to_servant (const PortableServer::ObjectId &oid) override;

  CORBA::Object_ptr id_to_reference (const PortableServer::ObjectId &oid) override;

  CORBA::OctetSeq *id () override;

  // End standard POA interface methods.

  CORBA::PolicyList *client_exposed_policies (CORBA::Short object_priority) override;

  TAO_RT_POA (const String &name,
              PortableServer::POAManager_ptr poa_manager,
              const TAO_POA_Policy_Set &policies,
              TAO_Root_POA *parent,
              ACE_Lock &lock,
              TAO_SYNCH_MUTEX &thread_lock,
              TAO_ORB_Core &orb_core,
              TAO_Object_Adapter *object_adapter);

  ~TAO_RT_POA () override = default;

  void *thread_pool () const override;

protected:
  /// Template method for creating new POA's of this type.
  TAO_Root_POA *new_POA (const String &name,
                         PortableServer::POAManager_ptr poa_manager,
                         const TAO_POA_Policy_Set &policies,
                         TAO_Root_POA *parent,
                         ACE_Lock &lock,
                         TAO_SYNCH_MUTEX &thread_lock,
                         TAO_ORB_Core &orb_core,
                         TAO_Object_Adapter *object_adapter) override;

  TAO_Stub *key_to_stub_i (const TAO::ObjectKey &key,
                           const char *type_id,
                           CORBA::Short priority) override;

  void validate_priority (RTCORBA::Priority priority);

  void validate_policies ();

  /// Parse the policy list for RT policies and update our
  /// cached policy instance.
  void parse_rt_policies (TAO_POA_Policy_Set &policies);

  size_t endpoint_count ();

  TAO_Stub *create_stub_object (const TAO::ObjectKey &object_key,
                                const char *type_id,
                                CORBA::PolicyList *policy_list,
                                TAO_Acceptor_Filter *filter,
                                TAO_PriorityBandedConnectionPolicy *priority_bands);

  int lane_required (TAO_Thread_Lane *lane,
                     TAO_PriorityBandedConnectionPolicy *priority_bands);

  TAO_Thread_Pool *thread_pool_ {};
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#if defined (__ACE_INLINE__)
# include "tao/RTPortableServer/RT_POA.inl"
#endif /* __ACE_INLINE__ */

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */

#include /**/ "ace/post.h"

#endif /* TAO_POA_H */
