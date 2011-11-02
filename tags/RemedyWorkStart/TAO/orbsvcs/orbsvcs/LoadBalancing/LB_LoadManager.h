// -*- C++ -*-

//=============================================================================
/**
 * @file   LB_LoadManager.h
 *
 * $Id$
 *
 * @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================


#ifndef TAO_LB_LOAD_MANAGER_H
#define TAO_LB_LOAD_MANAGER_H

#include /**/ "ace/pre.h"

#include "orbsvcs/CosLoadBalancingS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/LoadBalancing/LB_LoadAlertMap.h"
#include "orbsvcs/LoadBalancing/LB_MonitorMap.h"
#include "orbsvcs/LoadBalancing/LB_LoadListMap.h"
#include "orbsvcs/LoadBalancing/LB_Pull_Handler.h"

#include "orbsvcs/PortableGroupC.h"

#include "orbsvcs/PortableGroup/PG_PropertyManager.h"
#include "orbsvcs/PortableGroup/PG_GenericFactory.h"
#include "orbsvcs/PortableGroup/PG_ObjectGroupManager.h"
#include "ace/Unbounded_Queue.h"
#include "ace/Task.h"
#include "tao/Condition.h"


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_LoadBalancing_Export TAO_LB_LoadManager
  : public virtual POA_CosLoadBalancing::LoadManager,
    public ACE_Task_Base
{
public:

  /// Constructor.
  TAO_LB_LoadManager (int ping_timeout,
                      int ping_interval);

  virtual int svc (void);

  /**
   * @name CosLoadBalancing::LoadManager Methods
   *
   * Methods required by the CosLoadBalancing::LoadManager interface.
   */
  //@{

  /// For the PUSH load monitoring style.
  virtual void push_loads (const PortableGroup::Location & the_location,
                           const CosLoadBalancing::LoadList & loads);

  /// Return the raw loads at the given location.
  virtual CosLoadBalancing::LoadList * get_loads (
      const PortableGroup::Location & the_location);

  /// Inform member at given location of load alert condition.
  virtual void enable_alert (const PortableGroup::Location & the_location);

  /// Inform member at given location that load alert condition has
  /// passed.
  virtual void disable_alert (const PortableGroup::Location & the_location);

  /// Register a LoadAlert object for the member at the given
  /// location.
  virtual void register_load_alert (
      const PortableGroup::Location & the_location,
      CosLoadBalancing::LoadAlert_ptr load_alert);

  /// Retrieve the LoadAlert object for the member at the given
  /// location.
  virtual CosLoadBalancing::LoadAlert_ptr get_load_alert (
      const PortableGroup::Location & the_location);

  /// Remove (de-register) the LoadAlert object for the member at the
  /// given location.
  virtual void remove_load_alert (const PortableGroup::Location & the_location);

  /// Register a load monitor with the load balancer.
  virtual void register_load_monitor (
      const PortableGroup::Location & the_location,
      CosLoadBalancing::LoadMonitor_ptr load_monitor);

  /// Return a reference to the load monitor at the given location.
  virtual CosLoadBalancing::LoadMonitor_ptr get_load_monitor (
      const PortableGroup::Location & the_location);

  /// Remove a load monitor at the given location from the load
  /// balancer.
  virtual void remove_load_monitor (
      const PortableGroup::Location & the_location);

  //@}

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

  /// Return the next member to which client requests will be
  /// forwarded.
  /**
   * Select the next member of the object group corresponding to the
   * given ObjectId.  The object group's load balancing strategy
   * will be queried for that member.
   */
  CORBA::Object_ptr next_member (const PortableServer::ObjectId & oid);

public:

  /// Initialize the load balancer.  This will cause a child POA to be
  /// created with the appropriate policies to support ServantLocators
  /// (i.e. for the MemberLocator).
  void initialize (ACE_Reactor * reactor,
                   CORBA::ORB_ptr orb,
                   PortableServer::POA_ptr root_poa);

protected:

  /// Destructor.
  /**
   * Destructor is protected to enforce correct memory management
   * through reference counting.
   */
  ~TAO_LB_LoadManager (void);

private:

  /// Extract the value of the InitialNumberReplicas property from
  /// the_criteria.
  int get_initial_number_replicas (
    const char * type_id,
    const PortableGroup::Criteria & the_criteria,
    CORBA::UShort & initial_number_replicas) const;

  /// Extract the value of the Factories property from the_criteria.
  /**
   * This method ensures that the locations in the returned
   * FactoryInfos are unique.  This is necessary to ensure that only
   * one replica of a given type is created by the load balancer at a
   * given location.
   */
  int get_factory_infos (
    const char * type_id,
    const PortableGroup::Criteria & the_criteria,
    PortableGroup::FactoryInfos & factory_infos) const;

  /// Preprocess Strategy or CustomStrategy properties.
  /**
   * This method takes care of converting StrategyInfo properties to
   * Strategy properties, and verifying that CustomStrategy references
   * are not nil.
   */
  void preprocess_properties (PortableGroup::Properties & props);

  /// Create a built-in load balancing strategy and return a reference
  /// to it.
  CosLoadBalancing::Strategy_ptr make_strategy (
    CosLoadBalancing::StrategyInfo * info);

private:

  CORBA::ORB_var orb_;

  /// Reactor used when pulling loads from registered load monitors.
  ACE_Reactor * reactor_;

  /// The POA that dispatches requests to the MemberLocator.
  PortableServer::POA_var poa_;

  /// The Root POA.
  PortableServer::POA_var root_poa_;

  /// Mutex that provides synchronization for the LoadMonitor map.
  TAO_SYNCH_MUTEX monitor_lock_;

  /// Mutex that provides synchronization for the LoadMap table.
  TAO_SYNCH_MUTEX load_lock_;

  /// Mutex that provides synchronization for the LoadAlert table.
  TAO_SYNCH_MUTEX load_alert_lock_;

  /// Mutex that provides synchronization for the LoadManager's
  /// state.
  TAO_SYNCH_MUTEX lock_;

  /// Table that maps PortableGroup::Location to load monitor at that
  /// location.
  TAO_LB_MonitorMap monitor_map_;

  /// Table that maps location to load list.
  TAO_LB_LoadListMap load_map_;

  /// Table that maps object group and location to LoadAlert object.
  TAO_LB_LoadAlertMap load_alert_map_;

  /// The ObjectGroupManager that implements the functionality
  /// necessary for application-controlled object group membership.
  TAO_PG_ObjectGroupManager object_group_manager_;

  /// The PropertyManager that is responsible for parsing all criteria,
  /// and keeping track of property-type_id associations.
  TAO_PG_PropertyManager property_manager_;

  /// The GenericFactory responsible for creating all object groups.
  TAO_PG_GenericFactory generic_factory_;

  /// The event handler that performs "pull monitoring" on all
  /// registered load monitors.
  TAO_LB_Pull_Handler pull_handler_;

  /// Timer ID corresponding to the timer that fires off the "pull
  /// monitoring" event handler.
  long timer_id_;

  /// Cached object reference that points to this servant.
  CosLoadBalancing::LoadManager_var lm_ref_;

  /// Cached object reference that points to the AMI handler for all
  /// LoadAlert objects.
  CosLoadBalancing::AMI_LoadAlertHandler_var load_alert_handler_;

  /**
   * @name Built-in load balancing strategy implementations
   *
   * "Built-in" load balancing strategies defined by the load
   * balancing specification.
   */
  //@{
  /// The "RoundRobin" load balancing strategy.
  CosLoadBalancing::Strategy_var round_robin_;

  /// The "Random" load balancing strategy.
  CosLoadBalancing::Strategy_var random_;

  /// The "LeastLoaded" load balancing strategy.
  CosLoadBalancing::Strategy_var least_loaded_;

  /// The "LoadMinimum" load balancing strategy.
  CosLoadBalancing::Strategy_var load_minimum_;

  /// The "LoadAverage" load balancing strategy.
  CosLoadBalancing::Strategy_var load_average_;
  //@}

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

  /// Short timeout for non_exist call. defaults to 1 second
  TimeBase::TimeT ping_timeout_;
  /// Long timeout for non_exist call. defaults to 5 seconds
  ACE_Time_Value ping_interval_;
};


TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif  /* TAO_LB_LOAD_MANAGER_H */
