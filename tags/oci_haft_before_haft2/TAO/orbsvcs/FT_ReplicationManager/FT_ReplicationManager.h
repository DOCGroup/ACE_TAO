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

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

//@@ Review all these header files and make sure they are all needed
//   or if some can be replaced with forward declarations.
#include <orbsvcs/FT_ReplicationManagerS.h>
#include <orbsvcs/FT_NotifierC.h>
#include <tao/IORManipulation/IORC.h>
#include <orbsvcs/PortableGroupC.h>
#include <orbsvcs/PortableGroup/PG_PropertyManager.h>
#include <orbsvcs/PortableGroup/PG_GenericFactory.h>
#include <orbsvcs/PortableGroup/PG_ObjectGroupManager.h>
#include <orbsvcs/PortableGroup/PG_FactoryRegistry.h>
#include <orbsvcs/PortableGroup/PG_Object_Group_Map.h>
#include <orbsvcs/FaultTolerance/FT_Service_Activate.h>
#include <orbsvcs/FT_ReplicationManager/FT_FaultConsumer.h>
#include <orbsvcs/CosNamingC.h>

namespace TAO
{
  /**
  * Implement the ReplicationManager interfaces.
  *
  */
  class FT_ReplicationManager
    : public virtual POA_FT::ReplicationManager,
      public virtual PortableServer::RefCountServantBase
  {

    //////////////////////
    // non-CORBA interface

  public:
    /**
    * Default constructor.
    */
    FT_ReplicationManager ();

    /**
    * Destructor.
    */
    virtual ~FT_ReplicationManager ();

  public:

  /**
   * Parse command line arguments.
   * @param argc traditional C argc
   * @param argv traditional C argv
   * @return zero for success; nonzero is process return code for failure.
   */
  int parse_args (int argc, char * argv[]);

  /**
   * Initialize this object.
   * @param orb Our CORBA::ORB -- we keep var to it.
   * @return zero for success; nonzero is process return code for failure.
   */
  int init (CORBA::ORB_ptr orb ACE_ENV_ARG_DECL);

  /**
   * Prepare to exit.
   * @return zero for success; nonzero is process return code for failure.
   */
  int fini (ACE_ENV_SINGLE_ARG_DECL);

  /**
   * Idle-time activity.
   *
   * @param result is set to process return code if return value is non-zero.
   * @return zero to continue; nonzero to exit
   */
  int idle(int & result);


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
  char * type_id (PortableGroup::ObjectGroup_ptr object_group
      ACE_ENV_ARG_DECL);

    /////////////////////////
    // Implementation methods
  private:
    /**
    * Write this factory's IOR to a file
    */
    int write_ior (void);

    /**
    * Extract the value of the InitialNumberReplicas property from
    * the_criteria.
    */
    int get_initial_number_replicas (
      const char * type_id,
      const PortableGroup::Criteria & the_criteria,
      CORBA::UShort & initial_number_replicas) const;

    /// Registers the Fault Notifier with the Replication Manager.
    void register_fault_notifier_i (
        FT::FaultNotifier_ptr fault_notifier
        ACE_ENV_ARG_DECL
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));

    /// Implementation of TAO-specific shutdown operation.
    void shutdown_i (
        ACE_ENV_SINGLE_ARG_DECL
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));

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
        ACE_ENV_ARG_DECL
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));

    /// Returns the reference of the Fault Notifier.
    virtual FT::FaultNotifier_ptr get_fault_notifier (
        ACE_ENV_SINGLE_ARG_DECL
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
        , FT::InterfaceNotFound
      ));

    /// TAO-specific find factory registry
    virtual ::PortableGroup::FactoryRegistry_ptr get_factory_registry (
        const PortableGroup::Criteria & selection_criteria
        ACE_ENV_ARG_DECL
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));

    /// TAO-specific shutdown operation.
    virtual void shutdown (
        ACE_ENV_SINGLE_ARG_DECL
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));

    //@}

    /**
    * @name PortableGroup::PropertyManager Methods
    *
    * Methods required by the PortableGroup::PropertyManager interface.
    */
    //@{

    /// Set the default properties to be used by all object groups.
    virtual void set_default_properties (
        const PortableGroup::Properties & props
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                      PortableGroup::InvalidProperty,
                      PortableGroup::UnsupportedProperty));

    /// Get the default properties used by all object groups.
    virtual PortableGroup::Properties * get_default_properties (
        ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    /// Remove default properties.
    virtual void remove_default_properties (
        const PortableGroup::Properties & props
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                      PortableGroup::InvalidProperty,
                      PortableGroup::UnsupportedProperty));

    /// Set properties associated with a given Replica type.  These
    /// properties override the default properties.
    virtual void set_type_properties (
        const char * type_id,
        const PortableGroup::Properties & overrides
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                      PortableGroup::InvalidProperty,
                      PortableGroup::UnsupportedProperty));

    /**
    * Return the properties associated with a give Replica type.  These
    * properties include the type-specific properties in use, in
    * addition to the default properties that were not overridden.
    */
    virtual PortableGroup::Properties * get_type_properties (
        const char * type_id
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    /// Remove the given properties associated with the Replica type ID.
    virtual void remove_type_properties (
        const char * type_id,
        const PortableGroup::Properties & props
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                      PortableGroup::InvalidProperty,
                      PortableGroup::UnsupportedProperty));

    /**
    * Dynamically set the properties associated with a given object
    * group as the replication manager and replicas are being executed.
    * These properties override the type-specific and default
    * properties.
    */
    virtual void set_properties_dynamically (
        PortableGroup::ObjectGroup_ptr object_group,
        const PortableGroup::Properties & overrides
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                      PortableGroup::ObjectGroupNotFound,
                      PortableGroup::InvalidProperty,
                      PortableGroup::UnsupportedProperty));

    /**
    * Return the properties currently in use by the given object
    * group.  These properties include those that were set dynamically,
    * type-specific properties that weren't overridden, properties that
    * were used when the Replica was created, and default properties
    * that weren't overridden.
    */
    virtual PortableGroup::Properties * get_properties (
        PortableGroup::ObjectGroup_ptr object_group
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                      PortableGroup::ObjectGroupNotFound));

    //@}

    /**
    * @name FT::FTObjectGroupManager methods
    *
    * Methods required by the FTup::FTObjectGroupManager
    * interface.
    */
    //@{

    /// Create a member using the ObjectGroupManager, and
    /// add the created object to the ObjectGroup.
    virtual PortableGroup::ObjectGroup_ptr create_member (
        PortableGroup::ObjectGroup_ptr object_group,
        const PortableGroup::Location & the_location,
        const char * type_id,
        const PortableGroup::Criteria & the_criteria
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                      PortableGroup::ObjectGroupNotFound,
                      PortableGroup::MemberAlreadyPresent,
                      PortableGroup::NoFactory,
                      PortableGroup::ObjectNotCreated,
                      PortableGroup::InvalidCriteria,
                      PortableGroup::CannotMeetCriteria));

    /// Add an existing object to the ObjectGroup.
    virtual PortableGroup::ObjectGroup_ptr add_member (
        PortableGroup::ObjectGroup_ptr object_group,
        const PortableGroup::Location & the_location,
        CORBA::Object_ptr member
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                      PortableGroup::ObjectGroupNotFound,
                      PortableGroup::MemberAlreadyPresent,
                      PortableGroup::ObjectNotAdded));

    /**
    * Remove an object at a specific location from the given
    * ObjectGroup.  Deletion of application created objects must be
    * deleted by the application.  Objects created by the
    * infrastructure (replication manager) will be deleted by the
    * infrastructure.
    */
    virtual PortableGroup::ObjectGroup_ptr remove_member (
        PortableGroup::ObjectGroup_ptr object_group,
        const PortableGroup::Location & the_location
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                      PortableGroup::ObjectGroupNotFound,
                      PortableGroup::MemberNotFound));

    /// Return the locations of the members in the given ObjectGroup.
    virtual PortableGroup::Locations * locations_of_members (
        PortableGroup::ObjectGroup_ptr object_group
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                      PortableGroup::ObjectGroupNotFound));

    /// Return the locations of the members in the given ObjectGroup.
    virtual PortableGroup::ObjectGroups * groups_at_location (
        const PortableGroup::Location & the_location
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    /// Return the ObjectGroupId for the given ObjectGroup.
    virtual PortableGroup::ObjectGroupId get_object_group_id (
        PortableGroup::ObjectGroup_ptr object_group
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                      PortableGroup::ObjectGroupNotFound));

    /// ?
    virtual PortableGroup::ObjectGroup_ptr get_object_group_ref (
        PortableGroup::ObjectGroup_ptr object_group
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                      PortableGroup::ObjectGroupNotFound));

  /**
   * TAO-specific extension.
   * Return the ObjectGroup reference for the given ObjectGroupId.
   */
   virtual PortableGroup::ObjectGroup_ptr get_object_group_ref_from_id (
        PortableGroup::ObjectGroupId group_id
        ACE_ENV_ARG_DECL
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
        , PortableGroup::ObjectGroupNotFound
      ));

    /// Return the reference corresponding to the Replica of a given
    /// ObjectGroup at the given location.
    virtual CORBA::Object_ptr get_member_ref (
        PortableGroup::ObjectGroup_ptr object_group,
        const PortableGroup::Location & loc
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                      PortableGroup::ObjectGroupNotFound,
                      PortableGroup::MemberNotFound));

    /// Sets the primary member of a group.
    virtual FT::ObjectGroup_ptr set_primary_member (
        FT::ObjectGroup_ptr object_group,
        const FT::Location & the_location
        ACE_ENV_ARG_DECL
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
        , PortableGroup::ObjectGroupNotFound
        , PortableGroup::MemberNotFound
        , FT::PrimaryNotSet
        , FT::BadReplicationStyle
      ));

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
          factory_creation_id
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                      PortableGroup::NoFactory,
                      PortableGroup::ObjectNotCreated,
                      PortableGroup::InvalidCriteria,
                      PortableGroup::InvalidProperty,
                      PortableGroup::CannotMeetCriteria));

    /**
    * Delete the object corresponding to the provided
    * FactoryCreationId.  If the object is actually an ObjectGroup,
    * then all members within the ObjectGroup will be deleted.
    * Afterward, the ObjectGroup itself will be deleted.
    */
    virtual void delete_object (
        const PortableGroup::GenericFactory::FactoryCreationId &
          factory_creation_id
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                      PortableGroup::ObjectNotFound));

    //@}

    ////////////////
    // Forbidden methods
    FT_ReplicationManager (const FT_ReplicationManager & rhs);
    FT_ReplicationManager & operator = (const FT_ReplicationManager & rhs);

    ///////////////
    // Data Members
  private:

    /**
    * Protect internal state.
    * Mutex should be locked by corba methods, or by
    * external (public) methods before calling implementation
    * methods.
    * Implementation methods should assume the mutex is
    * locked if necessary.
    */
    ACE_SYNCH_MUTEX internals_;
    typedef ACE_Guard<ACE_SYNCH_MUTEX> InternalGuard;

    /// The orb
    CORBA::ORB_var orb_;

    /// The POA.
    PortableServer::POA_var poa_;

    // The ORB's IORManipulation object
    TAO_IOP::TAO_IOR_Manipulation_var iorm_;

    /// A file to which the factory's IOR should be written.
    const char * ior_output_file_;

    /// A name to be used to register the factory with the name service.
    //@@ Are *all* of these needed?
    const char * ns_name_;
    CosNaming::NamingContext_var naming_context_;
    CosNaming::Name this_name_;

    /// Our object reference.
    FT::ReplicationManager_var replication_manager_ref_;

    /// A human-readable string to identify this Replication Manager.
    ACE_CString identity_;

    /// The ObjectGroupManager that implements the functionality
    /// necessary for application-controlled object group membership.
//    TAO::PG_ObjectGroupManager object_group_manager_;
    TAO_PG_ObjectGroupManager object_group_manager_;

    /// The PropertyManager that is reponsible for parsing all criteria,
    /// and keeping track of property-type_id associations.
    TAO_PG_PropertyManager property_manager_;

    /// The GenericFactory responsible for creating all object groups.
    TAO_PG_GenericFactory generic_factory_;


    /// A container for our object group information
    TAO::PG_Object_Group_Map object_group_map_;
  

    /// The fault notifier.
    FT::FaultNotifier_var fault_notifier_;
    const char * fault_notifier_ior_;

    /// The fault consumer.
    TAO::FT_FaultConsumer fault_consumer_;

    /// The factory registry
    TAO::PG_FactoryRegistry factory_registry_;

    /// Quit flag.
    int quit_;
  };

} // namespace TAO

#include /**/ "ace/post.h"

#endif  /* FT_REPLICATION_MANAGER_H_ */

