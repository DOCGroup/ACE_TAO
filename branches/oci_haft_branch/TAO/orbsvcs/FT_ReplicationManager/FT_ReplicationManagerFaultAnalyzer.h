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

namespace TAO
{

  ///////////////////////
  // Forward declarations
  class FT_ReplicationManager;

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
    char* extract_type_id (const CORBA::Any& val);
    FT::ObjectGroupId extract_object_group_id (const CORBA::Any& val);

    int get_membership_style (
      const FT::Properties & properties,
      FT::MembershipStyleValue & membership_style);

    int get_replication_style (
      const FT::Properties & properties,
      FT::ReplicationStyleValue & replication_style);

    int get_minimum_number_replicas (
      const FT::Properties & properties,
      FT::MinimumNumberReplicasValue & minimum_number_replicas);

    int get_initial_number_replicas (
      const FT::Properties & properties,
      FT::InitialNumberReplicasValue & initial_number_replicas);

    ///////////////
    // Data Members
  private:
    FT_ReplicationManager * replication_manager_;

  };

} // namespace TAO

#include /**/ "ace/post.h"

#endif  /* FT_REPLICATION_MANAGER_FAULT_ANALYZER_H_ */

