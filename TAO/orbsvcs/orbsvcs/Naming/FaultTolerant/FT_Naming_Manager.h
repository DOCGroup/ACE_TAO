// -*- C++ -*-

//=============================================================================
/**
 * @file   FT_NamingManager.h
 *
 * $Id$
 *
 * @author Kevin Stanley <stanleyk@ociweb.com>
 */
//=============================================================================


#ifndef TAO_FT_NAMING_MANAGER_H
#define TAO_FT_NAMING_MANAGER_H

#include /**/ "ace/pre.h"

#include "orbsvcs/FT_NamingManagerS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/FT_NamingManagerC.h"

#include "orbsvcs/PortableGroup/PG_Group_Factory.h"
#include "orbsvcs/PortableGroup/PG_FactoryRegistry.h"
#include <orbsvcs/PortableGroup/PG_Properties_Support.h>
#include "ace/Task.h"
#include "tao/Condition.h"


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_FtNaming_Export TAO_FT_Naming_Manager
  : public virtual POA_FT::NamingManager,
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

  virtual ::CORBA::Object_ptr create_object_group (
    const char * group_name,
    const char * type_id,
    const ::PortableGroup::Criteria & the_criteria);

  virtual void delete_object_group (
    const char * group_name);

  virtual ::PortableGroup::ObjectGroup_ptr get_object_group_ref_from_name (
    const char * group_name);

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

  /// @note Does this method make sense for load balanced objects?
  virtual PortableGroup::ObjectGroup_ptr get_object_group_ref (
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
   * method to delete the object.
   */
  virtual CORBA::Object_ptr create_object (
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

  /// Destructor.
  /**
   * Destructor is protected to enforce correct memory management
   * through reference counting.
   */
  ~TAO_FT_Naming_Manager (void);

private:
  /// Preprocess Strategy or CustomStrategy properties.
  /**
   * This method takes care of converting StrategyInfo properties to
   * Strategy properties, and verifying that CustomStrategy references
   * are not nil.
   */
  void preprocess_properties (PortableGroup::Properties & props);

  /// Mutex that provides synchronization for the TAO_FT_Naming_Manager's
  /// state.
  TAO_SYNCH_MUTEX lock_;

  // Attempt to use group factory instead
#if 0 
  /// The ObjectGroupManager that implements the functionality
  /// necessary for application-controlled object group membership.
  TAO_PG_ObjectGroupManager object_group_manager_;

  /// The PropertyManager that is responsible for parsing all criteria,
  /// and keeping track of property-type_id associations.
  TAO_PG_PropertyManager property_manager_;

  /// The GenericFactory responsible for creating all object groups.
  TAO_PG_GenericFactory generic_factory_;

#endif

  /// an object that manages default and type_id related properties
  TAO::PG_Properties_Support properties_support_;

  /// Registry used by the PG_Group_Factory
  TAO::PG_FactoryRegistry factory_registry_;

  // The group factory responsible for creating object groups 
  TAO::PG_Group_Factory group_factory_;

  /// Cached instance of the Property name
  /// "org.omg.CosLoadBalancing.StrategyInfo".
  PortableGroup::Name built_in_balancing_strategy_info_name_;

  /// Cached instance of the Property name
  /// "org.omg.CosLoadBalancing.Strategy".
  PortableGroup::Name built_in_balancing_strategy_name_;

  /// Cached instance of the Property name
  /// "org.omg.CosLoadBalancing.CustomStrategy".
  PortableGroup::Name custom_balancing_strategy_name_;

  TAO_SYNCH_MUTEX validate_lock_;
  TAO_Condition<TAO_SYNCH_MUTEX> validate_condition_;

  bool shutdown_;

};


TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif  /* TAO_FT_NAMING_MANAGER_H */
