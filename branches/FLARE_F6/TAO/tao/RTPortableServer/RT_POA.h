// -*- C++ -*-

//=============================================================================
/**
 *  @file    RT_POA.h
 *
 *  $Id$
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
                                                    RTCORBA::Priority priority);

#if !defined (CORBA_E_MICRO)
  CORBA::Object_ptr create_reference_with_id_and_priority (const PortableServer::ObjectId & oid,
                                                           const char * intf,
                                                           RTCORBA::Priority priority);
#endif

  PortableServer::ObjectId * activate_object_with_priority (PortableServer::Servant p_servant,
                                                            RTCORBA::Priority priority);

#if !defined (CORBA_E_MICRO)
  void activate_object_with_id_and_priority (const PortableServer::ObjectId & oid,
                                             PortableServer::Servant p_servant,
                                             RTCORBA::Priority priority);
#endif
  // End RTCORBA specific methods

  // Standard POA interface methods
  PortableServer::POA_ptr create_POA (const char *adapter_name,
                                      PortableServer::POAManager_ptr poa_manager,
                                      const CORBA::PolicyList &policies);

  PortableServer::POA_ptr find_POA (const char *adapter_name,
                                    CORBA::Boolean activate_it);

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

  PortableServer::AdapterActivator_ptr the_activator (void);

  void the_activator (PortableServer::AdapterActivator_ptr adapter_activator);

  PortableServer::ServantManager_ptr get_servant_manager (void);

  void set_servant_manager (PortableServer::ServantManager_ptr imgr);

  PortableServer::Servant get_servant (void);

  void set_servant (PortableServer::Servant servant);

#endif /* TAO_HAS_MINIMUM_POA == 0 */

  PortableServer::ObjectId *activate_object (PortableServer::Servant p_servant);

#if !defined (CORBA_E_MICRO)
  void activate_object_with_id (const PortableServer::ObjectId &id,
                                PortableServer::Servant p_servant);
#endif

  void deactivate_object (const PortableServer::ObjectId &oid);

  CORBA::Object_ptr create_reference (const char *intf);

#if !defined (CORBA_E_MICRO)
  CORBA::Object_ptr create_reference_with_id (const PortableServer::ObjectId &oid,
                                              const char *intf);
#endif

  PortableServer::ObjectId *servant_to_id (PortableServer::Servant p_servant);

  CORBA::Object_ptr servant_to_reference (PortableServer::Servant p_servant);

  PortableServer::Servant reference_to_servant (CORBA::Object_ptr reference);

  PortableServer::ObjectId *reference_to_id (CORBA::Object_ptr reference);

  PortableServer::Servant id_to_servant (const PortableServer::ObjectId &oid);

  CORBA::Object_ptr id_to_reference (const PortableServer::ObjectId &oid);

  CORBA::OctetSeq *id (void);

  // End standard POA interface methods.

  virtual CORBA::PolicyList *client_exposed_policies (CORBA::Short object_priority);

  TAO_RT_POA (const String &name,
              PortableServer::POAManager_ptr poa_manager,
              const TAO_POA_Policy_Set &policies,
              TAO_Root_POA *parent,
              ACE_Lock &lock,
              TAO_SYNCH_MUTEX &thread_lock,
              TAO_ORB_Core &orb_core,
              TAO_Object_Adapter *object_adapter);

  virtual ~TAO_RT_POA (void);

  void *thread_pool (void) const;

protected:

  /// Template method for creating new POA's of this type.
  virtual TAO_Root_POA *new_POA (const String &name,
                                 PortableServer::POAManager_ptr poa_manager,
                                 const TAO_POA_Policy_Set &policies,
                                 TAO_Root_POA *parent,
                                 ACE_Lock &lock,
                                 TAO_SYNCH_MUTEX &thread_lock,
                                 TAO_ORB_Core &orb_core,
                                 TAO_Object_Adapter *object_adapter);

  virtual TAO_Stub *key_to_stub_i (const TAO::ObjectKey &key,
                                   const char *type_id,
                                   CORBA::Short priority);

  void validate_priority (RTCORBA::Priority priority);

  void validate_policies (void);

  /// Parse the policy list for RT policies and update our
  /// cached policy instance.
  void parse_rt_policies (TAO_POA_Policy_Set &policies);

  size_t endpoint_count (void);

  TAO_Stub *create_stub_object (const TAO::ObjectKey &object_key,
                                const char *type_id,
                                CORBA::PolicyList *policy_list,
                                TAO_Acceptor_Filter *filter,
                                TAO_PriorityBandedConnectionPolicy *priority_bands
                               );

  int lane_required (TAO_Thread_Lane *lane,
                     TAO_PriorityBandedConnectionPolicy *priority_bands);

  TAO_Thread_Pool *thread_pool_;

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
