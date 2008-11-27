/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    FT_ReplicationManager.h
 *
 *  $Id$
 *
 *  This file is part of Fault Tolerant CORBA.
 *
 *  @author Curt Hibbs <hibbs_c@ociweb.com>
 */
//=============================================================================


#ifndef FT_REPLICATION_MANAGER_H_
#define FT_REPLICATION_MANAGER_H_

#include /**/ "ace/pre.h"
#include <ace/ACE.h>

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include <orbsvcs/FT_ReplicationManagerS.h>
//#include <orbsvcs/PortableGroup/PG_PropertyManager.h>
//#include <orbsvcs/PortableGroup/PG_GenericFactory.h>
//#include <orbsvcs/PortableGroup/PG_ObjectGroupManager.h>
// Note: the new, improved versions...
//#include <orbsvcs/PortableGroup/PG_Object_Group_Map.h>
#include <orbsvcs/PortableGroup/PG_Properties_Support.h>
#include <orbsvcs/PortableGroup/PG_Group_Factory.h>

#include <orbsvcs/PortableGroup/PG_FactoryRegistry.h>
#include <orbsvcs/FT_ReplicationManager/FT_FaultConsumer.h>

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  /**
  * Implement the ReplicationManager interfaces.
  *
  * The ReplicationManager does most of its work by delegating to
  * support objects.  These include:
  *
  *  TAO::PG_Group_Factory group_factory_;
  *   The group factory contains a collection of TAO::PG_Object_Groups
  *   It provides methods to create new groups, destroy old groups and
  *   find existing groups.
  *
  *  TAO::PG_Object_Group
  *   These objects which can be found through the group factory provde
  *   methods to create and add group members, remove and delete group
  *   members and set group properties.
  *
  *  TAO::PG_Properties_Support properties_support_;
  *   This object maintains sets of properties(TAO::PG_Property_Set).
  *   In particular it has one default property set, and a collection of
  *   property sets indexed by type_id.
  *   The default property set acts as a parent to the type_id property
  *   sets and the type_id property sets act as parents to the property
  *   sets contained in PG_Object_Group.
  *
  *  FT::FaultNotifier_var fault_notifier_;
  *   This notification channel is "the" source of fault notifications.
  *
  *  TAO::FT_FaultConsumer fault_consumer_;
  *   This object subscribes to the fault_notifier_as a fault consumer. It
  *   analyzes incoming fault notifications and calls appropriate ReplicationManager
  *   methods to respond to the fault.
  *
  *  TAO::PG_FactoryRegistry factory_registry_;
  *   This object maintains a collection of factory registrations.  When a factory
  *   is started it registeres itself with the ReplicationManager (delegated to this
  *   object).  When a member needs to be created in an object group this factory
  *   registry is queried to find factories that can create the member.
  */
  class FT_ReplicationManager
    : public virtual POA_FT::ReplicationManager
  {

    //////////////////////
    // non-CORBA interface

  public:
    /**
     * Default constructor.
     * Call init after constructing the object to prepare it for use.
     */
    FT_ReplicationManager ();

    /**
     * Destructor.
     * Actual cleanup happens in the fini function.
     */
    virtual ~FT_ReplicationManager ();

  public:

    /**
     * Parse command line arguments.
     * @param argc traditional C argc
     * @param argv traditional C argv
     * @return zero for success; nonzero is process return code for failure.
     */
    int parse_args (int argc, ACE_TCHAR * argv[]);

    /**
     * Initialize this object.
     * @param orb Our CORBA::ORB -- we keep var to it.
     * @return zero for success; nonzero is process return code for failure.
     */
    int init (CORBA::ORB_ptr orb);

    /**
     * Prepare to exit.
     * @return zero for success; nonzero is process return code for failure.
     */
    int fini (void);

    /**
     * Idle-time activity.
     *
     * @param result is set to process return code if return value is non-zero.
     * @return zero to continue; nonzero to exit
     */
    int idle (int & result);


    /**
     * Identify this fault detector factory.
     * @return a string to identify this object for logging/console message purposes.
     */
    const char * identity () const;

    /**
     * Get the type_id associated with an object group.
     * @param object_group The ObjectGroup.
     * @return String identifying the type id associated with the ObjectGroup.
     */
    char * type_id (PortableGroup::ObjectGroup_ptr object_group);


  //////////////////////
  // CORBA interface(s)

  public:

    /**
    * @name POA_FT::ReplicationManager Methods
    *
    * Methods required by the POA_FT::ReplicationManager interface.
    */
    //@{

    /// Registers the Fault Notifier with the Replication Manager.
    virtual void register_fault_notifier (
        FT::FaultNotifier_ptr fault_notifier
      );

    /// Returns the reference of the Fault Notifier.
    virtual FT::FaultNotifier_ptr get_fault_notifier (
      );

    /// TAO-specific find factory registry
    virtual ::PortableGroup::FactoryRegistry_ptr get_factory_registry (
        const PortableGroup::Criteria & selection_criteria
      );

    /// TAO-specific shutdown operation.
    virtual void shutdown (
      );

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

    /**
     * Set properties associated with a given Replica type.  These
     * properties override the default properties on a name-by-name basis.
     */
    virtual void set_type_properties (
        const char * type_id,
        const PortableGroup::Properties & overrides);

    /**
     * Return the properties associated with a given Replica type.  These
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
     * group as the replication manager and replicas are being executed.
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
     * @name FT::FTObjectGroupManager methods
     *
     * Methods required by the FT::FTObjectGroupManager
     * interface.
     */
    //@{

    /// Create a member in an object group.
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
     * Remove the member at a specific location from an
     * ObjectGroup.  Application created objects must be
     * deleted by the application.  Objects created by the
     * infrastructure (replication manager) will be deleted by the
     * infrastructure.
     * For infrastructure-controlled membership: After the member
     * is removed from the group the minumum number of members
     * parameter will be checked and new members will be created
     * as necessary (if possible.)
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

    /**
     * Return an update the IOGR for an object group.  If no changes have
     * been made in the group the return value will be the same as the object_group
     * parameter.
     */
    virtual PortableGroup::ObjectGroup_ptr get_object_group_ref (
        PortableGroup::ObjectGroup_ptr object_group);

    /**
     * TAO-specific extension.
     * Return the ObjectGroup reference for the given ObjectGroupId.
     */
     virtual PortableGroup::ObjectGroup_ptr get_object_group_ref_from_id (
          PortableGroup::ObjectGroupId group_id
        );

    /**
     * Return the reference corresponding to the Replica of a given
     * ObjectGroup at the given location.
     */
    virtual CORBA::Object_ptr get_member_ref (
        PortableGroup::ObjectGroup_ptr object_group,
        const PortableGroup::Location & loc);

    /// Sets the primary member of a group.
    virtual PortableGroup::ObjectGroup_ptr set_primary_member (
        PortableGroup::ObjectGroup_ptr object_group,
        const PortableGroup::Location & the_location
      );

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
     *
     * Infrastructure controlled membership:  The initial number of members
     * property will be honored by creating new members and adding them to
     * the group.
     */
    virtual CORBA::Object_ptr create_object (
        const char * type_id,
        const PortableGroup::Criteria & the_criteria,
        PortableGroup::GenericFactory::FactoryCreationId_out
          factory_creation_id);

    /**
     * Delete the object group corresponding to the provided
     * FactoryCreationId.  For infratructure-controlled membership
     * all members will be deleted.  For application-controlled membership
     * the application is responsible for deleting group members.
     */
    virtual void delete_object (
        const PortableGroup::GenericFactory::FactoryCreationId &
          factory_creation_id);

    //@}

    /////////////////////////
    // Implementation methods
  private:
    /**
     * Write this factory's IOR to a file
     */
    int write_ior (void);

    /// Registers the Fault Notifier with the Replication Manager.
    void register_fault_notifier_i (
        FT::FaultNotifier_ptr fault_notifier
      );

    ////////////////
    // Forbidden methods
    FT_ReplicationManager (const FT_ReplicationManager & rhs);
    FT_ReplicationManager & operator = (const FT_ReplicationManager & rhs);

    ///////////////
    // Data Members
  private:

    /// The orb
    CORBA::ORB_var orb_;

    /// The POA.
    PortableServer::POA_var poa_;

    /// A file to which the factory's IOR should be written.
    const ACE_TCHAR * ior_output_file_;

    /// A name to be used to register the factory with the name service.
    ACE_CString ns_name_;
    CosNaming::NamingContext_var naming_context_;
    CosNaming::Name this_name_;

    /// Our object reference.
    FT::ReplicationManager_var replication_manager_ref_;

    /// A human-readable string to identify this Replication Manager.
    ACE_CString identity_;

    /// an object that manages a collection of object groups
    TAO::PG_Group_Factory group_factory_;

    /// an object that manages default and type_id related properties
    TAO::PG_Properties_Support properties_support_;

    /// The fault notifier.
    FT::FaultNotifier_var fault_notifier_;
    /// set by command line -f option
    const ACE_TCHAR * fault_notifier_ior_string_;

    /// The fault consumer.
    TAO::FT_FaultConsumer fault_consumer_;

    /// The factory registry
    TAO::PG_FactoryRegistry factory_registry_;

    /// Quit flag.
    int quit_;
  };

} // namespace TAO

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif  /* FT_REPLICATION_MANAGER_H_ */
