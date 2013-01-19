// -*- C++ -*-

//=============================================================================
/**
 * @file   FT_Naming_Manager.h
 *
 * $Id$
 *
 * @author Kevin Stanley <stanleyk@ociweb.com>
 */
//=============================================================================


#ifndef TAO_FT_NAMING_MANAGER_H
#define TAO_FT_NAMING_MANAGER_H

#include /**/ "ace/pre.h"

#include "orbsvcs/Naming/FaultTolerant/ftnaming_export.h"

#include "orbsvcs/FT_NamingManagerS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/FT_NamingManagerC.h"

#include "orbsvcs/PortableGroup/PG_FactoryRegistry.h"
#include "orbsvcs/PortableGroup/PG_Properties_Support.h"
#include "orbsvcs/Naming/FaultTolerant/FT_PG_Group_Factory.h"
#include "orbsvcs/Naming/FaultTolerant/FT_Round_Robin.h"

#include "ace/Task.h"
#include "tao/Condition.h"


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  class Storable_Factory;
}

namespace FT_Naming
{
  struct ObjectGroupUpdate;
}

/**
 * @class TAO_FT_Naming_Manager
 * @brief Implements the NamingManager interface for the Fault
 * tolerant naming service.
 *
 * This class implements the NamingManager interface in support
 * of the load balancing features of the FaultTolerant Naming
 * Service. Uses can create and manage object groups which can be
 * bound in the Naming Service, which will provide the load balancing
 * functionality.
 */
class TAO_FtNaming_Export TAO_FT_Naming_Manager
  : public virtual POA_FT_Naming::NamingManager,
    public ACE_Task_Base
{
public:

  /// Constructor.
  TAO_FT_Naming_Manager (void);

  /**
   * @name FT::NamingManager Methods
   *
   *
   */

  /// Creates an object group with the specified name and load
  /// balancing strategy.
  /// @param[in] group_name The symbolic name of the group that can
  /// be used to refer to the group in other operations.
  /// @param[in] lb_strategy The strategy to be used by the Naming
  /// Service when this object group is resolved.
  ///@param[in] the_criteria Properties to be used by the object group.
  virtual PortableGroup::ObjectGroup_ptr create_object_group (
    const char * group_name,
    FT_Naming::LoadBalancingStrategyValue lb_strategy,
    const ::PortableGroup::Criteria & the_criteria);

  /// Deletes the object group with the provided group_name.
  virtual void delete_object_group (
    const char * group_name);

  /// Retreives a reference to a group with the specified name
  virtual PortableGroup::ObjectGroup_ptr get_object_group_ref_from_name (
    const char * group_name);

  /// Provide a new load balancing strategy for the group with the provided
  /// name.
  virtual void set_load_balancing_strategy (
      const char * group_name,
      FT_Naming::LoadBalancingStrategyValue lb_strategy);

  /// Retreive the names of the groups with the specified load balanacing
  /// strategy that have been created in this Naming Manager.
  virtual FT_Naming::GroupNames * groups (
     ::FT_Naming::LoadBalancingStrategyValue target_strategy);

  /**
   * @name PortableGroup::PropertyManager Methods
   *
   * Methods required by the PortableGroup::PropertyManager interface.
   */
  //@{

  /// Set the default properties to be used by all object groups.
  virtual void set_default_properties (
      const PortableGroup::Properties & props);

  /// Get the default properties used by all object groups.
  virtual PortableGroup::Properties * get_default_properties ();

  /// Remove default properties.
  virtual void remove_default_properties (
      const PortableGroup::Properties & props);

  /// Set properties associated with a given Replica type.  These
  /// properties override the default properties.
  virtual void set_type_properties (
      const char * type_id,
      const PortableGroup::Properties & overrides);

  /**
   * Return the properties associated with a give Replica type.  These
   * properties include the type-specific properties in use, in
   * addition to the default properties that were not overridden.
   */
  virtual PortableGroup::Properties * get_type_properties (
      const char * type_id);

  /// Remove the given properties associated with the Replica type ID.
  virtual void remove_type_properties (
      const char * type_id,
      const PortableGroup::Properties & props);

  /**
   * Dynamically set the properties associated with a given object
   * group as the load balancer and replicas are being executed.
   * These properties override the type-specific and default
   * properties.
   */
  virtual void set_properties_dynamically (
      PortableGroup::ObjectGroup_ptr object_group,
      const PortableGroup::Properties & overrides);

  /**
   * Return the properties currently in use by the given object
   * group.  These properties include those that were set dynamically,
   * type-specific properties that weren't overridden, properties that
   * were used when the Replica was created, and default properties
   * that weren't overridden.
   */
  virtual PortableGroup::Properties * get_properties (
      PortableGroup::ObjectGroup_ptr object_group);

  //@}

  /**
   * @name PortableGroup::ObjectGroupManager methods
   *
   * Methods required by the PortableGroup::ObjectGroupManager
   * interface.
   */
  //@{

  /// Create a member using the load balancer ObjectGroupManager, and
  /// add the created object to the ObjectGroup.
  virtual PortableGroup::ObjectGroup_ptr create_member (
      PortableGroup::ObjectGroup_ptr object_group,
      const PortableGroup::Location & the_location,
      const char * type_id,
      const PortableGroup::Criteria & the_criteria);

  /// Add an existing object to the ObjectGroup.
  /// @param[in] object_group A reference for the group to which the
  /// specified member is to be added.
  /// @param[in] the_location The symbolic value that specifies this
  /// specific member. The location can be any string value.
  /// @param[in] member The object reference for the member. The first
  /// member's type is used to defined the object group type. All subsequence
  /// members must have the same type.
  virtual PortableGroup::ObjectGroup_ptr add_member (
      PortableGroup::ObjectGroup_ptr object_group,
      const PortableGroup::Location & the_location,
      CORBA::Object_ptr member);

  /**
   * Remove an object at a specific location from the given
   * ObjectGroup.  Deletion of application created objects must be
   * deleted by the application.  Objects created by the
   * infrastructure (load balancer) will be deleted by the
   * infrastructure.
   */
  virtual PortableGroup::ObjectGroup_ptr remove_member (
      PortableGroup::ObjectGroup_ptr object_group,
      const PortableGroup::Location & the_location);

  /// Return the locations of the members in the given ObjectGroup.
  virtual PortableGroup::Locations * locations_of_members (
      PortableGroup::ObjectGroup_ptr object_group);

  /// Return the locations of the members in the given ObjectGroup.
  virtual PortableGroup::ObjectGroups * groups_at_location (
      const PortableGroup::Location & the_location);

  /// Return the ObjectGroupId for the given ObjectGroup.
  virtual PortableGroup::ObjectGroupId get_object_group_id (
      PortableGroup::ObjectGroup_ptr object_group);

  /// TAO specific method
  virtual PortableGroup::ObjectGroup_ptr get_object_group_ref_from_id (
      PortableGroup::ObjectGroupId group_id);

  /// Return the reference corresponding to the Replica of a given
  /// ObjectGroup at the given location.
  virtual CORBA::Object_ptr get_member_ref (
      PortableGroup::ObjectGroup_ptr object_group,
      const PortableGroup::Location & loc);

  //@}

  /**
   * @name PortableGroup::GenericFactory methods
   *
   * Methods required by the PortableGroup::GenericFactory interface.
   */
  //@{

  /**
   * Create an object of the specified type that adheres to the
   * restrictions defined by the provided Criteria.  The out
   * FactoryCreationId parameter may be passed to the delete_object()
   * method to delete the object.  This signature is modified from
   * the generic factory operation to support the use of an object
   * name in addition to the factory_creation_id.
   */
  virtual CORBA::Object_ptr create_object (
      const char * object_name,
      const char * type_id,
      const PortableGroup::Criteria & the_criteria,
      PortableGroup::GenericFactory::FactoryCreationId_out
        factory_creation_id);

  /**
   * Delete the object corresponding to the provided
   * FactoryCreationId.  If the object is actually an ObjectGroup,
   * then all members within the ObjectGroup will be deleted.
   * Afterward, the ObjectGroup itself will be deleted.
   */
  virtual void delete_object (
      const PortableGroup::GenericFactory::FactoryCreationId &
        factory_creation_id);

  //@}

  /// Initialize the naming manager. This will provide the poa to
  /// the naming manager and underlying components for use in
  /// managing the object groups.
  void initialize (CORBA::ORB_ptr orb,
                   PortableServer::POA_ptr root_poa);


  /// Utilizing the load balancing strategy identified by the object
  /// group property, return the next object reference from the object
  /// group which should be used to service the next CORBA request
  CORBA::Object_ptr next_member (PortableGroup::ObjectGroup_ptr object_group);

  /// Load/save state of object groups from/to file for fault
  /// tolerant purposes.
  void
  set_object_group_storable_factory (TAO::Storable_Factory *
    factory);

  /// Indicate the object group state is stale.
  /// Only valid when object group persistence is enabled.
  void set_object_group_stale (const FT_Naming::ObjectGroupUpdate & group_info);

  /// Destructor.
  ~TAO_FT_Naming_Manager (void);

private:
  /// A utility to ensure we can access the latest object reference for
  /// the object group when referenced externally.
  virtual PortableGroup::ObjectGroup_ptr get_object_group_ref (
      PortableGroup::ObjectGroup_ptr object_group);

  /// TAO specific method  /// Preprocess Strategy or CustomStrategy properties.
  /**
   * This method takes care of converting StrategyInfo properties to
   * Strategy properties, and verifying that CustomStrategy references
   * are not nil.
   */
  void preprocess_properties (PortableGroup::Properties & props);

  /// Utility for accessing the object group name.  Returns true if a
  /// name is found and allocates a string with the name assigned to
  /// the name parameter.
  bool group_name (PortableGroup::ObjectGroup_ptr group, char*& name);

  /// Mutex that provides synchronization for the TAO_FT_Naming_Manager's
  /// state.
  TAO_SYNCH_MUTEX lock_;

  /// an object that manages default and type_id related properties
  TAO::PG_Properties_Support properties_support_;

  /// Registry used by the PG_Group_Factory
  TAO::PG_FactoryRegistry factory_registry_;

  /// The group factory responsible for creating object groups
  TAO::FT_PG_Group_Factory group_factory_;

  /**
   * @name Built-in load balancing strategy implementations
   *
   * "Built-in" load balancing strategies. Currently only RoundRobin
   * is supported.
   */
  //@{

  /// The "RoundRobin" load balancing strategy.
  TAO_FT_Round_Robin round_robin_;
  //@}

  /// Cached instance of the Property name
  /// "org.omg.CosLoadBalancing.Strategy".
  PortableGroup::Name built_in_balancing_strategy_name_;

  PortableGroup::Name object_group_property_name_;

  TAO_SYNCH_MUTEX validate_lock_;
  TAO_Condition<TAO_SYNCH_MUTEX> validate_condition_;

  bool shutdown_;

};


TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif  /* TAO_FT_NAMING_MANAGER_H */
