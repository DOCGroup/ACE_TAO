/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    FT_ReplicationManagerFaultAnalyzer.h
 *
 *  $Id$
 *
 *  This file is part of TAO's implementation of Fault Tolerant CORBA.
 *  This is the Replication Manager's implementation of a fault analyzer.
 *
 *  @author Steve Totten <totten_s@ociweb.com>
 */
//=============================================================================


#ifndef FT_REPLICATION_MANAGER_FAULT_ANALYZER_H_
#define FT_REPLICATION_MANAGER_FAULT_ANALYZER_H_

#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/FT_ReplicationManager/FT_DefaultFaultAnalyzer.h"
#include "orbsvcs/FT_CORBAC.h"

// Forward declarations.
template <class T> class ACE_Unbounded_Set;
template <class T> class ACE_Unbounded_Set_Iterator;

namespace TAO
{

  ///////////////////////
  // Forward declarations
  class FT_ReplicationManager;
  struct FT_FaultEventDescriptor;

  typedef ACE_Unbounded_Set<FT::Location> FT_Location_Set;

  /**
  * Replication Manager's fault analyzer.
  *
  */
  class FT_ReplicationManagerFaultAnalyzer
    : public TAO::FT_DefaultFaultAnalyzer
  {

  public:
    /**
    * Constructor.
    * @param replication_manager Pointer to TAO::FT_ReplicationManager.
    */
    FT_ReplicationManagerFaultAnalyzer (
      const TAO::FT_ReplicationManager * replication_manager);

    /**
    * Destructor.
    */
    virtual ~FT_ReplicationManagerFaultAnalyzer ();

  public:

    /**
    * Validate event type to make sure it is one we can handle.
    * @param event The structured fault event, as from the Fault Notifier.
    * @return 0 if it is a valid event type, -1 otherwise.
    */
    virtual int validate_event_type (
      const CosNotification::StructuredEvent & event);

    /**
    * Analyze a fault event.
    * @param event The structured fault event, as from the Fault Notifier.
    * @return 0 on success, -1 on failure.
    */
    virtual int analyze_fault_event (
      const CosNotification::StructuredEvent & event);

    ////////////////////
    // Forbidden methods
  private:
    /// Default constructor.
    FT_ReplicationManagerFaultAnalyzer ();

    /// Copy constructor.
    FT_ReplicationManagerFaultAnalyzer (
      const FT_ReplicationManagerFaultAnalyzer & rhs);

    /// Assignment operator.
    FT_ReplicationManagerFaultAnalyzer & operator = (
      const FT_ReplicationManagerFaultAnalyzer & rhs);

    /////////////////////////
    // Implementation methods
  protected:

    /// Helper functions for fault analysis.

    // Extract the type id from a CORBA any.
    int get_type_id (const CORBA::Any& val, FT::TypeId_out type_id);

    // Extract the ObjectGroupId from a CORBA any.
    int get_object_group_id (const CORBA::Any& val, FT::ObjectGroupId& id);

    // Extract the FT::Location from a CORBA any.
    int get_location (const CORBA::Any& val, FT::Location_out location);

    // Get the MembershipStyle property.
    int get_membership_style (
      const FT::Properties & properties,
      FT::MembershipStyleValue & membership_style);

    // Get the ReplicationStyle property.
    int get_replication_style (
      const FT::Properties & properties,
      FT::ReplicationStyleValue & replication_style);

    // Get the MinimumNumberMembers property.
    int get_minimum_number_members (
      const FT::Properties & properties,
      FT::MinimumNumberMembersValue & minimum_number_members);

    // Get the InitialNumberMembers property.
    int get_initial_number_members (
      const FT::Properties & properties,
      FT::InitialNumberMembersValue & initial_number_members);

    // Get the Factories property.
    int get_factories (
      const FT::Properties & properties,
      FT::FactoryInfos_out factories);

    // Handle a single replica failure.
    int single_replica_failure (
      TAO::FT_FaultEventDescriptor & fault_event_desc);

    // Handle a location failure.
    int location_failure (
      TAO::FT_FaultEventDescriptor & fault_event_desc);

    // Handle a type at location failure.
    int type_failure (
      TAO::FT_FaultEventDescriptor & fault_event_desc);

    // Is the replica at location the primary member of its ObjectGroup?
    // Sets <is_primary> and returns 0 on success.
    // Returns -1 on failure.
    int is_primary_member (
      const FT::ObjectGroup_ptr iogr,
      const FT::Location & location,
      int & object_is_primary);

    // Choose a new primary member for the ObjectGroup.
    // Sets <new_iogr> and returns 0 on success.
    // Returns -1 on failure.
    int set_new_primary (
      FT::ObjectGroup_ptr iogr,
      TAO::FT_FaultEventDescriptor & fault_event_desc,
      FT::ObjectGroup_out new_iogr);

    // While the number of members in the object group is less than
    // the MinimumNumberMembers property, add new members.
    // Sets <new_iogr> and returns 0 on success.
    // Returns -1 on failure.
    int add_members (
      FT::ObjectGroup_ptr iogr,
      TAO::FT_FaultEventDescriptor & fault_event_desc,
      FT::ObjectGroup_out new_iogr);


    ///////////////
    // Data Members
  private:
    FT_ReplicationManager * replication_manager_;

  };

} // namespace TAO

#include /**/ "ace/post.h"

#endif  /* FT_REPLICATION_MANAGER_FAULT_ANALYZER_H_ */

