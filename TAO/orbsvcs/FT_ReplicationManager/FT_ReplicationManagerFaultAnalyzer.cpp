/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    FT_ReplicationManagerFaultAnalyzer.cpp
 *
 *  $Id$
 *
 *  This file is part of TAO's implementation of Fault Tolerant CORBA.
 *
 *  @author Steve Totten <totten_s@ociweb.com>
 */
//=============================================================================

#include "orbsvcs/FT_ReplicationManager/FT_ReplicationManagerFaultAnalyzer.h"
#include "orbsvcs/CosNotifyCommC.h"
#include "orbsvcs/FT_NotifierC.h"
#include "orbsvcs/FT_ReplicationManager/FT_ReplicationManager.h"
#include "orbsvcs/FT_ReplicationManager/FT_FaultEventDescriptor.h"
#include "orbsvcs/PortableGroup/PG_Property_Utils.h"
#include "orbsvcs/FaultTolerance/FT_IOGR_Property.h"
#include <tao/debug.h>

#define INTEGRATED_WITH_REPLICATION_MANAGER 1

ACE_RCSID (FT_ReplicationManagerFaultAnalyzer,
           FT_ReplicationManagerFaultAnalyzer,
           "$Id$")

/// Constructor.
TAO::FT_ReplicationManagerFaultAnalyzer::FT_ReplicationManagerFaultAnalyzer (
  const TAO::FT_ReplicationManager * replication_manager)
  : replication_manager_ (
      ACE_const_cast (TAO::FT_ReplicationManager *, replication_manager))
{
}

/// Destructor.
TAO::FT_ReplicationManagerFaultAnalyzer::~FT_ReplicationManagerFaultAnalyzer ()
{
}

// Validate the event to make sure it is one we can handle.
// If it is not an event we can handle, this function logs the error
// and returns -1.
int TAO::FT_ReplicationManagerFaultAnalyzer::validate_event_type (
  const CosNotification::StructuredEvent & event)
{
  // Delegate to base class.
  //@@ Visual C++ 6.0 won't compile this if I include the namespace name
  // on the base class.
  // return TAO::FT_DefaultFaultAnalyzer::validate_event_type (event);
  return FT_DefaultFaultAnalyzer::validate_event_type (event);
}

/// Analyze a fault event.
int TAO::FT_ReplicationManagerFaultAnalyzer::analyze_fault_event (
  const CosNotification::StructuredEvent & event)
{
  int result = 0;

  const CosNotification::FilterableEventBody & filterable =
    event.filterable_data;
  CORBA::ULong item_count = filterable.length ();
#if (TAO_DEBUG_LEVEL_NEEDED == 1)
  if (TAO_debug_level > 6)
#endif /* (TAO_DEBUG_LEVEL_NEEDED == 1) */
  {
    for (CORBA::ULong n_prop = 0; n_prop < item_count; ++n_prop)
    {
      ACE_DEBUG ((LM_DEBUG,
        ACE_TEXT(
          "TAO::FT_ReplicationManagerFaultAnalyzer::analyze_fault_event: "
          "Property Name: <%s>\n"),
        filterable[n_prop].name.in()
      ));
    }
  }

  // Populate a TAO::FT_FaultEventDescriptor structure from the
  // properties in the event.
  TAO::FT_FaultEventDescriptor fault_event_desc;

  // Extract the location.
  if (result == 0)
  {
    FT::Location* temp_loc;
    if ((filterable[1].value >>= temp_loc) == 0)
    {
      ACE_ERROR ((LM_ERROR,
        ACE_TEXT (
          "TAO::FT_ReplicationManagerFaultAnalyzer::analyze_fault_event: "
          "Could not extract Location value from fault event.\n")
      ));
      result = -1;
    }
    else
    {
      // Make a deep copy of the Location.
      FT::Location* copy_loc = 0;
      ACE_NEW_RETURN (copy_loc, FT::Location (*temp_loc), -1);
      fault_event_desc.location = copy_loc;
    }
  }

  // CORBA 3.0.2, section 23.4.5.1 states:
  //
  //   The fault detector may or may not set the TypeId and
  //   ObjectGroupId fields with the following interpretations:
  //   - Neither is set if all objects at the given location have failed.
  //   - TypeId is set and ObjectGroupId is not set if all objects at
  //     the given location with the given type have failed.
  //   - Both are set if the member with the given ObjectGroupId at the
  //     given location has failed.
  if (result == 0)
  {
    // All objects at location failed.
    if (item_count == 2)
    {
      fault_event_desc.all_at_location_failed = 1;
    }

    // All objects of type at location failed.
    if (item_count == 3)
    {
      fault_event_desc.all_of_type_at_location_failed = 1;
      fault_event_desc.type_id = this->extract_type_id (filterable[2].value);
    }

    // An object (replica) at a location failed.
    if (item_count == 4)
    {
      fault_event_desc.object_at_location_failed = 1;
      fault_event_desc.type_id = this->extract_type_id (filterable[2].value);
      fault_event_desc.object_group_id =
        this->extract_object_group_id (filterable[3].value);
    }
  }

  //TODO: Finish the implementation of analyze_fault_event() to:
  // - if the ReplicationStyle of the object group is FT::SEMI_ACTIVE,
  //   and if the failed replica was the primary replica, set a new
  //   primary replica
  // - if the number of replicas falls below MinimumNumberReplicas
  //   property and MembershipStyle of the object group is FT::MEMB_INF_CTRL,
  //   create and add a new member.

#if (INTEGRATED_WITH_REPLICATION_MANAGER == 1)
  // If a specific object at a location failed, we need to determine
  // if it was the primary replica.
  if ((result == 0) &&
      (fault_event_desc.object_at_location_failed == 1))
  {
    FT::ObjectGroup_var the_object_group = FT::ObjectGroup::_nil();
    ACE_TRY_NEW_ENV
    {
      //@@ Q: How do we get the ObjectGroup from the ObjectGroupId?
      //@@ A: Use TAO-specific extension to PortableGroup::ObjectGroupManager
      // interface (get_object_group_ref_from_id()).
      the_object_group =
        this->replication_manager_->get_object_group_ref_from_id (
          fault_event_desc.object_group_id
          ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
    ACE_CATCH (PortableGroup::ObjectGroupNotFound, ex)
    {
      ACE_PRINT_EXCEPTION (
        ex,
        ACE_TEXT (
          "TAO::FT_ReplicationManagerFaultAnalyzer::analyze_fault_event: ")
      );
      result = -1;
    }
    ACE_ENDTRY;

    if (CORBA::is_nil (the_object_group.in()))
    {
      ACE_ERROR ((LM_ERROR,
        ACE_TEXT (
          "TAO::FT_ReplicationManagerFaultAnalyzer::analyze_fault_event: "
          "Could not get ObjectGroup reference from ObjectGroupId: <%Q>.\n"),
          fault_event_desc.object_group_id
      ));
      result = -1;
    }

    //@@ Q: How do we get the properties of the ObjectGroup to
    // figure out the ReplicationStyle, MinimumNumberReplicas, etc.?
    //@@ A: FT::PropertyManager::get_properties().

    // Get the properties associated with this ObjectGroup.
    FT::Properties_var properties;
    if (result == 0)
    {
      properties = this->replication_manager_->get_properties (
        the_object_group.in()
        ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);

      // Get the MembershipStyle property.
      FT::MembershipStyleValue membership_style;
      result = this->get_membership_style (properties.in(), membership_style);
      if (result != 0)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
          ACE_TEXT (
            "TAO::FT_ReplicationManagerFaultAnalyzer::analyze_fault_event: "
            "Could not extract MembershipStyle from properties on "
            "ObjectGroup with id <%Q>.\n"),
          fault_event_desc.object_group_id),
          -1);
      }
      else
      {
        fault_event_desc.membership_style = membership_style;
#if (TAO_DEBUG_LEVEL_NEEDED == 1)
        if (TAO_debug_level > 6)
#endif /* (TAO_DEBUG_LEVEL_NEEDED == 1) */
        {
          ACE_DEBUG ((LM_DEBUG,
            ACE_TEXT (
              "TAO::FT_ReplicationManagerFaultAnalyzer::validate_event_type: "
              "MembershipStyleValue = <%d>"),
              fault_event_desc.membership_style
          ));
        }
      }

      // Get the ReplicationStyle property.
      FT::ReplicationStyleValue replication_style;
      result = this->get_replication_style (properties.in(), replication_style);
      if (result != 0)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
          ACE_TEXT (
            "TAO::FT_ReplicationManagerFaultAnalyzer::analyze_fault_event: "
            "Could not extract ReplicationStyle from properties on "
            "ObjectGroup with id <%Q>.\n"),
          fault_event_desc.object_group_id),
          -1);
      }
      else
      {
        fault_event_desc.replication_style = replication_style;
#if (TAO_DEBUG_LEVEL_NEEDED == 1)
        if (TAO_debug_level > 6)
#endif /* (TAO_DEBUG_LEVEL_NEEDED == 1) */
        {
          ACE_DEBUG ((LM_DEBUG,
            ACE_TEXT (
              "TAO::FT_ReplicationManagerFaultAnalyzer::validate_event_type: "
              "ReplicationStyleValue = <%d>"),
              fault_event_desc.replication_style
          ));
        }
      }

      // Get the MinimumNumberReplicas property.
      FT::MinimumNumberReplicasValue minimum_number_replicas;
      result = this->get_minimum_number_replicas (
        properties.in(), minimum_number_replicas);
      if (result != 0)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
          ACE_TEXT (
            "TAO::FT_ReplicationManagerFaultAnalyzer::analyze_fault_event: "
            "Could not extract MinimumNumberReplicas from properties on "
            "ObjectGroup with id <%Q>.\n"),
          fault_event_desc.object_group_id),
          -1);
      }
      else
      {
        fault_event_desc.minimum_number_replicas = minimum_number_replicas;
#if (TAO_DEBUG_LEVEL_NEEDED == 1)
        if (TAO_debug_level > 6)
#endif /* (TAO_DEBUG_LEVEL_NEEDED == 1) */
        {
          ACE_DEBUG ((LM_DEBUG,
            ACE_TEXT (
              "TAO::FT_ReplicationManagerFaultAnalyzer::validate_event_type: "
              "MinimumNumberReplicas = <%d>"),
              fault_event_desc.minimum_number_replicas
          ));
        }
      }

      // Get the InitialNumberReplicas property.
      FT::InitialNumberReplicasValue initial_number_replicas;
      result = this->get_initial_number_replicas (
        properties.in(), initial_number_replicas);
      if (result != 0)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
          ACE_TEXT (
            "TAO::FT_ReplicationManagerFaultAnalyzer::analyze_fault_event: "
            "Could not extract InitialNumberReplicas from properties on "
            "ObjectGroup with id <%Q>.\n"),
          fault_event_desc.object_group_id),
          -1);
      }
      else
      {
        fault_event_desc.initial_number_replicas = initial_number_replicas;
#if (TAO_DEBUG_LEVEL_NEEDED == 1)
        if (TAO_debug_level > 6)
#endif /* (TAO_DEBUG_LEVEL_NEEDED == 1) */
        {
          ACE_DEBUG ((LM_DEBUG,
            ACE_TEXT (
              "TAO::FT_ReplicationManagerFaultAnalyzer::validate_event_type: "
              "InitialNumberReplicas = <%d>"),
              fault_event_desc.initial_number_replicas
          ));
        }
      }
    }

    // If the ReplicationStyle is COLD_PASSIVE, WARM_PASSIVE, or
    // SEMI_ACTIVE, we can see if it was the primary replica that
    // failed.
    if ((result == 0) &&
        (fault_event_desc.replication_style == FT::COLD_PASSIVE ||
         fault_event_desc.replication_style == FT::WARM_PASSIVE ||
         fault_event_desc.replication_style == FT::SEMI_ACTIVE))
    {
      result = this->is_primary_member (
        the_object_group.in(),
        fault_event_desc.location,
        fault_event_desc.object_is_primary);
    }

  }
#endif /* (INTEGRATED_WITH_REPLICATION_MANAGER == 1) */

  // Debugging support.
#if (TAO_DEBUG_LEVEL_NEEDED == 1)
  if (TAO_debug_level > 6)
#endif /* (TAO_DEBUG_LEVEL_NEEDED == 1) */
  {
    ACE_DEBUG ((LM_DEBUG,
      ACE_TEXT("Getting the location as a string.\n")
    ));

    // Get the location as a string.
    ACE_CString loc_as_string;
    FT::Location_var & loc = fault_event_desc.location;

    for (CORBA::ULong li = 0; li < loc->length(); ++li)
    {
      if (li > 0) loc_as_string += "/";
      // Assume only the "id" field of the CosNaming::Name is used.
      loc_as_string += CORBA::string_dup (loc[li].id);
    }

    if (fault_event_desc.all_at_location_failed == 1)
    {
      ACE_DEBUG ((LM_DEBUG,
        ACE_TEXT (
          "All objects at location <%s> failed.\n"),
          loc_as_string.c_str()
      ));
    }

    if (fault_event_desc.all_of_type_at_location_failed == 1)
    {
      ACE_DEBUG ((LM_DEBUG,
        ACE_TEXT (
          "All objects of type <%s> at location <%s> failed.\n"),
          fault_event_desc.type_id.in(),
          loc_as_string.c_str()
      ));
    }

    if (fault_event_desc.object_at_location_failed == 1)
    {
      ACE_DEBUG ((LM_DEBUG,
        ACE_TEXT (
          "Replica of type <%s> with ObjectGroupId <%Q> "
          "at location <%s> failed.\n"),
          fault_event_desc.type_id.in(),
          fault_event_desc.object_group_id,
          loc_as_string.c_str()
      ));
    }
  }

  return result;
}

// Extract a string type_id from CORBA::Any.
// Caller owns the returned string.
char*
TAO::FT_ReplicationManagerFaultAnalyzer::extract_type_id (const CORBA::Any& val)
{
  const char* type_id_value;
  if ((val >>= type_id_value) == 0)
  {
    ACE_ERROR ((LM_ERROR,
      ACE_TEXT (
        "TAO::FT_ReplicationManagerFaultAnalyzer::extract_type_id: "
        "Could not extract TypeId value from any.\n")
    ));
    return 0;
  }

  return CORBA::string_dup (type_id_value);
}

// Extract the ObjectGroupId from CORBA::Any.
FT::ObjectGroupId
TAO::FT_ReplicationManagerFaultAnalyzer::extract_object_group_id (const CORBA::Any& val)
{
  FT::ObjectGroupId id = (FT::ObjectGroupId)0;
  if ((val >>= id) == 0)
  {
    ACE_ERROR ((LM_ERROR,
      ACE_TEXT (
        "TAO::FT_ReplicationManagerFaultAnalyzer::extract_object_group_id: "
        "Could not extract ObjectGroupId value from any.\n")
    ));
  }

  return id;
}

//
//TODO: Use TAO_PG::find() to get property values from properties
// instead of all these specific "get" functions.
//

// Get the MembershipStyle property.
int TAO::FT_ReplicationManagerFaultAnalyzer::get_membership_style (
  const FT::Properties & properties,
  FT::MembershipStyleValue & membership_style)
{
  FT::Name prop_name (1);
  prop_name.length (1);
  prop_name[0].id = CORBA::string_dup (FT::FT_MEMBERSHIP_STYLE);
  int result = 0;

  FT::Value value;
  if (TAO_PG::get_property_value (prop_name, properties, value)
    && ((value >>= membership_style) == 1))
  {
#if (TAO_DEBUG_LEVEL_NEEDED == 1)
    if (TAO_debug_level > 6)
#endif /* (TAO_DEBUG_LEVEL_NEEDED == 1) */
    {
      ACE_DEBUG ((LM_DEBUG,
        ACE_TEXT (
          "TAO::FT_ReplicationManagerFaultAnalyzer::analyze_fault_event: "
          "MembershipStyle is <%d>:\n"),
        membership_style
      ));
    }
  }
  else
  {
    result = -1;
  }

  return result;
}

int TAO::FT_ReplicationManagerFaultAnalyzer::get_replication_style (
  const FT::Properties & properties,
  FT::ReplicationStyleValue & replication_style)
{
  FT::Name prop_name (1);
  prop_name.length (1);
  prop_name[0].id = CORBA::string_dup (FT::FT_REPLICATION_STYLE);
  int result = 0;

  FT::Value value;
  if (TAO_PG::get_property_value (prop_name, properties, value)
    && ((value >>= replication_style) == 1))
  {
#if (TAO_DEBUG_LEVEL_NEEDED == 1)
    if (TAO_debug_level > 6)
#endif /* (TAO_DEBUG_LEVEL_NEEDED == 1) */
    {
      ACE_DEBUG ((LM_DEBUG,
        ACE_TEXT (
          "TAO::FT_ReplicationManagerFaultAnalyzer::analyze_fault_event: "
          "ReplicationStyle is <%d>:\n"),
        replication_style
      ));
    }
  }
  else
  {
    result = -1;
  }

  return result;
}

int TAO::FT_ReplicationManagerFaultAnalyzer::get_minimum_number_replicas (
  const FT::Properties & properties,
  FT::MinimumNumberReplicasValue & minimum_number_replicas)
{
  FT::Name prop_name (1);
  prop_name.length (1);
  prop_name[0].id = CORBA::string_dup (FT::FT_MINIMUM_NUMBER_REPLICAS);
  int result = 0;

  FT::Value value;
  if (TAO_PG::get_property_value (prop_name, properties, value)
    && ((value >>= minimum_number_replicas) == 1))
  {
#if (TAO_DEBUG_LEVEL_NEEDED == 1)
    if (TAO_debug_level > 6)
#endif /* (TAO_DEBUG_LEVEL_NEEDED == 1) */
    {
      ACE_DEBUG ((LM_DEBUG,
        ACE_TEXT (
          "TAO::FT_ReplicationManagerFaultAnalyzer::analyze_fault_event: "
          "MinimumNumberReplicas is <%d>:\n"),
        minimum_number_replicas
      ));
    }
  }
  else
  {
    result = -1;
  }

  return result;
}

int TAO::FT_ReplicationManagerFaultAnalyzer::get_initial_number_replicas (
  const FT::Properties & properties,
  FT::InitialNumberReplicasValue & initial_number_replicas)
{
  FT::Name prop_name (1);
  prop_name.length (1);
  prop_name[0].id = CORBA::string_dup (FT::FT_INITIAL_NUMBER_REPLICAS);
  int result = 0;

  FT::Value value;
  if (TAO_PG::get_property_value (prop_name, properties, value)
    && ((value >>= initial_number_replicas) == 1))
  {
#if (TAO_DEBUG_LEVEL_NEEDED == 1)
    if (TAO_debug_level > 6)
#endif /* (TAO_DEBUG_LEVEL_NEEDED == 1) */
    {
      ACE_DEBUG ((LM_DEBUG,
        ACE_TEXT (
          "TAO::FT_ReplicationManagerFaultAnalyzer::analyze_fault_event: "
          "InitialNumberReplicas is <%d>:\n"),
        initial_number_replicas
      ));
    }
  }
  else
  {
    result = -1;
  }

  return result;
}

int TAO::FT_ReplicationManagerFaultAnalyzer::is_primary_member (
  const FT::ObjectGroup_ptr iogr,
  const FT::Location_var & location,
  int & object_is_primary)
{

  //@@ Q: How do we determine if this was a primary that faulted?
  //@@ A: Get the TagFTGroupTaggedComponent from the IOGR and search
  // for the primary, using the TAO_FT_IOGR_Property helper class.
  // Then, compare the TypeId and Location of the failed object with
  // those of the primary.  If they match, it was a primary fault.

  int result = 0;
  object_is_primary = 0;

  ACE_TRY_NEW_ENV
  {
    // Create an "empty" TAO_FT_IOGR_Property and use it to get the
    // tagged component.
    TAO_FT_IOGR_Property temp_ft_prop;
    FT::TagFTGroupTaggedComponent ft_group_tagged_component;
    CORBA::Boolean got_tagged_component =
      temp_ft_prop.get_tagged_component (
          iogr, ft_group_tagged_component ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
    if (got_tagged_component)
    {
      // Create a new TAO_FT_IOGR_Property with the tagged
      // component.
      TAO_FT_IOGR_Property ft_prop (ft_group_tagged_component);

      // Check to see if a primary is set.
      CORBA::Boolean primary_is_set = ft_prop.is_primary_set (
        iogr ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      if (primary_is_set)
      {
        // Get the primary object.
        CORBA::Object_var primary_obj = ft_prop.get_primary (
          iogr ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;
        if (CORBA::is_nil (primary_obj.in()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
            ACE_TEXT (
              "TAO::FT_ReplicationManagerFaultAnalyzer::is_primary_member: "
              "Could not get primary IOR from IOGR.\n")),
            -1);
        }

        // Get the object reference of the failed member.
        CORBA::Object_var failed_obj =
          this->replication_manager_->get_member_ref (
            iogr, location.in() ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;
        if (CORBA::is_nil (failed_obj.in()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
            ACE_TEXT (
              "TAO::FT_ReplicationManagerFaultAnalyzer::is_primary_member: "
              "Could not get IOR of failed member from IOGR.\n")),
            -1);
        }

        // Are the two object refs (primary and failed) equivalent?
        CORBA::Boolean equiv = primary_obj->_is_equivalent (
          failed_obj.in() ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;
        if (equiv)
        {
          object_is_primary = 1;
          result = 0;
        }
      }
      else  // primary is not set
      {
        ACE_ERROR ((LM_ERROR,
          ACE_TEXT (
            "TAO::FT_ReplicationManagerFaultAnalyzer::is_primary_member: "
            "Primary is not set on IOGR.\n")
        ));
        result = -1;
      }
    }
    else // could not get tagged component
    {
      ACE_ERROR ((LM_ERROR,
        ACE_TEXT (
          "TAO::FT_ReplicationManagerFaultAnalyzer::is_primary_member: "
          "Could not get tagged component from IOGR.\n")
      ));
      result = -1;
    }
  }
  ACE_CATCHANY
  {
    ACE_PRINT_EXCEPTION (
      ACE_ANY_EXCEPTION,
      ACE_TEXT (
        "TAO::FT_ReplicationManagerFaultAnalyzer::is_primary_member: ")
    );
    result = -1;
  }
  ACE_ENDTRY;

  return result;
}
