/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    FT_ReplicationManagerFaultAnalyzer.h
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
#include <ace/ACE.h>

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/FT_ReplicationManager/FT_DefaultFaultAnalyzer.h"
#include "orbsvcs/FT_CORBAC.h"

// Forward declarations.
ACE_BEGIN_VERSIONED_NAMESPACE_DECL
template <class T> class ACE_Unbounded_Set;
template <class T> class ACE_Unbounded_Set_Iterator;
ACE_END_VERSIONED_NAMESPACE_DECL

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  ///////////////////////
  // Forward declarations
  class FT_ReplicationManager;
  struct FT_FaultEventDescriptor;

  using FT_Location_Set = ACE_Unbounded_Set<PortableGroup::Location>;

  /**
  * Replication Manager's fault analyzer.
  *
  */
  class FT_ReplicationManagerFaultAnalyzer
    : public ::TAO::FT_DefaultFaultAnalyzer
  {
  public:
    /**
    * Constructor.
    * @param replication_manager Pointer to TAO::FT_ReplicationManager.
    */
    explicit FT_ReplicationManagerFaultAnalyzer (
      TAO::FT_ReplicationManager const * replication_manager);

    /**
    * Destructor.
    */
    ~FT_ReplicationManagerFaultAnalyzer () override;

  public:
    /**
    * Validate event type to make sure it is one we can handle.
    * @param event The structured fault event, as from the Fault Notifier.
    * @return 0 if it is a valid event type, -1 otherwise.
    */
    int validate_event_type (
      CosNotification::StructuredEvent const & event) override;

    /**
    * Analyze a fault event.
    * @param event The structured fault event, as from the Fault Notifier.
    * @return 0 on success, -1 on failure.
    */
    int analyze_fault_event (
      CosNotification::StructuredEvent const & event) override;

    ////////////////////
    // Forbidden methods
  private:
    /// Default constructor.
    FT_ReplicationManagerFaultAnalyzer () = delete;

    /// Copy constructor.
    FT_ReplicationManagerFaultAnalyzer (
      FT_ReplicationManagerFaultAnalyzer const &) = delete;

    /// Assignment operator.
    FT_ReplicationManagerFaultAnalyzer (FT_ReplicationManagerFaultAnalyzer &&) = delete;
    FT_ReplicationManagerFaultAnalyzer & operator= (
      FT_ReplicationManagerFaultAnalyzer const &) = delete;
    FT_ReplicationManagerFaultAnalyzer & operator= (
      FT_ReplicationManagerFaultAnalyzer &&) = delete;

    /////////////////////////
    // Implementation methods
  protected:
    /// Helper functions for fault analysis.

    // Extract the type id from a CORBA any.
    int get_type_id (CORBA::Any const &val, PortableGroup::TypeId_out type_id);

    // Extract the ObjectGroupId from a CORBA any.
    int get_object_group_id (CORBA::Any const &val, PortableGroup::ObjectGroupId &id);

    // Extract the PortableGroup::Location from a CORBA any.
    int get_location (CORBA::Any const &val, PortableGroup::Location_out location);

    // Get the MembershipStyle property.
    int get_membership_style (
      PortableGroup::Properties const & properties,
      PortableGroup::MembershipStyleValue & membership_style);

    // Get the ReplicationStyle property.
    int get_replication_style (
      PortableGroup::Properties const & properties,
      FT::ReplicationStyleValue & replication_style);

    // Get the MinimumNumberMembers property.
    int get_minimum_number_members (
      PortableGroup::Properties const & properties,
      PortableGroup::MinimumNumberMembersValue & minimum_number_members);

    // Get the InitialNumberMembers property.
    int get_initial_number_members (
      PortableGroup::Properties const & properties,
      PortableGroup::InitialNumberMembersValue & initial_number_members);

    // Get the Factories property.
    int get_factories (
      PortableGroup::Properties const & properties,
      PortableGroup::FactoryInfos_out factories);

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
    // Sets <object_is_primary> and returns 0 on success.
    // Returns -1 on failure.
    int is_primary_member (
      PortableGroup::ObjectGroup_ptr const iogr,
      PortableGroup::Location const & location,
      bool & object_is_primary);

    /// remove a failed member from a group
    int remove_failed_member (
      PortableGroup::ObjectGroup_ptr iogr,
      TAO::FT_FaultEventDescriptor & fault_event_desc,
      PortableGroup::ObjectGroup_out new_iogr);


    // Choose a new primary member for the ObjectGroup.
    // Sets <new_iogr> and returns 0 on success.
    // Returns -1 on failure.
    int set_new_primary (
      PortableGroup::ObjectGroup_ptr iogr,
      TAO::FT_FaultEventDescriptor & fault_event_desc,
      PortableGroup::ObjectGroup_out new_iogr);

#if 0 // this is handled by the remove_member method
    // While the number of members in the object group is less than
    // the MinimumNumberMembers property, add new members.
    // Sets <new_iogr> and returns 0 on success.
    // Returns -1 on failure.
    int add_members (
      PortableGroup::ObjectGroup_ptr iogr,
      TAO::FT_FaultEventDescriptor & fault_event_desc,
      PortableGroup::ObjectGroup_out new_iogr);
#endif

    ///////////////
    // Data Members
  private:
    FT_ReplicationManager * replication_manager_;
  };
} // namespace TAO

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif  /* FT_REPLICATION_MANAGER_FAULT_ANALYZER_H_ */

