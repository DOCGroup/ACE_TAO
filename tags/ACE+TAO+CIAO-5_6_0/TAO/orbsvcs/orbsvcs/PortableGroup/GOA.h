// -*- C++ -*-

//=============================================================================
/**
 *  @file    GOA.h
 *
 *  $Id$
 *
 *  Header file for Group Object Adapter
 *
 *  @author  Several
 */
//=============================================================================

#ifndef TAO_GOA_H
#define TAO_GOA_H

#include /**/ "ace/pre.h"

#include "orbsvcs/PortableGroup/portablegroup_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/PortableGroupC.h"
#include "tao/PortableServer/Regular_POA.h"

// This is to remove "inherits via dominance" warnings from MSVC.
// MSVC is being a little too paranoid.
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Profile;
class TAO_PortableGroup_Acceptor_Registry;

/**
 * @class TAO_GOA
 *
 * @brief Implementation of the PortableGroup::GOA interface.
 *
 * Implementation of the PortableGroup::GOA interface.
 */
class TAO_PortableGroup_Export TAO_GOA :
    public virtual PortableGroup::GOA,
    public virtual TAO_Regular_POA
{
public:
  // Methods added by the
  /// @name MIOP specification methods
  //@{
  virtual PortableServer::ObjectId * create_id_for_reference (
      CORBA::Object_ptr the_ref
    );


  virtual PortableGroup::IDs * reference_to_ids (
      CORBA::Object_ptr the_ref
    );

  virtual void associate_reference_with_id (
      CORBA::Object_ptr ref,
      const PortableServer::ObjectId & oid
    );

  virtual void disassociate_reference_with_id (
      CORBA::Object_ptr ref,
      const PortableServer::ObjectId & oid
    );
  //@}

  // Standard POA interface methods
  PortableServer::POA_ptr create_POA (const char *adapter_name,
                                      PortableServer::POAManager_ptr poa_manager,
                                      const CORBA::PolicyList &policies);

  PortableServer::POA_ptr find_POA (const char *adapter_name,
                                    CORBA::Boolean activate_it);

  void destroy (CORBA::Boolean etherealize_objects,
                CORBA::Boolean wait_for_completion);

#if (TAO_HAS_MINIMUM_POA == 0)

  PortableServer::ThreadPolicy_ptr create_thread_policy (PortableServer::ThreadPolicyValue value);

#endif /* TAO_HAS_MINIMUM_POA == 0 */

  PortableServer::LifespanPolicy_ptr create_lifespan_policy (PortableServer::LifespanPolicyValue value);

  PortableServer::IdUniquenessPolicy_ptr create_id_uniqueness_policy (PortableServer::IdUniquenessPolicyValue value);

  PortableServer::IdAssignmentPolicy_ptr create_id_assignment_policy (PortableServer::IdAssignmentPolicyValue value);

#if (TAO_HAS_MINIMUM_POA == 0)

  PortableServer::ImplicitActivationPolicy_ptr create_implicit_activation_policy (PortableServer::ImplicitActivationPolicyValue value);

  PortableServer::ServantRetentionPolicy_ptr create_servant_retention_policy (PortableServer::ServantRetentionPolicyValue value);

  PortableServer::RequestProcessingPolicy_ptr create_request_processing_policy (PortableServer::RequestProcessingPolicyValue value);

#endif /* TAO_HAS_MINIMUM_POA == 0 */

  char * the_name (void);

  PortableServer::POA_ptr the_parent (void);

  PortableServer::POAList *the_children (void);

  PortableServer::POAManager_ptr the_POAManager (void);

#if (TAO_HAS_MINIMUM_POA == 0)

  PortableServer::AdapterActivator_ptr the_activator (void);

  void the_activator (PortableServer::AdapterActivator_ptr adapter_activator);

  PortableServer::ServantManager_ptr get_servant_manager (void);

  void set_servant_manager (PortableServer::ServantManager_ptr imgr);

  PortableServer::Servant get_servant (void);

  void set_servant (PortableServer::Servant servant);

#endif /* TAO_HAS_MINIMUM_POA == 0 */

  PortableServer::ObjectId *activate_object (PortableServer::Servant p_servant);

  void activate_object_with_id (const PortableServer::ObjectId &id,
                                PortableServer::Servant p_servant);

  void deactivate_object (const PortableServer::ObjectId &oid);

  CORBA::Object_ptr create_reference (const char *intf);

  CORBA::Object_ptr create_reference_with_id (const PortableServer::ObjectId &oid,
                                              const char *intf);

  PortableServer::ObjectId *servant_to_id (PortableServer::Servant p_servant);

  CORBA::Object_ptr servant_to_reference (PortableServer::Servant p_servant);

  PortableServer::Servant reference_to_servant (CORBA::Object_ptr reference);

  PortableServer::ObjectId *reference_to_id (CORBA::Object_ptr reference);

  PortableServer::Servant id_to_servant (const PortableServer::ObjectId &oid);

  CORBA::Object_ptr id_to_reference (const PortableServer::ObjectId &oid);

  CORBA::OctetSeq *id (void);

  // End standard POA interface methods.

  TAO_GOA (const String &name,
           PortableServer::POAManager_ptr poa_manager,
           const TAO_POA_Policy_Set &policies,
           TAO_Root_POA *parent,
           ACE_Lock &lock,
           TAO_SYNCH_MUTEX &thread_lock,
           TAO_ORB_Core &orb_core,
           TAO_Object_Adapter *object_adapter);

  virtual ~TAO_GOA (void);

    // Used to force the initialization of the code.
    static int Initializer (void);

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

  int find_group_component (const CORBA::Object_ptr the_ref,
                            PortableGroup::TagGroupTaggedComponent &group);

  int find_group_component_in_profile (
      const TAO_Profile* profile,
      PortableGroup::TagGroupTaggedComponent &group
    );

  int create_group_acceptors (
      CORBA::Object_ptr the_ref,
      TAO_PortableGroup_Acceptor_Registry &acceptor_registry,
      TAO_ORB_Core &orb_core
    );

  /// Helper function to associate group references with
  /// object references.
  void associate_group_with_ref (
        CORBA::Object_ptr group_ref,
        CORBA::Object_ptr obj_ref);
};

TAO_END_VERSIONED_NAMESPACE_DECL

// ****************************************************************

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"

#endif /* TAO_GOA_H */
